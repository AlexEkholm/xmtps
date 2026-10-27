#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""Generate the ALAMO/ALTO mission parameters page from aggregate JSON files.

Reads the per-float <id>_summary.json files written by xmtps-aggregate-json.py (given as files
and/or directories, searched recursively and filtered by the -i regular expressions), ignoring
files whose JSON version is not JSON_VERSION, floats that were never deployed (no completed
cycle with a profile, as on the summary page) and, with --days, floats that have not reported
within that many days. For each remaining float, the <id>_mission.json file next to its summary
gives the most recently reported mission parameters: the last dive with any mission parameter
(floats need not send them every dive). Floats that have not reported any are left out.

Writes <output>/params.html (production: plots2/engineering/, so that the float pages and status
icons are one level up, see --plots-url) with one table per float type (ALAMO, including WIMS and MM2
floats, and ALTO), one row per float with its firmware, the dive the parameters come from and a selection
of important parameters (COLUMNS). Phase-dependent parameters (DTD0-2, PFD0-2, ...) are shown for
each mission phase, marking the float's current phase and greying out phases its mission does not
use (MissionPhases). Each value is coloured by how abnormal it is in its group
(how rare it is, or for MEDIAN_COLUMNS how far from the median), and values that are out of
range or inconsistent (drift or profile pressure deeper than the maximum depth) are marked as
errors. Unset values (255, 65535, ...) are shown as unknown, without colour. The output
directory must exist.

Example:
    xmtps-generate-mission-page.py -i"/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" -o/data1/alamo/data/plots2 /data1/alamo/data/json

