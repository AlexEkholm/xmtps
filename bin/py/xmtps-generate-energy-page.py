#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""Generate the ALAMO/ALTO battery and energy page from aggregate JSON files.

Reads the per-float <id>_summary.json files written by xmtps-aggregate-json.py (given as files
and/or directories, searched recursively and filtered by the -i regular expressions), ignoring
files whose JSON version is not JSON_VERSION, floats that were never deployed (no completed
cycle with a profile, as on the summary page) and, with --days, floats that have not reported
within that many days. For each remaining float, the <id>_energy.json file next to its summary
(the energy model, EnergyEstimates in xmtps-aggregate-json.py) and the <id>_pump.json file give
its battery and energy use; floats without an energy file, and floats whose pack is not one of
PACKS (Tadiran only), are left out.

Writes <output>/energy.html (production: plots2/engineering/, next to the mission parameters
page, so that the float pages and status icons are one level up, see --plots-url): one
DataTables table, one row per float, with its battery (pack, age, startup voltage, oil-pump
voltage and whether it is falling, EnergyModel.PumpVoltageFallDive), its energy use (used so far,
per cycle, cycle length, and the shares of pumps, sensors, communication and standby over its
life) and its outlook on the current mission (cycles and time left with their range,
EnergyModel.RemainingCycles), as on the float pages' remaining-cycles plot. Little
time left, much of the pack used and a falling pump voltage are colored (STATUS_CLASSES).
The output directory must exist.

Example:
    xmtps-generate-energy-page.py -i"/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" -o/data1/alamo/data/plots2/engineering --plots-url /alamo /data1/alamo/data/json

