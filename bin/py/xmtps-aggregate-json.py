#!/usr/bin/python3
#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  7 13:50:06 2022

@author: aekholm
"""

import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
warnings.filterwarnings(action='ignore', message='Cannot close a netcdf_file opened with mmap=True')
warnings.filterwarnings(action='ignore', message='The kopts argument is ignored if using a server')


import argparse
#from colour import Color
import colorsys
import gc
import datetime
import dateutil
# import json
import jsonpickle
import logging
import math
import numpy as np
import os
import pandas as pd
from PIL import Image
# import plotly.express as px
import plotly.graph_objects as go
import plotly.io as pio
# import pygmt
# import pprint
import re
# import time
from scipy import interpolate
from yattag import Doc 

# local
from FloatClasses import DateTime, Dive, File, Float, Gps, CompositePosition, PositionVector, Sbd

# Set library options

pd.options.mode.chained_assignment = None  # default='warn'

pio.renderers.default = "browser"

"""
Static configuration:
"""

CPOS_SPAN_SECONDS = 3600
CPOS_MAX_CEP_KM = 1.0

JSON_VERSION = "0.11"

BATHYMETRY_FILE = '/data1/etopo2/ETOPO2v2c_f4.nc'   # NOAA ETOPO2v2c, 2' cell-centred grid

bathymetry = None

def BathymetryPressure(lat, lon):
    """Return the seafloor pressure (dBar) from ETOPO2 at each position, NaN on land.

    Uses the nearest grid cell and the Saunders (1981) depth-to-pressure conversion. Returns
    None if the bathymetry file can't be read.
    """
    global bathymetry
    if bathymetry is None:
        try:
            from scipy.io import netcdf_file
            nc = netcdf_file(args.bathymetry, mmap = True)
            bathymetry = dict(nc = nc, x = np.array(nc.variables['x'][:], dtype = np.double),
                              y = np.array(nc.variables['y'][:], dtype = np.double), z = nc.variables['z'])
        except Exception as e:
            logger.warning('Failed to read bathymetry file {}: {}'.format(args.bathymetry, e))
            bathymetry = False
    if not bathymetry:
        return None
    x, y = bathymetry['x'], bathymetry['y']
    lat, lon = np.asarray(lat, dtype = np.double), np.asarray(lon, dtype = np.double)
    ix = np.mod(np.round((lon - x[0]) / (x[1] - x[0])).astype(int), len(x))
    iy = np.clip(np.round((lat - y[0]) / (y[1] - y[0])).astype(int), 0, len(y) - 1)
    depth = -np.array(bathymetry['z'][iy, ix], dtype = np.double)
    depth[depth <= 0] = np.nan
    c1 = (5.92 + 5.25 * np.sin(np.radians(lat)) ** 2) * 1e-3
    return ((1 - c1) - np.sqrt((1 - c1) ** 2 - 8.84e-6 * depth)) / 4.42e-6

opts = dict(
    fig = dict(
        height = 900, 
        width  = 900,
        html_height = 900, 
        html_width  = 900, 
        small_height = 415,
        small_width  = 390,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    fig2 = dict(
        height = 900, 
        width  = 900,
        html_height = None, 
        html_width  = None, 
        small_height = 415,
        small_width  = 390,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    figWide = dict(
        height = 900, 
        width  = 1600, 
        html_height = None, 
        html_width  = None, 
        small_height = 415,
        small_width  = 780,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),    
    figWideS = dict(
        height = 900, 
        width  = 1600, 
        html_height = None, 
        html_width  = None, 
        small_height = 315,
        small_width  = 780,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    figWideXS = dict(
        height = 900, 
        width  = 1600, 
        html_height = None, 
        html_width  = None, 
        small_height = 215,
        small_width  = 780,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    figWideL = dict(
        height = 900, 
        width  = 1600, 
        html_height = None, 
        html_width  = None, 
        small_height = 515,
        small_width  = 780,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    figWideXL = dict(
        height = 900,
        width  = 1600, 
        html_height = None, 
        html_width  = None, 
        small_height = 615,
        small_width  = 780,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    figWideXXL = dict(
        height = 900,
        width  = 1600, 
        html_height = None, 
        html_width  = None, 
        small_height = 715,
        small_width  = 780,
        margin=dict(l=15,r=15,b=15,t=40,pad=0)
        ),
    figThumb = dict(
        height = 9999,
        width  = 150, 
        ),
    )

"""
Global Variables:
"""

float_map = {}

float_aggregate_map = {}

lastprocessed_pending = []

"""
Function Definitions:
"""

def AggregateDiveData():
    
    global float_map
    global float_aggregate_map
    
    try:
        
        # Iterate over floats:
        
        for float_id in float_map.keys():
            
            logger.info('Aggregating data for float {:04d}...'.format(float_id))
            
            try: 
            
                float_data = float_map[float_id]
                
                float_aggregate_data = {
                    'id':       float_data.id,
                    'ascent':   
                        pd.DataFrame(columns = [ 'dive', 
                                                 'pres', 
                                                 'dt_epoch', 
                                                 'dt_ref_delta_sec', 
                                                 'dt_str', 
                                                 'phase_code', 
                                                 'rate',
                                                 ]) ,
                    'descent':  
                        pd.DataFrame(columns = [ 'dive', 
                                                 'pres', 
                                                 'dt_epoch', 
                                                 'dt_ref_delta_sec', 
                                                 'dt_str', 
                                                 'phase_code', 
                                                 'rate',
                                                 ]) ,
                    'drift':    
                        pd.DataFrame(columns = [ 'dive', 
                                                 'pres', 
                                                 'temp', 
                                                 'saln', 
                                                 'dens', 
                                                 'sigma0',
                                                 'dt_epoch', 
                                                 'dt_ref_delta_sec', 
                                                 'dt_str',                                                 
                                                 ]),
                    'engineering': 
                        pd.DataFrame(columns = [ 'dive', 
                                                 'ascent_dt_epoch', 
                                                 'ascent_dt_str', 
                                                 'ascent_vac0', 
                                                 'ascent_vac1' 
                                                 'descent_dt_epoch', 
                                                 'descent_dt_str', 
                                                 'descent_vac0', 
                                                 'descent_vac1' 
                                                 'drift_dt_epoch', 
                                                 'drift_dt_str', 
                                                 'drift_vac0', 
                                                 'drift_vac1' 
                                                 'surface_dt_epoch', 
                                                 'surface_dt_str', 
                                                 'surface_vac0', 
                                                 'surface_vac1',
                                                 ]),
                    'gps':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'dt_epoch', 
                                                 'dt_str', 
                                                 'lat', 
                                                 'lon', 
                                                 'cep_km', 
                                                 'idx', 
                                                 'ttf_sec', 
                                                 'nsat', 
                                                 'snr_min', 
                                                 'snr_avg', 
                                                 'snr_max', 
                                                 'hdop',  ]) ,
                    'iridium_position': 
                        pd.DataFrame(columns = [ 'dive', 
                                                 'dt_epoch', 
                                                 'dt_str', 
                                                 'dt_min_epoch', 
                                                 'dt_min_str', 
                                                 'dt_max_epoch', 
                                                 'dt_max_str', 
                                                 'lat', 
                                                 'lon', 
                                                 'cep_km', 
                                                 'num_pos', ]) ,                   
                    'profile':  
                        pd.DataFrame(columns = [ 'dive', 'pres', 'temp', 'tcnd', 'saln', 'saln_corr', 'dens', 'sigma0', ]),
                    'profile_position': 
                        pd.DataFrame(columns = [ 'dive', 'dt_epoch', 'dt_str', 'lat', 'lon', 'cep_km', 'src' ]),
                    'pump':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'pres', 
                                                 'time', 
                                                 'volt', 
                                                 'curr', 
                                                 'curr_adj', 
                                                 'vac', 
                                                 'power',
                                                 'energy',
                                                 ]) ,
                    'sbd':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'dt_epoch', 
                                                 'dt_str', 
                                                 'lat', 
                                                 'lon', 
                                                 'cep_km', 
                                                 'size_b',
                                                 'momsn',
                                                 'mtmsn',
                                                 'pid', 
                                                 ]) ,
                    'surface':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'dt_epoch', 
                                                 'dt_str', 
                                                 'pres',
                                                 'temp', 
                                                 ]),
                    'wave':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'mwd',
                                                 'period_peak', 
                                                 'period_avg',
                                                 'period_zero',
                                                 'swh',                                                  
                                                 ]),
                    'wave_spectra':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'freq', 
                                                 'Szz_g', 
                                                 'Syy_g', 
                                                 'Sxx_g', 
                                                 'Cxy_g', 
                                                 'Qzx_g',
                                                 'Qzy_g', 
                                                 'Szz', 
                                                 'Syy', 
                                                 'Sxx', 
                                                 'Cxy', 
                                                 'Qzx',
                                                 'Qzy', 
                                                 'a0',
                                                 'a1',
                                                 'a2',
                                                 'b1',
                                                 'b2',
                                                 'r1',
                                                 'r2',
                                                 'theta1',
                                                 'theta2',
                                                 ]),
                    'cycle_timing':
                        pd.DataFrame(columns = [ 'dive', 
                                                 'ascent_dt_epoch', 
                                                 'ascent_dt_str', 
                                                 'ascent_duration_s',
                                                 'descent_dt_epoch', 
                                                 'descent_dt_str', 
                                                 'descent_duration_s',
                                                 'drift_dt_epoch', 
                                                 'drift_dt_str',
                                                 'drift_duration_s',
                                                 'surface_dt_epoch', 
                                                 'surface_dt_str',
                                                 'surface_duration_s',
                                                 'total_duration_s',
                                                 ]),                    
                    'displacement_submerged':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'azi', 
                                                 'dist_m', 
                                                 'duration_s',
                                                 'speed_mps', 
                                                 'pos1_dt_epoch',
                                                 'pos1_dt_str',
                                                 'pos1_lat',
                                                 'pos1_lon',
                                                 'pos2_dt_epoch',
                                                 'pos2_dt_str',
                                                 'pos2_lat',
                                                 'pos2_lon',
                                                 ]) ,
                    'displacement_surface':      
                        pd.DataFrame(columns = [ 'dive', 
                                                 'azi', 
                                                 'dist_m', 
                                                 'duration_s',
                                                 'speed_mps', 
                                                 'pos1_dt_epoch',
                                                 'pos1_dt_str',
                                                 'pos1_lat',
                                                 'pos1_lon',
                                                 'pos2_dt_epoch',
                                                 'pos2_dt_str',
                                                 'pos2_lat',
                                                 'pos2_lon',
                                                 ]) ,
                    'figures': None,
                    'summary' : { 
                        'json_vers': JSON_VERSION,
                        'id': float_data.id,
                        'age': -999,
                        'fwver': '',
                        'ctdtype': '',
                        'profdepthsum': 0, 
                        'type': 'ALTO' if float_data.id.whoi > 10000 else 'ALAMO',
                        'startup':  { 
                            'gps': Gps(), 
                            'sbd': Sbd(),
                            },
                        'deployment': {
                            'gps': Gps(),
                            'sbd': Sbd(),
                            },       
                        'latest': {
                            'gps': Gps(),
                            'sbd': Sbd(),
                            'cycle': -999,
                            'duration': -999,
                            'nsbd': -999,
                            'depth': -999,
                            },                            
                        },
                        
                        
                }
                
                dives = sorted([dive_data.dive_num for dive_data in float_data.dives])
    
                # Aggregate ascent data
                
                logger.debug('Aggregating ascent data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives:
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                        
                        if(dive_num < 0):
                            continue
                        
                        logger.debug('Aggregating ascent data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        ascent_data = dive_data.ascent
                        df_profile = pd.DataFrame(
                            {
                                'dive':             np.array([dive_data.dive_num] * ascent_data.n, dtype='int'),
                                'pres':             np.array(ascent_data.pres, dtype='float'),
                                'dt_epoch':         np.array(ascent_data.dt_epoch, dtype='float'),
                                'dt_ref_delta_sec': np.array(ascent_data.dt_ref_delta_sec, dtype='float'),
                                'dt_str':           np.array(ascent_data.dt_str, dtype='str'),
                                'phase_code':       np.array(ascent_data.phase_code, dtype='int'),
                                'rate':             np.array(ascent_data.rate, dtype='float')*100,
                            }
                        )
                        float_aggregate_data['ascent'] = pd.concat([float_aggregate_data['ascent'], df_profile], ignore_index=True)
                        
                        # Append NaNs to separate dives
                        
                        df_profile = pd.DataFrame(
                            {
                                'dive':             np.array([dive_data.dive_num], dtype='int'),
                                'pres':             np.array([np.nan], dtype='float'),
                                'dt_epoch':         np.array([np.nan], dtype='float'),
                                'dt_ref_delta_sec': np.array([np.nan], dtype='float'),
                                'dt_str':           np.array([np.nan], dtype='str'),
                                'phase_code':       np.array([-1], dtype='int'),
                                'rate':             np.array([np.nan], dtype='float'),
                            }
                        )
                        float_aggregate_data['ascent'] = pd.concat([float_aggregate_data['ascent'], df_profile], ignore_index=True)
                        
                        # Get Summary data
                        
                        if not np.isnan(ascent_data.pres).all():
                        
                            
                        
                            if dive_num == dives[-1] and len(ascent_data.pres) > 1:
                                float_aggregate_data['summary']['latest']['depth'] = np.nanmax([np.nanmax(ascent_data.pres), float_aggregate_data['summary']['latest']['depth']])
                  
                    except:
                        
                        errstr = 'Failed to aggregate ascent data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                            
                # Aggregate descent data
                    
                logger.debug('Aggregating descent data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives: 
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                    
                        if(dive_num < 0):
                            continue
                        
                        logger.debug('Aggregating descent data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        descent_data = dive_data.descent
                        df_profile = pd.DataFrame(
                            {
                                'dive':             np.array([dive_data.dive_num] * descent_data.n, dtype='int'),
                                'pres':             np.array(descent_data.pres, dtype='float'),
                                'dt_epoch':         np.array(descent_data.dt_epoch, dtype='float'),
                                'dt_ref_delta_sec': np.array(descent_data.dt_ref_delta_sec, dtype='float'),
                                'dt_str':           np.array(descent_data.dt_str, dtype='str'),
                                'phase_code':       np.array(descent_data.phase_code, dtype='int'),
                                'rate':             np.array(descent_data.rate, dtype='float')*100,
                            }
                        )
                        float_aggregate_data['descent'] = pd.concat([float_aggregate_data['descent'], df_profile], ignore_index=True)
    
                        # Append NaNs to separate dives
    
                        df_profile = pd.DataFrame(
                            {
                                'dive':             np.array([dive_data.dive_num], dtype='int'),
                                'pres':             np.array([np.nan], dtype='float'),
                                'dt_epoch':         np.array([np.nan], dtype='float'),
                                'dt_ref_delta_sec': np.array([np.nan], dtype='float'),
                                'dt_str':           np.array([np.nan], dtype='str'),
                                'phase_code':       np.array([-1], dtype='int'),
                                'rate':             np.array([np.nan], dtype='float'),
                            }
                        )
                        float_aggregate_data['descent'] = pd.concat([float_aggregate_data['descent'], df_profile], ignore_index=True)
                        
                        # Get Summary data
                        
                        if not np.isnan(descent_data.pres).all():
                            if dive_num == dives[-1] and len(descent_data.pres) > 1:
                                float_aggregate_data['summary']['latest']['depth'] = np.nanmax([np.nanmax(descent_data.pres), float_aggregate_data['summary']['latest']['depth']])
                        
                    except:
                        
                        errstr = 'Failed to aggregate descent data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
    
                # Aggregate drift data
                
                logger.debug('Aggregating drift data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives: 
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                    
                        if(dive_num < 0):
                            continue
                        
                        logger.debug('Aggregating drift data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        drift_data = dive_data.drift
                        if(drift_data.n < 3):
                            continue
                        df_drift = pd.DataFrame(
                            {
                                'dive':     np.array([dive_data.dive_num] * drift_data.n, dtype='int'),
                                'pres':     np.array(drift_data.pres, dtype='float'),
                                'temp':     np.array(drift_data.temp, dtype='float'),
                                'saln':     np.array(drift_data.saln, dtype='float'),
                                'dens':     np.array(drift_data.dens, dtype='float'),
                                'sigma0':   np.array(drift_data.sigma0, dtype='float'),
                                'dt_epoch': np.array(drift_data.dt_epoch, dtype='float'),
                                'dt_ref_delta_sec': np.array(drift_data.dt_ref_delta_sec, dtype='float'),
                                'dt_str':   np.array(drift_data.dt_str, dtype='str'),
                            }
                        )
                        float_aggregate_data['drift'] = pd.concat([float_aggregate_data['drift'], df_drift], ignore_index=True)    
                    
                        df_drift = pd.DataFrame(
                            {
                                'dive':     np.array([dive_data.dive_num], dtype='int'),
                                'pres':     np.array(np.nan, dtype='float'),
                                'temp':     np.array(np.nan, dtype='float'),
                                'saln':     np.array(np.nan, dtype='float'),
                                'dens':     np.array(np.nan, dtype='float'),
                                'sigma0':   np.array(np.nan, dtype='float'),
                                'dt_epoch': np.array(np.nan, dtype='float'),
                                'dt_ref_delta_sec': np.array(np.nan, dtype='float'),
                                'dt_str':   np.array(np.nan, dtype='str'),
                            }
                        )
                        float_aggregate_data['drift'] = pd.concat([float_aggregate_data['drift'], df_drift], ignore_index=True)    

                        
                        # Get Summary data
                        
                        if not np.isnan(drift_data.pres).all():
                            if dive_num == dives[-1] and len(drift_data.pres) > 1:
                                float_aggregate_data['summary']['latest']['depth'] = np.nanmax([np.nanmax(drift_data.pres), float_aggregate_data['summary']['latest']['depth']])
                            
                    except:
                        
                        errstr = 'Failed to aggregate drift data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                
                # Aggregate engineering data
                
                logger.debug('Aggregating engineering data for float {:04d}...'.format(float_id))
                try: 
                    
                    df_eng = pd.DataFrame({
                        'dive': np.array([x.dive_num for x in float_data.dives], dtype='int'),
                        'ascent_dt_epoch': np.array([x.engineering.ascent.dt_start.dt_epoch for x in float_data.dives], dtype='int'),
                        'ascent_dt_str': np.array([x.engineering.ascent.dt_start.dt_str for x in float_data.dives], dtype='str'),
                        'ascent_vac0': np.array([x.engineering.ascent.vac0 for x in float_data.dives], dtype='float'),
                        'ascent_vac1': np.array([x.engineering.ascent.vac1 for x in float_data.dives], dtype='float'),
                        'descent_dt_epoch': np.array([x.engineering.descent.dt_start.dt_epoch for x in float_data.dives], dtype='int'),
                        'descent_dt_str': np.array([x.engineering.descent.dt_start.dt_str for x in float_data.dives], dtype='str'),
                        'descent_vac0': np.array([x.engineering.descent.vac0 for x in float_data.dives], dtype='float'),
                        'descent_vac1': np.array([x.engineering.descent.vac1 for x in float_data.dives], dtype='float'),
                        'drift_dt_epoch': np.array([x.engineering.drift.dt_start.dt_epoch for x in float_data.dives], dtype='int'),
                        'drift_dt_str': np.array([x.engineering.drift.dt_start.dt_str for x in float_data.dives], dtype='str'),
                        'drift_vac0': np.array([x.engineering.drift.vac0 for x in float_data.dives], dtype='float'),
                        'drift_vac1': np.array([x.engineering.drift.vac1 for x in float_data.dives], dtype='float'),
                        'surface_dt_epoch': np.array([x.engineering.surface.dt_start.dt_epoch for x in float_data.dives], dtype='int'),
                        'surface_dt_str': np.array([x.engineering.surface.dt_start.dt_str for x in float_data.dives], dtype='str'),
                        'surface_vac0': np.array([x.engineering.surface.vac0 for x in float_data.dives], dtype='float'),
                        'surface_vac1': np.array([x.engineering.surface.vac1 for x in float_data.dives], dtype='float'),
                        })
                    
                    float_aggregate_data['engineering'] = df_eng

                except:
                    errstr = 'Failed to aggregate engineering data for float {}, dive {}'.format(float_id, dive_num)

                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)

                # Aggregate mission parameters and all engineering values (one row per dive), and
                # the engineering error codes. Dive JSON written before these fields existed has
                # none, so those dives are left out.

                logger.debug('Aggregating mission and engineering values for float {:04d}...'.format(float_id))
                try:

                    rows_mission, rows_eng, rows_err = [], [], []

                    for dive in float_data.dives:

                        mission = getattr(dive, 'mission', None)
                        if mission is not None and (mission.fwver or mission.parameters):
                            row = { 'dive': dive.dive_num, 'fwver': mission.fwver,
                                    'cfgver': getattr(mission, 'cfgver', ''), 'ctdtype': mission.ctdtype }
                            row.update(mission.parameters)
                            rows_mission.append(row)

                        row = { 'dive': dive.dive_num }
                        for section in ('descent', 'drift', 'ascent', 'surface', 'ice', 'bit'):
                            obj = getattr(dive.engineering, section, None)
                            if obj is None:
                                continue
                            for attr, value in vars(obj).items():
                                if isinstance(value, DateTime):
                                    value = value.dt_str
                                row[section + '.' + attr] = value
                        for section, cols in getattr(dive.engineering, 'extra', {}).items():
                            for col, value in cols.items():
                                row[section + '.' + col] = value
                        rows_eng.append(row)

                        for error in getattr(dive.engineering, 'errors', []):
                            rows_err.append(dict({ 'dive': dive.dive_num }, **error))

                    if rows_mission:
                        float_aggregate_data['mission'] = pd.DataFrame(rows_mission)
                    if rows_eng:
                        # Drop columns that no dive of this float reports (NaN, '' or unset times).
                        df = pd.DataFrame(rows_eng).replace('', np.nan)
                        float_aggregate_data['engineering_data'] = df.dropna(axis = 1, how = 'all')
                    if rows_err:
                        float_aggregate_data['engineering_errors'] = pd.DataFrame(rows_err)

                except:
                    errstr = 'Failed to aggregate mission and engineering values for float {}'.format(float_id)

                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)

                # Aggregate gps data:
                
                logger.debug('Aggregating GPS data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives: 
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                    
                        logger.debug('Aggregating GPS data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        gps_data = dive_data.gps
                        for gps_fix in gps_data:
                            
                            # Ignore invalid GPS fixes
                            
                            if not gps_fix.is_valid():
                                continue
                            
                            # Generate dataframe entry
                        
                            df_gps = pd.DataFrame(
                                {
                                    'dive':     np.array([dive_data.dive_num], dtype='int'),
                                    'dt_epoch': np.array([gps_fix.dt_epoch], dtype='int'),
                                    'dt_str':   np.array([gps_fix.dt_str], dtype='str'),
                                    'lat':      np.array([gps_fix.lat], dtype='float'),
                                    'lon':      np.array([gps_fix.lon], dtype='float'),
                                    'cep_km':   np.array([gps_fix.cep_km], dtype='float'),
                                    'idx':      np.array([gps_fix.idx], dtype='int'),
                                    'ttf_sec':  np.array([gps_fix.ttf_sec], dtype='int'),
                                    'nsat':     np.array([gps_fix.nsat], dtype='int'),
                                    'snr_min':  np.array([gps_fix.snr_min], dtype='int'),
                                    'snr_avg':  np.array([gps_fix.snr_avg], dtype='int'),
                                    'snr_max':  np.array([gps_fix.snr_max], dtype='int'),
                                    'hdop':     np.array([gps_fix.hdop], dtype='float'),
                                }
                            )
                            float_aggregate_data['gps'] = pd.concat([float_aggregate_data['gps'], df_gps], ignore_index=True)
                            
                            
                            # TODO Get summary data
                            
                            
                        
                            # Get startup GPS fix
                            if dive_num < 0 and gps_fix.dt_epoch > float_aggregate_data['summary']['startup']['gps'].dt_epoch:
                                float_aggregate_data['summary']['startup']['gps'] = gps_fix
                            
                            # Get deployment GPS fix
                            if dive_num >= 0 and gps_fix.dt_epoch > float_aggregate_data['summary']['startup']['gps'].dt_epoch and (gps_fix.dt_epoch < float_aggregate_data['summary']['deployment']['gps'].dt_epoch or float_aggregate_data['summary']['deployment']['gps'].dt_epoch == -1):
                                float_aggregate_data['summary']['deployment']['gps'] = gps_fix
                                
                            # Get latest GPS fix
                            if dive_num >= 0 and gps_fix.dt_epoch > float_aggregate_data['summary']['latest']['gps'].dt_epoch:
                                float_aggregate_data['summary']['latest']['gps'] = gps_fix
                    except:
                        
                        errstr = 'Failed to aggregate gps data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                
                float_aggregate_data['gps'].sort_values('dt_epoch', inplace=True, ignore_index=True)
                
                # Aggregate Mission Parameters:
                
                logger.debug('Aggregating mission parameters for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives: 
                    
                    # dive_num = dive_data.dive_num
                    
                    # Get Summary data:
                    
                    if len(dive_data.mission.fwver):
                        float_aggregate_data['summary']['fwver'] = dive_data.mission.fwver
                        
                    if len(dive_data.mission.ctdtype):
                        float_aggregate_data['summary']['ctdtype'] = dive_data.mission.ctdtype
                
                
                # Aggregate profile data:
                
                logger.debug('Aggregating profile data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives: 
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                    
                        if(dive_num < 0):
                            continue
                        
                        logger.debug('Aggregating profile data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        profile_data = dive_data.profile
                        
                        # TODO Check for "ghost" profiles (float did not descend)
                        
                        if dive_data.ascent.n > 0 and max(dive_data.ascent.pres) < 3:
                            continue
                            
                        if dive_data.descent.n > 0 and max(dive_data.descent.pres) < 3:
                            continue
                        
                        # Append profile data
                        
                        df_profile = pd.DataFrame(
                            {
                                'dive':   np.array([dive_num] * profile_data.n, dtype='int'),
                                'pres':   np.array(profile_data.pres, dtype='float'),
                                'temp':   np.array(profile_data.temp, dtype='float'),
                                'tcnd':   np.array(profile_data.tcnd, dtype='float'),
                                'saln':   np.array(profile_data.saln, dtype='float'),
                                'saln_corr': np.array(getattr(profile_data, 'saln_corr', [np.nan] * profile_data.n), dtype='float'),
                                'dens':   np.array(profile_data.dens, dtype='float'),
                                'sigma0': np.array(profile_data.sigma0, dtype='float'),
                            }
                        )
                        float_aggregate_data['profile'] = pd.concat([float_aggregate_data['profile'], df_profile], ignore_index=True)
                    
                        # Get Summary data
                        
                        if not np.isnan(profile_data.pres).all():
                        
                            depth = np.nanmax(profile_data.pres)
                        
                            float_aggregate_data['summary']['profdepthsum'] = depth + float_aggregate_data['summary']['profdepthsum']
                        
                            if dive_num == dives[-1] and len(profile_data.pres) > 1:
                                float_aggregate_data['summary']['latest']['depth'] = np.nanmax([depth, float_aggregate_data['summary']['latest']['depth']])
                    
                    except:
                        
                        errstr = 'Failed to aggregate profile data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                    
                float_aggregate_data['profile'].sort_values('dive', ignore_index=True)
                
                # Aggregate pump data
                
                logger.debug('Aggregating pump data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives:
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                    
                        if(dive_num < 0):
                            continue
                        
                        logger.debug('Aggregating pump data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        pump_data = dive_data.pump
                        df_pump = pd.DataFrame(
                            {
                                'dive':   np.array([dive_num] * pump_data.n, dtype='int'),
                                'pres':   np.array(pump_data.pres, dtype='float'),
                                'time':   np.array(pump_data.time, dtype='float'),
                                'volt':   np.array(pump_data.volt, dtype='float'),
                                'curr':   np.array(pump_data.curr, dtype='float'),
                                'curr_adj':   np.array(pump_data.curr_adj, dtype='float'),
                                'vac':    np.array(pump_data.vac, dtype='float'),
                                'power':  np.array(pump_data.power, dtype='float'),
                                'energy': np.array(pump_data.energy, dtype='float'),
                            }
                        )
                        float_aggregate_data['pump'] = pd.concat([float_aggregate_data['pump'], df_pump], ignore_index=True)
                    
                    except:
                        
                        errstr = 'Failed to aggregate pump data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                
                # Aggregate SBD data
                
                logger.debug('Aggregating SBD data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives: 
                    
                    dive_num = dive_data.dive_num
                
                    try:
                
                        logger.debug('Aggregating SBD data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                
                        # Generate dataframe entry
                    
                        sbd_data = dive_data.sbd
                        df_sbd = pd.DataFrame(
                            {
                                'dive':     np.array([dive_data.dive_num]*len(sbd_data), dtype='int'),
                                'dt_epoch': np.array([sbd.dt_epoch for sbd in sbd_data], dtype='int'),
                                'dt_str':   np.array([sbd.dt_str for sbd in sbd_data], dtype='str'),
                                'lat':      np.array([sbd.lat for sbd in sbd_data], dtype='float'),
                                'lon':      np.array([sbd.lon for sbd in sbd_data], dtype='float'),
                                'cep_km':   np.array([sbd.cep_km for sbd in sbd_data], dtype='float'),
                                'size_b':   np.array([sbd.size_b for sbd in sbd_data], dtype='float'),
                                'momsn':    np.array([sbd.momsn for sbd in sbd_data], dtype='int'),
                                'mtmsn':    np.array([sbd.mtmsn for sbd in sbd_data], dtype='int'),
                                'pid':      np.array([sbd.pid for sbd in sbd_data], dtype='int'),
                                
                            }
                        )
                        float_aggregate_data['sbd'] = pd.concat([float_aggregate_data['sbd'], df_sbd], ignore_index=True)
                        
                        
                        # TODO Get summary data
                        
                        for sbd in sbd_data:
                            
                            # Get startup SBD 
                            if dive_num < 0 and sbd.dt_epoch > float_aggregate_data['summary']['startup']['sbd'].dt_epoch:
                                float_aggregate_data['summary']['startup']['sbd'] = sbd
                            
                            # Get deployment SBD
                            if dive_num >= 0 and sbd.dt_epoch > float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600 and (sbd.dt_epoch < float_aggregate_data['summary']['deployment']['sbd'].dt_epoch or float_aggregate_data['summary']['deployment']['sbd'].dt_epoch == -1):
                                float_aggregate_data['summary']['deployment']['sbd'] = sbd
                                
                            # Get latest SBD
                            if dive_num >= 0 and sbd.dt_epoch > float_aggregate_data['summary']['latest']['sbd'].dt_epoch:
                                float_aggregate_data['summary']['latest']['sbd'] = sbd
                                
                        if dive_num == dives[-1]:
                            float_aggregate_data['summary']['latest']['nsbd'] = len(df_sbd)
                            
                        
                        del sbd_data
                        
                        # for sbd in sbd_data:

                            # df_sbd = pd.DataFrame(
                                # {
                                    # 'dive':     np.array([dive_data.dive_num], dtype='int'),
                                    # 'dt_epoch': np.array([sbd.dt_epoch], dtype='int'),
                                    # 'dt_str':   np.array([sbd.dt_str], dtype='str'),
                                    # 'lat':      np.array([sbd.lat], dtype='float'),
                                    # 'lon':      np.array([sbd.lon], dtype='float'),
                                    # 'cep_km':   np.array([sbd.cep_km], dtype='float'),
                                    # 'size_b':   np.array([sbd.size_b], dtype='float'),
                                    # 'momsn':    np.array([sbd.momsn], dtype='int'),
                                    # 'mtmsn':    np.array([sbd.mtmsn], dtype='int'),
                                    # 'pid':      np.array([sbd.pid], dtype='int'),
                                    
                                # }
                            # )
                            # float_aggregate_data['sbd'] = pd.concat([float_aggregate_data['sbd'], df_sbd], ignore_index=True)
                
                    except:
                        
                        errstr = 'Failed to aggregate SBD data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                        
                # Aggregate surface data
                
                logger.debug('Aggregating surface data for float {:04d}...'.format(float_id))
                for dive_data in float_data.dives:
                    
                    dive_num = dive_data.dive_num
                    
                    try: 
                        
                        if(dive_num < 0):
                            continue
                        
                        logger.debug('Aggregating surface data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        surface_data = dive_data.surface
                        df_profile = pd.DataFrame(
                            {
                                'dive':             np.array([dive_data.dive_num] * surface_data.n, dtype='int'),
                                'dt_epoch':         np.array(surface_data.dt_epoch, dtype='float'),
                                'dt_ref_delta_sec': np.array(surface_data.dt_ref_delta_sec, dtype='float'),
                                'dt_str':           np.array(surface_data.dt_str, dtype='str'),
                                'pres':             np.array(surface_data.pres, dtype='float'),
                                'temp':             np.array(surface_data.temp, dtype='float'),
                            }
                        )
                        float_aggregate_data['surface'] = pd.concat([float_aggregate_data['surface'], df_profile], ignore_index=True)
                        
                        # Append NaNs to separate dives
                        
                        df_profile = pd.DataFrame(
                            {
                                'dive':             np.array([dive_data.dive_num], dtype='int'),
                                'dt_epoch':         np.array([np.nan], dtype='float'),
                                'dt_ref_delta_sec': np.array([np.nan], dtype='float'),
                                'dt_str':           np.array([np.nan], dtype='str'),
                                'pres':             np.array([np.nan], dtype='float'),
                                'temp':             np.array([np.nan], dtype='float'),
                            }
                        )
                        float_aggregate_data['surface'] = pd.concat([float_aggregate_data['surface'], df_profile], ignore_index=True)
                        
                    except:
                        
                        errstr = 'Failed to aggregate surface data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                           
                # TODO Aggregate wave data
                
                logger.debug('Aggregating wave data for float {:04d}...'.format(float_id))

                for dive_data in float_data.dives:
                    
                    dive_num = dive_data.dive_num
                    
                    try:
                    
                        if(dive_num < 0):
                            continue
                    
                        if not hasattr(dive_data, 'wave'):
                            continue
                            
                        logger.debug('Aggregating wave data for float {:04d}, dive {:d}...'.format(float_id, dive_num))
                        
                        wave_data = dive_data.wave
                        
                        if wave_data.slen < 1:
                            continue
                        
                        df_wave = pd.DataFrame(
                            {
                                'dive':         np.array([dive_num], dtype='int'),
                                'mwd':          np.array([wave_data.mwd], dtype='int'),
                                'period_peak':  np.array([wave_data.period_peak], dtype='float'),
                                'period_avg':   np.array([wave_data.period_avg], dtype='float'),
                                'period_zero':  np.array([wave_data.period_zero], dtype='float'),
                                'swh':          np.array([wave_data.swh], dtype='float'),
                            }
                        )
                        float_aggregate_data['wave'] = pd.concat([float_aggregate_data['wave'], df_wave], ignore_index=True)
                        
                        df_wave_spectra = pd.DataFrame(
                            {
                                'dive':     np.array([dive_num]*len(wave_data.freq), dtype='int'),
                                'freq':     np.array(wave_data.freq, dtype='float'),
                                'Szz_g':    np.array(wave_data.Szz_g, dtype='float'),
                                'Syy_g':    np.array(wave_data.Syy_g, dtype='float'),
                                'Sxx_g':    np.array(wave_data.Sxx_g, dtype='float'),
                                'Cxy_g':    np.array(wave_data.Cxy_g, dtype='float'),
                                'Qzx_g':    np.array(wave_data.Qzx_g, dtype='float'),
                                'Qzy_g':    np.array(wave_data.Qzy_g, dtype='float'),
                                'Szz':      np.array(wave_data.Szz, dtype='float'),
                                'Syy':      np.array(wave_data.Syy, dtype='float'),
                                'Sxx':      np.array(wave_data.Sxx, dtype='float'),
                                'Cxy':      np.array(wave_data.Cxy, dtype='float'),
                                'Qzx':      np.array(wave_data.Qzx, dtype='float'),
                                'Qzy':      np.array(wave_data.Qzy, dtype='float'),
                                'a0':       np.array(wave_data.a0, dtype='float'),
                                'a1':       np.array(wave_data.a1, dtype='float'),
                                'a2':       np.array(wave_data.a2, dtype='float'),
                                'b1':       np.array(wave_data.b1, dtype='float'),
                                'b2':       np.array(wave_data.b2, dtype='float'),
                                'r1':       np.array(wave_data.r1, dtype='float'),
                                'r2':       np.array(wave_data.r2, dtype='float'),
                                'theta1':   np.array(wave_data.theta1, dtype='int'),
                                'theta2':   np.array(wave_data.theta2, dtype='int'),
                            }
                        )
                        float_aggregate_data['wave_spectra'] = pd.concat([float_aggregate_data['wave_spectra'], df_wave_spectra], ignore_index=True)
                        
                    
                    except:
                    
                        errstr = 'Failed to aggregate wave data for float {}, dive {}'.format(float_id, dive_num)
                        
                        if hasattr(args, 'halt') and args.halt:
                            raise Exception(errstr)
                        logger.error(errstr)
                
                float_aggregate_data['wave_spectra'].sort_values(['dive', 'freq'], inplace=True, ignore_index=True)

                   
                if (len(float_aggregate_data['wave']) < 1) or (len(float_aggregate_data['wave_spectra']) < 1):
                
                    float_aggregate_data.pop('wave', None)
                    float_aggregate_data.pop('wave_spectra', None)
                    
                           
                # Post process -- get dive/profile positions and date/times
                
                logger.debug('Processing profile positions for float {:04d}...'.format(float_id))
                try: 
                    for dive_num in sorted([dive_data.dive_num for dive_data in float_data.dives]):
                    
                        logger.debug('Processing profile position for float {:04d}, dive {}...'.format(float_id, dive_num))
                        
                        try: 
                        
                            df_dive_gps = float_aggregate_data['gps'][ (float_aggregate_data['gps']['dive'] == dive_num)
                                                                         & ((float_aggregate_data['gps']['idx'] == 0) 
                                                                            | (float_aggregate_data['gps']['idx'] == 2))
                                                                         & (float_aggregate_data['gps']['dt_epoch'] > 0) ]
                            
                            if len(df_dive_gps) > 0: 
                                df_dive_pos = pd.DataFrame(
                                    {
                                        'dive':     np.array([dive_num], dtype='int'),
                                        'dt_epoch': np.array(df_dive_gps['dt_epoch'].values[0], dtype='int'),
                                        'dt_str':   np.array(df_dive_gps['dt_str'].values[0], dtype='str'),
                                        'lat':      np.array(df_dive_gps['lat'].values[0], dtype='float'),
                                        'lon':      np.array(df_dive_gps['lon'].values[0], dtype='float'),
                                        'cep_km':   np.array(df_dive_gps['cep_km'].values[0], dtype='float'),
                                        'src':      np.array(['gps'], dtype='str'),
                                    }
                                )
                                
                                float_aggregate_data['profile_position'] = pd.concat([float_aggregate_data['profile_position'], df_dive_pos], ignore_index=True)
                                
                                continue
                            
                            df_dive_gps = float_aggregate_data['gps'][ (float_aggregate_data['gps']['dive'] == (dive_num + 1))
                                                                         & (float_aggregate_data['gps']['idx'] == 1)
                                                                         & (float_aggregate_data['gps']['dt_epoch'] > 0) ]
                            
                            if len(df_dive_gps) > 0: 
                                df_dive_pos = pd.DataFrame(
                                    {
                                        'dive':     np.array([dive_num], dtype='int'),
                                        'dt_epoch': np.array(df_dive_gps['dt_epoch'].values[0], dtype='int'),
                                        'dt_str':   np.array(df_dive_gps['dt_str'].values[0], dtype='str'),
                                        'lat':      np.array(df_dive_gps['lat'].values[0], dtype='float'),
                                        'lon':      np.array(df_dive_gps['lon'].values[0], dtype='float'),
                                        'cep_km':   np.array(df_dive_gps['cep_km'].values[0], dtype='float'),
                                        'src':      np.array(['gps'], dtype='str'),
                                    }
                                )
                                
                                float_aggregate_data['profile_position'] = pd.concat([float_aggregate_data['profile_position'], df_dive_pos], ignore_index=True)
                        
                        except:
                            
                            errstr = 'Failed to get profile position for float {}, dive {}'.format(float_id, dive_num)
                            if hasattr(args, 'halt') and args.halt:
                                raise Exception(errstr)
                            logger.error(errstr)
                except:
                        
                    errstr = 'Failed to get profile positions for float {}'.format(float_id)
                    
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                    
                
                # Post Process - Get date/times for profiles with no GPS
                
                for dive_num in [x.dive_num for x in float_data.dives]:
                    df_dive_pos = float_aggregate_data['profile_position'][float_aggregate_data['profile_position']['dive'] == dive_num]
                
                    if len(df_dive_pos) == 0:
                        df_eng = float_aggregate_data['engineering'][float_aggregate_data['engineering']['dive'] == dive_num]
                        
                        if df_eng['surface_dt_epoch'].values[0] > 0:
                        
                            df_dive_pos = pd.DataFrame(
                                {
                                    'dive':     np.array([dive_num], dtype='int'),
                                    'dt_epoch': np.array([df_eng['surface_dt_epoch'].values[0]], dtype='int'),
                                    'dt_str':   np.array([df_eng['surface_dt_str'].values[0]], dtype='str'),
                                    'lat':      np.array([np.nan], dtype='float'),
                                    'lon':      np.array([np.nan], dtype='float'),
                                    'cep_km':   np.array([np.nan], dtype='float'),
                                    'src':      np.array(['none'], dtype='str'),
                                }
                            )
                            
                            float_aggregate_data['profile_position'] = pd.concat([float_aggregate_data['profile_position'], df_dive_pos], ignore_index=True)
                            continue
                
                float_aggregate_data['profile_position'].sort_values('dt_epoch', inplace=True, ignore_index=True)
                
                # Post Process - Generate composite Iridium Positions
                try: 
                
                    composite_positions = []
                
                    # Iterate over SBD messages
                
                    for dive_data in float_data.dives:
                        
                        for sbd in dive_data.sbd:
                        
                            if not sbd.is_valid():
                                continue
                        
                            new_cpos = True
                        
                            # If message falls within CPOS_SPAN_SECONDS/2 seconds of existing composite position, add message to existing composite position
                            
                            for cpos in composite_positions:
                            
                                if (sbd.dt_epoch > ((cpos.dt_epoch_max + cpos.dt_epoch_min)/2 - CPOS_SPAN_SECONDS/2)) and (sbd.dt_epoch < ((cpos.dt_epoch_max + cpos.dt_epoch_min)/2 + CPOS_SPAN_SECONDS/2)):
                                
                                    cpos.add_position(sbd, compute = False)
                                    
                                    new_cpos = False
                                    continue
                                    
                            # Otherwise, create new composite position
                            
                            if new_cpos:
                            
                                composite_positions.append(CompositePosition(pos = sbd, compute = False))
                                
                    # Process composite positions
                    
                    for cpos in composite_positions:
                    
                        # Compute weighted averages of composite positions
                    
                        cpos.compute()
                        
                        # Find corresponding profile position (nearest)
                                    
                        # ppos = float_aggregate_data['profile_position'][np.abs(float_aggregate_data['profile_position']['dt_epoch'] - cpos.dt_epoch) < CPOS_SPAN_SECONDS/2]
                        df_nearest_pos = float_aggregate_data['profile_position'][float_aggregate_data['profile_position']['dt_epoch'].astype(np.double).sub(cpos.dt_epoch).abs() <= CPOS_SPAN_SECONDS]
                        
                        if len(df_nearest_pos) < 1: 
                            continue # No corresponding profile position    
                        
                        ppos = df_nearest_pos.loc[[df_nearest_pos['dt_epoch'].astype(np.double).sub(cpos.dt_epoch).abs().idxmin()]]
                    
                        # Assign dive numbers to composite positions 
                        
                        cpos.dive = ppos['dive'].values[0]
                        
                        # Assign composite position to dive without position
                            
                        if not cpos.is_valid() or cpos.cep_km > CPOS_MAX_CEP_KM:
                            continue # Error too large
                        
                        float_aggregate_data['profile_position'].loc[
                                (float_aggregate_data['profile_position']['dive'] == cpos.dive ) & ( float_aggregate_data['profile_position']['lat'].isna() | float_aggregate_data['profile_position']['lon'].isna()),
                                ['dt_epoch','dt_str','lat','lon','cep_km','src']] = [cpos.dt_epoch, cpos.dt_str, cpos.lat, cpos.lon, cpos.cep_km, 'iridium']
                
                    # Generate aggregate Iridium positions:
                    
                    df_iridium_pos = pd.DataFrame(
                        {
                            'dive':         np.array([cpos.dive for cpos in composite_positions], dtype='float'),
                            'dt_epoch':     np.array([cpos.dt_epoch for cpos in composite_positions], dtype='int'),
                            'dt_str':       np.array([cpos.dt_str for cpos in composite_positions], dtype='str'),
                            'dt_min_epoch': np.array([cpos.dt_epoch_min for cpos in composite_positions], dtype='int'),
                            'dt_min_str':   np.array([DateTime().dt_from_epoch(cpos.dt_epoch_min).dt_str for cpos in composite_positions], dtype='str'),
                            'dt_max_epoch': np.array([cpos.dt_epoch_max for cpos in composite_positions], dtype='int'),
                            'dt_max_str':   np.array([DateTime().dt_from_epoch(cpos.dt_epoch_max).dt_str for cpos in composite_positions], dtype='str'),
                            'lat':          np.array([cpos.lat for cpos in composite_positions], dtype='float'),
                            'lon':          np.array([cpos.lon for cpos in composite_positions], dtype='float'),
                            'cep_km':       np.array([cpos.cep_km for cpos in composite_positions], dtype='float'),
                            'num_pos':      np.array([cpos.num_pos for cpos in composite_positions], dtype='int'),
                        }
                    )
                    float_aggregate_data['iridium_position'] = pd.concat([float_aggregate_data['iridium_position'], df_iridium_pos], ignore_index=True)
                
                    float_aggregate_data['iridium_position'].sort_values('dt_epoch', inplace=True, ignore_index=True)
               
                except: 
                
                    errstr = 'Failed to generate composite Iridium Positions for float {}.'.format(float_id)
                    
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                    
                # Post Process - Generate submerged and suface displacements
                
                logger.debug('Processing displacements for float {:04d}...'.format(float_id))
                try:
    
                    # Group GPS data by dive number
    
                    float_dives_gps = dict()
                    for dive_data in float_data.dives:
                        float_dives_gps[dive_data.dive_num] = dive_data.gps
                    
                    # Iterate over dives
                
                    for dive in range(1, max(float_dives_gps.keys()) + 1):
                    
                        gps1, gps2, gps3 = None, None, None

                        
                        # Get gps data for current dive
                        
                        if dive in float_dives_gps.keys():
                            
                            for gps_data in float_dives_gps[dive]:
                            
                                if gps_data.idx == 1:
                                    
                                    gps1 = gps_data
                                    
                                elif gps_data.idx == 2: 
                                
                                    gps2 = gps_data
                        
                        # Get gps data for next dive
                        
                        if dive + 1 in float_dives_gps.keys():
                        
                            for gps_data in float_dives_gps[dive + 1]:
                            
                                if gps_data.idx == 1:
                                    
                                    gps3 = gps_data
                                    break
                        
                        # Compute submerged displacement
                        
                        if (gps1 is not None) and (gps2 is not None) and (gps2.dt_epoch > gps1.dt_epoch):
                        
                            vec = PositionVector(pos1 = gps1, pos2 = gps2, compute = True)
                            
                            df_displacement_submerged =  pd.DataFrame({
                                'dive':             np.array([dive],   dtype='int'),
                                'azi':              np.array([vec.azi], dtype='float'),
                                'dist_m':           np.array([vec.dist_m], dtype='float'),
                                'duration_s':       np.array([vec.duration_s], dtype='float'),
                                'speed_mps':        np.array([vec.speed_mps], dtype='float'),
                                'pos1_dt_epoch':    np.array([vec.pos1.dt_epoch], dtype='int'),
                                'pos1_dt_str':      np.array([vec.pos1.dt_str], dtype='str'),
                                'pos1_lat':         np.array([vec.pos1.lat], dtype='float'),
                                'pos1_lon':         np.array([vec.pos1.lon], dtype='float'),
                                'pos2_dt_epoch':    np.array([vec.pos2.dt_epoch], dtype='int'),
                                'pos2_dt_str':       np.array([vec.pos2.dt_str], dtype='str'),
                                'pos2_lat':         np.array([vec.pos2.lat], dtype='float'),
                                'pos2_lon':          np.array([vec.pos2.lon], dtype='float'),
                                })
                        
                        else:
                        
                            df_displacement_submerged =  pd.DataFrame({
                                'dive':             np.array([dive],   dtype='int'),
                                'azi':              np.array([np.nan], dtype='float'),
                                'dist_m':           np.array([np.nan], dtype='float'),
                                'duration_s':       np.array([np.nan], dtype='float'),
                                'speed_mps':        np.array([np.nan], dtype='float'),
                                'pos1_dt_epoch':    np.array([-1], dtype='int'),
                                'pos1_dt_str':      np.array([''], dtype='str'),
                                'pos1_lat':         np.array([np.nan], dtype='float'),
                                'pos1_lon':         np.array([np.nan], dtype='float'),
                                'pos2_dt_epoch':    np.array([-1], dtype='int'),
                                'pos2_dt_str':      np.array([''], dtype='str'),
                                'pos2_lat':         np.array([np.nan], dtype='float'),
                                'pos2_lon':         np.array([np.nan], dtype='float'),
                                })
                                
                        # Compute surface dispacement
                        
                        if (gps2 is not None) and (gps3 is not None) and (gps3.dt_epoch > gps2.dt_epoch):
                        
                            vec = PositionVector(pos1 = gps2, pos2 = gps3, compute = True)
                            
                            df_displacement_surface =  pd.DataFrame({
                                'dive':             np.array([dive],   dtype='int'),
                                'azi':              np.array([vec.azi], dtype='float'),
                                'dist_m':           np.array([vec.dist_m], dtype='float'),
                                'duration_s':       np.array([vec.duration_s], dtype='float'),
                                'speed_mps':        np.array([vec.speed_mps], dtype='float'),
                                'pos1_dt_epoch':    np.array([vec.pos1.dt_epoch], dtype='int'),
                                'pos1_dt_str':      np.array([vec.pos1.dt_str], dtype='str'),
                                'pos1_lat':         np.array([vec.pos1.lat], dtype='float'),
                                'pos1_lon':         np.array([vec.pos1.lon], dtype='float'),
                                'pos2_dt_epoch':    np.array([vec.pos2.dt_epoch], dtype='int'),
                                'pos2_dt_str':       np.array([vec.pos2.dt_str], dtype='str'),
                                'pos2_lat':         np.array([vec.pos2.lat], dtype='float'),
                                'pos2_lon':          np.array([vec.pos2.lon], dtype='float'),
                                })
                                
                        else:
                        
                            df_displacement_surface =  pd.DataFrame({
                                'dive':             np.array([dive],   dtype='int'),
                                'azi':              np.array([np.nan], dtype='float'),
                                'dist_m':           np.array([np.nan], dtype='float'),
                                'duration_s':       np.array([np.nan], dtype='float'),
                                'speed_mps':        np.array([np.nan], dtype='float'),
                                'pos1_dt_epoch':    np.array([-1], dtype='int'),
                                'pos1_dt_str':      np.array([''], dtype='str'),
                                'pos1_lat':         np.array([np.nan], dtype='float'),
                                'pos1_lon':         np.array([np.nan], dtype='float'),
                                'pos2_dt_epoch':    np.array([-1], dtype='int'),
                                'pos2_dt_str':      np.array([''], dtype='str'),
                                'pos2_lat':         np.array([np.nan], dtype='float'),
                                'pos2_lon':         np.array([np.nan], dtype='float'),
                                })                       

                        float_aggregate_data['displacement_submerged'] = pd.concat([float_aggregate_data['displacement_submerged'], df_displacement_submerged], ignore_index=True)
                        float_aggregate_data['displacement_surface'] = pd.concat([float_aggregate_data['displacement_surface'], df_displacement_surface], ignore_index=True)

                except: 
                
                    errstr = 'Failed to generate displacements for float {}.'.format(float_id)
                    
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                
                # Post process - Generate Cycle Timings
                
                try:
                
                    logger.debug('Processing cycle timings for float {:04d}...'.format(float_id))
                    
                    df_eng = float_aggregate_data['engineering']
                
                    previous_surface_duration = 0
                    
                    for dive_num in sorted([dive_data.dive_num for dive_data in float_data.dives]):
                    
                        logger.debug('Processing cycle timings for float {:04d}, dive {}...'.format(float_id, dive_num))
                    
                        ascent_duration = 0.;
                        descent_duration = 0.;
                        drift_duration = 0.;
                        surface_duration = 0.;
                        total_duration = 0.;
                    
                        ascent_dt = DateTime()
                        descent_dt = DateTime()
                        drift_dt = DateTime()
                        surface_dt = DateTime()
                        next_dive_descent_dt = DateTime()
                        
                        # Get cycle phase times from engineering data
                        
                        df_eng = float_aggregate_data['engineering'][float_aggregate_data['engineering']['dive'] == dive_num]
                        
                        if len(df_eng) > 0:
                            
                            ascent_dt.dt_from_epoch(df_eng['ascent_dt_epoch'].values[0])
                            descent_dt.dt_from_epoch(df_eng['descent_dt_epoch'].values[0])
                            drift_dt.dt_from_epoch(df_eng['drift_dt_epoch'].values[0])
                            surface_dt.dt_from_epoch(df_eng['surface_dt_epoch'].values[0])
                        
                        df_eng = float_aggregate_data['engineering'][float_aggregate_data['engineering']['dive'] == (dive_num + 1)]
                        
                        if len(df_eng) > 0:
                        
                            next_dive_descent_dt.dt_from_epoch(df_eng['descent_dt_epoch'].values[0])
                            
                        # TODO if engineering not available, get time from ascent/descent time-series
                        
                        # Calculate cycle phase durations - descent                     
                        
                        if descent_dt.is_valid() and drift_dt.is_valid():
                        
                            descent_duration = drift_dt.dt_epoch - descent_dt.dt_epoch
                            
                        if descent_duration > 0 and descent_duration < 864000: # 10 days
                        
                            total_duration = total_duration + descent_duration
                        
                        else:
                        
                            descent_duration = np.nan
                        
                        # Calculate cycle phase durations - drift                     
                        
                        if drift_dt.is_valid() and ascent_dt.is_valid():
                        
                            drift_duration = ascent_dt.dt_epoch - drift_dt.dt_epoch
                        
                        if drift_duration > 0 and drift_duration < 3456000: # 40 days
                        
                            total_duration = total_duration + drift_duration
                        
                        else:
                        
                            drift_duration = np.nan

                        # Calculate cycle phase durations - ascent  

                        if ascent_dt.is_valid() and surface_dt.is_valid():
                        
                            ascent_duration = surface_dt.dt_epoch - ascent_dt.dt_epoch
 
                        if ascent_duration > 0 and ascent_duration < 864000: # 10 days
                        
                            total_duration = total_duration + ascent_duration
                        
                        else:
                        
                            ascent_duration = np.nan

                        # Calculate cycle phase durations - surface  

                        
                        if surface_dt.is_valid() and next_dive_descent_dt.is_valid():
                        
                            surface_duration = next_dive_descent_dt.dt_epoch - surface_dt.dt_epoch
                            
                        if surface_duration > 0 and surface_duration < 864000: # 1 day
                        
                            total_duration = total_duration + surface_duration
                        
                        else:
                        
                            surface_duration = np.nan


                        # Generate dataframe entry

                        df_ct = pd.DataFrame({
                            'dive': np.array([dive_num], dtype='int'),
                            'ascent_dt_epoch': np.array([ascent_dt.dt_epoch], dtype='int'),
                            'ascent_dt_str': np.array([ascent_dt.dt_str], dtype='str'),
                            'ascent_duration_s': np.array([ascent_duration], dtype='float'),

                            'descent_dt_epoch': np.array([descent_dt.dt_epoch], dtype='int'),
                            'descent_dt_str': np.array([descent_dt.dt_str], dtype='str'),
                            'descent_duration_s': np.array([descent_duration], dtype='float'),
                            
                            'drift_dt_epoch': np.array([drift_dt.dt_epoch], dtype='int'),
                            'drift_dt_str': np.array([drift_dt.dt_str], dtype='str'),
                            'drift_duration_s': np.array([drift_duration], dtype='float'),
                            
                            'surface_dt_epoch': np.array([surface_dt.dt_epoch], dtype='int'),
                            'surface_dt_str': np.array([surface_dt.dt_str], dtype='str'),
                            'surface_duration_s': np.array([surface_duration], dtype='float'),
                            
                            'total_duration_s': np.array([total_duration], dtype='float'),
                            
                            })    
                            
                        float_aggregate_data['cycle_timing'] = pd.concat([float_aggregate_data['cycle_timing'], df_ct], ignore_index=True)
                
                        # Summary data
                        
                        if dive_num == dives[-1] and not np.isnan(descent_duration) and not np.isnan(ascent_duration):
                        
                            float_aggregate_data['summary']['latest']['duration'] = int(total_duration + previous_surface_duration)
                        
                        elif not np.isnan(surface_duration):
                        
                            previous_surface_duration = surface_duration
                
                
                except: 
                
                    errstr = 'Failed to generate cycle timings for float {}.'.format(float_id)
                    
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                
                
                # TODO Post Process - Get summary data
                
               
                
                
                
                float_aggregate_data['summary']['latest']['cycle'] = dives[-1] 
                
                if float_aggregate_data['summary']['latest']['sbd'].is_valid():
                
                    if float_aggregate_data['summary']['deployment']['sbd'].is_valid():
                    
                        float_aggregate_data['summary']['age'] = float_aggregate_data['summary']['latest']['sbd'].dt_epoch - float_aggregate_data['summary']['deployment']['sbd'].dt_epoch
                        
                    elif float_aggregate_data['summary']['deployment']['gps'].is_valid():
                    
                        float_aggregate_data['summary']['age'] = float_aggregate_data['summary']['latest']['sbd'].dt_epoch - float_aggregate_data['summary']['deployment']['gps'].dt_epoch
                    
                float_aggregate_data['summary']['latest']['depth'] = float(round(float_aggregate_data['summary']['latest']['depth'], 1))
                float_aggregate_data['summary']['profdepthsum'] = float(round(float_aggregate_data['summary']['profdepthsum'], 1))
                
                
                # Save float aggregate data to map:
    
                float_aggregate_map[float_id] = float_aggregate_data
                    
            except:
                
                errstr = 'Failed to aggregate data for float {}'.format(float_id)
                if hasattr(args, 'halt') and args.halt:
                    raise Exception(errstr)
                logger.error(errstr)
        
    except:
        raise Exception('Failed to aggregate dive data.')    
    
def ProcessDirectory(dpath):

    dpath = re.sub('\\\\', '/', dpath)
    
    try: 
    
        process_files = True
    
        # Check directory:
        if not os.path.exists(dpath) or not os.path.isdir(dpath):
            logger.error('Invalid path: ' + dpath)
            return
        
        # Check if directory is empty
        if len(os.listdir(dpath)) == 0:
            logger.debug('Ignoring empty directory: ' + dpath)
            # total_empty_dirs += 1
            return
            
        logger.debug('Processing directory: ' + dpath + ' ...')
            
        # Process files and subdirectories and generate file list
        file_list = [] 
        for child in sorted(os.listdir(dpath)):
        
            fpath_child = re.sub('\\\\', '/', os.path.join(dpath, child))
            
            # Process subdirectories
            if os.path.isdir(fpath_child):
                if args.recursive:
                
                    # Check exclusions
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
                continue;
                
            # Check file 
            if not os.path.isfile(fpath_child):
                logger.warning('Not file or directory: ' + fpath_child)
                continue
            
            # Check inclusions
            if args.include is not None and len(args.include):
                include = False
                for re_include in args.include:
                    if re.search(re_include, fpath_child):
                        include = True
                        break
                if not include:
                    logger.debug('Ignoring file not included by REGEX: ' + fpath_child + ' ...')
                    continue
            
            # TODO Check exclusions
            
            # Add valid file to file list
            file_list.append(fpath_child)
        
        # Check if directory contains lastprocessed file
        process_files = True
        lproc = 0
        fpath_lproc = re.sub('\\\\', '/', os.path.join(dpath, '.lastprocessed'))
        if os.path.isfile(fpath_lproc) and len(file_list) > 0 and not (hasattr(args, 'force') and args.force):
        
            process_files = False
        
            try: 
            
                # Read file lastprocessed file
                try: 
                    with open(fpath_lproc, 'r') as file:
                        lproc = float(file.read())
                except:
                    raise Exception('Failed to read file: ' + fpath_lproc)
                    
                # Check if any files are newer
                for fpath_child in file_list:
                    if os.path.getmtime(fpath_child) > lproc:
                        process_files = True
                        break
            
            except:
                errstr = 'Failed to process lastprocessed file: ' + fpath_lproc
                
                if hasattr(args, 'halt') and args.halt:
                    raise Exception(errstr)
                logger.error(errstr)
                
                process_files = True
                    
        if not process_files:
            logger.debug('Ignoring directory with no new files: ' + dpath)
            return
        
        # Process files
        lproc = 0
        for file in file_list:
            
            try: 
                
                # Process the file:
                new_lproc = ProcessJsonListingFile(file)
                
                if new_lproc is not None and new_lproc > lproc:
                    lproc = new_lproc

            except:
            
                errstr = 'Failed to process file: ' + file
                
                if hasattr(args, 'halt') and args.halt:
                    raise Exception(errstr)
                logger.error(errstr)
            
        # Queue the lastprocessed file; it is written once the aggregate and plot files have been
        # written, so a failed or interrupted run is retried on the next run.
        if lproc > 0:
            lastprocessed_pending.append((fpath_lproc, lproc))
        
    except:
        
        # raise Exception('Failed to process directory: '  + dpath)
        
        errstr = 'Failed to process directory: '  + dpath
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
        
    logger.debug('Processed directory: ' + dpath + ' ')

def WriteLastProcessedFiles():

    for fpath_lproc, lproc in lastprocessed_pending:

        try:
            with open(fpath_lproc, 'w') as file:
                file.write(str(lproc))
        except:

            errstr = 'Failed write lastprocessed file: ' + fpath_lproc

            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)

def ProcessJsonListingFile(fpath):
    
    global float_map
    
    dive_data = Dive()
    
    
    fpath = re.sub('\\\\', '/', fpath)
    
    try: 
        
        # Check file:
        if not os.path.exists(fpath) or not os.path.isfile(fpath):
            logger.error('Invalid path: ' + fpath)
            return None
        
        # Check inclusions:
        if args.include is not None and len(args.include):
            include = False
            for re_include in args.include:
                if re.search(re_include, fpath):
                    include = True
                    break
            if not include:
                logger.debug('Ignoring file not included by REGEX: ' + fpath + ' ...')
                return None
            
        # TODO Check exclusions:
            
        logger.debug('Processsing file: ' + fpath + ' ...')
        
        # TODO Get file properies
        # dive_data.list_file.path = re.sub('.*/alamo/', '', fpath)
        # dive_data.list_file.mtime.dt_from_epoch(os.path.getmtime(fpath))
        
        # Read file:
            
        try:
            file = open(fpath, 'r')
            dive_json = file.read()
            file.close()
        except:
            raise Exception('Failed to read file:' + fpath)
            
        # Decode file: 
        
        try:     
            dive_data = jsonpickle.decode(dive_json) 
        except:
            raise Exception('Failed do decode file:' + fpath)
            
        # TODO Check JSON version
        if dive_data.json_vers != JSON_VERSION:
            logger.warning('Ignoring file with mismatched JSON version: ' + fpath)
            return None
            
        # Get JSON file info
        dive_data.json_file = File()
        dive_data.json_file.path = fpath
        
        if hasattr(args, 'strip') and len(args.strip):
            dive_data.json_file.path = re.sub(args.strip, '', dive_data.json_file.path)
        
        
        dive_data.list_file.mtime.dt_from_epoch(os.path.getmtime(fpath))
        
        # Save dive data
        
        if not dive_data.id.whoi in float_map:
            float_map[dive_data.id.whoi] = Float(dive_data.id)
        float_map[dive_data.id.whoi].dives.append(dive_data)
        float_map[dive_data.id.whoi].id.dac = dive_data.id.dac
        float_map[dive_data.id.whoi].id.wmo = dive_data.id.wmo
        
        return os.path.getmtime(fpath)
        
    except:
            
        errstr = 'Failed to proces file: ' + fpath
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
        
    return None 
        
def WriteAggregateJsonFiles():
    
    global float_aggregate_map
    
    try:
        
        # Check output directory:
            
        if not hasattr(args, 'output'):
            raise Exception('Output directory not specified.')
        dpath_out = re.sub('\\\\', '/', args.output)
        if not os.path.isdir(dpath_out):
            raise Exception('Output directory not found: ' + dpath_out)
        
        # Iterate over float aggregate data:
        
        for float_id in float_aggregate_map.keys():
            
            logger.info('Writing aggregate files for float {:04d}...'.format(float_id))
            
            float_aggregate_data = float_aggregate_map[float_id]
            
            # Check aggregate data object:
                
            if not 'id' in float_aggregate_data:
                logger.error('Invalid aggregate data object, no \'id\'.')
                continue
            if not hasattr(float_aggregate_data['id'], 'whoi'):
                logger.error('Invalid aggregate data object, no \'id.whoi\'.')
                continue
    
            # Generate path:
                
            dpath_float = os.path.join(dpath_out, '{}'.format(float_aggregate_data['id'].whoi))
            dpath_float = re.sub('\\\\', '/', dpath_float)
            
            if not os.path.isdir(dpath_float):
                if os.path.exists(dpath_float): 
                    raise Exception('Invalid path: \'' + dpath_float + '\'')
                else:
                    logger.warning('Creating directory: \'' + dpath_float + '\'')
                    os.makedirs(dpath_float)
                    
            
            # Write aggregate ascent data:
                
            if not 'ascent' in float_aggregate_data:
                logger.warning('No ascent data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_ascent.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate ascent file: ' + fpath)
                float_aggregate_data['ascent'].to_json(fpath, orient='split', index=False)
            
            # Write aggregate ascent data:
                
            if not 'descent' in float_aggregate_data:
                logger.warning('No descent data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_descent.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate descent file: ' + fpath)
                float_aggregate_data['descent'].to_json(fpath, orient='split', index=False)
                
            # Write drift data file:    
                
            if not 'drift' in float_aggregate_data:
                logger.debug('No drift data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_drift.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate drift file: ' + fpath)
                float_aggregate_data['drift'].to_json(fpath, orient='split', index=False)
                
            # Write engineering data file:    
                
            if not 'engineering' in float_aggregate_data:
                logger.warning('No engineering data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_engineering.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate engineering file: ' + fpath)
                float_aggregate_data['engineering'].to_json(fpath, orient='split', index=False)

            # Write mission parameter, engineering value and engineering error files:

            for key in ('mission', 'engineering_data', 'engineering_errors'):
                if key in float_aggregate_data:
                    fpath = os.path.join(dpath_float, '{}_{}.json'.format(float_id, key))
                    fpath = re.sub('\\\\', '/', fpath)

                    logger.debug('Writing aggregegate {} file: '.format(key) + fpath)
                    float_aggregate_data[key].to_json(fpath, orient='split', index=False)

            # Write gps data file:    
                
            if not 'gps' in float_aggregate_data:
                logger.warning('No gps data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_gps.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate gps file: ' + fpath)
                float_aggregate_data['gps'].to_json(fpath, orient='split', index=False)

            # Write aggregate profile file:
            
            if not 'profile' in float_aggregate_data:
                logger.warning('No profile data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_profile.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate profile file: ' + fpath)
                float_aggregate_data['profile'].to_json(fpath, orient='split', index=False)
 
            # Write profile positions file:
            
            if not 'profile_position' in float_aggregate_data:
                logger.warning('No profile data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_profile_position.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate profile position file: ' + fpath)
                float_aggregate_data['profile_position'].to_json(fpath, orient='split', index=False)

            # Write pump file:
            
            if not 'pump' in float_aggregate_data:
                logger.warning('No pump data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_pump.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate pump file: ' + fpath)
                float_aggregate_data['pump'].to_json(fpath, orient='split', index=False)

                
            # Write aggregate SBD file
            
            if not 'sbd' in float_aggregate_data:
                logger.warning('No SBD data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_sbd.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate SBD file: ' + fpath)
                float_aggregate_data['sbd'].to_json(fpath, orient='split', index=False)             
                
            # Write aggregate surfa file
            
            if not 'surface' in float_aggregate_data:
                logger.warning('No surface data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_surface.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate surface file: ' + fpath)
                float_aggregate_data['surface'].to_json(fpath, orient='split', index=False)
                
            # Write wave file
            
            if not 'wave' in float_aggregate_data:
                logger.debug('No wave data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_wave.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate wave file: ' + fpath)
                float_aggregate_data['wave'].to_json(fpath, orient='split', index=False)

            # Write wave spectra file
            
            if not 'wave_spectra' in float_aggregate_data:
                logger.debug('No wave spectra data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_wave_spectra.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate wave spectra file: ' + fpath)
                float_aggregate_data['wave_spectra'].to_json(fpath, orient='split', index=False)
    
            # Write aggregate Iridium positions file
            
            if not 'iridium_position' in float_aggregate_data:
                logger.warning('No iridium position data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_iridium_position.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing aggregegate iridium positions file: ' + fpath)
                float_aggregate_data['iridium_position'].to_json(fpath, orient='split', index=False)
 
            # Write submerged displacement file
            
            if not 'displacement_submerged' in float_aggregate_data:
                logger.warning('No submerged displacement data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_displacement_submerged.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing submerged displacement file: ' + fpath)
                float_aggregate_data['displacement_submerged'].to_json(fpath, orient='split', index=False)
 
            # Write surface displacement file
            
            if not 'displacement_surface' in float_aggregate_data:
                logger.warning('No surface displacement data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_displacement_surface.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing surface displacement file: ' + fpath)
                float_aggregate_data['displacement_surface'].to_json(fpath, orient='split', index=False)
 
            # Write cycle timing file
            
            if not 'cycle_timing' in float_aggregate_data:
                logger.warning('No cycle timing data for float {}'.format(float_id))
            else:
                fpath = os.path.join(dpath_float, '{}_cycle_timing.json'.format(float_id))
                fpath = re.sub('\\\\', '/', fpath)
                
                logger.debug('Writing cycle timing file: ' + fpath)
                float_aggregate_data['cycle_timing'].to_json(fpath, orient='split', index=False)
                
            # Write summary file
            
            if not 'summary' in float_aggregate_data:
                logger.warning('No cycle summary data for float {}'.format(float_id))
            else:
                try: 
                
                    fpath = os.path.join(dpath_float, '{}_summary.json'.format(float_id))
                    fpath = re.sub('\\\\', '/', fpath)
                    
                    with open(fpath, 'w') as file:
                        dive_json = jsonpickle.encode(float_aggregate_data['summary'], unpicklable=True, indent=2,  separators=(',', ':'))
                        # dive_json = jsonpickle.encode(dive_data, unpicklable=True, separators=(',', ':'))
                        file.write(dive_json)
                        
                    
                except:
                   
                    errstr = 'Failed to write summary file: ' + fpath
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
 
    except:
        raise Exception('Failed to write aggregate JSON files.')
 
def WriteHtmlFile(float_id):

    global float_aggregate_map

    try:
    
        # Generate path
    
        if not hasattr(args, 'plot_output'):
            raise Exception('Plot output directory not specified.')
        dpath_float = re.sub('\\\\', '/', os.path.join(args.plot_output, '{}'.format(float_id)))
        fpath_html = re.sub('\\\\', '/', os.path.join(dpath_float, 'index.html'))
    
        # Check aggregate data object
        
        if not float_id in float_aggregate_map.keys():
            raise Exception('Aggregate data for float {} not found.'.format(float_id))
        float_aggregate_data = float_aggregate_map[float_id]
        if not 'figures' in float_aggregate_data.keys():
            raise Exception('Figure metadata data for float {} not found.'.format(float_id))
        figs_meta_dict = float_aggregate_data['figures']
    
        # Create document
        
        doc, tag, text = Doc().tagtext()
        
        doc.asis('<!DOCTYPE html>')
        with tag('html'):
        
            # Create head
            
            with tag('head'):
            
                with tag('title'):
                    text('{}'.format(float_id))
                    
                # Create styles
                    
                with(tag('style')):
                
                    # Create styles - general
                
                    text('a { color: #2196F3; text-decoration: none; } ')
                    text('a:hover {color: #064579;} ')
                    text('body { font-family: "Open Sans", verdana, arial, sans-serif; margin: 5px; } ')
                    
                    # Create styles - headings
                    
                    text('h2 { font-size: 24px; } ')
                    
                    # Create styles - tables
                    
                    text('th { text-align: left; } ')
                    
                    # Create styles - navigation links
                    
                    text('.nav { width: 125px; position: fixed; z-index: 1; top: 10px; left: 10px; overflow-x: hidden; background: #f9f9f9; padding: 2px 0 6px 0; line-height: 1.4; border: 1px solid #dcdcdc; border-radius: 5px; } ')
                    text('.nav a { font-size: 12px; padding: 5px 8px; } ')

                    # Create styles - plot divs

                    text('.plots-top-div { margin-left: 130px; width: 85%; } ')
                    text('.plots-div {{ margin: auto; max-width: {}px; }} '.format(opts['figWide']['small_width']+25))
                    
                    # Create styles - figures
                    
                    
                    text('.fig, .fig2 { max-width: 50%; padding-top: 10px; margin: auto; } ')
                    text('.figWide, .figWideL, .figWideXL, .figWideXXL, .figWideS, .figWideXS { max-width: 100%; padding-top: 10px; margin: auto; } ')
                    
                    # Create styles - figures table
                    
                    text('.figt { font-size: 11px; margin: auto; } ')
                    text('.figt tbody td { height: 110px; padding: 5px; vertical-align: top; max-width: 220px; } ')
                    text('.figt tbody td img {{ width: {}px; text-align: center; }} '.format(opts['figThumb']['width']))
                    
                    # Create styles - footer
                    
                    text('.footer { font-size: 12px; color: #aaa; padding: 40px; } ')

            # Create body

            with tag('body'):
            
                # Create navigation pane
            
                with tag('div', klass="nav"):
                    
                    with tag('a', href='#top'):
                        doc.asis('&#8679 Top')
                    doc.stag('br')
                    with tag('a', href='#prof'):
                        text('Profile Data')
                    doc.stag('br') 
                    if 'wave' in float_aggregate_data and len(float_aggregate_data['wave']) > 0:
                        with tag('a', href='#wave'):
                            text('Wave Data')
                        doc.stag('br') 
                    with tag('a', href='#drift'):
                        text('Drift Data')
                    doc.stag('br') 
                    with tag('a', href='#surf'):
                        text('Surface Data')
                    doc.stag('br') 
                    with tag('a', href='#ascent'):
                        text('Ascent')
                    doc.stag('br') 
                    with tag('a', href='#descent'):
                        text('Descent')
                    doc.stag('br') 
                    with tag('a', href='#pump'):
                        text('Hydraulic System')
                    doc.stag('br') 
                    with tag('a', href='#sat'):
                        text('Satcom')
                    doc.stag('br') 
                    with tag('a', href='#figs'):
                        text('Figure Files')
                    doc.stag('br')
                    with tag('a', href='/alamo/'):
                        doc.asis('&#8678 Back')
                    
            
                # Create main plots div
            
                with tag('div', id='top', klass="plots-top-div"):
                    with tag('div', klass="plots-div"):
                    
                        # General plots
                    
                        count = 0
                        for id in [
                            'position_globe', 'position', '', 
                            'displacement_submerged_velocity', 'displacement_surface_velocity', '', 
                            'trajectory', '', 
                            'cycle_timing', '', 
                            ]:
                            
                            if (len(id) == 0) and (count > 0) :
                            
                                doc.stag('br')
                                count = 0
                                continue
                       
                            if id in figs_meta_dict:
                                fig_meta = figs_meta_dict[id]
                                with tag('a', href=fig_meta['fpath_html']):
                                    doc.stag(
                                        'img', 
                                        alt=fig_meta['title'],
                                        klass=fig_meta['size'], 
                                        title=fig_meta['title'],
                                        src=fig_meta['fpath_img_small'])
                                count = count + 1
                               

                        
                        # Profile plots
                        
                        with tag('h2', id='prof'):
                            text('Profile Data')
                            
                        if not 'profile' in float_aggregate_data or len(float_aggregate_data['profile']) < 1:
                            with tag('p'):
                                text('No profile data.')
                            doc.stag('br')
                        else:
                        
                            count = 0
                            for id in [
                                'profile_temp', 'profile_saln', '', 
                                'profile_ts', 'profile_sigma0', '', 
                                'profile_contour_temp', '', 
                                'profile_contour_saln', '', 
                                'profile_contour_sigma0', '', 
                                'profile_num_bins', '', 
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(
                                            'img',  
                                            alt=fig_meta['title'],
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1
                                    
                        # Wave data plots
                        
                        if 'wave' in float_aggregate_data and len(float_aggregate_data['wave']) > 0:

                            with tag('h2', id='wave'):
                                text('Wave Data')
                                
                            count = 0
                            for id in [
                                'wave_mwm', '', 
                                'wave_swh', '', 
                                'wave_period', '', 
                                'wave_spectra_disp_Szz', '', 
                                'wave_waterfall_disp_Szz', '', 
                                'wave_direction_mean', '', 
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(
                                            'img',  
                                            alt=fig_meta['title'],
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1

                        # Drift plots

                        with tag('h2', id='drift'):
                            text('Drift Data')
                        
                        if not 'drift' in float_aggregate_data or len(float_aggregate_data['drift']) < 1:
                            with tag('p'):
                                text('No drift data.')
                            doc.stag('br')
                        else:    
                        
                            count = 0
                            for id in [
                                'drift_pres', 'drift_pres_avg', '', 
                                'drift_temp', 'drift_temp_avg', '', 
                                'drift_saln', 'drift_saln_avg', '', 
                                'drift_dens', 'drift_dens_avg', '', 
                                'drift_sample_count', '', 
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(
                                            'img',  
                                            alt=fig_meta['title'], 
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1
                            
                                                    
                        # surface plots
                        
                        with tag('h2', id='surf'):
                            text('Surface Data')
                            
                        if not 'surface' in float_aggregate_data or len(float_aggregate_data['surface']) < 1:
                            with tag('p'):
                                text('No surface data.')
                            doc.stag('br')
                        else:    
                        
                            count = 0
                            for id in [
                                'surface_pres', 'surface_pres_avg', '', 
                                'surface_temp', 'surface_temp_avg', '', 
                                'surface_sample_count', '', 
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(  
                                            'img', 
                                            alt=fig_meta['title'],
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1
                        
                        # ascent plots
                        
                        with tag('h2', id='ascent'):
                            text('Ascent')
                            
                        if not 'ascent' in float_aggregate_data or len(float_aggregate_data['ascent']) < 1:
                            with tag('p'):
                                text('No ascent data.')
                            doc.stag('br')
                        else: 
                        
                            count = 0
                            for id in [
                                'ascent', 'ascent_rate_v_pres', '', 
                                'ascent_rate_v_time', 'ascent_rate_avg', '', 
                                'ascent_sample_count', '', 
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(
                                            'img',   
                                            alt=fig_meta['title'],
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1
                        
                        # descent plots
                        
                        with tag('h2', id='descent'):
                            text('Descent')
                            
                        if not 'descent' in float_aggregate_data or len(float_aggregate_data['descent']) < 1:
                            with tag('p'):
                                text('No descent data.')
                            doc.stag('br')
                        else: 
                        
                            count = 0
                            for id in [
                                'descent', 'descent_rate_v_pres', '', 
                                'descent_rate_v_time', 'descent_rate_avg', '', 
                                'descent_sample_count', '', 
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(
                                            'img',   
                                            alt=fig_meta['title'],
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1
                        
                        # pump plots
                        
                        with tag('h2', id='pump'):
                            text('Hydraulic System')
                            
                        if not 'pump' in float_aggregate_data or len(float_aggregate_data['pump']) < 1:
                            with tag('p'):
                                text('No hydraulic system data.')
                            doc.stag('br')
                        else:      
                        
                        
                            count = 0
                            for id in [
                                'pump_pres', 'pump_power_v_pres', '', 
                                'pump_volt', 'pump_volt_avg', '', 
                                'pump_curr', 'pump_curr_avg', '', 
                                # 'pump_power', 'pump_power_avg', '', 
                                'pump_energy', 'pump_energy_v_max_pres', '',
                                'pump_num_ops', '', 
                                'pump_time_sum', '',                                    
                                'pump_energy_sum', '',
                                'pump_vac', 'pump_vac_avg', '', 
                                'pump_vac_v_time', 'pump_vac_v_pres', '',                                     
                                ]:
                                
                                if (len(id) == 0) and (count > 0) :
                                
                                    doc.stag('br')
                                    count = 0
                                    continue
                           
                                if id in figs_meta_dict:
                                    fig_meta = figs_meta_dict[id]
                                    with tag('a', href=fig_meta['fpath_html']):
                                        doc.stag(
                                            'img',   
                                            alt=fig_meta['title'],
                                            klass=fig_meta['size'], 
                                            title=fig_meta['title'],
                                            # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                            src=fig_meta['fpath_img_small'])
                                    count = count + 1                            
                        
                        # satcom plots
                        
                        with tag('h2', id='sat'):
                            text('Satcom')
                            
                                                        
                        count = 0
                        for id in [
                            'satcom', '', 
                            'position_gps', 'satcom_gps_cno', '', 
                            'satcom_gps_hdop', 'satcom_gps_nsat', '', 
                            'satcom_gps_ttf', 'satcom_gps_fix_cep', '', 
                            'satcom_iridium_fix_cep_avg', 'satcom_sbd_data_size_avg', '', 
                            'satcom_sbd_count', '', 
                            'satcom_sbd_data_size', '',
                            ]:
                            
                            if (len(id) == 0) and (count > 0) :
                            
                                doc.stag('br')
                                count = 0
                                continue
                       
                            if id in figs_meta_dict:
                                fig_meta = figs_meta_dict[id]
                                with tag('a', href=fig_meta['fpath_html']):
                                    doc.stag(
                                        'img',   
                                        alt=fig_meta['title'],
                                        klass=fig_meta['size'], 
                                        title=fig_meta['title'],
                                        # src=fig_meta['fpath_img_small'] if fig_meta['size_img_small'] < fig_meta['size_svg_small'] else fig_meta['fpath_svg_small'])
                                        src=fig_meta['fpath_img_small'])
                                count = count + 1    

                        # TODO Generate table of data files
                        

                        #  Generate table of all plots
                        
                        with tag('h2', id='figs'):
                            text('Figure Files')
                        
                        with tag('table', klass='figt'):
                        
                            with tag('thead'):
                                with tag('tr'):
                                    with tag('th'):
                                        text(' ')
                                    with tag('th'):
                                        text('Title')
                                    with tag('th'):
                                        text('Category')
                                    with tag('th'):
                                        text(' ')
                                    with tag('th'):
                                        text('Files')
                                
                            with tag('tbody'):
                                i=0
                                for key in sorted(figs_meta_dict.keys()):
                                    fig_meta = figs_meta_dict[key]
                                    i=i+1
                                    
                                    with tag('tr'):
                                    
                                        with tag('td'): # no.
                                            text('{}'.format(i))
                                        
                                        with tag('td'): # title
                                            # text(re.sub('.*: ', '',fig_meta['title']))
                                            doc.asis(fig_meta['title'])
                                        
                                        with tag('td'): # category
                                            text(fig_meta['category'])
                                            
                                        
                                        with tag('td'): # image
                                            with tag('div'):
                                                with tag('a', href=fig_meta['fpath_html']):
                                                    doc.stag(
                                                        'img', 
                                                        alt=fig_meta['title'],
                                                        klass='itiny', 
                                                        src=fig_meta['fpath_img_thumb'])
                                        
                                        with tag('td'): # files
                                            text(key)
                                            
                                            doc.stag('br')
                                            # with tag('a', href=fig_meta['fpath_json']):
                                                # text('.json')
                                            # text(' ({:.1f}kB)'.format(fig_meta['size_json']/1024.))
                                            # doc.stag('br')
                                            with tag('a', href=fig_meta['fpath_html']):
                                                text('.html')
                                            text(' ({:.1f}kB)'.format(fig_meta['size_html']/1024.))
                                            doc.stag('br')                                       
                                            
                                            with tag('a', href=fig_meta['fpath_img_small']):
                                                text('.png [{}x{}]'.format(opts[fig_meta['size']]['small_width'], opts[fig_meta['size']]['small_height']))
                                            text(' ({:.1f}kB)'.format(fig_meta['size_img_small']/1024.))
                                            doc.stag('br')
                                            
                                            with tag('a', href=fig_meta['fpath_img']):
                                                text('.png [{}x{}]'.format(opts[fig_meta['size']]['width'], opts[fig_meta['size']]['height']))
                                            text(' ({:.1f}kB)'.format(fig_meta['size_img']/1024.))
                                            doc.stag('br')
                                            
                                            # with tag('a', href=fig_meta['fpath_svg_small']):
                                            #     text('.svg [{}x{}]'.format(opts[fig_meta['size']]['small_width'], opts[fig_meta['size']]['small_height']))
                                            # text(' ({:.1f}kB)'.format(fig_meta['size_svg_small']/1024.))
                                            # doc.stag('br')
                                            
                                            # with tag('a', href=fig_meta['fpath_svg']):
                                            #     text('.svg [{}x{}]'.format(opts[fig_meta['size']]['width'], opts[fig_meta['size']]['height']))
                                            # text(' ({:.1f}kB)'.format(fig_meta['size_svg']/1024.))
                                        
 
                # Create footer
                
                with tag('div', klass='footer'):        
                    with tag('i'):
                        text('Page last updated ' + datetime.datetime.now(datetime.UTC).isoformat()[:16] + 'Z' + ' - version ' + JSON_VERSION)
                        # text('Page last updated ' + datetime.datetime.now(datetime.UTC).isoformat()[:16] + 'Z' + ' - v' + JSON_VERSION + ' - Float Group, Department of Physical Oceanography, Woods Hole Oceanographic Institution, Woods Hole, MA, USA')
                        
        
        
        
        
        # write file
        
        with open(fpath_html, 'w', encoding='utf-8') as fout:
            # fout.write(indent(
            #     doc.getvalue(),
            #     indentation = '  ',
            #     newline = '\n',
            #     indent_text = True
            #     ))
            fout.write(doc.getvalue())
    
    except:
        errstr = 'Failed to write HTML file: ' + fpath_html
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)

def WriteGeoPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the geographic position plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'profile_position' in float_aggregate_data or len(float_aggregate_data['profile_position']) < 1:
        logger.warning('No position data for float {}'.format(float_id))
    else:
    
        try: 


            # Create color range
            
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            
            # Preprocess data
            
            df_profiles = float_aggregate_data['profile_position']
            df_profiles = df_profiles[df_profiles['dive'] > 0] # TODO fix
            df_profiles = df_profiles[(~df_profiles['lat'].isna()) & (~df_profiles['lon'].isna())]
            
            # 
            
            df_gps = float_aggregate_data['gps']
            df_gps_pre = df_gps[df_gps['dive'] < 0]
            df_iridium = float_aggregate_data['iridium_position']
            df_sbd = float_aggregate_data['sbd']
                                    
            if len(df_profiles) < 1:
                logger.warning('No position data for float {}'.format(float_id))
            
            # Calculate map ranges
            
            # TODO handle dateline crossing
            
            rng = dict(
                lat = np.percentile(df_profiles['lat'].values, [0,100]).astype(np.double),
                lon = np.percentile(df_profiles['lon'].values, [0,100]).astype(np.double)
                )
            
            # Check ranges 
            
            position_span = (rng['lat'][1] - rng['lat'][0], rng['lon'][1] - rng['lon'][0])
            position_mid = ((rng['lat'][1] + rng['lat'][0])/2, (rng['lon'][1] + rng['lon'][0])/2)
            position_mean = np.round(
                np.angle(
                    np.sum(np.exp(1j * np.array(df_profiles[['lat','lon']].values).astype(np.double) * np.pi/180.0),0) / len(df_profiles),
                    deg=True
                    ),
                0)
            if position_mean[0] > 27 and position_mean[0] < 29:
                position_mean[0] = 28.5
            
            # position_mean = np.array([
                # np.angle(1.0 / len(df_profiles) * np.sum(np.exp(1j * df_profiles['lat'].values * np.pi/180.0)), deg=True),
                # np.angle(1.0 / len(df_profiles) * np.sum(np.exp(1j * df_profiles['lon'].values * np.pi/180.0)), deg=True),
                # ])
            # position_mean = np.round(position_mean, 0)
            
            if position_span[1] > 1:
            
                # Check aspect ratio    
            
                position_aratio = position_span[0]/position_span[1]
            
                if position_aratio > 0.85:
                    
                    # Correct too-tall aspect ratio to ~ 3:4
                    
                    rng['lon'] = np.array([ position_mid[1], position_mid[1] ]) \
                        + np.array([ -position_span[0], position_span[0] ]) * 2 / 3
                        
                elif position_aratio < 0.65:
                    
                    # Correct too-wide aspect ratio to ~ 3:4
                    
                    rng['lat'] = np.array([ position_mid[0], position_mid[0] ]) \
                        + np.array([ -position_span[1], position_span[1] ]) * 3 / 8
                         
                rng['lat'] = np.round(rng['lat'] + [-0.5, 0.5], 1)
                rng['lon'] = np.round(rng['lon'] + [-0.5, 0.5], 1)  
                    
            else:
                
                # Set minimum range if span is too small
                
                rng['lat'] = np.round(np.array([ position_mid[0], position_mid[0] ]) + [-3.0, 3.0], 1)
                rng['lon'] = np.round(np.array([ position_mid[1], position_mid[1] ]) + [-4.0, 4.0], 1)

            # Generate figures and add trajectory trace
            
            figs=dict(
                position_globe = go.Figure(
                    go.Scattergeo(
                        lat = df_profiles['lat'],
                        lon = df_profiles['lon'],
                        hoverinfo='none',
                        line=dict(
                            color='rgb(0, 0, 0)',
                            width=1,
                            ),
                        mode = 'lines',
                        name = 'Trajectory',
                        ),
                    layout=dict(
                        title=dict(text='{}: Profile Positions'.format(float_id)),
                        showlegend = False,
                        geo = dict(
                            showland = True,
                            showcountries = False,
                            showocean = False,
                            coastlinewidth = 0.5,
                            coastlinecolor = 'rgb(188, 188, 188)',
                            landcolor = 'rgb(220, 220, 220)',
                            # lakecolor = 'rgb(255, 255, 255)',
                            # oceancolor = 'rgb(255, 255, 255)',
                            fitbounds = False,
                            projection = dict(
                                type = 'orthographic',
                                # type = 'satellite',
                                rotation = dict(
                                    lat = position_mean[0],
                                    lon = position_mean[1],
                                    roll = 0,
                                    ),
                                scale = 1.15,
                                ),
                            lataxis = dict(
                                showgrid = True,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                dtick = 15
                                ),
                            lonaxis = dict(
                                showgrid = True,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                dtick = 30
                                ),
                            resolution = 50,
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        ),
                    ),
                position = go.Figure(
                    go.Scattergeo(
                        lat = df_profiles['lat'],
                        lon = df_profiles['lon'],
                        hoverinfo='none',
                        line=dict(
                            color='rgb(0, 0, 0)',
                            width=1,
                            ),
                        mode = 'lines',
                        name = 'Trajectory',
                        ),
                    layout=dict(
                        title=dict(text='{}: Profile Positions'.format(float_id)),
                        showlegend = False,
                        geo = dict(
                            showland = True,
                            showcountries = False,
                            showocean = False,
                            coastlinewidth = 0.5,
                            coastlinecolor = 'rgb(188, 188, 188)',
                            landcolor = 'rgb(220, 220, 220)',
                            # lakecolor = 'rgb(255, 255, 255)',
                            # oceancolor = 'rgb(255, 255, 255)',
                            projection = dict(
                                type = 'winkel tripel',
                                # type = 'satellite',
                                # rotation = dict(
                                #     lat = position_mean[0],
                                #     lon = position_mean[1],
                                #     roll = 0,
                                #     ),
                                # scale = 1,
                                ),
                            fitbounds = False,
                            lataxis = dict(
                                showgrid = True,
                                dtick = 2.5,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                range = rng['lat'],
                                ),
                            lonaxis = dict(
                                showgrid = True,
                                dtick = 5,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                range = rng['lon'],
                                ),
                            resolution = 50,
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        ),
                    ),
                position_all = go.Figure(
                    go.Scattergeo(
                        lat = df_gps['lat'],
                        lon = df_gps['lon'],
                        mode = 'lines',
                        hoverinfo='none',
                        line=dict(
                            color='rgb(0, 0, 0)',
                            width=1,
                            ),
                        showlegend = False,
                        ),
                    layout=dict(
                        title=dict(text='{}: All Positions'.format(float_id)),
                        showlegend = True,
                        geo = dict(
                            showland = True,
                            showcountries = False,
                            showocean = False,
                            coastlinewidth = 0.5,
                            coastlinecolor = 'rgb(188, 188, 188)',
                            landcolor = 'rgb(220, 220, 220)',
                            projection = dict(
                                type = 'winkel tripel',
                                ),
                            fitbounds = False,
                            lataxis = dict(
                                showgrid = True,
                                dtick = 2.5,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                range = rng['lat'],
                                ),
                            lonaxis = dict(
                                showgrid = True,
                                dtick = 5,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                range = rng['lon'],
                                ),
                            resolution = 50,
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(    
                            font=dict(
                                size=10,
                                ),
                            groupclick="toggleitem",
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        ),
                    ),
                position_gps = go.Figure(
                    go.Scattergeo(
                        lat = df_gps['lat'],
                        lon = df_gps['lon'],
                        mode = 'lines',
                        hoverinfo='none',
                        line=dict(
                            color='rgb(0, 0, 0)',
                            width=1,
                            ),
                        showlegend = False,
                        ),
                    layout=dict(
                        title=dict(text='{}: GPS Positions'.format(float_id)),
                        showlegend = True,
                        geo = dict(
                            showland = True,
                            showcountries = False,
                            showocean = False,
                            coastlinewidth = 0.5,
                            coastlinecolor = 'rgb(188, 188, 188)',
                            landcolor = 'rgb(220, 220, 220)',
                            projection = dict(
                                type = 'winkel tripel',
                                ),
                            fitbounds = False,
                            lataxis = dict(
                                showgrid = True,
                                dtick = 2.5,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                range = rng['lat'],
                                ),
                            lonaxis = dict(
                                showgrid = True,
                                dtick = 5,
                                gridcolor = 'rgb(123, 123, 123)',
                                gridwidth = 0.5,
                                range = rng['lon'],
                                ),
                            resolution = 50,
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(    
                            font=dict(
                                size=10,
                                ),
                            groupclick="toggleitem",
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        ),
                    ),
                )
            
            # Add map details
            
            figs['position_globe'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((91), 23.436), 
                        np.nan, 
                        np.full((91), -23.436),
                        np.nan,
                        np.full((91), 66.564), 
                        np.nan, 
                        np.full((91), -66.564),
                        )),
                    lon = np.hstack((
                        np.linspace(-180, 180, 91), 
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123, 123, 123)',
                        dash='dash',
                        width=0.5,
                        )
                    ),
                )
            figs['position'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((91), 23.436), 
                        np.nan, 
                        np.full((91), -23.436),
                        np.nan,
                        np.full((91), 66.564), 
                        np.nan, 
                        np.full((91), -66.564),
                        )),
                    lon = np.hstack((
                        np.linspace(-180, 180, 91), 
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123, 123, 123)',
                        dash='dash',
                        width=0.5,
                        )
                    ),
                )
            figs['position_all'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((91), 23.436), 
                        np.nan, 
                        np.full((91), -23.436),
                        np.nan,
                        np.full((91), 66.564), 
                        np.nan, 
                        np.full((91), -66.564),
                        )),
                    lon = np.hstack((
                        np.linspace(-180, 180, 91), 
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123, 123, 123)',
                        dash='dash',
                        width=0.5,
                        ),
                    showlegend = False,
                    ),
                )
            figs['position_gps'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((91), 23.436), 
                        np.nan, 
                        np.full((91), -23.436),
                        np.nan,
                        np.full((91), 66.564), 
                        np.nan, 
                        np.full((91), -66.564),
                        )),
                    lon = np.hstack((
                        np.linspace(-180, 180, 91), 
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        np.nan, 
                        np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123, 123, 123)',
                        dash='dash',
                        width=0.5,
                        ),
                    showlegend = False,
                    ),
                )
            figs['position_globe'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((4), 0), 
                        # np.array([90, 0, -90, 0])
                        # np.full((91), -23.43639),
                        # np.nan,
                        # np.full((91), 66.56361), 
                        # np.nan, 
                        # np.full((91), -66.56361),
                        )),
                    lon = np.hstack((
                        np.array([0, 1, 2, 0]) * 120, 
                        # np.array([0,180,0,0])
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(88, 88, 88)',
                        # dash='dash',
                        width=0.6,
                        )
                    ),
                )
            figs['position'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((4), 0), 
                        # np.array([90, 0, -90, 0])
                        # np.full((91), -23.43639),
                        # np.nan,
                        # np.full((91), 66.56361), 
                        # np.nan, 
                        # np.full((91), -66.56361),
                        )),
                    lon = np.hstack((
                        np.array([0, 1, 2, 0]) * 120, 
                        # np.array([0,180,0,0])
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(88, 88, 88)',
                        # dash='dash',
                        width=0.6,
                        )
                    ),
                )
            figs['position_all'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((4), 0), 
                        # np.array([90, 0, -90, 0])
                        # np.full((91), -23.43639),
                        # np.nan,
                        # np.full((91), 66.56361), 
                        # np.nan, 
                        # np.full((91), -66.56361),
                        )),
                    lon = np.hstack((
                        np.array([0, 1, 2, 0]) * 120, 
                        # np.array([0,180,0,0])
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(88, 88, 88)',
                        # dash='dash',
                        width=0.6,
                        ),
                    showlegend = False,
                    ),
                )
                
            figs['position_gps'].add_trace(
                go.Scattergeo(
                    lat = np.hstack((
                        np.full((4), 0), 
                        # np.array([90, 0, -90, 0])
                        # np.full((91), -23.43639),
                        # np.nan,
                        # np.full((91), 66.56361), 
                        # np.nan, 
                        # np.full((91), -66.56361),
                        )),
                    lon = np.hstack((
                        np.array([0, 1, 2, 0]) * 120, 
                        # np.array([0,180,0,0])
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        # np.nan, 
                        # np.linspace(-180, 180, 91),
                        )),
                    mode = 'lines',
                    hoverinfo='none',
                    line=dict(
                        color='rgb(88, 88, 88)',
                        # dash='dash',
                        width=0.6,
                        ),
                    showlegend = False,
                    ),
                )
            
            
            # Add markers to figures -- predeployment
            
            for dive in df_gps_pre['dive'].values:
                idx = df_gps_pre['dive'] == dive
                figs['position_gps'].add_trace(go.Scattergeo(
                            lat = df_gps_pre['lat'][idx],
                            lon = df_gps_pre['lon'][idx],
                            # customdata=np.stack(
                                # (   df_gps_pre['dt_str'].values, 
                                    # df_gps_pre['cep_km'].values
                                # ), 
                                # -1),
                            hovertemplate = '<br>'.join([
                                'Startup GPS',
                                df_gps_pre[idx]['dt_str'].values[0],
                                '%{lat:.4f}°N %{lon:.4f}°E',
                                'CEP = {:.3f} km'.format(df_gps_pre[idx]['cep_km'].values[0]),
                                '<extra></extra>'
                                ]),
                            # hoverinfo = None,
                            legendgroup='Startup {}'.format(dive),
                            legendgrouptitle=dict(
                                text='Startup {}'.format(dive),
                                ),
                            opacity = 0.7,
                            marker = dict(
                                color='rgb(0, 0, 0)',
                                line=dict(
                                    color='rgb(0, 0, 0)',
                                    width = 0.5,
                                    ),
                                size=9, 
                                symbol='star',
                                ),
                            mode = 'markers',
                            name = 'Startup GPS',
                            ))
                figs['position_all'].add_trace(go.Scattergeo(
                            lat = df_gps_pre['lat'][idx],
                            lon = df_gps_pre['lon'][idx],
                            # customdata=np.stack(
                                # (   df_gps_pre['dt_str'].values, 
                                    # df_gps_pre['cep_km'].values
                                # ), 
                                # -1),
                            hovertemplate = '<br>'.join([
                                'Startup GPS',
                                df_gps_pre[idx]['dt_str'].values[0],
                                '%{lat:.4f}°N %{lon:.4f}°E',
                                'CEP = {:.3f} km'.format(df_gps_pre[idx]['cep_km'].values[0]),
                                '<extra></extra>'
                                ]),
                            # hoverinfo = None,
                            legendgroup='{}'.format(dive),
                            legendgrouptitle=dict(
                                text='Startup <b>{}</b>'.format(dive),
                                ),
                            opacity = 0.7,
                            marker = dict(
                                color='rgb(0, 0, 0)',
                                line=dict(
                                    color='rgb(0, 0, 0)',
                                    width = 0.5,
                                    ),
                                size=9, 
                                symbol='star',
                                ),
                            mode = 'markers',
                            name = 'Startup GPS',
                            ))
            
            # Add markers to figures -- all positions legend groups
            
            
            for dive in dives:
            
                try: 
                    if(dive < 0):
                        continue
                    
                    legendgroup=str(dive)
                        
                    figs['position_all'].add_trace(go.Scattergeo(
                        lat = [0],
                        lon = [0],
                        # hovertemplate = hovertemplate,
                        hoverinfo= 'none',
                        legendgroup=legendgroup,
                        legendgrouptitle=dict(text='Dive <b>{}</b>'.format(dive)),
                        showlegend = True,
                        opacity = 1,
                        marker = dict(
                            color='rgba(255,255,255,0)',
                            line=dict(
                                color='rgba(255,255,255,0)',
                                ),
                            ),
                        mode = 'markers',
                        name = '',
                        ))
                        
                except: 
                    errstr = 'Failed to add traces to profile position plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
            
            # Add markers to figure -- profile positions
            
            i=0
            for dive in dives:
            
                try: 
                    df_dive = df_profiles[df_profiles['dive'] == dive]

                    if len(df_dive) < 1:
                        i = i+1
                        continue
                    
                    dt_str = df_dive['dt_str'].values[0]
                    dt_str = dt_str[0:10] + ' ' + dt_str[11:16] + 'Z'
                    
                    hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_dive['cep_km'].values[0]),
                            'source = ' + df_dive['src'].values[0],
                            '<extra></extra>'
                            ])
                    
                    
                    figs['position_globe'].add_trace(go.Scattergeo(
                        lat = df_dive['lat'],
                        lon = df_dive['lon'],
                        
                        hovertemplate = hovertemplate,
                        opacity = 0.5,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.2,
                                ),
                            # size=12 if (dive == dives[-1]) else 8, 
                            # symbol='star' if dive == dives[-1]  else 'circle',
                            size=10, 
                            symbol='circle',
                            ),
                        mode = 'markers',
                        ))
                    figs['position'].add_trace(go.Scattergeo(
                        lat = df_dive['lat'],
                        lon = df_dive['lon'],
                        hovertemplate = hovertemplate,
                        opacity = 1.0 if (dive == dives[-1]) else 0.5,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5 if (dive == dives[-1]) else 0.2,
                                ),
                            size=10, 
                            symbol='star' if dive == dives[-1]  else 'circle',
                            ),
                        mode = 'markers',
                        ))
                        
                    legendgroup=str(dive)
                        
                    figs['position_all'].add_trace(go.Scattergeo(
                        lat = df_dive['lat'],
                        lon = df_dive['lon'],
                        # hovertemplate = hovertemplate,
                        hoverinfo = 'none',
                        legendgroup=legendgroup,
                        legendgrouptitle=dict(text='Dive <b>{}</b>'.format(dive)),
                        opacity = 1,
                        marker = dict(
                            color='rgba(255,255,255,0)',
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 1,
                                ),
                            size=15, 
                            symbol='star' if dive == dives[-1]  else 'circle',
                            ),
                        mode = 'markers',
                        name='Profile position',
                        ))
                        
                        
                    i=i+1
                    
                except: 
                    errstr = 'Failed to add traces to profile position plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
            
            

            
            
            # Add markers to figures -- all gps positions and all positions
            
            i=0
            for dive in dives:
            
                try: 
                
                    legendgroup=str(dive)
                
                    idx = (df_gps['dive'] == dive) & (df_gps['idx'] == 1)
                    figs['position_all'].add_trace(go.Scattergeo(
                        lat = df_gps['lat'][idx],
                        lon = df_gps['lon'][idx],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'GPS before descent',
                            df_gps['dt_str'][idx].values[0],
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_gps['cep_km'][idx].values[0]),
                            '<extra></extra>'
                            ]) if len(df_gps[idx]) > 0 else '',
                        # hoverinfo = None,
                        legendgroup=legendgroup,
                        opacity = 0.7,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=9, 
                            symbol='x',
                            ),
                        mode = 'markers',
                        name = 'GPS before descent',
                        ))
                    idx = (df_gps['dive'] == dive) & (df_gps['idx'] == 3)
                    figs['position_all'].add_trace(go.Scattergeo(
                        lat = df_gps['lat'][idx],
                        lon = df_gps['lon'][idx],
                        customdata = df_gps['dt_str'][idx].values,
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'GPS beacon',
                            '%{customdata}',
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_gps['cep_km'][idx].values[0]),
                            '<extra></extra>'
                            ]) if len(df_gps[idx]) > 0 else '',
                        # hoverinfo = None,
                        legendgroup=legendgroup,
                        opacity = 0.7,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=9, 
                            symbol='star',
                            ),
                        mode = 'markers',
                        name = 'GPS beacon',
                        ))
                    idx = (df_gps['dive'] == dive) & ((df_gps['idx'] == 2) | (df_gps['idx'] == 0))
                    figs['position_all'].add_trace(go.Scattergeo(
                        lat = df_gps['lat'][idx],
                        lon = df_gps['lon'][idx],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'GPS after ascent',
                            df_gps['dt_str'][idx].values[0],
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_gps['cep_km'][idx].values[0]),
                            '<extra></extra>'
                            ]) if len(df_gps[idx]) > 0 else '',
                        # hoverinfo = 'none',
                        legendgroup=legendgroup,
                        opacity = 0.7,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=9, 
                            symbol='cross',
                            ),
                        mode = 'markers',
                        name = 'GPS after ascent',
                        ))

                    if(dive < 0):
                    
                        idx = (df_sbd['dive'] == dive)    
                        figs['position_all'].add_trace(go.Scattergeo(
                            lat = df_sbd['lat'][idx],
                            lon = df_sbd['lon'][idx],
                            #hovertemplate = hovertemplate,
                            hoverinfo = 'none',
                            legendgroup='{}'.format(dive),
                            opacity = 0.8,
                            marker = dict(
                                color='rgb(255,255,255)',
                                line=dict(
                                    color='rgb(0, 0, 0)',
                                    width = 0.5,
                                    ),
                                size=5, 
                                symbol='circle',
                                ),
                            mode = 'markers',
                            name = 'Iridium',
                            ))
                    
                    
                    else:
                    
                        idx = (df_iridium['dive'] == dive)    
                        figs['position_all'].add_trace(go.Scattergeo(
                            lat = df_iridium['lat'][idx],
                            lon = df_iridium['lon'][idx],
                            hovertemplate = '<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                'Iridium Composite',
                                df_iridium['dt_str'][idx].values[0],
                                '%{lat:.4f}°N %{lon:.4f}°E',
                                'CEP = {:.3f} km'.format(df_iridium['cep_km'][idx].values[0]),
                                '<extra></extra>'
                                ]) if len(df_iridium[idx]) > 0 else '',
                            # hoverinfo = 'none',
                            legendgroup=legendgroup,
                            opacity = 0.7,
                            marker = dict(
                                color=colors[i],
                                line=dict(
                                    color='rgb(0, 0, 0)',
                                    width = 0.5,
                                    ),
                                size=9, 
                                symbol='diamond',
                                ),
                            mode = 'markers',
                            name = 'Iridium Composite'
                            ))
                        idx = (df_sbd['dive'] == dive)    
                        figs['position_all'].add_trace(go.Scattergeo(
                            lat = df_sbd['lat'][idx],
                            lon = df_sbd['lon'][idx],
                            #hovertemplate = hovertemplate,
                            hoverinfo = 'none',
                            legendgroup=legendgroup,
                            opacity = 0.8,
                            marker = dict(
                                color=colors[i],
                                line=dict(
                                    color='rgb(0, 0, 0)',
                                    width = 0.5,
                                    ),
                                size=5, 
                                symbol='circle',
                                ),
                            mode = 'markers',
                            name = 'Iridium',
                            ))


                    idx = (df_gps['dive'] == dive) & (df_gps['idx'] == 1)
                    figs['position_gps'].add_trace(go.Scattergeo(
                        lat = df_gps['lat'][idx],
                        lon = df_gps['lon'][idx],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'GPS before descent',
                            df_gps['dt_str'][idx].values[0],
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_gps['cep_km'][idx].values[0]),
                            '<extra></extra>'
                            ]) if len(df_gps[idx]) > 0 else '',
                        # hoverinfo = None,
                        legendgroup=legendgroup,
                        legendgrouptitle=dict(text='Dive <b>{}</b>'.format(dive)),
                        opacity = 0.7,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=9, 
                            symbol='x',
                            ),
                        mode = 'markers',
                        name = 'GPS before descent',
                        ))
                    idx = (df_gps['dive'] == dive) & (df_gps['idx'] == 3)
                    figs['position_gps'].add_trace(go.Scattergeo(
                        lat = df_gps['lat'][idx],
                        lon = df_gps['lon'][idx],
                        customdata = df_gps['dt_str'][idx].values,
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'GPS beacon',
                            '%{customdata}',
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_gps['cep_km'][idx].values[0]),
                            '<extra></extra>'
                            ]) if len(df_gps[idx]) > 0 else '',
                        # hoverinfo = None,
                        legendgroup=legendgroup,
                        legendgrouptitle=dict(text='Dive <b>{}</b>'.format(dive)),
                        opacity = 0.7,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=9, 
                            symbol='star',
                            ),
                        mode = 'markers',
                        name = 'GPS beacon',
                        ))
                    idx = (df_gps['dive'] == dive) & ((df_gps['idx'] == 2) | (df_gps['idx'] == 0))
                    figs['position_gps'].add_trace(go.Scattergeo(
                        lat = df_gps['lat'][idx],
                        lon = df_gps['lon'][idx],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'GPS after ascent',
                            df_gps['dt_str'][idx].values[0],
                            '%{lat:.4f}°N %{lon:.4f}°E',
                            'CEP = {:.3f} km'.format(df_gps['cep_km'][idx].values[0]),
                            '<extra></extra>'
                            ]) if len(df_gps[idx]) > 0 else '',
                        # hoverinfo = 'none',
                        legendgroup=legendgroup,
                        legendgrouptitle=dict(text='Dive <b>{}</b>'.format(dive)),
                        opacity = 0.7,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=9, 
                            symbol='cross',
                            ),
                        mode = 'markers',
                        name = 'GPS after ascent',
                        ))

                    i=i+1
                
                except: 
                    errstr = 'Failed to add traces to profile position plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
            
            # Write figures
            
            for fig in figs.keys():
                    
                # Append meta
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'position',
                    # wide = False,
                    size = 'fig',
                    ts_pos = [0.97, -0.02],
                    showlegend = True if fig == 'position_all' or fig == 'position_gps' else False,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                
            # cleanup
            
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_gps' in locals(): del df_gps
            if 'df_iridium' in locals(): del df_iridium
            if 'df_profiles' in locals(): del df_profiles
            if 'df_sbd' in locals(): del df_sbd
            if 'dive' in locals(): del dive
            if 'dt_str' in locals(): del dt_str
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'hovertemplate' in locals(): del hovertemplate
            if 'i' in locals(): del i
            if 'idx' in locals(): del idx
            if 'legendgroup' in locals(): del legendgroup
            if 'position_aratio' in locals(): del position_aratio
            if 'position_mean' in locals(): del position_mean
            if 'position_mid' in locals(): del position_mid
            if 'position_span' in locals(): del position_span
            if 'rng' in locals(): del rng
            gc.collect()
            
        except: 
            errstr = 'Failed to profile position plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteSubmergedDisplacementPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the submerged displacement plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'displacement_submerged' in float_aggregate_data or len(float_aggregate_data['displacement_submerged']) < 1:
        logger.warning('No submerged displacement data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Create color range
            
            #colors = list(Color("#33FF33").range_to(Color("#3333FF"), round(len(dives)/2))) + list(Color("#3333FF").range_to(Color("#FF3333"), round(len(dives)/2)+1))
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
        
            # Preprocess profile data
                
            df_dives = float_aggregate_data['displacement_submerged']
            
            df_dives.loc[
                (df_dives['speed_mps'] > 2) | (df_dives['dist_m'] > 500000),
                ['speed_mps', 'dist_m']] = np.nan
            
            df_dives = df_dives[(~df_dives['dist_m'].isna()) & (~df_dives['speed_mps'].isna())]
            
            # Calculate data ranges
            
            rng = dict(
                dist_km     = np.array([0, np.percentile(df_dives['dist_m'].values, [100])[0]/1000]).astype(np.double),
                speed_cmps  = np.array([0, np.percentile(df_dives['speed_mps'].values, [100])[0]*100]).astype(np.double),
                )
            rng['dist_km']    = np.round(rng['dist_km']    + [ 0.00, 10.0], -1)
            rng['speed_cmps'] = np.round(rng['speed_cmps'] + [ 0.00, 10.0], -1)

            
            # Generate Figures
            figs = dict(
                displacement_submerged = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Submerged Dispacement'.format(float_id)),
                        polar=dict(
                            angularaxis = dict(
                                direction='clockwise',
                                rotation=90,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linecolor='rgb(169,169,169)',
                                linewidth = 0.5,
                                showgrid = True,
                                showline = True,
                                showticklabels = True,
                                tickmode = 'array',
                                ticktext = ['N','','','','','','',''],
                                tickvals = [0, 45, 90, 135, 180, 225, 270, 315],
                                tickfont = dict(
                                    size = 12,
                                    ),
                                ),
                            bgcolor='white',
                            radialaxis = dict(
                                title=dict(
                                    text = 'Distance [km]',
                                    font = dict(
                                        size= 10,
                                        ),
                                    ),
                                autorange=False,
                                angle=22.5,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linewidth = 1,
                                nticks = 8,
                                range = rng['dist_km'],
                                showgrid = True,
                                showline = False,
                                tickangle = 22.5,
                                tickfont = dict(
                                    size = 10,
                                    ),
                                ),
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        # plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=10,t=70,pad=4),
                        )
                    ),
                displacement_submerged_velocity = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Submerged Velocity'.format(float_id)),
                        polar=dict(
                            angularaxis = dict(
                                direction='clockwise',
                                rotation=90,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linecolor='rgb(169,169,169)',
                                linewidth = 0.5,
                                showgrid = True,
                                showline = True,
                                showticklabels = True,
                                tickmode = 'array',
                                ticktext = ['N','','','','','','',''],
                                tickvals = [0, 45, 90, 135, 180, 225, 270, 315],
                                tickfont = dict(
                                    size = 12,
                                    ),
                                ),
                            bgcolor='white',
                            radialaxis = dict(
                                title=dict(
                                    text = 'Speed [cm s<sup>-1</sup>]',
                                    font = dict(
                                        size= 10,
                                        ),
                                    ),
                                autorange=False,
                                angle=22.5,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linewidth = 1,
                                nticks = 8,
                                range = rng['speed_cmps'],
                                showgrid = True,
                                showline = False,
                                tickangle = 22.5,
                                tickfont = dict(
                                    size = 10,
                                    ),
                                ),
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        # plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=10,t=70,pad=4),
                        )
                    ),
                )
                
            # Add traces to figures   
            
            i=0
            for dive in dives:
                try: 
                    
                    if dive == 0:
                        i=i+1
                        continue
                
                    df_dive = df_dives[df_dives['dive'] == dive]
                    
                    if len(df_dive) < 1:
                        i=i+1
                        logger.debug('No submerged displacement data for float {}, dive {}'.format(float_id, dive))
                        continue
                    
                    dt1 = df_dive['pos1_dt_str'].values[0]
                    dt1 = dt1[0:10] + ' ' + dt1[11:16] + 'Z'
                    dt2 = df_dive['pos2_dt_str'].values[0]
                    dt2 = dt2[0:10] + ' ' + dt2[11:16] + 'Z'
                        
                    
                    figs['displacement_submerged'].add_trace(go.Scatterpolar(
                        r = [df_dive['dist_m'].values[0]/1000],
                        theta = [ df_dive['azi'].values[0]],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'bearing = %{theta:.0f}°',
                            'dist = %{r:.2f} km',
                            'duration = {:.2f} hours'.format(df_dive['duration_s'].values[0]/3600.),
                            'speed = {:.2f} cm s<sup>-1</sup>'.format(df_dive['speed_mps'].values[0]*100),
                            'pos1 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos1_lat'].values[0], df_dive['pos1_lon'].values[0]),
                            ' ' + dt1,
                            'pos2 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos2_lat'].values[0], df_dive['pos2_lon'].values[0]),
                            ' ' + dt2,
                            '<extra></extra>'
                            ]),
                        # line=dict(
                            # color=colors[i],
                            # width=3,
                            # ),
                        marker=dict(
                            color = [ colors[i] ],
                            size = 10,
                            line = dict(
                                color=[  'black'] ,
                                width=0.3
                                ),
                            ),
                        mode='markers',
                        name = '{}'.format(dive),
                        # opacity=0.7,
                        ))
                    figs['displacement_submerged_velocity'].add_trace(go.Scatterpolar(
                        r = [df_dive['speed_mps'].values[0]*100],
                        theta = [ df_dive['azi'].values[0]],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'bearing = %{theta:.0f}°',
                            'dist = {:.2f} km'.format(df_dive['dist_m'].values[0]/1000),
                            'duration = {:.2f} hours'.format(df_dive['duration_s'].values[0]/3600.),
                            'speed = %{r:.2f} cm s<sup>-1</sup>',
                            'pos1 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos1_lat'].values[0], df_dive['pos1_lon'].values[0]),
                            ' ' + dt1,
                            'pos2 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos2_lat'].values[0], df_dive['pos2_lon'].values[0]),
                            ' ' + dt2,
                            '<extra></extra>',
                            ]),
                        # line=dict(
                            # color=colors[i],
                            # width=3,
                            # ),
                        marker=dict(
                            color = [ colors[i] ],
                            size = 10,
                            line = dict(
                                color=[ 'black'] ,
                                width=0.3
                                ),
                            ),
                        mode='markers',
                        name = '{}'.format(dive),
                        # opacity=0.7,
                        ))
                        
                    del dt1
                    del dt2
                    
                
                except: 
                    errstr = 'Failed to add traces to submerged displacement plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                i=i+1
                
                
            # Write figures
            
            for fig in figs.keys():
                
                # Append meta
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'displacement',
                    # wide = False,
                    size = 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
        
        except: 
            errstr = 'Failed to submerged displacement plotes for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteSurfaceDisplacementPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the surface displacement plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'displacement_surface' in float_aggregate_data or len(float_aggregate_data['displacement_surface']) < 1:
        logger.warning('No surface displacement data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Create color range
            
            #colors = list(Color("#33FF33").range_to(Color("#3333FF"), round(len(dives)/2))) + list(Color("#3333FF").range_to(Color("#FF3333"), round(len(dives)/2)+1))
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-2] = 'rgb(0,0,0)'
        
            # Preprocess profile data
                
            df_dives = float_aggregate_data['displacement_surface']
            
            df_dives.loc[
                (df_dives['speed_mps'] > 3) | (df_dives['dist_m'] > 5000),
                ['speed_mps', 'dist_m']] = np.nan
            
            df_dives = df_dives[(~df_dives['dist_m'].isna()) & (~df_dives['speed_mps'].isna())]
            
            # Calculate data ranges
            
            rng = dict(
                dist_m     = np.array([0, np.percentile(df_dives['dist_m'].values, [100])[0]]).astype(np.double),
                speed_cmps  = np.array([0, np.percentile(df_dives['speed_mps'].values, [100])[0]*100]).astype(np.double),
                )
            rng['dist_m']    = np.round(rng['dist_m']    + [ 0.00, 100.0], -2)
            rng['speed_cmps'] = np.round(rng['speed_cmps'] + [ 0.00, 10.00], -1)
            
            # Generate Figures
            figs = dict(
                displacement_surface = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Surface Dispacement'.format(float_id)),
                        polar=dict(
                            angularaxis = dict(
                                direction='clockwise',
                                rotation=90,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linecolor='rgb(169,169,169)',
                                linewidth = 0.5,
                                showgrid = True,
                                showline = True,
                                showticklabels = True,
                                tickmode = 'array',
                                ticktext = ['N','','','','','','',''],
                                tickvals = [0, 45, 90, 135, 180, 225, 270, 315],
                                tickfont = dict(
                                    size = 12,
                                    ),
                                ),
                            bgcolor='white',
                            radialaxis = dict(
                                title=dict(
                                    text = 'Distance [m]',
                                    font = dict(
                                        size= 10,
                                        ),
                                    ),
                                angle=22.5,
                                autorange = False,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linewidth = 1,
                                nticks = 8,
                                showgrid = True,
                                showline = False,
                                range = rng['dist_m'],
                                tickangle = 22.5,
                                tickfont = dict(
                                    size = 10,
                                    ),
                                ),
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        # plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=10,t=70,pad=4),
                        )
                    ),
                displacement_surface_velocity = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Surface Velocity'.format(float_id)),
                        polar=dict(
                            angularaxis = dict(
                                direction='clockwise',
                                rotation=90,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linecolor='rgb(169,169,169)',
                                linewidth = 0.5,
                                showgrid = True,
                                showline = True,
                                showticklabels = True,
                                tickmode = 'array',
                                ticktext = ['N','','','','','','',''],
                                tickvals = [0, 45, 90, 135, 180, 225, 270, 315],
                                tickfont = dict(
                                    size = 12,
                                    ),
                                ),
                            bgcolor='white',
                            radialaxis = dict(
                                title=dict(
                                    text = 'Speed [cm s<sup>-1</sup>]',
                                    font = dict(
                                        size= 10,
                                        ),
                                    ),
                                angle=22.5,
                                autorange=False,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linewidth = 1,
                                nticks = 8,
                                showgrid = True,
                                showline= False,
                                range=rng['speed_cmps'],
                                tickangle = 22.5,
                                tickfont = dict(
                                    size = 10,
                                    ),
                                ),
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        # plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=10,t=70,pad=4),
                        )
                    ),
                )
                
            # Add traces to figures   
                
            i=0
            for dive in dives:
                try: 
                    
                    if dive == 0:
                        i=i+1
                        continue
                
                    df_dive = df_dives[df_dives['dive'] == dive]

                    if len(df_dive) < 1:
                        i=i+1
                        logger.debug('No surface displacement data for float {}, dive {}'.format(float_id, dive))
                        continue

                    dt1 = df_dive['pos1_dt_str'].values[0]
                    dt1 = dt1[0:10] + ' ' + dt1[11:16] + 'Z'
                    dt2 = df_dive['pos2_dt_str'].values[0]
                    dt2 = dt2[0:10] + ' ' + dt2[11:16] + 'Z'

                    figs['displacement_surface'].add_trace(go.Scatterpolar(
                        r = [df_dive['dist_m'].values[0]],
                        theta = [ df_dive['azi'].values[0]],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'bearing = %{theta:.0f}°',
                            'dist = %{r:.0f} m',
                            'duration = {:.0f} minutes'.format(df_dive['duration_s'].values[0]/60.),
                            'speed = {:.1f} cm s<sup>-1</sup>'.format(df_dive['speed_mps'].values[0]*100),
                            'pos1 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos1_lat'].values[0], df_dive['pos1_lon'].values[0]),
                            ' ' + dt1,
                            'pos2 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos2_lat'].values[0], df_dive['pos2_lon'].values[0]),
                            ' ' + dt2,
                            '<extra></extra>'
                            ]),
                        # line=dict(
                            # color=colors[i],
                            # width=3,
                            # ),
                        marker=dict(
                            color = [ colors[i] ],
                            size=10,
                            line = dict(
                                color=[ 'black'] ,
                                width=0.3
                                ),
                            ),
                        mode='markers',
                        name = '{}'.format(dive),
                        # opacity=0.7,
                        ))
                    figs['displacement_surface_velocity'].add_trace(go.Scatterpolar(
                        r = [df_dive['speed_mps'].values[0]*100],
                        theta = [ df_dive['azi'].values[0]],
                        hovertemplate = '<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'bearing = %{theta:.0f}°',
                            'dist = {:.0f} m'.format(df_dive['dist_m'].values[0]),
                            'duration = {:.0f} minutes'.format(df_dive['duration_s'].values[0]/60.),
                            'speed = %{r:.1f} cm s<sup>-1</sup>',
                            'pos1 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos1_lat'].values[0], df_dive['pos1_lon'].values[0]),
                            ' ' + dt1,
                            'pos2 = ',
                            ' {:.4f}°N {:.4f}°E'.format(df_dive['pos2_lat'].values[0], df_dive['pos2_lon'].values[0]),
                            ' ' + dt2,
                            '<extra></extra>'
                            ]),
                        # line=dict(
                            # color=colors[i],
                            # width=3,
                            # ),
                        marker=dict(
                            color = [  colors[i] ],
                            size = 10,
                            line = dict(
                                color=[ 'black'] ,
                                width=0.3
                                ),
                            ),
                        mode='markers',
                        name = '{}'.format(dive),
                        # opacity=0.7,
                        ))
                        
                    del dt1
                    del dt2

                
                except: 
                    errstr = 'Failed to add traces to surface displacement plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                i=i+1
                
                
            # Write figures
            
            for fig in figs.keys():
                
                # Append meta
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'displacement',
                    # wide = False,
                    size = 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
        
        except: 
            errstr = 'Failed to submerged displacement plotes for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteTrajectoryPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the pressure-time trajectory plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if (not 'ascent' in float_aggregate_data or len(float_aggregate_data['ascent']) < 1) and (not 'descent' in float_aggregate_data or len(float_aggregate_data['descent']) < 1):
        logger.warning('No trajectory data for float {}'.format(float_id))
    else:
    
        try:
        
            # Preprocess data
            
            df_ascent = float_aggregate_data['ascent']
            df_descent = float_aggregate_data['descent']
            df_surface = float_aggregate_data['surface']
            df_drift = float_aggregate_data['drift']
            df_gps =  float_aggregate_data['gps']
            df_sbd = float_aggregate_data['sbd']
           
            df_ascent.loc[
                    (df_ascent['pres'] <= -5 ) | (df_ascent['pres'] >= 3000) | df_ascent['pres'].isna(),
                    ['pres','dt_str']] = np.nan
            df_descent.loc[
                    (df_descent['pres'] <= -5 ) | (df_descent['pres'] >= 3000) | df_descent['pres'].isna(),
                    ['pres','dt_str']] = np.nan
            df_drift.loc[
                    (df_drift['pres'] <= -5 ) | (df_drift['pres'] >= 3000) | df_drift['pres'].isna(),
                    ['pres','dt_str']] = np.nan
            df_surface.loc[
                    (df_surface['pres'] <= -5 ) | (df_surface['pres'] >= 600) | df_surface['pres'].isna(),
                    ['pres','dt_str']] = np.nan
                    
            df_ascent = df_ascent[df_ascent['dive'] > 0] # TODO fix
            df_descent = df_descent[df_descent['dive'] > 0]
            df_surface = df_surface[df_surface['dive'] > 0]
            df_drift = df_drift[df_drift['dive'] > 0]

            # Seafloor pressure at the GPS fixes of dives 1 and later

            df_bathy = df_gps[df_gps['lat'].between(-90, 90) & df_gps['lon'].between(-360, 360)
                              & ~((df_gps['lat'] == 0) & (df_gps['lon'] == 0)) & (df_gps['dt_epoch'] > 0)
                              & (df_gps['dive'] >= 1)]
            df_bathy = df_bathy.sort_values('dt_epoch')
            bathy_pres = BathymetryPressure(df_bathy['lat'].values, df_bathy['lon'].values) if len(df_bathy) else None

            # Calculate ranges
           
            rng = dict(
                pres = np.array([
                    np.max([
                        np.max(df_ascent['pres'][~df_ascent['pres'].isna()].values) if len(df_ascent['pres'][~df_ascent['pres'].isna()]) else -10, 
                        np.max(df_descent['pres'][~df_descent['pres'].isna()].values) if len(df_descent['pres'][~df_descent['pres'].isna()]) else -10,
                        np.max(df_drift['pres'][~df_drift['pres'].isna()].values) if len(df_drift['pres'][~df_drift['pres'].isna()]) else -10,
                        np.max(df_surface['pres'][~df_surface['pres'].isna()].values) if len(df_surface['pres'][~df_surface['pres'].isna()]) else -10,
                        ]), 
                    -10
                    ]).astype(np.double),
                time = np.array([
                    np.min([
                        np.min(df_ascent['dt_epoch'][df_ascent['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_ascent['dt_epoch'][df_ascent['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                        np.min(df_descent['dt_epoch'][df_descent['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_descent['dt_epoch'][df_descent['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                        np.min(df_drift['dt_epoch'][df_drift['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_drift['dt_epoch'][df_drift['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                        np.min(df_surface['dt_epoch'][df_surface['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_surface['dt_epoch'][df_surface['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,                                            
                        ]),
                    np.max([
                        np.max(df_ascent['dt_epoch'][df_ascent['dt_epoch'] > 0].values) if len(df_ascent['dt_epoch'][df_ascent['dt_epoch'] > 0]) else 0,
                        np.max(df_descent['dt_epoch'][df_descent['dt_epoch'] > 0].values) if len(df_descent['dt_epoch'][df_descent['dt_epoch'] > 0]) else 0,
                        np.max(df_drift['dt_epoch'][df_drift['dt_epoch'] > 0].values) if len(df_drift['dt_epoch'][df_drift['dt_epoch'] > 0]) else 0,
                        np.max(df_surface['dt_epoch'][df_surface['dt_epoch'] > 0].values) if len(df_surface['dt_epoch'][df_surface['dt_epoch'] > 0]) else 0,                                            
                        ]),
                    ])
                )
            rng['pres']   = np.round(rng['pres']   + [ 5.00, 0.00], 0)

            rng['time'] = np.round(rng['time'] + [(rng['time'][1] - rng['time'][0]) * -0.03, (rng['time'][1] - rng['time'][0]) * +0.03] + [-21600, +21600])
            rng['time'] = [ DateTime().dt_from_epoch(x).dt_str for x in rng['time'] ]

            
            # Generate Figures
            
            figs = dict(
                trajectory = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title=dict(text='{}: Trajectory'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        legend=dict(
                            title='Cycle Phase',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                )

            # Add seafloor trace (background): ETOPO2 depth at the GPS fixes, as pressure

            if bathy_pres is not None and np.any(~np.isnan(bathy_pres)):
                figs['trajectory'].add_trace(go.Scatter(
                    x=df_bathy['dt_str'],
                    y=bathy_pres,
                    customdata=np.stack(
                        (   df_bathy['dive'],
                            df_bathy['lat'],
                            df_bathy['lon'],
                            ),
                        -1),
                    hovertemplate='<br>'.join([
                        'Dive <b>%{customdata[0]}</b>: Bathymetry (ETOPO2)',
                        '%{x|%Y-%m-%d %H:%M}Z',
                        '%{customdata[1]:.4f}°N %{customdata[2]:.4f}°E',
                        '%{y:.0f} dBar',
                        '<extra></extra>',
                        ]),
                    line=dict(
                        color='rgb(0,0,0)',
                        width=2.0,
                        ),
                    mode='lines',
                    name='Bathymetry',
                    opacity=1.0,
                    ))

            # Add target drift pressure trace (background): DTD<n> for the current
            # mission phase MPS, or DTD on firmware without phases, as one continuous
            # stepped line that changes at the start of descent of each dive and ends at
            # the start of the last ascent, for dives 1 and later. Dives without mission
            # parameters keep the last reported target.

            targets = {}
            df_mission = float_aggregate_data.get('mission')
            if df_mission is not None:
                for _, mrow in df_mission.iterrows():
                    # Skip unset configurations (MPS 255, DTD 65535) sent before a mission is loaded
                    mps = pd.to_numeric(mrow.get('MPS'), errors = 'coerce')
                    if pd.notna(mps) and not 0 <= mps <= 9:
                        continue
                    # The phase is reported from before the dive's phase switch: once the
                    # phase's cycle count is used up (MPC >= MNM<n>), the dive drifts in the
                    # next phase MNS<n>.
                    if pd.notna(mps):
                        mnm, mns, mpc = (pd.to_numeric(mrow.get(k), errors = 'coerce')
                                         for k in ('MNM{:d}'.format(int(mps)), 'MNS{:d}'.format(int(mps)), 'MPC'))
                        if pd.notna(mnm) and pd.notna(mns) and pd.notna(mpc) and mpc >= mnm and 0 <= mns <= 9:
                            mps = mns
                    keys = (['DTD{:d}'.format(int(mps))] if pd.notna(mps) else []) + ['DTD0', 'DTD']
                    value = next((pd.to_numeric(mrow[k], errors = 'coerce') for k in keys if k in mrow and pd.notna(pd.to_numeric(mrow[k], errors = 'coerce'))), None)
                    if value is not None and 0 < value < 7000:
                        targets[int(mrow['dive'])] = float(value)

            target_x, target_y, target_dive = [], [], []
            if targets:
                t_start = pd.concat([df_descent, df_drift])
                t_start = t_start[(t_start['dive'] >= 1) & (t_start['dt_epoch'] > 0) & t_start['pres'].notna()].groupby('dive')['dt_epoch'].min()
                t_end_ascent = df_ascent[(df_ascent['dt_epoch'] > 0) & df_ascent['pres'].notna()].groupby('dive')['dt_epoch'].min()
                t_end_drift = df_drift[(df_drift['dt_epoch'] > 0) & df_drift['pres'].notna()].groupby('dive')['dt_epoch'].max()
                for dive_num, t0 in t_start.sort_values().items():
                    prior = [d for d in targets if d <= dive_num]
                    target = targets[np.max(prior)] if prior else targets[np.min(list(targets))]
                    target_x.append(DateTime().dt_from_epoch(t0).dt_str)
                    target_y.append(target)
                    target_dive.append(dive_num)
                if target_x:
                    t1 = t_end_ascent.get(target_dive[-1], t_end_drift.get(target_dive[-1]))
                    if t1 is not None and t1 > t_start[target_dive[-1]]:
                        target_x.append(DateTime().dt_from_epoch(t1).dt_str)
                        target_y.append(target_y[-1])
                        target_dive.append(target_dive[-1])

            if target_x:
                figs['trajectory'].add_trace(go.Scatter(
                    x=target_x,
                    y=target_y,
                    customdata=target_dive,
                    hovertemplate='<br>'.join([
                        'Dive <b>%{customdata}</b>: Drift Target',
                        '%{y:.0f} dBar',
                        '<extra></extra>',
                        ]),
                    line=dict(
                        color='rgb(190,190,190)',
                        width=1.0,
                        dash='dash',
                        shape='hv',
                        ),
                    mode='lines',
                    name='Drift Target',
                    opacity=1.0,
                    ))

            # Add satcom traces

            figs['trajectory'].add_trace(go.Scatter(
                x=df_sbd['dt_str'],
                y=[-0.5]*len(df_sbd),
                customdata=np.stack(
                    (   df_sbd['dive'], 
                        df_sbd['momsn'],
                        df_sbd['pid'],
                        df_sbd['size_b'],
                        ), 
                    -1),
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata[0]}</b>: Iridium',
                    '%{x|%Y-%m-%d %H:%M:%S}Z',
                    'MOMSN = %{customdata[1]}',
                    'PID = %{customdata[2]}',
                    'size = %{customdata[3]} bytes',
                    '<extra></extra>',
                    ]),
                hoverinfo='none',
                mode='markers',
                marker=dict(
                    color='rgb(69,255,255)',
                    line=dict(
                        color='rgb(69,255,255)',
                        width=2.0,
                        ),
                    size=8,
                    symbol='x-thin',
                    ),
                name='Iridium',
                opacity=0.8,
                ))

            figs['trajectory'].add_trace(go.Scatter(
                x=df_gps['dt_str'],
                y=[-0.5]*len(df_gps),
                customdata=np.stack(
                    (   df_gps['dive'], 
                        df_gps['lat'],
                        df_gps['lon'],
                        ), 
                    -1),
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata[0]}</b>: GPS',
                    '%{x|%Y-%m-%d %H:%M}Z',
                    '%{customdata[1]:.4f}°N %{customdata[2]:.4f}°E',
                    '<extra></extra>',
                    ]),
                mode='markers',
                marker=dict(
                    color='rgb(255,69,255)',
                    line=dict(
                        color='rgb(255,69,255)',
                        width=1.5,
                        ),
                    size=7,
                    symbol='cross-thin',
                    ),
                name='GPS',
                opacity=0.8,
                ))
            
            # Add traces
            
            figs['trajectory'].add_trace(go.Scatter(
                x=df_ascent['dt_str'],
                y=df_ascent['pres'],
                customdata=df_ascent['dive'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata}</b>: Ascent',
                    '%{x|%Y-%m-%d %H:%M:%S}Z',
                    '%{y:.2f} dBar',
                    '<extra></extra>',
                    ]),
                line=dict(
                    color="rgb(255,69,69)",
                    width=1.0,
                    ),
                mode='lines',
                name='Ascent',
                opacity=0.8,
                ))
            figs['trajectory'].add_trace(go.Scatter(
                x=df_descent['dt_str'],
                y=df_descent['pres'],
                customdata=df_descent['dive'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata}</b>: Descent',
                    '%{x|%Y-%m-%d %H:%M:%S}Z',
                    '%{y:.2f} dBar',
                    '<extra></extra>',
                    ]),
                line=dict(
                    color="rgb(69,255,69)",
                    width=1.0,
                    ),
                mode='lines',
                name='Descent',
                opacity=0.8,
                ))
            figs['trajectory'].add_trace(go.Scatter(
                x=df_drift['dt_str'],
                y=df_drift['pres'],
                customdata=df_drift['dive'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata}</b>: Drift',
                    '%{x|%Y-%m-%d %H:%M}Z',
                    '%{y:.2f} dBar',
                    '<extra></extra>',
                    ]),
                line=dict(
                    color="rgb(69,69,255)",
                    width=1.0,
                    ),
                mode='lines',
                name='Drift',
                opacity=0.8,
                ))
            figs['trajectory'].add_trace(go.Scatter(
                x=df_surface['dt_str'],
                y=df_surface['pres'],
                customdata=df_surface['dive'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata}</b>: Surface',
                    '%{x|%Y-%m-%d %H:%M:%S}Z',
                    '%{y:.2f} dBar',
                    '<extra></extra>',
                    ]),
                line=dict(
                    color='rgb(122,122,122)',
                    width=1.0
                    ),
                mode='lines+markers',
                marker=dict(
                    line=dict(
                        color='rgba(122,122,122,0.1)',
                        width=1.0
                        ),
                    size=5,
                    symbol='circle-open',
                    color="rgb(122,122,122)"
                    ),
                name='Surface',
                opacity=0.8,
                ))
                

            
            # Write plot files
            
            for fig in figs.keys():

                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'trajectory',
                    # wide = True,
                    size = 'figWideL' if rng['pres'][0] > 500 else 'figWide',
                    ts_pos = [1, -.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
            
            # Cleanup
            
            if 'df_ascent' in locals(): del df_ascent
            if 'df_descent' in locals(): del df_descent
            if 'df_drift' in locals(): del df_drift
            if 'df_gps' in locals(): del df_gps
            if 'df_sbd' in locals(): del df_sbd
            if 'df_surface' in locals(): del df_surface
            if 'df_bathy' in locals(): del df_bathy
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'rng' in locals(): del rng
            gc.collect()
                    
        except: 
            errstr = 'Failed to generate trajectory plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteCycleTimingPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the cycle timing plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'cycle_timing' in float_aggregate_data or len(float_aggregate_data['cycle_timing']) < 1:
        logger.warning('No cycle timing data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Preprocess
        
            df_ct = float_aggregate_data['cycle_timing']
            df_ct = df_ct[ df_ct['dive'] >= 0 ]
        
            # df_ct = df_ct[df_ct['dive'] > 0]
            # df_ct = df_ct[df_ct['surface_duration_s'] > 0]
        
            # Generate figures
            
            figs = dict(
                cycle_timing = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title=dict(text='{}: Dive Cycle Duration'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            ),
                        yaxis=dict(
                            title='Duration [hours]',
                            # autorange=False,
                            # range=rng['pres'],
                            ),
                        bargap=0,
                        barmode='stack',
                        legend=dict(
                            title='Cycle Phase',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                )
                
            # Add traces to figures
            


            figs['cycle_timing'].add_trace(go.Bar(
                x = df_ct['dive'],
                y = df_ct['descent_duration_s']/3600,
                customdata = [ DateTime.duration_to_hhmmss_str(x) for x in df_ct['descent_duration_s'].values ],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b> Descent: %{customdata}',
                    # 'starting %{customdata|%Y-%m-%d %H:%M:%S}Z'
                    '<extra></extra>',
                    ]),
                marker = dict(
                    color="rgb(99,222,99)",
                    line = dict(
                        color="rgb(99,222,99)",
                        width=1,
                        )
                    ),
                name = 'Descent',
                # opacity=0.8,
                ))
            figs['cycle_timing'].add_trace(go.Bar(
                x = df_ct['dive'],
                y = df_ct['drift_duration_s']/3600,
                customdata = [ DateTime.duration_to_hhmmss_str(x) for x in df_ct['drift_duration_s'].values ],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b> Drift: %{customdata}',
                    # 'starting %{customdata|%Y-%m-%d %H:%M:%S}Z'
                    '<extra></extra>',
                    ]),
                marker = dict(
                    color="rgb(99,99,222)",
                    line = dict(
                        color="rgb(99,99,222)",
                        width=1,
                        )
                    ),
                name = 'Drift',
                # opacity=0.8,
                ))
            figs['cycle_timing'].add_trace(go.Bar(
                x = df_ct['dive'],
                y = df_ct['ascent_duration_s']/3600,
                customdata = [ DateTime.duration_to_hhmmss_str(x) for x in df_ct['ascent_duration_s'].values ],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b> Ascent: %{customdata}',
                    # 'starting %{customdata|%Y-%m-%d %H:%M:%S}Z'
                    '<extra></extra>',
                    ]),
                marker = dict(
                    color="rgb(222,99,99)",
                    line = dict(
                        color="rgb(222,99,99)",
                        width=1,
                        )
                    ),
                name = 'Ascent',
                # opacity=0.8,
                ))
            figs['cycle_timing'].add_trace(go.Bar(
                x = df_ct['dive'],
                y = df_ct['surface_duration_s']/3600,
                customdata = [ DateTime.duration_to_hhmmss_str(x) for x in df_ct['surface_duration_s'].values ],
                # customdata = np.stack(
                    # (   np.round((df_ct['surface_duration_s']/60).astype(np.double), 1),
                        # df_ct['surface_dt_str'],
                        # ), -1),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b> Surface: %{customdata}',
                    # 'starting %{customdata[1]|%Y-%m-%d %H:%M:%S}Z'
                    '<extra></extra>',
                    ]),
                marker = dict(
                    color='rgb(122,122,122)',
                    line = dict(
                        color='rgb(122,122,122)',
                        width=1,
                        )
                    ),
                name = 'Surface',
                # opacity=0.8,
                )) 
            figs['cycle_timing'].add_trace(go.Bar(
                x = df_ct['dive'],
                y = [ 0 ] * len(df_ct),
                customdata = [ DateTime.duration_to_hhmmss_str(x) for x in df_ct['total_duration_s'].values ],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b> Total: %{customdata}',
                    '<extra></extra>',
                    ]),
                marker = dict(
                    color="rgba(0,0,0,0)",
                    # line = dict(
                        # color="rgb(99,222,99)",
                        # width=1,
                        # )
                    ),
                name = 'Dive',
                showlegend= False,
                # opacity=0.8,
                ))
               

                
            # Write figures
            
            for fig in figs.keys():
                
                # Append meta
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'cycle_timing',
                    # wide = False,
                    size = 'figWideS',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
            
            
          
            
        except: 
            errstr = 'Failed to cycle plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteProfilePlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate profile plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'profile' in float_aggregate_data or len(float_aggregate_data['profile']) < 1:
        logger.warning('No profile data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Create color range
            
            #colors = list(Color("#33FF33").range_to(Color("#3333FF"), round(len(dives)/2))) + list(Color("#3333FF").range_to(Color("#FF3333"), round(len(dives)/2)+1))
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
            
            # Preprocess profile data
            
            
            df_profiles = float_aggregate_data['profile']
            df_profiles.loc[
                (df_profiles['pres'] < 2 )|(df_profiles['pres'] >= 3000)|df_profiles['pres'].isna(),
                ['pres','dens','sigma0']] = np.nan
            df_profiles = df_profiles[~df_profiles['pres'].isna()]
            df_profiles.loc[
                (df_profiles['saln'] <= 20 )|(df_profiles['saln'] >= 40)|df_profiles['saln'].isna(),
                ['saln','dens','sigma0']] = np.nan
            df_profiles.loc[
                (df_profiles['temp'] <= -5 )|(df_profiles['temp'] >= 40)|df_profiles['temp'].isna(),
                ['temp','dens','sigma0']] = np.nan
                
            df_profiles_gb = df_profiles[['dive','pres']].groupby(['dive'])
             
            # Calculate data ranges
            
            rng = dict(
                dens   = np.percentile(df_profiles['dens'][~df_profiles['dens'].isna()].values, [1, 99]).astype(np.double),
                pres   = np.array([np.max(df_profiles['pres'][~df_profiles['pres'].isna()].values), 0 ]).astype(np.double),
                saln   = np.percentile(df_profiles['saln'][~df_profiles['saln'].isna()].values, [1, 99]).astype(np.double),
                sigma0 = np.percentile(df_profiles['sigma0'][~df_profiles['sigma0'].isna()].values, [1, 99]).astype(np.double),
                temp   = np.percentile(df_profiles['temp'][~df_profiles['temp'].isna()].values, [1, 99]).astype(np.double),
                )
            rng['dens']   = np.round(rng['dens']   + [-1.00, 1.00], 1)
            rng['pres']   = np.round(rng['pres']   + [ 1.00, 0.00], 0)
            rng['saln']   = np.round(rng['saln']   + [-0.95, 0.95], 2)
            rng['sigma0'] = np.round(rng['sigma0'] + [-0.50, 0.50], 1)
            rng['temp']   = np.round(rng['temp']   + [-1.00, 1.00], 1)
            
            # Generate Figures
            
            figs = dict(
                profile_dens = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: In-situ Density Profiles'.format(float_id)),
                        xaxis=dict(
                            title='In-situ Density Anomaly, ρ - 1000 [kg m<sup>−3</sup>]',
                            autorange=False,
                            range=rng['dens'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                profile_saln = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Salinity Profiles'.format(float_id)),
                        xaxis=dict(
                            title='Salinity [PSU]',
                            autorange=False,
                            range=rng['saln'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                profile_sigma0 = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Potential Density Profiles'.format(float_id)),
                        xaxis=dict(
                            title='Potential Density Anomaly, σ<sub>0</sub> [kg m<sup>−3</sup>]',
                            autorange=False,
                            range=rng['sigma0'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                profile_temp = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Temperature Profiles'.format(float_id)),
                        xaxis=dict(
                            title='Temperature [°C]',
                            autorange=False,
                            range=rng['temp'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                profile_ts = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: T-S Profiles'.format(float_id)),
                        xaxis=dict(
                            title='Salinity [PSU]',
                            autorange=False,
                            range=rng['saln'],
                            ),
                        yaxis=dict(
                            title='Temperature [°C]',
                            autorange=False,
                            range=rng['temp'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                profile_num_bins = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: No. Profile Bins'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['saln'],
                            ),
                        yaxis=dict(
                            title='Bins',
                            # autorange=False,
                            # range=rng['temp'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        # legend=dict(
                            # title='Dive',
                            # ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
            )
            
            # Add traces to figures
            
            i=0
            for dive in dives:
                try: 
            
                    df_dive = df_profiles[df_profiles['dive'] == dive]
                    
                    if len(df_dive) < 1:
                        i=i+1
                        logger.debug('No profile data for float {}, dive {}'.format(float_id, dive))
                        continue
                    
                    dt_str = ''
                    df_position = float_aggregate_data['profile_position'][float_aggregate_data['profile_position']['dive'] == dive]
                    if len(df_position) > 0:
                        dt_str = df_position['dt_str'].values[0]
                        dt_str = dt_str[0:10] + ' ' + dt_str[11:16] + 'Z'
                        
                    figs['profile_dens'].add_trace(go.Scatter(
                        x=df_dive['dens'],
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['temp'], df_dive['saln'], df_dive['sigma0']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            'p = %{y} dBar',
                            'T = %{customdata[0]} °C',
                            's = %{customdata[1]} PSU',
                            'σ<sub>0</sub> = %{customdata[2]} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{x} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        mode='lines',
                        name='{}'.format(dive),
                        ))
                    figs['profile_saln'].add_trace(go.Scatter(
                        x=df_dive['saln'],
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['temp'], df_dive['sigma0']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            'p = %{y:.2f} dBar',
                            't = %{customdata[0]:.3f} °C',
                            's = %{x:.3f} PSU',
                            # 'ρ = %{customdata[1]} kg m<sup>−3</sup>',
                            'σ<sub>0</sub> = %{customdata[1]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        mode='lines',
                        name='{}'.format(dive),
                        ))
                    figs['profile_temp'].add_trace(go.Scatter(
                        x=df_dive['temp'],
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['saln'], df_dive['sigma0']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            'p = %{y:.2f} dBar',
                            't = %{x:.3f} °C',
                            's = %{customdata[0]:.3f} PSU',
                            # 'ρ = %{customdata[1]} kg m<sup>−3</sup>',
                            'σ<sub>0</sub> = %{customdata[1]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        mode='lines',
                        name='{}'.format(dive),
                        ))
                    figs['profile_sigma0'].add_trace(go.Scatter(
                        x=df_dive['sigma0'],
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['temp'], df_dive['saln']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            'p = %{y:.2f} dBar',
                            't = %{customdata[0]:.3f} °C',
                            's = %{customdata[1]:.3f} PSU',
                            # 'ρ = %{customdata[1]} kg m<sup>−3</sup>',
                            'σ<sub>0</sub> = %{x:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        mode='lines',
                        name='{}'.format(dive),
                        ))
                    figs['profile_ts'].add_trace(go.Scatter(
                        x=df_dive['saln'],
                        y=df_dive['temp'],
                        customdata=np.stack((df_dive['pres'], df_dive['sigma0']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            'p = %{customdata[0]:.2f} dBar',
                            't = %{y:.3f} °C',
                            's = %{x:.3f} PSU',
                            # 'ρ = %{customdata[1]} kg m<sup>−3</sup>',
                            'σ<sub>0</sub> = %{customdata[1]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        mode='lines',
                        name='{}'.format(dive),
                        ))
                        
                except: 
                    errstr = 'Failed to add traces to profile plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                i=i+1
                
            figs['profile_num_bins'].add_trace(go.Bar(
                x=df_profiles_gb.count().index.values,
                y=df_profiles_gb.count()['pres'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))   
            
            # Write plot files
            
            fig_size = dict(
                profile_num_bins = 'figWideXS'
                )
            
            for fig in figs.keys():
                
                # Append meta
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'profile',
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend =True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                
            # Cleanup
            
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_position' in locals(): del df_position
            if 'df_profiles' in locals(): del df_profiles
            if 'dive' in locals(): del dive
            if 'dt_str' in locals(): del dt_str
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'i' in locals(): del i
            if 'rng' in locals(): del rng
            gc.collect()
            
        except: 
            errstr = 'Failed to generate profile plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteContourPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate profile contour plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'profile' in float_aggregate_data or len(float_aggregate_data['profile']) < 1:
        logger.warning('No profile data for float {}'.format(float_id))
    else:
        
        PCUTOFF = 500
        
        try: 
        
            # Preprocess profile data
            
            df_profiles = float_aggregate_data['profile']
            df_profile_positions = float_aggregate_data['profile_position']
            
            df_profiles.loc[
                (df_profiles['pres'] <= -5 )|(df_profiles['pres'] >= 3000)|df_profiles['pres'].isna(),
                ['pres','dens','sigma0']] = np.nan
            df_profiles.loc[
                (df_profiles['saln'] <= 20 )|(df_profiles['saln'] >= 40)|df_profiles['saln'].isna(),
                ['saln','dens','sigma0']] = np.nan
            df_profiles.loc[
                (df_profiles['temp'] <= -5 )|(df_profiles['temp'] >= 40)|df_profiles['temp'].isna(),
                ['temp','dens','sigma0']] = np.nan
            
            
            
            df_profiles = df_profiles[~df_profiles['pres'].isna()]
            
            df_profiles = df_profiles[df_profiles['dive'] > 0] # TODO fix
            df_profile_positions = df_profile_positions[df_profile_positions['dive'] > 0]
            
            df_profiles = df_profiles.drop_duplicates(subset=['dive','pres'], ignore_index=True)
            
            # Calculate data ranges
            
            rng = dict(
                dens   = np.array([]),
                pres   = np.array([ np.round(np.percentile(df_profiles['pres'].values, [90])[0]+5, -1), 2 ]).astype(np.double),
                saln   = np.array([]),
                sigma0 = np.array([]),
                temp   = np.array([]),
                time   = np.array([]),
                )

            if rng['pres'][0] > PCUTOFF:
                rng['pres'][0] = PCUTOFF
                
            df_profiles.loc[ (df_profiles['pres'] > (rng['pres'][0] + 5))|(df_profiles['pres'] < (rng['pres'][1]-1)), ['pres','saln', 'temp','dens','sigma0'] ] = np.nan
            df_profiles = df_profiles[~df_profiles['pres'].isna()]
            
            # if rng['pres'][0] > 1000:
                # rng['pres'][0] = 1000
            
            
            # df_profiles = df_profiles[df_profiles['pres']<rng['pres'][0]]
            
            rng['dens'] = np.percentile(df_profiles['dens'][~df_profiles['dens'].isna()].values, [1, 99]).astype(np.double)
            rng['dens'] = np.round(rng['dens'] + [ -0.05, 0.05 ], 1)
            
            rng['saln'] = np.percentile(df_profiles['saln'][~df_profiles['saln'].isna()].values, [5, 98]).astype(np.double)
            rng['saln'] = np.round(rng['saln'] + [ -0.55, 0.15 ], 1)

            rng['sigma0'] = np.percentile(df_profiles['sigma0'][~df_profiles['sigma0'].isna()].values, [1, 99]).astype(np.double)
            rng['sigma0'] = np.round(rng['sigma0'] + [ -0.05, 0.05 ], 1)

            rng['temp'] = np.percentile(df_profiles['temp'][~df_profiles['temp'].isna()].values, [1, 99]).astype(np.double)
            rng['temp'] = np.round(rng['temp'] + [ -1.25, 1.25 ], 1)
            
            rng['time'] = np.array([
                df_profile_positions.iloc[0]['dt_epoch'],
                df_profile_positions.iloc[-1]['dt_epoch']
                ])
            
            rng['time'] = np.round(rng['time'] + [(rng['time'][1] - rng['time'][0]) * -0.01, (rng['time'][1] - rng['time'][0]) * +0.01] + [-3600, +3600])
            rng['time'] = [ DateTime().dt_from_epoch(x).dt_str for x in rng['time'] ]     
            
            # Generate pressure (y-axis) grid
            
            grid_pres = np.array([])
            
            if rng['pres'][0] <= 100:
                
                grid_pres = np.array([ x for x in range(int(rng['pres'][1]), int(rng['pres'][0])+1, 1) ])
                
            elif rng['pres'][0] <= 300:
                
                grid_pres = np.array([ x for x in range(int(rng['pres'][1]), 100, 1) ])
                grid_pres = np.concatenate(
                    (grid_pres, np.array([ x for x in range(100, int(rng['pres'][0])+2, 2) ])))
                
            # elif rng['pres'][0] <= 1000:
            else:
                
                grid_pres = np.array([ x for x in range(int(rng['pres'][1]), 100, 1) ])
                grid_pres = np.concatenate(
                    (grid_pres, np.array([ x for x in range(100, 300, 2) ])))
                grid_pres = np.concatenate(
                    (grid_pres, np.array([ x for x in range(300, int(rng['pres'][0])+5, 5) ])))
            
            # else:
                
                # grid_pres = np.array([ x for x in range(int(rng['pres'][1]), 100, 2) ])
                # grid_pres = np.concatenate(
                    # (grid_pres, np.array([ x for x in range(100, 300, 5) ])))
                # grid_pres = np.concatenate(
                    # (grid_pres, np.array([ x for x in range(300, int(rng['pres'][0]), 20) ])))
                
            # Initialize grids
            
            grid_time = np.array([])
                
            regrid_dive   = np.array([]).reshape(0,len(grid_pres))
            regrid_dens   = np.array([]).reshape(0,len(grid_pres))
            regrid_saln   = np.array([]).reshape(0,len(grid_pres))
            regrid_sigma0 = np.array([]).reshape(0,len(grid_pres))
            regrid_temp   = np.array([]).reshape(0,len(grid_pres))

            # Regrid data
            
            for dive_num in dives:
                
                # Check dive data
                
                if(dive_num < 1):
                    continue
                
                df_profile_pos = df_profile_positions[df_profile_positions['dive'] == dive_num]
                
                if len(df_profile_pos) < 1:
                    continue
                
                df_profile = df_profiles[df_profiles['dive'] == dive_num]
                
                if len(df_profile) < 1:
                    continue
                
                if len(df_profile[~df_profile['pres'].isna()]) < 4:
                    continue
                
                # Interpolate data
                
                df_profile = df_profile.sort_values('pres', ignore_index=True)
                
                grid_time = np.append(grid_time, df_profile_pos['dt_str'].values[0])
                
                profile_regrid_dens = interpolate.interp1d(
                    df_profile['pres'].values, 
                    df_profile['dens'].values, 
                    kind='linear', 
                    bounds_error=False)(grid_pres)
                profile_regrid_saln = interpolate.interp1d(
                    df_profile['pres'].values, 
                    df_profile['saln'].values, 
                    kind='linear', 
                    bounds_error=False)(grid_pres)
                profile_regrid_sigma0 = interpolate.interp1d(
                    df_profile['pres'].values, 
                    df_profile['sigma0'].values, 
                    kind='linear', 
                    bounds_error=False)(grid_pres)
                profile_regrid_temp = interpolate.interp1d(
                    df_profile['pres'].values, 
                    df_profile['temp'].values, 
                    kind='linear', 
                    bounds_error=False)(grid_pres)
                
                regrid_dive = np.vstack([regrid_dive, np.array([dive_num] * len(grid_pres))])
                regrid_dens = np.vstack([regrid_dens, profile_regrid_dens]) if regrid_dens.size else profile_regrid_dens
                regrid_saln = np.vstack([regrid_saln, profile_regrid_saln]) if regrid_saln.size else profile_regrid_saln
                regrid_sigma0 = np.vstack([regrid_sigma0, profile_regrid_sigma0]) if regrid_sigma0.size else profile_regrid_sigma0
                regrid_temp = np.vstack([regrid_temp, profile_regrid_temp]) if regrid_temp.size else profile_regrid_temp
         
            # Reshape regridded data
            
            regrid_dive = np.transpose(regrid_dive)
            regrid_dens = np.transpose(regrid_dens).astype(np.double)
            regrid_dens = np.ndarray.round(regrid_dens, 2)
            regrid_saln = np.transpose(regrid_saln).astype(np.double)
            regrid_saln = np.ndarray.round(regrid_saln, 2)
            regrid_sigma0 = np.transpose(regrid_sigma0).astype(np.double)
            regrid_sigma0 = np.ndarray.round(regrid_sigma0, 2)
            regrid_temp = np.transpose(regrid_temp).astype(np.double)
            regrid_temp = np.ndarray.round(regrid_temp, 2)
            
            # custom data 
            
            customdata=np.stack(
                (   [ [ '{:.0f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_dive ], 
                    [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_temp ] , 
                    [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_saln ] , 
                    [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_sigma0 ]
                ), 
                -1)
                
            # adjust range:
            rng['pres'] = [ rng['pres'][0] * 1.02, 0 ]
           
            
            # Generate figures
            
            figs=dict(
                profile_contour_dens = go.Figure(
                    data = go.Contour(
                        x=grid_time,
                        y=grid_pres,
                        z=regrid_dens,
                        zauto=False,
                        zmin=rng['dens'][0],
                        zmax=rng['dens'][1],
                        connectgaps=False,
                        contours=dict(
                            coloring ='heatmap',
                            showlabels = False, 
                            labelfont = dict( 
                                size = 10,
                                color = 'white',
                            )
                        ),
                        colorbar=dict(
                            title=dict(
                                text='In-situ Density Anomaly, ρ - 1000 [kg m<sup>−3</sup>]',
                                side='top'
                                ), 
                            #side='top',
                            thickness=20,
                            orientation='h',
                        ),
                        colorscale='dense',
                        customdata=np.stack(
                            (   [ [ '{:.0f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_dive ], 
                                [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_dens ], 
                                [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_temp ] , 
                                [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_saln ] , 
                                [ [ '{:.2f}'.format(y) if ~np.isnan(y) else '-' for y in x ] for x in regrid_sigma0 ]
                            ), 
                            -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'p = %{y:.0f} dBar',
                            't = %{customdata[2]} °C',
                            's = %{customdata[3]} PSU',
                            'σ<sub>0</sub> = %{customdata[4]} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{customdata[1]} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color='rgba(0,0,0,0.5)',
                            smoothing=0.85,
                            width=0.3,
                            ),
                    ),
                    layout=dict(
                        title = dict(text='{}: In-situ Density Contour'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                        ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                        ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                    )
                ),
                profile_contour_saln = go.Figure(
                    data = go.Contour(
                        x=grid_time,
                        y=grid_pres,
                        z=regrid_saln,
                        zauto=False,
                        zmin=rng['saln'][0],
                        zmax=rng['saln'][1],
                        connectgaps=False,
                        contours=dict(
                            coloring ='heatmap',
                            showlabels = False, 
                            labelfont = dict( 
                                size = 10,
                                color = 'white',
                            ),
                        ),
                        colorbar=dict(
                            title=dict(
                                text='Salinity [PSU]',
                                side='top'
                                ), 
                            #side='top',
                            thickness=20,
                            orientation='h',
                        ),
                        colorscale='haline',
                        customdata=customdata,
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'p = %{y:.0f} dBar',
                            't = %{customdata[1]} °C',
                            's = %{customdata[2]} PSU',
                            'σ<sub>0</sub> = %{customdata[3]} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color='rgba(0,0,0,0.5)',
                            smoothing=0.85,
                            width=0.3,
                            ),
                    ),
                    layout=dict(
                        title = dict(text='{}: Salinity Contour'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                        ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                        ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                    )
                ),
                profile_contour_sigma0 = go.Figure(
                    data = go.Contour(
                        x=grid_time,
                        y=grid_pres,
                        z=regrid_sigma0,
                        zauto=False,
                        zmin=rng['sigma0'][0],
                        zmax=rng['sigma0'][1],
                        connectgaps=False,
                        contours=dict(
                            coloring ='heatmap',
                            showlabels = False, 
                            labelfont = dict( 
                                size = 10,
                                color = 'white',
                            )
                        ),
                        colorbar=dict(
                            title=dict(
                                text='Potential Density, σ<sub>0</sub> [kg m<sup>−3</sup>]',
                                side='top'
                                ), 
                            #side='top',
                            thickness=20,
                            orientation='h',
                        ),
                        colorscale='dense',
                        customdata=customdata,
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'p = %{y:.0f} dBar',
                            't = %{customdata[1]} °C',
                            's = %{customdata[2]} PSU',
                            'σ<sub>0</sub> = %{customdata[3]} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color='rgba(0,0,0,0.5)',
                            smoothing=0.85,
                            width=0.3,
                            ),
                    ),
                    layout=dict(
                        title = dict(text='{}: Potential Density Contour'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                        ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                        ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                    )
                ),
                profile_contour_temp = go.Figure(
                    data = go.Contour(
                        x=grid_time,
                        y=grid_pres,
                        z=regrid_temp,
                        zauto=False,
                        zmin=rng['temp'][0],
                        zmax=rng['temp'][1],
                        connectgaps=False,
                        contours=dict(
                            coloring ='heatmap',
                            showlabels = False, 
                            labelfont = dict( 
                                size = 10,
                                color = 'white',
                            )
                        ),
                        colorbar=dict(
                            title=dict(
                                text='Temperature [°C]',
                                side='top'
                                ), 
                            #side='top',
                            thickness=20,
                            orientation='h',
                        ),
                        colorscale='thermal',
                        customdata=customdata,
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'p = %{y:.0f} dBar',
                            't = %{customdata[1]} °C',
                            's = %{customdata[2]} PSU',
                            'σ<sub>0</sub> = %{customdata[3]} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color='rgba(0,0,0,0.5)',
                            smoothing=0.85,
                            width=0.3,
                            ),
                    ),
                    layout=dict(
                        title = dict(text='{}: Temperature Contour'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                        ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=rng['pres'],
                        ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                    )
                ),
            )
                
            # Write plot files
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'profile_contour',
                    # wide = True,
                    size = 'figWideXL' if rng['pres'][0] > 300 else 'figWideL',
                    ts_pos = [1.00, -0.06],
                    showlegend = False,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                
            # Cleanup
            
            if 'customdata' in locals(): del customdata
            if 'df_profile' in locals(): del df_profile
            if 'df_profile_pos' in locals(): del df_profile_pos
            if 'df_profiles' in locals(): del df_profiles
            if 'dive_num' in locals(): del dive_num
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'grid_pres' in locals(): del grid_pres
            if 'grid_time' in locals(): del grid_time
            if 'profile_regrid_dens' in locals(): del profile_regrid_dens
            if 'profile_regrid_saln' in locals(): del profile_regrid_saln
            if 'profile_regrid_sigma0' in locals(): del profile_regrid_sigma0
            if 'profile_regrid_temp' in locals(): del profile_regrid_temp
            if 'regrid_dens' in locals(): del regrid_dens
            if 'regrid_dive' in locals(): del regrid_dive
            if 'regrid_saln' in locals(): del regrid_saln
            if 'regrid_sigma0' in locals(): del regrid_sigma0
            if 'regrid_temp' in locals(): del regrid_temp
            if 'rng' in locals(): del rng
            gc.collect()
            
        except:
            
            errmsg = 'Failed to generate profile contour plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errmsg)
            else:
                logger.error(errmsg)
    
    return figs_meta


def WriteAscentPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate ascent plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'ascent' in float_aggregate_data or len(float_aggregate_data['ascent']) < 1:
        logger.warning('No ascent data for float {}'.format(float_id))
    else:
        try: 
            
            # Create color range
            
            #colors = list(Color("#3333FF").range_to(Color("#FF3333"), len(dives)))
            #colors = [ 'rgb'+str(tuple([round(255*y) for y in x.rgb])) for x in colors]
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
            
            # Preprocess Data
            
            df_dives = float_aggregate_data['ascent']
            df_dives = df_dives[~df_dives['pres'].isna()]
            df_dives = df_dives[~df_dives['dt_ref_delta_sec'].isna()]

            
            
            df_dives_gb =  df_dives[df_dives['phase_code'] == 13][["dive","pres","rate"]].groupby(['dive'])
            df_dives_gb_min = df_dives_gb.min()
            df_dives_gb_max = df_dives_gb.max()
            df_dives_gb_mean = df_dives_gb.mean()
            df_dives_gb_std = df_dives_gb.std()
            
            # Calculate data ranges
            
            rng = dict(
                dt_ref_delta_min = np.array([0, np.max(df_dives['dt_ref_delta_sec'].values)/60]).astype(np.double),
                pres             = np.array([np.max(df_dives['pres'].values), 0]).astype(np.double),
                rate             = np.percentile(df_dives['rate'][~df_dives['rate'].isna()].values, [1, 99]).astype(np.double),
                dive             = np.array([0, np.max(df_dives['dive'].values)]).astype(np.double),
                )
            rng['dt_ref_delta_min'] = np.round(rng['dt_ref_delta_min']   + [ 0.00, 1.00], 0)
            rng['pres']             = np.round(rng['pres']   + [ 10.00, 0.00], 0)
            rng['rate']             = np.round(rng['rate']   + [-9.00, 9.00], 0)
            rng['dive']             = rng['dive']   + [-0.50, 0.50]
            if rng['rate'][0] > -2: 
                rng['rate'][0] = -2
            
            # Generate figures
            
            figs = dict(
                ascent = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Ascent'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dt_ref_delta_min'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                ascent_rate_v_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Ascent Rate'.format(float_id)),
                        xaxis=dict(
                            title='Vertical Rate [mBar s<sup>-1</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['rate'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                ascent_rate_v_time = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Ascent Rate'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dt_ref_delta_min'],
                            ),
                        yaxis=dict(
                            title='Vertical Rate [mBar s<sup>-1</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['rate'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                ascent_rate_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Ascent Rate'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dive'],
                            ),
                        yaxis=dict(
                            title='Vertical Rate [mBar s<sup>-1</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['rate'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),                            
                ascent_sample_count = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: No. Ascent Samples'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='Samples',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['temp'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        # legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            # ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                )
            
            # Add traces to figures
            
            figs['ascent_rate_v_pres'].add_trace(go.Scatter(
                x=[0, 0],
                y=[0, rng['pres'][0]],
                hoverinfo='none',
                line=dict(
                    color='rgb(222, 222, 222)',
                    dash='dash',
                    width=1.5,
                ),
                mode = 'lines',
                showlegend=False,
                ))
            figs['ascent_rate_v_time'].add_trace(go.Scatter(
                x=[0, rng['dt_ref_delta_min'][1]],
                y=[0, 0],
                hoverinfo='none',
                line=dict(
                    color='rgb(222, 222, 222)',
                    dash='dash',
                    width=1.5,
                ),
                mode = 'lines',
                showlegend=False,
                ))
            figs['ascent_rate_avg'].add_trace(go.Scatter(
                x=[rng['dive'][0], rng['dive'][1]] ,
                y=[0, 0],
                hoverinfo='none',
                line=dict(
                    color='rgb(222, 222, 222)',
                    dash='dash',
                    width=1.5,
                ),
                mode = 'lines',
                showlegend=False,
                ))
            
            i=0
            for dive in dives:
                try: 
            
                    df_dive = df_dives[df_dives['dive'] == dive]
                    
                    if len(df_dive) < 1:
                        logger.debug('No ascent data for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue

                    if len(df_dive) == 1:
                        logger.warning('Only one ascent point for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue

                    figs['ascent'].add_trace(go.Scatter(
                        x=np.round(df_dive['dt_ref_delta_sec'].astype(np.double)/60,2),
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['dt_str'], [ '-' if np.isnan(x) else '{:.2f}'.format(x) for x in df_dive['rate'].astype(np.double)]), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
                            '%{y:.2f} dBar',
                            '%{customdata[1]} mBar s<sup>-1</sup>',
                            '<extra></extra>',
                            ]),                               
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['ascent_rate_v_pres'].add_trace(go.Scatter(
                        x=df_dive['rate'],
                        y=df_dive['pres'],
                        customdata=df_dive['dt_str'],
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata|%Y-%m-%d %H:%M:%S}Z',
                            '%{y:.2f} dBar',
                            '%{x:.2f} mBar s<sup>-1</sup>',
                            '<extra></extra>',
                            ]),                               
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['ascent_rate_v_time'].add_trace(go.Scatter(
                        x=np.round(df_dive['dt_ref_delta_sec'].astype(np.double)/60,2),
                        y=df_dive['rate'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
                            '%{customdata[1]:.2f} dBar',
                            '%{y:.2f} mBar s<sup>-1</sup>',
                            '<extra></extra>',
                            ]),                               
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                except: 
                    errstr = 'Failed to add traces to ascent plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                i=i+1
                
            # Add traces to average figures
               
            pre = 'ascent_'   
            for x in [ 
                    ('rate', 
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.1f} mBar s<sup>-1</sup>',
                        'mean = %{y:.2f} mBar s<sup>-1</sup>',
                        'max = %{customdata[1]:.1f} mBar s<sup>-1</sup>',
                        'σ = %{customdata[2]:.2f} mBar s<sup>-1</sup>',
                        '<extra></extra>',
                        ]),
                    ) ]:
                var = x[0]
                dive, min, mean, max, std = df_dives_gb_mean.index.values, df_dives_gb_min[var], df_dives_gb_mean[var], df_dives_gb_max[var], df_dives_gb_std[var]
                hovertemplate = x[1]
                
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=1.5,
                        ),
                    mode='lines',
                    name='min./max.'
                    )) 
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123,123,123)',
                        width=1.5,
                        ),                                
                    mode='lines',
                    name='±1σ',
                    ))   
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x=dive,
                    y=mean,
                    customdata=np.stack((min, max, std), -1),
                    hovertemplate=hovertemplate,
                    line=dict(
                        color='rgb(0,0,0)',
                        width=1.2,
                        ), 
                    mode='lines',
                    name='mean',
                    ))
                    
            figs['ascent_sample_count'].add_trace(go.Bar(
                x=df_dives_gb.count().index.values,
                y=np.round((df_dives_gb.count()['pres'].values + 1) / 2),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))                               
                
            # Write plot files    
            
            fig_size = dict(
                ascent_rate_avg = 'fig2',
                ascent_sample_count = 'figWideXS',
                )
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'ascent',
                    # wide = False,
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                    
            # Cleanup
            
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_dives' in locals(): del df_dives
            if 'df_dives_gb' in locals(): del df_dives_gb
            if 'dive' in locals(): del dive
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'i' in locals(): del i
            if 'rng' in locals(): del rng
            gc.collect()                
          
        except: 
            errstr = 'Failed to generate ascent plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteDescentPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate descent plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'descent' in float_aggregate_data or len(float_aggregate_data['descent']) < 1:
        logger.warning('No descent data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Create color range
            
            #colors = list(Color("#3333FF").range_to(Color("#FF3333"), len(dives)))
            #colors = [ 'rgb'+str(tuple([round(255*y) for y in x.rgb])) for x in colors]
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
            
            # Preprocess Data
            
            df_dives = float_aggregate_data['descent']
            df_dives = df_dives[~df_dives['pres'].isna()]
            df_dives = df_dives[~df_dives['dt_ref_delta_sec'].isna()]
            
            df_dives_gb = df_dives[["dive","pres","rate"]].groupby(['dive'])
            df_dives_gb_min = df_dives_gb.min()
            df_dives_gb_max = df_dives_gb.max()
            df_dives_gb_mean = df_dives_gb.mean()
            df_dives_gb_std = df_dives_gb.std()
            
            # Calculate data ranges
            
            rng = dict(
                dt_ref_delta_min = np.array([0, np.max(df_dives['dt_ref_delta_sec'].values)/60]).astype(np.double),
                pres             = np.array([np.max(df_dives['pres'].values), 0]).astype(np.double),
                rate             = np.percentile(df_dives['rate'][~df_dives['rate'].isna()].values, [1, 99]).astype(np.double),
                dive             = np.array([0, np.max(df_dives['dive'].values)]).astype(np.double),
                )
            rng['dt_ref_delta_min'] = np.round(rng['dt_ref_delta_min']   + [ 0.00, 1.00], 0)
            rng['pres']             = np.round(rng['pres']   + [ 1.00, 0.00], 0)
            rng['rate']             = np.round(rng['rate']   + [-9.00, 9.00], 0)
            rng['dive']             = rng['dive']   + [-0.50, 0.50]
            
            if rng['rate'][1] < 2: 
                rng['rate'][1] = 2
            
            # Generate figures 
            
            figs = dict(
                descent = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Descent'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dt_ref_delta_min'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                descent_rate_v_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Descent Rate'.format(float_id)),
                        xaxis=dict(
                            title='Vertical Rate [mBar s<sup>-1</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['rate'],
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                descent_rate_v_time = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Descent Rate'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dt_ref_delta_min'],
                            ),
                        yaxis=dict(
                            title='Vertical Rate [mBar s<sup>-1</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['rate'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                descent_rate_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Descent Rate'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dive'],
                            ),
                        yaxis=dict(
                            title='Vertical Rate [mBar s<sup>-1</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['rate'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                descent_sample_count = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: No. Descent Samples'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='Samples',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['temp'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),                        
                )
            
            # Add traces to figures
            
            figs['descent_rate_v_pres'].add_trace(go.Scatter(
                x=[0, 0],
                y=[0, rng['pres'][0]],
                hoverinfo='none',
                line=dict(
                    color='rgb(222, 222, 222)',
                    dash='dash',
                    width=1.5,
                ),
                mode = 'lines',
                showlegend=False,
                ))
            figs['descent_rate_v_time'].add_trace(go.Scatter(
                x=[0, rng['dt_ref_delta_min'][1]],
                y=[0, 0],
                hoverinfo='none',
                line=dict(
                    color='rgb(222, 222, 222)',
                    dash='dash',
                    width=1.5,
                ),
                mode = 'lines',
                showlegend=False,
                ))
            figs['descent_rate_avg'].add_trace(go.Scatter(
                x=[rng['dive'][0], rng['dive'][1]],
                y=[0, 0],
                hoverinfo='none',
                line=dict(
                    color='rgb(222, 222, 222)',
                    dash='dash',
                    width=1.5,
                ),
                mode = 'lines',
                showlegend=False,
                ))
            
            i=0
            for dive in dives:
            
                try: 
                    df_dive = df_dives[df_dives['dive'] == dive]

                    if len(df_dive) < 1:
                        logger.debug('No descent data for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue

                    if len(df_dive) == 1:
                        logger.warning('Only one descent point for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue
                    
                    figs['descent'].add_trace(go.Scatter(
                        x=np.round(df_dive['dt_ref_delta_sec'].astype(np.double)/60,2),
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['dt_str'], [ '-' if np.isnan(x) else '{:.2f}'.format(x) for x in df_dive['rate'].astype(np.double)]), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
                            '%{y:.2f} dBar',
                            '%{customdata[1]} mBar s<sup>-1</sup>',
                            '<extra></extra>',
                            ]),                                
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['descent_rate_v_pres'].add_trace(go.Scatter(
                        x=df_dive['rate'],
                        y=df_dive['pres'],
                        customdata=df_dive['dt_str'],
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata|%Y-%m-%d %H:%M:%S}Z',
                            '%{y:.2f} dBar',
                            '%{x:.2f} mBar s<sup>-1</sup>',
                            '<extra></extra>',
                            ]),                               
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['descent_rate_v_time'].add_trace(go.Scatter(
                        x=np.round(df_dive['dt_ref_delta_sec'].astype(np.double)/60,2),
                        y=df_dive['rate'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
                            '%{customdata[1]:.2f} dBar',
                            '%{y:.2f} mBar s<sup>-1</sup>',
                            '<extra></extra>',
                            ]),                               
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                except: 
                    errstr = 'Failed to add traces to ascent plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                i=i+1
            
            # Add traces to average figures
               
            pre = 'descent_'   
            for x in [ 
                    ('rate', 
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.1f} mBar s<sup>-1</sup>',
                        'mean = %{y:.2f} mBar s<sup>-1</sup>',
                        'max = %{customdata[1]:.1f} mBar s<sup>-1</sup>',
                        'σ = %{customdata[2]:.2f} mBar s<sup>-1</sup>',
                        '<extra></extra>',
                        ]),
                    ) ]:
                var = x[0]
                dive, min, mean, max, std = df_dives_gb_mean.index.values, df_dives_gb_min[var], df_dives_gb_mean[var], df_dives_gb_max[var], df_dives_gb_std[var]
                hovertemplate = x[1]
                
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=1.5,
                        ),
                    mode='lines',
                    name='min./max.'
                    )) 
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123,123,123)',
                        width=1.5,
                        ),                                
                    mode='lines',
                    name='±1σ',
                    ))   
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x=dive,
                    y=mean,
                    customdata=np.stack((min, max, std), -1),
                    hovertemplate=hovertemplate,
                    line=dict(
                        color='rgb(0,0,0)',
                        width=1.2,
                        ), 
                    mode='lines',
                    name='mean',
                    ))
                
            figs['descent_sample_count'].add_trace(go.Bar(
                x=df_dives_gb.count().index.values,
                y=np.round((df_dives_gb.count()['pres'].values + 1) / 2),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                )) 
                
            # Write plot files    
            
            fig_size = dict(
                descent_rate_avg = 'fig2',
                descent_sample_count = 'figWideXS',
                )
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'descent',
                    # wide = False,
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)

                
            # Cleanup
            
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_dives' in locals(): del df_dives
            if 'df_dives_gb' in locals(): del df_dives_gb
            if 'dive' in locals(): del dive
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'i' in locals(): del i
            if 'rng' in locals(): del rng
            gc.collect()   

        except: 
            errstr = 'Failed to generate descent plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteDriftPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate drift plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'drift' in float_aggregate_data or len(float_aggregate_data['drift']) < 1:
        logger.warning('No drift data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Create color range
            
            #colors = list(Color("#3333FF").range_to(Color("#FF3333"), len(dives)))
            #colors = [ 'rgb'+str(tuple([round(255*y) for y in x.rgb])) for x in colors]
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
            
            # Preprocess profile data
                
            df_dives = float_aggregate_data['drift']

            df_dives.loc[
                (df_dives['pres'] <= -5 )|(df_dives['pres'] >= 3000)|df_dives['pres'].isna(),
                ['pres','dens','sigma0']] = np.nan
            df_dives.loc[
                (df_dives['saln'] <= 25 )|(df_dives['saln'] >= 40)|df_dives['saln'].isna(),
                ['saln','dens','sigma0']] = np.nan
            df_dives.loc[
                (df_dives['temp'] <= -5 )|(df_dives['temp'] >= 40)|df_dives['temp'].isna(),
                ['temp','dens','sigma0']] = np.nan
            
            df_dives = df_dives[~df_dives['pres'].isna()]
            
            df_dives_gb = df_dives[["dive","pres","temp","saln","dens","sigma0"]].groupby(['dive'])
            df_dives_gb_min = df_dives_gb.min()
            df_dives_gb_max = df_dives_gb.max()
            df_dives_gb_mean = df_dives_gb.mean()
            df_dives_gb_std = df_dives_gb.std()
            
            # Calculate data ranges
            
            rng = dict(
                dens   = np.percentile(df_dives['dens'][~df_dives['dens'].isna()].values, [99, 1]).astype(np.double),
                dt_ref_delta_min = np.array([
                    0,
                    np.max(df_dives['dt_ref_delta_sec'][~df_dives['dt_ref_delta_sec'].isna()].values)/60
                    ]).astype(np.double),
                # pres   = np.array([np.nanmax(df_dives['pres'].values), 0 ]),
                pres   = np.percentile(df_dives['pres'].values, [100, 0]).astype(np.double),
                saln   = np.percentile(df_dives['saln'][~df_dives['saln'].isna()].values, [2, 98]).astype(np.double),
                sigma0 = np.percentile(df_dives['sigma0'][~df_dives['sigma0'].isna()].values, [99, 1]).astype(np.double),
                temp   = np.percentile(df_dives['temp'][~df_dives['sigma0'].isna()].values, [1, 99]).astype(np.double),
                )
            rng['dt_ref_delta_min']   = np.round(rng['dt_ref_delta_min']   + [ 0, 2], 0)
            rng['dens']   = np.round(rng['dens']   + [ 1.00,-1.00], 1)
            rng['pres']   = np.round(rng['pres']   + [ 1.00,-1.00], 0)
            rng['saln']   = np.round(rng['saln']   + [-0.95, 0.95], 2)
            rng['sigma0'] = np.round(rng['sigma0'] + [ 0.50,-0.50], 1)
            rng['temp']   = np.round(rng['temp']   + [-1.00, 1.00], 1)
            
            # Generate figures 
            
            figs = dict(
                drift_dens = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Drift In-Situ Density'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            autorange=False,
                            range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='In-situ Density Anomaly, ρ - 1000 [kg m<sup>−3</sup>]',
                            autorange=False,
                            range=rng['dens']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Drift Pressure'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            autorange=False,
                            range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_saln = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Drift Salinity'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            autorange=False,
                            range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Salinity [PSU]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['saln'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_sigma0 = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Drift Potential Density'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            autorange=False,
                            range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Potential Density Anomaly, σ<sub>0</sub> [kg m<sup>−3</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['sigma0'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_temp = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Drift Temperature'.format(float_id)),
                        xaxis=dict(
                            title='Time [minutes]',
                            autorange=False,
                            range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Temperature [°C]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['temp'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_pres_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Drift Pressure'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Pressure',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_temp_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Drift Temperature'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Temperature [°C]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['temp'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Temperature',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_saln_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Drift Salinity'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Salinity [PSU]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['saln'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Salinity',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_dens_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Drift In-Situ Density'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='In-situ Density Anomaly, ρ - 1000 [kg m<sup>−3</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['dens'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_sigma0_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Drift Potential Density'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='In-situ Density Anomaly, ρ - 1000 [kg m<sup>−3</sup>]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['sigma0'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                drift_sample_count = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: No. Drift Samples'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Samples',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['temp'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                )
            
            # Add traces to figures
            
            i=0
            for dive in dives:
                try: 
            
                    df_dive = df_dives[df_dives['dive'] == dive]
                    
                    if len(df_dive) < 1:
                        logger.debug('No drift data for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue

                    if len(df_dive) == 1:
                        logger.warning('Only one drift point for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue
                    
                    figs['drift_dens'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec']/60,
                        y=df_dive['dens'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres'], df_dive['temp'], df_dive['saln'], df_dive['sigma0']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                            'p = %{customdata[1]:.2f} dBar',
                            't = %{customdata[2]:.3f} °C',
                            's = %{customdata[3]:.3f} PSU',
                            'σ<sub>0</sub> = %{customdata[4]:.3f} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{y:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['drift_pres'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec']/60,
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['temp'], df_dive['saln'], df_dive['sigma0'], df_dive['dens']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                            'p = %{y:.2f} dBar',
                            't = %{customdata[1]:.3f} °C',
                            's = %{customdata[2]:.3f} PSU',
                            'σ<sub>0</sub> = %{customdata[3]:.3f} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{customdata[4]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['drift_saln'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec']/60,
                        y=df_dive['saln'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres'], df_dive['temp'], df_dive['sigma0'], df_dive['dens']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                            'p = %{customdata[1]:.2f} dBar',
                            't = %{customdata[2]:.3f} °C',
                            's = %{y:.3f} PSU',
                            'σ<sub>0</sub> = %{customdata[3]:.3f} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{customdata[4]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['drift_sigma0'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec']/60,
                        y=df_dive['sigma0'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres'], df_dive['temp'], df_dive['saln'], df_dive['dens']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                            'p = %{customdata[1]:.2f} dBar',
                            't = %{customdata[2]:.3f} °C',
                            's = %{customdata[3]:.3f} PSU',
                            'σ<sub>0</sub> = %{y:.3f} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{customdata[4]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['drift_temp'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec']/60,
                        y=df_dive['temp'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres'], df_dive['temp'], df_dive['saln'], df_dive['dens']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                            'p = %{customdata[1]:.2f} dBar',
                            't = %{y:.3f} °C',
                            's = %{customdata[2]:.3f} PSU',
                            'σ<sub>0</sub> = %{customdata[3].3f} kg m<sup>−3</sup>',
                            'ρ - 1000 = %{customdata[4]:.3f} kg m<sup>−3</sup>',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                        
                except: 
                    errstr = 'Failed to add traces to ascent plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                i=i+1
                
            # Add traces to average figures
               
               
            pre = 'drift_'   
            for x in [ 
                    ('pres', 
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.1f} dBar',
                        'mean = %{y:.1f} dBar',
                        'max = %{customdata[1]:.1f} dBar',
                        'σ = %{customdata[2]:.1f} dBar',
                        '<extra></extra>',
                        ]),
                    ),
                    ('temp',
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} °C',
                        'mean = %{y:.3f} °C',
                        'max = %{customdata[1]:.3f} °C',
                        'σ = %{customdata[2]:.3f} °C',
                        '<extra></extra>',
                        ])                                
                    ),
                    ('saln',
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} PSU',
                        'mean = %{y:.3f} PSU',
                        'max = %{customdata[1]:.3f} PSU',
                        'σ = %{customdata[2]:.3f} PSU',
                        '<extra></extra>',
                        ])                              
                    ),
                    ('sigma0',
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} kg m<sup>−3</sup>',
                        'mean = %{y:.3f} kg m<sup>−3</sup>',
                        'max = %{customdata[1]:.3f} kg m<sup>−3</sup>',
                        'σ = %{customdata[2]:.3f} kg m<sup>−3</sup>',
                        '<extra></extra>',
                        ])                               
                    ),
                    ('dens',
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} kg m<sup>−3</sup>',
                        'mean = %{y:.3f} kg m<sup>−3</sup>',
                        'max = %{customdata[1]:.3f} kg m<sup>−3</sup>',
                        'σ = %{customdata[2]:.3f} kg m<sup>−3</sup>',
                        '<extra></extra>',
                        ])                              
                    ) ]:
                var = x[0]
                dive, min, mean, max, std = df_dives_gb_mean.index.values, df_dives_gb_min[var], df_dives_gb_mean[var], df_dives_gb_max[var], df_dives_gb_std[var]
                hovertemplate = x[1]
                
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=1.5,
                        ),
                    mode='lines',
                    name='min./max.'
                    )) 
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123,123,123)',
                        width=1.5,
                        ),                                
                    mode='lines',
                    name='±1σ',
                    ))   
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x=dive,
                    y=mean,
                    customdata=np.stack((min, max, std), -1),
                    hovertemplate=hovertemplate,
                    line=dict(
                        color='rgb(0,0,0)',
                        width=1.2,
                        ), 
                    mode='lines',
                    name='mean',
                    ))
            
            figs['drift_sample_count'].add_trace(go.Bar(
                x=df_dives_gb.count().index.values,
                y=df_dives_gb.count()['pres'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))                             
                
            # Write plot files
            
            fig_size = dict(
                drift_dens_avg = 'fig2',
                drift_pres_avg = 'fig2',
                drift_saln_avg = 'fig2',
                drift_sample_count = 'figWideXS',
                drift_sigma0_avg = 'fig2',
                drift_temp_avg = 'fig2',
                )
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'drift',
                    # wide = False,
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                
            # Cleanup
            
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_dives' in locals(): del df_dives
            if 'df_dives_gb' in locals(): del df_dives_gb
            if 'dive' in locals(): del dive
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'i' in locals(): del i
            if 'rng' in locals(): del rng
            gc.collect()     
            
        except: 
            errstr = 'Failed to generate drift plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WritePumpPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate pump plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'pump' in float_aggregate_data or len(float_aggregate_data['pump']) < 1:
        logger.warning('No pump data for float {}'.format(float_id))
    else:
    
        try: 
        
            # Create color range
            
            #colors = list(Color("#3333FF").range_to(Color("#FF3333"), len(dives)))
            #colors = [ 'rgb'+str(tuple([round(255*y) for y in x.rgb])) for x in colors]
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
            
            # Preprocess data
                
            df_dives = float_aggregate_data['pump']
            
            df_dives.loc[
                (df_dives['pres'] <= -5 )|(df_dives['pres'] >= 3000),
                ['pres', ]] = np.nan
            df_dives.loc[
                (df_dives['curr'] <= 0 )|(df_dives['curr'] >= 100)|df_dives['curr'].isna(),
                ['curr', 'curr_adj', 'energy', 'power' ]] = np.nan
            
            df_dives_gb = df_dives.groupby(['dive'])
            df_dives_gb_min = df_dives_gb.min()
            df_dives_gb_max = df_dives_gb.max()
            df_dives_gb_mean = df_dives_gb.mean()
            df_dives_gb_std = df_dives_gb.std()
            df_dives_gb_sum = df_dives_gb.sum()
            # df_dives_gb_cs = df_dives_gb.cumsum()
            
            # Calculate data ranges
            
            rng = dict(
                curr   = np.array([0, np.percentile(df_dives['curr_adj'][~df_dives['curr_adj'].isna()].values, [99])[0]]).astype(np.double),
                pres   = np.array([np.max(df_dives['pres'][~df_dives['pres'].isna()].values), -10]).astype(np.double),
                time   = np.percentile(df_dives['time'][~df_dives['time'].isna()].values, [1,99]).astype(np.double),
                volt   = np.percentile(df_dives['volt'][~df_dives['volt'].isna()].values, [0,100]).astype(np.double),
                power   = np.percentile(df_dives['power'][~df_dives['power'].isna()].values, [1,99]).astype(np.double),
                energy   = np.array([0, np.percentile(df_dives['energy'][~df_dives['energy'].isna()].values, [99])[0]]).astype(np.double),
                num = np.array([0, np.max(df_dives_gb.count()['pres']) ]),
                # volt   = np.array([0, np.max(df_dives['volt'][~df_dives['volt'].isna()].values)]),
                )
            
            rng['pres']   = np.round(rng['pres']   + [10.00, 0.00], 0)
            rng['volt']   = np.round(rng['volt']   + [-0.20, 0.20], 1)
            rng['curr']   = np.round(rng['curr']   + [ 0.00, 0.20], 2)
            rng['power']  = np.round(rng['power']  + [-1.00, 1.00], 1)
            rng['energy'] = np.round(rng['energy'] + [ 0.00,  200], 0)
            rng['num']    = np.round(rng['num']    + [-1.00, 1.00], 0)
            
            # TODO Generate Figures
            
            figs = dict(
                pump_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Pump Pressure'.format(float_id)),
                        xaxis=dict(
                            title='Pump #',
                            autorange=False,
                            range=rng['num']
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_power_v_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Pump Power vs. Pressure'.format(float_id)),
                        xaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range=[rng['pres'][1], rng['pres'][0]]
                            ),
                        yaxis=dict(
                            title='Power [W]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['power'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_volt = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Pump Battery Voltage'.format(float_id)),
                        xaxis=dict(
                            title='Pump #',
                            autorange=False,
                            range=rng['num']
                            ),
                        yaxis=dict(
                            title='Voltage [V]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['volt'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_curr = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Pump Battery Current'.format(float_id)),
                        xaxis=dict(
                            title='Pump #',
                            autorange=False,
                            range=rng['num']
                            ),
                        yaxis=dict(
                            title='Current [A]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['curr'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_vac = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Internal Vacuum vs. Pump'.format(float_id)),
                        xaxis=dict(
                            title='Pump #',
                            autorange=False,
                            range=rng['num']
                            ),
                        yaxis=dict(
                            title='Vacuum [inHg]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['curr'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_vac_v_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Internal Vac. vs. Pump Pressure'.format(float_id)),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            autorange=False,
                            range= rng['pres'],
                            ),
                        xaxis=dict(
                            title='Vacuum [inHg]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['curr'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_vac_v_time = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Internal Vacuum vs. Pump Time'.format(float_id)),
                        xaxis=dict(
                            title='Cumulative Pump Duration [seconds]',
                            # autorange=False,
                            # range= rng['pres'],
                            ),
                        yaxis=dict(
                            title='Vacuum [inHg]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['curr'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_power = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Pump Power'.format(float_id)),
                        xaxis=dict(
                            title='Pump #',
                            autorange=False,
                            range=rng['num']
                            ),
                        yaxis=dict(
                            title='Power [W]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['power'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_energy = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Pump Energy'.format(float_id)),
                        xaxis=dict(
                            title='Pump #',
                            autorange=False,
                            range=rng['num']
                            ),
                        yaxis=dict(
                            title='Energy [J]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['energy'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_vac_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Internal Vacuum (Pump)'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Vacuum [inHg]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['volt'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_volt_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Pump Battery Voltage'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Voltage [V]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['volt'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_curr_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Pump Battery Current'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Current [A]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['curr'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_power_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Pump Power'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Power [W]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['power'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_energy_sum = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Total Pump Energy per Cycle'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Energy [kJ]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['power'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_time_sum = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Total Pump Duration per Cycle'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Duration [seconds]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['power'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        bargap=0,
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_energy_v_max_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Total Pump Energy vs. Max. Pres.'.format(float_id)),
                        xaxis=dict(
                            title='Max. Pressure [dBar]',
                            # autorange=False,
                            # range=[ rng['pres'][1], rng['pres'][0] ],
                            ),
                        yaxis=dict(
                            title='Energy [kJ]',
                            # ticklabelposition='inside',
                            
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                pump_num_ops = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: No. Pump Operations per Cycle'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Pumps',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['power'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                )
            
            # TODO Add traces to figures
            
            i=0
            for dive in dives:
                
                try:
                    df_dive = df_dives[df_dives['dive'] == dive]
                    df_dive.reset_index(inplace=True)
                    
                    if len(df_dive) < 1: 
                        logger.debug('No pump data for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue
                    
                    marker_size = (df_dive['time'].values.astype(np.double) - rng['time'][0]) / rng['time'][1] * 20 + 5; 
                    marker_size = np.where(marker_size < 25, marker_size, 25)
                    marker_size = np.where(marker_size > 5, marker_size, 5)
                    marker_size = np.round(marker_size, 0)
                    
                    figs['pump_pres'].add_trace(go.Scatter(
                        # x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['pres'],
                        customdata=np.stack(
                            (
                                df_dive['time'], 
                                df_dive['volt'], 
                                df_dive['curr_adj'], 
                                df_dive['power'], 
                                df_dive['energy'], ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'p = %{y:.2f} dBar',
                            't = %{customdata[0]:.0f} s',
                            'V = %{customdata[1]:.2f} V',
                            'I = %{customdata[2]:.3f} A',
                            'P = %{customdata[3]:.2f} W',
                            'E = %{customdata[4]:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        name='{}'.format(dive),
                        opacity=0.7,
                        ))
                    
                    figs['pump_power_v_pres'].add_trace(go.Scatter(
                        x=df_dive['pres'],
                        y=df_dive['power'],
                        customdata=np.stack(
                            (
                                df_dive['time'], 
                                df_dive['volt'], 
                                df_dive['curr_adj'], 
                                df_dive['energy'],
                                df_dive.index.values
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'pump # %{customdata[4]}',
                            'p = %{y:.2f} dBar',
                            't = %{customdata[0]:.0f} s',
                            'V = %{customdata[1]:.2f} V',
                            'I = %{customdata[2]:.3f} A',
                            'P = %{y:.2f} W',
                            'E = %{customdata[3]:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=marker_size,
                            ),
                        mode='markers',
                        name='{}'.format(dive),
                        opacity=0.7,
                        ))

                    figs['pump_volt'].add_trace(go.Scatter(
                        # x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['volt'],
                        customdata=np.stack(
                            (
                                df_dive['pres'], 
                                df_dive['time'], 
                                df_dive['curr_adj'], 
                                df_dive['power'], 
                                df_dive['energy'], ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'p = %{customdata[0]:.2f} dBar',
                            't = %{customdata[1]:.0f} s',
                            'V = %{y:.2f} V',
                            'I = %{customdata[2]:.3f} A',
                            'P = %{customdata[3]:.2f} W',
                            'E = %{customdata[4]:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        name='{}'.format(dive),
                        opacity=0.7,
                        ))

                    figs['pump_curr'].add_trace(go.Scatter(
                        # x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['curr_adj'],
                        customdata=np.stack(
                            (
                                df_dive['pres'], 
                                df_dive['time'], 
                                df_dive['volt'], 
                                df_dive['power'], 
                                df_dive['energy'], ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'p = %{customdata[0]:.2f} dBar',
                            't = %{customdata[1]:.0f} s',
                            'V = %{customdata[2]:.2f} V',
                            'I = %{y:.3f} A',
                            'P = %{customdata[3]:.2f} W',
                            'E = %{customdata[4]:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        name='{}'.format(dive),
                        opacity=0.7,
                        ))

                    figs['pump_power'].add_trace(go.Scatter(
                        # x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['power'],
                        customdata=np.stack(
                            (
                                df_dive['pres'], 
                                df_dive['time'], 
                                df_dive['volt'], 
                                df_dive['curr_adj'], 
                                df_dive['energy'], ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'p = %{customdata[0]:.2f} dBar',
                            't = %{customdata[1]:.0f} s',
                            'V = %{customdata[2]:.2f} V',
                            'I = %{customdata[3]:.3f} A',
                            'P = %{y:.2f} W',
                            'E = %{customdata[4]:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        name='{}'.format(dive),
                        opacity=0.7,
                        ))

                    figs['pump_energy'].add_trace(go.Scatter(
                        # x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['energy'],
                        customdata=np.stack(
                            (
                                df_dive['pres'], 
                                df_dive['time'], 
                                df_dive['volt'], 
                                df_dive['curr_adj'], 
                                df_dive['power'], 
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'p = %{customdata[0]:.2f} dBar',
                            't = %{customdata[1]:.0f} s',
                            'V = %{customdata[2]:.2f} V',
                            'I = %{customdata[3]:.3f} A',
                            'P = %{customdata[4]:.2f} W',
                            'E = %{y:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        name='{}'.format(dive),
                        opacity=0.7,
                        ))
                    
                    figs['pump_vac'].add_trace(go.Scatter(
                        # x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['vac'],
                        customdata=np.stack(
                            (
                                df_dive['pres'], 
                                df_dive['time'], 
                                df_dive['time'].cumsum(), 
                                # df_dive['curr_adj'], 
                                # df_dive['power'], 
                                # df_dive['power'], 
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'p = %{customdata[0]:.2f} dBar',
                            't = %{customdata[1]:.0f} s',
                            'Σt = %{customdata[2]:.0f} s',
                            'vac = %{y:.2f} inHg'
                            # 't = %{customdata[1]:.0f} s',
                            # 'V = %{customdata[2]:.2f} V',
                            # 'I = %{customdata[3]:.3f} A',
                            # 'P = %{customdata[4]:.2f} W',
                            # 'E = %{y:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        name='{}'.format(dive),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        opacity=0.7,
                        ))

                    figs['pump_vac_v_pres'].add_trace(go.Scatter(
                        y=df_dive['pres'],
                        x=df_dive['vac'],
                        customdata=np.stack(
                            (
                                df_dive.index.values,
                                df_dive['time'], 
                                df_dive['time'].cumsum(), 
                                # df_dive['volt'], 
                                # df_dive['curr_adj'], 
                                # df_dive['power'], 
                                # df_dive['power'], 
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'pump # %{customdata[0]}',
                            'p = %{y:.2f} dBar',
                            't = %{customdata[1]:.0f} s',
                            'Σt = %{customdata[2]:.0f} s',
                            'vac = %{x:.2f} inHg'
                            # 't = %{customdata[1]:.0f} s',
                            # 'V = %{customdata[2]:.2f} V',
                            # 'I = %{customdata[3]:.3f} A',
                            # 'P = %{customdata[4]:.2f} W',
                            # 'E = %{y:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        name='{}'.format(dive),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        opacity=0.7,
                        ))
                    
                    figs['pump_vac_v_time'].add_trace(go.Scatter(
                        x=df_dive['time'].cumsum(),
                        y=df_dive['vac'],
                        customdata=np.stack(
                            (
                                df_dive.index.values,
                                df_dive['pres'], 
                                df_dive['time'], 
                                # df_dive['volt'], 
                                # df_dive['curr_adj'], 
                                # df_dive['power'], 
                                # df_dive['power'], 
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'pump # %{customdata[0]}',
                            'p = %{customdata[1]:.2f} dBar',
                            't = %{customdata[2]:.0f} s',
                            'Σt = %{x:.0f} s',
                            'vac = %{y:.2f} inHg'
                            # 't = %{customdata[1]:.0f} s',
                            # 'V = %{customdata[2]:.2f} V',
                            # 'I = %{customdata[3]:.3f} A',
                            # 'P = %{customdata[4]:.2f} W',
                            # 'E = %{y:.1f} J',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=1,
                            ),
                        name='{}'.format(dive),
                        marker=dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.3,
                                ),
                            size=marker_size,
                            ),
                        mode='lines+markers',
                        opacity=0.7,
                        ))

                    
                    figs['pump_energy_v_max_pres'].add_trace(go.Scatter(
                        x=[df_dives_gb_max['pres'][dive]],
                        y=[df_dives_gb_sum['energy'][dive]/1000],
                        customdata=np.stack(
                            (
                                [ df_dives_gb.count()['pres'][dive] ], 
                                [ df_dives_gb_sum['time'][dive] ], 
                                ),
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            'max. p = %{x:.2f} dBar',
                            'ΣE = %{y:.3f} kJ',
                            'no. pumps = %{customdata[0]:.0f}',
                            'Σt = %{customdata[1]:.0f} s',
                            '<extra></extra>',
                            ]),
                        name='{}'.format(dive),
                        opacity = 0.6,
                        marker = dict(
                            color=colors[i],
                            line=dict(
                                color='rgb(0, 0, 0)',
                                width = 0.5,
                                ),
                            size=18, 
                            ),
                        mode='markers',
                        ))
                    
                except: 
                    errstr = 'Failed to add traces to pump plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                
                i=i+1
                
            
            # Add traces to average figures
            
            pre='pump_'
            for x in [ 
                    ('vac', 
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.2f} inHg',
                        'mean = %{y:.3f} inHg',
                        'max = %{customdata[1]:.2f} inHg',
                        'σ = %{customdata[2]:.3f} inHg',
                        '<extra></extra>',
                        ])
                    ),
                    ('volt',
                        '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} V',
                        'mean = %{y:.3f} V',
                        'max = %{customdata[1]:.3f} V',
                        'σ = %{customdata[2]:.3f} V',
                        '<extra></extra>',
                        ])                                
                    ),
                    ('curr',
                        '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} A',
                        'mean = %{y:.3f} A',
                        'max = %{customdata[1]:.3f} A',
                        'σ = %{customdata[2]:.3f} A',
                        '<extra></extra>',
                        ])                          
                    ),
                    ('power',
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} W',
                        'mean = %{y:.3f} W',
                        'max = %{customdata[1]:.3f} W',
                        'σ = %{customdata[2]:.3f} W',
                        '<extra></extra>',
                        ]),                            
                    ) ]:
                var = x[0]
                
                if var == 'curr':
                    dive, min, mean, max, std = df_dives_gb_mean.index.values, df_dives_gb_min['curr_adj'], df_dives_gb_mean['curr_adj'], df_dives_gb_max['curr_adj'], df_dives_gb_std['curr_adj']
                else: 
                    dive, min, mean, max, std = df_dives_gb_mean.index.values, df_dives_gb_min[var], df_dives_gb_mean[var], df_dives_gb_max[var], df_dives_gb_std[var]
                hovertemplate = x[1]
                
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=1.5,
                        ),
                    mode='lines',
                    name='min./max.'
                    )) 
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123,123,123)',
                        width=1.5,
                        ),                                
                    mode='lines',
                    name='±1σ',
                    ))   
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x=dive,
                    y=mean,
                    customdata=np.stack((min, max, std), -1),
                    hovertemplate=hovertemplate,
                    line=dict(
                        color='rgb(0,0,0)',
                        width=1.2,
                        ), 
                    mode='lines',
                    name='mean',
                    ))
            
            figs['pump_energy_sum'].add_trace(go.Bar(
                x=df_dives_gb_sum.index.values,
                y=df_dives_gb_sum['energy']/1000,
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.3f} kJ',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))                    
                
            figs['pump_time_sum'].add_trace(go.Bar(
                x=df_dives_gb_sum.index.values,
                y=df_dives_gb_sum['time'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f} s',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))                    

            figs['pump_num_ops'].add_trace(go.Bar(
                x=df_dives_gb.count().index.values,
                y=df_dives_gb.count()['pres'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))                             
                
            # Write plot files
            
            fig_size = dict(
                pump_volt_avg = 'fig2',
                pump_curr_avg = 'fig2',
                pump_power_avg = 'fig2',
                pump_vac_avg = 'fig2',
                pump_num_ops = 'figWideXS',
                pump_time_sum = 'figWideXS',
                pump_energy_sum = 'figWideXS',
                )
                
            # Write plot files
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'pump',
                    # wide = False,
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                
            # TODO Cleanup
            
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_dives' in locals(): del df_dives
            if 'df_dives_gb' in locals(): del df_dives_gb
            if 'dive' in locals(): del dive
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'figs' in locals(): del figs
            if 'i' in locals(): del i
            if 'rng' in locals(): del rng
            gc.collect()   
            
        except: 
            errstr = 'Failed generate pump plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteSurfacePlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the aggregate surface plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if not 'surface' in float_aggregate_data or len(float_aggregate_data['surface']) < 1:
        logger.warning('No surface data for float {}'.format(float_id))
    else:
    
        try: 
            
            # Create color range
            
            #colors = list(Color("#3333FF").range_to(Color("#FF3333"), len(dives)))
            #colors = [ 'rgb'+str(tuple([round(255*y) for y in x.rgb])) for x in colors]
            colors = [colorsys.hsv_to_rgb(x*.666/(len(dives)-1),.8,1) for x in range(len(dives))][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[len(dives)-1] = 'rgb(0,0,0)'
            
            # Preprocess data
                
            df_dives = float_aggregate_data['surface']

            df_dives.loc[
                (df_dives['pres'] <= -5 )|(df_dives['pres'] >= 600)|df_dives['pres'].isna(),
                ['pres', 'temp', 'dt_ref_delta_sec' ]] = np.nan
            df_dives.loc[
                (df_dives['temp'] <= -5 )|(df_dives['temp'] >= 40),
                ['temp']] = np.nan
            
            df_dives = df_dives[~df_dives['pres'].isna()]
            df_dives = df_dives[~df_dives['dt_ref_delta_sec'].isna()]
            df_dives = df_dives[df_dives['dt_ref_delta_sec'] < 30000]
            
            df_dives_gb = df_dives[["dive","pres","temp"]].groupby(['dive'])
            df_dives_gb_min = df_dives_gb.min()
            df_dives_gb_max = df_dives_gb.max()
            df_dives_gb_mean = df_dives_gb.mean()
            df_dives_gb_std = df_dives_gb.std()
            
            # Calculate data ranges
            
            rng = dict(
                dt_ref_delta_sec = np.array([
                    0,
                    np.max(df_dives['dt_ref_delta_sec'][~df_dives['dt_ref_delta_sec'].isna()].values)
                    ]).astype(np.double),
                pres   = np.percentile(df_dives['pres'].values, [99, 1]).astype(np.double),
                temp   = np.percentile(df_dives['temp'][~df_dives['temp'].isna()].values, [1, 99]).astype(np.double),
                )
            rng['pres']   = np.round(rng['pres']   + [ 1.00,-1.00], 1)
            rng['temp']   = np.round(rng['temp']   + [-2.00, 2.00], 1) 
            
            # Generate figures 
            
            figs = dict(
                surface_pres = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Surface Pressure'.format(float_id)),
                        xaxis=dict(
                            title='Time [seconds]',
                            autorange=False,
                            range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                surface_temp = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Surface Temperature'.format(float_id)),
                        xaxis=dict(
                            title='Time [seconds]',
                            autorange=False,
                            range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Temperature [°C]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['temp'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                surface_pres_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Surface Pressure'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Pressure [dBar]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['pres'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        # legend=dict(
                        #     title='Dive',
                        #     # font=dict(size=10)
                        #     ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                surface_temp_avg = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: Average Surface Temperature'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Temperature [°C]',
                            # ticklabelposition='inside',
                            autorange=False,
                            range=rng['temp'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        # legend=dict(
                        #     title='Dive',
                        #     # font=dict(size=10)
                        #     ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                surface_sample_count = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title = dict(text='{}: No. Surface Samples'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_min']
                            ),
                        yaxis=dict(
                            title='Samples',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=rng['temp'],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),

                )
            
            # Add traces to figures
            
            i=0
            for dive in dives:
                
                try: 
                    df_dive = df_dives[df_dives['dive'] == dive]
                    
                    if len(df_dive) < 1:
                        logger.debug('No surface data for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue
                    
                    if len(df_dive) == 1:
                        logger.debug('Only one surface point for float {}, dive {}'.format(float_id, dive))
                        i=i+1
                        continue
                    
                    figs['surface_pres'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['pres'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['temp']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
                            'p = %{y:.2f} dBar',
                            't = %{customdata[1]:.3f} °C',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                    figs['surface_temp'].add_trace(go.Scatter(
                        x=df_dive['dt_ref_delta_sec'],
                        y=df_dive['temp'],
                        customdata=np.stack((df_dive['dt_str'], df_dive['pres'], df_dive['temp']), -1),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
                            'p = %{customdata[1]:.2f} dBar',
                            't = %{y:.3f} °C',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color=colors[i],
                            width=2 if dive == dives[-1] else 1.2,
                            ),
                        name='{}'.format(dive),
                        mode='lines'
                        ))
                        
                except: 
                    errstr = 'Failed to add traces to surface pump plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                    
                i=i+1
                
            pre = 'surface_'  
            for x in [ 
                    ('pres', 
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.2f} dBar',
                        'mean = %{y:.3f} dBar',
                        'max = %{customdata[1]:.2f} dBar',
                        'σ = %{customdata[2]:.3f} dBar',
                        '<extra></extra>',
                        ]),
                    ),
                    ('temp',
                    '<br>'.join([
                        'Dive <b>%{x}</b>',
                        'min = %{customdata[0]:.3f} °C',
                        'mean = %{y:.3f} °C',
                        'max = %{customdata[1]:.3f} °C',
                        'σ = %{customdata[2]:.3f} °C',
                        '<extra></extra>',
                        ])                                
                    ) ]:
                var = x[0]
                dive, min, mean, max, std = df_dives_gb_mean.index.values, df_dives_gb_min[var], df_dives_gb_mean[var], df_dives_gb_max[var], df_dives_gb_std[var]
                hovertemplate = x[1]
                
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=1.5,
                        ),
                    mode='lines',
                    name='min./max.'
                    )) 
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                    y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(123,123,123)',
                        width=1.5,
                        ),                                
                    mode='lines',
                    name='±1σ',
                    ))   
                figs[pre+var+'_avg'].add_trace(go.Scatter(
                    x=dive,
                    y=mean,
                    customdata=np.stack((min, max, std), -1),
                    hovertemplate=hovertemplate,
                    line=dict(
                        color='rgb(0,0,0)',
                        width=1.2,
                        ), 
                    mode='lines',
                    name='mean',
                    ))
                
            figs['surface_sample_count'].add_trace(go.Bar(
                x=df_dives_gb.count().index.values,
                y=df_dives_gb.count()['pres'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y:.0f}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                showlegend=False,
                ))                         
            
            # Write plot files
            
            fig_size = dict(
                surface_temp_avg = 'fig2',
                surface_pres_avg = 'fig2',
                surface_sample_count = 'figWideXS'
                )
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'surface',
                    # wide = False,
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                
            # Cleanup
            
            if 'i' in locals(): del i
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'fig_size' in locals(): del fig_size
            if 'figs' in locals(): del figs
            if 'dive' in locals(): del dive
            if 'colors' in locals(): del colors
            if 'df_dive' in locals(): del df_dive
            if 'df_dives' in locals(): del df_dives
            if 'rng' in locals(): del rng
            gc.collect()
            
        except: 
            errstr = 'Failed to generate surface plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteSatcomPlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the Iridium and GPS plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if (not 'sbd' in float_aggregate_data or len(float_aggregate_data['sbd']) < 1) and (not 'gps' in float_aggregate_data or len(float_aggregate_data['gps']) < 1):
        logger.warning('No satcom data for float {}'.format(float_id))
    else: 
    
        try: 
        
            # Preprocess data
            
            df_sbd = float_aggregate_data['sbd']
            df_gps = float_aggregate_data['gps']
            df_iri = float_aggregate_data['iridium_position']
            
            # df_sbd = df_sbd[df_sbd['dive'] >= 0]
            # df_gps = df_gps[df_gps['dive'] >= 0]
            
            df_iri = df_iri[~df_iri['dive'].isna()]
            
            df_sbd_gb = df_sbd[['dive','size_b','cep_km']].groupby(['dive'])
            df_sbd_gb_min = df_sbd_gb.min()
            df_sbd_gb_max = df_sbd_gb.max()
            df_sbd_gb_mean = df_sbd_gb.mean()
            df_sbd_gb_std = df_sbd_gb.std()
            df_sbd_gb_sum = df_sbd_gb.sum()
            
            df_gps_gb = df_gps[['dive','cep_km']].groupby(['dive'])
            # df_gps_gb_min = df_gps_gb.min()
            # df_gps_gb_max = df_gps_gb.max()
            # df_gps_gb_mean = df_gps_gb.mean()
            # df_gps_gb_std = df_gps_gb.std()
            # df_gps_gb_sum = df_gps_gb.sum()

            df_gps.loc[
               (df_gps['snr_min'] == 255)|(df_gps['snr_avg'] == 0)|(df_gps['snr_max'] == 0),
               ['snr_min','snr_avg','snr_max', ]] = np.nan                        

            df_gps.loc[
               (df_gps['snr_min'] == 0),
               ['snr_min',]] = np.nan                        


            # Calculate ranges
           
            # rng = dict(
                # time = np.array([
                    # DateTime().dt_from_epoch(
                        # np.min([
                            # np.min(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                            # np.min(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                            # ]) - 86400*5
                        # ).dt_str,
                    # DateTime().dt_from_epoch(
                        # np.max([
                            # np.max(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 0,
                            # np.max(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 0,
                            # ]) + 86400*5
                        # ).dt_str,
                    # ]),
                # dive = np.array([.5, dives[-1]+0.5])
                # ) 

            rng = dict(
                time = np.array([
                    np.min([
                        np.min(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                        np.min(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 9999999999,
                        ]),
                    np.max([
                        np.max(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_sbd['dt_epoch'][df_sbd['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 0,
                        np.max(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)].values) if len(df_gps['dt_epoch'][df_gps['dt_epoch'] > (float_aggregate_data['summary']['startup']['sbd'].dt_epoch + 3600)]) else 0,
                        ]),
                    ]),
                dive = np.array([-0.5, dives[-1]+0.5])
                )                           
            
            rng['time'] = np.round(rng['time'] + [(rng['time'][1] - rng['time'][0]) * -0.03, (rng['time'][1] - rng['time'][0]) * +0.03] + [-21600, +21600])
            rng['time'] = [ DateTime().dt_from_epoch(x).dt_str for x in rng['time'] ] 
            
            
            # Generate Figures
            
            figs = dict(
                satcom = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Satcom'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time']
                            ),
                        yaxis=dict(
                            title='',
                            autorange=False,
                            range=[ 
                                dateutil.parser.parse('2021-02-11 23:00:00'),
                                dateutil.parser.parse('2021-02-13 01:00:00'),
                                ],
                            type='date',
                            tickformat='%H:%M',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                satcom_gps_ttf = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: GPS Time-to-fix (TTF)'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='TTF [seconds]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=[ 
                            #     dateutil.parser.parse('2021-02-11 23:00:00'),
                            #     dateutil.parser.parse('2021-02-13 01:00:00'),
                            #     ],
                            # type='date',
                            # tickformat='%H:%M',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                satcom_gps_cno = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: GPS Carrier-to-Noise (CNO) Ratio'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='CNO [dBHz]',
                            # ticklabelposition='inside',
                            # autorange=False,
                            # range=[ 
                            #     dateutil.parser.parse('2021-02-11 23:00:00'),
                            #     dateutil.parser.parse('2021-02-13 01:00:00'),
                            #     ],
                            # type='date',
                            # tickformat='%H:%M',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                 satcom_gps_nsat = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: GPS No. Satellites'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='No. Satellites',
                            autorange=False,
                            range=[ -1, np.max(df_gps['nsat'].values)+1 ],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                 satcom_gps_hdop = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: GPS Horiz. Dilution of Precision'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='HDOP',
                            autorange=False,
                            range=[ 0, np.max(df_gps['hdop'].values)+1 ],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                satcom_sbd_count = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: No. SBD Messages'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='No. Messages',
                            #autorange=False,
                            #range=[ 0, np.max(df_gps['hdop'].values)+1 ],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                satcom_sbd_data_size = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: SBD Data Size'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='Size [bytes]',
                            #autorange=False,
                            #range=[ 0, np.max(df_gps['hdop'].values)+1 ],
                            ),
                        bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                satcom_sbd_data_size_avg = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: SBD Message Average Size'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='Size [bytes]',
                            autorange=False,
                            range=[ 0, 350 ],
                            ),
                        #bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),  
                satcom_iridium_fix_cep_avg = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Iridium Fix Uncertainty'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            autorange=False,
                            range=rng['dive']
                            ),
                        yaxis=dict(
                            title='Uncertainty [km]',
                            autorange=False,
                            # range=[ 0, np.percentile(df_sbd['cep_km'].values, [99])[0]+100 ],
                            range=[ -0.5, 3.5 ],
                            type='log',
                            ),
                        #bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),  
                satcom_gps_fix_cep = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: GPS Fix Uncertainty Est.'.format(float_id)),
                        xaxis=dict(
                            title='Dive',
                            # autorange=False,
                            # range=rng['dt_ref_delta_sec']
                            ),
                        yaxis=dict(
                            title='Uncertainty [m]',
                            #autorange=False,
                            #range=[ -10, np.max(df_gps['cep_km'].values)*1000 + 100],
                            ),
                        #bargap=0,
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),  
                )
            
            # Add traces to figures
            
            figs['satcom'].add_trace(go.Scatter(
                x = df_sbd['dt_str'][df_sbd['mtmsn'] == 0],
                y = [ dateutil.parser.parse('2021-02-12 '+x[11:19]) for x in df_sbd['dt_str'][df_sbd['mtmsn'] == 0].values ],
                customdata=np.stack(
                    (
                        df_sbd['dive'][df_sbd['mtmsn'] == 0],
                        df_sbd['momsn'][df_sbd['mtmsn'] == 0],
                        df_sbd['pid'][df_sbd['mtmsn'] == 0],
                        df_sbd['size_b'][df_sbd['mtmsn'] == 0],
                        df_sbd['lat'][df_sbd['mtmsn'] == 0],
                        df_sbd['lon'][df_sbd['mtmsn'] == 0],
                        df_sbd['cep_km'][df_sbd['mtmsn'] == 0],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata[0]}</b>',
                    '%{x|%Y-%m-%d %H:%M:%S}Z',
                    '%{customdata[4]:.3f}°N %{customdata[5]:.3f}°E ± %{customdata[6]} km',
                    'MOMSN = %{customdata[1]}',
                    'PID = %{customdata[2]}',
                    'size = %{customdata[3]} bytes',
                    '<extra></extra>',
                    ]),
                name='Iridium TX',
                marker=dict(
                    color='rgba(255,69,69,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=16,
                    ),
                mode='markers',
                # opacity=0.5,
                ))
            figs['satcom'].add_trace(go.Scatter(
                x = df_sbd['dt_str'][df_sbd['mtmsn'] != 0],
                y = [ dateutil.parser.parse('2021-02-12 '+x[11:19]) for x in df_sbd['dt_str'][df_sbd['mtmsn'] != 0].values ],
                customdata=np.stack(
                    (
                        df_sbd['dive'][df_sbd['mtmsn'] != 0],
                        df_sbd['momsn'][df_sbd['mtmsn'] != 0],
                        df_sbd['pid'][df_sbd['mtmsn'] != 0],
                        df_sbd['size_b'][df_sbd['mtmsn'] != 0],
                        df_sbd['lat'][df_sbd['mtmsn'] != 0],
                        df_sbd['lon'][df_sbd['mtmsn'] != 0],
                        df_sbd['cep_km'][df_sbd['mtmsn'] != 0],
                        df_sbd['mtmsn'][df_sbd['mtmsn'] != 0],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata[0]}</b>',
                    '%{x|%Y-%m-%d %H:%M:%S}Z',
                    '%{customdata[4]:.3f}°N %{customdata[5]:.3f}°E ± %{customdata[6]} km',
                    'MOMSN = %{customdata[1]}',
                    'MTMSN = %{customdata[7]}',
                    'PID = %{customdata[2]}',
                    'size = %{customdata[3]} bytes',
                    '<extra></extra>',
                    ]),
                name='Iridium TX+RX',
                marker=dict(
                    color='rgba(69,69,255,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=16,
                    ),
                mode='markers',
                # opacity=0.5,
                ))
            figs['satcom'].add_trace(go.Scatter(
                x = df_gps['dt_str'],
                y = [ dateutil.parser.parse('2021-02-12 '+x[11:16]) for x in df_gps['dt_str'].values ],
                customdata=np.stack(
                    (
                        df_gps['dive'],
                        df_gps['lat'],
                        df_gps['lon'],
                        df_gps['ttf_sec'],
                        df_gps['nsat'],
                        df_gps['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{customdata[0]}</b>',
                    '%{x|%Y-%m-%d %H:%M}Z',
                    '%{customdata[1]:.4f}°N %{customdata[2]:.4f}°E',
                    'TTF = %{customdata[3]} s',
                    'no. sat = %{customdata[4]}',
                    'HDOP = %{customdata[5]:.1f}',
                    'CNO min. = %{customdata[6]} dBHz',
                    'CNO mean = %{customdata[7]} dBHz',
                    'CNO max. = %{customdata[8]} dBHz',
                    '<extra></extra>',
                    ]),
                name='GPS',
                marker=dict(
                    color='rgb(69,255,69)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    ),
                mode='markers',
                # opacity=0.7,
                ))
                
            # add traces -- satcom ttf 
                
            idx = df_gps['idx'] == 1   
            figs['satcom_gps_ttf'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'].values - 0.25,
                y = df_gps[idx]['ttf_sec'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['nsat'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{y} s',
                    'no. sat = %{customdata[1]}',
                    'HDOP = %{customdata[2]:.1f}',
                    'CNO min. = %{customdata[3]} dBHz',
                    'CNO mean = %{customdata[4]} dBHz',
                    'CNO max. = %{customdata[5]} dBHz',
                    '<extra></extra>',
                    ]),
                name='before descent',
                marker=dict(
                    color='rgba(255,69,69,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='x',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
            idx = (df_gps['idx'] == 0) | (df_gps['idx'] == 2)   
            figs['satcom_gps_ttf'].add_trace(go.Scatter(
                x = df_gps[idx]['dive']+0.25,
                y = df_gps[idx]['ttf_sec'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['nsat'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{y} s',
                    'no. sat = %{customdata[1]}',
                    'HDOP = %{customdata[2]:.1f}',
                    'CNO min. = %{customdata[3]} dBHz',
                    'CNO mean = %{customdata[4]} dBHz',
                    'CNO max. = %{customdata[5]} dBHz',
                    '<extra></extra>',
                    ]),
                name='after ascent',
                marker=dict(
                    color='rgba(69,69,255,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='cross',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
                
            # add traces -- satcom CNO 
                
            idx = df_gps['idx'] == 1
            figs['satcom_gps_cno'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] - 0.25,
                y = df_gps[idx]['snr_avg'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['nsat'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{customdata[2]}',
                    'HDOP = %{customdata[3]:.1f}',
                    'CNO max. = %{customdata[5]} dBHz',
                    'CNO mean = %{y} dBHz',
                    'CNO min. = %{customdata[4]} dBHz',
                    '<extra></extra>',
                    ]),
                name='before descent',
                marker=dict(
                    color='rgba(255,69,69,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='x',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
            figs['satcom_gps_cno'].add_trace(go.Scatter(
                # x = np.hstack((df_gps[idx]['dive'], np.nan, df_gps[idx]['dive'])),
                # y = np.hstack((df_gps[idx]['snr_min'], np.nan, df_gps[idx]['snr_max'])),
                x = np.vstack((df_gps[idx]['dive'], df_gps[idx]['dive'], df_gps[idx]['dive'], [np.nan]*len(df_gps[idx]))).flatten('F') - 0.25,
                y = np.vstack((df_gps[idx]['snr_min'], df_gps[idx]['snr_avg'], df_gps[idx]['snr_max'], [np.nan]*len(df_gps[idx]))).flatten('F'),
                hoverinfo='none',
                line=dict(
                    color='rgb(255,69,69)',
                    dash='dot',
                    width=0.5,
                    ),                        
                marker=dict(
                    line=dict(
                        color='rgb(255,69,69)',
                        width=1.0,
                        ),
                    size=6,
                    symbol='x-thin',
                ),
                mode='lines+markers',
                name='min./max.',
                # opacity=0.7,
                ))
            idx = (df_gps['idx'] == 0) | (df_gps['idx'] == 2)  
            figs['satcom_gps_cno'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] + 0.25,
                y = df_gps[idx]['snr_avg'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['nsat'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{customdata[2]}',
                    'HDOP = %{customdata[3]:.1f}',
                    'CNO max. = %{customdata[5]} dBHz',
                    'CNO mean = %{y} dBHz',
                    'CNO min. = %{customdata[4]} dBHz',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgba(69,69,255,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=6,
                    symbol='cross',
                    ),
                mode='markers',
                name='after ascent',
                # opacity=0.7,
                ))
            figs['satcom_gps_cno'].add_trace(go.Scatter(
                # x = np.hstack((df_gps[idx]['dive'], np.nan, df_gps[idx]['dive'])),
                # y = np.hstack((df_gps[idx]['snr_min'], np.nan, df_gps[idx]['snr_max'])),
                x = np.vstack((df_gps[idx]['dive'], df_gps[idx]['dive'], df_gps[idx]['dive'], [np.nan]*len(df_gps[idx]))).flatten('F') + 0.25,
                y = np.vstack((df_gps[idx]['snr_min'], df_gps[idx]['snr_avg'], df_gps[idx]['snr_max'], [np.nan]*len(df_gps[idx]))).flatten('F'),
                hoverinfo='none',
                line=dict(
                    color='rgb(69,69,255)',
                    dash='dot',
                    width=0.5,
                    ),
                marker=dict(
                    line=dict(
                        color='rgb(69,69,255)',
                        width=1.0,
                        ),
                    size=6,
                    symbol='cross-thin',
                ),    
                mode='lines+markers',
                name='min./max.',
                # opacity=0.7,
                ))
                
            # add traces -- satcom nsat 
                
            idx = df_gps['idx'] == 1   
            figs['satcom_gps_nsat'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] - 0.25,
                y = df_gps[idx]['nsat'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{y}',
                    'HDOP = %{customdata[2]:.1f}',
                    'CNO min. = %{customdata[3]} dBHz',
                    'CNO mean = %{customdata[4]} dBHz',
                    'CNO max. = %{customdata[5]} dBHz',
                    '<extra></extra>',
                    ]),
                name='before descent',
                marker=dict(
                    color='rgba(255,69,69,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='x',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
            idx = (df_gps['idx'] == 0) | (df_gps['idx'] == 2)    
            figs['satcom_gps_nsat'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] + 0.25,
                y = df_gps[idx]['nsat'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{y}',
                    'HDOP = %{customdata[2]:.1f}',
                    'CNO min. = %{customdata[3]} dBHz',
                    'CNO mean = %{customdata[4]} dBHz',
                    'CNO max. = %{customdata[5]} dBHz',
                    '<extra></extra>',
                    ]),
                name='after ascent',
                marker=dict(
                    color='rgba(69,69,255,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='cross',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
                
            # add traces -- hdop
                
            idx = df_gps['idx'] == 1   
            figs['satcom_gps_hdop'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] - 0.25,
                y = df_gps[idx]['hdop'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['nsat'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{customdata[2]}',
                    'HDOP = %{y:.1f}',
                    'CNO min. = %{customdata[3]} dBHz',
                    'CNO mean = %{customdata[4]} dBHz',
                    'CNO max. = %{customdata[5]} dBHz',
                    '<extra></extra>',
                    ]),
                name='before descent',
                marker=dict(
                    color='rgba(255,69,69,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='x',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
            idx = (df_gps['idx'] == 0) | (df_gps['idx'] == 2)   
            figs['satcom_gps_hdop'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] + 0.25,
                y = df_gps[idx]['hdop'],
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['nsat'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{customdata[2]}',
                    'HDOP = %{y:.1f}',
                    'CNO min. = %{customdata[3]} dBHz',
                    'CNO mean = %{customdata[4]} dBHz',
                    'CNO max. = %{customdata[5]} dBHz',
                    '<extra></extra>',
                    ]),
                name='after ascent',
                marker=dict(
                    color='rgba(69,69,255,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='cross',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
                
            # add traces -- sbd message count
            
            figs['satcom_sbd_count'].add_trace(go.Bar(
                x = df_sbd_gb.count().index.values,
                y = df_sbd_gb.count()['size_b'],
                customdata=np.stack(
                    (
                        df_sbd_gb_sum['size_b'],
                       ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{y} SBD messages',
                    '%{customdata} bytes',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                #mode='markers',
                showlegend=False,
                # opacity=0.7,
                ))
                
            # add traces -- sbd message size
            
            figs['satcom_sbd_data_size'].add_trace(go.Bar(
                x = df_sbd_gb_sum.index.values,
                y = df_sbd_gb_sum['size_b'],
                customdata=np.stack(
                    (
                        df_sbd_gb.count()['size_b'],
                       ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    '%{customdata} SBD messages',
                    '%{y} bytes',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    color='rgb(220,220,220)',
                    line=dict(
                        color='rgb(188, 188, 188)',
                        width=0.5,
                        ),
                    ),
                #mode='markers',
                showlegend=False,
                # opacity=0.7,
                ))
                
            # figs['satcom_sbd_data_size_avg'].add_trace(go.Scatter(
                # x=df_sbd_gb_mean.index.values,
                # y=df_sbd_gb_mean['size_b'],
                # customdata=np.stack(
                    # (
                        # df_sbd_gb_min['size_b'], 
                        # df_sbd_gb_max['size_b'],
                        # df_sbd_gb_std['size_b'],
                        # ), 
                    # -1
                    # ),
                # hovertemplate='<br>'.join([
                    # 'Dive <b>%{x}</b>',
                    # 'min = %{customdata[0]:.0f} bytes',
                    # 'mean = %{y:.1f} bytes',
                    # 'max = %{customdata[1]:.0f} bytes',
                    # 'σ = %{customdata[2]:.1f} bytes',
                    # '<extra></extra>',
                    # ]),
                # line=dict(
                    # color='rgb(0,0,0)',
                    # width=1,
                    # ),
                # mode='lines',
                # name='mean',
                # ))
            # idx=df_sbd_gb.count()['size_b'] > 1
            # figs['satcom_sbd_data_size_avg'].add_trace(go.Scatter(
                # x=df_sbd_gb_mean[idx].index.tolist() + df_sbd_gb_mean[idx].index.tolist()[::-1],
                # y= [ x if x < 340 else 340 for x in (df_sbd_gb_mean[idx]['size_b'] + df_sbd_gb_std[idx]['size_b']) ]
                # + [ x if x > 0 else 0 for x in (df_sbd_gb_mean[idx]['size_b'] - df_sbd_gb_std[idx]['size_b']) ][::-1],
                # fill='toself',
                # fillcolor='rgba(80,80,80,0.2)',
                # hoverinfo='none',
                # line_color='rgba(255,255,255,0)',
                # name='±1σ'
                # ))
            # figs['satcom_sbd_data_size_avg'].add_trace(go.Scatter(
                # x=np.hstack((df_sbd_gb_mean.index, np.nan, df_sbd_gb_mean.index)),
                # y=np.hstack((df_sbd_gb_min['size_b'].values, np.nan, df_sbd_gb_max['size_b'].values)),
                # hoverinfo='none',
                # line=dict(
                    # color='rgb(123,123,123)',
                    # width=1.0,
                    # dash='dot',
                    # ),
                # mode='lines',
                # name='min./max.',
                # ))

            # add traces -- iridium fix cep
                
            
            # figs['satcom_iridium_fix_cep_avg'].add_trace(go.Scatter(
                # x=df_sbd_gb_mean.index.values,
                # y=df_sbd_gb_mean['cep_km'],
                # customdata=np.stack(
                    # (
                        # df_sbd_gb_min['cep_km'], 
                        # df_sbd_gb_max['cep_km'],
                        # df_sbd_gb_std['cep_km'],
                        # ), 
                    # -1
                    # ),
                # hovertemplate='<br>'.join([
                    # 'Dive <b>%{x}</b>',
                    # 'min = %{customdata[0]:.0f} km',
                    # 'mean = %{y:.1f} km',
                    # 'max = %{customdata[1]:.0f} km',
                    # 'σ = %{customdata[2]:.1f} km',
                    # '<extra></extra>',
                    # ]),
                # line=dict(
                    # color='rgb(0,0,0)',
                    # width=1,
                    # ),
                # mode='lines',
                # name='mean',
                # ))
            # figs['satcom_iridium_fix_cep_avg'].add_trace(go.Scatter(
                # x=df_sbd_gb_mean.index.tolist() + df_sbd_gb_mean.index.tolist()[::-1],
                # y= (df_sbd_gb_mean['cep_km'] + df_sbd_gb_std['cep_km']).tolist()
                # + [ x if x > 2 else 2 for x in (df_sbd_gb_mean['cep_km'] - df_sbd_gb_std['cep_km']).tolist()[::-1]],
                # fill='toself',
                # fillcolor='rgba(80,80,80,0.2)',
                # hoverinfo='none',
                # line_color='rgba(255,255,255,0)',
                # name='±1σ'
                # ))
            # figs['satcom_iridium_fix_cep_avg'].add_trace(go.Scatter(
                # x=np.hstack((df_sbd_gb_mean.index, np.nan, df_sbd_gb_mean.index)),
                # y=np.hstack((df_sbd_gb_min['cep_km'].values, np.nan, df_sbd_gb_max['cep_km'].values)),
                # hoverinfo='none',
                # line=dict(
                    # color='rgb(123,123,123)',
                    # width=1.0,
                    # dash='dot',
                    # ),
                # mode='lines',
                # name='min./max.',
                # ))                       



            # add traces -- gps fix cep
                
            idx = df_gps['idx'] == 1   
            figs['satcom_gps_fix_cep'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] - 0.25,
                y = df_gps[idx]['cep_km'] * 1000,
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['nsat'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'CEP = %{y} m',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{customdata[2]}',
                    'HDOP = %{customdata[3]:.1f}',
                    'CNO min. = %{customdata[4]} dBHz',
                    'CNO mean = %{customdata[5]} dBHz',
                    'CNO max. = %{customdata[6]} dBHz',
                    '<extra></extra>',
                    ]),
                name='before descent',
                marker=dict(
                    color='rgba(255,69,69,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='x',
                    ),
                mode='markers',
                # opacity=0.7,
                ))
            idx = (df_gps['idx'] == 0) | (df_gps['idx'] == 2)     
            figs['satcom_gps_fix_cep'].add_trace(go.Scatter(
                x = df_gps[idx]['dive'] + 0.25,
                y = df_gps[idx]['cep_km'] * 1000,
                customdata=np.stack(
                    (
                        df_gps[idx]['dt_str'],
                        df_gps[idx]['ttf_sec'],
                        df_gps[idx]['nsat'],
                        df_gps[idx]['hdop'],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_min'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_avg'] ],
                        [ '-' if np.isnan(x) else str(x) for x in df_gps[idx]['snr_max'] ],
                        ), 
                    -1
                    ),
                hovertemplate='<br>'.join([
                    'Dive <b>%{x:.0f}</b>',
                    '%{customdata[0]|%Y-%m-%d %H:%M}Z',
                    'CEP = %{y} m',
                    'TTF = %{customdata[1]} s',
                    'no. sat = %{customdata[2]}',
                    'HDOP = %{customdata[3]:.1f}',
                    'CNO min. = %{customdata[4]} dBHz',
                    'CNO mean = %{customdata[5]} dBHz',
                    'CNO max. = %{customdata[6]} dBHz',
                    '<extra></extra>',
                    ]),
                name='after ascent',
                marker=dict(
                    color='rgba(69,69,255,0.7)',
                    line=dict(
                        color='rgb(0,0,0)',
                        width=0.5,
                        ),
                    size=8,
                    symbol='cross',
                    ),
                mode='markers',
                # opacity=0.7,
                ))                      
            
            # add traces to average figures
            
            pre = 'satcom_'   
            
            x = ('sbd_data_size', 
                '<br>'.join([
                    'Dive <b>%{x}</b>',
                    'min = %{customdata[0]:.0f} bytes',
                    'mean = %{y:.1f} bytes',
                    'max = %{customdata[1]:.0f} bytes',
                    'σ = %{customdata[2]:.1f} bytes',
                    '<extra></extra>',
                    ]))
            var = x[0]
            dive, min, mean, max, std = df_sbd_gb_mean.index.values, df_sbd_gb_min['size_b'], df_sbd_gb_mean['size_b'], df_sbd_gb_max['size_b'], df_sbd_gb_std['size_b']
            hovertemplate = x[1]
            
            figs[pre+var+'_avg'].add_trace(go.Scatter(
                x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                hoverinfo='none',
                line=dict(
                    color='rgb(194,204,204)',
                    width=1.5,
                    ),
                mode='lines',
                name='min./max.'
                )) 
            figs[pre+var+'_avg'].add_trace(go.Scatter(
                x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                hoverinfo='none',
                line=dict(
                    color='rgb(123,123,123)',
                    width=1.5,
                    ),                                
                mode='lines',
                name='±1σ',
                ))   
            figs[pre+var+'_avg'].add_trace(go.Scatter(
                x=dive,
                y=mean,
                customdata=np.stack((min, max, std), -1),
                hovertemplate=hovertemplate,
                line=dict(
                    color='rgb(0,0,0)',
                    width=1.2,
                    ), 
                mode='lines',
                name='mean',
                ))
                
       
            x = ('iridium_fix_cep',
                    '<br>'.join([
                    'Dive <b>%{x}</b>',
                    'min = %{customdata[0]:.0f} km',
                    'mean = %{y:.1f} km',
                    'max = %{customdata[1]:.0f} km',
                    'σ = %{customdata[2]:.1f} km',
                    '<extra></extra>',
                    ]))                           
            
            var = x[0]
            dive, min, mean, max, std = df_sbd_gb_mean.index.values, df_sbd_gb_min['cep_km'], df_sbd_gb_mean['cep_km'], df_sbd_gb_max['cep_km'], df_sbd_gb_std['cep_km']
            hovertemplate = x[1]
            
            figs[pre+var+'_avg'].add_trace(go.Scatter(
                x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                y = np.vstack((min, max, [np.nan]*len(dive))).flatten('F'),
                hoverinfo='none',
                line=dict(
                    color='rgb(194,204,204)',
                    width=1.5,
                    ),
                mode='lines',
                name='min./max.'
                )) 
            figs[pre+var+'_avg'].add_trace(go.Scatter(
                x = np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                y = np.vstack((np.maximum(mean - std, min), np.minimum(mean + std, max), [np.nan]*len(dive))).flatten('F'),
                hoverinfo='none',
                line=dict(
                    color='rgb(123,123,123)',
                    width=1.5,
                    ),                                
                mode='lines',
                name='±1σ',
                ))   
            figs[pre+var+'_avg'].add_trace(go.Scatter(
                x=dive,
                y=mean,
                customdata=np.stack((min, max, std), -1),
                hovertemplate=hovertemplate,
                line=dict(
                    color='rgb(0,0,0)',
                    width=1.2,
                    ), 
                mode='lines',
                name='mean',
                ))
            
            figs['satcom_iridium_fix_cep_avg'].add_trace(go.Scatter(
                x=df_iri['dive'],
                y=df_iri['cep_km'],
                customdata=df_iri['num_pos'],
                hovertemplate='<br>'.join([
                    'Dive <b>%{x}</b>',
                    'CEP = %{y:.2f} km',
                    'No. = %{customdata}',
                    '<extra></extra>',
                    ]),
                marker=dict(
                    line=dict(
                        color='rgb(0,0,255)',
                        width=1.0,
                        ),
                    size=6,
                    symbol='cross-thin',
                    ),
                mode='markers',
                name='composite',
                ))
            
            # Write plot files
            
            fig_size = dict(
                satcom           = 'figWideXL',
                satcom_sbd_count = 'figWideXS',
                satcom_sbd_data_size = 'figWideXS',
                )
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'satcom',
                    # wide = True if fig == 'satcom' else False,
                    size = fig_size[fig] if fig in fig_size.keys() else 'fig2',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
                    
            # Cleanup
            if 'fig' in locals(): del fig
            if 'fig_meta' in locals(): del fig_meta
            if 'fig_size' in locals(): del fig_size
            if 'idx' in locals(): del idx
            gc.collect()
        
        except: 
            errstr = 'Failed to generate satcom plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


def WriteWavePlots(float_id, float_aggregate_data, dpath_float, dives, max_dive):
    """Write the wave plots for one float; return their figure metadata."""
    
    figs_meta = []
    
    if (not 'wave' in float_aggregate_data or len(float_aggregate_data['wave']) < 1):
        logger.debug('No wave data for float {}'.format(float_id))
    else: 
        try:  
        
            # Create color range
            
                                    
            colors = [colorsys.hsv_to_rgb(x*.666/(max_dive),.8,1) for x in range(max_dive+1)][::-1]
            colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
            colors[-1] = 'rgb(0,0,0)'
        
            # Preprocess data
        
            df_wave = float_aggregate_data['wave']
            df_wave_spectra = float_aggregate_data['wave_spectra']
            df_profile_positions = float_aggregate_data['profile_position']
            
            df_wave = df_wave[(~df_wave['swh'].isna()) & (df_wave['dive'] > 0)]
            
            df_wave_spectra = df_wave_spectra[(~df_wave_spectra['Szz'].isna()) & (df_wave_spectra['freq'] >= 0.03) & (df_wave_spectra['freq'] <= 1.25) & (df_wave_spectra['dive'] > 0) ]
            # df_wave_spectra = df_wave_spectra[(~df_wave_spectra['Szz'].isna())  & (df_wave_spectra['dive'] > 0) ]
            df_wave_spectra.loc[
                (df_wave_spectra['Szz'] < 0.02),
                ['theta1']] = np.nan
            df_wave_spectra.loc[
                (df_wave_spectra['Sxx'] < 0.001),
                ['Sxx']] = 0.001
            df_wave_spectra.loc[
                (df_wave_spectra['Syy'] < 0.001),
                ['Syy']] = 0.001
            df_wave_spectra.loc[
                (df_wave_spectra['Szz'] < 0.001),
                ['Szz']] = 0.001
            df_wave_spectra.loc[
                (df_wave_spectra['Sxx_g'] < 0.0001),
                ['Sxx_g']] = 0.0001
            df_wave_spectra.loc[
                (df_wave_spectra['Syy_g'] < 0.0001),
                ['Syy_g']] = 0.0001
            df_wave_spectra.loc[
                (df_wave_spectra['Szz_g'] < 0.0001),
                ['Szz_g']] = 0.0001
            
            df_wave_spectra = pd.merge(df_wave_spectra, df_profile_positions, on=['dive'])
            
            df_wave = pd.merge(df_wave, df_profile_positions, on=['dive'])
            
            
            # Calculate Ranges
            
            rng = dict(
                freq       = np.array([0.03, 0.53]).astype(np.double),
                freq_g     = np.array([0.03, 0.53]).astype(np.double),
                period     = np.array([2, np.max(df_wave['period_peak'].values)]).astype(np.double),
                period_avg = np.array([2, np.max(df_wave['period_avg'].values)]).astype(np.double),
                swh        = np.array([0, np.max(df_wave['swh'].values)]).astype(np.double),
                Cxy        = np.array([0.00, np.max(np.abs(df_wave_spectra[~df_wave_spectra['Cxy'].isna()]['Cxy'].values))]).astype(np.double),
                Cxy_g      = np.array([0.00, np.max(np.abs(df_wave_spectra[~df_wave_spectra['Cxy_g'].isna()]['Cxy_g'].values))]).astype(np.double),
                Sxx        = np.array([0.01, np.max(df_wave_spectra[~df_wave_spectra['Sxx'].isna()]['Sxx'].values)]).astype(np.double),
                Sxx_g      = np.array([0.001, np.max(df_wave_spectra[~df_wave_spectra['Sxx_g'].isna()]['Sxx_g'].values)]).astype(np.double),
                Syy        = np.array([0.01, np.max(df_wave_spectra[~df_wave_spectra['Syy'].isna()]['Syy'].values)]).astype(np.double),
                Syy_g      = np.array([0.001, np.max(df_wave_spectra[~df_wave_spectra['Syy_g'].isna()]['Syy_g'].values)]).astype(np.double),
                Szz        = np.array([0.01, np.max(df_wave_spectra['Szz'].values)]).astype(np.double),
                Szz_g      = np.array([0.001, np.max(df_wave_spectra['Szz_g'].values)]).astype(np.double),
                # speed_cmps  = np.array([0, np.percentile(df_dives['speed_mps'].values, [100])[0]*100]).astype(np.double),
                time       = np.array([
                    df_profile_positions.iloc[0]['dt_epoch'], 
                    df_profile_positions.iloc[-1]['dt_epoch'],
                    ])
                
                )
            
            
                
            rng['Cxy'][0] = -rng['Cxy'][1] 
            rng['Cxy_g'][0] = -rng['Cxy_g'][1] 
            
            rng['Szz'][1] = np.max([rng['Szz'][1], 100])
            
            rng['time'] = np.round(rng['time'] + [(rng['time'][1] - rng['time'][0]) * -0.01, (rng['time'][1] - rng['time'][0]) * +0.01] + [-3600, +3600])
            rng['time'] = [ DateTime().dt_from_epoch(x).dt_str for x in rng['time'] ]     
                
            rng['swh']          = np.round(rng['swh']           + [ 0.00, 0.5], 0)
            rng['period']       = np.round(rng['period']        + [ 0.00, 0.5], 0)
            rng['period_avg']   = np.round(rng['period_avg']    + [ 0.00, 0.5], 0)

            # Generate Figures
            
            figs = dict(
                wave_direction_mean = go.Figure(
                    data = go.Heatmap(
                        x=[x[0:16] for x in df_wave_spectra['dt_str'].values],
                        y=df_wave_spectra['freq'],
                        z=np.where(df_wave_spectra['theta1'] < 180, df_wave_spectra['theta1'] + 180, df_wave_spectra['theta1'] - 180),
                        zauto=False,
                        zmin=0,
                        zmax=360,
                        connectgaps=False,
                        colorbar=dict(
                            title=dict(
                                text='Mean Direction',
                                side='top'
                                ), 
                            nticks=8,
                            tickvals=[0, 90, 180, 270, 360],
                            ticktext=['S', 'W', 'N', 'E', 'S'],
                            thickness=20,
                            orientation='h',
                            ),
                        colorscale=[
                            [0.00, "rgb(150,150,150)"], 
                            [0.01, "rgb(150,150,150)"], 
                            [0.125, "rgb(50,50,250)"], 
                            [0.375, "rgb(100,250,250)"], 
                            [0.500, "rgb(100,250,100)"],                                         
                            [0.625, "rgb(250,250,100)"], 
                            [0.875, "rgb(250,50,50)"], 
                            [0.99, "rgb(150,150,150)"], 
                            [1.00, "rgb(150,150,150)"], 
                            ],
                        customdata=np.stack((
                                df_wave_spectra['dive'],
                                df_wave_spectra['Szz'],
                                np.where(df_wave_spectra['theta1'].isna(), '-', df_wave_spectra['theta1'].map('{:.0f}'.format)),
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'f = %{y:.3f} Hz',
                            'θ<sub>1</sub> = %{customdata[2]}°',
                            'Szz = %{customdata[1]:.2f} m<sup>2</sup> Hz<sup>-1</sup>',
                            '<extra></extra>',
                            ]),
                        ),
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Mean Direction Spectra'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time']
                            ),
                        yaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_mwm = go.Figure(
                    layout=dict(
                        showlegend = False,
                        title=dict(text='{}: Significant Wave Height, Average Period, and Average Direction'.format(float_id)),
                        polar=dict(
                            angularaxis = dict(
                                direction='clockwise',
                                rotation=90,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linecolor='rgb(169,169,169)',
                                linewidth = 0.5,
                                showgrid = True,
                                showline = True,
                                showticklabels = True,
                                tickmode = 'array',
                                ticktext = ['N','','','','','','',''],
                                tickvals = [0, 45, 90, 135, 180, 225, 270, 315],
                                tickfont = dict(
                                    size = 12,
                                    ),
                                ),
                            bgcolor='white',
                            radialaxis = dict(
                                title=dict(
                                    text = 'Average Period [s]',
                                    font = dict(
                                        size= 12,
                                        ),
                                    ),
                                autorange=False,
                                angle=22.5,
                                gridcolor='rgb(169,169,169)',
                                griddash='dash',
                                gridwidth = 0.5,
                                linewidth = 1,
                                nticks = 8,
                                range =rng['period_avg'],
                                showgrid = True,
                                showline = False,
                                tickangle = 22.5,
                                tickfont = dict(
                                    size = 12,
                                    ),
                                ),
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        # plot_bgcolor='white',
                        width=900,
                        height=900,
                        margin=dict(l=10,r=10,b=10,t=70,pad=4),
                        )
                    ),
                wave_swh = go.Figure(
                    go.Scatter(
                        x = df_wave['dt_str'],
                        y = df_wave['swh'],
                        customdata=np.stack(
                            (
                                df_wave['dive'],
                                df_wave['dt_str'],
                                np.where(df_wave['mwd'].isna(), '-', df_wave['mwd'].map('{:.0f}'.format)),
                                df_wave['period_peak'],
                                df_wave['period_avg'],
                                df_wave['period_zero'],
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]:.0f}</b>',
                            '%{customdata[1]|%Y-%m-%d %H:%M}Z',
                            'H<sub>m0</sub> = %{y} m',
                            'θ<sub>mean</sub> = %{customdata[2]}°',
                            'T<sub>peak</sub> = %{customdata[3]:.0f} s',
                            'T<sub>avg</sub> = %{customdata[4]:.1f} s',
                            'T<sub>0</sub> = %{customdata[5]:.1f} s',
                            '<extra></extra>',
                            ]),
                        line=dict(
                            color='black',
                            width=2,
                            ),
                        marker=dict(
                            color=[ colors[x] for x in df_wave['dive'].values ],
                            line=dict(
                                color='rgb(88,88,88)',
                                width=0.5,
                                ),
                            symbol='cross',
                            size=6,
                            ),
                        mode='lines+markers',
                        showlegend=False,
                        ),
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Significant Wave Height'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                            type='date',
                            ),
                        yaxis=dict(
                            title='H<sub>m0</sub> [m]',
                            autorange=False,
                            range=rng['swh'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        legend=dict(
                            # title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_period = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Average Wave Period'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time'],
                            type='date',
                            ),
                        yaxis=dict(
                            title='Period [s]',
                            autorange=False,
                            range=rng['period'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        hovermode='x',
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_accel_Cxy_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Horizontal Acceleration Co-Spectra (C<sub>XY</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        yaxis=dict(
                            title='Co-Spectral Density (C<sub>XY</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Cxy_g'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_accel_Sxx = go.Figure(
                    layout=dict(
                    showlegend = True,
                    title = dict(text='{}: Wave East-West Horizontal Acceleration Spectra (S<sub>XX</sub>)'.format(float_id)),
                    xaxis=dict(
                        title='Frequency [Hz]',
                        autorange=False,
                        range=rng['freq_g']
                        ),
                    yaxis=dict(
                        title='Power Spectral Density (S<sub>XX</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                        autorange=False,
                        range=np.log10(rng['Sxx_g']),
                        type='log',
                        ),
                    hoverlabel=dict(
                        font=dict(
                            family='Consolas, monospace',
                            size=14,
                            ),
                        ),
                    legend=dict(
                        title='Dive',
                        ),
                    plot_bgcolor='white',
                    margin=dict(l=10,r=10,b=20,t=50,pad=4)
                    ),
                    ),
                wave_spectra_accel_Sxx_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave East-West Horizontal Acceleration Spectra (S<sub>XX</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>XX</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Sxx_g'],
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_accel_Syy = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave North-South Horizontal Acceleration Spectra (S<sub>YY</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>YY</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=np.log10(rng['Syy_g']),
                            type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_accel_Syy_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave North-South Horizontal Acceleration Spectra (S<sub>YY</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>YY</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Syy_g'],
                            # type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),                                
                wave_spectra_accel_Szz = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Acceleration Spectra (S<sub>ZZ</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>ZZ</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=np.log10(rng['Szz_g']),
                            type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_accel_Szz_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Acceleration Spectra (S<sub>ZZ</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>ZZ</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Szz_g'],
                            # type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_disp_Cxy_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Horizontal Heave Co-Spectra (C<sub>XY</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Co-Spectral Density (X<sub>XY</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Cxy'],
                            # type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_disp_Sxx = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave East-West Horizontal Heave Spectra (S<sub>XX</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>XX</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=np.log10(rng['Sxx']),
                            type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_disp_Sxx_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave East-West Horizontal Heave Spectra (S<sub>XX</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>XX</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Sxx'],
                            # type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_disp_Syy = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave North-South Horizontal Heave Spectra (S<sub>YY</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>YY</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=np.log10(rng['Syy']),
                            type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_disp_Syy_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave North-South Horizontal Heave Spectra (S<sub>YY</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>YY</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Syy'],
                            # type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),                                
                wave_spectra_disp_Szz = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Heave Spectra (S<sub>ZZ</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>ZZ</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=np.log10(rng['Szz']),
                            type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_spectra_disp_Szz_linear = go.Figure(
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Heave Spectra (S<sub>ZZ</sub>)'.format(float_id)),
                        xaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        yaxis=dict(
                            title='Power Spectral Density (S<sub>ZZ</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]',
                            autorange=False,
                            range=rng['Szz'],
                            # type='log',
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_waterfall_accel_Szz = go.Figure(
                    data = go.Heatmap(
                        x=[x[0:16] for x in df_wave_spectra['dt_str'].values],
                        y=df_wave_spectra['freq'],
                        z=np.round(np.where(df_wave_spectra['Szz_g'] > .001, np.log10(df_wave_spectra['Szz_g'].astype(np.double)), -3), 3).astype(np.double),
                        zauto=False,
                        zmin=-2.7,
                        zmax=-0.7,
                        connectgaps=True,
                        colorbar=dict(
                            title=dict(
                                text='Power Spectral Density (S<sub>ZZ</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                                side='top'
                                ), 
                            tickvals=[ -2.5, -2, -1.5, -1, -0.5],
                            ticktext=[ '10<sup>-2.5</sup>', '10<sup>-2.0</sup>','10<sup>-1.5</sup>', '10<sup>-1.0</sup>', '10<sup>-0.5</sup>'],
                            thickness=20,
                            orientation='h',
                            ),
                        colorscale=[
                            [0.00, "rgb(255,255,255)"], 
                            [0.40, "rgb(227,235,245)"], 
                            [0.55, "rgb(156,185,216)"], 
                            [0.70, "rgb(138,86,167)"], 
                            [0.85, "rgb(78,0,76)"], 
                            [1.00, "rgb(245,7,7)"], 
                            ],
                        customdata=np.stack((
                                df_wave_spectra['dive'],
                                df_wave_spectra['Szz'],
                                df_wave_spectra['Szz_g'],
                                np.where(df_wave_spectra['theta1'].isna(), '-', df_wave_spectra['theta1'].map('{:.0f}'.format)),
                                # np.array(['-' if np.isnan(x) else '{:.0f}'.format(x) for x in df_wave_spectra['theta1'].values])
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'f = %{y:.3f} Hz',
                            'Szz = %{customdata[2]:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[1]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                            'θ<sub>1</sub> = %{customdata[3]}°',
                            '<extra></extra>',
                            ]),
                        zsmooth='best',
                        ),
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Acceleration Spectra'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time']
                            ),
                        yaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_waterfall_accel_Szz_linear = go.Figure(
                    data = go.Heatmap(
                        x=[x[0:16] for x in df_wave_spectra['dt_str'].values],
                        y=df_wave_spectra['freq'],
                        z=df_wave_spectra['Szz_g'],
                        zauto=False,
                        zmin=0,
                        zmax=0.20,
                        connectgaps=True,
                        colorbar=dict(
                            title=dict(
                                text='Power Spectral Density (S<sub>ZZ</sub>) [g<sup>2</sup> Hz<sup>-1</sup>]',
                                side='top'
                                ), 
                            tickvals=[0, 0.05, 0.10, 0.15, 0.20],
                            ticktext=['0', '0.05', '0.10', '0.15', '0.20'],
                            # side='top',
                            thickness=20,
                            orientation='h',
                            ),
                        # colorscale='BuPu',
                        colorscale=[
                            [0.0, "rgb(255,255,255)"], 
                            [0.01, "rgb(255,255,255)"], 
                            [0.10, "rgb(156,185,216)"], 
                            [0.33, "rgb(138,86,167)"], 
                            [0.90, "rgb(78,0,76)"], 
                            [1.00, "rgb(245,7,7)"], 
                            ],
                        customdata=np.stack((
                                df_wave_spectra['dive'],
                                df_wave_spectra['Szz'],
                                np.where(df_wave_spectra['theta1'].isna(), '-', df_wave_spectra['theta1'].map('{:.0f}'.format)),
                                # np.array(['-' if np.isnan(x) else '{:.0f}'.format(x) for x in df_wave_spectra['theta1'].values])
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'f = %{y:.3f} Hz',
                            'Szz = %{z:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[1]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                            'θ<sub>1</sub> = %{customdata[2]}°',
                            '<extra></extra>',
                            ]),
                        zsmooth='best',
                    ),
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Acceleration Spectra'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time']
                            ),
                        yaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq_g']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_waterfall_disp_Szz = go.Figure(
                    data = go.Heatmap(
                        x=[x[0:16] for x in df_wave_spectra['dt_str'].values],
                        y=df_wave_spectra['freq'],
                        z=np.round(np.where(df_wave_spectra['Szz'] > .01, np.log10(df_wave_spectra['Szz'].astype(np.double)), -2).astype(np.double), 3),
                        zauto=False,
                        zmin=-2,
                        zmax=2.2,
                        connectgaps=True,
                        colorbar=dict(
                            title=dict(
                                text='Power Spectral Density (S<sub>ZZ</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]', 
                                side='top'
                                ), 
                            nticks=5,
                            tickvals=[-2, -1, 0, 1, 2],
                            ticktext=['10<sup>-2</sup>', '10<sup>-1</sup>', '10<sup>0</sup>', '10<sup>1</sup>', '10<sup>2</sup>'],
                            thickness=20,
                            orientation='h',
                            ),
                        colorscale=[
                            [0.00, "rgb(255,255,255)"], 
                            [0.40, "rgb(227,235,245)"], 
                            [0.55, "rgb(156,185,216)"], 
                            [0.70, "rgb(138,86,167)"], 
                            [0.85, "rgb(78,0,76)"], 
                            [1.00, "rgb(245,7,7)"], 
                            ],
                        customdata=np.stack((
                                df_wave_spectra['dive'],
                                df_wave_spectra['Szz'],
                                df_wave_spectra['Szz_g'],
                                np.where(df_wave_spectra['theta1'].isna(), '-', df_wave_spectra['theta1'].map('{:.0f}'.format)),
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'f = %{y:.3f} Hz',
                            'Szz = %{customdata[1]:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[2]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                            'θ<sub>1</sub> = %{customdata[3]}°',
                            '<extra></extra>',
                            ]),
                        zsmooth='best',
                        ),
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Heave Spectra'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time']
                            ),
                        yaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            # font=dict(size=10)
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                wave_waterfall_disp_Szz_linear = go.Figure(
                    data = go.Heatmap(
                        x=[x[0:16] for x in df_wave_spectra['dt_str'].values],
                        y=df_wave_spectra['freq'],
                        z=df_wave_spectra['Szz'],
                        zauto=False,
                        zmin=0,
                        zmax=100,
                        connectgaps=True,
                        colorbar=dict(
                            title=dict(
                                text='Power Spectral Density (S<sub>ZZ</sub>) [m<sup>2</sup> Hz<sup>-1</sup>]', 
                                side='top'
                                ), 
                            tickvals=[0, 20, 40, 60, 80, 100],
                            ticktext=['0', '20', '40', '60', '80', '100'],
                            thickness=20,
                            orientation='h',
                            ),
                        colorscale=[
                            [0.0, "rgb(255,255,255)"], 
                            [0.005, "rgb(156,185,216)"], 
                            [0.10, "rgb(138,86,167)"], 
                            [0.90, "rgb(78,0,76)"], 
                            [1.00, "rgb(245,7,7)"], 
                            ],
                        customdata=np.stack((
                                df_wave_spectra['dive'],
                                df_wave_spectra['Szz_g'],
                                np.where(df_wave_spectra['theta1'].isna(), '-', df_wave_spectra['theta1'].map('{:.0f}'.format)),
                                # np.array(['-' if np.isnan(x) else '{:.0f}'.format(x) for x in df_wave_spectra['theta1'].values])
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>%{customdata[0]}</b>',
                            '%{x|%Y-%m-%d %H:%M}Z',
                            'f = %{y:.3f} Hz',
                            'Szz = %{z:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[1]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                            'θ<sub>1</sub> = %{customdata[2]}°',
                            '<extra></extra>',
                            ]),
                        zsmooth='best',
                    ),
                    layout=dict(
                        showlegend = True,
                        title = dict(text='{}: Wave Vertical Heave Spectra'.format(float_id)),
                        xaxis=dict(
                            title='',
                            autorange=False,
                            range=rng['time']
                            ),
                        yaxis=dict(
                            title='Frequency [Hz]',
                            autorange=False,
                            range=rng['freq']
                            ),
                        hoverlabel=dict(
                            font=dict(
                                family='Consolas, monospace',
                                size=14,
                                ),
                            ),
                        legend=dict(
                            title='Dive',
                            ),
                        plot_bgcolor='white',
                        margin=dict(l=10,r=10,b=20,t=50,pad=4)
                        )
                    ),
                )
                

            # Add traces - wave period
            
            figs['wave_period'].add_trace(
                go.Scatter(
                    x = df_wave['dt_str'],
                    y = df_wave['period_zero'],
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=2,
                        dash='dot',
                        ),
                    mode='lines',
                    name='Zero-crossing',
                    ),
                )
            
            figs['wave_period'].add_trace(
                go.Scatter(
                    x = df_wave['dt_str'],
                    y = np.round(df_wave['period_peak'], 0),
                    hoverinfo='none',
                    line=dict(
                        color='rgb(194,204,204)',
                        width=1,
                        ),
                    mode='lines',
                    name='Dominant'
                    ),
                )
            
            figs['wave_period'].add_trace(
                go.Scatter(
                    x = df_wave['dt_str'],
                    y = df_wave['period_avg'],
                    customdata=np.stack(
                        (
                            df_wave['dive'],
                            df_wave['dt_str'],
                            df_wave['swh'],
                            np.where(df_wave['mwd'].isna(), '-', df_wave['mwd'].map('{:.0f}'.format)),
                            df_wave['period_peak'],
                            df_wave['period_zero'],
                            ), 
                        -1
                        ),
                    hovertemplate='<br>'.join([
                        'Dive <b>%{customdata[0]:.0f}</b>',
                        '%{customdata[1]|%Y-%m-%d %H:%M}Z',
                        'H<sub>m0</sub> = %{customdata[2]} m',
                        'θ<sub>mean</sub> = %{customdata[3]}°',
                        'T<sub>peak</sub> = %{customdata[4]:.0f} s',
                        'T<sub>avg</sub> = %{y:.1f} s',
                        'T<sub>0</sub> = %{customdata[5]:.1f} s',
                        '<extra></extra>',
                        ]),
                    
                    line=dict(
                        color='black',
                        width=2,
                        ),
                    marker=dict(
                        color=[ colors[x] for x in df_wave['dive'].values ],
                        line=dict(
                            color='rgb(88,88,88)',
                            width=0.5,
                            ),
                        symbol='cross',
                        size=6,
                        ),
                    # mode='lines+markers',
                    mode='lines+markers',
                    name='Average'
                    ),
                )
            
            # Add traces - mean wave direction
            
            i=0
            for dive in dives:
            
                try:
                
                    df_wave_dive = df_wave[df_wave['dive'] == dive]
                    
                    if len(df_wave_dive) < 1:
                        i = i+1
                        continue
                    
                    dt_str = df_wave_dive['dt_str'].values[0]
                    dt_str = dt_str[0:10] + ' ' + dt_str[11:16] + 'Z'
            
                    figs['wave_mwm'].add_trace(go.Scatterpolar(
                        r = df_wave_dive['period_avg'],
                        theta = df_wave_dive['mwd'],
                        customdata=np.stack((
                                df_wave_dive['swh'],
                                np.where(df_wave_dive['mwd'].isna(), '-', df_wave_dive['mwd'].map('{:.0f}'.format)),
                                df_wave_dive['period_peak'],
                                df_wave_dive['period_avg'],
                                df_wave_dive['period_zero'],
                                ), 
                            -1
                            ),
                        hovertemplate='<br>'.join([
                            'Dive <b>{}</b>'.format(dive),
                            dt_str,
                            'H<sub>m0</sub> = %{customdata[0]} m',
                            'θ<sub>mean</sub> = %{customdata[1]}°',
                            'T<sub>peak</sub> = %{customdata[2]:.1f} s',
                            'T<sub>avg</sub> = %{customdata[3]:.1f} s',
                            'T<sub>0</sub> = %{customdata[4]:.1f} s',
                            '<extra></extra>',
                            ]),
                        marker=dict(
                            color = colors[dive],
                            size = df_wave_dive['swh'].values[0] * 60 / 10 + 4,
                            line = dict(
                                color='rgb(0,0,0)' ,
                                width=0.3
                                ),
                            ),
                        mode='markers',
                        name = '{}'.format(dive),
                        # opacity=0.7,
                        ))
                
                except:
                    errstr = 'Failed to add traces to mean wave direction plot for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                    
                i = i+1
                
            figs['wave_mwm'].add_trace(go.Scatterpolar( 
                r = [rng['period_avg'][1]*0.90,rng['period_avg'][1]*0.93,rng['period_avg'][1]*0.95,rng['period_avg'][1]*0.955],
                theta = [-73, -66, -62.5, -60.6],
                hoverinfo = 'none',
                marker=dict(
                    color = 'rgb(255,255,255)',
                    size = [ x * 60 / 10 + 4 for x in [10,5,2,1]] ,
                    line = dict(
                        color='rgb(0,0,0)' ,
                        width=1
                        ),
                    ),
                text=['10m', '5m', '2', '1'],
                textfont=dict(
                    size=[14, 12, 10, 8],
                    ),
                textposition=['middle center','middle center','middle center','middle center'],    
                mode='markers+text',
                name = 'SWH',
                opacity=0.8,
                ))   
            
            
            # Add constant acceleration spectral density lines to vertical displacement spectra plot   
            
            for x in np.arange(-3.5, 0.5, 0.5): 
                f = np.arange(0.02, 1.255, 0.005)
                y = (5 * (10 ** x)) * 9.80665**2 / ((2*math.pi*f)**4)
                f = np.extract((y > 0.001) & (y < 500), f)
                y = np.extract((y > 0.001) & (y < 500), y)
                # y = np.where((y < 0.001) | (y > 2*rng['Szz'][1]), np.nan, y)
                figs['wave_spectra_disp_Szz'].add_trace(
                    go.Scatter(
                        x = f,
                        y = y,
                        hoverinfo='none',  
                        line=dict(
                            width = 1.0,
                            color = 'rgb(222,222,222)',
                            ),
                        mode = 'lines',
                        showlegend = False,
                        )
                    )
                
            # Add shaded region and text to vertical displacement spectra plot    
                
            f = np.arange(0.02, 1.255, 0.005)
            y = (0.0005) * 9.80665**2 / ((2*math.pi*f)**4)
            # f = np.extract((y > 0.001) & (y < 2*rng['Szz'][1]), f)
            # y = np.extract((y > 0.001) & (y < 2*rng['Szz'][1]), y)
            # y = np.where((y < 0.001) | (y > 2*rng['Szz'][1]), np.nan, y)
            f = np.extract(y < 500, f)
            y = np.extract(y < 500, y)
            y = np.where((y < 0.001) , 0.001, y)
            figs['wave_spectra_disp_Szz'].add_trace(
                go.Scatter(
                    x = f,
                    y = y,
                    fill = 'tozeroy',
                    fillcolor='rgba(244,244,244,0.7)',
                    hoverinfo='none',  
                    line=dict(
                        width = 1.0,
                        color = 'rgb(222,222,222)',
                        ),
                    mode = 'lines',
                    # name = '<5E-4 g<sup>2</sup>Hz<sup>-1</sup>',
                    showlegend = False,
                    )
                )
                
                
            # Add labels to constant acceleration spectral density lines
                
            f = np.array([ 0.50 ])
            x = np.array([ 5 ])
            y = x * 9.80665**2 / ((2*math.pi*f)**4)
            figs['wave_spectra_disp_Szz'].add_trace(
                go.Scatter(
                    x = f,
                    y = y,
                    mode = 'text',
                    text=[ '5g<sup>2</sup>Hz<sup>-1</sup>' ],
                    textposition=[ 'middle center' ],   
                    textfont=dict(
                        size=12,
                        ),
                    # name = '<5E-4 g<sup>2</sup>Hz<sup>-1</sup>',
                    showlegend = False,
                    )
                )
            x = np.array([ 0.5 ])
            y = x * 9.80665**2 / ((2*math.pi*f)**4)
            figs['wave_spectra_disp_Szz'].add_trace(
                go.Scatter(
                    x = f,
                    y = y,
                    mode = 'text',
                    text=[ '0.5' ],
                    textposition=[ 'middle center' ],   
                    textfont=dict(
                        size=12,
                        ),
                    # name = '<5E-4 g<sup>2</sup>Hz<sup>-1</sup>',
                    showlegend = False,
                    )
                )
            x = np.array([ 0.05 ])
            y = x * 9.80665**2 / ((2*math.pi*f)**4)
            figs['wave_spectra_disp_Szz'].add_trace(
                go.Scatter(
                    x = f,
                    y = y,
                    mode = 'text',
                    text=[ '0.05' ],
                    textposition=[ 'middle center' ],   
                    textfont=dict(
                        size=12,
                        ),
                    # name = '<5E-4 g<sup>2</sup>Hz<sup>-1</sup>',
                    showlegend = False,
                    )
                )
            
            del f, y, x
            
            # Add traces - wave spectra    
                
            i=0
            for dive in dives:
            
                try:
                
                    df_wave_spectra_dive = df_wave_spectra[df_wave_spectra['dive'] == dive]
                    df_wave_dive = df_wave[df_wave['dive'] == dive]
                    
                    if len(df_wave_spectra_dive) < 1:
                        i = i+1
                        continue
                    
                    if len(df_wave_dive) < 1:
                        i = i+1
                        continue
                    
                    dt_str = df_wave_dive['dt_str'].values[0]
                    dt_str = dt_str[0:10] + ' ' + dt_str[11:16] + 'Z'

                    figs['wave_spectra_accel_Cxy_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Cxy_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Cxy'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'C<sub>XY</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )

                    figs['wave_spectra_accel_Sxx'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Sxx_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Sxx'],
                                    # df_wave_spectra_dive['Syy'],
                                    # df_wave_spectra_dive['Syy_g'],
                                    # df_wave_spectra_dive['Szz'],
                                    # df_wave_spectra_dive['Szz_g'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'S<sub>XX</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                # 'S<sub>YY</sub> = %{customdata[1]:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[2]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                # 'S<sub>ZZ</sub> = %{customdata[3]:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[4]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                    figs['wave_spectra_accel_Sxx_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Sxx_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Sxx'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'S<sub>XX</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                        
                    figs['wave_spectra_accel_Syy'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Syy_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Syy'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'S<sub>YY</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                    figs['wave_spectra_accel_Syy_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Syy_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Syy'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'S<sub>YY</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )

                    figs['wave_spectra_accel_Szz'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Szz_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Szz'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                'S<sub>ZZ</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                    figs['wave_spectra_accel_Szz_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Szz_g'],
                            customdata=np.stack(
                                (
                                    df_wave_spectra_dive['Szz'],
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[1]}°',
                                'S<sub>ZZ</sub> = %{y:.3f} g<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.2f} m<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )

                    figs['wave_spectra_disp_Cxy_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Cxy'],
                            customdata=np.stack((
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    df_wave_spectra_dive['Cxy_g'],
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[0]}°',
                                'C<sub>XY</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )

                    figs['wave_spectra_disp_Sxx'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Sxx'],
                            customdata=np.stack((
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    df_wave_spectra_dive['Sxx_g'],
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[0]}°',
                                'S<sub>XX</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                    figs['wave_spectra_disp_Sxx_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Sxx'],
                            customdata=np.stack((
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    df_wave_spectra_dive['Sxx_g'],
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[0]}°',
                                'S<sub>XX</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                                                        
                    figs['wave_spectra_disp_Syy'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Syy'],
                            customdata=np.stack((
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    df_wave_spectra_dive['Syy_g'],
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[0]}°',
                                'S<sub>YY</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                    figs['wave_spectra_disp_Syy_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Syy'],
                            customdata=np.stack((
                                    # np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    df_wave_spectra_dive['Syy_g'],
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                # 'θ<sub>1</sub> = %{customdata[0]}°',
                                'S<sub>YY</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[0]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )                                    
                                                    
                    figs['wave_spectra_disp_Szz'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Szz'],
                            customdata=np.stack((
                                    round((1/df_wave_spectra_dive['freq']).astype(np.double), 1),
                                    np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    round(df_wave_spectra_dive['Szz'] / ( 9.80665**2 / ((2*math.pi*df_wave_spectra_dive['freq'])**4)), 3),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'T = %{customdata[0]:.1f} s',
                                'θ<sub>1</sub> = %{customdata[1]}°',
                                'S<sub>ZZ</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[2]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                    figs['wave_spectra_disp_Szz_linear'].add_trace(
                        go.Scatter(
                            x = df_wave_spectra_dive['freq'],
                            y = df_wave_spectra_dive['Szz'],
                            customdata=np.stack((
                                    round((1/df_wave_spectra_dive['freq']).astype(np.double), 1),
                                    np.where(df_wave_spectra_dive['theta1'].isna(), '-', df_wave_spectra_dive['theta1'].map('{:.0f}'.format)),
                                    round(df_wave_spectra_dive['Szz'] / ( 9.80665**2 / ((2*math.pi*df_wave_spectra_dive['freq'])**4)), 3),
                                    ), 
                                -1
                                ),
                            hovertemplate='<br>'.join([
                                'Dive <b>{}</b>'.format(dive),
                                dt_str,
                                'f = %{x:.3f} Hz',
                                'T = %{customdata[0]:.1f} s',
                                'θ<sub>1</sub> = %{customdata[1]}°',
                                'S<sub>ZZ</sub> = %{y:.2f} m<sup>2</sup> Hz<sup>-1</sup> (%{customdata[2]:.3f} g<sup>2</sup> Hz<sup>-1</sup>)',
                                '<extra></extra>',
                                ]),
                            line=dict(
                                color=colors[dive],
                                width=2 if dive == dives[-1] else 1.2,
                                ),
                            mode='lines',
                            name='{}'.format(dive)
                            ),
                        )
                
                except:
                    errstr = 'Failed to add traces to wave spectra plots for float {}, dive {}'.format(float_id, dive)
                    if hasattr(args, 'halt') and args.halt:
                        raise Exception(errstr)
                    logger.error(errstr)
                
                i = i+1
                
            # Write plot files
            
            fig_size = dict(
                wave_direction_mean='figWideXL',
                wave_mwm='figWideXXL',
                wave_spectra_accel_Cxy_linear='figWideL',
                wave_spectra_accel_Sxx='figWideL',
                wave_spectra_accel_Sxx_linear='figWideL',
                wave_spectra_accel_Syy='figWideL',
                wave_spectra_accel_Syy_linear='figWideL',
                wave_spectra_accel_Szz='figWideL',
                wave_spectra_accel_Szz_linear='figWideL',
                wave_spectra_disp_Cxy_linear='figWideL',
                wave_spectra_disp_Sxx='figWideL',
                wave_spectra_disp_Sxx_linear='figWideL',
                wave_spectra_disp_Syy='figWideL',
                wave_spectra_disp_Syy_linear='figWideL',
                wave_spectra_disp_Szz='figWideL',
                wave_spectra_disp_Szz_linear='figWideL',
                wave_waterfall_accel_Szz='figWideXL',
                wave_waterfall_accel_Szz_linear='figWideXL',
                wave_waterfall_disp_Szz='figWideXL',
                wave_waterfall_disp_Szz_linear='figWideXL',
                )
            
            for fig in figs.keys():
                
                fig_meta = WritePlotImageFiles(
                    id_str = fig, 
                    figure = figs[fig], 
                    float_id = float_id,
                    dpath_float = dpath_float,
                    category = 'wave',
                    size = fig_size[fig] if fig in fig_size.keys() else 'figWide',
                    ts_pos = [1.00, -0.06],
                    showlegend = True,
                    )
                if fig_meta is not None:
                    figs_meta.append(fig_meta)
        
        except: 
            errstr = 'Failed to generate wave plots for float {}'.format(float_id)
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
    
    return figs_meta


PLOT_WRITERS = [
    WriteGeoPlots,
    WriteSubmergedDisplacementPlots,
    WriteSurfaceDisplacementPlots,
    WriteTrajectoryPlots,
    WriteCycleTimingPlots,
    WriteProfilePlots,
    WriteContourPlots,
    WriteAscentPlots,
    WriteDescentPlots,
    WriteDriftPlots,
    WritePumpPlots,
    WriteSurfacePlots,
    WriteSatcomPlots,
    WriteWavePlots,
    ]


def WritePlotFiles():
    
    global float_aggregate_map
    
    try:
        
        # Check output directory:
        
        if not hasattr(args, 'plot_output'):
            raise Exception('Plot output directory not specified.')
        dpath_out = re.sub('\\\\', '/', args.plot_output)
        if not os.path.isdir(dpath_out):
            raise Exception('Plot output directory not found: ' + dpath_out)
            
        # Iterate over float aggregate data
        
        for float_id in float_aggregate_map.keys():
            
            logger.info('Writing plot files for float {:04d}...'.format(float_id))
            
            try:
                
                figs_meta = []
            
                # Check aggregate data object
                    
                float_aggregate_data = float_aggregate_map[float_id]
                if not 'id' in float_aggregate_data:
                    logger.error('Invalid aggregate data object, no \'id\'.')
                    continue
                if not hasattr(float_aggregate_data['id'], 'whoi'):
                    logger.error('Invalid aggregate data object, no \'id.whoi\'.')
                    continue
                
                # Check if dive data is available
                
                dives = sorted([*{*float_aggregate_data['sbd'].dive}])
                if len(dives) < 1:
                    logger.debug('No dive data for float {}.'.format(float_id))
                    continue
                max_dive = np.max(dives)
                
                
                # Generate path
                
                dpath_float = re.sub('\\\\', '/', os.path.join(args.plot_output, '{}'.format(float_aggregate_data['id'].whoi)))
                
                if not os.path.isdir(dpath_float):
                    if os.path.exists(dpath_float): 
                        raise Exception('Invalid path: \'' + dpath_float + '\'')
                    else:
                        logger.warning('Creating directory: \'' + dpath_float + '\'')
                        os.makedirs(dpath_float)
                        

                        
                # Write plots, one function per plot group
                
                for write_plots in PLOT_WRITERS:
                    figs_meta += write_plots(float_id, float_aggregate_data, dpath_float, dives, max_dive)
                
                # Save figure metadata
                
                figs_meta_dict = dict()
                for fig_meta in figs_meta:
                    # fig_meta['fpath_json'] = re.sub('.*/', '', fig_meta['fpath_json']) 
                    fig_meta['fpath_html']              = re.sub('.*/', '', fig_meta['fpath_html']) 
                    fig_meta['fpath_img']               = re.sub('.*/', '', fig_meta['fpath_img']) 
                    fig_meta['fpath_img_small']         = re.sub('.*/', '', fig_meta['fpath_img_small']) 
                    fig_meta['fpath_img_thumb']         = re.sub('.*/', '', fig_meta['fpath_img_thumb']) 
                    fig_meta['fpath_svg']               = re.sub('.*/', '', fig_meta['fpath_svg']) 
                    fig_meta['fpath_svg_small']         = re.sub('.*/', '', fig_meta['fpath_svg_small']) 
                    fig_meta['title']                   = re.sub('.*: ', '',fig_meta['title'])
                    figs_meta_dict[fig_meta['id_str']]  = fig_meta
                    
                float_aggregate_data['figures'] = figs_meta_dict
                
                # Save float aggregate data to map:

                # float_aggregate_map[float_id] = float_aggregate_data
                
                
                
                # Write HTML file
                    
                WriteHtmlFile(float_id)
              

                    

                  
            except:
                errstr = 'Failed to write plot files for float {}'.format(float_id)
                if hasattr(args, 'halt') and args.halt:
                    raise Exception(errstr)
                logger.error(errstr)
           
    except:
        
        errstr = 'Failed to write plot files'
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)

def WritePlotImageFiles(
        id_str, 
        figure, 
        float_id, 
        dpath_float = None, 
        category = 'uncategorized', 
        size = 'fig',
        ts_pos = [1, -.06],
        showlegend = True
        ):
    try: 
        
        if dpath_float is None or len(dpath_float) < 1:
            
            # Generate path
                
            dpath_float = os.path.join(args.plot_output, '{}'.format(float_id))
            dpath_float = re.sub('\\\\', '/', dpath_float)
            
            if not os.path.isdir(dpath_float):
                if os.path.exists(dpath_float): 
                    raise Exception('Invalid path: \'' + dpath_float + '\'')
                else:
                    logger.warning('Creating directory: \'' + dpath_float + '\'')
                    os.makedirs(dpath_float)
        
        # Generate metadata
        
        fig_meta = dict(
            id_str = id_str,
            dt_str = datetime.datetime.now(datetime.UTC).isoformat()[:16] + 'Z',
            title = figure.layout.title.text,
            category = category,
            fpath = re.sub('\\\\', '/', os.path.join(dpath_float, '{}_{}'.format(float_id, id_str))),
            fpath_html = '',
            fpath_img = '',
            fpath_img_small = '',
            size = size,
            )
        
        logger.debug('Writing plot files: ' + fig_meta['fpath'])
        
        # fig_meta['fpath_json'] = fig_meta['fpath'] + '.json'        
        fig_meta['fpath_html'] = fig_meta['fpath'] + '.html'
        fig_meta['fpath_img'] = fig_meta['fpath'] + '.png'
        fig_meta['fpath_img_small'] = fig_meta['fpath'] + '_small.png'
        fig_meta['fpath_svg'] = fig_meta['fpath'] + '.svg'
        fig_meta['fpath_svg_small'] = fig_meta['fpath'] + '_small.svg'
        fig_meta['fpath_img_thumb'] = fig_meta['fpath'] + '_thumb.png'
        
        # Write HTML file
        
        figure.update_layout(
            showlegend=showlegend, 
            height=opts[size]['html_height'], 
            width=opts[size]['html_width'],
            # height=None, 
            # width=None,
            )
        
        try:
            with open(fig_meta['fpath_html'], 'w', encoding='utf-8') as file:
            
                figure.write_html(
                    file, 
                    include_plotlyjs='cdn',
                    config=dict(
                        # scrollZoom = False,
                        displaylogo = False,
                        # autosizable = True,
                        modeBarButtonsToRemove = [ 'select2d', 'lasso2d' ]
                        )
                    )
            # figs[fig].write_html(fig_meta['fpath_html'])
        except:
            errstr = 'Failed to write file: ' + fig_meta['fpath_html']
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
            return None
                

        # Write small images
        
        figure.update_layout(
            showlegend=False, 
            # title_font_size=12,
            height=opts[size]['small_height'], 
            width=opts[size]['small_width'],
            margin=opts[size]['margin'],
            )
        
        try:
            figure.write_image(
                fig_meta['fpath_img_small'],
                height=opts[size]['small_height'], 
                width=opts[size]['small_width'],
                )
        except:
            errstr = 'Failed to write file: ' + fig_meta['fpath_img_small']
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
            return None
            
        # try:
        #     figure.write_image(
        #         fig_meta['fpath_svg_small'],
        #         height=opts[size]['small_height'], 
        #         width=opts[size]['small_width'],
        #         )
        # except:
        #     errstr = 'Failed to write file: ' + fig_meta['fpath_svg_small']
        #     if hasattr(args, 'halt') and args.halt:
        #         raise Exception(errstr)
        #     logger.error(errstr)
        #     return None

        # TODO Write thumbnail
        try:
           
            with Image.open(fig_meta['fpath_img_small']) as image:
                image.thumbnail((opts['figThumb']['width'], opts['figThumb']['height']), resample=Image.BICUBIC)
                image.save(fig_meta['fpath_img_thumb'])
           
        except:
            errstr = 'Failed to write file: ' + fig_meta['fpath_img_thumb']
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
            return None
                        
        # Add timestamp to plot
        
        # figure.add_annotation(
            # font=dict(size=8,color="rgb(169,169,169)"),
            # xref="paper",
            # yref="paper",
            # x=0.5,
            # y=1,
            # yshift=40,
            # text=fig_meta['dt_str'],
            # showarrow=False
            # )
        
        # Write JSON file
        # try:
            # with open(fig_meta['fpath_json'], 'w', encoding='utf-8') as file:
                # file.write(figure.to_json())
        # except:
            # errstr = 'Failed to write file: ' + fig_meta['fpath_json']
            # if hasattr(args, 'halt') and args.halt:
                # raise Exception(errstr)
            # logger.error(errstr)
            # return None
        

        
        # Write PNG file
        
        figure.update_layout(
            showlegend=False,
            height=opts[size]['height'], 
            width=opts[size]['width'],
            )
        
        try:
            figure.write_image(
                fig_meta['fpath_img'],
                height=opts[size]['height'], 
                width=opts[size]['width'],
                )
        except:
            errstr = 'Failed to write file: ' + fig_meta['fpath_img']
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
            return None
 
        # try:
        #     figure.write_image(
        #         fig_meta['fpath_svg'],
        #         height=opts[size]['height'], 
        #         width=opts[size]['width'],
        #         )
        # except:
        #     errstr = 'Failed to write file: ' + fig_meta['fpath_svg']
        #     if hasattr(args, 'halt') and args.halt:
        #         raise Exception(errstr)
        #     logger.error(errstr)
        #     return None
 
 
        # Display figures if option set
        
        if hasattr(args, 'display') and  args.display:
            figure.show()
        
        # Get file sizes
        
        # fig_meta['size_json'] = os.path.getsize(fig_meta['fpath_json'])
        fig_meta['size_html']       = os.path.getsize(fig_meta['fpath_html'])
        fig_meta['size_img']        = os.path.getsize(fig_meta['fpath_img'])
        fig_meta['size_img_small']  = os.path.getsize(fig_meta['fpath_img_small'])
        fig_meta['size_img_thumb']  = os.path.getsize(fig_meta['fpath_img_thumb'])
        #fig_meta['size_svg']        = os.path.getsize(fig_meta['fpath_svg'])
        #fig_meta['size_svg_small']  = os.path.getsize(fig_meta['fpath_svg_small'])
        
        # Strip paths
            
        if hasattr(args, 'strip') and len(args.strip):
            fig_meta['fpath_html']      = re.sub(args.strip, '', fig_meta['fpath_html'])
            fig_meta['fpath_img']       = re.sub(args.strip, '', fig_meta['fpath_img'])
            fig_meta['fpath_img_small'] = re.sub(args.strip, '', fig_meta['fpath_img_small'])
            fig_meta['fpath_svg']       = re.sub(args.strip, '', fig_meta['fpath_svg'])
            fig_meta['fpath_svg_small'] = re.sub(args.strip, '', fig_meta['fpath_svg_small'])
            fig_meta['fpath_img_thumb'] = re.sub(args.strip, '', fig_meta['fpath_img_thumb'])
            
        return fig_meta
        
    except: 
        errstr = 'Failed to write plot image files for float {}: {}'.format(
            float_id,
            id_str
            )
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
   
    return None
        

"""
Generate CLI arguments:
"""

parser = argparse.ArgumentParser(
    description='Generate aggregate JSON and plot files from JSON listing file(s).')

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
    help='descend into subdirectories subdirectories')

parser.add_argument(
    '-s', '--skip',
    action='store_true',
    dest='skip',
    help='skip directories with existing json files, even if listing files are newer')

"""
Generate arguments - directories:
"""

parser.add_argument(
    '-b', '--bathymetry', '--etopo2',
    default=BATHYMETRY_FILE,
    dest='bathymetry',
    help='read bathymetry for trajectory plots from ETOPO2 netCDF file FILE (default=\'{}\')'.format(BATHYMETRY_FILE),
    metavar='FILE')

parser.add_argument(
    '-D', '--display-figures',
    action='store_true',
    dest='display',
    help='display figures as they are generated')

parser.add_argument(
    '-i', '--include',
    action='append',
    dest='include',
    help='include paths that match regular expression REGEX (e.g. "/alamo/data/json/[0-9]+/jso.*\\.txt")',
    metavar='REGEX')

parser.add_argument(
    '-o', '--output',
    default='.',
    dest='output',
    help='write json files to specified directory (default=\'.\')',
    metavar='DIRECTORY')

parser.add_argument(
    '-p', '--plots',
    dest='plot_output',
    help='write plot files to specified directory',
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
    help='specify one or more paths to directories and/or JSON listing files',
    metavar='PATH ...')

  
"""
Parse arguments:
"""



args = parser.parse_args()


# setup logger

logging.basicConfig(format='%(asctime)s %(levelname)7s %(message)s', datefmt="%Y%m%dT%H%M%S")
logger = logging.getLogger('aggregate-json')
logger.propagate = False

# logger.handlers.clear()

# logger.setLevel(logging.DEBUG)

if not logger.hasHandlers():
    ch = logging.StreamHandler()
    formatter = logging.Formatter('%(asctime)s %(levelname)7s %(message)s', datefmt="%Y%m%dT%H%M%S")
    ch.setFormatter(formatter)
    logger.addHandler(ch)

# Check logging args:

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
            ProcessJsonListingFile(path)
        else:
            logger.error("Invalid path: " + path)
    
    # Aggregate dive data
    
    AggregateDiveData()
    
    del float_map
    gc.collect()
    
    # Write files
    
    WriteAggregateJsonFiles()
    
    if hasattr(args, 'plot_output') and args.plot_output is not None and len(args.plot_output):
        if not os.path.isdir(args.plot_output):
            logger.error('Invalid plot output directory: ' + args.plot_output)
        else:
            # Keep one Chromium browser running for all image exports instead of kaleido
            # starting one per image (about 0.6 s instead of 2.2 s per image).
            kaleido_server = False
            if float_aggregate_map:
                try:
                    import kaleido
                    kaleido.start_sync_server(silence_warnings = True)
                    kaleido_server = True
                except Exception as e:
                    logger.warning('Failed to start kaleido server, starting a browser per image: {}'.format(e))
            try:
                WritePlotFiles()
            finally:
                if kaleido_server:
                    kaleido.stop_sync_server(silence_warnings = True)

    # Record processed files only after all output has been written

    WriteLastProcessedFiles()

except:
    
    logger.error('Processing failed.')
    raise