Author: aekholm
"""

import argparse
import collections
import datetime
import itertools
import json
import logging
import jsonpickle
import math
import os
import re
import statistics
from yattag import Doc

from PageStatus import WriteStatusFooter

"""
Static configuration:
"""


JSON_VERSION = "0.11"

# Values the firmware sends for unset parameters, e.g. before a mission is loaded (U1, U2, U4),
# and as scaled in the listing (ARR, SAN/SAX, CPN, values in ms listed in s, the signed ICT)
UNSET_VALUES = (255, 65535, 4294967295, 655.35, 65.535, 6553.5, 4294967.295, -327.68)

# Groups with fewer floats than this are not checked against their most common values
MIN_GROUP_SIZE = 3

# Mission phases of phase-dependent parameters
PHASES = (0, 1, 2)

# Table columns: (header group, header, units, parameter, format, description). A parameter
# ending in '#' is phase-dependent and gets one column per mission phase: '#' is replaced by the
# phase (firmware without phases sends it without index, shown as phase 0). The 'MPS' column
# shows the current phase (MissionPhases). Formats: 'd' integer, 'f' as listed, 'h' seconds
# shown in hours, 'b' 0/1 shown as off/on.
COLUMNS = [
    ('Mission',     'Phase',     '',        'MPS',  'd', 'Current mission phase: MPS, or the next phase MNS once the cycle count MPC has reached MNM'),
    ('Pressures',   'Drift',     'dBar',    'DTD#', 'd', 'DTD: target drift/park pressure'),
    ('Pressures',   'Profile',   'dBar',    'PFD#', 'd', 'PFD: target profile pressure (used when the fall mode PFM is on)'),
    ('Pressures',   'Fall',      '',        'PFM#', 'b', 'PFM: fall to the profile pressure after the drift phase'),
    ('Pressures',   'Max.',      'dBar',    'MDS',  'd', 'MDS: maximum (mission abort) pressure'),
    ('Pressures',   'Cut-off',   'dBar',    'CTF',  'd', 'CTF: profile cut-off pressure'),
    ('Pressures',   'Surface',   'dBar',    'ASD',  'd', 'ASD: surface pressure threshold'),
    ('Timing',      'Drift',     'h',       'PTO#', 'h', 'PTO: drift/park duration'),
    ('Timing',      'GPS',       'min',     'SGT',  'd', 'SGT: maximum GPS duration'),
    ('Timing',      'Iridium',   'min',     'SET',  'd', 'SET: maximum Iridium duration'),
    ('Timing',      'Send Cfg.', '',        'APT',  'b', 'APT: send the mission parameters at every surfacing'),
    ('Descent',     '1st Pump',  's',       'DFT#', 'd', 'DFT: first descent pump duration'),
    ('Descent',     'Seek',      '',        'DCM',  'b', 'DCM: descent seek mode'),
    ('Descent',     'Park Seek', '',        'PSM',  'b', 'PSM: park seek mode'),
    ('Ascent',      'Rate',      'm/s',     'ARR',  'f', 'ARR: target ascent rate'),
    ('Ascent',      'Max. Pump', 's',       'APM',  'd', 'APM: maximum ascent pump duration'),
    ('Surface',     'Oil Pump',  's',       'SSP',  'd', 'SSP: oil pump duration at the surface'),
    ('Surface',     'Air Pump',  '',        'SAM',  'b', 'SAM: air pump (air-assist) mode'),
    ('Surface',     'Air Dur.',  's',       'SAO',  'd', 'SAO: air pump duration'),
    ('Ice',         'Mode',      '',        'ICM',  'd', 'ICM: ice-detection mode'),
    ('Ice',         'Temp.',     'degC',    'ICT',  'f', 'ICT: ice threshold temperature'),
    ('Other',       'RBR Corr.', '',        'PPE',  'd', 'PPE: RBR dynamic correction mode (2 = corrected salinity sent)'),
    ('Other',       'WIMS',      's',       'WST',  'd', 'WST/WMT: WIMS timeout'),
]

# The typical range shown in each table's footer: from this percentile to 100 minus it
TYPICAL_RANGE_PERCENTILE = 10

# Scores below this are not coloured
MIN_SCORE = 0.05

# Columns compared with the group's median instead of how common the value is (pump durations
# are tuned for each float): (relative deviation from the median that is still not coloured,
# relative deviation that gets the full colour)
MEDIAN_COLUMNS = {'DFT#': (0.25, 2.0)}

# Status by days since the latest report, as on the summary page: (days, status, image)
STATUS = ((30, 'ACTIVE', 'img/s2.png'), (180, 'OVERDUE', 'img/s1.png'), (math.inf, 'ARCHIVED', 'img/s0.png'))

# Alternative names of a parameter in other firmware layouts
ALIASES = {'WST': ('WMT',)}

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


def ProcessJsonSummaryFile(fpath):
    """Read one <id>_summary.json file and the <id>_mission.json file next to it into float_map.

    Floats that were never deployed, floats without mission parameters and, with --days, floats
    whose latest SBD message is older than that are ignored, as are summaries with a JSON version
    other than JSON_VERSION (with a warning). The entry stores the summary and the mission row of
    the latest dive with mission parameters. Errors are logged, or raised with -H.
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
            with open(fpath, 'r') as file:
                summary = jsonpickle.decode(file.read())
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

        # Latest dive with any mission parameter. The rows are the dives with mission data, in
        # dive order; a row may carry only the firmware words ("Incomplete mission parameters").
        # Floats that have not reported mission parameters are left out.

        fpath_mission = re.sub('\\\\', '/', os.path.join(os.path.dirname(fpath), '{}_mission.json'.format(float_id)))
        if not os.path.isfile(fpath_mission):
            logger.info('Ignoring float without mission file: {}'.format(float_id))
            return

        with open(fpath_mission, 'r') as file:
            mission = json.load(file)

        latest = None
        meta = ('dive', 'fwver', 'cfgver', 'ctdtype')
        for row in mission['data']:
            values = dict(zip(mission['columns'], row))
            if any(v is not None for k, v in values.items() if k not in meta):
                latest = values

        if latest is None:
            logger.info('Ignoring float without mission parameters: {}'.format(float_id))
            return

        float_map[float_id] = { 'summary': summary, 'mission': latest, 'phases': MissionPhases(latest) }

    except:

        errstr = 'Failed to process file: ' + fpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


def FirmwareString(fwver):
    """Format a firmware word '0xMMmmppbb' as 'v10.12.1', or return '' if it is not one."""
    if not re.fullmatch('0x[0-9A-Fa-f]{8}', fwver or ''):
        return ''
    return 'v{:d}.{:d}.{:d}'.format(int(fwver[2:4], 16), int(fwver[4:6], 16), int(fwver[6:8], 16))