Author: aekholm
"""

import argparse
import datetime
import json
import logging
import math
import os
import re

import jsonpickle
import numpy as np
import pandas as pd
from yattag import Doc

# local
from EnergyModel import (ENERGY_REMAINING_RECENT_DIVES, ENERGY_SECONDS_PER_YEAR, ENERGY_USABLE_FRACTION,
                         ENERGY_CONTROLLER_UNCERTAINTY, ENERGY_VOLTAGE_EARLY_DIVES,
                         ENERGY_VOLTAGE_FALL_FRACTION, CycleEnergy, PumpVoltageFallDive, RemainingCycles)
from FloatStatus import FloatStatus, LastLocation
from PageStatus import TABLE_PAGING, WritePageLinks, WriteStatusFooter

"""
Constants:
"""

JSON_VERSION = "0.11"

# Shares of a float's energy over its life: (header, energy columns)
ENERGY_SHARES = [
    ('Pumps',   ('pump_descent_j', 'pump_ascent_j', 'air_pump_j', 'internal_res_j')),
    ('Sensors', ('ctd_profile_j', 'ctd_drift_j', 'ctd_surface_j', 'wims_j', 'maple_j')),
    ('Comms',   ('iridium_j', 'gps_j')),
    ('Standby', ('controller_j', 'self_discharge_j', 'storage_j')),
    ]

# Expected time left (years) below which the outlook is colored: (limit, class), first match
TIME_LEFT_CLASSES = ((0.5, 'ebad'), (1.0, 'ewarn'), (2.0, 'elow'))

# Share of the nominal pack energy used, from which the Used cell is colored: (limit, class),
# first match: the high and central usable shares of the remaining-cycles estimate
USED_CLASSES = ((ENERGY_USABLE_FRACTION[2], 'ebad'), (ENERGY_USABLE_FRACTION[1], 'ewarn'))

PACK_NAMES = {'tadiran': 'Tadiran', 'saft': 'Saft', 'alkaline': 'Alkaline'}

# The static page describing the energy model (bin/py/html/energy-model.html, installed next to
# this page with its figures in energy-model/), linked from the introduction
MODEL_PAGE = 'energy-model.html'

# The float pages' Estimated Total Energy plot (energy_by_component, xmtps-aggregate-json.py), shown
# in the Total Energy column: its small image (PLOT_IMAGE_PX, 780 x 115 px), cropped to its rows
# PLOT_CROP_ROWS (the bar only, without the title, subtitle, tick labels and x-axis label; the
# small images all have this layout: bar in rows 32-76) and shown at PLOT_WIDTH_PX wide, linking to the float page's
# Engineering section (PLOT_LINK)
PLOT_ID = 'energy_by_component'
PLOT_IMAGE_PX = (780, 115)
PLOT_CROP_ROWS = (31, 78)
PLOT_WIDTH_PX = 300
PLOT_LINK = '#eng'

# Index of the Total Energy column: after the float (5), battery (4), energy (3) and share columns
PLOT_COLUMN = 5 + 4 + 3 + len(ENERGY_SHARES)

# The packs whose floats the page shows (PACK_NAMES); floats with another or an unknown pack are left out
PACKS = ('Tadiran',)

"""
Global Variables:
"""

float_map = {}

"""
Function Definitions:
"""


def ProcessDirectory(dpath):
    """Read every summary file in a directory and its subdirectories with ProcessJsonSummaryFile.

    Files are read in sorted order if they match an -i pattern (when given). Errors are logged, or
    raised with -H.
    """

    dpath = re.sub('\\\\', '/', dpath)

    try:

        if not os.path.exists(dpath) or not os.path.isdir(dpath):
            logger.error('Invalid path: ' + dpath)
            return
        logger.info('Processsing directory: ' + dpath + ' ...')

        for child in sorted(os.listdir(dpath)):

            fpath_child = re.sub('\\\\', '/', os.path.join(dpath, child))

            if os.path.isdir(fpath_child):
                ProcessDirectory(fpath_child)
                continue

            if not os.path.isfile(fpath_child):
                logger.warning('Not file or directory: ' + fpath_child)
                continue

            if not IsIncluded(fpath_child):
                continue

            ProcessJsonSummaryFile(fpath_child)

    except:

        errstr = 'Failed to process directory: ' + dpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


def IsIncluded(fpath):
    """Return True if fpath matches an -i pattern, or if none is given."""
    if args.include is None or not len(args.include):
        return True
    for re_include in args.include:
        if re.search(re_include, fpath):
            return True
    logger.debug('Ignoring file not included by REGEX: ' + fpath + ' ...')
    return False


def ReadAggregate(fpath, columns=None):
    """Read an aggregate JSON table (pandas orient 'split') into a DataFrame, keeping only columns
    when given; None if the file does not exist."""
    if not os.path.isfile(fpath):
        return None
    with open(fpath, 'r', encoding='utf-8') as file:
        d = json.load(file)
    df = pd.DataFrame(d['data'], columns=d['columns'])
    if columns is not None:
        df = df[[c for c in columns if c in df.columns]]
    return df


def ProcessJsonSummaryFile(fpath):
    """Read one <id>_summary.json file and the <id>_energy.json and <id>_pump.json files next to it
    into float_map, as EnergyStatus of the float.

    Floats that were never deployed, floats without an energy file, floats whose pack is not one
    of PACKS and, with --days, floats whose
    latest SBD message is older than that are ignored, as are summaries with a JSON version other
    than JSON_VERSION (with a warning). Errors are logged, or raised with -H.
    """

    global float_map

    fpath = re.sub('\\\\', '/', fpath)

    try:

        if not os.path.exists(fpath) or not os.path.isfile(fpath):
            logger.error('Invalid path: ' + fpath)
            return

        if not IsIncluded(fpath):
            return

        logger.debug('Processsing file: ' + fpath + ' ...')

        try:
            with open(fpath, 'r', encoding='utf-8') as file:
                summary = jsonpickle.decode(file.read(), keys=True)
        except:
            raise Exception('Failed to read or decode file: ' + fpath)

        if not 'id' in summary.keys() or not 'json_vers' in summary.keys():
            raise Exception('Invalid summary data.')

        if summary['json_vers'] != JSON_VERSION:
            logger.warning('Ignoring summary file with invalid json version: ' + fpath)
            return

        days_since_last = (datetime.datetime.now(datetime.UTC).timestamp() - summary['latest']['sbd'].dt_epoch) / 86400
        if args.days is not None and days_since_last > args.days:
            logger.debug('Ignoring float not reporting in the last {} days: {}'.format(args.days, fpath))
            return

        # Deployed floats only, as on the summary page: at least one completed cycle with a
        # profile (floats in pre-deployment tests have dives below 1)
        if summary['latest']['cycle'] < 1 or not summary['profdepthsum'] > 0:
            logger.debug('Ignoring float not deployed: {}'.format(fpath))
            return

        float_id = summary['id'].whoi
        dpath = os.path.dirname(fpath)

        energy = ReadAggregate(os.path.join(dpath, '{}_energy.json'.format(float_id)))
        if energy is None or len(energy) < 1:
            logger.info('Ignoring float without energy file: {}'.format(float_id))
            return
        pump = ReadAggregate(os.path.join(dpath, '{}_pump.json'.format(float_id)), ['dive', 'volt'])

        status = EnergyStatus(summary, energy, pump)
        if status['pack'] not in PACKS:
            logger.info('Ignoring float with pack {}: {}'.format(status['pack'] or 'unknown', float_id))
            return

        float_map[float_id] = { 'summary': summary, 'energy': status }

    except:

        errstr = 'Failed to process file: ' + fpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


def Finite(x):
    """Return x as a float, or None if it is missing or not finite."""
    try:
        x = float(x)
    except (TypeError, ValueError):
        return None
    return x if math.isfinite(x) else None


def EnergyStatus(summary, energy, pump):
    """Return a dict of a float's battery and energy figures from its 'energy' and 'pump' aggregates.

    Keys (None where unknown): pack (type name), nominal_j, age_s (battery age at the latest
    positive dive), start_v (startup voltage), v0 and v_last (median highest oil-pump voltage over
    dives 1 to ENERGY_VOLTAGE_EARLY_DIVES, and the latest dive's), fall (PumpVoltageFallDive),
    dive (latest positive dive), used_j and used_frac (energy used so far, storage included),
    cycle_j and cycle_s (energy without storage and duration of a cycle: medians over the last
    ENERGY_REMAINING_RECENT_DIVES dives), remaining (low, mid, high cycles left; RemainingCycles,
    as on the float pages' remaining-cycles plot), left_s (low, mid, high time left), shares (share of the float's total energy for each
    ENERGY_SHARES group), total_j.
    """
    s = dict(pack=None, nominal_j=None, age_s=None, start_v=None, v0=None, v_last=None, fall=None,
             dive=None, used_j=None, used_frac=None, cycle_j=None, cycle_s=None, remaining=None,
             left_s=None, shares=None, total_j=None)

    df, cycle_j = CycleEnergy(energy)
    if len(df) < 1:
        return s
    d = df.sort_values('dive')
    s['dive'] = int(d['dive'].iloc[-1])

    if 'pack_type' in d.columns and d['pack_type'].notna().any():
        pack = d['pack_type'].dropna().iloc[-1]
        s['pack'] = PACK_NAMES.get(pack, pack)
    if 'battery_age_s' in d.columns:
        s['age_s'] = Finite(d['battery_age_s'].dropna().iloc[-1]) if d['battery_age_s'].notna().any() else None
    if 'battery_start_v' in d.columns and d['battery_start_v'].notna().any():
        s['start_v'] = Finite(d['battery_start_v'].dropna().iloc[-1])

    # oil pump voltage: highest of each positive dive
    if pump is not None and len(pump) and 'volt' in pump.columns:
        p = pump.apply(pd.to_numeric, errors='coerce').dropna()
        v = p[p['dive'] > 0].groupby('dive')['volt'].max().sort_index()
        if len(v):
            s['v0'] = float(v.iloc[:ENERGY_VOLTAGE_EARLY_DIVES].median())
            s['v_last'] = float(v.iloc[-1])
    s['fall'] = PumpVoltageFallDive(pump)

    # shares of the total energy, storage included
    total = pd.to_numeric(d['total_j'], errors='coerce').sum()
    if total > 0:
        s['total_j'] = float(total)
        s['shares'] = [ float(sum(pd.to_numeric(d[c], errors='coerce').fillna(0).sum() for c in cols if c in d.columns)) / total
                        for _, cols in ENERGY_SHARES ]

    r = RemainingCycles(df, cycle_j)
    if r is not None:
        s['nominal_j'] = float(r['nominal_j'].iloc[-1])
        s['used_frac'] = Finite(r['used_frac'].iloc[-1])
        s['used_j'] = s['used_frac'] * s['nominal_j'] if s['used_frac'] is not None else None
        r = r[np.isfinite(r['remaining_mid'])]
        if len(r):
            last = r.iloc[-1]
            s['cycle_j'] = Finite(last['cycle_j'])
            s['cycle_s'] = Finite(last['cycle_s'])
            s['remaining'] = (float(last['remaining_low']), float(last['remaining_mid']), float(last['remaining_high']))
            if s['cycle_s'] is not None:
                s['left_s'] = tuple(x * s['cycle_s'] for x in s['remaining'])
    return s


def Class(value, limits, below=True):
    """Return the class of the first (limit, class) in limits that value is below (below=True) or
    at or above (below=False), or ''."""
    if value is None:
        return ''
    for limit, klass in limits:
        if (value < limit) if below else (value >= limit):
            return klass
    return ''


def Years(seconds):
    """Format a duration in years: one decimal, or '-' if unknown."""
    return '-' if seconds is None else '{:.1f}'.format(seconds / ENERGY_SECONDS_PER_YEAR)


def WriteEnergyFile(dpath):
    """Write the battery and energy page <dpath>/energy.html from float_map.

    One DataTables table, one row per float, most recent report first; every column can be sorted,
    and the search field filters the rows (by serial, type, status, location, pack, ...). The
    outlook columns are colored by the expected time left (TIME_LEFT_CLASSES), the Used column by
    the share of the pack used (USED_CLASSES), and the pump voltage when it is falling, except for
    ARCHIVED floats (no report in 180 days), whose batteries are mostly spent. The title
    text of each cell gives the details. Errors are logged, or raised with -H; a failed row is
    skipped.
    """

    global float_map

    fpath = re.sub('\\\\', '/', os.path.join(dpath, 'energy.html'))

    try:

        now = datetime.datetime.now(datetime.UTC).timestamp()
        float_ids = sorted(float_map, key=lambda i: float_map[i]['summary']['latest']['sbd'].dt_epoch, reverse=True)
        f_low, f_mid, f_high = ENERGY_USABLE_FRACTION

        doc, tag, text, line = Doc().ttl()

        doc.asis('<!DOCTYPE html>')
        with tag('html'):

            with tag('head'):

                doc.stag('meta', charset='utf-8')

                with tag('title'):
                    text('Float Battery and Energy')

                doc.asis('<script type="text/javascript" src="https://cdn.datatables.net/v/dt/jq-3.6.0/dt-1.12.1/datatables.min.js"></script>')
                doc.asis('<link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/v/dt/dt-1.12.1/datatables.min.css"/>')
                doc.asis('<script type="text/javascript">$(document).ready(function () { $("table.etable").DataTable({ order: [[3, \'desc\']], ' + TABLE_PAGING + ', columnDefs: [{ orderable: false, targets: ' + str(PLOT_COLUMN) + ' }], scrollX: false }); jQuery(".dataTable").wrap("<div class=\'tat\' />"); });</script>')

                with(tag('style')):

                    # general (as the mission parameters page)

                    text('a { color: #2196F3; text-decoration: none; } ')
                    text('a:hover {color: #064579;} ')
                    text('body { font-family: "Open Sans", verdana, arial, sans-serif; margin: 0px; font-size: 12px; } ')
                    text('#top { max-width: 1600px; margin: auto; } ')
                    text('.main { margin: 20px; }')
                    text('#tit { margin: auto; text-align: center; }')
                    text('#tit h1  { margin: 0px; }')
                    text('#intro { color: #555; margin: 15px 0px; line-height: 1.5; }')

                    # table

                    text('.tat { overflow-x: auto; width: 100%; margin-bottom: 20px; padding-top: 10px; }')
                    text('table.etable { width: 100%; }')
                    text('table.etable th { padding: 6px 4px; } ')
                    text('table.etable td { padding: 6px 4px; text-align: left; height: 28px; white-space: nowrap; } ')
                    text('table.etable td small, table.etable th small { color: #aaa; } ')
                    text('table.etable td .stimg { min-width: 80px; white-space: nowrap; } ')
                    text('table.etable td .stimg img { padding-right: 4px; display:inline; padding-bottom: 1px; } ')
                    text('table.etable td .sloc { display: block; color: #666; white-space: nowrap; } ')
                    text('table.etable td.ebad, table.etable td.ewarn, table.etable td.elow { font-weight: bold; } ')
                    text('table.etable td.eplot { padding: 2px 4px; } ')
                    scale = PLOT_WIDTH_PX / PLOT_IMAGE_PX[0]
                    text('table.etable td.eplot .eplotcrop { display: block; overflow: hidden; width: %dpx; height: %.0fpx; } '
                         % (PLOT_WIDTH_PX, (PLOT_CROP_ROWS[1] - PLOT_CROP_ROWS[0]) * scale))
                    text('table.etable td.eplot img { display: block; width: %dpx; height: auto; margin-top: -%.1fpx; } '
                         % (PLOT_WIDTH_PX, PLOT_CROP_ROWS[0] * scale))
                    text('table.etable td.ebad { background-color: #b71c1c !important; color: #fff; } ')
                    text('table.etable td.ebad small { color: #fdd; } ')
                    text('table.etable td.ewarn { background-color: hsl(0, 100%, 80%) !important; } ')
                    text('table.etable td.elow { background-color: hsl(50, 100%, 80%) !important; } ')
                    text('table.etable td.ewarn small, table.etable td.elow small { color: #555; } ')
                    text('.mkey { display: inline-block; padding: 1px 6px; font-weight: bold; } ')
                    text('.mkey.ebad { background-color: #b71c1c; color: #fff; } ')
                    text('.mkey.ewarn { background-color: hsl(0, 100%, 80%); } ')
                    text('.mkey.elow { background-color: hsl(50, 100%, 80%); } ')

                    # footer

                    text('#foot { font-size: 12px; color: #aaa; margin: 30px 0px; width: 100%;   } ')
                    text('#lfoot { float: left; width: 50%; white-space: preserve; margin: 0 0 30px 0;  } ')
                    text('#lfoot i { display: inline-block; margin 3px; } ')

            with tag('body'):

                with tag('div', id='top'):

                    with tag('div', klass='main'):

                        with tag('div', id='tit'):
                            with tag('h1'):
                                text('ALAMO/ALTO Battery and Energy')
                            WritePageLinks(doc, tag, text, 'energy', args.plots_url)

                        with tag('div', id='intro'):
                            text('The battery and energy use of the deployed floats (with at least one profile) with {} packs'.format(' or '.join(PACKS)))
                            if args.days is not None:
                                text(' that reported in the last {:g} days'.format(args.days))
                            text(', from the ')
                            with tag('a', href=MODEL_PAGE):
                                text('float energy model')
                            text(': the energy of each part of a cycle (pumps, sensors, '
                                 'Iridium and GPS, controller and self-discharge), from the engineering data and '
                                 'datasheets, as in each float page\'s energy plots. Used: all the energy used so far, '
                                 'storage before deployment included, of the pack\'s nominal energy. Cycle: medians over the '
                                 'last {:d} dives. Shares: of all the energy used, by pumps (oil and air, with the battery\'s '
                                 'internal loss), sensors (CTD, WIMS, microstructure), communication (Iridium and GPS) and '
                                 'standby (controller and self-discharge). Outlook on the current mission: cycles left when '
                                 '{:.0f}% of the nominal energy is usable, with the range for {:.0f}-{:.0f}% and the controller '
                                 'drain ±{:.0f}%; time left at the current cycle length. '.format(
                                     ENERGY_REMAINING_RECENT_DIVES, 100 * f_mid, 100 * f_low, 100 * f_high,
                                     100 * ENERGY_CONTROLLER_UNCERTAINTY))
                            text('Colors: expected time left under ')
                            for limit, klass in TIME_LEFT_CLASSES:
                                with tag('span', klass='mkey ' + klass):
                                    text('{:g} year{}'.format(limit, '' if limit == 1 else 's'))
                                text(' ')
                            text('; pack used at least ')
                            for limit, klass in USED_CLASSES:
                                with tag('span', klass='mkey ' + klass):
                                    text('{:.0f}%'.format(100 * limit))
                                text(' ')
                            text('; and an oil-pump voltage ')
                            with tag('span', klass='mkey ebad'):
                                text('falling')
                            text(' (its highest below {:.0f}% of its early median for 3 dives; it came a median 16% of '
                                 'their dives before the end on the floats that ran out of battery). Archived floats are '
                                 'not colored. Search the table to '
                                 'select floats, for example ACTIVE, ALTO or Tadiran; hover over a value for details.'.format(
                                     100 * ENERGY_VOLTAGE_FALL_FRACTION))

                        with tag('table', klass='etable stripe'):

                            with tag('thead'):
                                with tag('tr'):
                                    for header, n in (('Float', 5), ('Battery', 4), ('Energy', 3), ('Shares', len(ENERGY_SHARES)), ('Total Energy', 1), ('Outlook', 2)):
                                        with tag('th', colspan=n):
                                            text(header)
                                with tag('tr'):
                                    headers = [('ID', '', ''), ('Type', '', ''), ('Status', '', ''), ('Last Report', 'UTC', ''),
                                               ('Dive', '', 'Latest dive with a positive number'),
                                               ('Pack', '', 'Battery chemistry from the startup voltage, and nominal energy'),
                                               ('Age', 'years', 'Battery age at the latest dive, from the float\'s first message'),
                                               ('Startup', 'V', 'Startup (no-load) battery voltage'),
                                               ('Pump', 'V', 'Highest oil-pump voltage of the latest dive, and its early median (dives 1-{:d})'.format(ENERGY_VOLTAGE_EARLY_DIVES)),
                                               ('Used', 'MJ', 'Energy used so far, storage included, and its share of the nominal pack energy'),
                                               ('Cycle', 'kJ', 'Energy of a cycle, without storage: median of the last {:d} dives'.format(ENERGY_REMAINING_RECENT_DIVES)),
                                               ('Length', 'days', 'Length of a cycle: median of the last {:d} dives'.format(ENERGY_REMAINING_RECENT_DIVES))]
                                    headers += [ (name, '%', 'Share of all the energy used: ' + ', '.join(c[:-2] for c in cols)) for name, cols in ENERGY_SHARES ]
                                    headers += [('By Component', '', 'The float page\'s Estimated Total Energy plot: the energy used so far, by component; click for the Engineering section of the float page')]
                                    headers += [('Cycles Left', '', 'Expected cycles left on the current mission, and the range'),
                                                ('Time Left', 'years', 'Expected time left at the current cycle length, and the range')]
                                    for header, units, title in headers:
                                        with tag('th', title=title):
                                            text(header)
                                            if units:
                                                doc.stag('br')
                                                with tag('small'):
                                                    text('[' + units + ']')

                            with tag('tbody'):
                                for float_id in float_ids:

                                    try:

                                        x = float_map[float_id]
                                        summary = x['summary']
                                        e = x['energy']
                                        sbd = summary['latest']['sbd']

                                        with tag('tr'):

                                            with tag('td'): # id, with the WMO number under it
                                                with tag('a', href='{}/{}'.format(args.plots_url, float_id)):
                                                    text('{}'.format(float_id))
                                                if summary['id'].wmo > 1:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('{}'.format(summary['id'].wmo))

                                            with tag('td'): # type and CTD
                                                text(summary['type'])
                                                if summary['ctdtype']:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text(summary['ctdtype'])

                                            # DataTables searches a cell with data-order as its HTML (the
                                            # status icon path contains "alamo", titles hold details), so
                                            # those cells get their text as data-search
                                            location = LastLocation(summary)
                                            status = FloatStatus(summary, location)
                                            # archived floats (no report in 180 days) are not colored
                                            alert = status['name'] != 'ARCHIVED'
                                            with tag('td', ('data-order', '{:.0f}'.format(-sbd.dt_epoch)),
                                                     ('data-search', (status['name'] + ' ' + location).strip())): # status
                                                with tag('div', klass='stimg'):
                                                    doc.stag('img', src='{}/{}'.format(args.plots_url, status['img']))
                                                    text(status['name'])
                                                if location:
                                                    with tag('small', klass='sloc'):
                                                        text(location)

                                            report = sbd.dt_str[0:10] + ' ' + sbd.dt_str[11:16]
                                            ago = '{:.0f} days ago'.format((now - sbd.dt_epoch) / 86400)
                                            with tag('td', ('data-order', '{:.0f}'.format(sbd.dt_epoch)), ('data-search', report + ' ' + ago)): # last report
                                                text(report)
                                                doc.stag('br')
                                                with tag('small'):
                                                    text(ago)

                                            with tag('td'): # dive
                                                text('-' if e['dive'] is None else '{:d}'.format(e['dive']))

                                            # battery

                                            with tag('td', title='Nominal pack energy' if e['nominal_j'] else 'Pack unknown'): # pack
                                                text(e['pack'] or '-')
                                                if e['nominal_j']:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('{:.2f} MJ'.format(e['nominal_j'] / 1e6))

                                            with tag('td', ('data-order', '{:.0f}'.format(e['age_s'] if e['age_s'] is not None else -1)),
                                                     ('data-search', Years(e['age_s']))): # age
                                                text(Years(e['age_s']))

                                            with tag('td'): # startup voltage
                                                text('-' if e['start_v'] is None else '{:.1f}'.format(e['start_v']))

                                            fall = e['fall']
                                            title = ''
                                            if e['v0'] is not None:
                                                title = 'Latest dive {:.2f} V; early median {:.2f} V ({:.0f}%)'.format(e['v_last'], e['v0'], 100 * e['v_last'] / e['v0'])
                                            if fall is not None:
                                                title += '; below {:.0f}% of the early median since dive {:d}'.format(100 * ENERGY_VOLTAGE_FALL_FRACTION, fall[0])
                                            pump = ('-',) if e['v_last'] is None else ('{:.1f}'.format(e['v_last']),
                                                'falling since {:d}'.format(fall[0]) if fall is not None else 'of {:.1f}'.format(e['v0']))
                                            with tag('td', ('data-order', '{:.3f}'.format(e['v_last'] if e['v_last'] is not None else -1)),
                                                     ('data-search', ' '.join(pump)),
                                                     klass='ebad' if fall is not None and alert else '', title=title): # pump voltage
                                                text(pump[0])
                                                if len(pump) > 1:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text(pump[1])

                                            # energy

                                            used = e['used_frac']
                                            if used is None:
                                                used_text = ('-' if e['total_j'] is None else '{:.2f}'.format(e['total_j'] / 1e6),)
                                            else:
                                                used_text = ('{:.2f}'.format(e['used_j'] / 1e6), '{:.0f}%'.format(100 * used))
                                            with tag('td', ('data-order', '{:.4f}'.format(used if used is not None else -1)),
                                                     ('data-search', ' '.join(used_text)),
                                                     klass=Class(used, USED_CLASSES, below=False) if alert else '',
                                                     title='' if used is None else '{:.2f} MJ of {:.2f} MJ nominal'.format(e['used_j'] / 1e6, e['nominal_j'] / 1e6)): # used
                                                text(used_text[0])
                                                if len(used_text) > 1:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text(used_text[1])

                                            with tag('td'): # cycle energy
                                                text('-' if e['cycle_j'] is None else '{:.1f}'.format(e['cycle_j'] / 1000))

                                            with tag('td'): # cycle length
                                                text('-' if e['cycle_s'] is None else '{:.1f}'.format(e['cycle_s'] / 86400))

                                            for k, (name, cols) in enumerate(ENERGY_SHARES):
                                                share = e['shares'][k] if e['shares'] else None
                                                with tag('td', title='' if share is None else '{:.0f} kJ of {:.0f} kJ'.format(share * e['total_j'] / 1000, e['total_j'] / 1000)):
                                                    text('-' if share is None else '{:.0f}'.format(100 * share))

                                            # the float page's Estimated Total Energy plot, cropped to the
                                            # bar, linking to the float page's Engineering section; hidden
                                            # if the image is missing
                                            plot = '{}/{}/{}_{}'.format(args.plots_url, float_id, float_id, PLOT_ID)
                                            with tag('td', ('data-search', ''), klass='eplot'):
                                                with tag('a', klass='eplotcrop', href='{}/{}/{}'.format(args.plots_url, float_id, PLOT_LINK),
                                                         title='Estimated Total Energy of {}: open the Engineering section of its float page'.format(float_id)):
                                                    doc.stag('img', src=plot + '_small.png', alt='', loading='lazy',
                                                             onerror="this.parentNode.style.display='none'")

                                            # outlook

                                            years = e['left_s'][1] / ENERGY_SECONDS_PER_YEAR if e['left_s'] else None
                                            klass = Class(years, TIME_LEFT_CLASSES) if alert else ''
                                            rem = e['remaining']
                                            with tag('td', ('data-order', '{:.1f}'.format(rem[1] if rem else -1)),
                                                     ('data-search', '-' if rem is None else '{:.0f} {:.0f}-{:.0f}'.format(rem[1], rem[0], rem[2])), klass=klass,
                                                     title='' if rem is None else 'Expected {:.0f} cycles, range {:.0f}-{:.0f}'.format(rem[1], rem[0], rem[2])):
                                                if rem is None:
                                                    text('-')
                                                else:
                                                    text('{:.0f}'.format(rem[1]))
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('{:.0f}-{:.0f}'.format(rem[0], rem[2]))
                                            left = e['left_s']
                                            with tag('td', ('data-order', '{:.0f}'.format(left[1] if left else -1)),
                                                     ('data-search', '-' if left is None else '{} {}-{}'.format(Years(left[1]), Years(left[0]), Years(left[2]))), klass=klass,
                                                     title='' if left is None else 'Expected {:.0f} days, range {:.0f}-{:.0f}'.format(*[ t / 86400 for t in (left[1], left[0], left[2]) ])):
                                                if left is None:
                                                    text('-')
                                                else:
                                                    text(Years(left[1]))
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('{}-{}'.format(Years(left[0]), Years(left[2])))

                                    except:
                                        errstr = 'Failed to create energy row for float {}'.format(float_id)

                                        if hasattr(args, 'halt') and args.halt:
                                            raise Exception(errstr)
                                        logger.error(errstr)

                        with tag('div', id='foot'):
                            with tag('div', id='lfoot'):
                                with tag('i'):
                                    WriteStatusFooter(doc, tag, text, JSON_VERSION, args.runtime)

        with open(fpath, 'w', encoding='utf-8') as fout:
            fout.write(doc.getvalue())

    except:

        errstr = 'Failed to write energy file: ' + fpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


"""
Generate CLI arguments:
"""

parser = argparse.ArgumentParser(
    description='Generate the battery and energy page from aggregate JSON file(s).')

parser.add_argument('-d', '--debug', action='store_true', dest='debug', help='print debug output')
parser.add_argument('-q', '--quiet', action='store_true', dest='quiet', help='suppress output, except for warnings and errors')
parser.add_argument('-v', '--verbose', action='store_true', dest='verbose', help='print verbose output')
parser.add_argument('-H', '--halt-on-error', action='store_true', dest='halt',
                    help='halt execution on first error, otherwise continue processing')
parser.add_argument('-i', '--include', action='append', dest='include',
                    help='include paths that match regular expression REGEX (e.g. "/alamo/data/json/.*/[0-9]+_summary.json")',
                    metavar='REGEX')
parser.add_argument('-o', '--output', default='.', dest='output',
                    help='write energy.html to specified directory (default=\'.\')', metavar='PATH')
parser.add_argument('--plots-url', default='..', dest='plots_url',
                    help='URL of the plots directory (float pages and img/) relative to the page (default=\'..\', '
                         'for a page in plots2/engineering/)', metavar='URL')
parser.add_argument('--days', type=float, dest='days',
                    help='include only floats that reported in the last DAYS days (default: all deployed floats)',
                    metavar='DAYS')
parser.add_argument('--runtime', type=float, dest='runtime',
                    help='show SECONDS as the runtime in the page footer (see xmtps-generate-summary-page.py)',
                    metavar='SECONDS')
parser.add_argument(nargs='+', dest='path_in',
                    help='specify one or more paths to directories and/or JSON summary files', metavar='PATH ...')

args = parser.parse_args()

logger = logging.getLogger('generate-energy')
if not logger.hasHandlers():
    ch = logging.StreamHandler()
    ch.setFormatter(logging.Formatter('%(asctime)s %(levelname)7s %(message)s', datefmt="%Y%m%dT%H%M%S"))
    logger.addHandler(ch)
if args.debug:
    logger.setLevel(logging.DEBUG)
elif args.verbose:
    logger.setLevel(logging.INFO)
elif args.quiet:
    logger.setLevel(logging.ERROR)
else:
    logger.setLevel(logging.WARNING)

"""
Start of execution:
"""

try:

    if not args.output:
        args.output = '.'

    if not os.path.isdir(args.output):
        raise Exception('Invalid output directory: ' + args.output)

    for path in args.path_in:
        if os.path.isdir(path):
            ProcessDirectory(path)
        elif os.path.isfile(path):
            ProcessJsonSummaryFile(path)
        else:
            logger.error("Invalid path: " + path)

    WriteEnergyFile(args.output)

except:

    logger.error('Failed to generate energy file.')
    raise
