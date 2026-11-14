#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""How well the vertical rate predicts a float's neutral pressure, tested on real descents.

For every descent with no seek pump between the fall pump and the park level, and at several
distances D above the park level, the float's state is estimated from the samples before that
point only:

    w     mean rate over a window of the last `win` seconds (straight-line fit of p against t)
    s     slope of y = w|w| against p over the last `span` dBar of the descent (several windows)
    p_n   = p - y / s, with y = w|w|

and compared with the park level the float then reached (median of the middle half of the
drift). Reports the error by remaining distance, window length and float type, and the error
the formula expects from rate noise: dp_n = 2 |w| dw / s.

    xmtps-control-neutral.py fleet.json --json-dir /data1/alamo/data/json -o neutral.json
"""
import argparse
import json
import os
from multiprocessing import Pool

import numpy as np

ROOT = '/data1/alamo/data/json'
LEVELS = {}
DISTS = (400, 200, 100, 50, 25)
WINS = (600, 1200, 2400)
SPAN = 300.0


def _init(root, fleet):
    global ROOT, LEVELS
    ROOT = root
    LEVELS = {}
    for r in json.load(open(fleet)):
        if r.get('level') is not None and np.isfinite(r['level']) and r.get('n_seek_pumps') == 0:
            LEVELS[(r['sn'], r['dive'])] = r['level']


def window_rate(t, p, t_end, win):
    m = (t > t_end - win) & (t <= t_end)
    if m.sum() < 3 or t[m][-1] - t[m][0] < 0.5 * win:
        return None
    c = np.polyfit(t[m] - t_end, p[m], 1)
    return -c[0], float(np.mean(p[m]))


def one(sn):
    out = []
    f = '{}/{}/{}_descent.json'.format(ROOT, sn, sn)
    if not os.path.exists(f):
        return out
    try:
        d = json.load(open(f))
    except Exception:
        return out
    c = d['columns']
    i_d, i_p, i_t, i_ph = (c.index(k) for k in ('dive', 'pres', 'dt_epoch', 'phase_code'))
    byd = {}
    for r in d['data']:
        if r[i_d] is None or r[i_d] < 1 or r[i_p] is None or r[i_t] is None:
            continue
        byd.setdefault(r[i_d], []).append((r[i_t], r[i_p], r[i_ph]))
    for dv, rows in byd.items():
        level = LEVELS.get((sn, dv))
        if level is None:
            continue
        rows.sort()
        t = np.array([x[0] for x in rows], float)
        p = np.array([x[1] for x in rows], float)
        ph = np.array([x[2] for x in rows])
        k2 = np.where(ph == 2)[0]
        if not len(k2):
            continue
        t0 = t[k2[0]] + 120                                    # after the fall pump
        keep = t > t0
        t, p = t[keep], p[keep]
        if len(t) < 6:
            continue
        for D in DISTS:
            cut = level - D
            idx = np.where(p >= cut)[0]
            if not len(idx) or p[0] > cut - 50:
                continue
            t_end = t[idx[0]]
            for win in WINS:
                wr = window_rate(t, p, t_end, win)
                if wr is None:
                    continue
                w, pm = wr
                # slope s from window means over the last SPAN dBar
                means = []
                te = t_end
                while True:
                    r2 = window_rate(t, p, te, win)
                    if r2 is None or pm - r2[1] > SPAN:
                        break
                    means.append((r2[1], r2[0] * abs(r2[0])))     # y = w|w|, w up-positive
                    te -= win / 2
                if len(means) < 3:
                    continue
                M = np.array(means)
                s = np.polyfit(M[:, 0], M[:, 1], 1)[0]
                if s <= 0:
                    out.append(dict(sn=sn, dive=dv, D=D, win=win, ok=False))
                    continue
                y = w * abs(w)                                 # w is positive up (sinking: y < 0)
                pn = pm - y / s
                out.append(dict(sn=sn, dive=dv, type='ALTO' if sn >= 10000 else 'ALAMO', D=D,
                                win=win, ok=True, w=float(w), s=float(s), pn=float(pn),
                                level=float(level), err=float(pn - level)))
    return out


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('fleet', help='output of xmtps-control-fleet.py')
    ap.add_argument('--json-dir', default=ROOT)
    ap.add_argument('-o', '--out', default='neutral.json')
    ap.add_argument('-j', '--jobs', type=int, default=8)
    args = ap.parse_args()
    _init(args.json_dir, args.fleet)
    sns = sorted(set(k[0] for k in LEVELS))
    with Pool(args.jobs, initializer=_init, initargs=(args.json_dir, args.fleet)) as pool:
        res = pool.map(one, sns, chunksize=2)
    rows = [r for rr in res for r in rr]
    json.dump(rows, open(args.out, 'w'))
    ok = [r for r in rows if r['ok']]
    print(len(rows), 'estimates,', len(ok), 'with a positive slope')
    for typ in ('ALTO', 'ALAMO'):
        for D in DISTS:
            for win in WINS:
                e = np.array([r['err'] for r in ok if r['type'] == typ and r['D'] == D and r['win'] == win])
                n_all = sum(1 for r in rows if r.get('type', typ) == typ and r['D'] == D and r['win'] == win)
                if len(e) < 20:
                    continue
                w = np.array([abs(r['w']) for r in ok if r['type'] == typ and r['D'] == D and r['win'] == win])
                print('%-5s D %3d win %4d  n %5d  bias %+5.0f  |err| med %4.0f p75 %4.0f  within 25 %.2f 50 %.2f  |w| %.1f cm/s'
                      % (typ, D, win, len(e), np.median(e), np.median(abs(e)), np.percentile(abs(e), 75),
                         np.mean(abs(e) <= 25), np.mean(abs(e) <= 50), np.median(w) * 100))
