# -*- coding: utf-8 -*-
"""Figures and summary statistics of the buoyancy model (xmtps-buoyancy-model.py).

WriteFigures(out_dir) reads the CSV files the model wrote to out_dir and writes the figures
(PNG, 200 dpi) and buoyancy_summary.json (group statistics used by the write-up) to
out_dir/figures. Floats are 'usable' when the fit converged inside the C_dA search range, the
speed R^2 is at least USABLE_R2 and the bootstrap spread of log C_dA is under USABLE_LOG_CDA_SD.

The figures follow spec/plot-style-guide.md where it applies to static plots:
  - titles '<serial>: <Title>' (3.2; fleet figures have no title, the report's captions name
    them) and axis titles 'Quantity [unit]' with dBar and U+2212 minus signs (4.1);
  - no grid and a white background (3.4);
  - pressure axes reversed with an explicit [deep, 0] range (4.2);
  - named colors (5.2): the bright phase colors for lines and points, neutral bars for
    histograms, blue for configured values (firmware PFR, the usability threshold) and gray
    for reference lines (5.3, 5.4).
The guide's hover rules are for the interactive Plotly plots and don't apply here. Deliberate
departures, which the guide does not cover: the model's vertical speed w (cm s^-1, positive
up) is plotted instead of dp/dt; float types use two categorical colors (blue ALAMO, orange
ALTO); the three series of the mass-sensitivity plot use black, gray and the estimate blue
with distinct markers; matplotlib's own font and legends in the PNGs (the report has no
HTML version).

Author: aekholm
"""

import json
import os

import numpy as np
import pandas as pd

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

USABLE_R2 = 0.8
USABLE_LOG_CDA_SD = 0.3
MIN_SUPPORT = 20                    # events near a flow node for it to be plotted
VALVE_DP0_DBAR = 2.0                # as in xmtps-buoyancy-model.py
MIN_SURFACE_VALVE_POINTS = 50      # descent points with the surface valve open, for a fuse fit

COLORS = { 'ALAMO': '#2a78d6', 'ALTO': '#eb6834' }   # float types (categorical)
MARKERS = { 'SBE': 'o', 'RBR': '^', '': 's' }
PHASE_COLORS = { 'descent': '#45ff45', 'ascent': '#ff4545' }   # style guide 5.2, lines: rgb(69,255,69), rgb(255,69,69)
INK, INK2 = '#0b0b0b', '#52514e'
REF = '#dedede'                     # style guide 5.2-5.4: reference lines rgb(222,222,222), dashed, 1.5
CONFIG = '#0000ff'                  # style guide 5.2: configured value or limit, rgb(0,0,255), dashed, 1.5
GUIDE = '#969696'                   # style guide 5.4: darker guide rgb(150,150,150), dashed, 1
BAR_FILL, BAR_EDGE = '#dcdcdc', '#bcbcbc'   # style guide 5.2: neutral bars rgb(220,220,220), rgb(188,188,188)
MINUS = '−'

# Axis titles (style guide 4: 'Quantity [unit]'); mathtext renders - as U+2212
AX_PRES = 'Pressure [dBar]'
AX_SPEED = 'Vertical Speed (Up +), $w$ [cm s$^{-1}$]'
AX_PUMP = 'Pump Flow Rate [mL min$^{-1}$]'
AX_VALVE = 'Valve Flow Rate [mL min$^{-1}$]'

plt.rcParams.update({
    'font.size': 9, 'axes.edgecolor': INK2, 'axes.labelcolor': INK, 'xtick.color': INK2,
    'ytick.color': INK2, 'axes.grid': False,
    'axes.spines.top': False, 'axes.spines.right': False, 'legend.frameon': False,
    'axes.facecolor': 'white', 'figure.facecolor': 'white', 'axes.unicode_minus': True,
    'savefig.dpi': 200, 'savefig.bbox': 'tight',
})


def PresRange(ax, values, pad=10, top=0):
    """Pressure axis reversed with an explicit [deep, top] range (style guide 4.2)."""
    deep = np.nanmax(values) if len(values) else 100
    ax.set_ylim(round(deep + pad), top)