def TableCells():
    """Yield (column, phase) for each data column of the table: phase 0-2 for each mission phase
    of a phase-dependent COLUMNS entry, None for the others."""
    for col in COLUMNS:
        if col[3].endswith('#'):
            for phase in PHASES:
                yield col, phase
        else:
            yield col, None


def ParameterValue(mission, param, phase=None):
    """Return (name, value) of a COLUMNS parameter in a mission row, resolving the phase index '#'
    (firmware without phases sends the parameter without index, shown as phase 0) and ALIASES;
    value is None if the float doesn't send the parameter."""
    names = [param]
    if param.endswith('#'):
        base = param[:-1]
        names = [base + '{:d}'.format(phase)] + ([base] if phase == 0 else [])
    names += list(ALIASES.get(param, ()))
    for name in names:
        if mission.get(name) is not None:
            return name, mission[name]
    return names[0], None


def CellValue(x, param, phase):
    """Return (name, value) of a table cell of float_map entry x: the current phase
    (MissionPhases) for 'MPS' (the reported MPS if that is unset), otherwise ParameterValue."""
    if param == 'MPS':
        current = x['phases'][0]
        return 'MPS', current if current is not None else x['mission'].get('MPS')
    return ParameterValue(x['mission'], param, phase)


def MissionPhases(mission):
    """Return (current, used, description) of a float's mission phases.

    current is the phase of the float's next dive: the reported phase MPS, or, once its cycle
    count MPC has reached the phase's number of cycles MNM<MPS>, the next phase MNS<MPS> (repeated,
    as a phase with MNM 0 is passed through at once). It is None if the phase is unset or invalid.
    used is the set of phases the mission runs: those reached from phase 0 through MNS, except
    phases with MNM 0, plus the current and reported phases. Firmware without phases (no MPS)
    has current phase 0 and uses phase 0 only. description explains the current phase.
    """
    def get(key):
        value = mission.get(key)
        return None if value is None or value in UNSET_VALUES else int(value)

    if mission.get('MPS') is None:
        return 0, {0}, 'firmware without mission phases'
    mps, mpc = get('MPS'), get('MPC')
    if mps not in PHASES:
        return None, set(PHASES), 'mission phase unset or invalid'

    mnm = get('MNM{:d}'.format(mps))
    description = 'reported phase MPS {:d}, cycle MPC {} of MNM{:d} {}'.format(
        mps, '?' if mpc is None else mpc, mps, '?' if mnm is None else mnm)
    current, count = mps, mpc
    for _ in PHASES:
        mnm_current, mns = get('MNM{:d}'.format(current)), get('MNS{:d}'.format(current))
        if count is None or mnm_current is None or mns not in PHASES or count < mnm_current:
            break
        current, count = mns, 0
    if mpc is not None and mnm is not None and mpc >= mnm:
        description += '; phase done, next phase {:d} (MNS{:d})'.format(current, mps)

    used, phase = set(), 0
    while phase in PHASES and phase not in used:
        used.add(phase)
        phase = get('MNS{:d}'.format(phase))
    used = {p for p in used if get('MNM{:d}'.format(p)) != 0} | {current, mps}
    return current, used, description


def FormatValue(value, fmt):
    """Format a parameter value for the table."""
    if value is None or (isinstance(value, float) and math.isnan(value)):
        return '-'
    if value in UNSET_VALUES:
        return 'unknown'
    if fmt == 'b' and value in (0, 1):
        return 'on' if value else 'off'
    if fmt == 'h':
        return '{:g}'.format(round(value / 3600, 2))
    if fmt == 'd' and float(value).is_integer():
        return '{:d}'.format(int(value))
    return '{:g}'.format(value)


