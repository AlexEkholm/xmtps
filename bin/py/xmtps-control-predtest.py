#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Real-data test of the neutral-pressure estimate: fit y = w|w| against p on the descent between
the fall pump and a cut-off (fraction of the way to the park level), extrapolate to y = 0 and
compare with the park level reached (no seek pump in between)."""
import json, os, sys, numpy as np
from multiprocessing import Pool
ROOT = '/data1/alamo/data/json'      # --json-dir
lv = {}


def _init(root, fleet):
    global ROOT, lv
    ROOT = root
    lv = {}
    for r in json.load(open(fleet)):
        if r.get('level') is not None and np.isfinite(r['level']):
            lv[(r['sn'], r['dive'])] = r

def one(sn):
    out = []
    f = f'{ROOT}/{sn}/{sn}_descent.json'
    if not os.path.exists(f): return out
    d = json.load(open(f)); c = d['columns']
    ci = {k: c.index(k) for k in ('dive', 'pres', 'dt_epoch', 'phase_code')}
    byd = {}
    for r in d['data']:
        if r[ci['dive']] is None or r[ci['dive']] < 1 or r[ci['pres']] is None or r[ci['dt_epoch']] is None: continue
        byd.setdefault(r[ci['dive']], []).append((r[ci['dt_epoch']], r[ci['pres']], r[ci['phase_code']]))
    for dv, rows in byd.items():
        L = lv.get((sn, dv))
        if not L: continue
        level = L['level']
        rows.sort()
        t = np.array([x[0] for x in rows], float); p = np.array([x[1] for x in rows], float)
        ph = np.array([x[2] for x in rows])
        k2 = np.where(ph == 2)[0]
        if not len(k2): continue
        start = p[k2[0]] + 30
        for frac in (0.5, 0.7, 0.85):
            cut = start + frac * (level - start)
            m = (p > start) & (p < cut)
            if m.sum() < 8: continue
            tt, pp = t[m], p[m]
            w = -np.diff(pp) / np.diff(tt); pm = 0.5 * (pp[1:] + pp[:-1])
            ok = np.diff(tt) > 20
            if ok.sum() < 6: continue
            y = w[ok] * np.abs(w[ok]); cf = np.polyfit(pm[ok], y, 1)
            if cf[0] <= 0: pn = np.nan
            else: pn = -cf[1] / cf[0]
            out.append(dict(sn=sn, dive=dv, type='ALTO' if sn >= 10000 else 'ALAMO', frac=frac,
                            level=level, start=start, pn=pn, err=pn - level, n=int(ok.sum())))
    return out

if __name__ == '__main__':
    import argparse
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('fleet', help='output of xmtps-control-fleet.py')
    ap.add_argument('--json-dir', default=ROOT)
    ap.add_argument('-o', '--out', default='predtest.json')
    args = ap.parse_args()
    _init(args.json_dir, args.fleet)
    sns = sorted(set(k[0] for k in lv))
    with Pool(16, initializer=_init, initargs=(args.json_dir, args.fleet)) as pool:
        res = pool.map(one, sns)
    rows = [r for rr in res for r in rr]
    json.dump(rows, open(args.out, 'w'))
    import pandas as pd
    df = pd.DataFrame(rows)
    df['ok'] = df.pn.notna()
    for (t, fr), g in df.groupby(['type', 'frac']):
        e = g.err.dropna().abs(); dist = (g.level - g.start)
        print(t, fr, 'n', len(g), 'valid %.2f' % g.ok.mean(), '|err| med %.0f p75 %.0f p90 %.0f' % (e.median(), e.quantile(.75), e.quantile(.9)),
              'within25 %.2f within50 %.2f' % ((e <= 25).mean(), (e <= 50).mean()), 'remaining dist med %.0f' % ((1 - fr) * dist).median())
    d1 = df[df.dive == 1]
    for (t, fr), g in d1.groupby(['type', 'frac']):
        e = g.err.dropna().abs(); print(' dive1', t, fr, len(g), '|err| med %.0f' % e.median() if len(e) else '')
