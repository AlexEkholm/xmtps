#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""Generate the ALAMO/ALTO float startup page from aggregate JSON files.

Reads the per-float <id>_summary.json files written by xmtps-aggregate-json.py (given as files
and/or directories, searched recursively and filtered by the -i regular expressions), ignoring
files whose JSON version is not JSON_VERSION. For each float, the <id>_engineering_data.json file
next to its summary gives the built-in test (BIT) results: one row per dive with a BIT message
(E05 passed, E06 failed), in time order (BitOrder). The float sends its startup messages with
negative dive numbers, decremented by one at each start, so the latest BIT's dive gives the
startup number (StartupNumber), and its reported start time the startup time (StartupEpoch).
With --days, floats that started up more than that many days ago are ignored.

Writes <output>/startup.html (production: plots2/engineering/, next to the mission parameters
page; the float links point to --plots-url) with one table row per float: its status (started,
i.e. not yet deployed, or deployed, with the startup number and the number of cycles), startup
and deployment dates, last report, and its most recent BIT: the test duration, the result with
the failed tests (from the BIT results mask), the subsystem error flags (CTD, case vacuum,
battery, air and oil pumps), the firmware and the main measurements, including the CTD pressure
after calibration. Failed tests and set error flags are highlighted. The output directory must
exist.

Example:
    xmtps-generate-startup-page.py -i"/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" -o/data1/alamo/data/plots2/engineering /data1/alamo/data/json

