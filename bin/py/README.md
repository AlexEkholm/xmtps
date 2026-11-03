# `bin/py/` — Python tools

These tools turn the decoder's listing files into JSON, plots and the web pages published at
[argo.whoi.edu/alamo](https://argo.whoi.edu/alamo/). They run on Python 3.12 or later; production
uses Python 3.14 in the virtual environment `/data1/alamo/bin/py/.venv`. The package list and
versions are in [Python tools](../../README.md#python-tools-binpy) and
[Versions](../../README.md#versions) in the main README, which also documents every command-line
option. This page describes what each file does and how they fit together.

| File | Kind | Purpose |
|---|---|---|
| [`update-plots-service.sh`](#update-plots-servicesh) | shell | Plot service loop: runs the converters for floats with recent listings, then rebuilds the pages |
| [`xmtps-list2json.py`](#xmtps-list2jsonpy) | tool | Listing files → per-dive JSON files |
| [`xmtps-aggregate-json.py`](#xmtps-aggregate-jsonpy) | tool | Per-dive JSON → per-float aggregate JSON, plots and float page |
| [`xmtps-generate-summary-page.py`](#xmtps-generate-summary-pagepy) | tool | Fleet summary page, `index.html` |
| [`xmtps-generate-mission-page.py`](#xmtps-generate-mission-pagepy) | tool | Mission parameters page, `engineering/params.html` |
| [`xmtps-generate-startup-page.py`](#xmtps-generate-startup-pagepy) | tool | Float startup (BIT) page, `engineering/startup.html` |
| [`FloatClasses.py`](#floatclassespy) | module | Data classes for one dive, shared by the converters; wave spectra post-processing |
| [`RhoLUT.py`](#rholutpy) | module | Seawater density and σ₀ lookup table |
| [`PageStatus.py`](#pagestatuspy) | module | Page footer: update time, run time, decoder status and version |
| [`cp-files.sh`](#cp-filessh) | shell | Old helper that copied edited scripts from the home directory |

## Data flow

```
data/list/<sn>/lis<sn>-<dive>.txt          listing files, written by the decoder
        │  xmtps-list2json.py   (FloatClasses.Dive, RhoLUT)
        ▼
data/json/<sn>/dives/<sn>_<dive>.json      one JSON file per dive
        │  xmtps-aggregate-json.py   (FloatClasses)
        ▼
data/json/<sn>/<sn>_<name>.json            per-float aggregate JSON (summary, profile, gps, ...)
data/plots2/<sn>/                          per-float plots and index.html  (with -p)
        │  xmtps-generate-summary-page.py
        │  xmtps-generate-mission-page.py
        │  xmtps-generate-startup-page.py   (PageStatus)
        ▼
data/plots2/index.html                     fleet summary page
data/plots2/engineering/params.html        mission parameters page
data/plots2/engineering/startup.html       startup page
```

Paths are under `/data1/alamo/` in production. All tools read and write JSON version `0.11` and
ignore JSON files with any other version. The file formats are specified in
[spec/json-file-format.md](../../spec/json-file-format.md) and
[spec/plot-and-page-files.md](../../spec/plot-and-page-files.md).

## `update-plots-service.sh`

The loop run by the `xmtps-alamo-plot` systemd service (see [bin/README.md](../README.md)). It
activates the virtual environment and then, forever:

1. **Finds the floats to update:** every float with a listing file changed in the last 10 days
   (`find /data1/alamo/data/list/ -cmin -14400`).
2. **Converts each float**, in the background: `xmtps-list2json.py` on its listing directory, then
   `xmtps-aggregate-json.py` on its `dives` directory with plots (`-p data/plots2`). At most 8
   floats run at a time (`MAX_PROC`).
3. **Waits for the conversions to finish.** If they take longer than an hour (`MAX_RUN_T`, 3600 s)
   it kills the ones still running.
4. **Rebuilds the three pages:** the summary page in `data/plots2/`, then the mission parameters
   and startup pages in `data/plots2/engineering/` (created if missing), passing the run time of
   the pass for their footers.
5. **Sleeps** 5 minutes (`SLEEP_SEC`, 300 s) and starts again.

Each pass logs a `--- start ---` and a `--- done, run time = N ---` line with UTC times.

Things to know:

- **Only changed floats are converted.** `xmtps-list2json.py` skips listings whose JSON is newer
  and `xmtps-aggregate-json.py` skips directories with nothing new since `.lastprocessed`. After a
  change to `FloatClasses.py` or `RhoLUT.py`, existing data is updated only by running both tools
  by hand with `-f`.
- **The include pattern selects the floats:** listing paths must match
  `(908|909|91|92|93|94|10|11)[0-9]+`, so floats whose serial numbers start otherwise (for example
  9046) are never converted.
- **Restart after editing:** the script runs in a loop, so a change to it takes effect only after
  `sudo systemctl restart xmtps-alamo-plot`. Changes to the Python tools take effect on the next
  pass.

## `xmtps-list2json.py`

Converts listing files into per-dive JSON files.

- **Input:** listing files `lis<sn>-<dive>.txt` and/or directories of them (subdirectories only with
  `-r`), filtered by the `-i` include and `-x` exclude regular expressions.
- **Parsing:** reads each listing line by line (the line types are described in
  [Listing files](../../README.md#listing-files)) into a `FloatClasses.Dive`: float IDs, GPS fixes,
  SBD messages, mission configuration, engineering data (BIT, descent, drift, ascent, surface,
  ice), drift and profile CTD samples, descent, ascent and surface time series, pump records and
  wave spectra.
- **Derived values:** density and σ₀ for the drift and profile samples (`RhoLUT`), and, through
  `Dive.PostProcess()`, the wave statistics and corrected spectra (`FloatClasses.Wave`). For a
  dive without a valid GPS fix, the magnetic declination of its wave spectra uses the latest
  valid fix from an earlier dive's listing.
- **Output:** `<output>/<sn>/dives/<sn>_<dive>.json`, written with jsonpickle. The `dives`
  directory is created if needed; the output directory must exist.
- **Skipping:** a listing is skipped when its JSON file is newer, unless `-f` is given; with `-s`
  it is skipped whenever its JSON file exists.

## `xmtps-aggregate-json.py`

Combines a float's dive JSON files into per-float JSON files and, with `-p`, plots and a float page.
It is the largest of the tools.

- **Input:** usually a float's `dives` directory, or single dive JSON files. A directory is skipped
  unless one of its files is newer than the time in its `.lastprocessed` file; `-f` processes it
  anyway.
- **Aggregation:** groups the dives by float and builds pandas DataFrames for the ascent, descent,
  drift, profile, pump, surface, GPS, SBD, wave, engineering and mission data. From these it
  derives profile positions, composite Iridium positions, submerged and surface displacements,
  cycle timings and a float summary. Listing fill values (`-9.999`, `65535`, ...; see
  `FILL_VALUES`) become NaN.
- **Aggregate JSON:** writes `<output>/<sn>/<sn>_<name>.json`, one file per DataFrame (`ascent`,
  `descent`, `drift`, `profile`, `profile_position`, `pump`, `surface`, `gps`, `sbd`,
  `iridium_position`, `displacement_submerged`, `displacement_surface`, `cycle_timing`, `wave`,
  `wave_spectra`, `engineering`, `engineering_data`, `mission`, `data`), plus the jsonpickle-encoded
  `<sn>_summary.json` that the page generators read.
- **Plots (`-p DIR`):** runs the plot groups in `PLOT_WRITERS`: geographic positions, submerged and
  surface displacement, trajectory (with ETOPO 2022 bathymetry), cycle timing, profiles, contours,
  ascent, descent, drift, hydraulic and air pumps, engineering, surface, satellite communication
  and waves. Each plot is written to `DIR/<sn>/` as `<sn>_<plot>.html` (interactive Plotly) and as
  PNG images in three sizes (`.png`, `_small.png`, `_thumb.png`, through kaleido and one shared
  Chromium). `DIR/<sn>/index.html` links them all.
- **Completion:** `.lastprocessed` is written only after all output, so an interrupted or failed
  run is retried on the next pass. Errors are logged and processing continues unless `-H` is given.

## `xmtps-generate-summary-page.py`

Builds the fleet summary page, `<output>/index.html`, from every `<sn>_summary.json` found under the
input paths (searched recursively, filtered by `-i`). The page has an orthographic map of the
latest float positions, colored by status, and a sortable, searchable table with one row per
float: ID, WMO number, type and CTD, firmware, deployment date, age, distance profiled, status
(active, overdue or archived) and the latest cycle. The page is rebuilt from scratch on every run.
Its footer (`PageStatus`) shows when it was updated, the pass's run time, and the decoder's
status, uptime and version.

## `xmtps-generate-mission-page.py`

Builds the mission parameters page, `<output>/params.html`, from each float's `<sn>_summary.json`
and `<sn>_mission.json`.

- **Rows:** one per deployed float that has sent mission parameters (with `--days`, only floats
  that reported within that many days), showing the parameters from its latest dive that has any.
- **Tables:** one per float type, ALAMO (including WIMS and MM2) and ALTO.
- **Phases:** phase-dependent parameters are shown for each mission phase, with the current phase
  marked and unused phases grayed out.
- **Colors:** each value is colored by how unusual it is among the floats in its table. Impossible
  or inconsistent values are marked as errors, and unset values are shown as unknown.

The columns and coloring rules are listed in [Python tools](../../README.md#command-line-options).
`--plots-url` sets where the float pages are linked (production: `/alamo`).

## `xmtps-generate-startup-page.py`

Builds the startup page, `<output>/startup.html`, from each float's `<sn>_summary.json` and
`<sn>_engineering_data.json`. It lists the floats that started up in the last `--days` days
(default 180), latest first.

- **Startup number:** a float sends each startup's messages with the next negative dive number
  (-1, -2, ...), so the dive of the latest built-in test (BIT) gives the startup number, and that
  BIT's reported time the startup time.
- **Columns:** status (started or deployed), startup, deployment and last-report times, BITs sent
  and failed, and the latest BIT's results: duration, failed tests, subsystem error flags,
  firmware, CTD pressure after calibration, vacuum, battery voltage and the air and oil pump
  measurements.
- **Colors:** passed BITs green, failed BITs and set error flags dark red, floats not yet deployed
  yellow.

## `FloatClasses.py`

The data classes shared by the two converters. `xmtps-list2json.py` builds one `Dive` per listing
and pickles it; `xmtps-aggregate-json.py` unpickles the dives and groups them into `Float`
objects. Because the objects are pickled with jsonpickle, their attribute names and types are
part of the JSON format.

- **Times and positions:** `DateTime` (Unix seconds and ISO 8601 string), `Position`, `Gps`,
  `Sbd`, `CompositePosition` (combines Iridium positions) and `PositionVector`.
- **Dive data:** `Dive`, `Profile`, `Drift`, `Ascent` and `Descent` (both `PresTimeSeries`),
  `Surface`, `Pump`, `MissionParameters`, `FloatId`, `File` and `Float`.
- **Engineering:** `Engineering`, `EngineeringDescent`, `EngineeringDrift`, `EngineeringAscent`,
  `EngineeringSurface`, `EngineeringIce` and `EngineeringBit`.
- **Waves:** `Wave` holds a WIMS wave spectrum. Its `PostProcess()` converts the acceleration
  spectra to displacement, applies the noise-floor mask and the low-frequency response correction
  (a cosine taper, `ccoef`), and computes the directional moments, wave height, periods and
  directions. Directions are referenced to true north using `MagneticDeclination()`, which uses
  the optional `ppigrf` package; without it they stay magnetic.
- **Helpers:** `RollingAverage()`.

A change here alters the JSON only for files written afterwards; rebuild existing data with `-f`.

## `RhoLUT.py`

Lookup tables of in-situ density (`rho_lut`) and potential density referenced to the surface
(`sigma0_lut`), both as density minus 1000 kg/m³. They are tabulated on a grid of salinity
(0–39 PSU), temperature (−5–39 °C) and pressure (0–2900 dBar), in steps of 1 PSU, 1 °C and 100 dBar,
and interpolated trilinearly by `GetRhoFromLut()` and `GetSigma0FromLut()`. The grid limits are
`MIN_`/`MAX_SALN`, `_TEMP` and `_PRES`. The file is large because the tables are written out in
full.

## `PageStatus.py`

Writes the status line at the foot of the summary, mission parameters and startup pages
(`WriteStatusFooter()`), for example:

```
Page last updated 2026-10-26T17:40Z (105 minutes ago, runtime: 3 minutes) - python version 0.11
- decoder status: active (uptime: 3 hours, 4 minutes) - xmtps version 1.0.3318b (built 2026-10-26)
```

The decoder status and uptime come from `systemctl show xmtps-alamo`, which needs no privileges,
and the version from `/data1/alamo/bin/xmtps --version`, which prints it without touching the
running daemon. The "minutes ago" part is filled in by JavaScript when the page is viewed. Anything
that cannot be determined is shown as "unknown".

## `cp-files.sh`

An old helper for copying scripts edited elsewhere into this directory. For `FloatClasses.py`,
`xmtps-aggregate-json.py`, `xmtps-list2json.py` and `xmtps-generate-summary-page.py`, it reads the
copy in the home directory (`~/<name>`), strips carriage returns, prepends `#!/usr/bin/python3`
and writes the result here. It is not used by the service. Running it on a file that already has a
shebang adds a second one, which is why those four files start with two identical `#!` lines.
