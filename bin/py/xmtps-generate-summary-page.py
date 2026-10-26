#!/usr/bin/python3
#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""Generate the ALAMO/ALTO float summary page from aggregate JSON summary files.

Reads the per-float <id>_summary.json files written by xmtps-aggregate-json.py (given as files
and/or directories, searched recursively and filtered by the -i regular expressions), ignoring
files whose JSON version is not JSON_VERSION. Writes <output>/index.html with an orthographic
map of the latest float positions coloured by status (Plotly) and a sortable table (DataTables)
with one row per float: ID, WMO, type and CTD, firmware, deployment date, age, distance
profiled, status (active, overdue, archived) and the most recent cycle. The page is always
rebuilt from all matching summary files; the output directory must exist.

Example:
    xmtps-generate-summary-page.py -i"/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" -o/data1/alamo/data/plots2 /data1/alamo/data/json

Author: aekholm
"""

import argparse
import datetime
import logging
import jsonpickle
import numpy as np
import os
import plotly.graph_objects as go
import plotly.io as pio
import re
from yattag import Doc

from PageStatus import WriteStatusFooter

# local
# from FloatClasses import DateTime, Dive, File, Float, Gps, CompositePosition, PositionVector, Sbd


# Set library options


pio.renderers.default = "browser"

"""
Static configuration:
"""


JSON_VERSION = "0.11"

"""
Global Variables:
"""

float_map = {}

summary_plot = ''

"""
Function Definitions:
"""


def ProcessDirectory(dpath):
    """Read every summary file in a directory and its subdirectories into float_map.

    Files are read in sorted order with ProcessJsonSummaryFile if they match an -i pattern (when
    given). Empty directories are ignored. Errors are logged, or raised with -H.
    """
    
    dpath = re.sub('\\\\', '/', dpath)
    
    try: 
        
        # Check directory:
        if not os.path.exists(dpath) or not os.path.isdir(dpath):
            logger.error('Invalid path: ' + dpath)
            return
        logger.info('Processsing directory: ' + dpath + ' ...')
        
        # Check if directory is empty
        if len(os.listdir(dpath)) == 0:
            logger.debug('Ignoring empty directory: ' + dpath)
            # total_empty_dirs += 1
            return
        
        # Check if directory is empty
        if len(os.listdir(dpath)) == 0:
            logger.warning('Ignoring empty directory: ' + dpath)
            # total_empty_dirs += 1
            return
        
        for child in sorted(os.listdir(dpath)):
        
            fpath_child = re.sub('\\\\', '/', os.path.join(dpath, child))
            
            # Process subdirectories
            if os.path.isdir(fpath_child):
             
                ProcessDirectory(fpath_child)
                continue
                
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
        
            ProcessJsonSummaryFile(fpath_child)   
        
    except:

        errstr = 'Failed to process directory: ' + dpath
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
              
def ProcessJsonSummaryFile(fpath):
    """Decode one <id>_summary.json file with jsonpickle and store it in float_map.

    The summary is stored under its WHOI serial number (summary['id'].whoi). Files not matching an
    -i pattern (when given) are ignored, and files with a JSON version other than JSON_VERSION
    are ignored with a warning. Errors, including summaries without 'id' or 'json_vers', are
    logged, or raised with -H.
    """
    
    global float_map
        
    fpath = re.sub('\\\\', '/', fpath)
    
    try: 
        
        # Check file:
        if not os.path.exists(fpath) or not os.path.isfile(fpath):
            logger.error('Invalid path: ' + fpath)
            return

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
            
        logger.debug('Processsing file: ' + fpath + ' ...')
        
                
        # Read file:
            
        try:
            with open(fpath, 'r') as file:
                summary_json = file.read()

        except:
            raise Exception('Failed to read file:' + fpath)
            
                    
        # Decode file: 
        
        try:     
            summary = jsonpickle.decode(summary_json) 
        except:
            raise Exception('Failed do decode file:' + fpath)
            
        if not 'id' in summary.keys():
            raise Exception('Invalid summary data.')
            
        if not 'json_vers' in summary.keys():
            raise Exception('Invalid summary data.')
            
        if summary['json_vers'] != JSON_VERSION:
            logger.warning('Ignoring summary file with invalid json version: ' + fpath)
            return
            
            
        float_map[summary['id'].whoi] = summary
        
    except:

        errstr = 'Failed to process file: ' + fpath
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
        
def WriteSummaryFile(dpath):
    """Write the summary page <dpath>/index.html from float_map and summary_plot.

    The page embeds the position map from WriteSummaryPlots and a DataTables table with one row
    per float, most recent report first; floats without a completed cycle or without profiled
    distance are left out. A float is ACTIVE if it reported within 30 days, OVERDUE within 180
    days and ARCHIVED otherwise. Inline JavaScript adds elapsed times ('3 hours ago') to the
    deployment and last-report dates and handles the auto-refresh checkbox (URL parameter 'r',
    in seconds; 1 means 60, others are at least 10). Errors are logged, or raised with -H; a
    failed row is skipped.
    """
    
        
    global float_map
    global summary_plot     
        
    fpath = re.sub('\\\\', '/', os.path.join(dpath, 'index.html'))
    
    try: 
        
        # Create document
        
        doc, tag, text, line = Doc().ttl()
        
        doc.asis('<!DOCTYPE html>')
        with tag('html'):
        
        # Create head
            
            with tag('head'):
            
                with tag('title'):
                    text('Float Summary')
                    
                doc.asis('<script type="text/javascript" src="https://cdn.datatables.net/v/dt/jq-3.6.0/dt-1.12.1/datatables.min.js"></script>')
                doc.asis('<link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/v/dt/dt-1.12.1/datatables.min.css"/>')
                
                # doc.asis('<link href="https://cdn.datatables.net/v/dt/jq-3.7.0/dt-2.3.5/datatables.min.css" rel="stylesheet" integrity="sha384-lw6xqXSsLbv9Bk8p8sb0Eoqxc5YTKvHwiXBG3EDVIqCaj4r4rCCnDDHY4As+neWR" crossorigin="anonymous">')
                # doc.asis('<script src="https://cdn.datatables.net/v/dt/jq-3.7.0/dt-2.3.5/datatables.min.js" integrity="sha384-GnTdsJpz17/btlSP5bFmeEv1QtW4NUnE9Atd4eMZjHBvJvYTrQTmTzLAKL2wlBEt" crossorigin="anonymous"></script>')

                # doc.asis('<script type="text/javascript" src="/alamo/js/datatables.min.js"></script>')
                # doc.asis('<link rel="stylesheet" type="text/css" href="/alamo/js/datatables.min.css"/>')
                
                doc.asis('''<script type="text/javascript"> 
function elapsedTimeString(dt) {
    if (dt < 0) 
        return "";
    if (dt < 120) 
        return "just now";
    if (dt < 10800)
        return `${Math.floor(dt/60)} minutes ago`;
    if (dt < 345600)
        return `${Math.floor(dt/3600)} hours ago`;
    if (dt < 63072000)
        return `${Math.floor(dt/3600/24)} days ago`;
    return `${Math.floor(dt/3600/24/365)} years ago`;
}
function updateElapsedTimes() { 
    epoch_now = Math.floor(Date.now() / 1000);
    document.querySelectorAll('#last-report').forEach((elem) => {
        dt = epoch_now - elem.getAttribute('epoch');
        if (dt < 0 || dt > 946080000) 
            return;
        elapsed_text = elapsedTimeString(dt);  
        elapsed_elem = document.createElement('small');
        elapsed_elem.textContent = elapsed_text;
        elem.append(document.createElement('br'));
        elem.append(elapsed_elem);
    });
        document.querySelectorAll('#deployed').forEach((elem) => {
        dt = epoch_now - elem.getAttribute('epoch');
        if (dt < 0 || dt > 946080000) 
            return;
        elapsed_text = elapsedTimeString(dt);  
        elapsed_elem = document.createElement('small');
        elapsed_elem.textContent = elapsed_text;
        elem.append(document.createElement('br'));
        elem.append(elapsed_elem);
    });
}
var reloading;
function checkAutoRefresh() {
    const url = new URL(window.location.href);
    const urlparams = url.searchParams;  
    let r = +urlparams.get("r");
    if(r < 1) { 
        urlparams.delete("r")
        window.history.pushState({}, '', url.toString());
        return;
    } else if(r == 1) {
        r = 60;
    } else if(r < 10) { 
        r = 10;
        urlparams.set("r","10");
        window.history.pushState({}, '', url.toString());
    }
    reloading=setTimeout("window.location.reload();", r * 1000);
    document.getElementById("autorefresh").checked=true;
}
function toggleAutoRefresh(cb) {
    const url = new URL(window.location.href);
    const urlparams = url.searchParams;
    if (cb.checked) {
        urlparams.set("r","1");
        reloading=setTimeout("window.location.reload();", 60000);
    } else {
        urlparams.delete("r");
        clearTimeout(reloading);
    }
    window.history.pushState({}, '', url.toString());
}
$(document).ready(function () { updateElapsedTimes(); checkAutoRefresh(); } );
</script>''')
                doc.asis('<script type="text/javascript">$(document).ready(function () { $("#stable").DataTable({ order: [[9, \'desc\']], "pageLength": 10, "lengthMenu": [[ 10, 20, 50, 100, -1 ], [ 10, 20, 50, 100, "ALL" ]], scrollX: false }); jQuery(".dataTable").wrap("<div id=\'tat\' />"); });</script>')

                # Create styles
                    
                with(tag('style')):
                
                    # Create styles - general
                
                    text('a { color: #2196F3; text-decoration: none; } ')
                    text('a:hover {color: #064579;} ')
                    text('body { font-family: "Open Sans", verdana, arial, sans-serif; margin: 0px; font-size: 12px; margin = 0px; } ')
                    text('#top { max-width: 1100px; margin: auto; } ')
                    text('.main { margin: 20px; }')
                    
                    # Create styles - title
                    text('#tit { margin: auto; text-align: center; }')
                    text('#tit h1  { margin: 0px; }')
                    
                    text('#pit { margin: auto; height: 420px; width: 390px;  }')
                    
                    # Create styles - table
                    
                    text('#tat { overflow-x: auto; width: 100%; margin-bottom: 20px; padding-top: 10px; }')
                    text('#stable { width: 100%; }')
                    text('#stable th { padding: 8px; } ')
                    text('#stable td { padding: 8px; text-align: left; height: 28px; } ')
                    text('#stable td .stimg { width: 80px; } ')
                    text('#stable td .stimg img { padding-right: 4px; display:inline; padding-bottom: 1px; } ')
                    text('#stable td small { color: #aaa; } ')
                    
                    # text('.itiny { width: 120px; text-align: center; } ')
                    
                    
                    # Create styles - footer
                    
                    text('#foot { font-size: 12px; color: #aaa; margin: 30px 0px; width: 100%;   } ')
                    text('#lfoot { float: left; width: 50%; white-space: preserve; margin: 0 0 30px 0;  } ')
                    text('#lfoot i { display: inline-block; margin 3px; } ')
                    text('#rfoot { float: right; white-space: preserve; margin: 0 0 30px 0; } ')
                    text('#rfoot label,input { margin: 3px; inline-block; vertical-align:middle; } ')
                    
                    
                    # text('label,input { display:  }')

                
            # Create body

            with tag('body'):
                # Create main divs
            
                with tag('div', id='top'):
                    
                    with tag('div', klass='main'):
                        
                        with tag('div', id='tit'):
                            with tag('h1'):
                                text('ALAMO/ALTO Float Summary')
                                
                        # doc.stag('img', src='summary.png')
                        with tag('div', id='pit'):
                            doc.asis(summary_plot)
                        
                        # Generate table:
                        
                        # with tag('div', id='tat'):
                        with tag('table', id='stable', style='width:100%', klass='stripe'):
                        
                            # Generate Table header    
                        
                            with tag('thead'):
                                with tag('tr'):
                                    with tag('th', rowspan=2):
                                        text('ID')
                                        # doc.stag('br')
                                        # with tag('small'):
                                            # text('WMO')
                                    with tag('th', rowspan=2):
                                        text('WMO')   
                                    
                                    with tag('th', colspan=6):
                                        text('Summary')
                                    with tag('th', colspan=4):
                                        text('Most Recent Cycle')
                                with tag('tr'):
                                    # with tag('th'):
                                    #     text(' ')
                                    
                                    with tag('th'):
                                        text('Type')   
                                        
                                    with tag('th'):
                                        text('Firmware')   
                                        
                                    with tag('th'):
                                        text('Deployed')
                                        doc.stag('br')
                                        with tag('small'):
                                            text('[UTC]')
                                    with tag('th'):
                                        text('Age')
                                        doc.stag('br')
                                        with tag('small'):
                                            text('[days]')
                                    with tag('th'):
                                        text('Distance')
                                        doc.stag('br')
                                        text('Profiled')
                                        doc.stag('br')
                                        with tag('small'):
                                            text('[km]')
                                    with tag('th'):
                                        text('Status')
                                    with tag('th'):
                                        text('No.')
                                    with tag('th'):
                                        text('Last Report')
                                        doc.stag('br')
                                        with tag('small'):
                                            text('[UTC]')
                                    with tag('th'):
                                        text('Depth')
                                        doc.stag('br')
                                        with tag('small'):
                                            text('[dBar]')
                                    with tag('th'):
                                        text('Duration')
                                        doc.stag('br')
                                        with tag('small'):
                                            text('[hours]')
                                    
                                    
                                    # with tag('th'):
                                    #     text('No. SBD')
                            
                            # Generate table body
                            
                            with tag('tbody'):
                                # idx = 1
                                for float_id in dict(sorted(float_map.items(), key=lambda x: x[1]['latest']['sbd'].dt_epoch, reverse=True)).keys():
                                    
                                    try: 
                                    
                                        x = float_map[float_id]
                                        
                                        # Check Float Data
                                        
                                        if x['latest']['cycle'] < 1:
                                            continue
                                        
                                        if (x['profdepthsum'] <= 0) or np.isnan(x['profdepthsum']):
                                            continue
                                        
                                        # if len(x['fwver']) < 10:
                                        #     continue
                                        
                                        # if len(x['ctdtype']) == 0:
                                        #     continue
                                
                                        # Generate Table row
                                
                                        with tag('tr'):
                                            
                                            # with tag('td'): # idx
                                            #     text('{}'.format(idx))
                                            
                                            with tag('td'): # id
                                                with tag('a', href='{}'.format(float_id)):
                                                    text('{}'.format(float_id))
                                                    # if x['id'].wmo > 1:
                                                        # doc.stag('br')
                                                        # with tag('small'):
                                                            # text(x['id'].wmo)
                                                    
                                                    
                                                    
                                            with tag('td'): # wmo
                                                text('{}'.format(x['id'].wmo) if x['id'].wmo > 1 else '-')
                                                
                                            with tag('td'): # type
                                                text(x['type'])
                                                
                                                if len(x['ctdtype']) > 0:
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text(x['ctdtype'])
                                                    
                                            with tag('td'): # fw ver
                                                if len(x['fwver']) > 9:
                                                    text('v{:02d}.{}.{}'.format(int(x['fwver'][2:4], 16),int(x['fwver'][4:6], 16),int(x['fwver'][7:8], 16) ))
                                            
                                                    doc.stag('br')
                                                    with tag('small'):
                                                        text(x['fwver'])
                                                else:
                                                    text('-')
                                                    
                                            # deployed     
                                            dt_epoch = x['deployment']['gps'].dt_epoch    
                                            with tag('td', id='deployed', epoch=f'{dt_epoch}'): 
                                            
                                                if x['deployment']['gps'].is_valid():
                                            
                                                    dt_str = x['deployment']['gps'].dt_str
                                                    dt_str = dt_str[0:10]
                                                    text(dt_str)
                                                else: 
                                                    text('-')
                                                    
                                            with tag('td'): # age
                                            
                                                if (x['age'] <= 0) or np.isnan(x['age']):
                                                    text('-')
                                                else:
                                                    text('{:.0f}'.format(x['age']/86400))
                                                    
                                            with tag('td'): # distance profiled
                                            
                                                if (x['profdepthsum'] <= 0) or np.isnan(x['profdepthsum']):
                                                    text('-')
                                                else:
                                                    text('{:.1f}'.format(x['profdepthsum']/1000))
                                                    
                                            with tag('td'): # status

                                                time_since_last = (datetime.datetime.now(datetime.UTC).timestamp() - x['latest']['sbd'].dt_epoch) /86400

                                                if time_since_last < 30:
                                                    with tag('div', klass='stimg'):
                                                        doc.stag('img', src='img/s2.png')
                                                        text('ACTIVE')
                                                    
                                                elif time_since_last < 180:
                                                    with tag('div', klass='stimg'):
                                                        doc.stag('img', src='img/s1.png')
                                                        text('OVERDUE')
                                                    
                                                else:
                                                    with tag('div', klass='stimg'):
                                                        doc.stag('img', src='img/s0.png')
                                                        text('ARCHIVED')
                                                
                                            with tag('td'): # cycle no.
                                            
                                                if x['latest']['cycle'] < 9999:
                                                    text('{}'.format(x['latest']['cycle']))
                                                else: 
                                                    text('-')
                                             
                                            # last report
                                            dt_epoch = x['latest']['sbd'].dt_epoch    
                                            with tag('td', id='last-report', epoch=f'{dt_epoch}'): 
                                                dt_str = x['latest']['sbd'].dt_str
                                                dt_str = dt_str[0:10] + ' ' +  dt_str[11:16]
                                                text(dt_str)
                                                
                                            with tag('td'): # depth
                                                depth = x['latest']['depth']
                                            
                                                if (depth <= 0) or (depth >= 3000) or np.isnan(depth):
                                                    text('-')
                                                elif (depth < 100):
                                                    text('{:.1f}'.format(depth))
                                                else:
                                                    text('{:.0f}'.format(depth))
                                                
                                            with tag('td'): # cycle duration
                                            
                                                if (x['latest']['duration'] <= 0) or (np.isnan(x['latest']['duration'])):
                                                    text('-')
                                                else:
                                                    duration = x['latest']['duration']/3600.
                                                    
                                                    text('{:.1f}'.format(duration))
                                                    
                                            
                                                
                                           
                                                
                                            # with tag('td'): # no. sbd
                                            #     text('{}'.format(x['latest']['nsbd']))
                                            
                                    except:
                                        errstr = 'Failed to create summary row for float {}'.format(float_id)
                                        
                                        if hasattr(args, 'halt') and args.halt:
                                            raise Exception(errstr)
                                        logger.error(errstr)
                                        
                            
                              
                        # Create footer
                        
                        with tag('div', id='foot'):
                            with tag('div', id='lfoot'):
                                with tag('i'):
                                    WriteStatusFooter(doc, tag, text, JSON_VERSION, args.runtime)
                                # text('Page last updated ' + datetime.datetime.now(datetime.UTC).isoformat()[:16] + 'Z' + ' - v' + JSON_VERSION + ' - Float Group, Department of Physical Oceanography, Woods Hole Oceanographic Institution, Woods Hole, MA, USA')
                            with tag('div', id='rfoot'): 
                                with tag('label', ( 'for', 'autorefresh')):
                                    text('Auto Refresh ')
                                    doc.input(name = 'autorefresh', type = 'checkbox', id = 'autorefresh', value = 'no', onclick="toggleAutoRefresh(this);")
            
        # Write Summary file:
        
        with open(fpath, 'w', encoding='utf-8') as fout:
            # fout.write(indent(
            #     doc.getvalue(),
            #     indentation = '  ',
            #     newline = '\n',
            #     indent_text = True
            #     ))
            fout.write(doc.getvalue())
        
    except:

        errstr = 'Failed to write summary file: ' + fpath
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
   
def WriteSummaryPlots(dpath):
    """Build the position map for the summary page and store its HTML in summary_plot.

    Plots the latest valid GPS position of each float on an orthographic globe with the tropics
    and polar circles, coloured and sized by time since the last report (green within 30 days,
    blue within 180 days, red otherwise). The HTML is a div that loads plotly.js from the CDN.
    Nothing is written to dpath, which is unused. Errors are logged, or raised with -H.
    """
    
        
        
    global float_map
    
    global summary_plot
    
    try: 
        
        position_summary = go.Figure(
            # go.Scattergeo(
            #     lat = df_profiles['lat'],
            #     lon = df_profiles['lon'],
            #     hoverinfo='none',
            #     line=dict(
            #         color='rgb(0, 0, 0)',
            #         width=1,
            #         ),
            #     mode = 'lines',
            #     name = 'Trajectory',
            #     ),
            layout=dict(
                # title=dict(text='{}: Profile Positions'.format(float_id)),
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
                    # projection = dict(
                    #     type = 'winkel tripel',
                    #     # type = 'satellite',
                    #     # rotation = dict(
                    #     #     lat = position_mean[0],
                    #     #     lon = position_mean[1],
                    #     #     roll = 0,
                    #     #     ),
                    #     # scale = 1,
                    #     ),
                    projection = dict(
                        type = 'orthographic',
                        # type = 'satellite',
                        rotation = dict(
                            lat = 30,
                            lon = -45,
                            # roll = 0,
                            ),
                        scale = 1,
                        ),
                    fitbounds = False,
                    lataxis = dict(
                        showgrid = True,
                        dtick = 15,
                        gridcolor = 'rgb(123, 123, 123)',
                        gridwidth = 0.5,
                        # range = [-80, 80],
                        ),
                    lonaxis = dict(
                        showgrid = True,
                        dtick = 30,
                        gridcolor = 'rgb(123, 123, 123)',
                        gridwidth = 0.5,
                        # range = [-180, 180],
                        ),
                    resolution = 50,
                    ),
                hoverlabel=dict(
                    font=dict(
                        family='Consolas, monospace',
                        size=14,
                        ),
                    ),
                plot_bgcolor='rgba(255,255,255,0)',
                margin=dict(l=5,r=5,b=5,t=5,pad=5)
                ),
            )
        
        position_summary.add_trace(
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
        
        position_summary.add_trace(
            go.Scattergeo(
                lat = np.hstack((
                    np.full((4), 0), 
                    )),
                lon = np.hstack((
                    np.array([0, 1, 2, 0]) * 120, 
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
        
    
        
        for float_id in dict(sorted(float_map.items(), key=lambda x: x[1]['latest']['sbd'].dt_epoch)).keys():
            
            try: 
                                    
                x = float_map[float_id]
                
                gps = x['latest']['gps']
                
                if not gps.is_valid():
                    continue
                
                time_since_last = (datetime.datetime.now(datetime.UTC).timestamp() - x['latest']['sbd'].dt_epoch) /86400

                color = 'rbg(0,0,0)'

                if time_since_last < 30:
                    color = 'rgb(123,255,123)'
                    size = 6
                    
                elif time_since_last < 180:
                    color = 'rgb(123,123,255)'
                    size = 5
                    
                else:
                    color = 'rgb(255,123,123)'
                    size = 4

                position_summary.add_trace(go.Scattergeo(
                    lat = [ gps.lat ],
                    lon = [ gps.lon ],
                    text = '{}'.format(float_id),
                    hovertemplate = '{}<extra></extra>'.format(float_id),
                    marker=dict(
                        color = color,
                        line=dict(
                            color='rgb(123,123,123)',
                            width=0.5,
                            ),
                        size=size,
                        ),
                    mode='markers',
                    name='{}'.format(float_id),
                    
                    ))
                # position_summary.add_annotation(
                #     text="""<a href="/alamo/plots2/{}/"> </a>""".format("float_id"),
                #         showarrow=False,
                #         xanchor='center',
                #         yanchor='middle',
                #         )
                
            except:

                errstr = 'Failed to add trace to summary plot for float: {}'.format(float_id)
                
                if hasattr(args, 'halt') and args.halt:
                    raise Exception(errstr)
                logger.error(errstr)
                
                
        # position_summary.show()
        
        # position_summary.update_layout(
        #     showlegend=False,
        #     height=400, 
        #     width=400,
        #     )
        
        # fpath = re.sub('\\\\', '/', os.path.join(dpath, 'summary.html'))
        
        # summary_plot = position_summary.write_html(full_html = False)
        
        try:
            # position_summary.write_image(fpath)
            summary_plot = position_summary.to_html(
                full_html = False, 
                include_plotlyjs='cdn', 
                div_id='plot',
                config=dict(
                    displaylogo = False,
                    displayModeBar = False,
                    responsive = True,
                    modeBarButtonsToRemove = [ 'select2d', 'lasso2d' ],
                    # showtips = False,
                    ),
                )
        except:
            errstr = 'Failed to write summary plot'
            if hasattr(args, 'halt') and args.halt:
                raise Exception(errstr)
            logger.error(errstr)
        
        
        
    except:

        errstr = 'Failed to write summary plots '
        
        if hasattr(args, 'halt') and args.halt:
            raise Exception(errstr)
        logger.error(errstr)
   
        
"""
Generate CLI arguments:
"""

parser = argparse.ArgumentParser(
    description='Generate summary page from aggregate JSON file(s).')

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
    help='write summary page to specified directory (default=\'.\')',
    metavar='PATH')

parser.add_argument(
    '--runtime',
    type=float,
    dest='runtime',
    help='show SECONDS as the runtime in the page footer: the duration of the JSON and plot '
         'updates before this page is generated (update-plots-service.sh passes it); without '
         'it, no runtime is shown',
    metavar='SECONDS')


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

# Check logging args:
logger = logging.getLogger('generate-summary')
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
    
    # Check inputs:
    if not hasattr(args, 'output') or not len(args.output):
        args.output = '.'
        
    if not os.path.isdir(args.output):
        raise Exception('Invalid output directory: ' + args.output)

    # Iterate over directories:
    for path in args.path_in:
        if os.path.isdir(path):
            ProcessDirectory(path)
        elif os.path.isfile(path):
            ProcessJsonSummaryFile(path)
        else:
            logger.error("Invalid path: " + path)
            
    WriteSummaryPlots(args.output)
    
    WriteSummaryFile(args.output)
        
        
except:
    
    logger.error('Failed to generate summary file.')
    raise