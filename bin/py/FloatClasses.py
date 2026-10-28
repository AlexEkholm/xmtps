#!/usr/bin/python3
#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""Data classes for one float dive, shared by the XMTPS Python tools.

xmtps-list2json.py parses a decoder listing file into a Dive object (its ascent and descent
pressure time series, drift and profile CTD samples, GPS fixes, SBD messages, pump runs, surface
samples, wave spectra, mission parameters and engineering messages), calls Dive.PostProcess() and
pickles the result to a per-dive JSON file with jsonpickle. xmtps-aggregate-json.py unpickles
those files, groups the dives into Float objects and aggregates them; it also uses
CompositePosition and PositionVector to combine and compare positions.

Because the objects are pickled, the attribute names and types are part of the JSON format: the
PostProcess() methods convert numpy arrays to lists and remove helper attributes before pickling.
Times are UTC; DateTime holds them as seconds since 1970-01-01T00:00:00Z and as ISO 8601 strings.

Author: aekholm (created 2021-09-21)
"""

import datetime
from dateutil import parser
from geographiclib.geodesic import Geodesic
import math
import numpy as np
import pytz
import re
from scipy.interpolate import interp1d

try:
    import ppigrf     # IGRF geomagnetic field model, for the magnetic declination
except ImportError:
    ppigrf = None

"""
Helper functions
"""

def MagneticDeclination(lat: float, lon: float, dt_epoch: float) -> float:
    """Return the magnetic declination (degrees, east of true north positive) from the IGRF.

    Args:
        lat, lon: position, in decimal degrees.
        dt_epoch: time, in seconds since 1970-01-01T00:00:00Z.

    Returns:
        The declination, or NaN if ppigrf is not installed or the inputs are invalid.
    """
    if ppigrf is None or dt_epoch is None or not (dt_epoch > 0) or np.isnan(lat) or np.isnan(lon):
        return np.nan
    date = datetime.datetime(1970, 1, 1) + datetime.timedelta(seconds=float(dt_epoch))
    b_e, b_n, _ = ppigrf.igrf(lon, lat, 0, date)
    return float(np.degrees(np.arctan2(np.ravel(b_e)[0], np.ravel(b_n)[0])))

def RollingAverage(array, N: int ):
    """Return the centred N-point moving average of a 1-D array, rounded to 5 decimals.

    The array is zero padded with N//2 values at each end and averaged with a cumulative sum, so
    the first and last N//2 outputs are biased toward zero. For odd N the result has the length
    of the input (for even N it has one element more). A NaN propagates to every later output.

    Args:
        array: 1-D array of values (e.g. a spectrum).
        N: window length in samples; N <= 1 returns the input unchanged (and unrounded).

    Returns:
        The smoothed numpy array.
    """

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
    
    """A UTC date/time held both as seconds since epoch and as an ISO 8601 string.

    Attributes:
        dt_epoch: seconds since 1970-01-01T00:00:00Z, or -1 if unset or invalid.
        dt_str: ISO 8601 string (e.g. '2021-09-21T15:11:38Z'), or '' if unset or invalid.

    Only times between epoch 1262322000 and 2209006800 (2010-01-01 and 2040-01-01, at 05:00Z)
    are accepted; anything outside that range is stored as the unset values above.
    """
    
    def __init__(self, dt = None, dt_epoch: float = None, dt_str: str = None):
        """Initialize from the first usable argument: dt, then dt_epoch, then dt_str.

        Args:
            dt: another DateTime to copy.
            dt_epoch: seconds since epoch; used only if > 0.
            dt_str: ISO 8601 date/time string; used only if non-empty.

        With no usable argument the object is unset (dt_epoch -1, dt_str '').
        """
        
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
        """Return dt_str parsed as a datetime.datetime, or None if it is not valid ISO 8601."""
        dt = None
        try: 
            dt = parser.isoparse(self.dt_str)
        except:
            dt = None
        return dt
        
    def dt_from_iso8601_str(self, dt_str):
        """Set the time from an ISO 8601 string and return self.

        dt_epoch is the parsed time rounded to whole seconds, and dt_str keeps the string as
        given. A string without a time zone is interpreted as local time by
        datetime.timestamp(). If the string can't be parsed or the time is outside the accepted
        range (see the class docstring), the object is set to dt_epoch -1 and dt_str ''.
        """
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
        """Set the time from seconds since epoch and return self.

        dt_epoch is stored as given (not rounded) and dt_str is formatted as
        'YYYY-MM-DDThh:mm:ssZ' in UTC. If the value can't be converted or is outside the accepted
        range (see the class docstring), the object is set to dt_epoch -1 and dt_str ''.
        """
    
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
        """Copy dt_epoch and dt_str from another DateTime; do nothing if dt is not a DateTime."""
        
        if dt is not None and isinstance(dt, DateTime):
            # self.dt_from_epoch(dt.dt_epoch)
            self.dt_epoch = dt.dt_epoch
            self.dt_str = dt.dt_str
        
    def is_valid(self):
        """Return True if dt_epoch is within the accepted range (2010-01-01 to 2040-01-01)."""
    
        return (self.dt_epoch >= 1262322000) and (self.dt_epoch <= 2209006800)
    
    def duration_to_hhmmss_str(duration_s: float, units = True):
        """Return a duration in seconds formatted as 'h:mm:ss' (one hour or more) or 'mm:ss'.

        This takes no self: call it on the class, as DateTime.duration_to_hhmmss_str(x).
        Seconds are truncated, not rounded.

        Args:
            duration_s: duration in seconds; NaN gives ''.
            units: if True, append ' [hh:mm:ss]' or ' [mm:ss]'.

        Raises:
            Exception: if the duration can't be formatted (e.g. it is not a number).
        """
    
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
    """A time-stamped geographic position with its uncertainty."""
    
    def __init__(self, dt: DateTime = None, lat: float = np.nan, lon: float = np.nan, cep_km: float = np.nan):
        """Initialize the position.

        Args:
            dt: time of the position (copied); unset if None.
            lat: latitude in decimal degrees north.
            lon: longitude in decimal degrees east.
            cep_km: circular error probable in km; NaN if unknown.
        """
        DateTime.__init__(self, dt)
        self.lat    = lat
        self.lon    = lon
        self.cep_km = cep_km
        
    def pos_from_iso6709_str(self, string):
        """Set lat and lon (decimal degrees) from an ISO 6709 string such as '+41.5246-070.6711'.

        Raises AttributeError if the string contains no signed latitude/longitude pair.
        """
        (self.lat, self.lon) = re.search('([+-][\\d.]+)([+-][\\d.]+)', string).groups()
        self.lat = float(self.lat)
        self.lon = float(self.lon)
        
    def is_valid(self):
        """Return True if the time is valid, lat/lon are in range and cep_km is positive.

        A position with an unknown (NaN) or sentinel (e.g. -9) cep_km is therefore invalid.
        """
    
        return DateTime.is_valid(self) and (self.lat >= -90) and (self.lat <= 90) and (self.lon >= -180) and (self.lon <= 180) and (self.cep_km > 0)
   
class PositionVector:
    """The displacement from one position to a later one: distance, bearing and speed."""

    def __init__(self, pos1: Position = Position(), pos2: Position = Position, compute = False):
        """Initialize the vector from pos1 to pos2, computing it now if compute is True.

        Attributes:
            dist_m: geodesic (WGS84) distance in metres, rounded to 1 m.
            azi: initial azimuth at pos1 in degrees clockwise from north, [0, 360), to 0.1.
            duration_s: pos2 time minus pos1 time, in seconds.
            speed_mps: dist_m / duration_s in m/s, rounded to 4 decimals.
            pos1, pos2: the start and end positions.

        The four results are NaN until compute() is called with two valid positions.
        """
        self.dist_m = np.nan
        self.azi = np.nan
        self.duration_s = np.nan
        self.speed_mps = np.nan
        
        self.pos1 = pos1
        self.pos2 = pos2
        
        if(compute):
            self.compute()
        

    def compute(self):
        """Compute dist_m, azi, duration_s and speed_mps from pos1 and pos2.

        Uses the WGS84 inverse geodesic problem (geographiclib). If either position is invalid
        (see Position.is_valid), all four results are set to NaN and self is returned; otherwise
        returns None. Equal times raise ZeroDivisionError.
        """
    
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
    """A pressure time series (the ascent or descent pressure/time samples of a dive)."""
    
    def __init__(self, num: int = 0):
        """Initialize an empty time series.

        Attributes:
            dt_ref: reference time (DateTime); the listing's F/R line time while parsing, the
                earliest sample time after PostProcess().
            n: number of samples.
            dt_ref_delta_sec: sample times in seconds after dt_ref.
            dt: sample times as DateTime objects; filled while parsing and removed by
                PostProcess().
            dt_epoch: sample times in seconds since epoch (filled by PostProcess()).
            dt_str: sample times as ISO 8601 strings (filled by PostProcess()).
            pres: pressures in dBar.
            rate: pressure rates in dBar/s, positive for decreasing pressure (filled by
                PostProcess()).
            phase_code: phase code of each sample, as listed.
        """
        self.dt_ref           = DateTime()
        self.n                = num
        self.dt_ref_delta_sec = []
        self.dt               = []
        self.dt_epoch         = []
        self.dt_str           = []
        self.pres             = []
        self.rate             = []
        self.phase_code       = []
        
    def PostProcess(self):
        """Insert midpoints, compute pressure rates and fill the time arrays for pickling.

        dt_ref is reset to the earliest sample time and dt_ref_delta_sec recomputed from it.
        With two or more samples, a midpoint (mean pressure, rounded to 0.01 dBar, and mean time,
        rounded to 0.1 s) is inserted between each pair of samples, so n becomes 2n-1. Rates in
        dBar/s (rounded to 4 decimals; positive when pressure decreases) are:

        - at a midpoint: the difference between its two neighbouring samples;
        - at an original sample: the difference between the midpoints either side of it;
        - NaN at the first and last sample.

        Where the two times are equal, the rate of the previous point is repeated. A midpoint
        gets phase code 10 after a sample with code 1, 2, 3 or 10, 12 after 6 or 12, 13 after
        7 or 13, and -1 otherwise. Finally dt_epoch and dt_str are filled from dt_ref and
        dt_ref_delta_sec and the dt attribute is deleted.

        With no samples, dt_ref is unset and dt deleted. With one sample, nothing else is done:
        dt is kept and dt_epoch, dt_str and rate stay empty.
        """
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
            new_phase_code       = np.empty(new_len, dtype='int')
        
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
                
                # Generate new phase codes
                
                new_phase_code[2*i] = self.phase_code[i]
                
                if self.phase_code[i] == 1:
                    new_phase_code[2*i+1] = 10
                elif self.phase_code[i] == 2:
                    new_phase_code[2*i+1] = 10
                elif self.phase_code[i] == 3:
                    new_phase_code[2*i+1] = 10
                elif self.phase_code[i] == 6:
                    new_phase_code[2*i+1] = 12
                elif self.phase_code[i] == 7:
                    new_phase_code[2*i+1] = 13
                elif self.phase_code[i] == 10:
                    new_phase_code[2*i+1] = 10
                elif self.phase_code[i] == 12:
                    new_phase_code[2*i+1] = 12
                elif self.phase_code[i] == 13:
                    new_phase_code[2*i+1] = 13
                else:
                    new_phase_code[2*i+1] = -1  
                
                
            new_pres[2*(old_len-1)] = self.pres[old_len-1]
            new_dt_ref_delta_sec[2*(old_len-1)] = self.dt_ref_delta_sec[old_len-1]
            new_rate[2*(old_len-1)] = None
            new_phase_code[2*(old_len-1)] = self.phase_code[old_len-1]
            
            self.dt_ref_delta_sec = new_dt_ref_delta_sec.tolist()
            self.pres = new_pres.tolist()
            self.rate = new_rate.tolist()
            self.n = new_len
            self.phase_code = new_phase_code.tolist()
                
            # Create date/time array
            
            self.dt  = [ DateTime().dt_from_epoch(self.dt_ref.dt_epoch + x) for x in self.dt_ref_delta_sec ]
            self.dt_epoch  = [ x.dt_epoch for x in self.dt ]
            self.dt_str  = [ x.dt_str for x in self.dt ]
            del self.__dict__["dt"]
                
class EngineeringData:
    """Base class for the dive-phase engineering messages (descent, drift, ascent, surface)."""
    
    def __init__(self, dt_start = DateTime(), vac0 = -9.99, vac1 = -9.99):
        """Initialize the attributes common to the dive-phase engineering messages.

        Args:
            dt_start: start time of the dive phase (DateTime).
            vac0: case vacuum at the start of the phase, in inHg; -9.99 if not reported.
            vac1: case vacuum at the end of the phase, in inHg; -9.99 if not reported.
        """
        self.dt_start = dt_start
        self.vac0     = vac0               
        self.vac1     = vac1
                
"""
Dive Data classes:
"""

class Ascent(PresTimeSeries):
    """Ascent pressure time series (the listing's R and r lines)."""
    
    def __init__(self, num: int = 0):
        PresTimeSeries.__init__(self, num)
        
    def PostProcess(self):
        """Post-process as PresTimeSeries.PostProcess() (returns None)."""
        super().PostProcess()
        
class Descent(PresTimeSeries):
    """Descent pressure time series (the listing's F and f lines)."""
    
    def __init__(self, num: int = 0):
        PresTimeSeries.__init__(self, num)
        
    def PostProcess(self):
        """Post-process as PresTimeSeries.PostProcess() (returns None)."""
        super().PostProcess()
        
class Dive:
    """All the data decoded from one dive's listing file; the object pickled to a dive's JSON."""
    
    def __init__(self, json_vers = "?"):
        """Initialize an empty dive.

        Attributes:
            json_vers: version of the JSON format that wrote the object.
            id: float identifiers (FloatId).
            list_file: the listing file the dive was read from (File).
            dive_num: dive number; -999 until read from the LIS line (negative numbers are
                pre-deployment dives).
            ascent, descent: pressure time series (Ascent, Descent).
            drift: drift CTD samples (Drift).
            engineering: engineering messages (Engineering).
            gps: GPS fixes ([Gps]).
            mission: mission configuration (MissionParameters).
            profile: profile CTD bins (Profile).
            pump: pump runs (Pump).
            sbd: Iridium SBD messages ([Sbd]).
            surface: surface CTD samples (Surface).
            wave: wave spectra (Wave), or None; the attribute is deleted by PostProcess() if
                the dive has no wave data.
        """
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
   
    def SurfaceFix(self):
        """Return the GPS fix of the surfacing that ends the dive, for the wave measurement.

        This is the first valid fix with idx 0 or 2 (as for the profile position), else the first
        valid fix of the dive, else None.
        """
        fixes = [g for g in self.gps if g.is_valid()]
        for g in fixes:
            if g.idx in (0, 2):
                return g
        return fixes[0] if fixes else None

    def PostProcess(self, fallback_fix: 'Gps' = None):
        """Post-process every part of the dive and set the drift sample times, before pickling.

        The wave spectra are post-processed with the position of SurfaceFix(), for the magnetic
        declination; if the dive has no valid fix, fallback_fix (the most recent valid fix of
        the float, from an earlier dive) is used instead, if given.

        Drift sample times are the measured ones if every sample has one (see
        Drift.HasMeasuredDatetimes); otherwise, if there are ascent and descent samples, they are
        estimated by spacing the samples evenly between the last descent sample time and the
        first ascent sample time (Drift.EstimateDatetimes).
        """
        self.ascent.PostProcess()
        self.descent.PostProcess()
        self.drift.PostProcess()
        self.profile.PostProcess()
        self.pump.PostProcess()
        self.surface.PostProcess()
        if self.wave is not None:
            fix = self.SurfaceFix()
            if fix is None and fallback_fix is not None and fallback_fix.is_valid():
                fix = fallback_fix
            if fix is None:
                self.wave.PostProcess()
            else:
                self.wave.PostProcess(lat=fix.lat, lon=fix.lon, dt_epoch=fix.dt_epoch)
        else:
            del self.wave
        
        if self.drift.HasMeasuredDatetimes():
            self.drift.SetDatetimes(self.drift.dt_epoch)
        elif self.ascent.n > 0 and self.descent.n > 0:
            self.drift.EstimateDatetimes(
                self.descent.dt_epoch[-1],
                self.ascent.dt_epoch[0]
                )

class Drift:
    """CTD samples taken while the float drifts at its park depth (the listing's D/d lines)."""
    
    def __init__(self, num_samples = 0):
        """Initialize num_samples drift samples, all NaN.

        Attributes:
            dt_epoch: sample times in seconds since epoch; NaN where not measured.
            dt_str: sample times as ISO 8601 strings; NaN until set.
            dt_ref: time of the first sample (DateTime) once times are set.
            dt_ref_delta_sec: sample times in seconds after dt_ref.
            pres: pressure in dBar.
            temp: temperature in degC.
            saln: salinity in PSU.
            dens: in-situ density in kg/m^3 (RhoLUT lookup; NaN outside its table).
            sigma0: potential density minus 1000, in kg/m^3 (RhoLUT lookup; NaN outside it).
            n: number of samples.

        The CTD and density arrays are numpy arrays until PostProcess() converts them to lists.
        """
        self.dt_epoch         = [np.nan] * num_samples
        self.dt_str           = [np.nan] * num_samples
        self.dt_ref           = DateTime()
        self.dt_ref_delta_sec = [np.nan] * num_samples
        self.pres             = np.full(num_samples, np.nan)
        self.temp             = np.full(num_samples, np.nan)
        self.saln             = np.full(num_samples, np.nan)
        self.dens             = np.full(num_samples, np.nan)
        self.sigma0           = np.full(num_samples, np.nan)
        self.n                = num_samples
        
    def HasMeasuredDatetimes(self):
        """Return True if every drift sample has a measured time.

        Measured times come from the time column of the d lines (relative to the drift start on
        the D line), sent only by floats with the drift-times message.
        """
        return self.n > 0 and all(x == x for x in self.dt_epoch)   # x == x is False for NaN

    def EstimateDatetimes(self, epoch_start, epoch_end):
        """Estimate the sample times by spacing the samples evenly between two times.

        The n samples are placed at the interior points of n+2 evenly spaced times from
        epoch_start to epoch_end (so neither end time is used), rounded to whole seconds, and
        set with SetDatetimes(). With no samples, the time attributes are set to empty lists
        (dt_ref too).

        Args:
            epoch_start: start of the drift (the last descent sample time), seconds since epoch.
            epoch_end: end of the drift (the first ascent sample time), seconds since epoch.
        """

        if self.n == 0:
            self.dt_epoch = []
            self.dt_str   = []
            self.dt_ref   = []
            self.dt_ref_delta_sec   = []
            return

        self.SetDatetimes(np.round(np.linspace(epoch_start, epoch_end, self.n+2)[1:-1], 0).tolist())

    def SetDatetimes(self, dt_epoch):
        """Set the sample times from a sequence of seconds since epoch.

        Fills dt_epoch and dt_str, sets dt_ref to the first sample's time and dt_ref_delta_sec
        to each sample's seconds after it. dt_epoch must not be empty.
        """

        self.dt_epoch = list(dt_epoch)
        self.dt_str = [ DateTime().dt_from_epoch(x).dt_str for x in self.dt_epoch ]
        self.dt_ref = DateTime().dt_from_epoch(self.dt_epoch[0])
        self.dt_ref_delta_sec = [ x - self.dt_ref.dt_epoch for x in self.dt_epoch ]
        
    def PostProcess(self):
        """Convert the CTD and density arrays to lists for pickling (times are set by Dive)."""
        self.pres     = self.pres.tolist()
        self.temp     = self.temp.tolist()
        self.saln     = self.saln.tolist()
        self.dens     = self.dens.tolist()
        self.sigma0   = self.sigma0.tolist()
        
class Engineering():
    """Engineering data for one dive, one object per listing section.

    The sections are descent (E07), drift (E08), ascent (E09), surface (E0A), ice (E0C) and the
    built-in test (BIT, E05 passed / E06 failed). Each section object has an attribute for every
    column of every version of its engineering message; the ENG_COLUMNS table in
    xmtps-list2json.py maps listing columns to these attributes. Physical quantities are named
    with their units (e.g. _dbar, _degc, _psu, _v, _a, _s, _inhg, _counts); identifiers, counts
    and status fields such as version, seeks, mode, results and the vac0/vac1 vacuums (inHg)
    are not. Attributes that a float's message version does not report keep their default:
    NaN, '', an unset DateTime, or -9.99 for vac0/vac1.
    """

    def __init__(self):
        """Initialize empty section objects, the error list and the extra-column dictionary.

        Attributes:
            ascent, descent, drift, surface, ice, bit: the section objects.
            errors: one dict per engineering error (e0x) line, with 'section', 'idx', 'code'
                and, where the line has one, 'dt_str' (surface v0x10 error lines have no
                date-time).
            extra: {section: {column: value}} for listing columns not in ENG_COLUMNS.
        """
        self.ascent  = EngineeringAscent()
        self.descent = EngineeringDescent()
        self.drift   = EngineeringDrift()
        self.surface = EngineeringSurface()
        self.ice     = EngineeringIce()
        self.bit     = EngineeringBit()
        self.errors  = []   # [{'section', 'idx', 'dt_str', 'code'}] from the e0x error lines
        self.extra   = {}   # {section: {column: value}} for listing columns not in ENG_COLUMNS

class EngineeringDescent(EngineeringData):
    """Descent engineering message (E07, versions 0x10-0x11)."""

    def __init__(self, dt_start = None, vac0 = -9.99, vac1 = -9.99):
        """Initialize every attribute to its not-reported default (dt_start None: unset).

        The trailing comments give each attribute's listing column, units and message versions.
        """
        EngineeringData.__init__(self, dt_start if dt_start else DateTime(), vac0, vac1)
        self.version                         = ''       # engVer
        # dt_start: start-of-descent; vac0, vac1: case vacuum at start/end (inHg), v0x11
        self.seeks                           = np.nan   # seeks, v0x11
        self.seek_pump_adj_s                 = np.nan   # seekPumpAdjT (+sec), v0x11
        self.oil_valve_open_pos_surf_v       = np.nan   # OilValveOpenPosSurf (V), v0x11
        self.oil_valve_close_pos_fall_pump_v = np.nan   # OilValveClosePosFallPump (V), v0x11
        self.air_valve_mid_pos_surf_v        = np.nan   # AirValveMidPosSurf (V), v0x11
        self.pump_time_s                     = np.nan   # pumpT (sec), v0x10
        self.pump_current_a                  = np.nan   # pumpI (A), v0x10
        self.system_current_a                = np.nan   # sysI (A), v0x10
        self.system_voltage_v                = np.nan   # sysV (V), v0x10
        self.case_vac_inhg                   = np.nan   # vac (inHg), v0x10
        self.first_pump_adj_s                = np.nan   # pumpAdj (+sec), v0x10

class EngineeringDrift(EngineeringData):
    """Park/drift engineering message (E08, versions 0x10-0x12)."""

    def __init__(self, dt_start = None, vac0 = -9.99, vac1 = -9.99):
        """Initialize every attribute to its not-reported default (dt_start None: unset).

        The trailing comments give each attribute's listing column, units and message versions.
        """
        EngineeringData.__init__(self, dt_start if dt_start else DateTime(), vac0, vac1)
        self.version                         = ''       # engVer
        # dt_start: start-of-drift; vac0, vac1: case vacuum at start/end (inHg), v0x11+
        self.seeks                           = np.nan   # seeks, v0x11+
        self.seek_pump_adj_s                 = np.nan   # seekPumpAdjT (+sec), v0x11+
        self.mean_pres_dbar                  = np.nan   # meanP (v0x10) / pres (v0x11+) (dBar)
        self.mean_pres_counts                = np.nan   # pres (counts), v0x11+
        self.mean_temp_degc                  = np.nan   # meanT (v0x10) / temp (v0x11+) (degC)
        self.mean_temp_counts                = np.nan   # temp (counts), v0x11+
        self.mean_saln_psu                   = np.nan   # saln (PSU), v0x11+
        self.mean_saln_counts                = np.nan   # saln (counts), v0x11+
        self.dt_fall_start                   = DateTime()   # start-of-fall, v0x10
        self.fall_start_pres_dbar            = np.nan   # fallP (dBar), v0x10

class EngineeringAscent(EngineeringData):
    """Ascent engineering message (E09, versions 0x10-0x12)."""

    def __init__(self, dt_start = None, vac0 = -9.99, vac1 = -9.99):
        """Initialize every attribute to its not-reported default (dt_start None: unset).

        The trailing comments give each attribute's listing column, units and message versions.
        """
        EngineeringData.__init__(self, dt_start if dt_start else DateTime(), vac0, vac1)
        self.version                         = ''       # engVer
        # dt_start: start-of-ascent; vac0, vac1: case vacuum at start/end (inHg), v0x11+
        self.system_current_a                = np.nan   # sysI (A), v0x10
        self.pump_time_s                     = np.nan   # pumpT (sec), v0x10
        self.dt_fall_start                   = DateTime()   # start-of-fall, v0x12
        self.fall_start_pres_dbar            = np.nan   # fallP (dBar), v0x12

class EngineeringSurface(EngineeringData):
    """Surface engineering message (E0A, versions 0x10-0x14)."""

    def __init__(self, dt_start = None, vac0 = -9.99, vac1 = -9.99):
        """Initialize every attribute to its not-reported default (dt_start None: unset).

        The trailing comments give each attribute's listing column, units and message versions.
        """
        EngineeringData.__init__(self, dt_start if dt_start else DateTime(), vac0, vac1)
        self.version                         = ''       # engVer
        # dt_start: start-of-surface; vac0, vac1: case vacuum at start/end (inHg), v0x11+
        self.ctd_status                      = ''       # CTD-status, v0x10
        self.case_vac_inhg                   = np.nan   # vac (inHg), v0x10
        self.system_voltage_v                = np.nan   # sysV (V), v0x10
        self.system_current_a                = np.nan   # sysI (A), v0x10
        self.num_sbd                         = np.nan   # nSBD
        self.num_prev_attempts               = np.nan   # nAtmpt (v0x10) / nPrevAttempt
        self.air_pump_current_a              = np.nan   # APCur (A)
        self.air_pump_runtime_s              = np.nan   # APRunT (sec)
        self.air_bladder_max_pres_psi        = np.nan   # AirBladMxP (PSI)
        self.air_valve_open_start_v          = np.nan   # AVOpenStart (V)
        self.air_valve_mid_end_v             = np.nan   # AVMidEnd (V)
        self.air_valve_open_cal_v            = np.nan   # ACVpenCal (V)
        self.air_valve_mid_cal_v             = np.nan   # AVMidCal (V)
        self.air_valve_close_cal_v           = np.nan   # AVCloseCal (V)
        self.oil_valve_open_air_pump_v       = np.nan   # OVOpenAirPmp (V)
        self.oil_valve_close_end_v           = np.nan   # OVCloseEnd (V)
        self.oil_valve_open_cal_v            = np.nan   # OVOpenCal (V)
        self.oil_valve_close_cal_v           = np.nan   # OVClosCal (V)
        self.pres_offset_dbar                = np.nan   # presOffset (dBar), v0x12+
        self.prof_first_pres_dbar            = np.nan   # firstPres (dBar), v0x13+
        self.prof_first_pres_counts          = np.nan   # firstPres (counts)
        self.prof_first_temp_degc            = np.nan   # firstTemp (degC)
        self.prof_first_temp_counts          = np.nan   # firstTemp (counts)
        self.prof_first_saln_psu             = np.nan   # firstSaln (PSU)
        self.prof_first_saln_counts          = np.nan   # firstSaln (counts)
        self.prof_last_pres_dbar             = np.nan   # lastPres (dBar), v0x13+
        self.prof_last_pres_counts           = np.nan   # lastPres (counts)
        self.prof_last_temp_degc             = np.nan   # lastTemp (degC)
        self.prof_last_temp_counts           = np.nan   # lastTemp (counts)
        self.prof_last_saln_psu              = np.nan   # lastSaln (PSU)
        self.prof_last_saln_counts           = np.nan   # lastSaln (counts)
        self.prof_num_bins                   = np.nan   # numBins, v0x13+
        self.prof_num_empty_bins             = np.nan   # numEmptyBins
        self.prof_num_raw_samples            = np.nan   # numRawSamples
        self.gps_rtc_offset_s                = np.nan   # gpsOffset (sec), v0x13+
        self.dt_sampling_start               = DateTime()   # start (surface sampling), v0x14
        self.dt_sampling_end                 = DateTime()   # end (surface sampling), v0x14
        self.num_errors                      = np.nan   # nErr

class EngineeringIce:
    """Ice engineering message (E0C, versions 0x10-0x11)."""

    def __init__(self):
        """Initialize every attribute to its not-reported default.

        The trailing comments give each attribute's listing column, units and message versions.
        """
        self.version                         = ''       # engVer
        self.mode                            = np.nan   # mode
        self.ice_status                      = np.nan   # ice-status
        self.urgency                         = np.nan   # urgency
        self.surface_attempts                = np.nan   # surfAttempts
        self.num_stored_sbd                  = np.nan   # numStoredSBD
        self.time_since_last_xmit_s          = np.nan   # lastXmit (sec)
        self.min_pres_dbar                   = np.nan   # minPres (dBar)
        self.test_temp                       = np.nan   # testTemp (listed as raw counts)
        self.test_saln_psu                   = np.nan   # testSalinity (PSU)
        self.num_errors                      = np.nan   # nErr, v0x11

class EngineeringBit:
    """Built-in test messages (E05 passed / E06 failed; BIT versions 6-8 and 9-11)."""

    def __init__(self):
        """Initialize every attribute to its not-reported default.

        The trailing comments give each attribute's listing column, units and message versions.
        """
        self.version                         = np.nan   # BITVer
        self.sw_version                      = ''       # SWVer (v6-8) / FWVer (v9-11)
        self.results                         = ''       # BITResults
        self.sensor_type                     = np.nan   # sensorType, v9-11
        self.sensor_type_name                = ''       # sensorTypeName, v9-11
        self.duration_s                      = np.nan   # BITTime (v6-8) / duration (sec)
        self.dt_start                        = DateTime()   # startTime, v9-11
        self.dt_end                          = DateTime()   # endTime, v9-11
        self.ctd_status                      = ''       # CTDStat (v6-8) / status (v9-11)
        self.ctd_errors                      = ''       # errors (CTD), v9-11
        self.pres_dbar                       = np.nan   # pres (dBar), v7
        self.pres_counts                     = np.nan   # pres (counts), v7
        self.pres_post_reset_dbar            = np.nan   # presPostReset (dBar) (v8) / presPostCal
        self.pres_post_reset_counts          = np.nan   # presPostReset (counts), v8
        self.pres_pre_reset_dbar             = np.nan   # presPreReset (dBar) (v8) / presPreCal
        self.pres_pre_reset_counts           = np.nan   # presPreReset (counts), v8
        self.pres_offset_dbar                = np.nan   # presOffset (dBar)
        self.temp_degc                       = np.nan   # temp (degC)
        self.temp_counts                     = np.nan   # temp (counts), v7-8
        self.saln_psu                        = np.nan   # saln (PSU)
        self.saln_counts                     = np.nan   # saln (counts), v7-8
        self.case_vac_errors                 = ''       # errors (Case Vacuum), v9-11
        self.case_vac_start_inhg             = np.nan   # VacStart / caseVacStart (inHg)
        self.case_vac_end_inhg               = np.nan   # VacEnd / caseVacEnd (inHg)
        self.battery_errors                  = ''       # errors (Battery Voltage), v9-11
        self.battery_start_v                 = np.nan   # BatStart / batVoltStart (V)
        self.battery_end_v                   = np.nan   # BatEnd / batVoltEnd (V)
        self.air_valve_open_v                = np.nan   # AVOpenPos / open (V)
        self.air_valve_open_cal_v            = np.nan   # AVOpenPosCal / open, calibration (V)
        self.air_valve_mid_v                 = np.nan   # AVMidPos / mid (V)
        self.air_valve_mid_cal_v             = np.nan   # AVMidPosCal / mid, calibration (V)
        self.air_valve_closed_v              = np.nan   # AVClosePos / closed (V)
        self.air_valve_closed_cal_v          = np.nan   # AVClosePosCal / closed, calibration (V)
        self.air_pump_errors                 = ''       # errors (Air Pump), v9-11
        self.air_pump_test_s                 = np.nan   # APTestTime / duration (sec)
        self.air_pump_cycles                 = np.nan   # APCycleCount / cycles
        self.air_pump_max_pres_psi           = np.nan   # APMxAPres / presMax (PSI)
        self.air_pump_current_avg_a          = np.nan   # currAvg (A), v9-11
        self.air_pump_current_max_a          = np.nan   # APMxLoadCur / currMax (A)
        self.air_pump_current_max_12v_a      = np.nan   # APMxLoadCur12V / 12VCurrMax (A)
        self.air_pump_battery_max_v          = np.nan   # APMxLoadBatV / batMax (V)
        self.air_pump_power_max_w            = np.nan   # powerMax (W), v9-11
        self.oil_valve_open_v                = np.nan   # OVOpenPos / open (V)
        self.oil_valve_open_cal_v            = np.nan   # OVOpenPosCal / open, calibration (V)
        self.oil_valve_closed_v              = np.nan   # OVClosePos / closed (V)
        self.oil_valve_closed_cal_v          = np.nan   # OVClosePosCal / closed, calibration (V)
        self.oil_pump_errors                 = ''       # errors (Oil Pump), v9-11
        self.oil_pump_test_s                 = np.nan   # OPTestTime / duration (sec)
        self.oil_pump_current_avg_a          = np.nan   # currAvg (A), v9-11
        self.oil_pump_current_max_a          = np.nan   # OPMxLoadCur / currMax (A)
        self.oil_pump_current_max_12v_a      = np.nan   # OPMxLoadCur12V / 12VCurrMax (A)
        self.oil_pump_battery_max_v          = np.nan   # OPMxLoadBatV / batMax (V)
        self.oil_pump_power_max_w            = np.nan   # powerMax (W), v9-11

class File:
    """A file the data was read from."""
    
    def __init__(self):
        """Initialize path (the file path, '') and mtime (its modification time, a DateTime)."""
        self.path = ''
        self.mtime = DateTime()

class FloatId:
    """The identifiers of a float; -1 where unknown."""
   
    def __init__(self, whoi = -1, dac = -1, wmo = -1):
        """Initialize the identifiers.

        Args:
            whoi: WHOI float serial number (the listing's SN).
            dac: Argo data assembly centre number, as listed on the LIS line.
            wmo: WMO platform number.
        """
        self.whoi = whoi
        self.dac  = dac
        self.wmo  = wmo
        
class Float: 
    """One float and its dives, as grouped by xmtps-aggregate-json.py."""
        
    def __init__(self, float_id = FloatId()):
        """Initialize with the float's FloatId (id) and an empty list of Dive objects (dives)."""
        self.dives = []
        self.id = float_id
   
class Gps(Position):
    """A GPS fix (the listing's GPS line); -1 (or -9 for hdop) where not set."""
    
    def __init__(self):
        """Initialize an empty fix.

        Attributes:
            idx: index of the fix in the listing.
            ttf_sec: time to fix, in seconds.
            nsat: number of satellites used.
            snr_min, snr_avg, snr_max: minimum, mean and maximum satellite SNR (dB-Hz).
            hdop: horizontal dilution of precision.

        Time, lat, lon and cep_km are inherited from Position; cep_km is set by
        EstimateUncertainty().
        """
        Position.__init__(self)
        self.idx      = -1
        self.ttf_sec  = -1
        self.nsat     = -1
        self.snr_min  = -1
        self.snr_avg  = -1
        self.snr_max  = -1
        self.hdop     = -9.

    def EstimateUncertainty(self):
        """Estimate the fix's circular error probable (cep_km) from its quality fields.

        Starts from 6.66 m and adds 16.66 m per unit of HDOP above 1, 26.66 m times the square
        of the number of satellites below 6, and 1.666 m per dB of mean SNR below 25. The result
        is in km, rounded to 1 m. If the time or position is invalid, cep_km is set to -9 (which
        makes the fix invalid).
        """
        
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
    """The mission configuration reported for a dive (the listing's mission lines)."""

    def __init__(self, fwver = '', ctdtype= ''):
        """Initialize with the firmware version (fwver) and CTD type (ctdtype) strings.

        cfgver, parameters and entries are described by their trailing comments; parameters
        values are floats where numeric, otherwise strings.
        """
        self.fwver = fwver
        self.ctdtype = ctdtype
        self.cfgver = ''            # mission configuration message version (MIS line)
        self.parameters = {}        # {name: value}
        self.entries = []           # [{'section', 'idx', 'name', 'value', 'units'}]

class Profile:
    """The binned CTD profile measured during the ascent (the listing's P/p lines)."""
    
    def __init__(self, num_bins = 0):
        """Initialize num_bins profile bins, all NaN.

        Attributes:
            pres: pressure in dBar.
            temp: temperature in degC.
            tcnd: temperature of the conductivity cell sensor (temp2 column) in degC, if sent.
            saln: salinity in PSU.
            saln_corr: dynamically corrected salinity in PSU, if sent.
            dens: in-situ density in kg/m^3 (RhoLUT lookup; NaN outside its table).
            sigma0: potential density minus 1000, in kg/m^3 (RhoLUT lookup).
            n: number of bins.

        The arrays are numpy arrays until PostProcess() converts them to lists.
        """
        self.pres      = np.empty(num_bins)
        self.pres[:]   = np.nan
        self.temp      = np.empty(num_bins)
        self.temp[:]   = np.nan
        self.tcnd      = np.empty(num_bins)
        self.tcnd[:]   = np.nan
        self.saln      = np.empty(num_bins)
        self.saln[:]   = np.nan
        self.saln_corr = np.full(num_bins, np.nan)   # dynamically corrected salinity, if sent
        self.dens      = np.empty(num_bins)
        self.dens[:]   = np.nan
        self.sigma0    = np.empty(num_bins)
        self.sigma0[:] = np.nan
        self.n         = num_bins
        
    def PostProcess(self):
        """Convert the profile arrays to lists for pickling."""
    
        self.pres   = self.pres.tolist()
        self.temp   = self.temp.tolist()
        self.tcnd   = self.tcnd.tolist()
        self.saln   = self.saln.tolist()
        self.saln_corr = self.saln_corr.tolist()
        self.dens   = self.dens.tolist()
        self.sigma0 = self.sigma0.tolist()

class Pump:
    """The oil pump runs of a dive (the listing's pmp lines), one list entry per run."""
    
    def __init__(self):
        """Initialize empty lists of pump runs.

        Attributes:
            pres: pressure at the run, in dBar.
            time: pump run time, in seconds.
            volt: pump voltage, in V.
            curr: pump current, in A.
            curr_adj: adjusted current, in A (computed by PostProcess()).
            vac: case vacuum after pumping, in inHg.
            power: power, in W (computed by PostProcess()).
            energy: energy, in J (computed by PostProcess()).
            n: number of runs; -1 until PostProcess().
        """
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
        """Set n and compute the adjusted current, power and energy of each pump run.

        curr_adj = curr * 12 / volt / 0.85 (A, rounded to 3 decimals; NaN if volt is 0),
        power = volt * curr_adj (W, 3 decimals) and energy = power * time (J, 2 decimals).
        """
        self.n = len(self.pres)
        
        self.curr_adj = [ round(c * 12 / v / 0.85, 3) if v else np.nan for (c, v) in zip(self.curr, self.volt) ]
        self.power = [ round(v * i, 3) for (i, v) in zip(self.curr_adj, self.volt) ]
        self.energy = [ round(p * t, 2) for (p, t) in zip(self.power, self.time) ] 
   
class Sbd(Position):
    """An Iridium SBD message (the listing's SBD line), positioned by the Iridium network."""

    def __init__(self):
        """Initialize an empty message; -1 where not set.

        Attributes:
            size_b: message size in bytes.
            momsn: mobile-originated message sequence number.
            mtmsn: mobile-terminated message sequence number.
            pid: packet identifier.

        The time, lat, lon and cep_km (Iridium CEP radius in km) are inherited from Position.
        """
        Position.__init__(self)
        # self.idx      = -1
        self.size_b   = -1
        self.momsn    = -1
        self.mtmsn    = -1
        self.pid      = -1
        
class Surface:
    """CTD samples taken at the surface (the listing's SFC/sfc lines)."""
    
    def __init__(self, num: int = 0):
        """Initialize an empty time series.

        Attributes:
            dt_ref: reference time (DateTime); the SFC line time while parsing, the earliest
                sample time after PostProcess().
            n: number of samples.
            dt_ref_delta_sec: sample times in seconds after dt_ref.
            dt: sample times as DateTime objects; removed by PostProcess().
            dt_epoch: sample times in seconds since epoch (filled by PostProcess()).
            dt_str: sample times as ISO 8601 strings (filled by PostProcess()).
            pres: pressure in dBar.
            temp: temperature in degC; NaN if not valid.
            saln: salinity in PSU; NaN if not valid.
            pres_correction: surface pressure offset correction in dBar (the PRC line's
                presOffset); NaN if the listing has no PRC line.
            pres_correction_valid: whether the correction is valid (the PRC line's valid flag).
            pres_correction_num_samples: number of surface samples the correction comes from.
            pres_correction_num_dives: number of dives the correction comes from.
        """
        self.dt_ref           = DateTime()
        self.n                = num
        self.dt_ref_delta_sec = []
        self.dt               = []
        self.dt_epoch         = []
        self.dt_str           = []
        self.pres             = []
        self.temp             = []
        self.saln             = []
        self.pres_correction             = np.nan
        self.pres_correction_valid       = False
        self.pres_correction_num_samples = -1
        self.pres_correction_num_dives   = -1

    def PostProcess(self):
        """Rebase the sample times on the earliest sample and fill the time arrays for pickling.

        dt_ref becomes the earliest sample time, dt_ref_delta_sec is recomputed from it, and
        dt_epoch and dt_str are filled; the dt attribute is deleted. With no samples, dt_ref is
        unset and dt deleted.
        """
        
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
    """Surface wave spectra measured by the float's accelerometer, and the wave statistics.

    The listing's WVS line gives the sample rate and FFT length; each wvs line gives one
    frequency bin of the acceleration auto-spectra Szz (vertical), Syy and Sxx (horizontal),
    the horizontal co-spectrum Cxy and the quadrature spectra Qzx and Qzy, in g^2/Hz.
    PostProcess() turns them into displacement spectra (m^2/Hz), directional coefficients and
    bulk statistics. Attributes ending in _g are acceleration spectra (g^2/Hz), the raw_ ones
    as received.
    """

    def __init__(self, fsam: float = -1, slen: int = -1):
        """Initialize slen//2 frequency bins with NaN spectra and statistics.

        Args:
            fsam: accelerometer sample rate, in Hz.
            slen: FFT length (samples per spectral segment).

        Attributes:
            raw_freq, freq: bin frequencies k * fsam / slen for k = 0 .. slen//2 - 1, in Hz. Bin 0 is the
                DC bin of the sensor FFT.
            raw_Szz_g .. raw_Qzy_g: the spectra as received (set by PostProcess()).
            Szz_g .. Qzy_g: acceleration spectra, filled from the wvs lines.
            Szz .. Qzy: displacement spectra in m^2/Hz (computed by PostProcess()).
            a0, a1, a2, b1, b2: Longuet-Higgins directional Fourier coefficients.
            r1, r2: directional spreading parameters, in [0, 1].
            theta1: mean wave direction per frequency, in degrees clockwise from north (see
                direction_ref), toward which the waves propagate, [0, 360).
            theta2: principal wave direction per frequency, in degrees clockwise from north (see
                direction_ref), toward which the waves propagate, [0, 360); the end of the
                principal axis nearest theta1.
            wavenumber: deep-water wavenumber per frequency, in rad/m.
            moment: spectral moments m0, m1, m2 of Szz.
            swh: significant wave height, in m.
            mwd: mean wave direction, in degrees clockwise from north, toward which the waves
                propagate, [0, 360).
            declination: magnetic declination applied to the directions, in degrees (east
                positive), or NaN if none was applied.
            direction_ref: 'true' if the directions are referenced to true north (the declination
                was applied), else 'magnetic'.
            period_peak, period_avg, period_zero: peak, mean and zero-crossing periods, in s.
        """
        self.slen = slen
        flen = slen//2
        
        self.fsam = fsam
        
        
       
        # self.freq           = np.empty(flen)
        # self.freq[:]        = np.nan
        
        
        # raw spectra
        
        self.raw_freq           = np.array([ x*self.fsam/self.slen for x in range(0, flen) ])
        
        self.raw_Szz_g          = np.empty(flen, dtype=np.double)
        self.raw_Szz_g[:]       = np.nan
        self.raw_Syy_g          = np.empty(flen, dtype=np.double)
        self.raw_Syy_g[:]       = np.nan
        self.raw_Sxx_g          = np.empty(flen, dtype=np.double)
        self.raw_Sxx_g[:]       = np.nan
        self.raw_Cxy_g          = np.empty(flen, dtype=np.double)
        self.raw_Cxy_g[:]       = np.nan
        self.raw_Qzx_g          = np.empty(flen, dtype=np.double)
        self.raw_Qzx_g[:]       = np.nan
        self.raw_Qzy_g          = np.empty(flen, dtype=np.double)
        self.raw_Qzy_g[:]       = np.nan
        
        # processes spectra
        
        self.freq           = np.array([ x*self.fsam/self.slen for x in range(0, flen) ])
        
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
        self.declination    = np.nan            # Magnetic declination applied to directions
        self.direction_ref  = 'magnetic'        # Reference of the directions
        
        self.period_peak    = np.nan            # Dominant period         
        self.period_avg     = np.nan            # Average period
        self.period_zero    = np.nan            # Zero-crossing
        
    def PostProcess(self, freq_min: float = 0.0625, interpolate = True, rolling_average: int = 3,
                    lat: float = np.nan, lon: float = np.nan, dt_epoch: float = None):
        """Compute displacement spectra, directional coefficients and wave statistics.

        Does nothing if slen < 1. Otherwise:

        1. Keep the received acceleration spectra in the raw_ attributes.
        2. If interpolate, interpolate the acceleration spectra linearly onto 0.030 to 1.250 Hz
           in 0.005 Hz steps (this becomes freq); values outside the received band are 0.
        3. For the vertical spectrum only, zero values below 0.0005 g^2/Hz and multiply by
           the low-frequency response correction, a cosine taper with coefficients f0, w:
           0 below f0, (1 - cos(pi (f - f0) / w)) / 2 from f0 to f0 + w, and 1 above.
        4. Convert to displacement spectra in m^2/Hz by multiplying by g^2 / (2 pi f)^4 and
           compute the deep-water wavenumber (2 pi f)^2 / g in rad/m.
        5. Smooth the displacement spectra with a rolling_average-point RollingAverage.
        6. Reject the spectra (set all displacement spectra to NaN) if any acceleration
           auto-spectrum exceeds 0.09 g^2/Hz at or below 0.05 Hz (low-frequency noise) or
           0.3 g^2/Hz anywhere, or any displacement auto-spectrum at or above freq_min exceeds
           300 m^2/Hz or the vertical one is NaN.
        7. Compute the moments m0, m1, m2 of Szz (summed up to the first NaN bin) and the peak
           bin, and the directional coefficients: a0 = Szz / pi,
           a1, b1 = Qzx, Qzy / sqrt(Szz (Sxx + Syy)), a2 = (Sxx - Syy) / (Sxx + Syy),
           b2 = 2 Cxy / (Sxx + Syy), r1, r2 their magnitudes, theta1 = atan2(a1, b1) and
           theta2 = atan2(b2, -a2) / 2, taken at the end of that axis (theta2 or theta2 + 180)
           nearest theta1 (NaN angles become 0). Directions are clockwise from magnetic north and
           give the direction the waves propagate toward (not from).
        8. If m0 > 0: swh = 4 sqrt(m0) (m), period_peak = 1 / f_peak, period_avg = m0 / m1 and
           period_zero = sqrt(m0 / m2) (s), and mwd is the mean of the theta1 directions
           weighted by sqrt(Szz) over freq_min to 0.6 Hz (bins without a direction, where r1 is
           NaN, get no weight). If the magnetic declination at
           lat, lon and dt_epoch is available (IGRF, ppigrf), it is added to the directions,
           which are then referenced to true north (direction_ref 'true'); otherwise they stay
           referenced to magnetic north. Angles are converted to whole degrees in [0, 360). If m0 is not positive, the spectra, coefficients, moments and
           statistics are set to NaN and the angles to 0.
        9. Round everything and convert the arrays to lists for pickling.

        Args:
            freq_min: lowest frequency, in Hz, used for the 300 m^2/Hz check and for mwd.
            interpolate: if True, interpolate the spectra onto the fixed frequency grid.
            rolling_average: rolling average window, in bins; 1 or less for none.
            lat, lon, dt_epoch: position (decimal degrees) and time (seconds since epoch) of the
                measurement, for the magnetic declination.

        Raises:
            Exception: 'Failed to post-process wave data.' if any step fails.
        """
    
        g = 9.80665 # m s^-2
        pi = math.pi
        
        # range checking
        
        zero_below_freq_min = False
        spectra_max   = 300 # m^2/Hz
        spectra_max_g = 0.3 # g^2/Hz
        
        # interpolation frequencies
        
        F_INTER_MIN  = 0.030 # Hz
        F_INTER_STEP = 0.005 # Hz
        F_INTER_MAX  = 1.250 # Hz
        
        # Low-frequency response correction (WIMS user manual rev. 3.15), a cosine taper
        # [f0, w] in Hz: fitted to the Hurricane Erin (NDBC 41047, A-DWSD) and Hurricane Lowell
        # (A-DWSD) colocations.
        ccoef = [0.03189, 0.07842]
    
        # spectra peak
        peak_zz  = 0
        peak_idx = -1
        
        try: 
        
            if self.slen < 1:
                return
            
            # copy raw spectra
                
            self.raw_Szz_g          = self.Szz_g 
            self.raw_Syy_g          = self.Syy_g 
            self.raw_Sxx_g          = self.Sxx_g 
            self.raw_Cxy_g          = self.Cxy_g 
            self.raw_Qzx_g          = self.Qzx_g  
            self.raw_Qzy_g          = self.Qzy_g
                
            # interpolate acceleration spectra    
            
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
            
                # new_freq = np.array([ x*self.fsam/self.slen/2 for x in range(1, self.slen + 1)])
                new_freq = np.arange(
                    F_INTER_MIN, 
                    F_INTER_MAX + F_INTER_STEP, 
                    F_INTER_STEP)
                    
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
            
            Szz_g = self.Szz_g 
            
            # Zero acceleration spectra below minimum sensitivity

            Szz_g = np.where(Szz_g < 0.0005, 0, self.Szz_g)                
            
            # apply the low-frequency response correction (cosine taper)

            if ccoef[1] > 0:

                f0, w = ccoef[0], ccoef[1]

                taper = 0.5 * (1 - np.cos(math.pi * np.clip((self.freq - f0) / w, 0, 1)))

                Szz_g = Szz_g * taper
                
            # convert acceleration spectra to displacement spectra

            factor = g**2 / ((2*pi*self.freq)**4)

            self.Szz = factor * Szz_g
            self.Sxx = factor * self.Sxx_g 
            self.Syy = factor * self.Syy_g 
            self.Cxy = factor * self.Cxy_g 
            self.Qzx = factor * self.Qzx_g 
            self.Qzy = factor * self.Qzy_g 
            
            # estimate wavenumber
            
            self.wavenumber = (2 * math.pi * self.freq) ** 2 / g
            
            # zero spectra below cutoff
            
            if zero_below_freq_min:
                for idx in range(0, len(self.freq)):
                    if self.freq[idx] < freq_min:
                        self.Sxx[idx] = 0
                        self.Syy[idx] = 0
                        self.Szz[idx] = 0
                        self.Cxy[idx] = 0
                        self.Qzy[idx] = 0
                        self.Qzx[idx] = 0
                    else: 
                        break;
                    
            # apply rolling average to displacemnt spectra
            
            if rolling_average > 1:
            
                self.Szz = RollingAverage(self.Szz, rolling_average)
                self.Syy = RollingAverage(self.Syy, rolling_average)
                self.Sxx = RollingAverage(self.Sxx, rolling_average)
                self.Cxy = RollingAverage(self.Cxy, rolling_average)
                self.Qzx = RollingAverage(self.Qzx, rolling_average)
                self.Qzy = RollingAverage(self.Qzy, rolling_average)
                    
            # Remove spectra with vert LF accel noise
            
            spectra_max_lf_g = 0.09
            spectra_lf_cutoff_f = 0.05
            for idx in range(0, len(self.freq)):
                if self.freq[idx] > spectra_lf_cutoff_f:
                    break    
                if self.Szz_g[idx] > spectra_max_lf_g or self.Sxx_g[idx] > spectra_max_lf_g or self.Syy_g[idx] > spectra_max_lf_g:
                # if self.Szz_g[idx] > 0.1:
                    self.Szz[:] = np.nan
                    self.Syy[:] = np.nan
                    self.Sxx[:] = np.nan
                    self.Cxy[:] = np.nan
                    self.Qzx[:] = np.nan
                    self.Qzy[:] = np.nan
                    break
                
            # 
                
                
            # Remove spectra with peaks above maximum
            
            if spectra_max_g > 0:
                for idx in range(0, len(self.freq)):
                    if self.Szz_g[idx] > spectra_max_g or self.Sxx_g[idx] > spectra_max_g or self.Syy_g[idx] > spectra_max_g:
                    # if self.Szz_g[idx] > spectra_max_g:
                        self.Szz[:] = np.nan
                        self.Syy[:] = np.nan
                        self.Sxx[:] = np.nan
                        self.Cxy[:] = np.nan
                        self.Qzx[:] = np.nan
                        self.Qzy[:] = np.nan
                        break
            
            if spectra_max > 0:
                for idx in range(0, len(self.freq)):
                    if self.freq[idx] < freq_min:
                        continue
                    if self.Szz[idx] > spectra_max or self.Sxx[idx] > spectra_max or self.Syy[idx] > spectra_max or np.isnan(self.Szz[idx]):
                        self.Szz[:] = np.nan
                        self.Syy[:] = np.nan
                        self.Sxx[:] = np.nan
                        self.Cxy[:] = np.nan
                        self.Qzx[:] = np.nan
                        self.Qzy[:] = np.nan
                        break
        
            
        
            # Calculate derived fields
        
            self.moment[:]  = 0          
            
            for idx in range(0, len(self.freq)):
                
                # if self.freq[idx] < freq_min:   # exclude frequencies below cutoff
                #     continue
                    
                if np.isnan(self.Szz[idx]):     # check if spectra is bad
                    break;
                    
                # Calculate moments    
                    
                fstep = self.freq[1] - self.freq[0]
                for i in range(0,3):
                    self.moment[i] = self.moment[i] + (self.freq[idx]**i * self.Szz[idx] * fstep)
                    
            for idx in range(0, len(self.freq)):
                    
                # Caclulate peak
                
                # if self.freq[idx] < freq_min: # exclude frequencies below cutoff
                #     continue

                if self.Szz[idx] > peak_zz:
                    peak_zz     = self.Szz[idx]
                    peak_idx    = idx    
            
            # Calculate directional spreading coeffs
            
            # Displacement (heave/north/east) formulation: the horizontal channels are displacements,
            # not surface slopes, so the coefficients are normalized by the spectra rather than by
            # the wavenumber. r1 and r2 are then in [0, 1].

            self.a0 = self.Szz / math.pi

            norm1 = np.sqrt(self.Szz * (self.Sxx + self.Syy))
            norm1 = np.where(norm1 > 0, norm1, np.nan)
            norm2 = self.Sxx + self.Syy
            norm2 = np.where(norm2 > 0, norm2, np.nan)

            self.a1 = self.Qzx / norm1
            self.b1 = self.Qzy / norm1
            self.a2 = ( self.Sxx - self.Syy ) / norm2
            self.b2 = ( 2 * self.Cxy ) / norm2

            self.r1 = np.sqrt((self.a1 ** 2) + (self.b1 ** 2))
            self.r2 = np.sqrt((self.a2 ** 2) + (self.b2 ** 2))
            
            # Directions are clockwise from magnetic north (x = east, y = north) and give the
            # direction the waves propagate toward. a2 and b2 only give the principal axis, unique
            # modulo 180, so theta2 is taken at the end of the axis nearest theta1.

            self.theta1 = np.arctan2(self.a1, self.b1)
            self.theta1 = np.where(np.isnan(self.theta1), 0, self.theta1)
            self.theta2 = 0.5 * np.arctan2(self.b2, -self.a2)
            self.theta2 = np.where(np.isnan(self.theta2), 0, self.theta2)
            self.theta2 = np.where(np.cos(self.theta2 - self.theta1) < 0, self.theta2 + math.pi, self.theta2)
            
            # Check that we have good spectra
            
            if ~np.isnan(self.moment[0]) and (self.moment[0] > 0.):
            
                self.swh    = 4 * math.sqrt(self.moment[0]) # significant wave height
               
                self.period_peak = 1 / self.freq[peak_idx]
                self.period_avg  = self.moment[0] / self.moment[1]
                self.period_zero = math.sqrt(self.moment[0] / self.moment[2])
                
                # Compute mean wave direction weighted average:
                
                sum_x = 0
                sum_y = 0

                for idx in range(0, len(self.freq)):
                
                    if self.freq[idx] < freq_min: # exclude frequencies below cutoff
                        continue
                        
                    if self.freq[idx] > 0.6: # exclude high freq
                        break
                        
                    
                    # Bins without a direction (theta1 NaN, set to 0 above) get no weight.
                    weight = 0 if np.isnan(self.r1[idx]) else math.sqrt(self.Szz[idx])
                    
                    sum_x = sum_x + (math.cos(self.theta1[idx]) * weight)
                    sum_y = sum_y + (math.sin(self.theta1[idx]) * weight)
                
                self.mwd = math.atan2(sum_y, sum_x)
                  
                # Convert angles to degrees
                
                self.mwd            = self.mwd * 180 / math.pi
                self.theta1         = self.theta1 * 180 / math.pi
                self.theta2         = self.theta2 * 180 / math.pi

                # Rotate from magnetic to true north (bins without a direction stay 0)

                self.declination = MagneticDeclination(lat, lon, dt_epoch)
                if not np.isnan(self.declination):
                    self.mwd            = self.mwd + self.declination
                    self.theta1         = np.where(np.isnan(self.r1), self.theta1, self.theta1 + self.declination)
                    self.theta2         = np.where(np.isnan(self.r2), self.theta2, self.theta2 + self.declination)
                    self.direction_ref  = 'true'
                
                # self.theta1         = np.round(self.theta1/15, 0)
                # self.theta1         = self.theta1 * 15
                
                self.mwd            = np.round(self.mwd, 0)
                self.theta1         = np.round(self.theta1, 0)
                self.theta2         = np.round(self.theta2, 0)
                
                self.mwd            = self.mwd % 360
                self.theta1         = np.mod(self.theta1, 360)
                self.theta2         = np.mod(self.theta2, 360)
                  
            else:
            
                # self.freq[:]        = np.nan
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
                
                self.wavenumber[:]  = np.nan
                
                self.r1[:]          = np.nan
                self.r2[:]          = np.nan
                self.theta1[:]      = 0
                self.theta2[:]      = 0
                self.mwd            = 0
            
                self.moment[:]      = np.nan
                
                self.swh            = np.nan            # Significant wave height
                
                self.period_peak    = np.nan            # Dominant period         
                self.period_avg     = np.nan            # Average period
                self.period_zero    = np.nan            # Zero-crossing
                
            # Convert arrays to lists
        
            self.raw_freq       = np.round(self.raw_freq, 3).tolist()
            self.raw_Szz_g      = np.round(self.raw_Szz_g, 4).tolist()
            self.raw_Syy_g      = np.round(self.raw_Syy_g, 4).tolist()
            self.raw_Sxx_g      = np.round(self.raw_Sxx_g, 4).tolist()
            self.raw_Cxy_g      = np.round(self.raw_Cxy_g, 4).tolist()
            self.raw_Qzx_g      = np.round(self.raw_Qzx_g, 4).tolist()
            self.raw_Qzy_g      = np.round(self.raw_Qzy_g, 4).tolist()
            self.freq           = np.round(self.freq, 3).tolist()
            self.Szz_g          = np.round(self.Szz_g, 4).tolist()
            self.Syy_g          = np.round(self.Syy_g, 4).tolist()
            self.Sxx_g          = np.round(self.Sxx_g, 4).tolist()
            self.Cxy_g          = np.round(self.Cxy_g, 4).tolist()
            self.Qzx_g          = np.round(self.Qzx_g, 4).tolist()
            self.Qzy_g          = np.round(self.Qzy_g, 4).tolist()
            self.Szz            = np.round(self.Szz, 3).tolist()
            self.Syy            = np.round(self.Syy, 3).tolist()
            self.Sxx            = np.round(self.Sxx, 3).tolist()
            self.Cxy            = np.round(self.Cxy, 3).tolist()
            self.Qzx            = np.round(self.Qzx, 3).tolist()
            self.Qzy            = np.round(self.Qzy, 3).tolist()
            
            self.wavenumber     = np.round(self.wavenumber, 5).tolist()
            
            self.a0             = np.round(self.a0, 4).tolist()
            self.a1             = np.round(self.a1, 4).tolist()
            self.a2             = np.round(self.a2, 4).tolist()
            self.b1             = np.round(self.b1, 4).tolist()
            self.b2             = np.round(self.b2, 4).tolist()
            
            self.r1             = np.round(self.r1, 3).tolist()
            self.r2             = np.round(self.r2, 3).tolist()
            self.theta1         = np.round(self.theta1, 0).astype(int).tolist()
            self.theta2         = np.round(self.theta2, 0).astype(int).tolist()
            self.mwd            = np.round(self.mwd, 0).astype(int).tolist()
        
            self.moment         = np.round(self.moment, 5).tolist()
            
            self.swh            = np.round(self.swh, 2).tolist()
            
            self.period_peak    = float(np.round(self.period_peak, 1)) # Dominant period         
            self.period_avg     = float(np.round(self.period_avg, 1))  # Average period
            self.period_zero    = float(np.round(self.period_zero, 1)) # Zero-crossing
            
            
        except: 
        
            raise Exception('Failed to post-process wave data.')

"""
Composite classes
"""        

class CompositePosition(Position):
    """A position combined from several positions close in time (e.g. Iridium SBD positions).

    xmtps-aggregate-json.py groups a float's SBD positions that fall within a time window, then
    uses the composite to fill in profile positions for dives without a GPS fix. The inherited
    time, lat, lon and cep_km hold the combined result (see compute()).
    """

    # GPS-calibrated error term (km) added to the composite CEP. The SBD positions of a
    # transmission session are not independent (they often repeat, and each float has an offset
    # that changes from session to session), so sqrt(1 / sum(CEP^-2)) alone was overconfident:
    # against interpolated GPS fixes (40,518 composites from 234 floats, 2026-10-27) the error was
    # <= it in only 25% of composites, and about 2 km whatever the number of positions. With
    # this term the error is <= cep_km in 50% of composites, as a CEP should be.
    BIAS_CEP_KM = 1.55

    def __init__(self, pos: Position = None, compute = True):
        """Initialize, adding pos (and computing the composite if compute) if given.

        Attributes:
            dive: dive number the composite is assigned to; NaN until set by the caller.
            dt_epoch_min, dt_epoch_max: earliest and latest time of the added positions, in
                seconds since epoch (9999999999 and -1 while empty).
            num_pos: number of valid positions used by the last compute().
            positions: the added Position objects.
        """
        Position.__init__(self)
        self.dive = np.nan
        self.dt_epoch_min = 9999999999
        self.dt_epoch_max = -1
        self.num_pos = 0
        self.positions = []
        
        if pos is not None:
            self.add_position(pos = pos, compute = compute)
        
    def add_position(self, pos: Position, compute = True):
        """Add a position, update dt_epoch_min/max, recompute if compute, and return self.

        Invalid positions are added too (and count toward dt_epoch_min/max) but compute()
        ignores them. Raises Exception('Failed to append position.') on any error, including a
        failed compute().
        """
        
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
        """Combine the valid positions into one, weighting each by 1 / cep_km^2, and return self.

        Each valid position (see Position.is_valid) is converted to a unit vector on the
        sphere; the weighted mean vector is converted back to lat/lon (degrees, rounded to 4
        decimals), so the average is correct across the dateline. The time is the weighted mean
        time (whole seconds since epoch) and cep_km = sqrt(1 / sum(1 / cep_km^2) + BIAS_CEP_KM^2),
        rounded to 1 m: the CEP of independent estimates plus the GPS-calibrated error term.
        num_pos is set to the number of positions used.

        Raises:
            Exception: 'Failed to compute composite position.' if there are no valid positions
                or the computation fails.
        """
        
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
                        
            self.cep_km = round(math.sqrt(1/cep_sum + self.BIAS_CEP_KM**2), 3)
            
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
        
        