Author: aekholm
"""

import argparse
import datetime
import json
import logging
import jsonpickle
import math
import os
import re
from yattag import Doc

from PageStatus import WriteStatusFooter

"""
Static configuration:
"""


JSON_VERSION = "0.11"

# Default --days: floats that started up within this many days
DEFAULT_DAYS = 180

# BIT results mask bits (spec B.2.6; names from firmware 10.12.0, 0x400 inferred)
BIT_RESULT_BITS = [
    (0x001, 'air pump'),
    (0x002, 'oil pump'),
    (0x004, 'air valve'),
    (0x008, 'oil valve'),
    (0x010, 'GPS'),
    (0x020, 'CTD'),
    (0x040, 'satellite modem'),
    (0x080, 'case pressure'),
    (0x100, 'battery voltage'),
    (0x200, 'accelerometer'),
    (0x400, 'traveler (inferred)'),
]

# Per-subsystem error bits (spec B.2.6, BIT formats 9-11), by engineering_data column
BIT_ERROR_BITS = {
    'bit.ctd_errors':      [(0x01, 'unable to start profile'), (0x02, 'no data from CTD'),
                            (0x04, 'unable to retrieve profile'), (0x08, 'fields outside thresholds')],
    'bit.case_vac_errors': [(0x01, 'case pressure low'), (0x02, 'case pressure high')],
    'bit.battery_errors':  [(0x01, 'low voltage warning'), (0x02, 'low voltage error')],
    'bit.air_pump_errors': [(0x01, 'low pressure'), (0x02, 'high current'), (0x04, 'high cycle count')],
    'bit.oil_pump_errors': [(0x01, 'low current'), (0x02, 'high current warning'), (0x04, 'high current error')],
}

# Values the firmware sends for unset fields (U2, U4, and as scaled in the listing), and the
# listing's fill value for missing sensor values
UNSET_VALUES = (65535, 4294967295, 655.35, 65.535, 6553.5, -9.999)

# BIT measurement columns: (header group, header, units, engineering_data column(s), format).
# Two columns are shown as 'start / end'. Formats are str.format specifications.
BIT_COLUMNS = [
    ('CTD',       'Pressure',  'dBar', ('bit.pres_post_reset_dbar',),                     '{:.2f}'),
    ('CTD',       'Temp.',     'degC', ('bit.temp_degc',),                                '{:.3f}'),
    ('CTD',       'Saln.',     'PSU',  ('bit.saln_psu',),                                 '{:.3f}'),
    ('Case',      'Vacuum',    'inHg', ('bit.case_vac_start_inhg', 'bit.case_vac_end_inhg'), '{:.2f}'),
    ('Battery',   'Voltage',   'V',    ('bit.battery_start_v', 'bit.battery_end_v'),       '{:.2f}'),
    ('Air Pump',  'Duration',  's',    ('bit.air_pump_test_s',),                          '{:.0f}'),
    ('Air Pump',  'Cycles',    '',     ('bit.air_pump_cycles',),                          '{:.0f}'),
    ('Air Pump',  'Max. Pres.', 'PSI', ('bit.air_pump_max_pres_psi',),                    '{:.3f}'),
    ('Air Pump',  'currAvg',   'A',    ('bit.air_pump_current_avg_a',),                   '{:.3f}'),
    ('Air Pump',  'currMax',   'A',    ('bit.air_pump_current_max_a',),                   '{:.3f}'),
    ('Air Pump',  '12VCurrMax', 'A',   ('bit.air_pump_current_max_12v_a',),               '{:.3f}'),
    ('Air Pump',  'batMax',    'V',    ('bit.air_pump_battery_max_v',),                   '{:.3f}'),
    ('Air Pump',  'powerMax',  'W',    ('bit.air_pump_power_max_w',),                     '{:.3f}'),
    ('Oil Pump',  'Duration',  's',    ('bit.oil_pump_test_s',),                          '{:.0f}'),
    ('Oil Pump',  'currAvg',   'A',    ('bit.oil_pump_current_avg_a',),                   '{:.3f}'),
    ('Oil Pump',  'currMax',   'A',    ('bit.oil_pump_current_max_a',),                   '{:.3f}'),
    ('Oil Pump',  '12VCurrMax', 'A',   ('bit.oil_pump_current_max_12v_a',),               '{:.3f}'),
    ('Oil Pump',  'batMax',    'V',    ('bit.oil_pump_battery_max_v',),                   '{:.3f}'),
    ('Oil Pump',  'powerMax',  'W',    ('bit.oil_pump_power_max_w',),                     '{:.3f}'),
]

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
    """Return True if fpath matches an -i pattern, or if none is given and it is a summary file."""
    if args.include is None or not len(args.include):
        return re.search('[0-9]+_summary.json$', fpath) is not None
    for re_include in args.include:
        if re.search(re_include, fpath):
            return True
    logger.debug('Ignoring file not included by REGEX: ' + fpath + ' ...')
    return False


def IsSet(value):
    """Return True if a JSON value is a real value: not None, NaN, an empty string or unset."""
    if value is None or value == '':
        return False
    if isinstance(value, float) and math.isnan(value):
        return False
    return value not in UNSET_VALUES


def EpochOrNone(dt):
    """Return the epoch seconds of a FloatClasses.DateTime, or None if it is unset (NaN or 0)."""
    epoch = getattr(dt, 'dt_epoch', None)
    if epoch is None or (isinstance(epoch, float) and math.isnan(epoch)) or epoch <= 0:
        return None
    return epoch


def BitStartEpoch(bit):
    """Return the start time of a BIT in epoch seconds (BIT formats 9-11), or None."""
    dt = bit.get('bit.dt_start') if bit else None
    if not IsSet(dt) or dt.startswith('0000'):
        return None
    return datetime.datetime.strptime(dt[0:19], '%Y-%m-%dT%H:%M:%S').replace(tzinfo=datetime.UTC).timestamp()


def BitOrder(bit):
    """Sort key putting a float's BITs in time order: by start time (BIT formats 9-11), else by
    dive. The float sends its startup messages with negative dive numbers, decremented by one at
    each start (-1, -2, ...), so a lower negative dive is later."""
    dive = bit.get('dive') or 0
    return (BitStartEpoch(bit) or 0, -dive if dive < 0 else dive)


def StartupNumber(x):
    """Return how many times the float has been started: minus the dive number of its latest BIT
    (startup messages have negative dive numbers, decremented at each start), else of its latest
    dive if that is negative, or None."""
    dive = x['bits'][-1].get('dive') if x['bits'] else None
    if dive is None or dive >= 0:
        dive = x['summary']['latest'].get('cycle')
    return -dive if dive is not None and dive < 0 else None


def StartupEpoch(x):
    """Return the float's startup time in epoch seconds: the start time reported in its latest BIT,
    else the time of its first SBD message (the summary's startup time), or None."""
    return BitStartEpoch(x['bits'][-1] if x['bits'] else None) \
        or EpochOrNone(x['summary'].get('startup', {}).get('sbd'))


def ProcessJsonSummaryFile(fpath):
    """Read one <id>_summary.json file and the BIT rows of the <id>_engineering_data.json file next
    to it into float_map.

    Floats without a startup time and, with --days, floats that started up longer ago are
    ignored, as are summaries with a JSON version other than JSON_VERSION (with a warning). A
    float without BIT messages is kept, with no BIT rows. Errors are logged, or raised with -H.
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

        float_id = summary['id'].whoi

        # BIT rows: the dives whose engineering data has a BIT section, in dive order

        bits = []
        fpath_eng = re.sub('\\\\', '/', os.path.join(os.path.dirname(fpath), '{}_engineering_data.json'.format(float_id)))
        if os.path.isfile(fpath_eng):
            with open(fpath_eng, 'r') as file:
                eng = json.load(file)
            for row in eng['data']:
                values = dict(zip(eng['columns'], row))
                if IsSet(values.get('bit.version')) or IsSet(values.get('bit.results')):
                    bits.append(values)
            bits.sort(key=BitOrder)
        else:
            logger.info('Float without engineering data file: {}'.format(float_id))

        x = { 'summary': summary, 'bits': bits }

        startup = StartupEpoch(x)
        if startup is None:
            logger.debug('Ignoring float without startup time: {}'.format(fpath))
            return

        if (datetime.datetime.now(datetime.UTC).timestamp() - startup) / 86400 > args.days:
            logger.debug('Ignoring float not started up in the last {} days: {}'.format(args.days, fpath))
            return

        float_map[float_id] = x

    except:

        errstr = 'Failed to process file: ' + fpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


def ParseMask(value):
    """Return a '0x...' bitmask string as an int, or None if it is not one."""
    if isinstance(value, str) and re.fullmatch('0x[0-9A-Fa-f]+', value):
        return int(value, 16)
    if isinstance(value, (int, float)) and IsSet(value):
        return int(value)
    return None


def MaskNames(mask, bits):
    """Return the names of the bits set in mask, plus any undocumented bits as one hex value."""
    names = [name for bit, name in bits if mask & bit]
    rest = mask & ~sum(bit for bit, _ in bits)
    if rest:
        names.append('other: 0x{:X}'.format(rest))
    return names


def BitFailures(bit):
    """Return (results mask or None, failed test names, [subsystem error descriptions]) of a BIT
    row."""
    results = ParseMask(bit.get('bit.results'))
    failed = MaskNames(results, BIT_RESULT_BITS) if results else []
    errors = []
    for column, bits in BIT_ERROR_BITS.items():
        mask = ParseMask(bit.get(column))
        if mask:
            subsystem = column[4:-7].replace('_', ' ')
            errors.append('{} (0x{:X}): {}'.format(subsystem, mask, ', '.join(MaskNames(mask, bits))))
    return results, failed, errors


def FirmwareString(fwver):
    """Format a firmware word '0xMMmmppbb' as on the summary page, e.g. 'v10.12.1', or return '' if
    it is not one."""
    if not re.fullmatch('0x[0-9A-Fa-f]{8}', fwver or ''):
        return ''
    return 'v{:02d}.{}.{}'.format(int(fwver[2:4], 16), int(fwver[4:6], 16), int(fwver[6:8], 16))


def FormatValue(bit, columns, fmt):
    """Format one or two (start / end) BIT values, '-' for missing values."""
    return ' / '.join(fmt.format(bit[c]) if IsSet(bit.get(c)) else '-' for c in columns)


def FormatTime(epoch):
    """Format epoch seconds as 'YYYY-MM-DD HH:MM'."""
    return datetime.datetime.fromtimestamp(epoch, datetime.UTC).strftime('%Y-%m-%d %H:%M')


def WriteBitCells(doc, tag, text, bit):
    """Write the cells of one BIT: duration, result, errors, firmware and the BIT_COLUMNS values. A
    float without BIT messages (bit is None) gets one cell per column (DataTables does not allow
    cells spanning columns in the body), its first cell saying so."""

    if bit is None:
        with tag('td'):
            text('No BIT messages received')
        for _ in range(3 + len(BIT_COLUMNS)):
            with tag('td'):
                text('-')
        return

    results, failed, errors = BitFailures(bit)

    duration = bit.get('bit.duration_s') if IsSet(bit.get('bit.duration_s')) else None
    with tag('td', ('data-order', '{:.0f}'.format(duration) if duration is not None else '-1')): # duration
        text('{:.0f}'.format(duration / 60) if duration is not None else '-')

    # result: from the results mask (E05 passed, E06 failed)
    if results is None:
        with tag('td'):
            text('-')
    elif results == 0:
        with tag('td', klass='bpass', title='BIT results 0x{:08X}'.format(results)):
            text('Passed')
    else:
        with tag('td', klass='bfail', title='BIT results 0x{:08X}'.format(results)):
            text('Failed')
            doc.stag('br')
            with tag('small'):
                text(', '.join(failed))

    with tag('td', klass='berr' if errors else ''): # subsystem errors
        if errors:
            for i, error in enumerate(errors):
                if i:
                    doc.stag('br')
                text(error)
        else:
            text('none' if any(c in bit for c in BIT_ERROR_BITS) else '-')

    with tag('td'): # firmware (as on the summary page) and BIT format
        fwver = bit.get('bit.sw_version') or ''
        if FirmwareString(fwver):
            text(FirmwareString(fwver))
            doc.stag('br')
            with tag('small'):
                text(fwver + (', BIT v{:.0f}'.format(bit['bit.version']) if IsSet(bit.get('bit.version')) else ''))
        else:
            text(fwver or '-')
            if IsSet(bit.get('bit.version')):
                doc.stag('br')
                with tag('small'):
                    text('BIT v{:.0f}'.format(bit['bit.version']))

    for _, _, _, columns, fmt in BIT_COLUMNS:
        with tag('td'):
            text(FormatValue(bit, columns, fmt))


def WriteBitHeader(doc, tag, text, float_columns):
    """Write the two header rows of a BIT table: float_columns ((header, units) pairs) first."""
    with tag('tr'):
        with tag('th', colspan=len(float_columns)):
            text('Float')
        with tag('th', colspan=4):
            text('BIT')
        groups = []
        for c in BIT_COLUMNS:
            if groups and groups[-1][0] == c[0]:
                groups[-1][1] += 1
            else:
                groups.append([c[0], 1])
        for name, n in groups:
            with tag('th', colspan=n):
                text(name)
    with tag('tr'):
        headers = list(float_columns) + [('Duration', 'min'), ('Result', ''), ('Errors', ''), ('Firmware', '')]
        headers += [(c[1], c[2]) for c in BIT_COLUMNS]
        for header, units in headers:
            with tag('th'):
                text(header)
                if units:
                    doc.stag('br')
                    with tag('small'):
                        text('[' + units + ']')


def WriteStartupFile(dpath):
    """Write <dpath>/startup.html from float_map: one row per float with its most recent BIT.

    Errors are logged, or raised with -H.
    """

    fpath = os.path.join(dpath, 'startup.html')

    try:

        logger.info('Writing startup file: ' + fpath + ' ...')

        now = datetime.datetime.now(datetime.UTC).timestamp()

        # Floats, latest startup first
        float_ids = sorted(float_map, key=lambda i: StartupEpoch(float_map[i]), reverse=True)

        # Create document

        doc, tag, text, line = Doc().ttl()

        doc.asis('<!DOCTYPE html>')
        with tag('html'):

            with tag('head'):

                doc.stag('meta', charset='utf-8')

                with tag('title'):
                    text('Float Startup')

                doc.asis('<script type="text/javascript" src="https://cdn.datatables.net/v/dt/jq-3.6.0/dt-1.12.1/datatables.min.js"></script>')
                doc.asis('<link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/v/dt/dt-1.12.1/datatables.min.css"/>')
                doc.asis('<script type="text/javascript">$(document).ready(function () { $("table#sfloats").DataTable({ order: [[3, \'desc\']], paging: false, info: false, searching: true }); jQuery(".dataTable").wrap("<div class=\'tat\' />"); });</script>')

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
                    text('table.stable { width: 100%; }')
                    text('table.stable th { padding: 6px 4px; } ')
                    text('table.stable td { padding: 6px 4px; text-align: left; height: 28px; white-space: nowrap; } ')
                    text('table.stable td small, table.stable th small { color: #aaa; } ')
                    text('table.stable td.bfail small, table.stable td.berr small { color: #fff; } ')
                    text('.bpass { background-color: #c8e6c9 !important; } ')
                    text('.bfail, .berr { background-color: #b71c1c !important; color: #fff; font-weight: bold; } ')
                    text('.bstart { background-color: #fff3c4 !important; } ')
                    text('.mkey { display: inline-block; padding: 1px 6px; } ')

                    # Create styles - footer

                    text('#foot { font-size: 12px; color: #aaa; margin: 30px 0px; width: 100%;   } ')
                    text('#lfoot { float: left; width: 50%; white-space: preserve; margin: 0 0 30px 0;  } ')
                    text('#lfoot i { display: inline-block; margin 3px; } ')

            with tag('body'):

                with tag('div', id='top'):

                    with tag('div', klass='main'):

                        with tag('div', id='tit'):
                            with tag('h1'):
                                text('ALAMO/ALTO Float Startup')

                        with tag('div', id='intro'):
                            text('Floats that started up in the last {:g} days (the start time of their latest built-in test, BIT), '.format(args.days))
                            text('both ')
                            with tag('span', klass='mkey bstart'):
                                text('started')
                            text(' (not yet deployed) and deployed, with the results of their most recent BIT: ')
                            with tag('span', klass='mkey bpass'):
                                text('passed')
                            text(', or ')
                            with tag('span', klass='mkey bfail'):
                                text('failed')
                            text(' with the failed tests, and any subsystem error flags. ')
                            text('The startup number counts the float\'s starts (its startup messages have dive numbers -1, -2, ...). ')
                            text('The BITs column counts all the BITs the float sent. Values are as reported by the float; ')
                            text('a dash means the BIT format does not report the value. Hover over a result for its results mask.')

                        # Floats table

                        with tag('h2'):
                            text('Recent Startups ')
                            with tag('small'):
                                text('({:d})'.format(len(float_ids)))

                        float_columns = [('ID', ''), ('Type', ''), ('Status', ''), ('Startup', 'UTC'), ('Deployment', 'UTC'), ('Last Report', 'UTC'), ('BITs', '')]

                        with tag('table', id='sfloats', klass='stable stripe'):

                            with tag('thead'):
                                WriteBitHeader(doc, tag, text, float_columns)

                            with tag('tbody'):
                                for float_id in float_ids:

                                    try:

                                        x = float_map[float_id]
                                        summary = x['summary']
                                        bits = x['bits']
                                        cycle = summary['latest'].get('cycle', -1)
                                        in_startup = not (cycle >= 1 and summary.get('profdepthsum', 0) > 0)

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
                                                if len(summary.get('ctdtype', '')) > 0:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text(summary['ctdtype'])

                                            # status: started (not yet deployed) or deployed, with the startup number
                                            number = StartupNumber(x)
                                            with tag('td', klass='bstart' if in_startup else ''):
                                                text('Started' if in_startup else 'Deployed')
                                                doc.stag('br')
                                                with tag('small'):
                                                    details = ['startup {}'.format(number)] if number is not None else []
                                                    if not in_startup:
                                                        details.append('{} cycle{}'.format(cycle, '' if cycle == 1 else 's'))
                                                    text(', '.join(details) or '-')

                                            for key in ('startup', 'deployment', 'latest'):
                                                if key == 'startup':
                                                    epoch = StartupEpoch(x)
                                                elif key == 'deployment' and in_startup:
                                                    epoch = None
                                                else:
                                                    epoch = EpochOrNone(summary.get(key, {}).get('sbd'))
                                                with tag('td', ('data-order', '{:.0f}'.format(epoch) if epoch is not None else '0')):
                                                    if epoch is None:
                                                        text('-')
                                                    else:
                                                        text(FormatTime(epoch))
                                                        doc.stag('br')
                                                        with tag('small'):
                                                            text('{:.0f} days ago'.format((now - epoch) / 86400))

                                            num_failed = sum(1 for b in bits if ParseMask(b.get('bit.results')))
                                            with tag('td'): # BIT count
                                                text('{:d}'.format(len(bits)))
                                                if num_failed:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text('{:d} failed'.format(num_failed))

                                            WriteBitCells(doc, tag, text, bits[-1] if bits else None)

                                    except:
                                        errstr = 'Failed to create startup row for float {}'.format(float_id)

                                        if hasattr(args, 'halt') and args.halt:
                                            raise Exception(errstr)
                                        logger.error(errstr)

                        # Create footer

                        with tag('div', id='foot'):
                            with tag('div', id='lfoot'):
                                with tag('i'):
                                    WriteStatusFooter(doc, tag, text, JSON_VERSION, args.runtime)

        # Write startup file:

        with open(fpath, 'w', encoding='utf-8') as fout:
            fout.write(doc.getvalue())

    except:

        errstr = 'Failed to write startup file: ' + fpath

        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)


"""
Generate CLI arguments:
"""

parser = argparse.ArgumentParser(
    description='Generate the float startup page from aggregate JSON file(s).')

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
    help='write startup.html to specified directory (default=\'.\')',
    metavar='PATH')

parser.add_argument(
    '--plots-url',
    default='..',
    dest='plots_url',
    help='URL of the plots directory (float pages) relative to the page (default=\'..\', '
         'for a page in plots2/engineering/)',
    metavar='URL')

parser.add_argument(
    '--days',
    type=float,
    default=DEFAULT_DAYS,
    dest='days',
    help='include floats that started up in the last DAYS days (default={:d})'.format(DEFAULT_DAYS),
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

logger = logging.getLogger('generate-startup')
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
            logger.error('Invalid path: ' + path)

    WriteStartupFile(args.output)


except:

    logger.error('Failed to generate startup file.')
    raise
