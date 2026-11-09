"""The float energy model's battery outlook, shared by xmtps-aggregate-json.py and the energy page.

The energy model itself (EnergyEstimates in xmtps-aggregate-json.py) writes each float's energy use
per dive to <serial>_energy.json. This module holds what is computed from it: the cycles left on
the current mission (RemainingCycles), a falling oil-pump voltage (PumpVoltageFallDive), the
energy of each cycle without storage (CycleEnergy), and the constants they use.
"""

import numpy as np
import pandas as pd

ENERGY_SECONDS_PER_YEAR = 365.25 * 86400

# Energy components of the _energy file: (column, label), stacked bottom up in the plots
ENERGY_COMPONENTS = [
    ('pump_descent_j', 'Pump-descent'),
    ('pump_ascent_j', 'Pump-ascent'),
    ('air_pump_j', 'Air pump'),
    ('ctd_profile_j', 'CTD-profile'),
    ('ctd_drift_j', 'CTD-drift'),
    ('ctd_surface_j', 'CTD-surface'),
    ('iridium_j', 'Iridium'),
    ('gps_j', 'GPS'),
    ('wims_j', 'WIMS'),
    ('maple_j', 'Microstructure (MAPLe)'),
    ('internal_res_j', 'Internal Res.'),
    ('controller_j', 'Controller'),
    ('self_discharge_j', 'Self-discharge'),
    ('storage_j', 'Self-discharge (storage)'),
    ]
ENERGY_PIE_ONLY = ('storage_j',)  # in the totals and energy_by_component, not in the per-cycle bars

# Remaining-cycles estimate (energy_remaining_cycles): the usable share of the pack's nominal
# energy, low / central / high, from the floats that ran out of battery: most used 50-95 % of
# nominal, median about 67 %; backtested on 92 of them, 50 / 68 / 95 % removes the bias that
# 65 / 80 / 95 % had (predicted / actual cycles left 1.4-1.9) and puts about three quarters of the
# floats inside the range; the controller drain's uncertainty (+-30 %), applied to each cycle's controller
# energy; and the number of latest dives whose median gives the energy and duration of a cycle on
# the current mission.
ENERGY_USABLE_FRACTION = (0.50, 0.68, 0.95)
ENERGY_CONTROLLER_UNCERTAINTY = 0.30
ENERGY_REMAINING_RECENT_DIVES = 10
# Falling pump voltage, marked on the remaining-cycles plot: a run of ENERGY_VOLTAGE_FALL_DIVES dives
# whose highest oil-pump voltage is below ENERGY_VOLTAGE_FALL_FRACTION of its median over the first
# ENERGY_VOLTAGE_EARLY_DIVES dives. On the floats that ran out of battery it came a median 60 dives,
# 16 % of their dives, before the end, and caught the abrupt failures the energy estimate misses.
ENERGY_VOLTAGE_FALL_FRACTION = 0.95
ENERGY_VOLTAGE_FALL_DIVES = 3
ENERGY_VOLTAGE_EARLY_DIVES = 10


def CycleEnergy(df_energy):
    """Return (df, cycle_j): the 'energy' aggregate's dives with a positive number and energy, and
    each one's energy without storage (ENERGY_PIE_ONLY), as the per-cycle plot and the
    remaining-cycles estimate use them. df is empty when there are none."""
    df = df_energy[(pd.to_numeric(df_energy['dive'], errors='coerce') > 0)
                   & (pd.to_numeric(df_energy['total_j'], errors='coerce') > 0)]
    cycle_j = df['total_j'] - df[[c for c in ENERGY_PIE_ONLY if c in df.columns]].sum(axis=1)
    return df, cycle_j


def RemainingCycles(df, cycle_j):
    """Return a DataFrame, one row per dive of df (the 'energy' aggregate's positive dives, sorted
    by dive), of the cycles left on the current mission after that dive: remaining_mid,
    remaining_low, remaining_high, with used_frac (energy used so far, storage included, over the
    nominal pack energy), cycle_j (median energy per cycle over the last
    ENERGY_REMAINING_RECENT_DIVES dives) and cycle_s (their median cycle duration). cycle_j is each
    dive's energy without storage. None when the pack energy is unknown."""
    pack_wh = pd.to_numeric(df['pack_wh'], errors='coerce') if 'pack_wh' in df.columns else pd.Series(dtype=float)
    pack_wh = pack_wh[pack_wh > 0]
    if len(pack_wh) < 1:
        return None
    nominal = float(pack_wh.iloc[-1]) * 3600
    d = df.sort_values('dive')
    n = ENERGY_REMAINING_RECENT_DIVES
    m = min(3, n)                                    # dives needed before a first estimate
    e_mid = cycle_j.loc[d.index].rolling(n, min_periods=m).median()
    ctrl = (d['controller_j'] if 'controller_j' in d.columns else 0 * e_mid).rolling(n, min_periods=m).median()
    e_low = (e_mid - ENERGY_CONTROLLER_UNCERTAINTY * ctrl).clip(lower=1.0)
    e_high = e_mid + ENERGY_CONTROLLER_UNCERTAINTY * ctrl
    used = d['total_j'].cumsum()
    f_low, f_mid, f_high = ENERGY_USABLE_FRACTION
    cyc_s = d['self_discharge_s'].where(d['self_discharge_s'] > 0) if 'self_discharge_s' in d.columns \
        else pd.Series(np.nan, index=d.index)
    return pd.DataFrame({
        'dive':           d['dive'].values,
        'remaining_mid':  ((f_mid * nominal - used) / e_mid).clip(lower=0).values,
        'remaining_low':  ((f_low * nominal - used) / e_high).clip(lower=0).values,
        'remaining_high': ((f_high * nominal - used) / e_low).clip(lower=0).values,
        'used_frac':      (used / nominal).values,
        'cycle_j':        e_mid.values,
        'cycle_s':        cyc_s.rolling(n, min_periods=1).median().values,
        'nominal_j':      nominal,
        })


def PumpVoltageFallDive(df_pump):
    """Return (dive, v0, v_last) for the first positive dive after dive ENERGY_VOLTAGE_EARLY_DIVES
    that starts a run of ENERGY_VOLTAGE_FALL_DIVES dives whose highest oil-pump voltage is below
    ENERGY_VOLTAGE_FALL_FRACTION of its median over dives 1 to ENERGY_VOLTAGE_EARLY_DIVES (v0); v_last
    is the latest dive's highest voltage. None when the voltage has not fallen or the pump data is
    missing. On the floats that ran out of battery this flag came a median 16 % of their dives before
    the end."""
    if not isinstance(df_pump, pd.DataFrame) or len(df_pump) < 1 or 'volt' not in df_pump.columns:
        return None
    p = df_pump[['dive', 'volt']].apply(pd.to_numeric, errors='coerce').dropna()
    v = p[p['dive'] > 0].groupby('dive')['volt'].max().sort_index()
    n0 = ENERGY_VOLTAGE_EARLY_DIVES
    if len(v) <= n0:
        return None
    v0 = float(v.iloc[:n0].median())
    below = (v < ENERGY_VOLTAGE_FALL_FRACTION * v0).values
    run = 0
    for i in range(n0, len(v)):
        run = run + 1 if below[i] else 0
        if run >= ENERGY_VOLTAGE_FALL_DIVES:
            return int(v.index[i - run + 1]), v0, float(v.iloc[-1])
    return None
