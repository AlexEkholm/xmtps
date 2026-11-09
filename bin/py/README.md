# `bin/py/` — Python tools

These tools turn the decoder's listing files into JSON, plots and the web pages published at
[argo.whoi.edu/alamo](https://argo.whoi.edu/alamo/). This page describes every file in the
directory, what each tool reads and writes, and all of their command-line options.

| File | Kind | Purpose |
|---|---|---|
| [`update-plots-service.sh`](#update-plots-servicesh) | shell | Plot service loop: runs the converters for floats with recent listings, then rebuilds the pages |
| [`xmtps-list2json.py`](#xmtps-list2jsonpy) | tool | Listing files → per-dive JSON files |
| [`xmtps-aggregate-json.py`](#xmtps-aggregate-jsonpy) | tool | Per-dive JSON → per-float aggregate JSON, plots and float page |
| [`xmtps-generate-summary-page.py`](#xmtps-generate-summary-pagepy) | tool | Fleet summary page, `index.html` |
| [`xmtps-generate-mission-page.py`](#xmtps-generate-mission-pagepy) | tool | Mission parameters page, `engineering/params.html` |
| [`xmtps-generate-startup-page.py`](#xmtps-generate-startup-pagepy) | tool | Float startup (BIT) page, `engineering/startup.html` |
| `xmtps-generate-energy-page.py` | tool | Battery and energy page, `engineering/energy.html`: each float's pack, battery age and voltages, energy used, per cycle and by part, and the cycles and time left on its mission, from the energy model |
| [`FloatClasses.py`](#floatclassespy) | module | Data classes for one dive, shared by the converters; wave spectra post-processing |
| [`RhoLUT.py`](#rholutpy) | module | Seawater density and σ₀ lookup table |
| [`PageStatus.py`](#pagestatuspy) | module | Page footer: update time, run time, decoder status and version; the links between the table pages and their paging |
| `EnergyModel.py` | module | The energy model's battery outlook: cycles left on the current mission (`RemainingCycles`), a falling oil-pump voltage (`PumpVoltageFallDive`) and their constants, shared by `xmtps-aggregate-json.py` and the energy page |
| `FloatStatus.py` | module | A float's status (PREDEPLOYMENT, ACTIVE, OVERDUE, ARCHIVED) and last known location, shared by the summary and mission parameters pages |
| `SeaNames.py` | module | Names the sea or ocean of a position ("WHOI" near Woods Hole) for the summary page's status column, from `data/marine-areas.json` (Natural Earth marine areas, public domain) |
| `img/` | assets | The status icons `s0.png` to `s3.png`, served from `<plots>/img/` (copy them there on a new install) |

## Contents

- [Requirements](#requirements)
- [Data flow](#data-flow)
- [Conventions shared by the tools](#conventions-shared-by-the-tools)
- [`update-plots-service.sh`](#update-plots-servicesh)
- [`xmtps-list2json.py`](#xmtps-list2jsonpy)
- [`xmtps-aggregate-json.py`](#xmtps-aggregate-jsonpy)
- [`xmtps-generate-summary-page.py`](#xmtps-generate-summary-pagepy)
- [`xmtps-generate-mission-page.py`](#xmtps-generate-mission-pagepy)
- [`xmtps-generate-startup-page.py`](#xmtps-generate-startup-pagepy)
- [`FloatClasses.py`](#floatclassespy), [`RhoLUT.py`](#rholutpy) and [`PageStatus.py`](#pagestatuspy)

## Requirements

Python 3.12 or later (tested with 3.12 and 3.14; pandas 2.3 and 3.x) with `numpy`, `scipy`,
`pandas`, `plotly`, `kaleido` (PNG export), `jsonpickle`, `geographiclib`, `pytz`,
`python-dateutil`, `Pillow` and `yattag`. `ppigrf` is optional; without it, wave directions stay
referenced to magnetic north. Production uses Python 3.14 in the virtual environment
`/data1/alamo/bin/py/.venv`; the versions are listed in [Versions](../../README.md#versions). The
trajectory plots also read the ETOPO 2022 bathymetry grid (see
[`xmtps-aggregate-json.py`](#xmtps-aggregate-jsonpy), `-b`).

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

## Conventions shared by the tools

All five tools are run as `python <tool> [options] PATH ...` and share these rules:

- **Inputs:** one or more `PATH`s, each a file or a directory. A directory's files are read; which
  subdirectories are entered depends on the tool (see each tool).
- **Include patterns (`-i REGEX`):** regular expressions searched anywhere in the full path
  (Python `re.search`, with `/` separators), so a pattern need only match part of the path. `-i`
  is repeatable; a file is read if it matches any pattern. Without `-i`, every file is read. Quote
  patterns on the command line.
- **Logging:** messages go to standard error as `YYYYmmddTHHMMSS LEVEL message`. The level is
  set by the first of `-d`, `-v`, `-q` that is given; with none of them, warnings and errors are
  printed.
- **Errors:** an error is logged and processing continues with the next item, unless `-H` is
  given.
- **Output directory (`-o`):** must exist. The page generators stop with an error if it does not;
  the two converters log an error and then fail to write each file.

Options common to all five tools:

| Option | Default | Description |
|---|---|---|
| `-h`, `--help` | | Print the usage and exit. |
| `-d`, `--debug` | | Print debug, informational, warning and error messages. |
| `-v`, `--verbose` | | Print informational, warning and error messages (if `-d` is not given). |
| `-q`, `--quiet` | | Print only errors (if neither `-d` nor `-v` is given). |
| `-H`, `--halt-on-error` | off | Stop at the first error instead of logging it and continuing. |
| `-i`, `--include REGEX` | all files | Read only files whose path matches `REGEX`. Repeatable. |
| `-o`, `--output DIR` | `.` | Output directory (what is written there depends on the tool). |

## `update-plots-service.sh`

The loop run by the `xmtps-alamo-plot` systemd service (see [bin/README.md](../README.md)). It
takes no arguments; its settings are variables at the top of the script:

| Variable | Value | Meaning |
|---|---|---|
| `DLIST` | `/data1/alamo/data/list` | listing files |
| `DJSON` | `/data1/alamo/data/json` | JSON files |
| `DPLOT` | `/data1/alamo/data/plots2` | plots and pages |
| `DBIN` | `/data1/alamo/bin/py` | the tools |
| `MAX_PROC` | `8` | floats converted at the same time |
| `MAX_RUN_T` | `3600` | seconds after which conversions still running are killed |
| `SLEEP_SEC` | `300` | seconds between passes |

It activates the virtual environment and then, forever:

1. **Finds the floats to update:** every float with a listing file changed in the last 10 days
   (`find /data1/alamo/data/list/ -cmin -14400`).
2. **Converts each float** in the background, at most `MAX_PROC` at a time:

   ```
   xmtps-list2json.py -v -i"$DLIST/(90|91|92|93|94|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" \
       -x"/\..*" -o"$DJSON" "$DLIST/<sn>"
   xmtps-aggregate-json.py -v -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" \
       -o"$DJSON" -p"$DPLOT" "$DJSON/<sn>/dives"
   ```

3. **Waits for the conversions to finish,** killing any still running after `MAX_RUN_T`.
4. **Rebuilds the three pages,** passing the run time of the pass for their footers:

   ```
   xmtps-generate-summary-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT" \
       --runtime <s> $DJSON
   xmtps-generate-mission-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" \
       --plots-url /alamo --runtime <s> $DJSON
   xmtps-generate-startup-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" \
       --plots-url /alamo --runtime <s> $DJSON
   ```

   `$DPLOT/engineering` is created if missing; it is published at
   `http://argus.whoi.edu/etcetc/alamo/engineering/`.
5. **Sleeps** `SLEEP_SEC` and starts again.

Each pass logs a `--- start ---` and a `--- done, run time = N ---` line with UTC times.

Things to know:

- **Only changed floats are converted.** `xmtps-list2json.py` skips listings whose JSON is newer
  and `xmtps-aggregate-json.py` skips directories with nothing new since `.lastprocessed`. After a
  change to `FloatClasses.py` or `RhoLUT.py`, existing data is updated only by running both tools
  by hand with `-f`.
- **The include pattern selects the floats:** listing paths must match
  `(90|91|92|93|94|10|11)[0-9]+` (ALAMO 9000-9499, ALTO 10000-11999), so stray directories with
  other serial numbers (for example 4790 or 9672) are never converted. Before 2026-11-05 the
  pattern began `(908|909|...)`, which left out the older ALAMOs 9003-9079. Of those, the ones
  without usable profile data (`NOPROF` in the script: 9004, 9005, 9011, 9012, 9024, 9025, 9027,
  9037, 9038, 9042, 9047, 9050, 9056, 9059, 9063) are excluded with a second `-x`. The others
  have temperature profiles but, except 9003, no valid salinity.
- **Restart after editing:** the script runs in a loop, so a change to it takes effect only after
  `sudo systemctl restart xmtps-alamo-plot`. Changes to the Python tools take effect on the next
  pass.

## `xmtps-list2json.py`

```
xmtps-list2json.py [-h] [-d] [-q] [-v] [-f] [-H] [-r] [-s] [-i REGEX] [-o DIR] [-S REGEX]
                   [-x REGEX] PATH ...
```

Converts listing files into per-dive JSON files.

- **Input:** listing files `lis<sn>-<dive>.txt` and/or directories of them.
- **Parsing:** reads each listing line by line (the line types are described in
  [Listing files](../../README.md#listing-files)) into a `FloatClasses.Dive`: float IDs, GPS fixes,
  SBD messages, mission configuration, engineering data (BIT, descent, drift, ascent, surface,
  ice), drift and profile CTD samples, descent, ascent and surface time series, pump records and
  wave spectra.
- **Derived values:** density and σ₀ for the drift and profile samples (`RhoLUT`), and, through
  `Dive.PostProcess()`, the wave statistics and corrected spectra (`FloatClasses.Wave`). For a
  dive without a valid GPS fix, the magnetic declination of its wave spectra uses the latest
  valid fix from an earlier dive's listing.
- **Output:** `<output>/<sn>/dives/<sn>_<dive>.json`, written with jsonpickle. The `<sn>/dives`
  directories are created if needed.

Options, besides the [common ones](#conventions-shared-by-the-tools):

| Option | Default | Description |
|---|---|---|
| `PATH ...` | | Listing files and/or directories of listing files. |
| `-o`, `--output DIR` | `.` | Write the dive JSON files to `DIR/<sn>/dives/`. |
| `-r`, `--recursive` | off | Also read the subdirectories of the input directories (otherwise only their files). |
| `-x`, `--exclude REGEX` | none | Skip files, and with `-r` subdirectories, whose path matches `REGEX` (`re.search`). Repeatable. The service uses `-x "/\..*"` to skip hidden files and directories. |
| `-f`, `--force` | off | Convert every listing, even if its JSON file is newer. |
| `-s`, `--skip` | off | Skip every listing whose JSON file already exists, even if the listing is newer. |
| `-S`, `--strip-path REGEX` | `.*/alamo/` | Remove the part of the listing path that matches `REGEX` before storing the path in the JSON (`list_file.path`). |

Without `-f` or `-s`, a listing is converted only when its JSON file is missing or older than the
listing.

Examples:

```
# Convert the listings of float 9308 that are new or changed
xmtps-list2json.py -v -i "/data1/alamo/data/list/[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x "/\..*" \
    -o /data1/alamo/data/json /data1/alamo/data/list/9308

# Rewrite every dive JSON file of all floats, e.g. after changing FloatClasses.py
xmtps-list2json.py -q -f -r -i "/lis[0-9]+-(-?)[0-9]+.txt$" -x "/\..*" \
    -o /data1/alamo/data/json /data1/alamo/data/list
```

## `xmtps-aggregate-json.py`

```
xmtps-aggregate-json.py [-h] [-d] [-q] [-v] [-f] [-H] [-r] [-s] [-b FILE] [-D] [-i REGEX]
                        [-o DIR] [-p DIR] [-S REGEX] [-x REGEX] PATH ...
```

Combines a float's dive JSON files into per-float JSON files and, with `-p`, plots and a float
page. It is the largest of the tools.

- **Input:** usually a float's `dives` directory, or single dive JSON files. A directory is skipped
  unless one of its included files is newer than the time in its `.lastprocessed` file.
- **Aggregation:** groups the dives by float and builds pandas DataFrames for the ascent, descent,
  drift, profile, pump, surface, GPS, SBD, wave, engineering and mission data. From these it
  derives profile positions, composite Iridium positions, submerged and surface displacements,
  cycle timings and a float summary. Listing fill values (`-9.999`, `65535`, ...; see
  `FILL_VALUES`) become NaN before anything is plotted or saved.
- **Aggregate JSON:** writes `<output>/<sn>/<sn>_<name>.json`, one file per DataFrame (`ascent`,
  `descent`, `drift`, `profile`, `profile_position`, `pump`, `surface`, `gps`, `sbd`,
  `iridium_position`, `displacement_submerged`, `displacement_surface`, `cycle_timing`, `wave`,
  `wave_spectra`, `engineering`, `engineering_data`, `mission`, `data`; pandas `split`
  orientation), plus the jsonpickle-encoded `<sn>_summary.json` that the page generators read.
- **Plots (`-p DIR`):** runs the plot groups in `PLOT_WRITERS`: geographic positions, submerged and
  surface displacement, trajectory (with bathymetry), cycle timing, profiles, contours, ascent,
  descent, drift, hydraulic and air pumps, engineering, surface, satellite communication and
  waves. Each plot is written to `DIR/<sn>/` as `<sn>_<plot>.html` (interactive Plotly) and as PNG
  images in three sizes (`.png`, `_small.png`, `_thumb.png`, through kaleido and one shared
  Chromium). `DIR/<sn>/index.html` links them all.
- **Completion:** `.lastprocessed` is written only after all output, so an interrupted or failed
  run is retried on the next pass.

Options, besides the [common ones](#conventions-shared-by-the-tools):

| Option | Default | Description |
|---|---|---|
| `PATH ...` | | Dive JSON files and/or directories of them (usually `<json>/<sn>/dives`). |
| `-o`, `--output DIR` | `.` | Write the aggregate JSON files to `DIR/<sn>/`. |
| `-p`, `--plots DIR` | none | Also write the plots and the float page to `DIR/<sn>/` (created if needed). `DIR` must exist. Without `-p`, no plots are made. |
| `-b`, `--bathymetry`, `--etopo`, `--etopo2 FILE` | `/data1/etopo2022/ETOPO_2022_v1_60s_N90W180_surface_classic.nc` | Classic-format netCDF ETOPO grid for the bathymetry line of the trajectory plots and the bathymetry of the Profile Positions plot. ETOPO 2022 (`lon`/`lat`) and ETOPO2v2c (`x`/`y`) both work. If it cannot be read, the plots are made without bathymetry and a warning is logged. |
| `-r`, `--recursive` | off | Also read the subdirectories of the input directories. |
| `-x`, `--exclude REGEX` | none | With `-r`, skip subdirectories whose path matches `REGEX` (`re.search`). Repeatable. Files are not excluded. |
| `-f`, `--force` | off | Process every directory, ignoring `.lastprocessed`. Needed after a change to `FloatClasses.py` or `RhoLUT.py` or to this script. |
| `-S`, `--strip-path REGEX` | `.*/alamo/` | Remove the part of file paths that matches `REGEX` before storing them in the JSON and plot metadata (dive JSON paths and the figure file paths used by the float page). |
| `--image-tabs N` | `4` | Render the PNG images in `N` browser tabs at once. Each plot group's images are written in one batch, spread over the tabs. On argus, 4 tabs made a float's plots about twice as fast as 1 (9305: 125 s instead of 239 s). Lower it if many floats are updated at once and the CPU is busy. |
| `--image-timeout S` | `120` | Watchdog on the PNG export. A Kaleido call that takes longer than `S` s plus 10 s an image counts as hung. Its Chrome (only this process's) is killed, a new browser is started and the images are retried one at a time. After 3 restarts, the rest of the images fail. `0` turns the watchdog off. |
| `--meta DIR` | `/data1/alamo/meta/dac` | Float metadata files, for the launch times. A startup dive more than a day after the launch time, once the float has sent a dive since the launch, is a reset in the field (a GPS firmware bug made floats restart at sea): it is listed in the summary's `field_resets` and is not the float's startup. The launch time is also the earliest deployment. Without the directory, or for a float without metadata, nothing is a field reset. |
| `-D`, `--display-figures` | off | Also open each figure in a web browser as it is made (interactive use). |
| `-s`, `--skip` | off | Accepted for compatibility with `xmtps-list2json.py`; it has no effect. |

Examples:

```
# Aggregate float 9308 and make its plots (as the service does)
xmtps-aggregate-json.py -v -i "/data1/alamo/data/json/[0-9]+/dives/[0-9]+_-?[0-9]+.json" \
    -o /data1/alamo/data/json -p /data1/alamo/data/plots2 /data1/alamo/data/json/9308/dives

# Rebuild the aggregate JSON only, without plots, into a scratch directory
mkdir -p /tmp/agg
xmtps-aggregate-json.py -q -f -i "_-?[0-9]+\.json$" -o /tmp/agg /data1/alamo/data/json/9308/dives
```

## `xmtps-generate-summary-page.py`

```
xmtps-generate-summary-page.py [-h] [-d] [-q] [-v] [-H] [-i REGEX] [-o DIR]
                               [--runtime SECONDS] PATH ...
```

Builds the fleet summary page, `<output>/index.html`, from every `<sn>_summary.json` found under the
input paths. Directories are always searched recursively; use `-i` to select the summary files.

The page has:

- an orthographic map of the latest float positions, colored by status (Plotly);
- a sortable, searchable table (DataTables) with one row per float: ID (with the WMO number under it), type and CTD (`/WIMS` added for a float
  with valid WIMS wave data),
  firmware, deployment date, age, distance profiled, status (active, overdue or archived, or
  predeployment for a float last seen at WHOI) with the last known location under it ("WHOI",
  or the sea or ocean, from Natural Earth's marine areas via `SeaNames.py`), and the latest cycle;
- a footer (`PageStatus`) with when the page was written, the pass's run time, and the decoder's
  status, uptime and version.

The page is rebuilt from scratch on every run. The table is also written to
`<output>/summary.json`, one record per row (see
[JSON files](../../README.md#fleet-summary-table-summaryjson)).

Options, besides the [common ones](#conventions-shared-by-the-tools):

| Option | Default | Description |
|---|---|---|
| `PATH ...` | | Summary files and/or directories to search (recursively). |
| `-o`, `--output DIR` | `.` | Write `index.html` and `summary.json` (the table as JSON, one record per row) to `DIR`. |
| `--runtime SECONDS` | none | Show `SECONDS` as the run time in the footer: the duration of the JSON and plot updates before the page was made (the service passes it). Without it, no run time is shown. |

Example:

```
xmtps-generate-summary-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2 --runtime 185 /data1/alamo/data/json
```

## `xmtps-generate-mission-page.py`

```
xmtps-generate-mission-page.py [-h] [-d] [-q] [-v] [-H] [-i REGEX] [-o DIR] [--plots-url URL]
                               [--days DAYS] [--runtime SECONDS] PATH ...
```

Builds the mission parameters page, `<output>/params.html`, from each float's `<sn>_summary.json`
and the `<sn>_mission.json` next to it. Directories are always searched recursively.

- **Rows:** every deployed float (one with a completed profile) that has sent mission parameters;
  with `--days`, only those that reported within `DAYS` days. Each row shows the parameters from
  the float's latest dive that has any, with its firmware, last report, status (as on the summary
  page: PREDEPLOYMENT when last seen at WHOI, otherwise ACTIVE within 30 days, OVERDUE within 180,
  otherwise ARCHIVED, with the last known location under it) and the dive.
- **Table:** one table of all floats, ALAMO (including WIMS and MM2) and ALTO, newest report first.
  Values are compared with those of the floats shown: typing in the search field, for example
  `ALTO`, `RBR` or a firmware version, recomputes the colors and the footer for the rows that
  match (in the browser; sorting does not change them). The heading shows how many floats are
  shown.
- **Columns:** phase `MPS`; pressures `DTD`, `PFD`/`PFM`, `MDS`, `CTF`, `ASD`; timing `PTO`, `SGT`,
  `SET`, `APT`; descent `DFT`, `DCM`, `PSM`; ascent `ARR`, `APM`; surface `SSP`, `SAM`, `SAO`; ice
  `ICM`, `ICT`; `PPE` and `WST`/`WMT`.
- **Phases:** `DTD`, `PFD`, `PFM`, `PTO` and `DFT` have a column per phase, P0–P2 (firmware without
  phases uses P0). The current phase is `MPS`, or `MNS<MPS>` once `MPC` reaches `MNM<MPS>`; its
  values are bold and underlined, and the Phase column shows `MPC`/`MNM`. Phases the mission
  never reaches, or with `MNM` 0, are gray and left out of the statistics.
- **Colors:** with three or more floats shown, each value is colored by how rare it is among them
  (1 − its floats / the most common value's floats): yellow to red above the most common value,
  blue below. `DFT` is colored by its distance from their median instead (white within 25%, full
  color at 200%). The mission phase column is never colored. The footer shows the most common
  value (`DFT`: the median) and the 10th–90th percentile range of the floats shown.
- **Unset and impossible values:** unset values (255, 65535, ...) are shown as "unknown" and not
  counted. Impossible values (phase outside 0–2, drift pressure outside 0–7000 dBar, drift or
  profile pressure deeper than `MDS`) are dark red.
- **Hover:** hovering over a cell shows the parameter, its value and how many floats share it. The
  table has a search field.

Options, besides the [common ones](#conventions-shared-by-the-tools):

| Option | Default | Description |
|---|---|---|
| `PATH ...` | | Summary files and/or directories to search (recursively). |
| `-o`, `--output DIR` | `.` | Write `params.html` to `DIR` (production: `plots2/engineering/`). |
| `--plots-url URL` | `..` | URL of the plots directory (float pages and `img/` status icons), relative to the page or absolute. The default suits a page one level below the plots directory; the service passes `/alamo`. |
| `--days DAYS` | all deployed floats | Include only floats that reported in the last `DAYS` days (decimals allowed). |
| `--runtime SECONDS` | none | Run time shown in the footer, as for the summary page. |

Example:

```
xmtps-generate-mission-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2/engineering --plots-url /alamo --days 365 /data1/alamo/data/json
```

## `xmtps-generate-startup-page.py`

```
xmtps-generate-startup-page.py [-h] [-d] [-q] [-v] [-H] [-i REGEX] [-o DIR] [--plots-url URL]
                               [--days DAYS] [--runtime SECONDS] PATH ...
```

Builds the startup page, `<output>/startup.html`, from each float's `<sn>_summary.json` and the
`<sn>_engineering_data.json` next to it. Directories are always searched recursively. It lists
the floats that started up in the last `--days` days, latest first.

- **Startup number and time:** each startup's messages have the next negative dive number (-1, -2,
  ...), so the startup number is minus the dive of the latest built-in test (BIT), and the startup
  time is the time that BIT reports (or the first SBD message, for BIT formats without times).
- **Columns:** status (Started or Deployed, with the startup number and cycles); startup,
  deployment and last report times; BITs sent and failed; and the latest BIT: duration, failed
  tests (spec B.2.6), subsystem error flags, firmware and BIT format, CTD pressure after
  calibration with temperature and salinity, vacuum and battery voltage (start / end), and for the
  air and oil pumps the duration and current, voltage and power (formats 9–11 report all of them,
  format 8 only the maximum currents and battery voltage).
- **Colors:** passed BITs green, failed BITs and error flags dark red, floats not yet deployed
  yellow. The table is sortable by any column and has a search field.

Options, besides the [common ones](#conventions-shared-by-the-tools):

| Option | Default | Description |
|---|---|---|
| `PATH ...` | | Summary files and/or directories to search (recursively). |
| `-o`, `--output DIR` | `.` | Write `startup.html` to `DIR` (production: `plots2/engineering/`). |
| `--plots-url URL` | `..` | URL of the plots directory (float pages), relative to the page or absolute; the service passes `/alamo`. |
| `--days DAYS` | `180` | Include floats whose latest startup was in the last `DAYS` days (decimals allowed). |
| `--runtime SECONDS` | none | Run time shown in the footer, as for the summary page. |

Example:

```
xmtps-generate-startup-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2/engineering --plots-url /alamo --days 30 /data1/alamo/data/json
```

## `FloatClasses.py`

The data classes shared by the two converters; not run directly. `xmtps-list2json.py` builds one
`Dive` per listing and pickles it; `xmtps-aggregate-json.py` unpickles the dives and groups them
into `Float` objects. Because the objects are pickled with jsonpickle, their attribute names and
types are part of the JSON format.

- **Times and positions:** `DateTime` (Unix seconds and ISO 8601 string), `Position`, `Gps`,
  `Sbd`, `CompositePosition` (combines Iridium positions) and `PositionVector`.
- **Dive data:** `Dive`, `Profile`, `Drift`, `Ascent` and `Descent` (both `PresTimeSeries`),
  `Surface`, `Pump`, `MissionParameters`, `FloatId`, `File` and `Float`.
- **Engineering:** `Engineering`, `EngineeringDescent`, `EngineeringDrift`, `EngineeringAscent`,
  `EngineeringSurface`, `EngineeringIce` and `EngineeringBit`.
- **Waves:** `Wave` holds a WIMS wave spectrum. Its `PostProcess()` interpolates the acceleration
  spectra onto a fixed frequency grid, applies the noise-floor mask and the low-frequency response
  correction (a cosine taper, `ccoef`), converts them to displacement, and computes the directional
  moments, wave height, periods and directions. Directions are referenced to true north using
  `MagneticDeclination()`, which needs the optional `ppigrf` package; without it they stay
  magnetic. The steps are listed in [spec/json-file-format.md](../../spec/json-file-format.md).
- **Helpers:** `RollingAverage()`.

A change here alters the JSON only for files written afterwards; rebuild existing data with `-f`.

## `RhoLUT.py`

Lookup tables of in-situ density (`rho_lut`) and potential density referenced to the surface
(`sigma0_lut`), both as density minus 1000 kg/m³; not run directly. They are tabulated on a grid
of salinity (0–39 PSU), temperature (−5–39 °C) and pressure (0–2900 dBar), in steps of 1 PSU, 1 °C
and 100 dBar, and interpolated trilinearly by `GetRhoFromLut(saln, temp, pres)` and
`GetSigma0FromLut(saln, temp, pres)`. The grid limits are `MIN_`/`MAX_SALN`, `_TEMP` and `_PRES`.
The file is large because the tables are written out in full.

## `PageStatus.py`

Writes the status line at the foot of the summary, mission parameters and startup pages
(`WriteStatusFooter()`); not run directly. For example:

```
Page last updated 2026-10-26T17:40Z (105 minutes ago, runtime: 3 minutes) - python version 0.11
- decoder status: active (uptime: 3 hours, 4 minutes) - xmtps version 1.0.3318b (built 2026-10-26)
```

- The decoder status and uptime come from `systemctl show xmtps-alamo` (`DECODER_SERVICE`), which
  needs no privileges.
- The version comes from `/data1/alamo/bin/xmtps --version` (`DECODER_BINARY`), which prints it
  without touching the running daemon.
- The "minutes ago" part is filled in by JavaScript when the page is viewed.
- Anything that cannot be determined is shown as "unknown".