def RefLine(ax, value, horizontal=False):
    """A reference line in the style guide's look: dashed light grey, width 1.5."""
    (ax.axhline if horizontal else ax.axvline)(value, color=REF, ls='--', lw=1.5, zorder=0)


def Groups(df):
    df = df.copy()
    df['ctd'] = df['ctd'].fillna('').astype(str)
    df['group'] = df['type'] + ' ' + df['ctd'].replace('', '?')
    return df


def Usable(fl):
    ok = (fl['status'] == 'ok') & (~fl['cda_at_bound'].astype(bool)) & (fl['r2_w'] >= USABLE_R2)
    ok &= fl['log_cda_sd'].fillna(0) < USABLE_LOG_CDA_SD
    return ok


def Save(fig, path):
    fig.savefig(path)
    plt.close(fig)


def FigExample(points, fl, sn, path):
    p = points[points['sn'] == sn]
    fig, axes = plt.subplots(1, 3, figsize=(10, 4.4), sharey=True)
    for ax, branch, title, phase in zip(axes, ('descent', 'profile_descent', 'ascent'),
                                        ('Descent to Park', 'Descent from Park to Profile', 'Ascent'),
                                        ('descent', 'descent', 'ascent')):
        q = p[p['branch'] == branch]
        if not len(q):
            ax.set_title(title + ' (None)', fontsize=9, color=INK2)
            continue
        ax.scatter(q['w'] * 100, q['pres'], s=3, color=INK2, alpha=0.25, lw=0, label='Measured')
        ax.scatter(q['w_model'] * 100, q['pres'], s=3, color=PHASE_COLORS[phase], alpha=0.35, lw=0,
                   label='Model')
        ax.set_title(title, fontsize=9)
        ax.set_xlabel(AX_SPEED)
        # one legend per panel, below its axis, so it never covers data
        leg = ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.17), ncol=2, markerscale=4)
        for h in leg.legend_handles:
            h.set_alpha(1)
    PresRange(axes[0], p['pres'].to_numpy(), pad=1)
    axes[0].set_ylabel(AX_PRES)
    row = fl.set_index('sn').loc[sn]
    fig.suptitle('{}: Measured and Modelled Vertical Speed'.format(sn), fontsize=10, y=1.04)
    fig.text(0.5, 0.985,'{} dives, rms residual {:.2f} cm s$^{{-1}}$, $R^2$ = {:.3f}'.format(
        int(row['n_dives']), row['rms_w_cm_s'], row['r2_w']), ha='center', fontsize=8, color=INK2)
    Save(fig, path)


def FigPumpFlow(flow, fl, path, kind='pump'):
    use = fl[Usable(fl)].set_index('sn')
    f = flow[(flow['kind'] == kind) & (flow['support'] >= MIN_SUPPORT) & flow['sn'].isin(use.index)]
    fig, axes = plt.subplots(1, 2, figsize=(10, 4.2), sharex=True, sharey=True)
    for ax, ftype in zip(axes, ('ALAMO', 'ALTO')):
        sns = use.index[use['type'] == ftype]
        g = f[f['sn'].isin(sns)]
        for sn, h in g.groupby('sn'):
            ax.plot(h['q_ml_min'], h['pres'], color=COLORS[ftype], alpha=0.15, lw=0.8)
        med = g.groupby('pres')['q_ml_min'].agg(['median', lambda x: x.quantile(0.25),
                                                 lambda x: x.quantile(0.75), 'size'])
        med.columns = ['median', 'q25', 'q75', 'n']
        med = med[med['n'] >= 3]
        ax.fill_betweenx(med.index, med['q25'], med['q75'], color=COLORS[ftype], alpha=0.25, lw=0)
        ax.plot(med['median'], med.index, color=COLORS[ftype], lw=2.2, label='Median, Interquartile Range')
        if kind == 'pump':
            pfr = use.loc[sns, 'pfr_ml_min'].median()
            if np.isfinite(pfr):
                ax.axvline(pfr, color=CONFIG, lw=1.5, ls='--', label='Firmware PFR ({:.0f} mL min$^{{-1}}$)'.format(pfr))
        ax.set_title('{} ({} Floats)'.format(ftype, g['sn'].nunique()), fontsize=9)
        ax.set_xlabel(AX_PUMP if kind == 'pump' else AX_VALVE)
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=1)   # below the axis
    PresRange(axes[0], f['pres'].to_numpy())
    axes[0].set_ylabel(AX_PRES)
    lo, hi = np.nanpercentile(f['q_ml_min'], [1, 99]) if len(f) else (0, 1)
    axes[0].set_xlim(min(0, lo), hi * 1.1)
    Save(fig, path)