def CheckValue(mission, param, value, phase=None):
    """Return an error text for an out-of-range or inconsistent value (of the given phase, for
    phase-dependent parameters), or '' (also for unset values, which are shown as unknown)."""
    if value is None or value in UNSET_VALUES:
        return ''
    base = param.rstrip('#')
    if base == 'MPS' and not 0 <= value <= 2:
        return 'mission phase outside 0-2'
    mds = mission.get('MDS')
    mds_ok = mds is not None and mds not in UNSET_VALUES
    if base == 'DTD':
        if not 0 <= value <= 7000:
            return 'drift pressure outside 0-7000 dBar'
        if mds_ok and value > mds:
            return 'drift pressure deeper than the maximum pressure MDS ({:g} dBar)'.format(mds)
    if base == 'PFD' and mds_ok and value > mds:
        _, pfm = ParameterValue(mission, 'PFM#', phase)
        if pfm == 1:
            return 'profile pressure deeper than the maximum pressure MDS ({:g} dBar)'.format(mds)
    return ''


def Abnormality(param, value, stat):
    """Return (score, description) of a value against its group: score 0 for a normal value up to
    1 for the most abnormal.

    stat is the group's (reference value, its count, number of values, Counter of the values).
    For MEDIAN_COLUMNS the score rises linearly from 0 at the column's tolerated deviation from
    the median (relative to the median) to 1 at its full-scale deviation. Otherwise it is 1 - (floats with this value) / (floats
    with the most common value): 0 for the most common value, near 1 for a value only one float
    of a large group has.
    """
    ref, ref_count, n, counts = stat
    if param in MEDIAN_COLUMNS:
        dev = abs(value - ref) / abs(ref) if ref else (0 if value == ref else 1)
        tolerance, full = MEDIAN_COLUMNS[param]
        return min(1, max(0, dev - tolerance) / (full - tolerance)), '{:.0f}% from the group median {:g} ({:d} floats)'.format(100 * dev, ref, n)
    count = counts.get(value, 0)
    return 1 - count / ref_count, '{:d} of {:d} floats; most common {} ({:d} floats)'.format(
        count, n, ref if isinstance(ref, str) else '{:g}'.format(ref), ref_count)


def TypicalRange(values):
    """Return (low, high), the TYPICAL_RANGE_PERCENTILE-th and (100 - TYPICAL_RANGE_PERCENTILE)-th
    percentiles of values by nearest rank, so that both are values floats actually use."""
    values = sorted(values)
    n = len(values) - 1
    return (values[math.floor(n * TYPICAL_RANGE_PERCENTILE / 100)],
            values[math.ceil(n * (100 - TYPICAL_RANGE_PERCENTILE) / 100)])


def ScoreStyle(score, low=False):
    """Return the inline style of a cell with abnormality score > 0: from white (near 0) through
    yellow to red, or for values below the group's most common value (or median), from white to
    blue, so that values close to typical are nearly white."""
    if low:
        return 'background-color: hsl({:.0f}, 100%, {:.0f}%) !important;'.format(195 + 25 * score, 100 - 32 * score)
    return 'background-color: hsl({:.0f}, 100%, {:.0f}%) !important;'.format(55 * (1 - score), 100 - 35 * score)


