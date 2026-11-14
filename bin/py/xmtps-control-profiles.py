#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Picks a representative sample of real water columns and targets for the seek simulations.

For every float with a mission configuration and CTD profiles, takes the deepest of its last ten
profiles that reaches at least 300 dBar below the float's park target, with the target (DTD0)
and fall-pump start pressure (DSD0) of that dive. Profiles are averaged onto a 10 dBar grid
(in-situ density and temperature). A seeded random sample of ALTOs and ALAMOs is written out.

    xmtps-control-profiles.py --json-dir /data1/alamo/data/json -o profiles.json
"""
import argparse
import json
import os

import numpy as np

GRID = 10.0          # dBar
BELOW = 300.0        # dBar of profile needed below the target


def load(root, sn, kind):
    f = '{}/{}/{}_{}.json'.format(root, sn, sn, kind)
    if not os.path.exists(f):
        return None
    try:
        m = json.load(open(f))
    except Exception:
        return None
    if not isinstance(m, dict) or not m.get('data'):
        return None
    return m['columns'], m['data']


def one(root, sn):
    mis, prof = load(root, sn, 'mission'), load(root, sn, 'profile')
    if mis is None or prof is None:
        return None
    mc = mis[0]
    mrows = {int(r[0]): dict(zip(mc, r)) for r in mis[1] if r[0] is not None}
    c = prof[0]
    i_d, i_p, i_t, i_r = (c.index(k) for k in ('dive', 'pres', 'temp', 'dens'))
    byd = {}
    for r in prof[1]:
        if r[i_d] is None or r[i_d] < 1 or None in (r[i_p], r[i_t], r[i_r]):
            continue
        byd.setdefault(int(r[i_d]), []).append((r[i_p], r[i_t], r[i_r]))
    best = None
    for dv in sorted(byd)[-10:]:
        m = mrows.get(dv) or mrows.get(max(k for k in mrows if k <= dv), {}) if mrows else {}
        dtd, dsd = m.get('DTD0'), m.get('DSD0')
        if not dtd or not (50 <= dtd <= 2000) or dsd is None or dsd > 2000:
            continue
        a = np.array(byd[dv], float)
        if a[:, 0].max() < dtd + BELOW or a[:, 0].min() > 20:
            continue
        if best is None or a[:, 0].max() > best[1][:, 0].max():
            best = (dv, a, dtd, dsd)
    if best is None:
        return None
    dv, a, dtd, dsd = best
    edges = np.arange(0, a[:, 0].max() + GRID, GRID)
    k = np.digitize(a[:, 0], edges)
    p, T, rho = [], [], []
    for j in sorted(set(k)):
        m = k == j
        p.append(float(a[m, 0].mean())); T.append(float(np.median(a[m, 1])))
        rho.append(float(np.median(a[m, 2])) + 1000.0)
    return dict(sn=sn, type='ALTO' if sn >= 10000 else 'ALAMO', dive=dv, dtd=float(dtd),
                dsd=float(dsd), p=p, T=T, rho=rho)


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('--json-dir', default='/data1/alamo/data/json')
    ap.add_argument('-o', '--out', default='profiles.json')
    ap.add_argument('--n-alto', type=int, default=40)
    ap.add_argument('--n-alamo', type=int, default=30)
    ap.add_argument('--seed', type=int, default=1)
    args = ap.parse_args()
    sns = sorted(int(d) for d in os.listdir(args.json_dir) if d.isdigit())
    rows = [r for r in (one(args.json_dir, sn) for sn in sns) if r]
    rng = np.random.default_rng(args.seed)
    out = []
    for typ, n in (('ALTO', args.n_alto), ('ALAMO', args.n_alamo)):
        pool = [r for r in rows if r['type'] == typ]
        pick = rng.choice(len(pool), size=min(n, len(pool)), replace=False)
        out += [pool[i] for i in sorted(pick)]
    json.dump(out, open(args.out, 'w'))
    for typ in ('ALTO', 'ALAMO'):
        q = [r for r in rows if r['type'] == typ]
        s = [r for r in out if r['type'] == typ]
        print('%s: %d floats usable, %d sampled; targets %s' % (
            typ, len(q), len(s), sorted(set(int(r['dtd']) for r in s))))