def FuseUsable(fl):
    """Usable floats whose fuse pressure is constrained by surface-valve bleed data."""
    return Usable(fl) & fl['fuse_constrained'].fillna(False).astype(bool) & \
        (fl['n_surface_valve_points'].fillna(0) >= MIN_SURFACE_VALVE_POINTS)


def FigValve(flow, fl, path):
    """Valve flow against pressure (log scale) per float, with the fuse step, and type medians."""
    use = fl[FuseUsable(fl)].set_index('sn')
    # Curves from each float's fitted law on a fine grid (sharp fuse step)
    grid = np.concatenate([np.arange(0, 200, 1.0), np.arange(200, 2001, 20.0)])
    rows = []
    for sn, r in use.iterrows():
        root = np.sqrt(grid + VALVE_DP0_DBAR)
        lo = r['valve_c_lo'] * (root if r['fuse_law'] == 'sqrt' else 1.0)
        q = np.where(grid < r['fuse_dbar'], r['valve_c_hi'] * root, lo)
        rows.append(pd.DataFrame(dict(sn=sn, pres=grid, q_ml_min=q)))
    f = pd.concat(rows) if rows else pd.DataFrame(columns=['sn', 'pres', 'q_ml_min'])
    fig, axes = plt.subplots(1, 2, figsize=(10, 4.4), sharex=True, sharey=True)
    for ax, ftype in zip(axes, ('ALAMO', 'ALTO')):
        sns = use.index[use['type'] == ftype]
        g = f[f['sn'].isin(sns)]
        for sn, h in g.groupby('sn'):
            ax.plot(h['q_ml_min'], h['pres'] + 1, color=COLORS[ftype], alpha=0.2, lw=0.8)
        if len(g):
            med = g.groupby('pres')['q_ml_min'].median()
            ax.plot(med.values, med.index + 1, color=COLORS[ftype], lw=2.2, label='Median')
            pf = use.loc[sns, 'fuse_dbar'].median()
            ax.axhline(pf + 1, color=GUIDE, ls='--', lw=1,
                       label='Median Fuse Pressure ({:.0f} dBar)'.format(pf))
        ax.set_title('{} ({} Floats)'.format(ftype, len(sns)), fontsize=9)
        ax.set_xlabel(AX_VALVE)
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=1)   # below the axis
    axes[0].set_yscale('log')
    axes[0].set_ylim(f['pres'].max() + 1 if len(f) else 1000, 1)
    axes[0].set_ylabel('Pressure + 1 [dBar]')
    lo, hi = np.nanpercentile(f['q_ml_min'], [1, 99]) if len(f) else (0, 1)
    axes[0].set_xlim(min(0, lo), hi * 1.1)
    Save(fig, path)