def WriteMissionFile(dpath):
    """Write the mission parameters page <dpath>/params.html from float_map.

    One DataTables table per float type, most recent report first, with the group's
    most common value (or median) of each column in the footer. In groups of at least
    MIN_GROUP_SIZE floats with parameters, each value is coloured by its Abnormality score (from
    white to red, or white to blue for values below the most common value or median, ScoreStyle); errors found by CheckValue are dark red. The title text of
    each cell gives the details. Errors are logged, or
    raised with -H; a failed row is skipped.
    """

    global float_map

    fpath = re.sub('\\\\', '/', os.path.join(dpath, 'params.html'))

    try:

        now = datetime.datetime.now(datetime.UTC).timestamp()

        # Group the floats by type (ALAMO, including WIMS and MM2 floats, and ALTO)

        groups = collections.defaultdict(list)
        for float_id, x in float_map.items():
            groups[x['summary']['type']].append(float_id)

        # Create document

        doc, tag, text, line = Doc().ttl()

        doc.asis('<!DOCTYPE html>')
        with tag('html'):

            with tag('head'):

                doc.stag('meta', charset='utf-8')

                with tag('title'):
                    text('Float Mission Parameters')

                doc.asis('<script type="text/javascript" src="https://cdn.datatables.net/v/dt/jq-3.6.0/dt-1.12.1/datatables.min.js"></script>')
                doc.asis('<link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/v/dt/dt-1.12.1/datatables.min.css"/>')
                doc.asis('<script type="text/javascript">$(document).ready(function () { $("table.mtable").DataTable({ order: [[3, \'desc\']], paging: false, info: false, scrollX: false }); jQuery(".dataTable").wrap("<div class=\'tat\' />"); });</script>')

                with(tag('style')):

                    # Create styles - general (as the summary page)

                    text('a { color: #2196F3; text-decoration: none; } ')
                    text('a:hover {color: #064579;} ')
                    text('body { font-family: "Open Sans", verdana, arial, sans-serif; margin: 0px; font-size: 12px; } ')
                    text('#top { max-width: 1600px; margin: auto; } ')
                    text('.main { margin: 20px; }')

                    text('#tit { margin: auto; text-align: center; }')
                    text('#tit h1  { margin: 0px; }')
                    text('#intro { color: #555; margin: 15px 0px; line-height: 1.5; }')
                    text('h2 { margin: 25px 0px 0px 0px; font-size: 16px; }')

                    # Create styles - table

                    text('.tat { overflow-x: auto; width: 100%; margin-bottom: 20px; padding-top: 10px; }')
                    text('table.mtable { width: 100%; }')
                    text('table.mtable th { padding: 6px 4px; } ')
                    text('table.mtable td { padding: 6px 4px; text-align: left; height: 28px; white-space: nowrap; } ')
                    text('table.mtable td small, table.mtable th small { color: #aaa; } ')
                    text('table.mtable td .stimg { width: 80px; } ')
                    text('table.mtable td .stimg img { padding-right: 4px; display:inline; padding-bottom: 1px; } ')
                    text('table.mtable td[style] small { color: #555; } ')
                    text('table.mtable tfoot td { color: #777; font-style: italic; } ')
                    text('table.mtable tfoot tr:first-child td { border-top: 1px solid #111; } ')
                    text('table.mtable th.mphases { text-align: center; } ')
                    text('.mcur { font-weight: bold; box-shadow: inset 0 -3px 0 #555; } ')
                    text('.munused { color: #ccc; } ')
                    text('.merr { background-color: #b71c1c !important; color: #fff; font-weight: bold; } ')
                    text('.mkey { display: inline-block; padding: 1px 6px; } ')
                    text('.mscale { background: linear-gradient(to right, hsl(55, 100%, 100%), hsl(28, 100%, 83%), hsl(0, 100%, 65%)); padding: 1px 30px; } ')
                    text('.mscalelow { background: linear-gradient(to right, hsl(195, 100%, 100%), hsl(208, 100%, 84%), hsl(220, 100%, 68%)); padding: 1px 30px; } ')

                    # Create styles - footer

                    text('#foot { font-size: 12px; color: #aaa; margin: 30px 0px; width: 100%;   } ')
                    text('#lfoot { float: left; width: 50%; white-space: preserve; margin: 0 0 30px 0;  } ')
                    text('#lfoot i { display: inline-block; margin 3px; } ')

            with tag('body'):

                with tag('div', id='top'):

                    with tag('div', klass='main'):

                        with tag('div', id='tit'):
                            with tag('h1'):
                                text('ALAMO/ALTO Mission Parameters')

                        with tag('div', id='intro'):
                            text('The most recently reported mission parameters of the deployed floats (with at least one profile)')
                            if args.days is not None:
                                text(' that reported in the last {:g} days'.format(args.days))
                            text(', from the latest dive with mission parameters (floats need not send them every dive; floats that never sent any are left out). ')
                            text('Phase-dependent parameters are shown for each mission phase (P0-P2): the float\'s ')
                            with tag('span', klass='mkey mcur'):
                                text('current phase')
                            text(' (the next phase once the cycle count MPC has reached MNM), and ')
                            with tag('span', klass='mkey munused'):
                                text('phases its mission does not use')
                            text(' (not reached from phase 0 through MNS, or with MNM 0), which are not compared. ')
                            text('Floats are compared within their group (groups of at least {:d} floats); the colour shows how abnormal a value is: '.format(MIN_GROUP_SIZE))
                            with tag('span', klass='mkey mscale'):
                                text('higher: less common \u2192 rare')
                            text(' and ')
                            with tag('span', klass='mkey mscalelow'):
                                text('lower: less common \u2192 rare')
                            text(' than the group\'s most common value (first pump: distance from the group median), ')
                            with tag('span', klass='mkey merr'):
                                text('out of range or deeper than the maximum pressure')
                            text('. Unset values (sent before a mission is loaded) are shown as unknown. Hover over a header or value for details.')

                        for group in sorted(groups.keys()):

                            float_ids = sorted(groups[group], key=lambda i: float_map[i]['summary']['latest']['sbd'].dt_epoch, reverse=True)

                            # Statistics of each column (parameter and phase) over the group's floats
                            # with set values, in phases their mission uses: (most common value or
                            # median, its count, number of values, counts), and the typical range
                            # (TypicalRange)

                            modes, ranges = {}, {}
                            for col, phase in TableCells():
                                param = col[3]
                                values = []
                                for float_id in float_ids:
                                    value = CellValue(float_map[float_id], param, phase)[1]
                                    if phase is not None and phase not in float_map[float_id]['phases'][1]:
                                        continue
                                    if value is not None and value not in UNSET_VALUES:
                                        values.append(value)
                                if values:
                                    counts = collections.Counter(values)
                                    most_common = counts.most_common(1)[0]
                                    if param in MEDIAN_COLUMNS:
                                        modes[param, phase] = (statistics.median(values), None, len(values), counts)
                                    else:
                                        modes[param, phase] = (most_common[0], most_common[1], len(values), counts)
                                    ranges[param, phase] = TypicalRange(values)

                            check_group = len(float_ids) >= MIN_GROUP_SIZE

                            with tag('h2'):
                                text('{} '.format(group))
                                with tag('small'):
                                    text('({:d} float{})'.format(len(float_ids), '' if len(float_ids) == 1 else 's'))

                            with tag('table', klass='mtable stripe'):

                                # Table header: column groups, then the columns, then the phases of
                                # phase-dependent columns

                                with tag('thead'):
                                    with tag('tr'):
                                        with tag('th', colspan=6):
                                            text('Float')
                                        for header_group, n in [(k, len(list(g))) for k, g in itertools.groupby(c[0] for c, _ in TableCells())]:
                                            with tag('th', colspan=n):
                                                text(header_group)
                                    with tag('tr'):
                                        for header, units in (('ID', ''), ('Type', ''), ('Firmware', ''), ('Last Report', 'UTC'), ('Status', ''), ('Dive No.', '')):
                                            with tag('th', rowspan=2):
                                                text(header)
                                                if units:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('[' + units + ']')
                                        for _, header, units, param, _, description in COLUMNS:
                                            span = ('colspan', len(PHASES)) if param.endswith('#') else ('rowspan', 2)
                                            with tag('th', span, title=description, klass='mphases' if param.endswith('#') else ''):
                                                text(header)
                                                doc.stag('br')
                                                with tag('small'):
                                                    text(param.replace('#', '') + (' [' + units + ']' if units else ''))
                                    with tag('tr'):
                                        for col, phase in TableCells():
                                            if phase is not None:
                                                with tag('th', title='{}, mission phase {:d}'.format(col[5], phase)):
                                                    text('P{:d}'.format(phase))

                                # Table body

                                with tag('tbody'):
                                    for float_id in float_ids:

                                        try:

                                            x = float_map[float_id]
                                            summary = x['summary']
                                            mission = x['mission']

                                            with tag('tr'):

                                                with tag('td'): # id
                                                    with tag('a', href='{}/{}'.format(args.plots_url, float_id)):
                                                        text('{}'.format(float_id))
                                                    if summary['id'].wmo > 1:
                                                        doc.stag('br')
                                                        with tag('small'):
                                                            text('{}'.format(summary['id'].wmo))

                                                with tag('td'): # type
                                                    text(summary['type'])
                                                    ctdtype = mission['ctdtype'] if mission is not None and mission.get('ctdtype') else summary['ctdtype']
                                                    if len(ctdtype) > 0:
                                                        doc.stag('br')
                                                        with tag('small'):
                                                            text(ctdtype)

                                                # firmware
                                                fwver = mission['fwver'] if mission.get('fwver') else summary['fwver']
                                                with tag('td'):
                                                    if FirmwareString(fwver):
                                                        text(FirmwareString(fwver))
                                                        doc.stag('br')
                                                        with tag('small'):
                                                            text(fwver)
                                                    else:
                                                        text('-')

                                                # last report (sorted by epoch)
                                                sbd = summary['latest']['sbd']
                                                with tag('td', ('data-order', '{:.0f}'.format(sbd.dt_epoch))):
                                                    text(sbd.dt_str[0:10] + ' ' + sbd.dt_str[11:16])
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('{:.0f} days ago'.format((now - sbd.dt_epoch) / 86400))

                                                days = (now - sbd.dt_epoch) / 86400
                                                with tag('td', ('data-order', '{:.0f}'.format(-sbd.dt_epoch))): # status
                                                    _, status, image = next(s for s in STATUS if days < s[0])
                                                    with tag('div', klass='stimg'):
                                                        doc.stag('img', src='{}/{}'.format(args.plots_url, image))
                                                        text(status)

                                                with tag('td'): # dive of the mission parameters
                                                    text('{}'.format(mission['dive']))
                                                    if summary['latest']['cycle'] != mission['dive']:
                                                        doc.stag('br')
                                                        with tag('small'):
                                                            text('of {}'.format(summary['latest']['cycle']))

                                                current, used, phase_description = x['phases']
                                                for col, phase in TableCells():

                                                    param, fmt = col[3], col[4]
                                                    name, value = CellValue(x, param, phase)
                                                    stat = modes.get((param, phase))
                                                    klasses, style, title = [], '', name
                                                    if value is not None:
                                                        title += ' = {:g}'.format(value)
                                                    if param == 'MPS':
                                                        title += ': ' + phase_description
                                                    if phase is not None and phase == current:
                                                        klasses.append('mcur')
                                                        title += ' (current phase)'
                                                    error = CheckValue(mission, param, value, phase)
                                                    if value in UNSET_VALUES:
                                                        title += ': unknown (unset; the firmware sends {:g} before a mission is loaded)'.format(value)
                                                    elif phase is not None and phase not in used:
                                                        klasses.append('munused')
                                                        title += ': phase {:d} not used by the mission'.format(phase)
                                                    elif error:
                                                        klasses.append('merr')
                                                        title += ': ' + error
                                                    elif check_group and value is not None and stat is not None:
                                                        score, description = Abnormality(param, value, stat)
                                                        title += ': ' + description
                                                        if score >= MIN_SCORE:
                                                            style = ScoreStyle(score, value < stat[0])
                                                    with tag('td', klass=' '.join(klasses), style=style, title=title):
                                                        text(FormatValue(value, fmt))
                                                        if param == 'MPS' and value is not None and value not in UNSET_VALUES and mission.get('MPC') is not None:
                                                            doc.stag('br')
                                                            with tag('small'):
                                                                mnm = mission.get('MNM{:d}'.format(int(mission['MPS'])))
                                                                text('{:.0f}/{:.0f}'.format(mission['MPC'], mnm) if mnm is not None else '')

                                        except:
                                            errstr = 'Failed to create mission row for float {}'.format(float_id)

                                            if hasattr(args, 'halt') and args.halt:
                                                raise Exception(errstr)
                                            logger.error(errstr)

                                # Table footer: the group's most common values and typical ranges

                                with tag('tfoot'):
                                    with tag('tr'):
                                        with tag('td', colspan=6, klass='mfoot'):
                                            text('Most common')
                                        for col, phase in TableCells():
                                            param, fmt = col[3], col[4]
                                            with tag('td'):
                                                text(FormatValue(modes[param, phase][0], fmt) if (param, phase) in modes else '-')
                                                if param in MEDIAN_COLUMNS and (param, phase) in modes:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('median')
                                    with tag('tr'):
                                        with tag('td', colspan=6, klass='mfoot', title='Range of the middle {:.0f}% of the group\'s values'.format(100 - 2 * TYPICAL_RANGE_PERCENTILE)):
                                            text('Typical range')
                                            doc.stag('br')
                                            with tag('small'):
                                                text('{:.0f}th-{:.0f}th percentile'.format(TYPICAL_RANGE_PERCENTILE, 100 - TYPICAL_RANGE_PERCENTILE))
                                        for col, phase in TableCells():
                                            param, fmt = col[3], col[4]
                                            with tag('td'):
                                                if (param, phase) not in ranges:
                                                    text('-')
                                                elif ranges[param, phase][0] == ranges[param, phase][1]:
                                                    text(FormatValue(ranges[param, phase][0], fmt))
                                                else:
                                                    text(FormatValue(ranges[param, phase][0], fmt) + ' – ' + FormatValue(ranges[param, phase][1], fmt))

                        # Create footer

                        with tag('div', id='foot'):
                            with tag('div', id='lfoot'):
                                with tag('i'):
                                    WriteStatusFooter(doc, tag, text, JSON_VERSION, args.runtime)

        # Write mission file:

        with open(fpath, 'w', encoding='utf-8') as fout:
            fout.write(doc.getvalue())

    except:

        errstr = 'Failed to write mission file: ' + fpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


