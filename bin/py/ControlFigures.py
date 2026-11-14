#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Figures for the depth-control report (style as BuoyancyPlots.py).

    XMTPS_JSON_DIR=/data1/alamo/data/json python ControlFigures.py FIGDIR RESULTDIR [figure ...]
"""
import json
import os
import sys
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import SeekSim as S
import AscentSim as A

OUT = sys.argv[1]                    # figure folder
ARG = sys.argv[2]                    # folder with fleetctl.json, seek2sim*.json
JSON = os.environ.get('XMTPS_JSON_DIR', '/data1/alamo/data/json')
os.makedirs(OUT, exist_ok=True)
INK, INK2 = '#0b0b0b', '#52514e'
REF = '#dedede'
CONFIG = '#0000ff'
COLORS = {'ALAMO': '#2a78d6', 'ALTO': '#eb6834'}
CTRL_COL = {'prog': '#1f77b4', 'fw': '#7f7f7f', 'fw_dca8': '#bcbcbc', 'fw_gainK': '#2ca02c', 'so': '#d62728',
            'ekf': '#9467bd', 'drag': '#ff9896', 'so_t60d5': '#e377c2'}
CTRL_LAB = {'prog': 'Second order, progressive', 'fw': 'Firmware rule (fleet settings)', 'fw_dca8': 'Firmware rule, DCA 8, DSW 1 h',
            'fw_gainK': 'First order, true stiffness (benchmark)',
            'so': 'Second order, brake', 'ekf': 'Second order, brake, Kalman filter',
            'drag': 'Second order, 10 min windows', 'so_t60d5': 'Second order, 1 h trims'}
plt.rcParams.update({
    'font.size': 9, 'axes.edgecolor': INK2, 'axes.labelcolor': INK, 'xtick.color': INK2,
    'ytick.color': INK2, 'axes.grid': False, 'axes.spines.top': False, 'axes.spines.right': False,
    'legend.frameon': False, 'axes.facecolor': 'white', 'figure.facecolor': 'white',
    'axes.unicode_minus': True, 'savefig.dpi': 200, 'savefig.bbox': 'tight'})


def save(fig, name):
    fig.savefig(os.path.join(OUT, name))
    plt.close(fig)


# ---- 1. y = w|w| against p on real descents (11333)
def fig_ylinear():
    sn = 11333
    d = json.load(open(f'{JSON}/{sn}/{sn}_descent.json'))
    c = d['columns']
    rows = [dict(zip(c, r)) for r in d['data'] if r[0] is not None and r[0] >= 1]
    df = pd.DataFrame(rows).dropna(subset=['pres', 'dt_epoch'])
    fig, ax = plt.subplots(figsize=(6.5, 4.2))
    dives = sorted(df.dive.unique())[-40::4]
    cmap = plt.get_cmap('viridis')
    for n, dv in enumerate(dives):
        q = df[df.dive == dv].sort_values('dt_epoch')
        p, t = q.pres.values, q.dt_epoch.values
        w = -np.diff(p) / np.diff(t)
        pm = 0.5 * (p[1:] + p[:-1])
        m = (pm > 260) & (pm < 980) & (np.diff(t) > 20)
        if m.sum() < 10:
            continue
        y = w[m] * np.abs(w[m]) * 1e4
        col = cmap(n / max(len(dives) - 1, 1))
        ax.plot(y, pm[m], '.', ms=3, color=col, alpha=0.7)
        cf = np.polyfit(pm[m], y, 1)
        pp = np.linspace(pm[m].min(), max(pm[m].max(), -cf[1] / cf[0] if cf[0] else pm[m].max()), 50)
        pp = pp[pp < 1300]
        ax.plot(np.polyval(cf, pp), pp, '--', color=col, lw=1)
        if cf[0] > 0:
            pn = -cf[1] / cf[0]
            if 700 < pn < 1300:
                ax.plot(0, pn, 'o', color=col, ms=5, mec=INK, mew=0.3)
    ax.axvline(0, color=REF, ls='--', lw=1.5, zorder=0)
    ax.set_ylim(1300, 250)
    ax.set_xlabel(r'$y = w|w|$ [cm$^2$ s$^{-2}$]')
    ax.set_ylabel('Pressure [dBar]')
    ax.set_title('11333: Descent After the Fall Pump')
    save(fig, 'fig_ylinear.png')


# ---- 2. fleet: park level vs target, first dive vs later
def fig_fleet():
    d = pd.DataFrame(json.load(open(os.path.join(ARG, 'fleetctl.json'))))
    d = d[d.level.notna()]

    def tgt(r):
        c = [x for x in (r.dtd, r.dtd1, r.dtd2) if x is not None and np.isfinite(x) and 50 <= x <= 2500]
        return min(c, key=lambda x: abs(x - r.level)) if c else np.nan
    d['tgt'] = d.apply(tgt, axis=1)
    d = d[d.tgt.notna()]
    d['err'] = (d.level - d.tgt).abs()
    d['grp'] = np.where(d.dive == 1, 'Dive 1', np.where(d.dive <= 5, 'Dives 2–5', 'Dives > 5'))
    stats = {}
    fig, axes = plt.subplots(1, 2, figsize=(9, 3.6), sharey=True)
    for ax, t in zip(axes, ('ALTO', 'ALAMO')):
        x = d[d.type == t]
        for g, ls in (('Dive 1', '-'), ('Dives 2–5', '--'), ('Dives > 5', ':')):
            e = np.sort(x[x.grp == g].err.values)
            if not len(e):
                continue
            ax.plot(e, np.arange(1, len(e) + 1) / len(e), ls, color=COLORS[t], lw=1.8,
                    label='{} (n = {})'.format(g, len(e)))
            stats['{} {}'.format(t, g)] = dict(n=int(len(e)), median=float(np.median(e)),
                                               within25=float(np.mean(e <= 25)),
                                               within50=float(np.mean(e <= 50)),
                                               reach_h=float(x[x.grp == g].reach_level_h.median()),
                                               seek_pumps=float(x[x.grp == g].n_seek_pumps.mean()))
        ax.axvline(25, color=REF, ls='--', lw=1.5, zorder=0)
        ax.set_xscale('log')
        ax.set_xlim(1, 1000)
        ax.set_xlabel('|Park Level − Target| [dBar]')
        ax.set_title(t)
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.17), ncol=1)
    axes[0].set_ylabel('Fraction of Dives')
    save(fig, 'fig_fleet_seek.png')
    json.dump(stats, open(os.path.join(OUT, 'fleet_stats.json'), 'w'), indent=1)


# ---- 3. example seek trajectories
def fig_traj():
    fig, axes = plt.subplots(1, 2, figsize=(10, 4), sharey=True)
    for ax, A_ in zip(axes, (0.0, 5.0)):
        for ctrl in ('fw', 'so', 'prog'):
            rng = np.random.default_rng([3, 1000, int(A_ * 10), 75, 4, 4])
            fl = S.Float(rng, 'ALTO', 1000.0, A_, 'weak', 0.75, 0.05)
            fw = dict(DPT=10, DUT=10, DUC=0.5, DCT=10800, DSW=10800, DCA=3, PST=21600, PSD=200,
                      DCP=2, DCF=2, DSD=200)
            if ctrl == 'fw':
                ts, ps = S.run_firmware(fl, 12 * 3600, 10.0, **fw)
            elif ctrl == 'fw_dca8':
                ts, ps = S.run_firmware(fl, 12 * 3600, 10.0, **dict(fw, DCA=8, DSW=3600))
            elif ctrl == 'so':
                ts, ps = S.run_so(fl, 12 * 3600, 10.0)
            else:
                ts, ps = S.run_prog(fl, 12 * 3600, 10.0, win=1200.0, fall_frac=0.9, n_sig=2.0,
                                    p_fall=200.0)
            ax.plot(np.array(ts) / 3600, ps, color=CTRL_COL[ctrl], lw=1.4,
                    label='{} ({} runs, {:.0f} J)'.format(CTRL_LAB[ctrl], fl.n_pump + fl.n_valve,
                                                         fl.energy))
        ax.axhline(1000, color=CONFIG, ls='--', lw=1.5, zorder=0)
        ax.set_ylim(1400, 0)
        ax.set_xlabel('Time Since Leaving the Surface [h]')
        ax.set_title('No Internal Waves' if A_ == 0 else 'Internal Waves, ~1 cm/s rms')
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=1, fontsize=8)
    axes[0].set_ylabel('Pressure [dBar]')
    save(fig, 'fig_seek_traj.png')


# ---- 4. seek summary
def fig_seek_summary():
    """Seek simulations with energy, for the first dive (ballast plan +-75 %) and later dives
    (+-10 %), from seek_energy.json."""
    s = pd.DataFrame(json.load(open(os.path.join(ARG, 'seek_energy.json')))['seek'])
    ctrls = [c for c in ('fw', 'fw_dca8', 'so', 'prog', 'ekf', 'fw_gainK') if c in set(s.ctrl)]
    cols = ['settled_mean25', 'settle_mean25_h', 'ops', 'pump_s', 'valve_s', 'energy',
            'abs_err_last4h']
    g = s.groupby(['prior_err', 'iw', 'ctrl'])[cols].mean()
    g.round(3).to_csv(os.path.join(OUT, 'seek_summary.csv'))
    s.groupby(['type', 'p_T', 'prior_err', 'iw', 'ctrl'])[cols].mean().round(3).to_csv(
        os.path.join(OUT, 'seek_detail.csv'))
    iws = sorted(s.iw.unique())
    lab = {0.0: '0', 5.0: '~1', 15.0: '~3'}
    x = np.arange(len(iws))
    wdt = 0.8 / len(ctrls)
    fig, axes = plt.subplots(2, 3, figsize=(11, 6.4), sharex=True)
    for row, pe, name in ((0, 0.75, 'First Dive'), (1, 0.1, 'Later Dives')):
        for k, c in enumerate(ctrls):
            vals = [g.loc[(pe, i, c)] for i in iws]
            for ax, col, sc in zip(axes[row], ('settled_mean25', 'energy', 'ops'), (100, 1, 1)):
                ax.bar(x + (k - len(ctrls) / 2 + 0.5) * wdt, [v[col] * sc for v in vals], wdt,
                       color=CTRL_COL[c], label=CTRL_LAB[c])
        for ax, t in zip(axes[row], ('Settled Within 25 dBar [%]', 'Pump Energy [J]',
                                     'Pump and Valve Runs')):
            ax.set_title('{}: {}'.format(name, t))
    for ax in axes[1]:
        ax.set_xticks(x)
        ax.set_xticklabels([lab[i] for i in iws])
        ax.set_xlabel('Internal-Wave Speed, rms [cm/s]')
    h, l = axes[0, 0].get_legend_handles_labels()
    fig.legend(h, l, loc='upper center', ncol=3, fontsize=8, bbox_to_anchor=(0.5, 0.0))
    fig.tight_layout()
    save(fig, 'fig_seek_summary.png')


def fig_neutral():
    """Error of the neutral pressure predicted from the rate on real descents, against the
    distance still to go (xmtps-control-neutral.py output, neutral.json)."""
    r = [x for x in json.load(open(os.path.join(ARG, 'neutral.json'))) if x.get('ok')]
    d = pd.DataFrame(r)
    d = d[d.win == 1200]
    fig, axes = plt.subplots(1, 2, figsize=(9, 3.6), sharey=True)
    for ax, t in zip(axes, ('ALTO', 'ALAMO')):
        q = d[d.type == t]
        D = sorted(q.D.unique())
        med = [np.median(q[q.D == v].err) for v in D]
        lo = [np.percentile(q[q.D == v].err, 25) for v in D]
        hi = [np.percentile(q[q.D == v].err, 75) for v in D]
        ax.fill_between(D, lo, hi, color=COLORS[t], alpha=0.25, lw=0, label='Interquartile Range')
        ax.plot(D, med, 'o-', color=COLORS[t], label='Median')
        ax.axhline(0, color=REF, ls='--', lw=1.5, zorder=0)
        ax.set_xscale('log')
        ax.minorticks_off()
        ax.set_xticks(D)
        ax.set_xticklabels([str(int(v)) for v in D])
        ax.set_xlabel('Distance Still to Go [dBar]')
        ax.set_title('{} ({} Estimates)'.format(t, len(q)))
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.17), ncol=2)
    axes[0].set_ylabel('Predicted − Actual Neutral Pressure [dBar]')
    save(fig, 'fig_neutral.png')


# ---- 5. real ascent (11333)
def fig_ascent_real():
    sn, dive = 11333, 58
    a = json.load(open(f'{JSON}/{sn}/{sn}_ascent.json'))
    c = a['columns']
    q = pd.DataFrame([dict(zip(c, r)) for r in a['data'] if r[0] == dive])
    q = q[(q.phase_code == 13) & q.pres.notna()].sort_values('dt_epoch')
    p, t = q.pres.values, q.dt_epoch.values
    w = -np.diff(p) / np.diff(t) * 100
    pm = 0.5 * (p[1:] + p[:-1])
    pu = json.load(open(f'{JSON}/{sn}/{sn}_pump.json'))
    pp = [r[1] for r in pu['data'] if r[0] == dive and r[1] is not None and r[1] < 1995 and r[1] > 3]
    m = json.load(open(f'{JSON}/{sn}/{sn}_mission.json'))
    mr = {r[0]: dict(zip(m['columns'], r)) for r in m['data']}[dive]
    fig, ax = plt.subplots(figsize=(5, 5))
    ax.plot(w, pm, color='#ff4545', lw=1.2)
    for x in pp:
        ax.axhline(x, color=REF, lw=1, zorder=0)
    ax.axvline(mr['ARR'] * 100, color=CONFIG, ls='--', lw=1.5, label='ARR')
    ax.set_ylim(2010, 0)
    ax.set_xlabel('Rise Rate [cm/s]')
    ax.set_ylabel('Pressure [dBar]')
    ax.set_title('{}: Ascent of Dive {} ({} pump runs)'.format(sn, dive, len(pp)))
    ax.legend(loc='lower right')
    save(fig, 'fig_ascent_real.png')


# ---- 6. ascent trade-off
def fig_ascent_tradeoff():
    res = []
    for iw in (0.0, 5.0):
        for kind, pars in (('bangbang', (10, 20, 40, 80)), ('ff', (600, 1200, 2400, 4800))):
            for par in pars:
                out = []
                for i in range(24):
                    rng = np.random.default_rng([i, 7, int(iw * 10), 6, 4])
                    fl = S.Float(rng, 'ALTO', 2000.0, iw, 'strong', 0.0, 0.05)
                    fl.O, fl.p = 0.0, 2000.0
                    if kind == 'bangbang':
                        r = S.ascent_bangbang(fl, 5.0, ARR=0.1, APS=120, APN=par, APF=100)
                    else:
                        nz = 1 + 0.2 * rng.normal()
                        kf = lambda p, fl=fl, nz=nz: nz * (fl.k_c + fl.k_w + fl.k_s0 * np.exp(-p / fl.h_s))
                        r = A.ff_ctrl(fl, 5.0, 0.1, T=par, Kp=2.0, k_of_p=kf)
                    r = r[len(r) // 20:]
                    rm = np.convolve(r, np.ones(720) / 720, 'valid')
                    out.append((np.sqrt(np.mean((rm - 0.1) ** 2)) * 100, (np.mean(r) - 0.1) * 100,
                                fl.n_pump, fl.pump_s))
                o = np.median(np.array(out), 0)
                res.append(dict(iw=iw, kind=kind, par=par, rms_1h=o[0], bias=o[1], n_pump=o[2],
                                pump_s=o[3]))
    df = pd.DataFrame(res)
    df.to_csv(os.path.join(OUT, 'ascent_tradeoff.csv'), index=False)
    fig, axes = plt.subplots(1, 2, figsize=(9, 3.6), sharey=True)
    for ax, iw in zip(axes, (0.0, 5.0)):
        for kind, col, lab in (('bangbang', '#7f7f7f', 'Bang-bang (APN 10–80 s)'),
                               ('ff', '#d62728', 'Feed-forward (every 10–80 min)')):
            q = df[(df.iw == iw) & (df.kind == kind)].sort_values('n_pump')
            ax.plot(q.n_pump, q.rms_1h, 'o-', color=col, label=lab)
        ax.set_xlabel('Pump Runs per Ascent')
        ax.set_title('No Internal Waves' if iw == 0 else 'Internal Waves, ~1 cm/s rms')
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.17), ncol=1)
    axes[0].set_ylabel('Hour-Mean Rate Error, rms [cm/s]')
    save(fig, 'fig_ascent_tradeoff.png')


if __name__ == '__main__':
    which = sys.argv[3:] or ['ylinear', 'fleet', 'traj', 'summary', 'ascreal', 'asctrade']
    for w in which:
        {'ylinear': fig_ylinear, 'fleet': fig_fleet, 'traj': fig_traj, 'summary': fig_seek_summary,
         'neutral': fig_neutral,
         'ascreal': fig_ascent_real, 'asctrade': fig_ascent_tradeoff}[w]()
        print('done', w)