def FigStrip(fl, col, label, path, log=False, ref=None, mask=None, ref_config=False):
    use = Groups(fl[Usable(fl) if mask is None else mask])
    groups = sorted(use['group'].unique())
    fig, ax = plt.subplots(figsize=(6.5, 3.6))
    rng = np.random.default_rng(0)
    if ref is not None:
        ax.axhline(ref[0], color=CONFIG if ref_config else REF, ls='--', lw=1.5, zorder=0,
                   label=ref[1])
        ax.legend(loc='best', fontsize=8)
    for k, g in enumerate(groups):
        v = use.loc[use['group'] == g, col].dropna()
        ftype, ctd = g.split(' ')
        x = k + rng.uniform(-0.18, 0.18, len(v))
        ax.scatter(x, v, s=12, color=COLORS[ftype], marker=MARKERS.get(ctd, 's'), alpha=0.6, lw=0)
        if len(v):
            ax.plot([k - 0.3, k + 0.3], [v.median()] * 2, color=INK, lw=2)
            ax.annotate('{:.3g}'.format(v.median()).replace('-', MINUS), (k, v.median()),
                        xytext=(0, 4), textcoords='offset points', ha='center', va='bottom',
                        fontsize=8, color=INK,
                        bbox=dict(boxstyle='round,pad=0.15', fc='white', ec='none', alpha=0.85))
    ax.set_xticks(range(len(groups)))
    ax.set_xticklabels(['{}\n(n = {})'.format(g, (use['group'] == g).sum()) for g in groups])
    ax.set_ylabel(label)
    if log:
        ax.set_yscale('log')
    Save(fig, path)


