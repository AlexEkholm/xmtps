#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fleet telemetry: how well the present seek and ascent control perform (reads the decoder's JSON files only).

Per float and dive: target DTD, time from leaving the surface to settle within +-25 dBar of DTD,
park error, seek pump runs (pump records before the deepest run, minus the initial DFT run),
ascent pumps (the run of records from the deepest one on), rise-rate statistics.
"""
import json
import os
import sys
import numpy as np
from multiprocessing import Pool

ROOT = '/data1/alamo/data/json'      # --json-dir


def _init(root):
    global ROOT
    ROOT = root


def load(sn, kind):
    f = f'{ROOT}/{sn}/{sn}_{kind}.json'
    if not os.path.exists(f):
        return None
    try:
        m = json.load(open(f))
    except Exception:
        return None
    if not isinstance(m, dict) or 'columns' not in m or not m['data']:
        return None
    return m['columns'], m['data']


def col(tab, name):
    c, d = tab
    i = c.index(name)
    return np.array([r[i] if r[i] is not None else np.nan for r in d], float)


def per_float(sn):
    try:
        mis, des, dri, asc, pmp = (load(sn, k) for k in ('mission', 'descent', 'drift', 'ascent', 'pump'))
        if mis is None or des is None or dri is None:
            return []
        mc = mis[0]
        mrows = {int(r[0]): dict(zip(mc, r)) for r in mis[1]}
        dd, dp, dtt = col(des, 'dive'), col(des, 'pres'), col(des, 'dt_epoch')
        rd, rp, rt = col(dri, 'dive'), col(dri, 'pres'), col(dri, 'dt_epoch')
        if asc is not None:
            ad, ap, at, aph = col(asc, 'dive'), col(asc, 'pres'), col(asc, 'dt_epoch'), col(asc, 'phase_code')
        if pmp is not None:
            pd_, pp, pt = col(pmp, 'dive'), col(pmp, 'pres'), col(pmp, 'time')
        out = []
        for dive in sorted(set(int(x) for x in dd if x >= 1)):
            m = mrows.get(dive)
            if m is None:
                continue
            dtd, dsd, dft = m.get('DTD0'), m.get('DSD0'), m.get('DFT0')
            same = all(m.get(k) in (None, dtd) for k in ('DTD1', 'DTD2'))
            arr, aps, apn, apf = m.get('ARR'), m.get('APS'), m.get('APN'), m.get('APF')
            if not dtd or dtd > 2500:
                continue
            md = dd == dive
            mr = rd == dive
            if mr.sum() < 5 or md.sum() < 3:
                continue
            t0 = np.nanmin(dtt[md])
            tt = np.concatenate([dtt[md], rt[mr]])
            pp_ = np.concatenate([dp[md], rp[mr]])
            o = np.argsort(tt)
            tt, pp_ = tt[o], pp_[o]
            ok_ = np.isfinite(pp_)
            tt, pp_ = tt[ok_], pp_[ok_]
            rpm_all = rp[mr][np.isfinite(rp[mr])]
            n4 = len(rpm_all) // 4
            level = float(np.median(rpm_all[n4:len(rpm_all) - n4])) if len(rpm_all) > 8 else np.nan
            bad_l = np.where(np.abs(pp_ - level) > 25)[0]
            settle_l = np.nan
            if len(bad_l) and bad_l[-1] < len(pp_) - 1:
                settle_l = (tt[bad_l[-1] + 1] - t0) / 3600
            # time to first reach within 25 dBar of the level
            hit = np.where(np.abs(pp_ - level) <= 25)[0]
            reach_l = (tt[hit[0]] - t0) / 3600 if len(hit) else np.nan
            bad = np.where(np.abs(pp_ - dtd) > 25)[0]
            if len(bad) == 0:
                settle = 0.0
            elif bad[-1] == len(pp_) - 1:
                settle = np.nan
            else:
                settle = (tt[bad[-1] + 1] - t0) / 3600
            rpm = rp[mr]
            row = dict(sn=sn, type='ALTO' if sn >= 10000 else 'ALAMO', dive=dive, dtd=dtd, dsd=dsd,
                       dft=dft, dca=m.get('DCA'), dsw=m.get('DSW'), duc=m.get('DUC'), dpt=m.get('DPT'),
                       settle25_h=settle,
                       same_dtd=same, level=level, level_err=level - dtd,
                       dtd1=m.get('DTD1'), dtd2=m.get('DTD2'), dft1=m.get('DFT1'), dft2=m.get('DFT2'),
                       settle_level_h=settle_l, reach_level_h=reach_l,
                       level_spread=float(np.percentile(np.abs(rpm_all - level), 90)) if len(rpm_all) > 8 else np.nan,
                       drift_h=float((np.nanmax(rt[mr]) - np.nanmin(rt[mr])) / 3600),
                       arr=arr, aps=aps, apn=apn, apf=apf)
            if pmp is not None:
                mp = pd_ == dive
                prs, tms = pp[mp], pt[mp]
                if len(prs):
                    k = int(np.nanargmax(prs))
                    seek = list(range(0, k))
                    if seek and dsd and abs(prs[0] - dsd) < 30:
                        seek = seek[1:]
                    row.update(n_seek_pumps=len(seek), seek_pump_s=float(np.nansum(tms[seek])),
                               n_asc_pumps=len(prs) - k, asc_pump_s=float(np.nansum(tms[k:])),
                               dft_pump_s=float(tms[0]) if dsd and abs(prs[0] - dsd) < 30 else np.nan)
            if asc is not None:
                ma = (ad == dive) & (ap > 20) & (aph == 13)
                a_p, a_t = ap[ma], at[ma]
                o = np.argsort(a_t)
                a_p, a_t = a_p[o], a_t[o]
                dt_ = np.diff(a_t)
                ok = dt_ >= 20
                if ok.sum() > 5:
                    w = -np.diff(a_p)[ok] / dt_[ok]
                    row.update(rise_mean=float(np.mean(w)), rise_sd=float(np.std(w)),
                               rise_first300=float(np.mean(w[:max(3, len(w) // 10)])))
                    row.update(rise_med=float(np.median(w)), rise_p10=float(np.percentile(w, 10)),
                               rise_p90=float(np.percentile(w, 90)),
                               rise_rms_arr=float(np.sqrt(np.mean((w - arr) ** 2))) if arr and arr < 1 else np.nan,
                               ascent_h=float((a_t[-1] - a_t[0]) / 3600),
                               ascent_dp=float(a_p[0] - a_p[-1]))
            out.append(row)
        return out
    except Exception as e:
        print(sn, 'error', e, file=sys.stderr)
        return []


if __name__ == '__main__':
    import argparse
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    ap.add_argument('out', help='output JSON (one record per dive)')
    ap.add_argument('--json-dir', default=ROOT)
    args = ap.parse_args()
    ROOT = args.json_dir
    sns = sorted(int(d) for d in os.listdir(ROOT) if d.isdigit())
    with Pool(16, initializer=_init, initargs=(ROOT,)) as pool:
        res = pool.map(per_float, sns, chunksize=4)
    rows = [r for rr in res for r in rr]
    json.dump(rows, open(args.out, 'w'))
    print(len(rows), 'dives')
