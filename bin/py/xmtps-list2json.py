#!/usr/bin/python3
#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
Created on Wed Sep  1 14:37:00 2021

@author: aekholm
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

def ProcessListingFile(fpath_list):
    
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
            
            file = open(fpath_list, 'r')
            lines = file.readlines()
            file.close()
            
            del file
            
        except:
            
            # Error handling: 
            
            raise Exception('Failed to read file:' + fpath_list)
           
        # Parse file line by line: 
           
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
                    continue

                # Argo data message:
                
                if re.match('^A ', line):
                    continue

                # Drift Data:
                    
                if re.match('^D ', line):
                    x = line.split()
                    
                    dive_data.drift = Drift(num_samples = int(x[3]))
                    
                    del x
                    
                    continue
                
                if re.match('^d ', line):
                    x = line.split()
                    
                    idx = int(x[3])
                    dive_data.drift.pres[idx] = float(x[4])
                    dive_data.drift.temp[idx] = float(x[5]) if float(x[5]) > MIN_TEMP else np.nan
                    dive_data.drift.saln[idx] = float(x[6]) if float(x[6]) > MIN_SALN else np.nan
                    
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
                
                # Engineering data - descent
                
                if re.match('^E07 ', line):
                    
                    x = line.split();
                    midx = int(x[3])
                    
                    if midx == 0:
                        dive_data.engineering.descent.dt_start = DateTime(dt_str = x[5])
                        dive_data.engineering.descent.vac0 = float(x[6])
                        dive_data.engineering.descent.vac1 = float(x[7])
                    
                    del midx
                    del x
                    
                    continue
                
                # Engineering data - drift
                
                if re.match('^E08 ', line):
                    
                    x = line.split();
                    midx = int(x[3])
                    
                    if midx == 0:
                        dive_data.engineering.drift.dt_start = DateTime(dt_str = x[5])
                        dive_data.engineering.drift.vac0 = float(x[6])
                        dive_data.engineering.drift.vac1 = float(x[7])
                    
                    del midx
                    del x
                    
                    continue                

                # Engineering data - ascent
                
                if re.match('^E09 ', line):
                    
                    x = line.split();
                    midx = int(x[3])
                    
                    if midx == 0:
                        dive_data.engineering.ascent.dt_start = DateTime(dt_str = x[5])
                        dive_data.engineering.ascent.vac0 = float(x[6])
                        dive_data.engineering.ascent.vac1 = float(x[7])
                    
                    del midx
                    del x
                    
                    continue                
                
                # Engineering data - surface
                
                if re.match('^E0A ', line):
                    
                    x = line.split();
                    midx = int(x[3])
                    
                    if midx == 0:
                        dive_data.engineering.surface.dt_start = DateTime(dt_str = x[5])
                        dive_data.engineering.surface.vac0 = float(x[6])
                        dive_data.engineering.surface.vac1 = float(x[7])
                    
                    del midx
                    del x
                    
                    continue                
                
                if re.match('^e0A ', line):
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
                    
                    dive_data.descent.phase_code.append(int(x[9]))
                    
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
                
                    x = line.split()
                    
                    dive_data.mission.fwver = x[4]
                    
                    if int(x[5]) == 0:
                        dive_data.mission.ctdtype = 'RBR'
                    elif int(x[5]) == 1:
                        dive_data.mission.ctdtype = 'SBE'
                    else:
                        dive_data.mission.ctdtype = ''                    
                    
                    continue
                
                if re.match('^m ', line):
                    x = line.split()
                    
                    dive_data.mission.parameters[x[4]] = float(x[5])
                    
                    continue
                
                # Profile data:
                    
                if re.match('^PRO ', line):
                    x = line.split()
                    
                    dive_data.profile = Profile(num_bins = int(x[3]))
                    
                    del x
                    
                    continue
                
                if re.match('^pro ', line):
                    
                    x = line.split();
                    idx = int(x[3])
                    if len(x) == 7:
                        dive_data.profile.pres[idx] = float(x[4]) 
                        dive_data.profile.temp[idx] = float(x[5]) if float(x[5]) > MIN_TEMP else np.nan
                        dive_data.profile.tcnd[idx] = np.nan  # TODO handle no tcnd better
                        dive_data.profile.saln[idx] = float(x[6]) if float(x[6]) > MIN_SALN else np.nan
                    elif len(x) == 8:
                        dive_data.profile.pres[idx] = float(x[4])
                        dive_data.profile.temp[idx] = float(x[5]) if float(x[5]) > MIN_TEMP else np.nan
                        dive_data.profile.tcnd[idx] = float(x[6]) if float(x[6]) > MIN_TEMP else np.nan
                        dive_data.profile.saln[idx] = float(x[7]) if float(x[7]) > MIN_SALN else np.nan
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
                    
                    dive_data.ascent.phase_code.append(int(x[9]))
                    
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
                    dive_data.wave.Szz_g[idx]  = float(x[5])# if float(x[5]) < 500 else np.nan
                    dive_data.wave.Syy_g[idx]  = float(x[6])# if float(x[6]) < 500 else np.nan
                    dive_data.wave.Sxx_g[idx]  = float(x[7])# if float(x[7]) < 500 else np.nan
                    dive_data.wave.Cxy_g[idx]  = float(x[8])# if (float(x[8]) < 500) and (float(x[8]) > -500)  else np.nan
                    dive_data.wave.Qzx_g[idx]  = float(x[9])# if (float(x[9]) < 500) and (float(x[9]) > -500) else np.nan
                    dive_data.wave.Qzy_g[idx]  = float(x[10])# if (float(x[10]) < 500) and (float(x[10]) > -500) else np.nan

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
            dive_data.PostProcess()
        
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
            file = open(fpath_json, 'w')
            # json.dump(dive_data.toJson(), file)
            # file.write(dive_data.toJson(), file)
            # dive_json = jsonpickle.encode(dive_data, unpicklable=True, indent=2, separators=(',', ':'))
            dive_json = jsonpickle.encode(dive_data, unpicklable=True, separators=(',', ':'))
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
