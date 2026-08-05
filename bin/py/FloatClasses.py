#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 21 15:11:38 2021

@author: aekholm
"""

import datetime
from dateutil import parser
from geographiclib.geodesic import Geodesic
import math
import numpy as np
import pytz
import re
from scipy.interpolate import interp1d

"""
Helper functions
"""

def RollingAverage(array, N: int ):

    if N > 1:
                
        array = np.cumsum(np.pad(array,(N//2,N//2),'constant'))
        array[N:] = array[N:] - array[:-N]
        array = array[N-1:] / N
        array = np.round(array, 5)
        
    return array



"""
Base classes:
"""

class DateTime:
    
    """
    A wrapper class for date/time
    """
    
    def __init__(self, dt = None, dt_epoch: float = None, dt_str: str = None):
        
        self.dt_epoch = None
        self.dt_str = None
        
        if dt is not None and isinstance(dt, DateTime):
            self.from_dt(dt)
            return
        if (dt_epoch is not None) and dt_epoch > 0:
            self.dt_from_epoch(dt_epoch)
            return
        if (dt_str is not None) and len(dt_str):
            self.dt_from_iso8601_str(dt_str)
            return
            
        self.dt_epoch = -1
        self.dt_str = ''
        
    def to_datetime(self):
        dt = None
        try: 
            dt = parser.isoparse(self.dt_str)
        except:
            dt = None
        return dt
        
    def dt_from_iso8601_str(self, dt_str):
        self.dt_str = dt_str
        dt = self.to_datetime()
                
        if dt is not None:
            # Valid dt_str
            self.dt_epoch = round(dt.timestamp())
        else:
            # invalid dt_str
            self.dt_epoch = -1
            self.dt_str = ''
            return self
            
        if self.dt_epoch < 1262322000: # 2010-01-01T00:00:00
            self.dt_epoch = -1
            self.dt_str = ''
            return self
            
        if self.dt_epoch > 2209006800: # 2040-01-01T00:00:00
            self.dt_epoch = -1
            self.dt_str = ''
            return self
            
        return self
            
    def dt_from_epoch(self, dt_epoch):
    
        self.dt_epoch = dt_epoch
        try:
            dt = datetime.datetime.fromtimestamp(dt_epoch, tz=pytz.UTC)
            self.dt_str = dt.strftime('%Y-%m-%dT%H:%M:%SZ')
        except:
            self.dt_epoch = -1
            self.dt_str = ''
            return self
        
        if self.dt_epoch < 1262322000: # 2010-01-01T00:00:00
            self.dt_epoch = -1
            self.dt_str = ''
            return self
            
        if self.dt_epoch > 2209006800: # 2040-01-01T00:00:00
            self.dt_epoch = -1
            self.dt_str = ''
            return self
        
        return self
        
    def from_dt(self, dt):
        
        if dt is not None and isinstance(dt, DateTime):
            # self.dt_from_epoch(dt.dt_epoch)
            self.dt_epoch = dt.dt_epoch
            self.dt_str = dt.dt_str
        
    def is_valid(self):
    
        return (self.dt_epoch >= 1262322000) and (self.dt_epoch <= 2209006800)
    
    def duration_to_hhmmss_str(duration_s: float, units = True):
    
        dstr = ''
        
        try: 
        
            if np.isnan(duration_s):
                return dstr
        
            if duration_s >= 3600:
            
                hr  = math.floor(duration_s/3600)
                min = math.floor(duration_s/60) % 60
                sec = math.floor(duration_s) % 60
                
                dstr = '{:d}:{:02d}:{:02d}'.format(hr,min,sec)
                
                if units:
                    
                    dstr = dstr + ' [hh:mm:ss]'
                    
                return dstr
        
            min = math.floor(duration_s/60) % 60
            sec = math.floor(duration_s) % 60
            
            dstr = '{:02d}:{:02d}'.format(min,sec)
            
            if units:
                
                dstr = dstr + ' [mm:ss]'
                
            return dstr
    
        except:
        
            raise Exception('Failed to generate hh:mm:ss string from duration.')
            
        return dstr
            
    
class Position(DateTime):
    
    def __init__(self, dt: DateTime = None, lat: float = np.nan, lon: float = np.nan, cep_km: float = np.nan):
        DateTime.__init__(self, dt)
        self.lat    = lat
        self.lon    = lon
        self.cep_km = cep_km
        
    def pos_from_iso6709_str(self, string):
        (self.lat, self.lon) = re.search('([+-][\d.]+)([+-][\d.]+)', string).groups()
        self.lat = float(self.lat)
        self.lon = float(self.lon)
        
    def is_valid(self):
    
        return DateTime.is_valid(self) and (self.lat >= -90) and (self.lat <= 90) and (self.lon >= -180) and (self.lon <= 180) and (self.cep_km > 0)
   
class PositionVector:

    def __init__(self, pos1: Position = Position(), pos2: Position = Position, compute = False):
        self.dist_m = np.nan
        self.azi = np.nan
        self.duration_s = np.nan
        self.speed_mps = np.nan
        
        self.pos1 = pos1
        self.pos2 = pos2
        
        if(compute):
            self.compute()
        

    def compute(self):
    
        if (not self.pos1.is_valid()) or (not self.pos2.is_valid()):
            self.dist_m = np.nan
            self.duration_s = np.nan
            self.azi = np.nan
            self.speed_mps = np.nan
            
            return self
            
        res = Geodesic.WGS84.Inverse(self.pos1.lat, self.pos1.lon, self.pos2.lat, self.pos2.lon)
        self.azi, self.dist_m = res['azi1'], res['s12']
        self.duration_s = self.pos2.dt_epoch - self.pos1.dt_epoch
        self.speed_mps = self.dist_m / self.duration_s
        
        if self.azi < 0:
            self.azi = self.azi + 360
        
        self.dist_m = round(self.dist_m, 0)
        self.speed_mps = round(self.speed_mps, 4)
        self.azi = round(self.azi,1)
        
class PresTimeSeries:
    
    def __init__(self, num: int = 0):
        self.dt_ref           = DateTime()
        self.n                = num
        self.dt_ref_delta_sec = []
        self.dt               = []
        self.dt_epoch         = []
        self.dt_str           = []
        self.pres             = []
        self.rate             = []
        
    def PostProcess(self):
        if not len(self.dt):
            self.dt_ref         = DateTime()
            del self.__dict__["dt"]
            return self
        
        self.dt_ref = DateTime().dt_from_epoch(min([dt.dt_epoch for dt in self.dt]))
        self.dt_ref_delta_sec = [(dt.dt_epoch - self.dt_ref.dt_epoch) for dt in self.dt ]
            
        # Calculate pressure rates
        
        old_len = len(self.pres)
        
        if old_len > 1:
            
            new_len = old_len*2-1
            
            new_pres             = np.empty(new_len)
            new_dt_ref_delta_sec = np.empty(new_len)
            new_rate             = np.empty(new_len)
        
            for i in range(0,old_len-1):
                
                new_pres[2*i] = self.pres[i]
                new_pres[2*i+1] = round((self.pres[i]+self.pres[i+1])/2,2)
                new_dt_ref_delta_sec[2*i] = self.dt_ref_delta_sec[i]
                new_dt_ref_delta_sec[2*i+1] = round((self.dt_ref_delta_sec[i]+self.dt_ref_delta_sec[i+1])/2,1)
                
                if i < 1:
                    new_rate[2*i] = None
                else:
                    if not new_dt_ref_delta_sec[2*i+1] == new_dt_ref_delta_sec[2*i-1]:
                        new_rate[2*i] = round(
                            (new_pres[2*i-1]- new_pres[2*i+1]) / (new_dt_ref_delta_sec[2*i+1] - new_dt_ref_delta_sec[2*i-1]),
                            4)
                    else:
                        new_rate[2*i] = new_rate[2*i-1]
                if not self.dt_ref_delta_sec[i+1] == self.dt_ref_delta_sec[i]:
                   new_rate[2*i+1] = round(
                       (self.pres[i] - self.pres[i+1]) / (self.dt_ref_delta_sec[i+1] - self.dt_ref_delta_sec[i]),
                       4)
                else:
                    new_rate[2*i+1] = new_rate[2*i]
                
            new_pres[2*(old_len-1)] = self.pres[old_len-1]
            new_dt_ref_delta_sec[2*(old_len-1)] = self.dt_ref_delta_sec[old_len-1]
            new_rate[2*(old_len-1)] = None
            
            self.dt_ref_delta_sec = new_dt_ref_delta_sec.tolist()
            self.pres = new_pres.tolist()
            self.rate = new_rate.tolist()
            self.n = new_len
                
            # Create date/time array
            
            self.dt  = [ DateTime().dt_from_epoch(self.dt_ref.dt_epoch + x) for x in self.dt_ref_delta_sec ]
            self.dt_epoch  = [ x.dt_epoch for x in self.dt ]
            self.dt_str  = [ x.dt_str for x in self.dt ]
            del self.__dict__["dt"]
                
class EngineeringData:
    
    def __init__(self, dt_start = DateTime(), vac0 = -9.99, vac1 = -9.99):
        self.dt_start = dt_start
        self.vac0     = vac0               
        self.vac1     = vac1
                
"""
Dive Data classes:
"""

class Ascent(PresTimeSeries):
    
    def __init__(self, num: int = 0):
        PresTimeSeries.__init__(self, num)
        
    def PostProcess(self):
        super().PostProcess()
        
class Descent(PresTimeSeries):
    
    def __init__(self, num: int = 0):
        PresTimeSeries.__init__(self, num)
        
    def PostProcess(self):
        super().PostProcess()
        
class Dive:
    
    def __init__(self, json_vers = "?"):
        self.json_vers   = json_vers
        self.id          = FloatId()
        self.list_file   = File()
        self.dive_num    = -999
        self.ascent      = Ascent()
        self.descent     = Descent()
        self.drift       = Drift()
        self.engineering = Engineering()
        self.gps         = []
        self.mission     = MissionParameters()
        self.profile     = Profile()
        self.pump        = Pump()
        self.sbd         = []
        self.surface     = Surface()
        self.wave        = None
   
    def PostProcess(self):
        self.ascent.PostProcess()
        self.descent.PostProcess()
        self.drift.PostProcess()
        self.profile.PostProcess()
        self.pump.PostProcess()
        self.surface.PostProcess()
        if self.wave is not None:
            self.wave.PostProcess()
        else:
            del self.wave
        
        if self.ascent.n > 0 and self.descent.n > 0:
            self.drift.EstimateDatetimes(
                self.descent.dt_epoch[-1], 
                self.ascent.dt_epoch[0]
                )

class Drift:
    
    def __init__(self, num_samples = 0):
        self.dt_epoch         = [np.nan] * num_samples
        self.dt_str           = [np.nan] * num_samples
        self.dt_ref           = DateTime()
        self.dt_ref_delta_sec = [np.nan] * num_samples
        self.pres             = np.empty(num_samples)
        self.temp             = np.empty(num_samples)
        self.saln             = np.empty(num_samples)
        self.dens             = np.empty(num_samples)
        self.sigma0           = np.empty(num_samples)
        self.n                = num_samples
        
    def EstimateDatetimes(self, epoch_start, epoch_end):
        
        if self.n == 0:
            self.dt_epoch = []
            self.dt_str   = []
            self.dt_ref   = []
            self.dt_ref_delta_sec   = []
            return
        
        self.dt_epoch = np.round(np.linspace(epoch_start, epoch_end, self.n+2)[1:-1], 0).tolist()
        self.dt_str = [ DateTime().dt_from_epoch(x).dt_str for x in self.dt_epoch ]
        self.dt_ref = DateTime().dt_from_epoch(self.dt_epoch[0])
        self.dt_ref_delta_sec = [ x - self.dt_ref.dt_epoch for x in self.dt_epoch ]
        
    def PostProcess(self):
        self.pres     = self.pres.tolist()
        self.temp     = self.temp.tolist()
        self.saln     = self.saln.tolist()
        self.dens     = self.dens.tolist()
        self.sigma0   = self.sigma0.tolist()
        
class Engineering():
    
    def __init__(self):
        self.ascent  = EngineeringAscent()
        self.descent = EngineeringDescent()
        self.drift   = EngineeringDrift()
        self.surface = EngineeringSurface()
        
class EngineeringAscent(EngineeringData):
    
    def __init__(self, dt_start = DateTime(), vac0 = -9.99, vac1 = -9.99):
        EngineeringData.__init__(self, dt_start, vac0, vac1)
        
class EngineeringDescent(EngineeringData):
    
    def __init__(self, dt_start = DateTime(), vac0 = -9.99, vac1 = -9.99):
        EngineeringData.__init__(self, dt_start, vac0, vac1)
        
class EngineeringDrift(EngineeringData):
    
    def __init__(self, dt_start = DateTime(), vac0 = -9.99, vac1 = -9.99):
        EngineeringData.__init__(self, dt_start, vac0, vac1)        

class EngineeringSurface(EngineeringData):
    
    def __init__(self, dt_start = DateTime(), vac0 = -9.99, vac1 = -9.99):
        EngineeringData.__init__(self, dt_start, vac0, vac1)

class File:
    
    def __init__(self):
        self.path = ''
        self.mtime = DateTime()

class FloatId:
   
    def __init__(self, whoi = -1, dac = -1, wmo = -1):
        self.whoi = whoi
        self.dac  = dac
        self.wmo  = wmo
        
class Float: 
        
    def __init__(self, float_id = FloatId()):
        self.dives = []
        self.id = float_id
   
class Gps(Position):
    
    def __init__(self):
        Position.__init__(self)
        self.idx      = -1
        self.ttf_sec  = -1
        self.nsat     = -1
        self.snr_min  = -1
        self.snr_avg  = -1
        self.snr_max  = -1
        self.hdop     = -9.

    def EstimateUncertainty(self):
        
        self.cep_km = 0.00666
        
        if not Position.is_valid(self):
            self.cep_km = -9
            return
    
        if self.hdop > 1:
            self.cep_km = self.cep_km + 0.01666 * (self.hdop - 1) 
        if self.nsat < 6:
            self.cep_km = self.cep_km + 0.02666 * (6 - self.nsat)**2 
        if self.snr_avg < 25: 
            self.cep_km = self.cep_km + 0.001666 * (25 - self.snr_avg) 
        self.cep_km = round(self.cep_km, 3)

class MissionParameters:

    def __init__(self, fwver = '', ctdtype= ''):
        self.fwver = fwver
        self.ctdtype = ctdtype
        self.parameters = {}

class Profile:
    
    def __init__(self, num_bins = 0):
        self.pres      = np.empty(num_bins)
        self.pres[:]   = np.nan
        self.temp      = np.empty(num_bins)
        self.temp[:]   = np.nan
        self.tcnd      = np.empty(num_bins)
        self.tcnd[:]   = np.nan
        self.saln      = np.empty(num_bins)
        self.saln[:]   = np.nan
        self.dens      = np.empty(num_bins)
        self.dens[:]   = np.nan
        self.sigma0    = np.empty(num_bins)
        self.sigma0[:] = np.nan
        self.n         = num_bins
        
    def PostProcess(self):
    
        self.pres   = self.pres.tolist()
        self.temp   = self.temp.tolist()
        self.tcnd   = self.tcnd.tolist()
        self.saln   = self.saln.tolist()
        self.dens   = self.dens.tolist()
        self.sigma0 = self.sigma0.tolist()
        
class Pump:
    
    def __init__(self):
        self.pres = []
        self.time = []
        self.volt = []
        self.curr = []
        self.curr_adj = []
        self.vac = []
        self.power = []
        self.energy = []
        self.n = -1
        
    def PostProcess(self):
        self.n = len(self.pres)
        
        self.curr_adj = [ round(c * 12 / v / 0.85, 3) for (c, v) in zip(self.curr, self.volt) ]
        self.power = [ round(v * i, 3) for (i, v) in zip(self.curr_adj, self.volt) ]
        self.energy = [ round(p * t, 2) for (p, t) in zip(self.power, self.time) ] 
   
class Sbd(Position):

    def __init__(self):
        Position.__init__(self)
        # self.idx      = -1
        self.size_b   = -1
        self.momsn    = -1
        self.mtmsn    = -1
        self.pid      = -1
        
class Surface:
    
    def __init__(self, num: int = 0):
        self.dt_ref           = DateTime()
        self.n                = num
        self.dt_ref_delta_sec = []
        self.dt               = []
        self.dt_epoch         = []
        self.dt_str           = []
        self.pres             = []
        self.temp             = []
        self.saln             = []

    def PostProcess(self):
        
        # self.pres = np.array(self.pres).tolist()
        # self.temp = np.array(self.temp).tolist()
        # self.saln = np.array(self.saln).tolist()
        
        if not len(self.dt):
            self.dt_ref         = DateTime()
            del self.__dict__["dt"]
            return self
        
        self.dt_ref = DateTime().dt_from_epoch(min([dt.dt_epoch for dt in self.dt]))
        self.dt_ref_delta_sec = [(dt.dt_epoch - self.dt_ref.dt_epoch) for dt in self.dt ]
        
        self.dt  = [ DateTime().dt_from_epoch(self.dt_ref.dt_epoch + x) for x in self.dt_ref_delta_sec ]
        self.dt_epoch  = [ x.dt_epoch for x in self.dt ]
        self.dt_str  = [ x.dt_str for x in self.dt ]
        del self.__dict__["dt"]
              
class Wave: 

    def __init__(self, fsam: float = -1, slen: int = -1):
        self.slen = slen
        flen = slen//2
        
        self.fsam = fsam
        
        
       
        # self.freq           = np.empty(flen)
        # self.freq[:]        = np.nan
        
        self.freq           = np.array([ x*self.fsam/self.slen for x in range(1, flen + 1) ])
        
        self.Szz_g          = np.empty(flen, dtype=np.double)
        self.Szz_g[:]       = np.nan
        self.Syy_g          = np.empty(flen, dtype=np.double)
        self.Syy_g[:]       = np.nan
        self.Sxx_g          = np.empty(flen, dtype=np.double)
        self.Sxx_g[:]       = np.nan
        self.Cxy_g          = np.empty(flen, dtype=np.double)
        self.Cxy_g[:]       = np.nan
        self.Qzx_g          = np.empty(flen, dtype=np.double)
        self.Qzx_g[:]       = np.nan
        self.Qzy_g          = np.empty(flen, dtype=np.double)
        self.Qzy_g[:]       = np.nan

       
        self.Szz            = np.empty(flen, dtype=np.double)
        self.Szz[:]         = np.nan
        self.Syy            = np.empty(flen, dtype=np.double)
        self.Syy[:]         = np.nan
        self.Sxx            = np.empty(flen, dtype=np.double)
        self.Sxx[:]         = np.nan
        self.Cxy            = np.empty(flen, dtype=np.double)
        self.Cxy[:]         = np.nan
        self.Qzx            = np.empty(flen, dtype=np.double)
        self.Qzx[:]         = np.nan
        self.Qzy            = np.empty(flen, dtype=np.double)
        self.Qzy[:]         = np.nan


        
        # Longuet-Higgins directional fourier series parameters
        
        self.a0             = np.empty(flen, dtype=np.double)
        self.a0[:]          = np.nan
        self.a1             = np.empty(flen, dtype=np.double)
        self.a1[:]          = np.nan
        self.a2             = np.empty(flen, dtype=np.double)
        self.a2[:]          = np.nan
        self.b1             = np.empty(flen, dtype=np.double)
        self.b1[:]          = np.nan
        self.b2             = np.empty(flen, dtype=np.double)
        self.b2[:]          = np.nan
        
        self.r1             = np.empty(flen, dtype=np.double)
        self.r1[:]          = np.nan
        self.r2             = np.empty(flen, dtype=np.double)
        self.r2[:]          = np.nan    
        self.theta1         = np.empty(flen, dtype=np.double)   # mean wave direction
        self.theta1[:]      = np.nan
        self.theta2         = np.empty(flen, dtype=np.double)   # principal wave direction
        self.theta2[:]      = np.nan
        
        
        # Derived fields
        
        self.wavenumber     = np.empty(slen, dtype=np.double)
        self.wavenumber[:]  = np.nan
            
        self.moment         = np.empty(3)       # Spectral moments
        self.moment[:]      = np.nan
        
        self.swh            = np.nan            # Significant wave height
        self.mwd            = np.nan            # Mean wave direction
        
        self.period_peak    = np.nan            # Dominant period         
        self.period_avg     = np.nan            # Average period
        self.period_zero    = np.nan            # Zero-crossing
        
    def PostProcess(self, freq_min: float = 0.03, interpolate = True, rolling_average: int = 3):
    
        g = 9.80665 # m s^-2
        pi = math.pi
        
        taper_freq_rng = [ 0.04, 0.08 ] 
    
        peak_zz = 0
        peak_idx = -1
    
        
    
        
        try: 
        
            if self.slen < 1:
                return
                
                
            # TODO interpolate acceleration spectra    
            
            if(interpolate):
            
                # Update derived fields array lengths
        
                self.a0             = np.empty(self.slen, dtype=np.double)
                self.a0[:]          = np.nan
                self.a1             = np.empty(self.slen, dtype=np.double)
                self.a1[:]          = np.nan
                self.a2             = np.empty(self.slen, dtype=np.double)
                self.a2[:]          = np.nan
                self.b1             = np.empty(self.slen, dtype=np.double)
                self.b1[:]          = np.nan
                self.b2             = np.empty(self.slen, dtype=np.double)
                self.b2[:]          = np.nan
                
                self.r1             = np.empty(self.slen, dtype=np.double)
                self.r1[:]          = np.nan
                self.r2             = np.empty(self.slen, dtype=np.double)
                self.r2[:]          = np.nan    
                self.theta1         = np.empty(self.slen, dtype=np.double)
                self.theta1[:]      = np.nan
                self.theta2         = np.empty(self.slen, dtype=np.double) 
                self.theta2[:]      = np.nan
                self.wavenumber     = np.empty(self.slen, dtype=np.double)
                self.wavenumber[:]  = np.nan
                
                # Do interpolation
            
                new_freq = np.array([ x*self.fsam/self.slen/2 for x in range(1, self.slen + 1)])
                    
                f = interp1d(self.freq, self.Szz_g, bounds_error = False, assume_sorted=True)
                self.Szz_g = f(new_freq) 
                f = interp1d(self.freq, self.Syy_g, bounds_error = False, assume_sorted=True)
                self.Syy_g = f(new_freq) 
                f = interp1d(self.freq, self.Sxx_g, bounds_error = False, assume_sorted=True)
                self.Sxx_g = f(new_freq) 
                f = interp1d(self.freq, self.Cxy_g, bounds_error = False, assume_sorted=True)
                self.Cxy_g = f(new_freq) 
                f = interp1d(self.freq, self.Qzx_g, bounds_error = False, assume_sorted=True)
                self.Qzx_g = f(new_freq) 
                f = interp1d(self.freq, self.Qzy_g, bounds_error = False, assume_sorted=True)
                self.Qzy_g = f(new_freq) 
                 
                self.freq = new_freq
                
                self.Szz_g = np.where(np.isnan(self.Szz_g), 0, self.Szz_g)
                self.Syy_g = np.where(np.isnan(self.Syy_g), 0, self.Syy_g)
                self.Sxx_g = np.where(np.isnan(self.Sxx_g), 0, self.Sxx_g)
                self.Cxy_g = np.where(np.isnan(self.Cxy_g), 0, self.Cxy_g)
                self.Qzx_g = np.where(np.isnan(self.Qzx_g), 0, self.Qzx_g)
                self.Qzy_g = np.where(np.isnan(self.Qzy_g), 0, self.Qzy_g)
            
            # TODO apply rolling average to acceleration spectra
            
            if rolling_average > 1:
            
                self.Szz_g = RollingAverage(self.Szz_g, rolling_average)
                self.Syy_g = RollingAverage(self.Syy_g, rolling_average)
                self.Sxx_g = RollingAverage(self.Sxx_g, rolling_average)
                self.Cxy_g = RollingAverage(self.Cxy_g, rolling_average)
                self.Qzx_g = RollingAverage(self.Qzx_g, rolling_average)
                self.Qzy_g = RollingAverage(self.Qzy_g, rolling_average)

            
                
            # TODO Apply taper to acceleration spectra    
                
            # for idx in range(0, int(self.slen/2)):
            
                # if self.freq[idx] > taper_freq_rng[1]:
                    # break

                # if self.freq[idx] < taper_freq_rng[0]:
                    # self.Szz_g[idx] = 0
                    # self.Syy_g[idx] = 0
                    # self.Sxx_g[idx] = 0
                    # self.Cxy_g[idx] = 0
                    # self.Qzx_g[idx] = 0
                    # self.Qzy_g[idx] = 0
                    # continue
                    
                # if np.isnan(self.Szz[idx]):
                    # self.Szz[:] = np.nan
                    # return
                 
                # self.Szz_g[idx] = self.Szz_g[idx] * ( ((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)    
                # self.Sxx_g[idx] = self.Sxx_g[idx] * ( ((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)    
                # self.Syy_g[idx] = self.Syy_g[idx] * ( ((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)    
                # self.Cxy_g[idx] = self.Cxy_g[idx] * ( ((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)    
                # self.Qzx_g[idx] = self.Qzx_g[idx] * ( ((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)    
                # self.Qzy_g[idx] = self.Qzy_g[idx] * ( ((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)    
                
            # TODO Convert acceleration spectra to displacement spectra

            factor = g**2 / ((2*pi*self.freq)**4)

            self.Szz = factor * self.Szz_g 
            self.Sxx = factor * self.Sxx_g 
            self.Syy = factor * self.Syy_g 
            self.Cxy = factor * self.Cxy_g 
            self.Qzx = factor * self.Qzx_g 
            self.Qzy = factor * self.Qzy_g 
            
            self.Szz = np.where(self.Szz > 500, np.nan, self.Szz)
            self.Sxx = np.where(self.Sxx > 500, np.nan, self.Sxx)
            self.Syy = np.where(self.Syy > 500, np.nan, self.Syy)
            self.Cxy = np.where(np.abs(self.Cxy) > 500, np.nan, self.Cxy)
            self.Qzx = np.where(np.abs(self.Qzx) > 500, np.nan, self.Qzx)
            self.Qzy = np.where(np.abs(self.Qzy) > 500, np.nan, self.Qzy)
                
                
            # Estimate wavenumber
            
            self.wavenumber = (2 * math.pi * self.freq[:]) ** 2 / g       
                
            # TODO Apply taper to displacement spectra
            
            for idx in range(0, len(self.freq)):
            
                if self.freq[idx] > taper_freq_rng[1]:
                    break

                if self.freq[idx] < taper_freq_rng[0]:
                    self.Sxx[idx] = 0
                    self.Syy[idx] = 0
                    self.Szz[idx] = 0
                    self.Cxy[idx] = 0
                    self.Qzy[idx] = 0
                    self.Qzx[idx] = 0
                    continue
                    
                if np.isnan(self.Szz[idx]):# or np.isnan(self.Syy[idx]) or np.isnan(self.Sxx[idx]) or np.isnan(self.Cxy[idx]) or np.isnan(self.Qzx[idx]) or np.isnan(self.Qzy[idx]):
                    self.Szz[:] = np.nan
                    self.Syy[:] = np.nan
                    self.Sxx[:] = np.nan
                    self.Cxy[:] = np.nan
                    self.Qzx[:] = np.nan
                    self.Qzy[:] = np.nan
                    return
                    
                taper = (((self.freq[idx] - taper_freq_rng[0]) / ( taper_freq_rng[1] - taper_freq_rng[0] )) ** 2)
                 
                self.Szz[idx] = self.Szz[idx] * taper
                self.Syy[idx] = self.Syy[idx] * taper
                self.Sxx[idx] = self.Sxx[idx] * taper
                self.Cxy[idx] = self.Cxy[idx] * taper
                self.Qzy[idx] = self.Qzy[idx] * taper
                self.Qzx[idx] = self.Qzx[idx] * taper
                
            # TODO apply rolling average
            
            # N=3 
            
            # self.Szz = np.cumsum(np.pad(self.Szz,(N//2,N//2),'constant'))
            
            # self.Szz[N:] = self.Szz[N:] - self.Szz[:-N]
            # self.Szz = self.Szz[N-1:] / N
            # self.Szz = np.round(self.Szz, 4)
        

        
            # Calculate derived fields
            
            self.moment[:]  = 0          
            
            for idx in range(0, len(self.freq)):
            
                if self.freq[idx] < freq_min: # exclude frequencies below cutoff
                    continue
                    
                if np.isnan(self.Szz[idx]):     # check if spectra is bad
                    
                    break;
                    
                # Calculate moments    
                    
                for i in range(0,3):
                    self.moment[i] = self.moment[i] + (self.freq[idx]**i * self.Szz[idx] * self.freq[0])
                    
                # Caclulate peak
                
                if self.freq[idx] < 0.06: # exclude frequencies below cutoff
                    continue

                if self.Szz[idx] > peak_zz:
                    peak_zz     = self.Szz[idx]
                    peak_idx    = idx
                    
            
            # Calculate directional spreading coeffs
            
            self.a0 = self.Szz / ((2*math.pi*self.freq) ** 4 * math.pi)
            self.a0 = np.where(self.a0 == 0, np.nan, self.a0)
            self.a1 = self.Qzx / ((2*math.pi*self.freq) ** 2 * self.wavenumber * math.pi)
            self.b1 = self.Qzy / ((2*math.pi*self.freq) ** 2 * self.wavenumber * math.pi)      
            self.a2 = ( self.Sxx - self.Syy ) / ( self.wavenumber ** 2 * math.pi )
            self.b2 = ( 2 * self.Cxy ) / ( self.wavenumber ** 2 * math.pi )
            
            self.r1 = np.sqrt((self.a1 ** 2) + (self.b1 ** 2)) / self.a0
            self.r2 = np.sqrt((self.a2 ** 2) + (self.b2 ** 2)) / self.a0
            self.theta1 = np.arctan2(self.b1, self.a1)
            self.theta2 = np.arctan2(self.b2, self.a2)
            

            


            
            # Check that we have good spectra
            
            if ~np.isnan(self.moment[0]) and (self.moment[0] > 0.):
            
                self.swh    = 4 * math.sqrt(self.moment[0]) # significant wave height
               
                self.period_peak = 1 / self.freq[peak_idx]
                self.period_avg  = self.moment[0] / self.moment[1]
                self.period_zero = math.sqrt(self.moment[0] / self.moment[2])
                
                
                # TODO Compute mean wave direction weighted average:
                
                
                
                
                sum_x = 0
                sum_y = 0

                for idx in range(0, len(self.freq)):
                
                    if self.freq[idx] < 0.06: # exclude frequencies below cutoff
                        continue
                        
                    if self.freq[idx] > 0.6: # exclude high freq
                        break
                        
                    
                    weight = math.sqrt(self.Szz[idx])
                    
                    sum_x = sum_x + (math.cos(self.theta1[idx]) * weight)
                    sum_y = sum_y + (math.sin(self.theta1[idx]) * weight)
                
                self.mwd = math.atan2(sum_y, sum_x)
                  
                # Convert angles to degrees
                
                self.mwd            = self.mwd * 180 / math.pi
                self.theta1         = self.theta1 * 180 / math.pi
                self.theta2         = self.theta2 * 180 / math.pi
                
                
                
                # self.theta1         = np.round(self.theta1/15, 0)
                # self.theta1         = self.theta1 * 15
                
                self.mwd            = np.round(self.mwd, 0)
                self.theta1         = np.round(self.theta1, 0)
                self.theta2         = np.round(self.theta2, 0)
                
                self.mwd            = self.mwd if self.mwd >=0 else self.mwd + 360
                self.theta1         = np.where(self.theta1 >= 0, self.theta1, self.theta1 + 360)
                self.theta2         = np.where(self.theta2 >= 0, self.theta2, self.theta2 + 360)
                  
            else:
            
                self.freq[:]        = np.nan
                self.Szz[:]         = np.nan
                self.Syy[:]         = np.nan
                self.Sxx[:]         = np.nan
                self.Cxy[:]         = np.nan
                self.Qzx[:]         = np.nan
                self.Qzy[:]         = np.nan
            
                self.a0[:]          = np.nan
                self.a1[:]          = np.nan
                self.a2[:]          = np.nan
                self.b1[:]          = np.nan
                self.b2[:]          = np.nan
                
                self.r1[:]          = np.nan
                self.r2[:]          = np.nan
                self.theta1[:]      = np.nan
                self.theta2[:]      = np.nan
            
                self.moment[:]      = np.nan
                
                self.swh            = np.nan            # Significant wave height
                
                self.period_peak    = np.nan            # Dominant period         
                self.period_avg     = np.nan            # Average period
                self.period_zero    = np.nan            # Zero-crossing

            


            # Convert arrays to lists
        
            self.freq           = self.freq.tolist()
            self.Szz_g          = self.Szz_g.tolist()
            self.Syy_g          = self.Syy_g.tolist()
            self.Sxx_g          = self.Sxx_g.tolist()
            self.Cxy_g          = self.Cxy_g.tolist()
            self.Qzx_g          = self.Qzx_g.tolist()
            self.Qzy_g          = self.Qzy_g.tolist()
            self.Szz            = np.round(self.Szz, 3).tolist()
            self.Syy            = np.round(self.Syy, 3).tolist()
            self.Sxx            = np.round(self.Sxx, 3).tolist()
            self.Cxy            = np.round(self.Cxy, 3).tolist()
            self.Qzx            = np.round(self.Qzx, 3).tolist()
            self.Qzy            = np.round(self.Qzy, 3).tolist()
            
            self.wavenumber     = np.round(self.wavenumber, 6).tolist()
            
            self.a0             = self.a0.tolist()
            self.a1             = self.a1.tolist()
            self.a2             = self.a2.tolist()
            self.b1             = self.b1.tolist()
            self.b2             = self.b2.tolist()
            
            self.r1             = np.round(self.r1, 3).tolist()
            self.r2             = np.round(self.r2, 3).tolist()
            self.theta1         = np.round(self.theta1, 0).astype(int).tolist()
            self.theta2         = np.round(self.theta2, 0).astype(int).tolist()
            self.mwd            = np.round(self.mwd, 0).astype(int).tolist()
        
            self.moment         = np.round(self.moment, 6).tolist()
            
            self.swh            = np.round(self.swh, 2).tolist()
            
            self.period_peak    = float(np.round(self.period_peak, 2)) # Dominant period         
            self.period_avg     = float(np.round(self.period_avg, 2))  # Average period
            self.period_zero    = float(np.round(self.period_zero, 2)) # Zero-crossing
            
        except: 
        
            raise Exception('Failed to post-process wave data.')

"""
Composite classes
"""        

class CompositePosition(Position):

    def __init__(self, pos: Position = None, compute = True):
        Position.__init__(self)
        self.dive = np.nan
        self.dt_epoch_min = 9999999999
        self.dt_epoch_max = -1
        self.num_pos = 0
        self.positions = []
        
        if pos is not None:
            self.add_position(pos = pos, compute = compute)
        
    def add_position(self, pos: Position, compute = True):
        
        try: 
    
            self.positions.append(pos)
            
            self.dt_epoch_min = min(self.dt_epoch_min, pos.dt_epoch)
            self.dt_epoch_max = max(self.dt_epoch_max, pos.dt_epoch)
            
            if compute:
                self.compute()
            
        except:
        
            raise Exception('Failed to append position.')
            
        return self
                
    def compute(self):
        
        try: 
        
            cep_sum = 0      # circular error probable, in km
            dt_epoch_sum = 0 # date/time in seconds since 1970-01-01T00:00:00Z
            x_sum = 0
            y_sum = 0
            z_sum = 0
            weight_sum = 0
            self.num_pos = 0
            
            # iterate over grouped positions
            
            for pos in self.positions:
            
                if not pos.is_valid(): 
                    continue
                    
                # convert lat/lon to unit vector
                    
                x = math.cos(math.radians(pos.lon)) * math.cos(math.radians(pos.lat))
                y = math.sin(math.radians(pos.lon)) * math.cos(math.radians(pos.lat))
                z = math.sin(math.radians(pos.lat))
                
                # calculate weight
                
                weight = 1/(pos.cep_km**2)
                weight_sum = weight_sum + weight
                
                # add weighted values to sums
                
                cep_sum = cep_sum + (1/(pos.cep_km**2))
                dt_epoch_sum = dt_epoch_sum + (pos.dt_epoch * weight)
                
                x_sum = x_sum + (x * weight)
                y_sum = y_sum + (y * weight)
                z_sum = z_sum + (z * weight)

                self.num_pos = self.num_pos + 1
                
            if(weight_sum == 0):
                raise Exception('Zero valid positions.')
                
            # get weighted averages
                        
            self.cep_km = round(math.sqrt(1/cep_sum), 3)
            
            self.dt_from_epoch(round(dt_epoch_sum/weight_sum))
            
            x = x_sum / weight_sum
            y = y_sum / weight_sum
            z = z_sum / weight_sum
            
            # convert vector to lat/lon

            self.lon = round(math.degrees(math.atan2(y,x)),4)
            self.lat = round(math.degrees(math.asin(
                z/math.sqrt(x**2+y**2+z**2) # normalize
                )),4)
           
        except:
        
            raise Exception('Failed to compute composite position.')
            
        return self
        
        