#!/usr/bin/python3
#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""Convert XMTPS decoder listing files into per-dive JSON files.

Parses listing files (lis<SN>-<dive>.txt, see "Listing files" in the XMTPS README) line by line
into a FloatClasses Dive object: float IDs, GPS fixes, SBD messages, mission configuration,
engineering data, drift, profile, descent, ascent and surface time series, pump records and wave
spectra. Density and sigma0 are added to the drift and profile samples from RhoLUT. Each dive is
written with jsonpickle to <output>/<serial>/dives/<serial>_<dive>.json, creating the dives
directory if needed.

Inputs are listing files and/or directories (subdirectories only with -r), filtered by the -i
include and -x exclude regular expressions. Unless -f is given, a listing is skipped when its
JSON file is newer than it, and with -s whenever its JSON file exists.

Example:
    xmtps-list2json.py -v -i"/data1/alamo/data/list/(908|909|91|92|93|94|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o/data1/alamo/data/json /data1/alamo/data/list/<id>

Author: aekholm
"""

import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import argparse
import gc
# import datetime
# import dateutil
# import json
import jsonpickle
import logging
# import math
import numpy as np
import os
# import pprint
import re

# local
from FloatClasses import DateTime, Dive, Drift, FloatId, Gps, Sbd, Profile, Wave
import RhoLUT

"""
Generate arguments:
"""

parser = argparse.ArgumentParser(
    description='Generate JSON files from XMTPS listing file(s).')

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

"""
Generate arguments - file updating:
"""

parser.add_argument(
    '-f', '--force',
    action='store_true',
    dest='force',
    help='force overwriting of json files, even if listing files are not newer')

parser.add_argument(
    '-H', '--halt-on-error',
    action='store_true',
    dest='halt',
    help='halt execution on first error, otherwise continue processing')

parser.add_argument(
    '-r', '--recursive',
    action='store_true',
    dest='recursive',
    help='descend into subdirectories')

parser.add_argument(
    '-s', '--skip',
    action='store_true',
    dest='skip',
    help='skip directories with existing json files, even if listing files are newer')

"""
Generate arguments - directories:
"""

parser.add_argument(
    '-i', '--include',
    action='append',
    dest='include',
    help='include paths that match regular expression REGEX (e.g. "/alamo/data/list/[0-9]+/lis.*\\.txt")',
    metavar='REGEX')

parser.add_argument(
    '-o', '--output',
    default='.',
    dest='output',
    help='write json files to specified directory (default=\'.\')',
    metavar='DIRECTORY')

parser.add_argument(
    '-S', '--strip-path',
    default='.*/alamo/',
    dest='strip',
    help='strip string from path(s) written to JSON file (default=\'.*/alamo/\')',
    metavar='REGEX')

parser.add_argument(
    '-x', '--exclude',
    action='append',
    dest='exclude',
    help='exclude paths that match regular expression REGEX',
    metavar='REGEX')

parser.add_argument(
    # action='append',
    nargs='+',
    dest='path_in',
    help='specify one or more paths to directories and/or XMTPS listing files',
    metavar='PATH ...')

"""
Global Variables:
"""

JSON_VERSION = "0.11"

#float_map = {}

MIN_TEMP = RhoLUT.T[0]
MIN_SALN = RhoLUT.S[0]


"""
Function Definitions:
"""

def ProcessDirectory(dpath):
    """Process every listing file in a directory, descending into subdirectories with -r.

    Files are processed in sorted order with ProcessListingFile, which applies the -i and -x
    filters; subdirectories matching an -x pattern are skipped. Empty directories are ignored with
    a warning. Errors are logged, or raised with -H.
    """

    dpath = re.sub('\\\\', '/', dpath)
    
    try: 
    
        # Check directory:
        if not os.path.exists(dpath) or not os.path.isdir(dpath):
                
            raise Exception('Invalid input directory: ' + dpath)
                
        logger.debug('Processing directory: ' + dpath + ' ...')
        
        # Check if directory is empty
        if len(os.listdir(dpath)) == 0:
            
            logger.warning('Ignoring empty directory: ' + dpath)
            # total_empty_dirs += 1
            return
        
        # Iterate over files in directory:
        for file in sorted(os.listdir(dpath)):
            
            # Process subdirectories
            fpath_child = re.sub('\\\\', '/', os.path.join(dpath, file))
            if os.path.isdir(fpath_child):
                if args.recursive:
                    
                    # TODO Check exclusions
                    if args.exclude is not None and len(args.exclude):
                        exclude = False
                        
                        for re_exclude in args.exclude:
                            if re.search(re_exclude, fpath_child):
                                exclude = True
                                break
                            
                        if exclude:
                            logger.debug('Ignoring directory excluded by REGEX: ' + fpath_child + ' ...')
                            continue
                        
                        del re_exclude
                        del exclude
                    
                    ProcessDirectory(fpath_child)
                else:
                    logger.debug('Ignoring directory: ' + fpath_child)
                    # total_ignored_dirs +=1
                continue;
                
            if not os.path.isfile(fpath_child):
                logger.debug('Not file or directory?: ' + fpath_child)
               
            try: 
                
                # Process the file:
                # dive_data = ProcessListingFile(os.path.join(dpath, file))
                ProcessListingFile(fpath_child)

            except:
                
                # Error handling
                
                if hasattr(args, 'halt') and args.halt:
                    
                    raise Exception('Failed to process file: ' + file)
                    
                else:
                    
                    logger.error('Failed to process file: ' + file)
            
    except:
        
        # Error handling
        
        if hasattr(args, 'halt') and args.halt:
            
            raise Exception('Failed to process directory: '  + dpath)
            
        else:
            
            logger.error('Failed to process directory: '  + dpath)
        
    # logger.debug('Processed directory: ' + dpath + ' ')

def ColumnValues(columns, x):
    """Map the fields of a pro/d data line onto the latest column header.

    columns: the names from the last '#      SN dive   sampleIdx ...' (or fall/rise-style
             '#      SN dive idx nSam ...', or drift '#      SN dive nSam ...') header line.
    x:       the split data line, whose first field is the line prefix.
    Returns a dict of name -> string, or None if the line does not match the header.
    """
    if not columns or len(columns) + 1 != len(x):
        return None
    return { name: x[i + 1] for i, name in enumerate(columns) }

ENG_SECTIONS = { 'E05': 'bit', 'E06': 'bit', 'E07': 'descent', 'E08': 'drift', 'E09': 'ascent',
                 'E0A': 'surface', 'E0C': 'ice' }

# Listing column -> attribute of the FloatClasses Engineering* section objects, for every
# engineering message version. A key is the column name, 'name|units' where the same name is
# listed in physical units and in counts, or 'sub-group/name' where BIT versions 9-11 reuse
# names in different listing sub-groups.
ENG_COLUMNS = {
    'descent': {
        'engVer': 'version', 'start-of-descent': 'dt_start', 'vac0': 'vac0', 'vac1': 'vac1',
        'seeks': 'seeks', 'seekPumpAdjT': 'seek_pump_adj_s',
        'OilValveOpenPosSurf': 'oil_valve_open_pos_surf_v',
        'OilValveClosePosFallPump': 'oil_valve_close_pos_fall_pump_v',
        'AirValveMidPosSurf': 'air_valve_mid_pos_surf_v',
        'pumpT': 'pump_time_s', 'pumpI': 'pump_current_a', 'sysI': 'system_current_a',
        'sysV': 'system_voltage_v', 'vac': 'case_vac_inhg', 'pumpAdj': 'first_pump_adj_s',
        },
    'drift': {
        'engVer': 'version', 'start-of-drift': 'dt_start', 'vac0': 'vac0', 'vac1': 'vac1',
        'seeks': 'seeks', 'seekPumpAdjT': 'seek_pump_adj_s',
        'meanP': 'mean_pres_dbar', 'meanT': 'mean_temp_degc',
        'start-of-fall': 'dt_fall_start', 'fallP': 'fall_start_pres_dbar',
        'pres': 'mean_pres_dbar', 'pres|counts': 'mean_pres_counts',
        'temp': 'mean_temp_degc', 'temp|counts': 'mean_temp_counts',
        'saln': 'mean_saln_psu', 'saln|counts': 'mean_saln_counts',
        },
    'ascent': {
        'engVer': 'version', 'start-of-ascent': 'dt_start', 'vac0': 'vac0', 'vac1': 'vac1',
        'sysI': 'system_current_a', 'pumpT': 'pump_time_s',
        'start-of-fall': 'dt_fall_start', 'fallP': 'fall_start_pres_dbar',
        },
    'surface': {
        'engVer': 'version', 'start-of-surface': 'dt_start', 'vac0': 'vac0', 'vac1': 'vac1',
        'CTD-status': 'ctd_status', 'vac': 'case_vac_inhg', 'sysV': 'system_voltage_v',
        'sysI': 'system_current_a', 'nSBD': 'num_sbd', 'nAtmpt': 'num_prev_attempts',
        'nPrevAttempt': 'num_prev_attempts',
        'APCur': 'air_pump_current_a', 'APRunT': 'air_pump_runtime_s',
        'AirBladMxP': 'air_bladder_max_pres_psi',
        'AVOpenStart': 'air_valve_open_start_v', 'AVMidEnd': 'air_valve_mid_end_v',
        'ACVpenCal': 'air_valve_open_cal_v', 'AVMidCal': 'air_valve_mid_cal_v',
        'AVCloseCal': 'air_valve_close_cal_v',
        'OVOpenAirPmp': 'oil_valve_open_air_pump_v', 'OVCloseEnd': 'oil_valve_close_end_v',
        'OVOpenCal': 'oil_valve_open_cal_v', 'OVClosCal': 'oil_valve_close_cal_v',
        'presOffset': 'pres_offset_dbar',
        'firstPres': 'prof_first_pres_dbar', 'firstPres|counts': 'prof_first_pres_counts',
        'firstTemp': 'prof_first_temp_degc', 'firstTemp|counts': 'prof_first_temp_counts',
        'firstSaln': 'prof_first_saln_psu', 'firstSaln|counts': 'prof_first_saln_counts',
        'lastPres': 'prof_last_pres_dbar', 'lastPres|counts': 'prof_last_pres_counts',
        'lastTemp': 'prof_last_temp_degc', 'lastTemp|counts': 'prof_last_temp_counts',
        'lastSaln': 'prof_last_saln_psu', 'lastSaln|counts': 'prof_last_saln_counts',
        'numBins': 'prof_num_bins', 'numEmptyBins': 'prof_num_empty_bins',
        'numRawSamples': 'prof_num_raw_samples', 'gpsOffset': 'gps_rtc_offset_s',
        'start': 'dt_sampling_start', 'end': 'dt_sampling_end', 'nErr': 'num_errors',
        },
    'ice': {
        'engVer': 'version', 'mode': 'mode', 'ice-status': 'ice_status', 'urgency': 'urgency',
        'surfAttempts': 'surface_attempts', 'numStoredSBD': 'num_stored_sbd',
        'lastXmit': 'time_since_last_xmit_s', 'minPres': 'min_pres_dbar',
        'testTemp': 'test_temp', 'testSalinity': 'test_saln_psu', 'nErr': 'num_errors',
        },
    'bit': {
        # BIT versions 6-8
        'BITVer': 'version', 'SWVer': 'sw_version', 'BITResults': 'results',
        'BITTime': 'duration_s', 'CTDStat': 'ctd_status',
        'pres': 'pres_dbar', 'pres|counts': 'pres_counts',
        'presPostReset': 'pres_post_reset_dbar', 'presPostReset|counts': 'pres_post_reset_counts',
        'presPreReset': 'pres_pre_reset_dbar', 'presPreReset|counts': 'pres_pre_reset_counts',
        'presOffset': 'pres_offset_dbar',
        'temp': 'temp_degc', 'temp|counts': 'temp_counts',
        'saln': 'saln_psu', 'saln|counts': 'saln_counts',
        'VacStart': 'case_vac_start_inhg', 'VacEnd': 'case_vac_end_inhg',
        'BatStart': 'battery_start_v', 'BatEnd': 'battery_end_v',
        'AVOpenPos': 'air_valve_open_v', 'AVOpenPosCal': 'air_valve_open_cal_v',
        'AVMidPos': 'air_valve_mid_v', 'AVMidPosCal': 'air_valve_mid_cal_v',
        'AVClosePos': 'air_valve_closed_v', 'AVClosePosCal': 'air_valve_closed_cal_v',
        'APTestTime': 'air_pump_test_s', 'APCycleCount': 'air_pump_cycles',
        'APMxAPres': 'air_pump_max_pres_psi', 'APMxLoadCur': 'air_pump_current_max_a',
        'APMxLoadCur12V': 'air_pump_current_max_12v_a', 'APMxLoadBatV': 'air_pump_battery_max_v',
        'OVOpenPos': 'oil_valve_open_v', 'OVOpenPosCal': 'oil_valve_open_cal_v',
        'OVClosePos': 'oil_valve_closed_v', 'OVClosePosCal': 'oil_valve_closed_cal_v',
        'OPTestTime': 'oil_pump_test_s', 'OPMxLoadCur': 'oil_pump_current_max_a',
        'OPMxLoadCur12V': 'oil_pump_current_max_12v_a', 'OPMxLoadBatV': 'oil_pump_battery_max_v',
        # BIT versions 9-11
        'FWVer': 'sw_version', 'sensorType': 'sensor_type', 'sensorTypeName': 'sensor_type_name',
        'startTime': 'dt_start', 'endTime': 'dt_end', 'Date/Time/duration': 'duration_s',
        'CTD/errors': 'ctd_errors', 'CTD/status': 'ctd_status',
        'presPreCal': 'pres_pre_reset_dbar', 'presPostCal': 'pres_post_reset_dbar',
        'Case Vacuum/errors': 'case_vac_errors', 'caseVacStart': 'case_vac_start_inhg',
        'caseVacEnd': 'case_vac_end_inhg',
        'Battery Voltage/errors': 'battery_errors', 'batVoltStart': 'battery_start_v',
        'batVoltEnd': 'battery_end_v',
        'Air Valve Positions/open': 'air_valve_open_v', 'Air Valve Positions/mid': 'air_valve_mid_v',
        'Air Valve Positions/closed': 'air_valve_closed_v',
        'Air Valve Calibration Positions/open': 'air_valve_open_cal_v',
        'Air Valve Calibration Positions/mid': 'air_valve_mid_cal_v',
        'Air Valve Calibration Positions/closed': 'air_valve_closed_cal_v',
        'Air Pump/errors': 'air_pump_errors', 'Air Pump/duration': 'air_pump_test_s',
        'Air Pump/cycles': 'air_pump_cycles', 'Air Pump/presMax': 'air_pump_max_pres_psi',
        'Air Pump/currAvg': 'air_pump_current_avg_a', 'Air Pump/currMax': 'air_pump_current_max_a',
        'Air Pump/12VCurrMax': 'air_pump_current_max_12v_a',
        'Air Pump/batMax': 'air_pump_battery_max_v', 'Air Pump/powerMax': 'air_pump_power_max_w',
        'Oil Valve Positions/open': 'oil_valve_open_v', 'Oil Valve Positions/closed': 'oil_valve_closed_v',
        'Oil Valve Calibration Positions/open': 'oil_valve_open_cal_v',
        'Oil Valve Calibration Positions/closed': 'oil_valve_closed_cal_v',
        'Oil Pump/errors': 'oil_pump_errors', 'Oil Pump/duration': 'oil_pump_test_s',
        'Oil Pump/currAvg': 'oil_pump_current_avg_a', 'Oil Pump/currMax': 'oil_pump_current_max_a',
        'Oil Pump/12VCurrMax': 'oil_pump_current_max_12v_a',
        'Oil Pump/batMax': 'oil_pump_battery_max_v', 'Oil Pump/powerMax': 'oil_pump_power_max_w',
        },
    }

def ParseValue(s):
    """Convert a listing field to an int or float where possible; otherwise keep the string.

    Only plain integers ('42', '-7') and decimal numbers with a decimal point ('1.5', '.5', '2.',
    '1.5e3') are converted. Anything else stays a string, including hex values ('0x10'),
    exponents without a decimal point ('1e5'), 'nan' and 'inf'.
    """
    if re.fullmatch(r'[+-]?\d+', s):
        return int(s)
    if re.fullmatch(r'[+-]?(\d+\.\d*|\.\d+)([eE][+-]?\d+)?', s):
        return float(s)
    return s

WAVE_SPECTRA_FILL = -999.99999    # XWaveSpectralSample::SPECTRA_FILL_VALUE

def ParseWaveSpectrum(s):
    """Convert a wvs spectral value, in g^2/Hz, to a float, or NaN for the fill value of a bin that
    was not received."""
    v = float(s)
    return np.nan if v <= WAVE_SPECTRA_FILL + 0.5e-5 else v

def HeaderFields(names_line, units_line):
    """Return the (name, unit) pairs for the columns of an engineering header after 'SN dive midx'.

    The listing right-aligns each unit under its column name, so a unit is matched to a name when
    the right edges of their text are within 2 characters; the unit is '' when none is. Units are
    returned without their parentheses, e.g. 'dBar' for '(dBar)'.

    Args:
        names_line: the '#      SN dive midx ...' (or 'idx') header line.
        units_line: the units line under it, or None.
    """
    names = [(m.group(), m.end()) for m in re.finditer(r'\S+', names_line[1:])][3:]
    units = [(m.group().strip('()'), m.end()) for m in re.finditer(r'\S+', units_line[1:])] \
        if units_line else []
    return [(name, next((u for u, e in units if abs(e - end) <= 2), '')) for name, end in names]

def ParseEngineeringLine(engineering, line, header, units_line, group):
    """Store one engineering (E0x) or engineering error (e0x) listing line.

    Each column is stored in the section object's attribute given by ENG_COLUMNS (looked up by
    'sub-group/name', then 'name|unit', then 'name'); date/time attributes get a DateTime.
    Columns not in ENG_COLUMNS go into engineering.extra[section]; when a column name is already
    there (e.g. 'pres' in dBar and in counts), the key gets a '_<unit>' suffix ('pres_counts').
    Error lines ('e0x SN dive idx date-time err-code', but surface engineering v0x10 error lines
    have no date-time) go into engineering.errors as dicts with 'section', 'idx' and one entry per
    column of their header ('date-time' stored as 'dt_str', 'err-code' as 'code').

    Args:
        engineering: the Dive's engineering object to fill in.
        line: the listing line.
        header: the latest '#      SN dive midx ...' (or 'idx') header line, or None.
        units_line: the units line under the header, or None.
        group: the current '# --- Name ---' sub-group name ('' outside one), used for the
            'sub-group/name' keys of ENG_COLUMNS.

    Raises:
        Exception: if the number of data fields does not match the header; the caller logs this
            at debug level and skips the line.
    """
    x = line.split('#', 1)[0].split()
    section = ENG_SECTIONS.get(x[0].upper(), x[0].lower())
    fields = HeaderFields(header, units_line) if header else []
    data = x[4:]
    # A last column listed as a comment, e.g. the BIT sensorTypeName ('... 0 # RBR')
    if '#' in line and len(fields) == len(data) + 1:
        data.append(line.split('#', 1)[1].strip())
    if len(fields) != len(data):
        raise Exception('Engineering line does not match its header: \'' + line + '\'')

    if x[0][0] == 'e':
        # Error line: SN dive idx date-time err-code
        error = { 'section': section, 'idx': int(x[3]) }
        for (name, unit), value in zip(fields, data):
            error[{ 'date-time': 'dt_str', 'err-code': 'code' }.get(name, name)] = ParseValue(value)
        engineering.errors.append(error)
        return

    obj = getattr(engineering, section, None)
    columns = ENG_COLUMNS.get(section, {})
    for (name, unit), value in zip(fields, data):
        attr = columns.get(group + '/' + name) or columns.get(name + '|' + unit) or columns.get(name)
        if obj is not None and attr is not None:
            setattr(obj, attr, DateTime(dt_str = value) if attr.startswith('dt_') else ParseValue(value))
        else:
            extra = engineering.extra.setdefault(section, {})
            key = name + ('_' + unit if name in extra and unit else '')
            extra[key] = ParseValue(value)

def PreviousValidFix(fpath_list, dive_num):
    """Return the most recent valid GPS fix of the float before dive dive_num, or None.

    Searches the listing files of earlier dives of the same float (lisNNNN-DDDD.txt, in the
    directory of fpath_list), from the most recent dive back to dive 0, and returns the latest
    valid fix of the first one that has a valid fix. Dives with negative numbers, which are
    tests before deployment, are not searched. Used for the magnetic declination of wave spectra
    from a dive without a valid fix.
    """
    dpath = os.path.dirname(fpath_list)
    m = re.match(r'(lis\d+-)', os.path.basename(fpath_list))
    if m is None:
        return None
    dives = []
    for fname in os.listdir(dpath):
        mm = re.match(re.escape(m.group(1)) + r'(-?\d+)\.txt$', fname)
        if mm and 0 <= int(mm.group(1)) < dive_num:     # negative dives: tests before deployment
            dives.append((int(mm.group(1)), fname))
    for _, fname in sorted(dives, reverse = True):
        fixes = []
        try:
            with open(os.path.join(dpath, fname), 'r', encoding='utf-8', errors = 'replace') as f:
                for line in f:
                    if not re.match('^GPS ', line):
                        continue
                    x = line.split()
                    try:
                        gps = Gps()
                        gps.idx = int(x[3])
                        gps.dt_from_iso8601_str(x[4])
                        gps.pos_from_iso6709_str(x[5])
                        gps.ttf_sec = int(x[6])
                        gps.nsat = int(x[7])
                        gps.snr_min = int(x[8])
                        gps.snr_avg = int(x[9])
                        gps.snr_max = int(x[10])
                        gps.hdop = float(x[11])
                        gps.EstimateUncertainty()
                    except (IndexError, ValueError, AttributeError):
                        continue
                    if gps.is_valid():
                        fixes.append(gps)
        except OSError:
            continue
        if fixes:
            return max(fixes, key = lambda g: g.dt_epoch)
    return None

def ProcessListingFile(fpath_list):
    """Parse one listing file into a Dive object and write its JSON file.

    The file is skipped unless it matches an -i pattern (when given) and no -x pattern. Comment
    lines are not stored, but the parser remembers the latest data-table column header
    ('SN dive sampleIdx|idx nSam|nSam ...'), engineering header ('SN dive midx|idx ...') and its
    units line, and '# --- Name ---' sub-group. Data lines by tag:

        LIS          serial, dive number, DAC and WMO IDs; without -f, stop here if the dive's
                     JSON file is newer than the listing (or exists at all, with -s)
        MIS, m       mission configuration and firmware versions, CTD type, parameters
        E0x, e0x     engineering sections and errors (ParseEngineeringLine)
        GPS, SBD     GPS fixes (with estimated uncertainty) and SBD message metadata
        D, d         drift samples (pres, temp, saln, sample times when the float sends them)
        PRO, pro     binned profile (pres, temp, conductivity-cell temp, saln, corrected saln)
        F/f, R/r     descent and ascent pressure time series with phase codes
        SFC, sfc     surface time series (pres, temp, saln)
        PRC          surface pressure offset correction (offset, valid flag, samples, dives)
        pmp         pump records (pres, time, voltage, current, vacuum)
        WVS, wvs     wave spectra; fill values of bins not received become NaN (wvm is ignored)

    Drift and profile samples get density and sigma0 from RhoLUT; temperatures and salinities at
    or below the lookup table minimum become NaN. A, PMP, PRD/prd, IRI, SFD, SHX/shx, VLV/vlv and
    XSV lines are ignored, and unknown lines are logged at debug level. A line that fails to
    parse is logged as an error (raised with -H) and skipped. After parsing, Dive.PostProcess is
    called and the result is written with WriteJsonDiveDataFile.
    """

    #global float_map

    # fpath_list = re.sub('\\\\', '/', fpath_list)
    dive_data = Dive(json_vers = JSON_VERSION) 

    try: 
    
        # Check file:
        if not os.path.exists(fpath_list) or not os.path.isfile(fpath_list):
            logger.error('Invalid path listing file: ' + fpath_list)
            return
        
        # Check inclusions
        if args.include is not None and len(args.include):
            include = False
            
            for re_include in args.include:
                if re.search(re_include, fpath_list):
                    include = True
                    break

            if not include:
                logger.debug('Ignoring file not included by REGEX: ' + fpath_list + ' ...')
                return
            
            del re_include
            del include
            
        # TODO Check exclusions
        if args.exclude is not None and len(args.exclude):
            exclude = False
            
            for re_exclude in args.exclude:
                if re.search(re_exclude, fpath_list):
                    exclude = True
                    break
                
            if exclude:
                logger.debug('Ignoring file excluded by REGEX: ' + fpath_list + ' ...')
                return
            
            del re_exclude
            del exclude
                    
        logger.debug('Processsing file: ' + fpath_list + ' ...')
        
        # Get file properies
        dive_data.list_file.mtime.dt_from_epoch(os.path.getmtime(fpath_list))
        
        if hasattr(args, 'strip') and len(args.strip):
            dive_data.list_file.path = re.sub(args.strip, '', fpath_list)
        
        # Open and read file:
        lines = [];
        try: 
            
            file = open(fpath_list, 'r', encoding='utf-8')
            lines = file.readlines()
            file.close()
            
            del file
            
        except:
            
            # Error handling: 
            
            raise Exception('Failed to read file:' + fpath_list)
           
        # Parse file line by line:

        columns = []    # column names of the latest data-table header
        drift_ref = None    # drift start time from the D line (floats that send drift times)
        eng_header = None   # latest '#      SN dive midx ...' (or idx) header line
        eng_units = None    # the units line under it, if any
        expect_units = False
        group = ''          # latest '# --- Name ---' sub-group (engineering and mission sections)

        for line in lines:
            
            try: 
            
                # Strip endline characters:
                    
                line = line.replace('\n', '')
                
                # Ignore empty lines:
                    
                if not len(line):
                    continue
                
                # Ignore comment lines:
                    
                if re.match('^#', line):
                    # logger.debug('Ignoring comment line: \'' + line + '\'')

                    # Remember the column names of data-table headers; the pro and d columns
                    # depend on the float (aux temperature, corrected salinity, drift times).
                    if re.match(r'^#\s+SN\s+dive\s+(sampleIdx|idx\s+nSam|nSam)\s', line):
                        columns = line[1:].split()

                    # Engineering headers ('SN dive midx ...', or 'SN dive idx ...' for error
                    # lines) and the units line under them.
                    if re.match(r'^#\s+SN\s+dive\s+(midx|idx)\s', line):
                        eng_header, eng_units, expect_units = line, None, True
                        continue
                    if expect_units:
                        expect_units = False
                        if not re.match(r'^#\s*-', line):
                            eng_units = line

                    # Sub-group headings, e.g. '# --- Air Pump & Bladder ------'.
                    m = re.match(r'^# --- (.*?)\s*-+$', line)
                    if m:
                        group = m.group(1)
                    elif re.match(r'^# (Engineering|Mission Configuration)\b', line):
                        group = ''

                    continue

                expect_units = False

                # Engineering data, all sections and minor indices:

                if re.match(r'^[Ee]0[0-9A-Fa-f] ', line):
                    try:
                        ParseEngineeringLine(dive_data.engineering, line, eng_header, eng_units, group)
                    except Exception as e:
                        logger.debug(str(e))

                    continue

                # Argo data message:
                
                if re.match('^A ', line):
                    continue

                # Drift Data:
                    
                if re.match('^D ', line):
                    x = line.split()
                    
                    dive_data.drift = Drift(num_samples = int(x[3]))

                    # Floats that send drift times: D SN dive nSam driftStart, and the d-line
                    # times are relative to driftStart.
                    drift_ref = DateTime(dt_str = x[4]) if len(x) > 4 else None

                    del x
                    
                    continue
                
                if re.match('^d ', line):
                    x = line.split()
                    
                    col = ColumnValues(columns, x)
                    if col is None:
                        col = { 'pres': x[4], 'temp': x[5], 'saln': x[6] }

                    # The sample index is nSam in the drift-times layout, otherwise the 4th field.
                    idx = int(col['nSam']) if 'nSam' in col else int(x[3])

                    dive_data.drift.pres[idx] = float(col['pres'])
                    dive_data.drift.temp[idx] = float(col['temp']) if float(col['temp']) > MIN_TEMP else np.nan
                    dive_data.drift.saln[idx] = float(col['saln']) if float(col['saln']) > MIN_SALN else np.nan

                    # Measured sample time (floats that send drift times): the time column is
                    # +seconds since the drift start on the D line.
                    if 'time' in col and drift_ref is not None and drift_ref.dt_epoch > 0:
                        dive_data.drift.dt_epoch[idx] = drift_ref.dt_epoch + int(col['time'])

                    del x
                    
                    # Lookup density:
                        
                    dive_data.drift.dens[idx] = RhoLUT.GetRhoFromLut(
                            dive_data.drift.saln[idx],
                            dive_data.drift.temp[idx],
                            dive_data.drift.pres[idx])
                            
                    dive_data.drift.sigma0[idx] = RhoLUT.GetSigma0FromLut(
                            dive_data.drift.saln[idx],
                            dive_data.drift.temp[idx],
                            dive_data.drift.pres[idx])

                    continue

                # Descent time-series data:
                
                if re.match('^F ', line):
                    x = line.split();
                    
                    # Get reference time:
                    dive_data.descent.dt_ref = DateTime(dt_str = x[5])
                    
                    del x
                    
                    continue
                
                if re.match('^f ', line):
                    x = line.split();
                    
                    dt_ref = dive_data.descent.dt_ref
                    
                    # Get datetime from time delta:
                    dt_ref_delta_sec = float(x[5])
                    dive_data.descent.dt_ref_delta_sec.append(dt_ref_delta_sec)
                    dt = DateTime().dt_from_epoch(dt_ref.dt_epoch + dt_ref_delta_sec)
                    dive_data.descent.dt.append(dt)
                    
                    # Get pressure
                    dive_data.descent.pres.append(float(x[6]))
                    dive_data.descent.n += 1
                    
                    # The phase code precedes the '#' comment (older listings have temp and
                    # saln columns before it).
                    dive_data.descent.phase_code.append(int(x[x.index('#') - 1]))
                    
                    del x
                    
                    continue
                
                # GPS positions: 
    
                if re.match('^GPS ', line):
                    x = line.split()
                    
                    gps                 = Gps()
                    gps.idx             = int(x[3])
                    gps.dt_from_iso8601_str(x[4]) 
                    # logger.debug('gps.dt = {}'.format(gps.dt))
                    gps.pos_from_iso6709_str(x[5])
                    gps.ttf_sec         = int(x[6])
                    gps.nsat            = int(x[7])
                    gps.snr_min         = int(x[8])
                    gps.snr_avg         = int(x[9])
                    gps.snr_max         = int(x[10])
                    gps.hdop            = float(x[11])
                    
                    del x
                    
                    # estimate uncertainty
                    gps.EstimateUncertainty()
                    
                    dive_data.gps.append(gps)
                    
                    del gps
                    
                    continue

                # Float IDs:

                if re.match('^LIS ', line):
                    x = line.split()
                    
                    dive_data.dive_num  = int(x[2])
                    dive_data.id        = FloatId(whoi = int(x[1]),
                                                  dac  = int(x[3]),
                                                  wmo  = int(x[5]))
                    
                    del x
                    
                    # Check if JSON file is newer if "--force" option not set
                    
                    if not args.force:
                    
                        # Check output directory
                        
                        if not hasattr(args, 'output'):
                            raise Exception('Output directory not specified.')
                        dpath_out = re.sub('\\\\', '/', args.output)
                        if not os.path.isdir(dpath_out):
                            raise Exception('Output directory not found: ' + dpath_out)
                        
                        # Generate JSON file path
                            
                        dpath_float = os.path.join(dpath_out, '{}'.format(dive_data.id.whoi))
                        dpath_float = re.sub('\\\\', '/', dpath_float)
                        dpath_dives = os.path.join(dpath_float, 'dives')
                        dpath_dives = re.sub('\\\\', '/', dpath_dives)
                        fpath_json = os.path.join(dpath_dives, '{:04d}_{:04d}.json'.format(dive_data.id.whoi, dive_data.dive_num))
                        fpath_json = re.sub('\\\\', '/', fpath_json)
                        
                        if not os.path.exists(fpath_json):
                            
                            # JSON file does not exist
                            
                            del dpath_out
                            del dpath_float
                            del dpath_dives
                            del fpath_json
                            
                            continue
                        
                        elif hasattr(args, 'skip') and args.skip:
                            
                            # If "--skip" option is set, ignore listing file, even if newer.
                            
                            logger.debug('Skipping listing file with existing JSON: ' + fpath_list)
                            
                            return
                        
                        # TODO check if JSON file is newer
                        
                        mtime_json = os.path.getmtime(fpath_json)
                        
                        if mtime_json > dive_data.list_file.mtime.dt_epoch:
                            
                            logger.debug('Skipping listing file with newer JSON: ' + fpath_list)
                            
                            return
                    
                        del dpath_out
                        del dpath_float
                        del dpath_dives
                        del fpath_json
                        del mtime_json
                    
                    continue
                
                # TODO Mission Parameters:
                if re.match('^MIS ', line):
                
                    # MIS SN dive [midx] cfgVer fwVer sensorType # sensorTypeName; older listings
                    # have the minor index, so locate the fields by the hex versions.
                    x = line.split('#', 1)[0].split()
                    hexes = [i for i, t in enumerate(x) if i > 2 and t.lower().startswith('0x')]

                    if len(hexes) >= 2:
                        dive_data.mission.cfgver = x[hexes[0]]
                        dive_data.mission.fwver = x[hexes[1]]
                        sensor = x[hexes[1] + 1] if len(x) > hexes[1] + 1 else ''
                    else:
                        dive_data.mission.fwver = x[4]
                        sensor = x[5]

                    if sensor == '0':
                        dive_data.mission.ctdtype = 'RBR'
                    elif sensor == '1':
                        dive_data.mission.ctdtype = 'SBE'
                    else:
                        dive_data.mission.ctdtype = ''

                    continue

                if re.match('^m ', line):
                    # m SN dive idx name value [units] # description (not kept)
                    x = line.split('#', 1)[0].split()
                    value = ParseValue(x[5])

                    dive_data.mission.parameters[x[4]] = float(value) if isinstance(value, (int, float)) else value
                    dive_data.mission.entries.append({
                        'section': group,
                        'idx': int(x[3]),
                        'name': x[4],
                        'value': value,
                        'units': x[6].strip('[]') if len(x) > 6 else '',
                        })

                    continue
                
                # Profile data:
                    
                if re.match('^PRO ', line):
                    x = line.split()
                    
                    dive_data.profile = Profile(num_bins = int(x[3]))
                    
                    del x
                    
                    continue
                
                # Profile column descriptions (documentation only):

                if re.match('^(PRD|prd) ', line):
                    continue

                if re.match('^pro ', line):
                    
                    x = line.split();
                    idx = int(x[3])
                    col = ColumnValues(columns, x)
                    if col is None:
                        # No matching header: fall back to the field count.
                        if len(x) == 7:
                            col = { 'pres': x[4], 'temp': x[5], 'saln': x[6] }
                        elif len(x) == 8:
                            col = { 'pres': x[4], 'temp1': x[5], 'temp2': x[6], 'saln': x[7] }

                    if col is not None and 'pres' in col and 'saln' in col:
                        temp = col.get('temp', col.get('temp1'))
                        dive_data.profile.pres[idx] = float(col['pres'])
                        dive_data.profile.temp[idx] = float(temp) if float(temp) > MIN_TEMP else np.nan
                        dive_data.profile.tcnd[idx] = float(col['temp2']) if 'temp2' in col and float(col['temp2']) > MIN_TEMP else np.nan
                        dive_data.profile.saln[idx] = float(col['saln']) if float(col['saln']) > MIN_SALN else np.nan
                        dive_data.profile.saln_corr[idx] = float(col['salnCorr']) if 'salnCorr' in col and float(col['salnCorr']) > MIN_SALN else np.nan
                    else:
                        logger.error('Unsupported profile line format: \'' + line + '\'')
                        
                    del x
                        
                    # Lookup density:
                    dive_data.profile.dens[idx] = RhoLUT.GetRhoFromLut(
                            dive_data.profile.saln[idx],
                            dive_data.profile.temp[idx],
                            dive_data.profile.pres[idx])
                    dive_data.profile.sigma0[idx] = RhoLUT.GetSigma0FromLut(
                            dive_data.profile.saln[idx],
                            dive_data.profile.temp[idx],
                            dive_data.profile.pres[idx])
                    
                    del idx
                        
                    continue
                
                # Pump
                
                if re.match('^PMP ', line):
                    continue
                
                if re.match('^pmp ', line):
                    
                    x = line.split();
                    
                    v = float(x[7])
                    i = float(x[8])
                    t = float(x[6])
                    # p = v * i
                    # e = p * t
                    
                    dive_data.pump.pres.append(float(x[5]))
                    dive_data.pump.time.append(t)
                    dive_data.pump.volt.append(v)
                    dive_data.pump.curr.append(i)
                    dive_data.pump.vac.append(float(x[10]))
                    
                    del x
                    
                    continue
                    
                # Ascent time-series:
                
                if re.match('^R ', line):
                    x = line.split();
                    
                    # Get reference time:
                    dive_data.ascent.dt_ref = DateTime(dt_str = x[5])
                    
                    del x
                    
                    continue
                
                if re.match('^r ', line):
                    x = line.split();
                    
                    dt_ref = dive_data.ascent.dt_ref
                    
                    # Get datetime from time delta:
                    dt_ref_delta_sec = float(x[5])
                    dive_data.ascent.dt_ref_delta_sec.append(dt_ref_delta_sec)
                    dt = DateTime().dt_from_epoch(dt_ref.dt_epoch + dt_ref_delta_sec)
                    dive_data.ascent.dt.append(dt)
                    
                    # Get pressure
                    dive_data.ascent.pres.append(float(x[6]))
                    dive_data.ascent.n += 1
                    
                    dive_data.ascent.phase_code.append(int(x[x.index('#') - 1]))
                    
                    del x
                    del dt_ref_delta_sec
                    
                    continue        
                
                # SBD messages:
                
                if re.match('^SBD ', line):
                    x = line.split()
                    
                    sbd                 = Sbd();
                    # sbd.idx = len(dive_data.sbd)
                    sbd.size_b          = int(x[3])
                    sbd.momsn           = int(x[5])
                    sbd.mtmsn           = int(x[6])
                    sbd.pid             = int(x[7])
                    sbd.dt_from_iso8601_str(x[8])
                    sbd.pos_from_iso6709_str(x[9])
                    sbd.cep_km          = int(x[10])
                    dive_data.sbd.append(sbd)
                    
                    del x
                    del sbd

                    continue
             
                if re.match('^IRI ', line):
                    continue
             
                # Surface time-series:
                
                if re.match('^SFC ', line):
                    x = line.split();
                    
                    # Get reference time:
                    dive_data.surface.dt_ref = DateTime(dt_str = x[5])
                    
                    del x
                    
                    continue
                
                if re.match('^sfc ', line):
                    x = line.split();
                    
                    dt_ref = dive_data.surface.dt_ref
                    
                    # Get datetime from time delta:
                    dt_ref_delta_sec = int(x[5])
                    dive_data.surface.dt_ref_delta_sec.append(dt_ref_delta_sec)
                    dt = DateTime().dt_from_epoch(dt_ref.dt_epoch + dt_ref_delta_sec)
                    dive_data.surface.dt.append(dt)
                    
                    # Get pressure, etc
                    dive_data.surface.pres.append(float(x[6]))
                    dive_data.surface.temp.append(float(x[7]) if float(x[7]) > -9.99 else np.nan)
                    dive_data.surface.saln.append(float(x[8]) if float(x[8]) > MIN_SALN else np.nan)
                    dive_data.surface.n += 1
                    
                    del x
                    del dt
                    del dt_ref
                    del dt_ref_delta_sec
                    
                    continue        
             
                if re.match('^SFD ', line):
                    continue
                    
                if re.match('^PRC ', line):
                    x = line.split()

                    # SN dive midx nSam nDive valid presOffset [sfcPmin sfcPmean sfcPmax sfcPstd]
                    dive_data.surface.pres_correction_num_samples = int(x[4])
                    dive_data.surface.pres_correction_num_dives   = int(x[5])
                    dive_data.surface.pres_correction_valid       = int(x[6]) == 1
                    dive_data.surface.pres_correction             = float(x[7])

                    del x

                    continue
             
                # SBD message data:
             
                if re.match('^SHX ', line):
                    # TODO
                    continue
                
                if re.match('^shx ', line):
                    # TODO
                    continue
                    
                # Valve data:

                if re.match('^VLV ', line):
                    continue
                
                if re.match('^vlv ', line):
                    continue
                    
                # Wave data:
                    
                if re.match('^WVS ', line):
                    x = line.split()
                    
                    dive_data.wave = Wave(slen = int(x[4]), fsam =float(x[5]))
                    
                    del x
                
                    continue
                
                if re.match('^wvs ', line):
                    x = line.split()
                    
                    idx = int(x[3])
                    
                    # dive_data.wave.freq[idx]   = float(x[4])
                    # Bins that were not received hold the fill value -999.99999; they become NaN.
                    dive_data.wave.Szz_g[idx]  = ParseWaveSpectrum(x[5])
                    dive_data.wave.Syy_g[idx]  = ParseWaveSpectrum(x[6])
                    dive_data.wave.Sxx_g[idx]  = ParseWaveSpectrum(x[7])
                    dive_data.wave.Cxy_g[idx]  = ParseWaveSpectrum(x[8])
                    dive_data.wave.Qzx_g[idx]  = ParseWaveSpectrum(x[9])
                    dive_data.wave.Qzy_g[idx]  = ParseWaveSpectrum(x[10])

                    del x
                
                    continue                
                
                if re.match('^wvm ', line):
                    # x = line.split()
                    
                    # idx = int(x[3])
                    
                    # dive_data.wave.freq[idx] = float(x[4])
                    # dive_data.wave.Szz[idx]  = float(x[5]) if float(x[5]) < 500 else np.nan
                    # dive_data.wave.Syy[idx]  = float(x[6]) if float(x[6]) < 500 else np.nan
                    # dive_data.wave.Sxx[idx]  = float(x[7]) if float(x[7]) < 500 else np.nan
                    # dive_data.wave.Cxy[idx]  = float(x[8]) if (float(x[8]) < 500) and (float(x[8]) > -500)  else np.nan
                    # dive_data.wave.Qzx[idx]  = float(x[9]) if (float(x[9]) < 500) and (float(x[9]) > -500) else np.nan
                    # dive_data.wave.Qzy[idx]  = float(x[10]) if (float(x[10]) < 500) and (float(x[10]) > -500) else np.nan

                    # del x
                
                    continue
                
                # decoder version:
                
                if re.match('^XSV ', line):
                    continue
                
                # Handle unknown lines:
                
                logger.debug('Ignoring unknown line: \'' + line + '\'')
                
            except: 
                
                # Error handling for line parsing
            
                errstr = 'Failed to process line: \'' + line + '\''
                if hasattr(args, 'halt') and args.halt:
                    raise Exception(errstr)
                else:
                    logger.error(errstr)
        
        # Post process

        try:
            fallback_fix = None
            if dive_data.wave is not None and dive_data.SurfaceFix() is None:
                fallback_fix = PreviousValidFix(fpath_list, dive_data.dive_num)
                if fallback_fix is not None:
                    logger.debug('No valid fix; using the fix of {} for the magnetic declination'.format(
                        fallback_fix.dt_str))
            dive_data.PostProcess(fallback_fix = fallback_fix)
        
        except: 
            
            # Post process error handling
        
            errstr = 'Failed to post-process dive data object'
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            else:
                logger.error(errstr)
            
        # logger.debug('dive_data: ' + json.dumps(dive_data.toJson()))
        
        WriteJsonDiveDataFile(dive_data)
        
    except:
    
        # Process file error handling
        
        errstr = 'Failed to proces file: ' + fpath_list
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        else:
            logger.error(errstr)
    
    # Cleanup
    
    if 'dive_data' in locals():     del dive_data
    if 'fpath_list' in locals():    del fpath_list
    if 'line' in locals():          del line
    if 'lines' in locals():         del lines
    gc.collect()
        
    return

        
def WriteJsonDiveDataFile(dive_data):
    """Write a Dive object to <output>/<serial>/dives/<serial>_<dive>.json with jsonpickle.

    The dives directory is created if it does not exist. The serial and dive number are formatted
    with at least 4 digits. Errors are logged, or raised with -H.
    """
    
    # Check dive data object
    
    try:
        
        if not hasattr(dive_data, 'id'):
            raise Exception('Invalid dive data object, no \'id\'.')
            
        if not hasattr(dive_data.id, 'whoi'):
            raise Exception('Invalid dive data object, no \'id.whoi\'.')
            
        if not hasattr(dive_data, 'dive_num'):
            raise Exception('Invalid dive data object, no \'dive_num\'.')
            
    except:
                        
        # Error handling
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception('Failed to write JSON file.')
            
        else:
            logger.error('Failed to write JSON file.')
            return
    
    try: 
        
        whoi_id = dive_data.id.whoi;
        dive    = dive_data.dive_num;
 
        # Check output directory:
        if not hasattr(args, 'output'):
            raise Exception('Output directory not specified.')
        dpath_out = re.sub('\\\\', '/', args.output)
        if not os.path.isdir(dpath_out):
            raise Exception('Output directory not found: ' + dpath_out)
        
        # Generate path:
        dpath_float = os.path.join(dpath_out, '{}'.format(dive_data.id.whoi))
        dpath_float = re.sub('\\\\', '/', dpath_float)
        dpath_dives = os.path.join(dpath_float, 'dives')
        dpath_dives = re.sub('\\\\', '/', dpath_dives)
        
        # logger.debug('Output directory: \'' + path + '\'')
        
        # TODO remove
        os.path.isdir(dpath_dives)
        os.path.isdir(dpath_dives)
        os.path.isdir(dpath_dives)
        
        if not os.path.isdir(dpath_dives):
            if os.path.exists(dpath_dives): 
                raise Exception('Invalid path: \'' + dpath_dives + '\'')
            else:
                logger.warning('Creating directory: \'' + dpath_dives + '\'')
                os.makedirs(dpath_dives)
        
        fname_json = '{:04d}_{:04d}.json'.format(whoi_id, dive)
        fpath_json = os.path.join(dpath_dives, fname_json)
        fpath_json = re.sub('\\\\', '/', fpath_json)
        
        # Write dive files:
        logger.info('Writing JSON file: ' + fpath_json + ' ...')
        try:
            file = open(fpath_json, 'w', encoding='utf-8')
            # json.dump(dive_data.toJson(), file)
            # file.write(dive_data.toJson(), file)
            # dive_json = jsonpickle.encode(dive_data, unpicklable=True, indent=2, separators=(',', ':'))
            dive_json = jsonpickle.encode(dive_data, unpicklable=True, keys=True, separators=(',', ':'))
            file.write(dive_json)
            file.close()
        except:
            raise Exception('Failed to write JSON file: \'' + path + '\'')
        
    except:
                
        # Error handling
        
        if hasattr(args, 'halt') and args.halt:
            
            raise Exception('Failed to write JSON file for float {}, dive {}.'.format(whoi_id, dive))
            
        else:
            
            logger.error('Failed to write JSON file for float {}, dive {}.'.format(whoi_id, dive))
        
    

"""
Parse arguments:
"""

args = parser.parse_args()


# setup logger

logging.basicConfig(format='%(asctime)s %(levelname)7s %(message)s', datefmt="%Y%m%dT%H%M%S")
logger = logging.getLogger('aggregate-json')
logger.propagate = False

logger.handlers.clear()

# logger.setLevel(logging.DEBUG)

if not logger.hasHandlers():
    ch = logging.StreamHandler()
    formatter = logging.Formatter('%(asctime)s %(levelname)7s %(message)s', datefmt="%Y%m%dT%H%M%S")
    ch.setFormatter(formatter)
    logger.addHandler(ch)
    
# Check boolean args:
debug    = hasattr(args, 'debug')   and args.debug
quiet    = hasattr(args, 'quiet')   and args.quiet
verbose  = hasattr(args, 'verbose') and args.verbose

# Check logging args:

if hasattr(args, 'debug')   and args.debug:
    # ch.setLevel(logging.DEBUG)
    logger.setLevel(logging.DEBUG)
elif hasattr(args, 'verbose') and args.verbose:
    # ch.setLevel(logging.INFO)
    logger.setLevel(logging.INFO)
elif hasattr(args, 'quiet')   and args.quiet:
    # ch.setLevel(logging.ERROR)
    logger.setLevel(logging.ERROR)
else:
    # ch.setLevel(logging.WARNING)
    logger.setLevel(logging.WARNING)
    
del debug
del quiet
del verbose

logger.debug('Debug output enabled.')

"""
Start of execution:
"""

try: 
    
    # Check inputs:
    if not hasattr(args, 'output') or not len(args.output):
        args.output = '.'
    elif not os.path.isdir(args.output):
        logger.error('Output directory not found: ' + args.output)

    # Iterate over directories:
    for path in args.path_in:
        if os.path.isdir(path):
            ProcessDirectory(path)
        elif os.path.isfile(path):
           ProcessListingFile(path)
        else:
            logger.error("Invalid path: " + path)
        
        
except:
    
    logger.error('Processing failed.')
    raise