"""
Generate CLI arguments:
"""

parser = argparse.ArgumentParser(
    description='Generate the mission parameters page from aggregate JSON file(s).')

"""
Generate arguments - logging:
"""

parser.add_argument(
    '-d', '--debug',
    action='store_true',
    dest='debug',
    help='print debug output')

parser.add_argument(
    '-q', '--quiet',
    action='store_true',
    dest='quiet',
    help='suppress output, except for warnings and errors')

parser.add_argument(
    '-v', '--verbose',
    action='store_true',
    dest='verbose',
    help='print verbose output')

parser.add_argument(
    '-H', '--halt-on-error',
    action='store_true',
    dest='halt',
    help='halt execution on first error, otherwise continue processing')

"""
Generate arguments - paths:
"""

parser.add_argument(
    '-i', '--include',
    action='append',
    dest='include',
    help='include paths that match regular expression REGEX (e.g. "/alamo/data/json/.*/[0-9]+_summary.json")',
    metavar='REGEX')

parser.add_argument(
    '-o', '--output',
    default='.',
    dest='output',
    help='write params.html to specified directory (default=\'.\')',
    metavar='PATH')

parser.add_argument(
    '--plots-url',
    default='..',
    dest='plots_url',
    help='URL of the plots directory (float pages and img/) relative to the page (default=\'..\', '
         'for a page in plots2/engineering/)',
    metavar='URL')