def FigQuality(points, fl, path):
    use = fl[Usable(fl)]
    fig, axes = plt.subplots(1, 3, figsize=(11, 3.6))
    ax = axes[0]
    ax.hist(fl.loc[fl['status'] == 'ok', 'rms_w_cm_s'].clip(upper=8), bins=40, color=BAR_FILL,
            edgecolor=BAR_EDGE, linewidth=0.5)
    ax.set_xlabel('RMS Speed Residual per Float [cm s$^{-1}$]')
    ax.set_ylabel('Floats')
    ax = axes[1]
    ax.hist(fl.loc[fl['status'] == 'ok', 'r2_w'].clip(lower=-0.5), bins=40, color=BAR_FILL,
            edgecolor=BAR_EDGE, linewidth=0.5)
    ax.axvline(USABLE_R2, color=CONFIG, ls='--', lw=1.5, label='Usability Threshold')
    ax.legend(loc='upper left')
    ax.set_xlabel('$R^2$ of Speed per Float')
    ax = axes[2]
    p = points[points['sn'].isin(use['sn'])].copy()
    p['res'] = (p['w'] - p['w_model']) * 100
    p['bin'] = (p['pres'] // 100) * 100 + 50
    RefLine(ax, 0)
    for branch in ('descent', 'ascent'):
        color = PHASE_COLORS[branch]
        g = p[p['branch'] == branch].groupby('bin')['res']
        st = g.agg(['median', lambda x: x.quantile(0.25), lambda x: x.quantile(0.75), 'size'])
        st.columns = ['m', 'a', 'b', 'n']
        st = st[st['n'] >= 200]
        ax.fill_betweenx(st.index, st['a'], st['b'], color=color, alpha=0.25, lw=0)
        ax.plot(st['m'], st.index, color=color, lw=1.8, label=branch.capitalize())
    PresRange(ax, st.index.to_numpy() if len(st) else np.array([100.0]), pad=50)
    ax.set_xlabel('Speed Residual, Measured {} Model [cm s$^{{-1}}$]'.format(MINUS))
    ax.set_ylabel(AX_PRES)
    ax.legend()
    fig.tight_layout()
    Save(fig, path)


def FigMassSensitivity(fl, sens, path):
    """Ratio of C_dA, q0 and the valve c at other mass scales to the nominal fit."""
    fig, ax = plt.subplots(figsize=(5.5, 3.6))
    base = fl[Usable(fl)].set_index('sn')
    scales, cols = sorted(sens), [('cda_m2', r'$C_dA$'), ('q0_ml_min', r'Pump $q_0$'),
                                  ('valve_q_1000_ml_min', 'Valve Flow at 1000 dBar')]
    x = [s for s in scales] + [1.0]
    ax.plot([0.6, 1.5], [0.6, 1.5], color=REF, ls='--', lw=1.5, label='Proportional', zorder=0)
    for k, (col, label) in enumerate(cols):
        ys = []
        for s in x:
            df = base if s == 1.0 else sens[s].set_index('sn')
            r = (df[col] / base[col]).reindex(base.index)
            ys.append(np.nanmedian(r[np.isfinite(r)]))
        order = np.argsort(x)
        ax.plot(np.array(x)[order], np.array(ys)[order], marker='os^'[k], ms=6, lw=1.5,
                color=['#000000', '#7b7b7b', '#1f77b4'][k], label=label)
    ax.set_xlabel('Assumed Mass / Nominal Mass')
    ax.set_ylabel('Median Ratio to Nominal Fit')
    ax.legend()
    Save(fig, path)


def Summary(fl, flow, sens):
    """Group statistics for the write-up."""
    fl = Groups(fl)
    use = fl[Usable(fl)]
    out = dict(n_floats=int(len(fl)), status=fl['status'].fillna('').str.split(':').str[0].value_counts().to_dict(),
               n_ok=int((fl['status'] == 'ok').sum()), n_usable=int(len(use)),
               n_points=int(fl['n_points'].fillna(0).sum()), n_dives=int(fl['n_dives'].fillna(0).sum()))
    cols = ['cda_m2', 'log_cda_sd', 'kappa_ml_dbar', 'v_gamma_w_ml_dbar', 'q0_ml_min',
            'q1_ml_min_per_kdbar', 'air_ml', 'valve_q_surface_ml_min', 'valve_q_1000_ml_min',
            'rms_w_cm_s', 'r2_w', 'sigma_wm_cm_s', 'dive_offset_sd_ml', 'vac_corr',
            'frac_ref_lost', 'n_dives']
    use = use.assign(net_ml_dbar=use['kappa_ml_dbar'] - use['v_gamma_w_ml_dbar'])
    groups = {}
    for name, g in list(use.groupby('group')) + [('all', use)]:
        d = dict(n=int(len(g)))
        for c in cols + ['net_ml_dbar']:
            v = g[c].dropna()
            d[c] = [float(v.quantile(q)) for q in (0.25, 0.5, 0.75)] if len(v) else None
        groups[name] = d
    out['groups'] = groups
    fu = fl[FuseUsable(fl)]
    out['fuse'] = dict(n=int(len(fu)), n_usable=int(len(use)))
    for name, g in list(fu.groupby('group')) + [('all', fu)]:
        out['fuse'][name] = dict(
            n=int(len(g)),
            fuse_dbar=[float(g['fuse_dbar'].quantile(q)) for q in (0.25, 0.5, 0.75)] if len(g) else None,
            frac_const=float((g['fuse_law'] == 'const').mean()) if len(g) else None,
            q_surface=[float(g['valve_q_surface_ml_min'].quantile(q)) for q in (0.25, 0.5, 0.75)] if len(g) else None,
            q_1000=[float(g['valve_q_1000_ml_min'].quantile(q)) for q in (0.25, 0.5, 0.75)] if len(g) else None,
            dnll_law=float(g['fuse_dnll_other_law'].median()) if len(g) else None)
    fw = use.groupby(['type', 'fwver']).agg(n=('sn', 'size'), cda=('cda_m2', 'median'),
                                            q0=('q0_ml_min', 'median'),
                                            net=('net_ml_dbar', 'median'), r2=('r2_w', 'median'))
    out['firmware'] = [dict(type=t, fwver=f, **{k: float(v) for k, v in r.items()})
                       for (t, f), r in fw.iterrows()]
    not_usable = fl[(fl['status'] == 'ok') & ~Usable(fl)]
    out['not_usable'] = dict(at_bound=int(not_usable['cda_at_bound'].astype(bool).sum()),
                             low_r2=int((not_usable['r2_w'] < USABLE_R2).sum()),
                             fwver=not_usable['fwver'].value_counts().head(8).to_dict())
    f = flow[(flow['support'] >= MIN_SUPPORT) & flow['sn'].isin(use['sn'])].merge(use[['sn', 'type']], on='sn')
    out['pump_profile'] = {t: g[g['kind'] == 'pump'].groupby('pres')['q_ml_min'].median().round(2).to_dict()
                           for t, g in f.groupby('type')}
    out['valve_profile'] = {t: g[g['kind'] == 'valve'].groupby('pres')['q_ml_min'].median().round(2).to_dict()
                            for t, g in f.groupby('type')}
    for s, df in sens.items():
        r = (df.set_index('sn')['cda_m2'] / fl.set_index('sn')['cda_m2']).reindex(use['sn'])
        rq = (df.set_index('sn')['q0_ml_min'] / fl.set_index('sn')['q0_ml_min']).reindex(use['sn'])
        out.setdefault('mass_sensitivity', {})[str(s)] = dict(cda_ratio=float(np.nanmedian(r)),
                                                               q0_ratio=float(np.nanmedian(rq)))
    return out


def WriteFigures(out_dir, sens_dirs=None, example=None):
    fl = pd.read_csv(os.path.join(out_dir, 'buoyancy_floats.csv'))
    flow = pd.read_csv(os.path.join(out_dir, 'buoyancy_flow.csv'))
    points = pd.read_csv(os.path.join(out_dir, 'buoyancy_points.csv.gz'),
                         usecols=['sn', 'branch', 'pres', 'w', 'w_model'])
    sens = {}
    for s, d in (sens_dirs or {}).items():
        sens[s] = pd.read_csv(os.path.join(d, 'buoyancy_floats.csv'))
    fdir = os.path.join(out_dir, 'figures')
    os.makedirs(fdir, exist_ok=True)

    use = fl[Usable(fl)]
    if example is None and len(use):
        example = int(use.sort_values('n_points', ascending=False).iloc[0]['sn'])
    if example is not None:
        FigExample(points, fl, example, os.path.join(fdir, 'fig_example.png'))
    FigPumpFlow(flow, fl, os.path.join(fdir, 'fig_pump_flow.png'))
    FigValve(flow, fl, os.path.join(fdir, 'fig_valve_flow.png'))
    FigStrip(fl, 'cda_m2', 'Drag Area, $C_dA$ [m$^2$]', os.path.join(fdir, 'fig_cda.png'), log=True)
    FigStrip(fl.assign(net=fl['kappa_ml_dbar'] - fl['v_gamma_w_ml_dbar']), 'net',
             r'Net Pressure Coefficient, $\kappa - V\gamma_w$ [mL dBar$^{-1}$]',
             os.path.join(fdir, 'fig_net_compress.png'), ref=(0, 'Neutral'))
    FigStrip(fl, 'q0_ml_min', 'Pump Flow at the Surface, $q_0$ [mL min$^{-1}$]',
             os.path.join(fdir, 'fig_q0.png'), ref=(19, 'Firmware PFR (19 mL min$^{-1}$)'),
             ref_config=True)
    FigStrip(fl, 'fuse_dbar', 'Hydraulic Fuse Pressure, $p_f$ [dBar]',
             os.path.join(fdir, 'fig_fuse.png'), mask=FuseUsable(fl))
    FigStrip(fl, 'air_ml', 'Residual Bladder Air at the Surface [mL]',
             os.path.join(fdir, 'fig_air.png'), ref=(0, 'None'))
    FigQuality(points, fl, os.path.join(fdir, 'fig_quality.png'))
    if sens:
        FigMassSensitivity(fl, sens, os.path.join(fdir, 'fig_mass_sensitivity.png'))
    summary = Summary(fl, flow, sens)
    summary['example'] = example
    with open(os.path.join(fdir, 'buoyancy_summary.json'), 'w', encoding='utf-8') as file:
        json.dump(summary, file, indent=1, default=str)
    return summary


if __name__ == '__main__':
    import sys
    out = sys.argv[1]
    sens, example = {}, None
    for arg in sys.argv[2:]:
        if arg.startswith('--example='):
            example = int(arg.split('=')[1])
            continue
        s, d = arg.split('=')
        sens[float(s)] = d
    print(json.dumps(WriteFigures(out, sens, example), indent=1, default=str)[:4000])
