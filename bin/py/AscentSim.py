#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Ascent-rate control: the bang-bang rule against a predictive band rule and a feed-forward
rule (stiffness profile from the descent, slow correction from the hour-mean rate).

    python AscentSim.py      # sweep, writes ascsweep.json
"""
import numpy as np, json
from multiprocessing import Pool
import SeekSim as S

def band_ctrl(fl, dt, ARR, delta, APS_s=None, check=20.0, win=200.0, q_nom=0.317, cda_nom=0.07):
    """Predictive band rule: start pump sized from the model to reach ARR; then pump when the rate
    is predicted to fall below ARR - delta before the next check, for the time that brings it to
    ARR + delta. G (dy per pump s) from the start pump's response."""
    tr = S.Track()
    G = 2 * 9.81 * q_nom * 1e-6 / cda_nom
    t_start = (ARR ** 2) / G if APS_s is None else APS_s
    fl.pump(t_start)
    t_cal = t_start + 120
    calibrated = False
    rates = []
    t_check = t_cal
    lo, hi = (ARR - delta) ** 2, (ARR + delta) ** 2
    while fl.p > 2 and fl.t < 6 * 3600:
        p = fl.measure(); tr.add(fl.t, p)
        if fl.t >= t_check and not fl.busy():
            fit = tr.yline(fl.t - win, fl.t, lag=60.0)
            if fit is not None:
                a, s, _, _ = fit
                y_now = a + s * p
                if not calibrated:
                    G = max(y_now, 1e-4) / t_start; calibrated = True
                w = np.sqrt(max(y_now, 0))
                y_next = y_now - max(s, 0) * w * (check + 60)     # predicted after next check + lag
                if y_next < lo:
                    fl.pump(max((hi - y_now) / G, 2.0))
            t_check = fl.t + check
        fl.step(dt)
        rates.append(fl.w() + fl.w_iw)
    return np.array(rates)

def case(c):
    ftype, strat, iw, ctrl, par, n = c
    out = []
    for i in range(n):
        rng = np.random.default_rng([i, 7, int(iw * 10), len(strat), len(ftype)])
        p0 = 2000.0 if ftype == 'ALTO' else 1000.0
        fl = S.Float(rng, ftype, p0, iw, strat, 0.0, 0.05 if ftype == 'ALTO' else 0.5)
        fl.O, fl.p = 0.0, p0
        ARR = 0.10
        if ctrl == 'bangbang':
            APS, APF = (120, 100) if ftype == 'ALTO' else (60, 20)
            r = S.ascent_bangbang(fl, 5.0, ARR=ARR, APS=APS, APN=par, APF=APF)
        else:
            r = band_ctrl(fl, 5.0, ARR, par / 100, cda_nom=0.07 if ftype == 'ALTO' else 0.033)
        r = r[len(r) // 20:]
        out.append(dict(rms=np.sqrt(np.mean((r - ARR) ** 2)) * 100, bias=(np.mean(r) - ARR) * 100,
                        p10=(np.percentile(r, 10) - ARR) * 100, n_pump=fl.n_pump, pump_s=fl.pump_s, t_h=fl.t / 3600))
    return dict(type=ftype, strat=strat, iw=iw, ctrl=ctrl, par=par, **{k: float(np.median([o[k] for o in out])) for k in out[0]})

if __name__ == '__main__':
    cs = [(ft, st, iw, 'bangbang', apn, 60) for ft in ('ALTO', 'ALAMO') for st in ('strong', 'weak') for iw in (0.0, 5.0) for apn in (5, 10, 20, 40, 80)]
    cs += [(ft, st, iw, 'band', d, 60) for ft in ('ALTO', 'ALAMO') for st in ('strong', 'weak') for iw in (0.0, 5.0) for d in (0.5, 1.0, 1.5, 2.0)]
    with Pool(24) as p: res = p.map(case, cs, chunksize=1)
    json.dump(res, open('ascsweep.json', 'w'), indent=1)
    import pandas as pd; pd.set_option('display.width', 200)
    print(pd.DataFrame(res).round(2).to_string())


def ff_ctrl(fl, dt, ARR, T=600.0, k_prior=0.035, cda_nom=0.07, q_nom=0.317, Kp=0.5, win=3600.0,
            t_min=2.0, k_of_p=None):
    """Feed-forward ascent: start pump sized for ARR from the drag prior; then every T s a pump of
    the time that replaces the buoyancy the float loses while rising (stiffness x ARR x T), scaled
    by a factor that a slow loop corrects from the mean rate over the last `win` s."""
    G = 2 * 9.81 * q_nom * 1e-6 / cda_nom
    s_ff = 2 * 9.81 * k_prior * 1e-6 / cda_nom          # y lost per dBar risen
    t_start = ARR ** 2 / G
    fl.pump(t_start)
    tr = S.Track()
    rates = []
    scale = 1.0
    acc = 0.0                                            # pump seconds owed (sub-minimum runs)
    t_next = t_start + T
    while fl.p > 2 and fl.t < 8 * 3600:
        p = fl.measure(); tr.add(fl.t, p)
        if fl.t >= t_next and not fl.busy():
            tt = np.array(tr.t); m = tt >= fl.t - win
            if m.sum() > 20 and fl.t > 1800:
                c = np.polyfit(tt[m], np.array(tr.p)[m], 1)
                w_m = -c[0]
                # slow loop: mean rate error -> scale of the feed-forward, and a one-off correction
                scale = float(np.clip(scale * (1 + Kp * (ARR - w_m) / ARR * T / win), 0.3, 3.0))
            kk = k_of_p(fl.p) if k_of_p else k_prior          # stiffness here (mL/dBar)
            acc += scale * (2 * 9.81 * kk * 1e-6 / cda_nom) * ARR * T / G
            if acc >= t_min:
                fl.pump(acc); acc = 0.0
            t_next = fl.t + T
        fl.step(dt)
        rates.append(fl.w() + fl.w_iw)
    return np.array(rates)