parser.add_argument(
    '--days',
    type=float,
    dest='days',
    help='include only floats that reported in the last DAYS days (default: all deployed floats)',
    metavar='DAYS')

parser.add_argument(
    '--runtime',
    type=float,
    dest='runtime',
    help='show SECONDS as the runtime in the page footer (see xmtps-generate-summary-page.py)',
    metavar='SECONDS')

parser.add_argument(
    nargs='+',
    dest='path_in',
    help='specify one or more paths to directories and/or JSON summary files',
    metavar='PATH ...')


"""
Parse arguments:
"""

args = parser.parse_args()

logger = logging.getLogger('generate-mission')
if not logger.hasHandlers():
    ch = logging.StreamHandler()
    ch.setFormatter(logging.Formatter('%(asctime)s %(levelname)7s %(message)s', datefmt="%Y%m%dT%H%M%S"))
    logger.addHandler(ch)
if hasattr(args, 'debug')   and args.debug:
    logger.setLevel(logging.DEBUG)
elif hasattr(args, 'verbose') and args.verbose:
    logger.setLevel(logging.INFO)
elif hasattr(args, 'quiet')   and args.quiet:
    logger.setLevel(logging.ERROR)
else:
    logger.setLevel(logging.WARNING)

logger.debug('Debug output enabled.')


"""
Start of execution:
"""


try:

    if not hasattr(args, 'output') or not len(args.output):
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

    WriteMissionFile(args.output)


except:

    logger.error('Failed to generate mission file.')
    raise
