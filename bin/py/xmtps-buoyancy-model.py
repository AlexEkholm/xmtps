#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fit a buoyancy model to ALAMO/ALTO floats: drag, hull compressibility and oil flow rates.

For each float, the vertical speeds measured in the fall (descent) and rise (ascent) time series
are balanced against the net buoyancy the float should have at that moment:

    1/2 rho C_dA w|w| = g rho dV        so        w|w| C_dA / (2 g) = dV

where dV is the float's excess volume (the volume it displaces minus its mass over the in-situ
seawater density). dV is referenced to the end of the drift, where the float is taken to be
neutral at the park pressure p_park:

    dV = dOil - kappa (p - p_park) + beta (T - T_park) + V_park (1 - rho_park / rho)
         + V_air (P_atm / (p + P_atm) - P_atm / (p_park + P_atm)) + d_dive

    dOil    oil moved to the external bladder since the reference: + q_p(p) t for each pump run,
            - the integral of q_v(p) over each valve opening (and the opposite for events between
            a descent sample and the reference). While the surface valve is still open during
            the descent, the oil it has yet to bleed is integrated along the fall path.
    kappa   hull compressibility, mL/dbar (ALTO: fixed at the lab's measurement; ALAMO: fitted)
    beta    hull thermal expansion, mL/K, fixed: V_park x HULL_ALPHA_PER_K (aluminium)
    V_park  displaced volume at park = assumed float mass / rho_park (NOMINAL_MASS_KG)
    V_air   residual air in the bladder, mL at the surface (held at 0: the air valve stays open
            after the surface phase; --fit-air fits it, Boyle's law)
    d_dive  per-dive offset from neutral at park (fitted, with a Gaussian prior SIGMA_DIVE_ML)

q_p(p), the pump flow rate, is piecewise linear in pressure (nodes every FLOW_NODE_STEP_DBAR with
a smoothness prior); a second fit uses the firmware's linear form q_p = q0 + q1 p. The valve has a
hydraulic fuse that restricts the flow above a pressure p_f set by its spring:
q_v = c_hi sqrt(p + dP0) below p_f, and c_lo sqrt(p + dP0) or a constant c_lo above it. p_f
(FUSE_GRID_DBAR) and the restricted law are chosen per float by a grid search. dV is linear in
all other parameters except C_dA, so for each trial C_dA and fuse the fit is a weighted linear
least-squares problem (with Huber reweighting); C_dA is found by a 1-D search on the negative
log-likelihood. Uncertainties come from a dive-block bootstrap at the chosen fuse.

The absolute scale of C_dA and of the flow rates is set only by the assumed float volume (the
seawater-density term), so both scale roughly in proportion to NOMINAL_MASS_KG.

Inputs (read-only):
    the aggregate JSON files that xmtps-aggregate-json.py writes, <json>/<sn>/<sn>_<key>.json
    (pandas 'split'): descent, ascent, pump, profile, mission; and the valve records, which
    xmtps-list2json.py does not parse (it skips VLV/vlv): either the listing directory
    (--list-dir) or a text file of the listings' vlv lines (--valve-file, may be gzipped).

Outputs (in --output): buoyancy_floats.csv (one row per float), buoyancy_flow.csv (flow-rate
curves per float), buoyancy_points.csv.gz (every velocity point with its fit), buoyancy_vacuum.csv
(case-vacuum cross-check), and with --plots the figures of bin/py/html/buoyancy-model.html.

Usage:
    xmtps-buoyancy-model.py --json-dir /data1/alamo/data/json --list-dir /data1/alamo/data/list
                            --output OUT [--floats 9303 11186 ...] [--bootstrap 30] [--plots]

Author: aekholm
"""

import argparse
import glob
import gzip
import logging
import os
import re
import sys

import numpy as np
import pandas as pd
import scipy.sparse as sp
import scipy.sparse.linalg as spla
from scipy.optimize import minimize_scalar


# ------------------------------------------------------------------------------------------------
# Constants and assumptions
# ------------------------------------------------------------------------------------------------

G = 9.80665                         # m/s^2
ALTO_MIN_SERIAL = 10000             # as ENERGY_ALTO_MIN_SERIAL in xmtps-aggregate-json.py
NOMINAL_MASS_KG = { 'ALAMO': 10.0, 'ALTO': 18.9 }   # ALTO: median ballasted weight in the lab's
                                    # chain-ballast records; ALAMO assumed. Sets the scale of C_dA
                                    # and flows; --lab-csv gives per-float weights
HULL_ALPHA_PER_K = 8.74e-5          # volumetric thermal expansion of the aluminium hull, 1/K: the
                                    # value the lab's ballasting uses (assumed bulk value, not measured)
KAPPA_REL = { 'ALTO': 2.42e-6 }     # hull compressibility per dBar relative to the volume: median
                                    # of the lab's chain-ballast measurements (ALTO). ALAMOs are
                                    # not measured, so their kappa is fitted freely
KAPPA_SD_FRAC = 0.002               # the lab's compressibility is measured precisely: held
                                    # fixed (0.2 % prior spread, far tighter than the data)
FIT_AIR = False                     # residual bladder air: the air valve stays open after the
                                    # surface phase, so the bladder carries no air on the dive
VAC_FILL = 655.35                   # vacuum not sent (inHg)
P_ATM_INHG = 29.92

DESCENT_MIN_PRES = 10.0             # dbar; shallower, surface waves
P_ATM_DBAR = 10.1325                # dbar, one atmosphere
FUSE_GRID_DBAR = (10, 15, 20, 25, 30, 35, 40, 50, 60, 80, 100, 120, 150, 200)   # hydraulic fuse pressures tried
VALVE_CURVE_DBAR = tuple(range(0, 200, 10)) + tuple(range(200, 2001, 100))
VALVE_PATH_STEP_S = 10.0            # s, step of the surface valve's bleed along the fall path
ASCENT_MIN_PRES = 10.0              # dbar; surface layer and near-surface waves
EVENT_SETTLE_S = 30.0               # s after a pump/valve run ends before a speed is used
MIN_INTERVAL_S = 20.0               # s, shortest sample pair used
MAX_SPEED = 1.0                     # m/s, implausible speeds are dropped
SIGMA_W = 0.004                     # m/s, speed noise for the weights
SIGMA_W_DP = 0.05                   # dbar, pressure noise (adds SIGMA_W_DP * sqrt(2) / dt)
SIGMA_WM_INIT = 0.01                # m/s, model noise (re-estimated per float)
SIGMA_WM_MIN = 0.002                # m/s, lower limit of the model noise
W_MIN = 0.02                        # m/s, speed floor in the noise mapping to volume
SIGMA_DIVE_ML = 2.0                 # mL, prior on the per-dive offset from neutral at park
SIGMA_DIVE_LOST_ML = 100.0          # mL, the same when the dive's park-to-profile valve is missing
FLOW_NODE_STEP_DBAR = 250.0         # pump/valve flow-rate nodes
SIGMA_SMOOTH = 0.02                 # mL/s, prior on the second difference of the flow nodes
HUBER_K = 2.5                       # Huber threshold, in standard deviations
MIN_DIVES = 10                      # dives with points needed for a fit
MIN_NODE_EVENTS = 5                 # events within one node step for a node to be reported
PROFILE_EXTRAP_DBAR = 20.0          # a point may be this far outside the profile's range
VALVE_DP0_DBAR = 2.0                # valve law q_v = c sqrt(p + VALVE_DP0_DBAR): the case is at
                                    # about 0.8 atm (6 inHg vacuum), so dP = p + 10 - 8 dbar

CDA_BOUNDS = (1e-4, 0.2)            # m^2, search range
MASS_SCALE = 1.0                    # --mass-scale, for the sensitivity to NOMINAL_MASS_KG


logger = logging.getLogger('buoyancy')


# ------------------------------------------------------------------------------------------------
# Reading
# ------------------------------------------------------------------------------------------------

def ReadAggregate(json_dir, sn, key):
    """Return one aggregate DataFrame of a float (None if the file is missing)."""

    fpath = os.path.join(json_dir, str(sn), '{}_{}.json'.format(sn, key))
    if not os.path.isfile(fpath):
        return None
    return pd.read_json(fpath, orient='split', convert_dates=False)


def ReadValveLines(lines):
    """Parse listing 'vlv' lines into a DataFrame (sn, dive, order, pres, time, vac)."""

    rows = []
    for ln in lines:
        x = ln.split()
        if len(x) < 9 or x[0] != 'vlv':
            continue
        try:
            rows.append((int(x[1]), int(x[2]), int(x[3]), int(x[4]), float(x[5]), float(x[6]),
                         float(x[8])))
        except ValueError:
            continue
    df = pd.DataFrame(rows, columns=['sn', 'dive', 'idx', 'sam', 'pres', 'time', 'vac'])
    df = df.drop_duplicates(['sn', 'dive', 'idx', 'sam'])
    return df.sort_values(['sn', 'dive', 'idx', 'sam'], kind='stable').reset_index(drop=True)


def ReadValves(args, serials):
    """Read the valve records of the floats, from --valve-file or the listings in --list-dir."""

    if args.valve_file:
        opener = gzip.open if args.valve_file.endswith('.gz') else open
        with opener(args.valve_file, 'rt', encoding='latin-1') as file:
            return ReadValveLines(file)

    lines = []
    for sn in serials:
        for fpath in sorted(glob.glob(os.path.join(args.list_dir, str(sn), 'lis*-*.txt'))):
            if not re.search(r'lis\d+-\d{4}\.txt$', fpath):
                continue                # startup dives and archived startup sessions
            with open(fpath, encoding='latin-1') as file:
                lines.extend(ln for ln in file if ln.startswith('vlv '))
    return ReadValveLines(lines)


def MostCommon(series):
    series = series.dropna()
    series = series[series.astype(str) != '']
    return series.mode().iloc[0] if len(series) else None


def FloatInfo(sn, mission):
    """Type (ALAMO/ALTO), CTD and firmware version."""

    info = dict(sn=sn, type='ALTO' if sn >= ALTO_MIN_SERIAL else 'ALAMO', ctd='', fwver='')
    if mission is not None and len(mission):
        info['ctd'] = str(MostCommon(mission['ctdtype']) or '') if 'ctdtype' in mission else ''
        info['fwver'] = str(MostCommon(mission['fwver']) or '') if 'fwver' in mission else ''
    return info


# ------------------------------------------------------------------------------------------------
# Velocity points
# ------------------------------------------------------------------------------------------------

class Event:
    """A pump run or valve opening: pressure, duration, and when it started (None if unknown)."""

    def __init__(self, kind, pres, dur, phase, t0=None, vac=np.nan, volt=np.nan, curr=np.nan):
        self.kind, self.pres, self.dur, self.phase, self.t0 = kind, pres, dur, phase, t0
        self.vac, self.volt, self.curr = vac, volt, curr

    def ended_before(self, t):
        return self.t0 is not None and self.t0 + self.dur + EVENT_SETTLE_S <= t

    def overlaps(self, t_a, t_b):
        """True if the run, or its settling time, overlaps the interval (t_a, t_b)."""
        if self.t0 is None:
            return False
        return self.t0 < t_b and self.t0 + self.dur + EVENT_SETTLE_S > t_a


def Series(df):
    df = df.dropna(subset=['pres', 'dt_epoch'])
    return (df['dt_epoch'].to_numpy(float), df['pres'].to_numpy(float),
            df['phase_code'].to_numpy(int))


def NearestIndex(values, value, tol):
    if not len(values):
        return None
    i = int(np.argmin(np.abs(values - value)))
    return i if abs(values[i] - value) <= tol else None


def ClassifyEvents(t_d, p_d, ph_d, t_a, p_a, ph_a, pumps, valves, p_park):
    """Split a dive's pump runs and valve openings into descent, drift/seek and ascent events.

    Pump and valve records carry no time and, in the listings, no phase code. They are in the
    order the float made them, and each has the pressure where it ran, so they are placed by
    matching that pressure to the fall and rise series:
      - ascent pumps: from the first pump within 10 dbar of the start-of-ascent pressure after
        which no pump is more than 20 dbar deeper; timed by the rise series; a final pump
        shallower than ASCENT_MIN_PRES is the surface pump. When the profile starts at the park
        pressure, seek pumps at park can be taken for the start-of-ascent pump (a known limit);
      - the profile valve: the last valve within 20 dbar of the park pressure, when the rise
        series starts with the descent to profile (phase 6; timed at that sample) or the profile
        starts deeper than park (timed before the rise series);
      - the surface valve: the first valve, shallower than 5 dbar; timed at the first fall sample;
      - the first descent pump: the first remaining pump matched to a phase-2 fall sample;
      - everything else happened during the seek to park or the drift: before the reference
        (end of drift), after the usable part of the descent, time unknown.
    """

    events = []
    pumps = list(pumps)
    valves = list(valves)

    # Ascent pumps
    asc_pumps = []
    i7 = np.where(ph_a == 7)[0]
    t7 = t_a[i7[0]] if len(i7) else (t_a[0] if len(t_a) else None)
    p7 = p_a[i7[0]] if len(i7) else (p_a[0] if len(p_a) else None)
    if t7 is not None and pumps:
        pres = np.array([pump['pres'] for pump in pumps])
        for j in range(len(pumps)):
            if abs(pres[j] - p7) <= 10.0 and np.all(pres[j:] <= p7 + 20.0):
                asc_pumps = pumps[j:]
                pumps = pumps[:j]
                break

    if asc_pumps:
        rising = t_a >= t7
        tr, pr = t_a[rising], p_a[rising]
        order = np.argsort(pr)
        for n, pump in enumerate(asc_pumps):
            if n == 0:
                t0, phase = t7, 'asc_start'
            elif pump['pres'] < ASCENT_MIN_PRES and n == len(asc_pumps) - 1:
                t0, phase = (tr[-1] if len(tr) else None), 'surface'
            else:
                t0, phase = float(np.interp(pump['pres'], pr[order], tr[order])), 'asc'
            events.append(Event('pump', pump['pres'], pump['time'], phase, t0, pump['vac'],
                                pump['volt'], pump['curr']))

    # Profile valve
    i6 = np.where(ph_a == 6)[0]
    deeper = len(i6) > 0 or (p7 is not None and p7 > p_park + 30.0)
    profile_valve = False
    if deeper:
        for n in range(len(valves) - 1, -1, -1):
            if abs(valves[n]['pres'] - p_park) <= 20.0:
                valve = valves.pop(n)
                t0 = t_a[i6[0]] if len(i6) else t_a[0] - valve['time'] - EVENT_SETTLE_S - 1.0
                events.append(Event('valve', valve['pres'], valve['time'], 'asc_valve', t0,
                                    valve['vac']))
                profile_valve = True
                break

    # Surface valve
    if valves and valves[0]['pres'] < 5.0 and len(t_d):
        valve = valves.pop(0)
        events.append(Event('valve', valve['pres'], valve['time'], 'surface_valve', t_d[0],
                            valve['vac']))

    # First descent pump
    i2 = np.where(ph_d == 2)[0]
    if pumps and len(i2):
        n = NearestIndex(np.array([pump['pres'] for pump in pumps]), p_d[i2[0]], 1.0)
        if n is not None:
            pump = pumps.pop(n)
            events.append(Event('pump', pump['pres'], pump['time'], 'desc', t_d[i2[0]],
                                pump['vac'], pump['volt'], pump['curr']))

    # Seek and drift
    for pump in pumps:
        events.append(Event('pump', pump['pres'], pump['time'], 'drift', None, pump['vac'],
                            pump['volt'], pump['curr']))
    for valve in valves:
        events.append(Event('valve', valve['pres'], valve['time'], 'drift', None, valve['vac']))

    # The reference (neutral at park) is lost for the ascent when the float valved its way
    # deeper but the valve record is missing
    ref_ok = not deeper or profile_valve
    return events, ref_ok


def ProfileInterpolators(prof):
    """Density (kg/m^3) and temperature vs pressure from one dive's profile."""

    prof = prof.dropna(subset=['pres', 'dens', 'temp'])
    prof = prof[(prof['dens'] > 0) & (prof['dens'] < 80)]
    if len(prof) < 10:
        return None
    prof = prof.groupby('pres', as_index=False)[['dens', 'temp']].mean().sort_values('pres')
    p = prof['pres'].to_numpy(float)
    rho = prof['dens'].to_numpy(float) + 1000.0
    temp = prof['temp'].to_numpy(float)
    p_lo, p_hi = p[0] - PROFILE_EXTRAP_DBAR, p[-1] + PROFILE_EXTRAP_DBAR

    def at(pres):
        pres = np.asarray(pres, float)
        ok = (pres >= p_lo) & (pres <= p_hi)
        return np.interp(pres, p, rho), np.interp(pres, p, temp), ok

    return at


def EventCoef(ev, sign):
    """A point's term for one event: ('pump', pres, dur, sign) or, for a valve opening, its
    pressure path ('valve', pres[], dt[], sign) (one step at the opening's pressure)."""
    if ev.kind == 'pump':
        return ('pump', ev.pres, ev.dur, sign)
    return ('valve', np.array([ev.pres]), np.array([float(ev.dur)]), sign)


def DivePoints(sn, dive, desc, asc, pumps, valves, prof_at, p_park=None):
    """Velocity points of one dive, each with the oil events that set its buoyancy.

    p_park is the drift pressure (end of drift); the first rise sample when not known.
    """

    t_d, p_d, ph_d = Series(desc)
    t_a, p_a, ph_a = Series(asc)
    if len(t_a) < 3:
        return [], []
    if p_park is None or not np.isfinite(p_park):
        p_park = p_a[0]

    events, ref_ok = ClassifyEvents(t_d, p_d, ph_d, t_a, p_a, ph_a, pumps, valves, p_park)

    rho_park, t_park, ok = prof_at([p_park])
    if not ok[0]:
        return [], events
    rho_park, t_park = float(rho_park[0]), float(t_park[0])

    points = []

    def add(branch, t_a_, t_b_, p_a_, p_b_, coefs):
        dt = t_b_ - t_a_
        if dt < MIN_INTERVAL_S:
            return
        p_mid = 0.5 * (p_a_ + p_b_)
        rho, temp, ok_ = prof_at([p_mid])
        if not ok_[0]:
            return
        rho, temp = float(rho[0]), float(temp[0])
        w = -(p_b_ - p_a_) * 1e4 / (rho * G) / dt          # m/s, positive up
        if abs(w) > MAX_SPEED:
            return
        sigma_w = np.hypot(SIGMA_W, SIGMA_W_DP * np.sqrt(2) * 1e4 / (rho * G) / dt)
        points.append(dict(sn=sn, dive=dive, branch=branch, t=0.5 * (t_a_ + t_b_), dt=dt,
                           pres=p_mid, rho=rho, temp=temp, w=w, sigma_w=sigma_w,
                           p_park=p_park, rho_park=rho_park, t_park=t_park, ref_ok=ref_ok,
                           coefs=coefs))

    # Descent: from the first usable sample to the first seek/drift sample or turnaround
    cut = len(t_d)
    for n in range(1, len(t_d)):
        if ph_d[n] in (3, 4, 5, 11) or p_d[n] < p_d[n - 1] - 1.0:
            cut = n
            break
    sv = next((ev for ev in events if ev.phase == 'surface_valve'), None)
    for n in range(cut - 1):
        ta, tb, pa, pb = t_d[n], t_d[n + 1], p_d[n], p_d[n + 1]
        if min(pa, pb) < DESCENT_MIN_PRES:
            continue
        if any(ev.overlaps(ta, tb) for ev in events if ev.phase == 'desc'):
            continue
        # Oil relative to the end of drift: every descent/drift event after this interval is undone
        coefs = []
        for ev in events:
            if ev.phase == 'drift' or (ev.phase == 'desc' and not ev.ended_before(ta)):
                coefs.append(EventCoef(ev, -1.0 if ev.kind == 'pump' else 1.0))
        # Surface valve still open at the interval's midpoint: the oil it has yet to bleed is
        # still outside, integrated along the fall path (fuse regime changes with pressure)
        tm = 0.5 * (ta + tb)
        if sv is not None and sv.t0 is not None and sv.t0 + sv.dur > tm:
            t_edges = np.arange(max(tm, sv.t0), sv.t0 + sv.dur, VALVE_PATH_STEP_S)
            t_edges = np.append(t_edges, sv.t0 + sv.dur)
            if len(t_edges) >= 2:
                t_mid = 0.5 * (t_edges[1:] + t_edges[:-1])
                coefs.append(('valve', np.interp(t_mid, t_d, p_d), np.diff(t_edges), 1.0))
        add('descent', ta, tb, pa, pb, coefs)

    # Ascent series: descent to profile, then the ascent proper
    asc_events = [ev for ev in events if ev.phase in ('asc_valve', 'asc_start', 'asc')]
    for n in range(len(t_a) - 1):
        ta, tb, pa, pb = t_a[n], t_a[n + 1], p_a[n], p_a[n + 1]
        if ph_a[n + 1] in (8, 14) or min(pa, pb) < ASCENT_MIN_PRES:
            continue
        if any(ev.overlaps(ta, tb) for ev in asc_events):
            continue
        if any(ev.t0 is None for ev in asc_events):
            continue
        coefs = [EventCoef(ev, 1.0 if ev.kind == 'pump' else -1.0)
                 for ev in asc_events if ev.ended_before(ta)]
        add('profile_descent' if pb > pa else 'ascent', ta, tb, pa, pb, coefs)

    return points, events


# ------------------------------------------------------------------------------------------------
# Fitting
# ------------------------------------------------------------------------------------------------

def NodeWeights(pres, nodes):
    """Linear-interpolation weights of a pressure on the node grid (clamped at the ends)."""

    w = np.zeros(len(nodes))
    if pres <= nodes[0]:
        w[0] = 1.0
    elif pres >= nodes[-1]:
        w[-1] = 1.0
    else:
        k = int(np.searchsorted(nodes, pres) - 1)
        f = (pres - nodes[k]) / (nodes[k + 1] - nodes[k])
        w[k], w[k + 1] = 1.0 - f, f
    return w


class Design:
    """The linear model dV = A theta + b of one float's points.

    Columns: kappa, pump flow (nodes, or q0 and q1 in the linear form), the valve's flow
    coefficients below and above the fuse pressure (c_hi, c_lo), the residual air in the bladder
    (mL at the surface), and one offset per dive. Pump flows are in mL/s, valve coefficients in
    mL/s/sqrt(dBar) (c_lo in mL/s for the constant law), kappa in mL/dBar, offsets and dV in mL.
    The valve columns depend on the fuse pressure and the restricted law: set_fuse() rebuilds
    them.
    """

    def __init__(self, pts, mass_kg, pump_form='nodes', kappa_prior=None, fit_air=False):
        """kappa_prior: (mean, sd) in mL/dBar, or None for a free kappa. fit_air: fit the
        residual bladder air; otherwise it is held at 0."""
        self.pump_form = pump_form
        self.sigma_wm = SIGMA_WM_INIT
        # One offset per dive; the ascent of a dive whose park-to-profile valve is missing gets
        # its own, loosely constrained offset (negative key)
        def key(p):
            return p['dive'] if p['ref_ok'] or p['branch'] == 'descent' else -p['dive']
        for p in pts:
            p['key'] = key(p)
        self.dives = np.array(sorted(set(p['key'] for p in pts)))
        dive_col = { d: n for n, d in enumerate(self.dives) }

        pump_p = [c[1] for p in pts for c in p['coefs'] if c[0] == 'pump']
        step = FLOW_NODE_STEP_DBAR
        self.pump_nodes = np.arange(0.0, (np.ceil(max(pump_p + [step]) / step) + 1) * step, step)
        n_pump = 2 if pump_form == 'linear' else len(self.pump_nodes)
        self.i_kappa = 0
        self.i_pump = slice(1, 1 + n_pump)
        self.i_valve = slice(1 + n_pump, 3 + n_pump)            # c_hi, c_lo
        self.i_air = 3 + n_pump
        self.i_dive = slice(4 + n_pump, 4 + n_pump + len(self.dives))
        n_col = self.i_dive.stop

        n = len(pts)
        Af = np.zeros((n, 1 + n_pump))             # kappa and pump columns (dense, narrow)
        air = np.zeros(n)
        b = np.zeros(n)
        y = np.zeros(n)
        s_w = np.zeros(n)
        vi, vp, vw = [], [], []                     # valve path steps: point, pressure, sign * dt
        for i, p in enumerate(pts):
            v_park = mass_kg / p['rho_park'] * 1e6                       # mL
            Af[i, self.i_kappa] = -(p['pres'] - p['p_park'])
            air[i] = P_ATM_DBAR / (p['pres'] + P_ATM_DBAR) - P_ATM_DBAR / (p['p_park'] + P_ATM_DBAR)
            b[i] = v_park * (HULL_ALPHA_PER_K * (p['temp'] - p['t_park'])
                             + 1.0 - p['rho_park'] / p['rho'])
            for c in p['coefs']:
                if c[0] == 'pump':
                    _, pres, dur, sign = c
                    if pump_form == 'linear':
                        Af[i, self.i_pump.start] += sign * dur
                        Af[i, self.i_pump.start + 1] += sign * dur * pres / 1000.0
                    else:
                        Af[i, self.i_pump] += sign * dur * NodeWeights(pres, self.pump_nodes)
                else:
                    _, pres, dts, sign = c
                    vi.append(np.full(len(pres), i))
                    vp.append(pres)
                    vw.append(sign * dts)
            y[i] = p['w'] * abs(p['w'])
            s_w[i] = p['sigma_w']
        self.vi = np.concatenate(vi) if vi else np.zeros(0, int)
        self.vp = np.concatenate(vp) if vp else np.zeros(0)
        self.vw = np.concatenate(vw) if vw else np.zeros(0)
        self.n = n
        point_dive = np.array([dive_col[p['key']] for p in pts])
        self.Af = sp.csr_matrix(Af)
        self.Aair = sp.csr_matrix(air[:, None])
        self.Ad = sp.csr_matrix((np.ones(n), (np.arange(n), point_dive)), shape=(n, len(self.dives)))
        self.b, self.y, self.s_w = b, y, s_w
        self.w = np.array([p['w'] for p in pts])
        self.point_dive = point_dive

        # Priors: per-dive offsets toward 0; smooth pump nodes (second differences); kappa toward
        # its measured value; residual air at 0 unless fitted. Each row r has a target m: r theta ~ m
        rows, means = [], []
        if kappa_prior is not None:
            r = np.zeros(n_col)
            r[self.i_kappa] = 1.0 / kappa_prior[1]
            rows.append(r)
            means.append(kappa_prior[0] / kappa_prior[1])
        if not fit_air:
            r = np.zeros(n_col)
            r[self.i_air] = 1.0 / 1e-3                                  # held at 0 (1 uL)
            rows.append(r)
            means.append(0.0)
        for k, dive in enumerate(self.dives):
            r = np.zeros(n_col)
            r[self.i_dive.start + k] = 1.0 / (SIGMA_DIVE_LOST_ML if dive < 0 else SIGMA_DIVE_ML)
            rows.append(r)
        if pump_form != 'linear':
            for k in range(1, n_pump - 1):
                r = np.zeros(n_col)
                sl = self.i_pump
                r[sl.start + k - 1], r[sl.start + k], r[sl.start + k + 1] = 1, -2, 1
                rows.append(r / SIGMA_SMOOTH)
        # Every column gets a very weak ridge so the system stays regular (unsupported columns)
        for k in range(n_col):
            r = np.zeros(n_col)
            r[k] = 1e-4
            rows.append(r)
        self.P = sp.csr_matrix(np.array(rows))
        self.Pm = np.concatenate([means, np.zeros(len(rows) - len(means))])
        self.PtP = (self.P.T @ self.P).tocsc()
        self.Ptm = self.P.T @ self.Pm
        self.set_fuse(FUSE_GRID_DBAR[len(FUSE_GRID_DBAR) // 2], 'sqrt')

    def valve_columns(self, p_fuse, law):
        """The (n, 2) valve columns for a fuse pressure and restricted law ('sqrt', 'const').

        Below the fuse: q = c_hi sqrt(p + dP0). Above: q = c_lo sqrt(p + dP0) ('sqrt') or c_lo
        ('const').
        """
        cols = np.zeros((self.n, 2))
        if len(self.vi):
            root = np.sqrt(np.maximum(self.vp, 0) + VALVE_DP0_DBAR)
            below = self.vp < p_fuse
            cols[:, 0] = np.bincount(self.vi, weights=self.vw * root * below, minlength=self.n)
            g = root if law == 'sqrt' else np.ones_like(root)
            cols[:, 1] = np.bincount(self.vi, weights=self.vw * g * ~below, minlength=self.n)
        return cols

    def set_fuse(self, p_fuse, law):
        self.p_fuse, self.law = p_fuse, law
        self.A = sp.hstack([self.Af, sp.csr_matrix(self.valve_columns(p_fuse, law)), self.Aair,
                            self.Ad], format='csr')

    def valve_flow(self, theta, pres):
        """Valve flow (mL/s) at the given pressures for fitted coefficients theta."""
        pres = np.asarray(pres, float)
        c_hi, c_lo = theta[self.i_valve]
        root = np.sqrt(np.maximum(pres, 0) + VALVE_DP0_DBAR)
        lo = c_lo * root if self.law == 'sqrt' else c_lo * np.ones_like(root)
        return np.where(pres < self.p_fuse, c_hi * root, lo)

    def solve(self, cda, robust=True, rows=None):
        """Weighted (Huber) least squares at fixed C_dA. Returns theta, sigma, residuals, NLL."""

        s = cda * 1e6 / (2 * G)                                          # mL per (m/s)^2
        A, b, y = self.A, self.b, self.y
        if rows is not None:
            A, b, y = A[rows], b[rows], y[rows]
            s_w = self.s_w[rows]
            w = self.w[rows]
        else:
            s_w, w = self.s_w, self.w
        # Noise in speed units, mapped to volume: d(w|w|) = 2 |w| dw (|w| floored at W_MIN), so
        # the likelihood is that of the speeds and a vanishing C_dA cannot fit them for free
        sigma = 2 * s * np.maximum(np.abs(w), W_MIN) * np.hypot(s_w, self.sigma_wm)
        target = s * y - b
        hub = np.ones(len(y))
        At = A.T.tocsr()
        for _ in range(4 if robust else 1):
            wt2 = hub / sigma ** 2
            N = (At @ sp.diags(wt2) @ A).tocsc() + self.PtP
            theta = spla.spsolve(N, At @ (wt2 * target) + self.Ptm)
            z = (target - A @ theta) / sigma
            hub = np.where(np.abs(z) <= HUBER_K, 1.0, HUBER_K / np.maximum(np.abs(z), 1e-12))
        # Huber negative log-likelihood (Gaussian core, linear tails), with the scale terms
        az = np.abs(z)
        rho = np.where(az <= HUBER_K, 0.5 * z * z, HUBER_K * az - 0.5 * HUBER_K ** 2)
        prior = 0.5 * np.sum((self.P @ theta - self.Pm) ** 2)
        # Scale term in speed units (the volume-unit sigma carries a log C_dA that would favour
        # C_dA -> 0); constant during the C_dA search
        nll = np.sum(rho) + np.sum(np.log(np.hypot(s_w, self.sigma_wm))) + prior
        return theta, sigma, z, nll

    def fit(self, rows=None, noise_iter=2, around=None):
        """C_dA by a bounded 1-D search on log C_dA; returns (cda, theta, sigma, z, at_bound, nll).

        The model noise sigma_wm (m/s, added to each speed's measurement noise) is re-estimated
        from the residuals' robust spread (noise_iter times) so the weights match the float's
        real scatter. With around (a C_dA), only a factor of 3 either side is searched.
        """

        s_w = self.s_w if rows is None else self.s_w[rows]
        for it in range(noise_iter + 1):
            cda, theta, sigma, z, at_bound, nll = self._fit_cda(rows, around)
            if it == noise_iter:
                break
            # z is in units of the total speed noise; scale it and remove the measurement part
            tot = 1.4826 * np.median(np.abs(z) * np.hypot(s_w, self.sigma_wm))
            meas = np.median(s_w)
            self.sigma_wm = float(max(np.sqrt(max(tot ** 2 - meas ** 2, 0)), SIGMA_WM_MIN))
        return cda, theta, sigma, z, at_bound, nll

    def _fit_cda(self, rows=None, around=None):
        f = lambda lc: self.solve(np.exp(lc), rows=rows)[3]
        if around is None:
            grid = np.linspace(np.log(CDA_BOUNDS[0]), np.log(CDA_BOUNDS[1]), 25)
        else:
            lo_b = max(np.log(around) - np.log(3), np.log(CDA_BOUNDS[0]))
            hi_b = min(np.log(around) + np.log(3), np.log(CDA_BOUNDS[1]))
            grid = np.linspace(lo_b, hi_b, 7)
        vals = [f(g) for g in grid]
        k = int(np.argmin(vals))
        lo, hi = grid[max(k - 1, 0)], grid[min(k + 1, len(grid) - 1)]
        res = minimize_scalar(f, bounds=(lo, hi), method='bounded', options=dict(xatol=1e-3))
        cda = float(np.exp(res.x))
        theta, sigma, z, nll = self.solve(cda, rows=rows)
        at_bound = bool(np.isclose(cda, CDA_BOUNDS[0], rtol=0.05) or np.isclose(cda, CDA_BOUNDS[1], rtol=0.05))
        return cda, theta, sigma, z, at_bound, nll

    def fit_fuse(self):
        """Grid search of the fuse pressure and the restricted law (C_dA refitted at each).

        Returns the best (p_fuse, law), and the profile of the negative log-likelihood as
        {law: [(p_fuse, nll), ...]}. Leaves the design set to the best fuse.
        """
        cda0 = self.fit()[0]                     # noise level and starting C_dA at the default
        profile = {}
        best = (np.inf, None, None)
        for law in ('sqrt', 'const'):
            profile[law] = []
            for p_fuse in FUSE_GRID_DBAR:
                self.set_fuse(p_fuse, law)
                nll = self._fit_cda(around=cda0)[5]
                profile[law].append((p_fuse, nll))
                if nll < best[0]:
                    best = (nll, p_fuse, law)
        self.set_fuse(best[1], best[2])
        return best[1], best[2], profile


def ModelSpeed(cda, dv_ml):
    """Speed (m/s, positive up) for an excess volume (mL) and C_dA (m^2)."""
    return np.sign(dv_ml) * np.sqrt(2 * G * np.abs(dv_ml) * 1e-6 / cda)


def FitFloat(sn, pts, info, n_boot, rng):
    """Fit one float; returns the float summary row and its flow curves.

    The fuse pressure and restricted valve law are chosen by a grid search on the full data;
    the bootstrap refits everything else at that fuse.
    """

    lab = LAB.get(sn, {})
    mass_src = 'lab' if lab.get('total_g') else 'nominal'
    mass = (lab['total_g'] / 1000.0 if lab.get('total_g') else NOMINAL_MASS_KG[info['type']]) \
        * MASS_SCALE
    # Measured hull compressibility (per float, else the type's median), as a prior on kappa
    kappa_rel = lab.get('compress') or KAPPA_REL.get(info['type'])
    kappa_prior = None
    if kappa_rel:
        v_ml = mass / 1.035 * 1e3                 # mL (mass in kg)
        kappa_prior = (kappa_rel * v_ml, KAPPA_SD_FRAC * kappa_rel * v_ml)
    row = dict(info)
    row.update(mass_kg=mass, mass_source=mass_src, n_points=len(pts),
               n_dives=len(set(p['dive'] for p in pts)), status='ok',
               kappa_prior_ml_dbar=kappa_prior[0] if kappa_prior else np.nan,
               kappa_source=('lab' if lab.get('compress') else 'type') if kappa_prior else 'free')

    des = Design(pts, mass, kappa_prior=kappa_prior, fit_air=FIT_AIR)
    p_fuse, law, profile = des.fit_fuse()
    cda, theta, sigma, z, at_bound, nll = des.fit(noise_iter=1)
    dv = des.A @ theta + des.b
    w_mod = ModelSpeed(cda, dv)
    res_w = des.w - w_mod

    # Fuse profile: pressures within 1.92 of the best NLL (about 95 % for one parameter), and how
    # much worse the other restricted law is at its own best fuse pressure
    best_nll = min(v for prof in profile.values() for _, v in prof)
    ok_p = [pf for pf, v in profile[law] if v - best_nll <= 1.92]
    other = 'const' if law == 'sqrt' else 'sqrt'
    d_other = min(v for _, v in profile[other]) - best_nll

    # Linear pump form (q0 + q1 p), same fuse
    des_lin = Design(pts, mass, pump_form='linear', kappa_prior=kappa_prior, fit_air=FIT_AIR)
    des_lin.set_fuse(p_fuse, law)
    des_lin.sigma_wm = des.sigma_wm
    cda_lin, theta_lin = des_lin.fit(noise_iter=0)[:2]

    # Dive-block bootstrap at the chosen fuse
    boot = []
    by_dive = [np.where(des.point_dive == k)[0] for k in range(len(des.dives))]
    for _ in range(n_boot):
        pick = rng.integers(0, len(by_dive), len(by_dive))
        rows = np.concatenate([by_dive[k] for k in pick])
        c_b, th_b = des.fit(rows=rows, noise_iter=0, around=cda)[:2]
        c_l, th_l = des_lin.fit(rows=rows, noise_iter=0, around=cda_lin)[:2]
        boot.append((c_b, th_b[des.i_kappa], th_b[des.i_pump], th_b[des.i_valve], c_l,
                     th_l[des_lin.i_pump], th_b[des.i_air], th_b))

    def bstd(vals):
        vals = np.array(vals, float)
        return float(np.nanstd(vals, axis=0, ddof=1)) if len(vals) > 1 else np.nan

    # Valve events in the fit, and the shallowest pressure a valve path reaches (the fuse can only
    # be seen if the valve bleeds on both sides of it)
    n_valve_events = len(set((p['dive'], float(c[1][0]), float(c[2].sum()))
                             for p in pts for c in p['coefs'] if c[0] == 'valve'))
    n_sv_points = int(sum(any(c[0] == 'valve' and len(c[1]) > 1 for c in p['coefs']) for p in pts))
    c_hi, c_lo = theta[des.i_valve]
    row.update(
        cda_m2=cda, cda_sd=bstd([b[0] for b in boot]), cda_at_bound=at_bound,
        log_cda_sd=bstd([np.log(b[0]) for b in boot]),
        kappa_ml_dbar=float(theta[des.i_kappa]), kappa_sd=bstd([b[1] for b in boot]),
        # Seawater compressibility of the displaced volume over the same points (V gamma_w),
        # and the net pressure coefficient of the buoyancy, kappa - V gamma_w, which the
        # data fix independently of the assumed mass
        v_gamma_w_ml_dbar=float(np.polyfit(
            [p['pres'] - p['p_park'] for p in pts],
            [mass / p['rho_park'] * 1e6 * (1 - p['rho_park'] / p['rho']) for p in pts], 1)[0]),
        beta_ml_k=mass / 1030.0 * 1e6 * HULL_ALPHA_PER_K,
        air_ml=float(theta[des.i_air]), air_sd=bstd([b[6] for b in boot]),
        q0_ml_min=float(theta_lin[des_lin.i_pump][0] * 60),
        q0_sd=bstd([b[5][0] * 60 for b in boot]),
        q1_ml_min_per_kdbar=float(theta_lin[des_lin.i_pump][1] * 60),
        q1_sd=bstd([b[5][1] * 60 for b in boot]),
        cda_lin_m2=cda_lin,
        fuse_dbar=p_fuse, fuse_law=law,
        fuse_lo_dbar=min(ok_p), fuse_hi_dbar=max(ok_p),
        fuse_constrained=bool(min(ok_p) > FUSE_GRID_DBAR[0] or max(ok_p) < FUSE_GRID_DBAR[-1]),
        fuse_dnll_other_law=float(d_other),
        valve_c_hi=float(c_hi * 60), valve_c_hi_sd=bstd([b[3][0] * 60 for b in boot]),
        valve_c_lo=float(c_lo * 60), valve_c_lo_sd=bstd([b[3][1] * 60 for b in boot]),
        valve_q_surface_ml_min=float(des.valve_flow(theta, [0.0])[0] * 60),
        valve_q_1000_ml_min=float(des.valve_flow(theta, [1000.0])[0] * 60),
        valve_q_1000_sd=bstd([des.valve_flow(b[7], [1000.0])[0] * 60 for b in boot]),
        n_valve_events=n_valve_events, n_surface_valve_points=n_sv_points,
        valve_pres_median=float(np.median(des.vp)) if len(des.vp) else np.nan,
        sigma_wm_cm_s=des.sigma_wm * 100,
        rms_w_cm_s=float(np.sqrt(np.mean(res_w ** 2)) * 100),
        mad_w_cm_s=float(np.median(np.abs(res_w)) * 100 * 1.4826),
        r2_w=float(1 - np.sum(res_w ** 2) / np.sum((des.w - des.w.mean()) ** 2)),
        frac_outliers=float(np.mean(np.abs(z) > HUBER_K)),
        dive_offset_sd_ml=float(np.std(theta[des.i_dive][des.dives > 0])),
        frac_ref_lost=float(np.mean([not p['ref_ok'] for p in pts])),
        n_descent=int(sum(p['branch'] == 'descent' for p in pts)),
        n_ascent=int(sum(p['branch'] == 'ascent' for p in pts)),
        n_profile_descent=int(sum(p['branch'] == 'profile_descent' for p in pts)),
    )
    for law_k, prof in profile.items():
        for pf, v in prof:
            row['nll_{}_{}'.format(law_k, pf)] = v - best_nll

    # Flow curves: pump nodes with support counts; valve law on a pressure grid, with the
    # number of valve-path seconds within 25 dBar of each pressure as its support
    flows = []
    ev = set((p['dive'], c[1], c[2]) for p in pts for c in p['coefs'] if c[0] == 'pump')
    ev_p = np.array([e[1] for e in ev])
    for k, node in enumerate(des.pump_nodes):
        support = int(np.sum(np.abs(ev_p - node) < FLOW_NODE_STEP_DBAR)) if len(ev_p) else 0
        flows.append(dict(sn=sn, kind='pump', pres=node, q_ml_min=float(theta[des.i_pump][k] * 60),
                          q_sd=bstd([b[2][k] * 60 for b in boot]), support=support))
    v_seen = des.vp[np.abs(des.vw) > 0]
    for pres in VALVE_CURVE_DBAR:
        support = int(np.sum(np.abs(v_seen - pres) < 25.0))
        flows.append(dict(sn=sn, kind='valve', pres=float(pres),
                          q_ml_min=float(des.valve_flow(theta, [pres])[0] * 60),
                          q_sd=bstd([des.valve_flow(b[7], [pres])[0] * 60 for b in boot]),
                          support=support))

    for p, dvi, wm, zi in zip(pts, dv, w_mod, z):
        p['dv_ml'], p['w_model'], p['z'] = dvi, wm, zi
    return row, flows


def VacuumCheck(sn, events_by_dive, prof_by_dive, flows, row):
    """Oil volume implied by the case vacuum between consecutive ascent pumps.

    With the case air at absolute pressure P (inHg, P_ATM_INHG - vacuum) and temperature T
    (the water temperature at the pump's pressure), P V / T is constant, so moving dV of oil
    out of the case grows the air volume V_air by dV:  f = (P0 / T0) / (P1 / T1) - 1 = dV / V_air.
    """

    fl = pd.DataFrame(flows)
    fl = fl[(fl['kind'] == 'pump')]
    if not len(fl):
        return []
    rows = []
    for dive, events in events_by_dive.items():
        prof_at = prof_by_dive.get(dive)
        if prof_at is None:
            continue
        asc = [ev for ev in events if ev.kind == 'pump' and ev.phase in ('asc_start', 'asc')]
        for e0, e1 in zip(asc[:-1], asc[1:]):
            if not (0 < e0.vac < 30 and 0 < e1.vac < 30) or e1.dur <= 0:
                continue
            _, (t0, t1), ok = prof_at([e0.pres, e1.pres])
            if not ok.all():
                continue
            P0, P1 = P_ATM_INHG - e0.vac, P_ATM_INHG - e1.vac
            f = (P0 / (t0 + 273.15)) / (P1 / (t1 + 273.15)) - 1.0
            q = np.interp(e1.pres, fl['pres'], fl['q_ml_min'])
            rows.append(dict(sn=sn, dive=dive, pres=e1.pres, dur=e1.dur, f=f,
                             dv_model_ml=q * e1.dur / 60.0, dtemp=t1 - t0))
    return rows


# ------------------------------------------------------------------------------------------------
# Driver
# ------------------------------------------------------------------------------------------------

def LoadFloat(sn, args, valves_all):
    """Read one float's data and build its velocity points.

    Returns (info, status, pts, events_by_dive, prof_by_dive): status is '' when the float can
    be fitted, else the reason ('missing data', 'too few dives', 'no pump events').
    """

    desc_all = ReadAggregate(args.json_dir, sn, 'descent')
    asc_all = ReadAggregate(args.json_dir, sn, 'ascent')
    pump_all = ReadAggregate(args.json_dir, sn, 'pump')
    prof_all = ReadAggregate(args.json_dir, sn, 'profile')
    mission = ReadAggregate(args.json_dir, sn, 'mission')
    info = FloatInfo(sn, mission)

    if any(df is None or not len(df) for df in (desc_all, asc_all, pump_all, prof_all)):
        return info, 'missing data', [], {}, {}

    # Park pressure: median of the last 5 drift samples, else the E08 drift mean pressure
    park = {}
    eng = ReadAggregate(args.json_dir, sn, 'engineering_data')
    if eng is not None and 'drift.mean_pres_dbar' in eng:
        for dive, pres in zip(eng['dive'], pd.to_numeric(eng['drift.mean_pres_dbar'],
                                                          errors='coerce')):
            if np.isfinite(pres) and pres > 0:
                park[dive] = float(pres)
    drift = ReadAggregate(args.json_dir, sn, 'drift')
    if drift is not None and len(drift):
        for dive, g in drift.dropna(subset=['pres']).groupby('dive'):
            if len(g):
                park[dive] = float(np.median(g['pres'].to_numpy()[-5:]))

    valves = valves_all[valves_all['sn'] == sn]
    pts, events_by_dive, prof_by_dive = [], {}, {}
    groups = { k: g for k, g in pump_all.groupby('dive', sort=False) }
    vgroups = { k: g for k, g in valves.groupby('dive', sort=False) }
    dgroups = { k: g for k, g in desc_all.groupby('dive') }
    pgroups = { k: g for k, g in prof_all.groupby('dive') }
    for dive, asc in asc_all.groupby('dive'):
        if dive < 1 or dive not in dgroups or dive not in pgroups:
            continue
        prof_at = ProfileInterpolators(pgroups[dive])
        if prof_at is None:
            continue
        pumps = groups.get(dive)
        pumps = pumps.to_dict('records') if pumps is not None else []
        vlv = vgroups.get(dive)
        vlv = vlv.to_dict('records') if vlv is not None else []
        # An event with an invalid pressure or duration leaves the dive's oil budget unknown
        if not all(np.isfinite(e['pres']) and np.isfinite(e['time']) for e in pumps + vlv):
            continue
        try:
            dpts, events = DivePoints(sn, dive, dgroups[dive], asc, pumps, vlv, prof_at,
                                      park.get(dive))
        except Exception as exc:
            logger.debug('Float %s dive %s: %s', sn, dive, exc)
            continue
        pts.extend(dpts)
        events_by_dive[dive] = events
        prof_by_dive[dive] = prof_at

    n_dives = len(set(p['dive'] for p in pts))
    status = ''
    if n_dives < MIN_DIVES or not any(c[0] == 'pump' for p in pts for c in p['coefs']):
        status = 'too few dives' if n_dives < MIN_DIVES else 'no pump events'
    return info, status, pts, events_by_dive, prof_by_dive


def ProcessFloat(sn, args, valves_all, rng):
    """Read one float's data, build its points and fit it. Returns (row, flows, points, vac)."""

    info, status, pts, events_by_dive, prof_by_dive = LoadFloat(sn, args, valves_all)
    if status:
        row = dict(info, status=status)
        if pts:
            row['n_points'], row['n_dives'] = len(pts), len(set(p['dive'] for p in pts))
        return row, [], [], []

    row, flows = FitFloat(sn, pts, info, args.bootstrap, rng)
    vac = VacuumCheck(sn, events_by_dive, prof_by_dive, flows, row)
    if vac:
        df = pd.DataFrame(vac)
        good = (df['dv_model_ml'] > 0) & np.isfinite(df['f'])
        if good.sum() >= 10:
            # V_air: through-origin regression of dV_model on f
            x, yv = df.loc[good, 'f'].to_numpy(), df.loc[good, 'dv_model_ml'].to_numpy()
            row['vac_v_air_ml'] = float(np.sum(x * yv) / np.sum(x * x))
            row['vac_corr'] = float(np.corrcoef(x, yv)[0, 1])
            row['vac_n'] = int(good.sum())
    for p in pts:
        p.pop('coefs')
    return row, flows, pts, vac


LAB = {}


def LoadLab(path):
    """Per-float lab values from a CSV with columns sn, compress (1/dBar), total_g (g): the
    chain-ballast compressibility and ballasted weight. Missing values may be empty."""

    if not path or LAB:
        return
    df = pd.read_csv(path)
    for r in df.itertuples():
        LAB[int(r.sn)] = { k: (float(getattr(r, k)) if pd.notna(getattr(r, k)) else None)
                           for k in ('compress', 'total_g') }


def FloatTask(task):
    """Fit one float (worker entry point); errors become a status."""

    global MASS_SCALE, FIT_AIR
    sn, args, valves = task
    MASS_SCALE = args.mass_scale
    FIT_AIR = args.fit_air
    if args.free_kappa:
        KAPPA_REL.clear()
    LoadLab(args.lab_csv)
    rng = np.random.default_rng([args.seed, sn])
    try:
        return ProcessFloat(sn, args, valves, rng)
    except Exception as exc:
        logger.error('Float %s failed (%s: %s)', sn, type(exc).__name__, exc)
        return dict(sn=sn, status='error: {}'.format(exc)), [], [], []


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--json-dir', default='/data1/alamo/data/json')
    parser.add_argument('--list-dir', default='/data1/alamo/data/list')
    parser.add_argument('--valve-file', default='', help='vlv lines of the listings (.gz ok)')
    parser.add_argument('--output', '-o', required=True)
    parser.add_argument('--floats', nargs='*', type=int, default=None)
    parser.add_argument('--bootstrap', type=int, default=30)
    parser.add_argument('--seed', type=int, default=1)
    parser.add_argument('--mass-scale', type=float, default=1.0,
                        help='multiply NOMINAL_MASS_KG (sensitivity test)')
    parser.add_argument('--lab-csv', default='',
                        help='per-float lab values: sn, compress (1/dBar), total_g (g)')
    parser.add_argument('--fit-air', action='store_true',
                        help='fit residual bladder air (off: the air valve stays open on the dive)')
    parser.add_argument('--free-kappa', action='store_true',
                        help='fit kappa freely, without the measured ALTO prior')
    parser.add_argument('--jobs', '-j', type=int, default=1, help='parallel processes')
    parser.add_argument('--plots', action='store_true', help='write the figures')
    parser.add_argument('--verbose', '-v', action='store_true')
    args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO,
                        format='%(asctime)s %(levelname)s %(message)s')
    os.makedirs(args.output, exist_ok=True)

    serials = args.floats or sorted(int(d) for d in os.listdir(args.json_dir) if d.isdigit())
    valves = ReadValves(args, serials)
    logger.info('%d floats, %d valve records', len(serials), len(valves))

    global MASS_SCALE
    MASS_SCALE = args.mass_scale
    rows, flows, points, vacs = [], [], [], []
    tasks = [(sn, args, valves[valves['sn'] == sn]) for sn in serials]
    if args.jobs > 1:
        from concurrent.futures import ProcessPoolExecutor
        pool = ProcessPoolExecutor(args.jobs)
        results = pool.map(FloatTask, tasks, chunksize=1)
    else:
        results = map(FloatTask, tasks)
    for n, (sn, (row, fl, pts, vac)) in enumerate(zip(serials, results)):
        rows.append(row)
        flows.extend(fl)
        points.extend(pts)
        vacs.extend(vac)
        logger.info('[%d/%d] %s: %s, %s points, C_dA %s', n + 1, len(serials), sn,
                    row.get('status'), row.get('n_points'),
                    '{:.4f}'.format(row['cda_m2']) if 'cda_m2' in row else '-')

    pd.DataFrame(rows).to_csv(os.path.join(args.output, 'buoyancy_floats.csv'), index=False)
    pd.DataFrame(flows).to_csv(os.path.join(args.output, 'buoyancy_flow.csv'), index=False)
    pd.DataFrame(points).to_csv(os.path.join(args.output, 'buoyancy_points.csv.gz'), index=False)
    pd.DataFrame(vacs).to_csv(os.path.join(args.output, 'buoyancy_vacuum.csv'), index=False)

    if args.plots:
        import importlib.util
        spec = importlib.util.spec_from_file_location(
            'buoyancy_plots', os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                           'BuoyancyPlots.py'))
        plots = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(plots)
        plots.WriteFigures(args.output)


if __name__ == '__main__':
    sys.exit(main())
