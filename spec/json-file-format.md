# XMTPS JSON File Format Specification

This document specifies the JSON files that the XMTPS Python tools (`bin/py/`) write and read: the per-dive files, the per-float aggregate tables, the per-float summary and the `.lastprocessed` marker. `xmtps-list2json.py` writes the per-dive files from the decoder's listing files. `xmtps-aggregate-json.py` reads them and writes the other three kinds. The aggregate script's plots and the three page generators read the aggregate and summary files.

Status: compiled on 2026-10-30 from the Python sources at commit `8643acf`, the README section "JSON files", the files in `example/json/`, and the production files on argus (`/data1/alamo/data/json/`, 460 float directories and 55,003 per-dive files on that date). Floats 9303 (ALAMO with WIMS wave sensor, firmware 10.10) and 11186 (ALTO, firmware 10.12) were inspected in detail.

## Contents

1. [Overview](#1-overview)
2. [Common conventions](#2-common-conventions)
3. [Per-dive files: `dives/<serial>_<dive>.json`](#3-per-dive-files-divesserial_divejson)
4. [Aggregate files: `<serial>_<name>.json`](#4-aggregate-files-serial_namejson)
5. [Float summary: `<serial>_summary.json`](#5-float-summary-serial_summaryjson)
6. [`dives/.lastprocessed`](#6-diveslastprocessed)
7. [Reading the files](#7-reading-the-files)
8. [Open questions / discrepancies](#8-open-questions--discrepancies)

Related specifications: [listing-file-format.md](listing-file-format.md) (the listing input of `xmtps-list2json.py`), [xmessage-format.md](xmessage-format.md) (the binary messages behind each listing line), [plot-and-page-files.md](plot-and-page-files.md) (the plots and pages made from these files).

## 1. Overview

### 1.1 Directory layout

All files of one float live in one directory under the output directory given with `-o`. In production that is `/data1/alamo/data/json/`.

```
/data1/alamo/data/json/
  9303/
    9303_ascent.json  9303_cycle_timing.json  ...  9303_summary.json  ...  9303_wave_spectra.json
    dives/
      .lastprocessed
      9303_-001.json  9303_-002.json  ...  9303_0000.json  ...  9303_0483.json
  11186/
    ...
```

### 1.2 Writers and readers

| File | Written by | Read by |
|---|---|---|
| `dives/<serial>_<dive>.json` | `bin/py/xmtps-list2json.py` (`WriteJsonDiveDataFile`) | `bin/py/xmtps-aggregate-json.py` (`ProcessJsonListingFile`) |
| `<serial>_<name>.json` | `bin/py/xmtps-aggregate-json.py` (`WriteAggregateJsonFiles`) | the plot functions of `xmtps-aggregate-json.py` (in memory, not from disk); `bin/py/xmtps-generate-mission-page.py` (`_mission`); `bin/py/xmtps-generate-startup-page.py` (`_engineering_data`) |
| `<serial>_summary.json` | `bin/py/xmtps-aggregate-json.py` | `bin/py/xmtps-generate-summary-page.py`, `bin/py/xmtps-generate-mission-page.py`, `bin/py/xmtps-generate-startup-page.py` |
| `dives/.lastprocessed` | `bin/py/xmtps-aggregate-json.py` (`WriteLastProcessedFiles`) | `bin/py/xmtps-aggregate-json.py` (`ProcessDirectory`) |

`bin/py/update-plots-service.sh` runs the two converters for each float with a recent listing, then the three page generators (README, "Python tools").

### 1.3 Data flow

1. `xmtps-list2json.py` parses one listing file `lis<serial>-<dive>.txt` into a `FloatClasses.Dive` object, adds density and σ₀ from `bin/py/RhoLUT.py`, computes wave statistics, calls `Dive.PostProcess()` and pickles the object with jsonpickle.
2. `xmtps-aggregate-json.py` unpickles every dive file of a float, concatenates the dives into pandas DataFrames, replaces listing fill values with NaN, derives positions, displacements and cycle timings, and writes each DataFrame with `DataFrame.to_json(orient='split', index=False)`. It writes the summary dict with jsonpickle.

## 2. Common conventions

### 2.1 Times

- Every time appears as a pair: `dt_epoch` (seconds since 1970-01-01T00:00:00Z) and `dt_str` (ISO 8601).
- `dt_str` has the form `YYYY-MM-DDThh:mm:ssZ` when it is made from an epoch value (`DateTime.dt_from_epoch`). When it is parsed from a listing it keeps the listing's text, which has the same form.
- `dt_epoch` parsed from a string is rounded to whole seconds and is an integer. `dt_epoch` set from a number keeps that number, so it can be a float (for example `1787845284.0`, or `1793229604.3083725` for a file modification time).
- The tools accept only times from epoch 1262322000 to 2209006800 (2010-01-01T05:00Z to 2040-01-01T05:00Z). A time outside that range, or one that cannot be parsed, is stored as `dt_epoch` -1 and `dt_str` `""` ("unset").

### 2.2 Units

| Quantity | Unit |
|---|---|
| Pressure | dBar |
| Temperature | °C (ITS-90 as reported by the CTD [inferred]) |
| Salinity | PSU |
| `dens` | in-situ density minus 1000, kg/m³ |
| `sigma0` | potential density referenced to 0 dBar, minus 1000, kg/m³ |
| Latitude, longitude | decimal degrees, north and east positive |
| CEP (`cep_km`) | km |
| Durations | s |
| Case vacuum | inHg |

`dens` and `sigma0` come from `bin/py/RhoLUT.py`: trilinear interpolation in tables on a grid of salinity 0–39 PSU, temperature −5–39 °C and pressure 0–2900 dBar (steps 1, 1 and 100), rounded to 3 decimals. Outside the grid (salinity or temperature out of range, pressure above 2900 dBar, or any input NaN) the value is NaN. Pressures below 0 are treated as 0.

### 2.3 File naming

| File | Name | Formatting |
|---|---|---|
| Per-dive | `<serial>_<dive>.json` | Python `'{:04d}_{:04d}'`: each number is zero padded to at least 4 characters, and the minus sign counts as a character. |
| Aggregate | `<serial>_<name>.json` | Serial as a plain integer (`'{}'`). |
| Summary | `<serial>_summary.json` | Serial as a plain integer. |
| Float directory | `<serial>/` | Serial as a plain integer. |

Examples of per-dive names:

| Serial | Dive | File |
|---|---|---|
| 9303 | 483 | `9303_0483.json` |
| 9303 | 0 | `9303_0000.json` |
| 9303 | −1 | `9303_-001.json` |
| 11186 | −12 | `11186_-012.json` |
| 9303 | 10000 | `9303_10000.json` |
| −1 (no `LIS` line) | −999 (no `LIS` line) | `-1/dives/-001_-999.json` |

Negative dive numbers are the messages a float sends before its mission starts: built-in tests and startup surfacings. Each startup uses the next negative number (−1, −2, …) (README, startup page). The listing files use the same numbers, so dive −1 of float 11186 comes from `lis11186--001.txt` (see [listing-file-format.md](listing-file-format.md)).

Because the dive part is at least four characters wide, `sorted()` on the file names gives −1, −2, …, −N, then 0, 1, 2, … for dives −999 to 9999. `xmtps-aggregate-json.py` reads a directory in that order, and the order carries into the row order of several aggregate files.

### 2.4 Versioning: `json_vers`

- The format version is the string `"0.11"` (`JSON_VERSION` in `xmtps-list2json.py`, `xmtps-aggregate-json.py` and the three page generators).
- It is stored as `json_vers` in every per-dive file and in every summary file. Aggregate files carry no version.
- Every reader compares `json_vers` with its own `JSON_VERSION` and ignores a file that differs, with a warning ("Ignoring file with mismatched JSON version").
- The version was not changed when attributes were added (October 2026). Files written by older code and files written by current code both say `0.11` (section 3.17). Readers must therefore treat attributes added after the first 0.11 files as optional. `xmtps-aggregate-json.py` does so with `getattr(..., default)`.

### 2.5 Missing values

| File kind | Missing value |
|---|---|
| Per-dive (jsonpickle) | The bare token `NaN` for a missing float. Integer sentinels (-1, -9, -999, -9.99) and empty strings for some attributes; see section 3.16. |
| Aggregate (pandas) | `null`. Unset times keep -1 and `""`. Separator rows have the string `"nan"` in `dt_str` (section 4.2). |
| Summary (jsonpickle) | Sentinels -999 and -1; `NaN` in an unset `Gps`/`Sbd` position. |

## 3. Per-dive files: `dives/<serial>_<dive>.json`

### 3.1 When a file is written

`xmtps-list2json.py` writes one file per listing file. A listing is converted when:

- its path matches an `-i` pattern (if any) and no `-x` pattern; and
- `-f` is given, or the dive's JSON file does not exist, or the JSON file's modification time is older than the listing's. With `-s` an existing JSON file is never replaced.

The check happens when the parser reaches the `LIS` line, which names the serial and dive. The output directory `<output>/<serial>/dives/` is created if needed. The file is written even if some lines failed to parse or `Dive.PostProcess()` failed; those failures are only logged (unless `-H`).

Production runs (`update-plots-service.sh`) include only listings of floats whose serial starts with 908, 909, 91, 92, 93, 94, 10 or 11, and exclude hidden files.

### 3.2 Encoding

The file is `jsonpickle.encode(dive, unpicklable=True, keys=True, separators=(',', ':'))` (`xmtps-list2json.py`, `WriteJsonDiveDataFile`):

- One line, no indentation, no spaces after `,` and `:`. UTF-8.
- Each object is a JSON object with a first member `"py/object"` naming its class, for example `"FloatClasses.Dive"`. Its other members are the object's attributes, in the order they were first assigned. The attribute names in `bin/py/FloatClasses.py` are therefore part of the format.
- Lists of numbers are plain JSON arrays. `PostProcess()` methods turn numpy arrays into lists before pickling.
- Missing floats are written as `NaN` (jsonpickle uses Python's `json` with `allow_nan=True`). `Infinity` is possible in principle but was not found in production.
- `py/id` back-references: jsonpickle writes `{"py/id": n}` for an object that already appeared in the same file. No current production per-dive file has a `py/id` that refers to a class object; the ones found are inside `py/reduce` numpy encodings (next item).
- `py/reduce`, `py/tuple`, `py/type`, `py/b64`: these appear only when a numpy array was not converted to a list, which happens when `Wave.PostProcess()` fails (section 3.15). Production example: `9237/dives/9237_0000.json` (a `WVS` line with `slen` 1).

### 3.3 Example

Trimmed from production file `9303/dives/9303_0483.json` (arrays cut to three elements):

```
{"py/object":"FloatClasses.Dive","json_vers":"0.11",
 "id":{"py/object":"FloatClasses.FloatId","whoi":9303,"dac":-1,"wmo":-1},
 "list_file":{"py/object":"FloatClasses.File","path":"data/list/9303/lis9303-0483.txt",
   "mtime":{"py/object":"FloatClasses.DateTime","dt_epoch":1793229604.3083725,"dt_str":"2026-10-28T23:20:04Z"}},
 "dive_num":483,
 "ascent":{"py/object":"FloatClasses.Ascent","dt_ref":{...},"n":21,"dt_ref_delta_sec":[0.0,...],...},
 "descent":{...},"drift":{...},"engineering":{...},
 "gps":[{"py/object":"FloatClasses.Gps","dt_epoch":1792313280,"dt_str":"2026-10-18T08:48:00Z",
   "lat":28.27064,"lon":-67.21926,"cep_km":0.082,"idx":1,"ttf_sec":0,"nsat":5,
   "snr_min":0,"snr_avg":0,"snr_max":0,"hdop":1.4},...],
 "mission":{"py/object":"FloatClasses.MissionParameters","fwver":"","ctdtype":"","cfgver":"",
   "parameters":{},"entries":[]},
 "profile":{"py/object":"FloatClasses.Profile","pres":[290.32,...],"saln_corr":[NaN,NaN,...],...},
 "pump":{...},"sbd":[...],"surface":{...},
 "wave":{"py/object":"FloatClasses.Wave","slen":256,"fsam":5.0,"raw_freq":[0.0,0.02,0.039,...],
   ...,"swh":2.87,"mwd":168,"declination":-13.477772008417247,"direction_ref":"true",
   "period_peak":12.5,"period_avg":9.7,"period_zero":8.6}}
```

The repository copy `example/json/dives/9303_0440.json` is in the same format but predates two changes (section 8, item 13).

### 3.4 `Dive` (top level)

| Attribute | Type | Contents and source |
|---|---|---|
| `json_vers` | string | `"0.11"` (section 2.4). |
| `id` | `FloatId` | Float identifiers, from the `LIS` line (3.5). |
| `list_file` | `File` | The listing the file was made from (3.5). |
| `dive_num` | int | Dive number, `LIS` field 2. -999 if the listing has no `LIS` line. Negative for pre-mission messages. |
| `ascent` | `Ascent` | Rise pressure time series (3.6). |
| `descent` | `Descent` | Fall pressure time series (3.6). |
| `drift` | `Drift` | Drift CTD samples (3.7). |
| `engineering` | `Engineering` | Engineering messages (3.14). |
| `gps` | list of `Gps` | GPS fixes, in listing order (3.11). Empty if none. |
| `mission` | `MissionParameters` | Mission configuration (3.13). |
| `profile` | `Profile` | Binned CTD profile (3.8). |
| `pump` | `Pump` | Pump runs (3.10). |
| `sbd` | list of `Sbd` | Iridium SBD messages of the dive, in listing order (3.12). |
| `surface` | `Surface` | Surface time series and pressure correction (3.9). |
| `wave` | `Wave` | Wave spectra and statistics (3.15). The key is absent when the listing has no `WVS` line. |

Members appear in the order above, except that `wave` is last.

### 3.5 `FloatId`, `File`, `DateTime`

`FloatId` (from the `LIS` line `LIS SN dive DAC-ID IMEI WMO-ID`):

| Attribute | Type | Contents |
|---|---|---|
| `whoi` | int | WHOI serial number (`SN`). -1 if no `LIS` line. |
| `dac` | int | DAC ID as listed. -1 when unknown (the listing prints `-001`). |
| `wmo` | int | WMO platform number. -1 when unknown (listing `-000001`). |

The `IMEI` field is not stored.

`File`:

| Attribute | Type | Contents |
|---|---|---|
| `path` | string | Listing path with the `-S` regular expression (default `.*/alamo/`) removed, for example `data/list/9303/lis9303-0483.txt`. The form depends on the path given on the command line; production also has paths like `11186/lis11186--001.txt`. `""` if `-S` is empty. |
| `mtime` | `DateTime` | Listing modification time. `dt_epoch` is the float from `os.path.getmtime`. |

`DateTime`:

| Attribute | Type | Contents |
|---|---|---|
| `dt_epoch` | int or float | Seconds since epoch; -1 if unset (section 2.1). |
| `dt_str` | string | ISO 8601 UTC; `""` if unset. |

### 3.6 `Descent` and `Ascent`

Both are `PresTimeSeries` objects. `Descent` comes from the `F`/`f` lines and `Ascent` from the `R`/`r` lines:

```
F    9303  483   0   17 2026-10-18T08:49:01Z
f    9303  483   0    0                   +0      0.00         1 # start of descent
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `dt_ref` | `DateTime` | | Reference time. While parsing it is the `F`/`R` line time (field 5). `PostProcess()` resets it to the earliest sample time. Unset if there are no samples. |
| `n` | int | | Number of samples, including inserted midpoints (below). |
| `dt_ref_delta_sec` | list of float | s | Sample time minus `dt_ref`. Listed samples come from the `f`/`r` field 5 (`+sec`). |
| `dt_epoch` | list of float | s | Sample times. |
| `dt_str` | list of string | | Sample times, ISO 8601. |
| `pres` | list of float | dBar | Pressure, `f`/`r` field 6. A listing fill value (-9.999) is kept. |
| `rate` | list of float | dBar/s | Pressure rate, positive when pressure decreases (rising). `NaN` at the first and last sample. |
| `phase_code` | list of int | | Phase code, the token before `#` on the `f`/`r` line. Midpoints: see below. Codes are listed in [xmessage-format.md](xmessage-format.md) A.5.2. |

`PostProcess()` (`FloatClasses.PresTimeSeries`) does the following with two or more samples:

1. It inserts a midpoint between each pair of listed samples, so `n` becomes 2n − 1. A midpoint has the mean pressure (rounded to 0.01 dBar) and the mean time offset (rounded to 0.1 s).
2. It computes `rate` (rounded to 4 decimals). At a midpoint the rate is the difference between its two neighbors. At a listed sample it is the difference between the midpoints on either side. Where the two times are equal, the previous point's rate is repeated.
3. It assigns midpoint phase codes: 10 after a sample with code 1, 2, 3 or 10; 12 after 6 or 12; 13 after 7 or 13; otherwise -1.
4. It fills `dt_epoch` and `dt_str` from `dt_ref` + `dt_ref_delta_sec` and deletes the parse-time `dt` attribute.

With no samples, `dt` is deleted and the lists stay empty. With exactly one sample nothing is done: the object then keeps a `dt` member (a list of one `DateTime`) and `dt_epoch`, `dt_str` and `rate` are empty lists. Readers should not rely on `dt`.

### 3.7 `Drift`

From the `D` and `d` lines. The `d` columns are matched by name to the latest `#  SN dive nSam ...` header; without a matching header, fields 4–6 are taken as `pres`, `temp`, `saln`.

```
D    9303  483    8 2026-10-18T12:57:53Z
d    9303  483    0                   +1    284.56  19.080  36.689
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `dt_epoch` | list of int or float | s | Sample times (below). `NaN` where none could be set. |
| `dt_str` | list of string | | Sample times, ISO 8601. `NaN` (not a string) where none could be set. |
| `dt_ref` | `DateTime` | | Time of the first sample. Unset if times could not be set. An empty list `[]` when `n` is 0 and the dive has ascent and descent samples (code path `EstimateDatetimes`). |
| `dt_ref_delta_sec` | list | s | Sample time minus `dt_ref`. |
| `pres` | list of float | dBar | `pres` column. Fill value -9.999 is kept. |
| `temp` | list of float | °C | `temp` column; `NaN` if ≤ −5 (`RhoLUT.T[0]`). |
| `saln` | list of float | PSU | `saln` column; `NaN` if ≤ 0 (`RhoLUT.S[0]`). |
| `dens` | list of float | kg/m³ − 1000 | `RhoLUT.GetRhoFromLut(saln, temp, pres)`. |
| `sigma0` | list of float | kg/m³ − 1000 | `RhoLUT.GetSigma0FromLut(saln, temp, pres)`. |
| `n` | int | | Number of samples, the `D` line `nSam`. |

Sample times (`Dive.PostProcess()`):

- **Measured.** Floats that send drift times (message 0x78–0x7F, [xmessage-format.md](xmessage-format.md) A.4.1) list a time on the `D` line and a `time` column (`+sec` after it) on each `d` line. If every sample has a time, those are used.
- **Estimated.** Otherwise, if the dive has ascent and descent samples, the n samples are spaced evenly between the last descent sample time and the first ascent sample time, at the interior points of n + 2 evenly spaced times, rounded to whole seconds. These times are floats.
- **None.** Otherwise the times stay `NaN`.

The sample index is the `nSam` column in the drift-times layout and field 3 otherwise.

### 3.8 `Profile`

From the `PRO` line (number of bins) and the `pro` lines. Columns are matched by name to the latest `#  SN dive sampleIdx ...` header. Without one, a 7-field line is read as `pres temp saln` and an 8-field line as `pres temp1 temp2 saln`.

```
PRO  9303  483         290
pro  9303  483           0      290.32      19.070      19.063      36.689
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `pres` | list of float | dBar | `pres`. Fill value kept. |
| `temp` | list of float | °C | `temp`, or `temp1` when the float has an auxiliary channel. `NaN` if ≤ −5. |
| `tcnd` | list of float | °C | `temp2`, the conductivity-cell temperature (auxiliary channel, [xmessage-format.md](xmessage-format.md) A.4.3). `NaN` if not listed or ≤ −5. |
| `saln` | list of float | PSU | `saln`. `NaN` if ≤ 0. |
| `saln_corr` | list of float | PSU | `salnCorr`, the dynamically corrected salinity (message 0xDC, A.4.2). `NaN` if not listed or ≤ 0. |
| `dens` | list of float | kg/m³ − 1000 | From `RhoLUT` (uses `temp`, not `tcnd`). |
| `sigma0` | list of float | kg/m³ − 1000 | From `RhoLUT`. |
| `n` | int | | Number of bins (`PRO` `numSamples`). 0 without a `PRO` line. |

Bins are in listing order, which is deepest first. A bin that has no `pro` line stays `NaN`. The `PRD`/`prd` lines (column descriptions) are ignored.

### 3.9 `Surface`

From the `SFC`/`sfc` lines (surface time series) and the `PRC` line (surface pressure offset correction).

```
PRC  9303  483    0   79     4     1           -0.04    -0.05    -0.01     0.06    0.034
SFC  9303  483   0   21 2026-10-19T14:36:43Z
sfc  9303  483   0    0                   +0      0.13  29.218  36.727        14 # surface
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `dt_ref` | `DateTime` | | `SFC` time while parsing; the earliest sample time after `PostProcess()`. |
| `n` | int | | Number of `sfc` lines. |
| `dt_ref_delta_sec` | list of int | s | `sfc` field 5. |
| `dt_epoch` | list of int | s | Sample times. |
| `dt_str` | list of string | | Sample times. |
| `pres` | list of float | dBar | `sfc` field 6. |
| `temp` | list of float | °C | `sfc` field 7; `NaN` if ≤ −9.99. |
| `saln` | list of float | PSU | `sfc` field 8; `NaN` if ≤ 0. |
| `pres_correction` | float | dBar | `PRC` `presOffset` (field 7). `NaN` without a `PRC` line. |
| `pres_correction_valid` | bool | | `PRC` `valid` = 1. `false` without a `PRC` line. |
| `pres_correction_num_samples` | int | | `PRC` `nSam`. -1 without a `PRC` line. |
| `pres_correction_num_dives` | int | | `PRC` `nDive`. -1 without a `PRC` line. |

The `sfc` phase code and the `PRC` statistics (`sfcPmin` … `sfcPstd`) are not stored.

### 3.10 `Pump`

From the `pmp` lines, one list entry per pump run:

```
#      SN dive idx nSam       pres      pumpT      pumpV      pumpI       vac0       vac1
pmp  9303  483   0    0      70.68         93       7.31      0.217     655.35       5.77
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `pres` | list of float | dBar | `pres` (field 5). |
| `time` | list of float | s | `pumpT` (field 6). |
| `volt` | list of float | V | `pumpV` (field 7). |
| `curr` | list of float | A | `pumpI` (field 8). |
| `curr_adj` | list of float | A | `curr × 12 / volt / 0.85`, 3 decimals; `NaN` if `volt` is 0. The current referred to a 12 V supply at 85 % efficiency [inferred]. |
| `vac` | list of float | inHg | `vac1` (field 10), the case vacuum after the run. `vac0` is not stored. Fill 655.35 is kept. |
| `power` | list of float | W | `volt × curr_adj`, 3 decimals. |
| `energy` | list of float | J | `power × time`, 2 decimals. |
| `n` | int | | Number of runs. |

`PMP` lines are ignored.

### 3.11 `Gps`

One object per `GPS` line ([xmessage-format.md](xmessage-format.md) A.2):

```
GPS  9303  483   1 2026-10-18T08:48:00Z  +28.27064-67.21926     0    5     0     0     0  1.4    0
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `dt_epoch`, `dt_str` | int, string | | Fix time, field 4. Unset for an invalid fix. |
| `lat`, `lon` | float | deg | Field 5 (ISO 6709). An invalid fix has the decoder's 99.99999 / 999.99999. |
| `cep_km` | float | km | Estimated CEP (below). |
| `idx` | int | | Field 3: the GPS message ID. 0 = after the diagnostic dive, 1 = start of dive, 2 = end of dive, 3 = after an abort, 5 = built-in test. |
| `ttf_sec` | int | s | Time to fix, field 6. |
| `nsat` | int | | Satellites used, field 7. |
| `snr_min`, `snr_avg`, `snr_max` | int | | Fields 8–10. 0 on 10.12 firmware; 255 when not available [inferred from the aggregate fill rule]. |
| `hdop` | float | | Field 11. |

The `stat` column is not stored. Unset defaults are -1 for the integers and -9.0 for `hdop`.

`cep_km` (`Gps.EstimateUncertainty`): if the time or position is invalid, -9. Otherwise, in km, rounded to 3 decimals:

```
cep_km = 0.00666
       + 0.01666   * (hdop - 1)        if hdop > 1
       + 0.02666   * (6 - nsat)^2      if nsat < 6
       + 0.001666  * (25 - snr_avg)    if snr_avg < 25
```

A fix is "valid" (`Position.is_valid`) when its time is in range, |lat| ≤ 90, |lon| ≤ 180 and `cep_km` > 0.

### 3.12 `Sbd`

One object per `SBD` line:

```
SBD  9303  483  287 000000002418110  6328     0  11 2026-10-19T14:46:44Z  +28.44964-67.49324   22
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `dt_epoch`, `dt_str` | int, string | | Iridium session time, field 8. |
| `lat`, `lon` | float | deg | Iridium position, field 9. |
| `cep_km` | int | km | Iridium CEP, field 10. |
| `size_b` | int | bytes | Message size, field 3. |
| `momsn` | int | | MOMSN, field 5. |
| `mtmsn` | int | | MTMSN, field 6. |
| `pid` | int | | Packet ID, field 7. |

The IMEI (field 4) is not stored. Unset defaults are -1. See [iridium-mail-format.md](iridium-mail-format.md) and [sbd-file-format.md](sbd-file-format.md) for where these values come from.

### 3.13 `MissionParameters`

From the `MIS` line and the `m` lines. Most dives have none; floats send their mission configuration on request or after a change.

```
MIS 11186  113             0x18  0x0A0C0001           0      # RBR
# --- General ------------------------------------------------------------------------------------
m   11186  113   0 MSN          113      [-]          # dive cycle number
```

| Attribute | Type | Contents and source |
|---|---|---|
| `fwver` | string | Firmware version word, for example `"0x0A0C0001"` (the second hex field of `MIS`; field 4 on old listings without a configuration version). `""` if no `MIS` line. |
| `ctdtype` | string | `"RBR"` for sensor type 0, `"SBE"` for 1, otherwise `""`. |
| `cfgver` | string | Mission configuration message version, for example `"0x18"`. `""` if no `MIS` line or an old listing. |
| `parameters` | object | `{name: value}` for every `m` line. A numeric value is a float (`113.0`); anything else (for example a hex string) stays a string. A later line with the same name replaces an earlier one. |
| `entries` | list of object | One object per `m` line, in listing order (below). |

`entries` members:

| Key | Type | Contents |
|---|---|---|
| `section` | string | The latest `# --- Name ---` heading, for example `"General"`; `""` outside one. |
| `idx` | int | `m` field 3. |
| `name` | string | Parameter name, for example `"MSN"`, `"DTD0"`. |
| `value` | int, float or string | `m` field 5, converted by `ParseValue` (integers and decimals with a point become numbers; hex, `nan`, `1e5` stay strings). |
| `units` | string | Field 6 without brackets, for example `"-"`, `"dBar"`; `""` if missing. |

The description after `#` is not stored. Unset configuration values (255, 65535, 4294967295, -32768, 655.35, ...) are stored as listed. The parameters themselves are described in [configuration-file-format.md](configuration-file-format.md) and [xmessage-format.md](xmessage-format.md) C.1.

### 3.14 `Engineering`

#### 3.14.1 Structure

| Attribute | Type | Contents |
|---|---|---|
| `ascent` | `EngineeringAscent` | `E09` lines. |
| `descent` | `EngineeringDescent` | `E07` lines. |
| `drift` | `EngineeringDrift` | `E08` lines. |
| `surface` | `EngineeringSurface` | `E0A` lines. |
| `ice` | `EngineeringIce` | `E0C` lines. |
| `bit` | `EngineeringBit` | `E05` (passed) and `E06` (failed) built-in test lines. |
| `errors` | list of object | One object per error line `e0x` (3.14.9). |
| `extra` | object | `{section: {column: value}}` for listing columns not in the mapping (3.14.10). |

#### 3.14.2 How listing columns become attributes

`xmtps-list2json.py` (`ParseEngineeringLine`) reads each `E0x` line against the latest `#  SN dive midx ...` header and the units line under it. A unit belongs to a column when the right edges of their text are within 2 characters. Each column is looked up in `ENG_COLUMNS[section]` in this order:

1. `<sub-group>/<name>`, where the sub-group is the latest `# --- Name ---` heading (used by BIT versions 9–11);
2. `<name>|<unit>` (the same name listed in physical units and in counts, for example `pres|counts`);
3. `<name>`.

Values are converted with `ParseValue`: integers stay ints, decimals become floats, everything else (hex words such as `"0x14"`, status strings) stays a string. Attributes whose name starts with `dt_` get a `DateTime`. A last column printed as a comment (the BIT `sensorTypeName`, `... 0 # RBR`) is taken from after the `#`. A line whose field count does not match its header is skipped (logged at debug level). If a section has several lines, later lines overwrite earlier values.

Attributes that the float's message version does not report keep their default: `NaN` for numbers, `""` for strings, an unset `DateTime`, and -9.99 for `vac0`/`vac1`. Units are encoded in attribute names: `_dbar`, `_degc`, `_psu`, `_v` (V), `_a` (A), `_w` (W), `_s` (s), `_inhg`, `_psi`, `_counts` (raw sensor counts).

The message layouts are in [xmessage-format.md](xmessage-format.md) part B.

#### 3.14.3 `EngineeringDescent` (`E07`, versions 0x10–0x11)

| Attribute | Listing column | Units | Versions |
|---|---|---|---|
| `dt_start` | `start-of-descent` | `DateTime` | all |
| `vac0`, `vac1` | `vac0`, `vac1` | inHg (start, end of phase) | 0x11 |
| `version` | `engVer` | string, e.g. `"0x11"` | all |
| `seeks` | `seeks` | count | 0x11 |
| `seek_pump_adj_s` | `seekPumpAdjT` | s | 0x11 |
| `oil_valve_open_pos_surf_v` | `OilValveOpenPosSurf` | V | 0x11 |
| `oil_valve_close_pos_fall_pump_v` | `OilValveClosePosFallPump` | V | 0x11 |
| `air_valve_mid_pos_surf_v` | `AirValveMidPosSurf` | V | 0x11 |
| `pump_time_s` | `pumpT` | s | 0x10 |
| `pump_current_a` | `pumpI` | A | 0x10 |
| `system_current_a` | `sysI` | A | 0x10 |
| `system_voltage_v` | `sysV` | V | 0x10 |
| `case_vac_inhg` | `vac` | inHg | 0x10 |
| `first_pump_adj_s` | `pumpAdj` | s | 0x10 |

#### 3.14.4 `EngineeringDrift` (`E08`, versions 0x10–0x12)

| Attribute | Listing column | Units | Versions |
|---|---|---|---|
| `dt_start` | `start-of-drift` | `DateTime` | all |
| `vac0`, `vac1` | `vac0`, `vac1` | inHg | 0x11+ |
| `version` | `engVer` | string | all |
| `seeks` | `seeks` | count | 0x11+ |
| `seek_pump_adj_s` | `seekPumpAdjT` | s | 0x11+ |
| `mean_pres_dbar` | `meanP` (0x10), `pres` in dBar (0x11+) | dBar | all |
| `mean_pres_counts` | `pres` in counts | counts | 0x11+ |
| `mean_temp_degc` | `meanT` (0x10), `temp` in degC (0x11+) | °C | all |
| `mean_temp_counts` | `temp` in counts | counts | 0x11+ |
| `mean_saln_psu` | `saln` in PSU | PSU | 0x11+ |
| `mean_saln_counts` | `saln` in counts | counts | 0x11+ |
| `dt_fall_start` | `start-of-fall` | `DateTime` | 0x10 |
| `fall_start_pres_dbar` | `fallP` | dBar | 0x10 |

#### 3.14.5 `EngineeringAscent` (`E09`, versions 0x10–0x12)

| Attribute | Listing column | Units | Versions |
|---|---|---|---|
| `dt_start` | `start-of-ascent` | `DateTime` | all |
| `vac0`, `vac1` | `vac0`, `vac1` | inHg | 0x11+ |
| `version` | `engVer` | string | all |
| `system_current_a` | `sysI` | A | 0x10 |
| `pump_time_s` | `pumpT` | s | 0x10 |
| `dt_fall_start` | `start-of-fall` | `DateTime` | 0x12 |
| `fall_start_pres_dbar` | `fallP` | dBar | 0x12 |

#### 3.14.6 `EngineeringSurface` (`E0A`, versions 0x10–0x14)

| Attribute | Listing column | Units | Versions |
|---|---|---|---|
| `dt_start` | `start-of-surface` | `DateTime` | all |
| `vac0`, `vac1` | `vac0`, `vac1` | inHg | 0x11+ |
| `version` | `engVer` | string | all |
| `ctd_status` | `CTD-status` | string | 0x10 |
| `case_vac_inhg` | `vac` | inHg | 0x10 |
| `system_voltage_v` | `sysV` | V | 0x10 |
| `system_current_a` | `sysI` | A | 0x10 |
| `num_sbd` | `nSBD` | count | all |
| `num_prev_attempts` | `nAtmpt` (0x10), `nPrevAttempt` | count | all |
| `air_pump_current_a` | `APCur` | A | 0x11+ [inferred] |
| `air_pump_runtime_s` | `APRunT` | s | 0x11+ [inferred] |
| `air_bladder_max_pres_psi` | `AirBladMxP` | psi | 0x11+ [inferred] |
| `air_valve_open_start_v` | `AVOpenStart` | V | 0x11+ [inferred] |
| `air_valve_mid_end_v` | `AVMidEnd` | V | 0x11+ [inferred] |
| `air_valve_open_cal_v` | `ACVpenCal` | V | 0x11+ [inferred] |
| `air_valve_mid_cal_v` | `AVMidCal` | V | 0x11+ [inferred] |
| `air_valve_close_cal_v` | `AVCloseCal` | V | 0x11+ [inferred] |
| `oil_valve_open_air_pump_v` | `OVOpenAirPmp` | V | 0x11+ [inferred] |
| `oil_valve_close_end_v` | `OVCloseEnd` | V | 0x11+ [inferred] |
| `oil_valve_open_cal_v` | `OVOpenCal` | V | 0x11+ [inferred] |
| `oil_valve_close_cal_v` | `OVClosCal` | V | 0x11+ [inferred] |
| `pres_offset_dbar` | `presOffset` | dBar | 0x12+ |
| `prof_first_pres_dbar`, `prof_first_pres_counts` | `firstPres` (dBar, counts) | dBar, counts | 0x13+ |
| `prof_first_temp_degc`, `prof_first_temp_counts` | `firstTemp` | °C, counts | 0x13+ |
| `prof_first_saln_psu`, `prof_first_saln_counts` | `firstSaln` | PSU, counts | 0x13+ |
| `prof_last_pres_dbar`, `prof_last_pres_counts` | `lastPres` | dBar, counts | 0x13+ |
| `prof_last_temp_degc`, `prof_last_temp_counts` | `lastTemp` | °C, counts | 0x13+ |
| `prof_last_saln_psu`, `prof_last_saln_counts` | `lastSaln` | PSU, counts | 0x13+ |
| `prof_num_bins` | `numBins` | count | 0x13+ |
| `prof_num_empty_bins` | `numEmptyBins` | count | 0x13+ |
| `prof_num_raw_samples` | `numRawSamples` | count | 0x13+ |
| `gps_rtc_offset_s` | `gpsOffset` | s | 0x13+ |
| `dt_sampling_start` | `start` (surface sampling) | `DateTime` | 0x14 |
| `dt_sampling_end` | `end` (surface sampling) | `DateTime` | 0x14 |
| `num_errors` | `nErr` | count | 0x11+ [inferred] |

"First" and "last" refer to the first and last profile bin.

#### 3.14.7 `EngineeringIce` (`E0C`, versions 0x10–0x11)

No `dt_start`, `vac0` or `vac1`.

| Attribute | Listing column | Units | Versions |
|---|---|---|---|
| `version` | `engVer` | string | all |
| `mode` | `mode` | | all |
| `ice_status` | `ice-status` | | all |
| `urgency` | `urgency` | | all |
| `surface_attempts` | `surfAttempts` | count | all |
| `num_stored_sbd` | `numStoredSBD` | count | all |
| `time_since_last_xmit_s` | `lastXmit` | s | all |
| `min_pres_dbar` | `minPres` | dBar | all |
| `test_temp` | `testTemp` | raw counts, as listed | all |
| `test_saln_psu` | `testSalinity` | PSU | all |
| `num_errors` | `nErr` | count | 0x11 |

#### 3.14.8 `EngineeringBit` (`E05`/`E06`, BIT versions 6–8 and 9–11)

No `vac0`/`vac1`. Versions 9–11 list several sub-groups (`# --- Air Pump ---` and so on) that reuse column names; the second column below gives `sub-group/name` for those.

| Attribute | Versions 6–8 column | Versions 9–11 column | Units |
|---|---|---|---|
| `version` | `BITVer` | `BITVer` | int |
| `sw_version` | `SWVer` | `FWVer` | string (hex word) |
| `results` | `BITResults` | `BITResults` | string (hex bit mask of failed tests) |
| `sensor_type` | — | `sensorType` | int |
| `sensor_type_name` | — | `sensorTypeName` | string, e.g. `"RBR"` |
| `duration_s` | `BITTime` | `Date/Time/duration` | s |
| `dt_start` | — | `startTime` | `DateTime` |
| `dt_end` | — | `endTime` | `DateTime` |
| `ctd_status` | `CTDStat` | `CTD/status` | string |
| `ctd_errors` | — | `CTD/errors` | string (hex) |
| `pres_dbar`, `pres_counts` | `pres` (v7) | — | dBar, counts |
| `pres_post_reset_dbar` | `presPostReset` (v8) | `presPostCal` | dBar |
| `pres_post_reset_counts` | `presPostReset` counts (v8) | — | counts |
| `pres_pre_reset_dbar` | `presPreReset` (v8) | `presPreCal` | dBar |
| `pres_pre_reset_counts` | `presPreReset` counts (v8) | — | counts |
| `pres_offset_dbar` | `presOffset` | `presOffset` | dBar |
| `temp_degc`, `temp_counts` | `temp` (counts v7–8) | `temp` | °C, counts |
| `saln_psu`, `saln_counts` | `saln` (counts v7–8) | `saln` | PSU, counts |
| `case_vac_errors` | — | `Case Vacuum/errors` | string |
| `case_vac_start_inhg` | `VacStart` | `caseVacStart` | inHg |
| `case_vac_end_inhg` | `VacEnd` | `caseVacEnd` | inHg |
| `battery_errors` | — | `Battery Voltage/errors` | string |
| `battery_start_v` | `BatStart` | `batVoltStart` | V |
| `battery_end_v` | `BatEnd` | `batVoltEnd` | V |
| `air_valve_open_v` | `AVOpenPos` | `Air Valve Positions/open` | V |
| `air_valve_open_cal_v` | `AVOpenPosCal` | `Air Valve Calibration Positions/open` | V |
| `air_valve_mid_v` | `AVMidPos` | `Air Valve Positions/mid` | V |
| `air_valve_mid_cal_v` | `AVMidPosCal` | `Air Valve Calibration Positions/mid` | V |
| `air_valve_closed_v` | `AVClosePos` | `Air Valve Positions/closed` | V |
| `air_valve_closed_cal_v` | `AVClosePosCal` | `Air Valve Calibration Positions/closed` | V |
| `air_pump_errors` | — | `Air Pump/errors` | string |
| `air_pump_test_s` | `APTestTime` | `Air Pump/duration` | s |
| `air_pump_cycles` | `APCycleCount` | `Air Pump/cycles` | count |
| `air_pump_max_pres_psi` | `APMxAPres` | `Air Pump/presMax` | psi |
| `air_pump_current_avg_a` | — | `Air Pump/currAvg` | A |
| `air_pump_current_max_a` | `APMxLoadCur` | `Air Pump/currMax` | A |
| `air_pump_current_max_12v_a` | `APMxLoadCur12V` | `Air Pump/12VCurrMax` | A |
| `air_pump_battery_max_v` | `APMxLoadBatV` | `Air Pump/batMax` | V |
| `air_pump_power_max_w` | — | `Air Pump/powerMax` | W |
| `oil_valve_open_v` | `OVOpenPos` | `Oil Valve Positions/open` | V |
| `oil_valve_open_cal_v` | `OVOpenPosCal` | `Oil Valve Calibration Positions/open` | V |
| `oil_valve_closed_v` | `OVClosePos` | `Oil Valve Positions/closed` | V |
| `oil_valve_closed_cal_v` | `OVClosePosCal` | `Oil Valve Calibration Positions/closed` | V |
| `oil_pump_errors` | — | `Oil Pump/errors` | string |
| `oil_pump_test_s` | `OPTestTime` | `Oil Pump/duration` | s |
| `oil_pump_current_avg_a` | — | `Oil Pump/currAvg` | A |
| `oil_pump_current_max_a` | `OPMxLoadCur` | `Oil Pump/currMax` | A |
| `oil_pump_current_max_12v_a` | `OPMxLoadCur12V` | `Oil Pump/12VCurrMax` | A |
| `oil_pump_battery_max_v` | `OPMxLoadBatV` | `Oil Pump/batMax` | V |
| `oil_pump_power_max_w` | — | `Oil Pump/powerMax` | W |

A dive with several BIT lines (several tests) keeps the values of the last one. The startup page (`xmtps-generate-startup-page.py`) reads the BIT values back from `_engineering_data` (section 4.11).

#### 3.14.9 `errors`

One object per error line (`e05` … `e0C`):

```
#      SN dive  idx            date-time        err-code
e0A 09303  483    0 2026-10-19T14:15:58Z      0x00000114
```

| Key | Type | Contents |
|---|---|---|
| `section` | string | `descent`, `drift`, `ascent`, `surface`, `ice` or `bit` (from the tag; an unknown tag gives its lower-case tag, e.g. `"e0b"`). |
| `idx` | int | Field 3. |
| `dt_str` | string | The `date-time` column. Absent for surface version 0x10 error lines, which have no time. |
| `code` | string | The `err-code` column, a hex word such as `"0x00000114"`. |

Any other column in the error header becomes a key of the same name.

#### 3.14.10 `extra`

Listing columns that `ENG_COLUMNS` does not map are kept here as `{section: {column: value}}`. When a name repeats within a section, later ones get a `_<unit>` suffix (for example `pres_counts`). No production file had a non-empty `extra` on 2026-10-30.

### 3.15 `Wave`

Present only when the listing has a `WVS` line (floats with a WIMS wave sensor; firmware build byte 0x31). From the `WVS` line (spectral setup) and the `wvs` lines (one frequency bin each). The `wvm` lines (the decoder's own displacement spectra) are ignored. See [xmessage-format.md](xmessage-format.md) C.3.

```
WVS  9303  483 0x02  256   5.0000   45    18     2048  4.000    35000.0
wvs  9303  483    0   0.0000    +0.00006    +0.00040    +0.00089    +0.00023    +0.00000    +0.00000
```

| Attribute | Type | Units | Contents and source |
|---|---|---|---|
| `slen` | int | samples | FFT length, `WVS` field 4. |
| `fsam` | float | Hz | Sample rate, `WVS` field 5. |
| `raw_freq` | list of float, `slen/2` values | Hz | `k × fsam / slen` for k = 0 … slen/2 − 1, 3 decimals. Bin 0 is the DC bin. The listing's `freq` column is not used. |
| `raw_Szz_g`, `raw_Syy_g`, `raw_Sxx_g` | list of float | g²/Hz | Vertical and the two horizontal acceleration auto-spectra as received (`wvs` fields 5, 6, 7), 4 decimals. `NaN` for a bin not received (listing fill -999.99999). |
| `raw_Cxy_g` | list of float | g²/Hz | Horizontal co-spectrum, field 8. |
| `raw_Qzx_g`, `raw_Qzy_g` | list of float | g²/Hz | Quadrature spectra, fields 9, 10. |
| `freq` | list of float, 245 values | Hz | 0.030 to 1.250 in steps of 0.005 (the interpolation grid). |
| `Szz_g` … `Qzy_g` | list of float, 245 | g²/Hz | The six acceleration spectra interpolated linearly onto `freq`; 0 outside the received band; 4 decimals. |
| `Szz`, `Syy`, `Sxx`, `Cxy`, `Qzx`, `Qzy` | list of float, 245 | m²/Hz | Displacement spectra (below), 3 decimals. All `NaN` when the dive's spectra are rejected. |
| `a0`, `a1`, `a2`, `b1`, `b2` | list of float, 245 | | Directional Fourier coefficients, 4 decimals. |
| `r1`, `r2` | list of float, 245 | | Spreading parameters √(a1² + b1²), √(a2² + b2²), in [0, 1], 3 decimals. |
| `theta1` | list of int, 245 | deg | Mean direction per bin, toward which waves travel, clockwise from north, [0, 360). 0 where undefined. |
| `theta2` | list of int, 245 | deg | Principal direction per bin, the end of the principal axis nearest `theta1`, [0, 360). |
| `wavenumber` | list of float, 245 | rad/m | Deep-water wavenumber (2πf)²/g, 5 decimals. |
| `moment` | list of 3 floats | m², m²·Hz, m²·Hz² | Spectral moments m0, m1, m2 of `Szz`, summed up to the first `NaN` bin, 5 decimals. |
| `swh` | float | m | Significant wave height 4√m0, 2 decimals. |
| `mwd` | int | deg | Mean wave direction, toward which waves travel, [0, 360). 0 when the spectra are rejected. |
| `declination` | float | deg, east positive | Magnetic declination added to the directions (IGRF via `ppigrf`). `NaN` if none was applied. |
| `direction_ref` | string | | `"true"` when the declination was applied, otherwise `"magnetic"`. |
| `period_peak` | float | s | 1 / frequency of the largest `Szz` bin, 1 decimal. |
| `period_avg` | float | s | m0 / m1, 1 decimal. |
| `period_zero` | float | s | √(m0 / m2), 1 decimal. |

Processing (`Wave.PostProcess`, defaults `freq_min` 0.0625 Hz, interpolation on, 3-point rolling average):

1. Copy the received spectra to the `raw_` attributes.
2. Interpolate the six acceleration spectra onto `freq` (values outside the received band become 0).
3. Weight every spectrum by 0 where `Szz_g` < 0.0005 g²/Hz (noise-floor mask) and by the low-frequency taper: 0 below f0, (1 − cos(π (f − f0) / w)) / 2 from f0 to f0 + w, 1 above, with f0 = 0.02595 Hz and w = 0.08834 Hz. The `_g` attributes keep the unweighted values.
4. Displacement spectrum = weight × g² / (2πf)⁴ × acceleration spectrum, with g = 9.80665 m/s².
5. Smooth the six displacement spectra with a centered 3-point moving average.
6. Reject the spectra (set all six displacement spectra to `NaN`) if any acceleration auto-spectrum exceeds 0.09 g²/Hz at or below 0.05 Hz, or 0.3 g²/Hz anywhere, or any displacement auto-spectrum at or above `freq_min` exceeds 300 m²/Hz, or `Szz` is `NaN` there.
7. a0 = Szz / π; a1 = Qzx / √(Szz (Sxx + Syy)); b1 = Qzy / √(Szz (Sxx + Syy)); a2 = (Sxx − Syy) / (Sxx + Syy); b2 = 2 Cxy / (Sxx + Syy); θ1 = atan2(a1, b1); θ2 = atan2(b2, −a2) / 2, flipped by 180° if it points away from θ1.
8. If m0 > 0: compute `swh` and the periods; `mwd` is the mean of `theta1` weighted by √Szz over `freq_min` to 0.6 Hz (bins with `r1` `NaN` get no weight). The declination at the dive's surface fix is added when available. If m0 ≤ 0: the displacement spectra, coefficients, moments and statistics are `NaN`, the angles 0, and no declination is applied.

The declination uses the first valid fix with `idx` 0 or 2, else the first valid fix of the dive (`Dive.SurfaceFix`). A dive without a valid fix uses the latest valid fix of the most recent earlier dive with dive number ≥ 0 (`PreviousValidFix` in `xmtps-list2json.py`).

If `Wave.PostProcess()` raises (for example `slen` 1, which gives empty arrays), the `Wave` object is written with its numpy arrays unconverted (`py/reduce` encoding), and the rest of `Dive.PostProcess()` after the wave step (the drift sample times) is skipped.

### 3.16 Defaults and sentinels in per-dive files

| Value | Where | Meaning |
|---|---|---|
| `NaN` | any float | Not reported, rejected, or out of range. |
| -1 / `""` | `DateTime` | Unset time. |
| -999 | `Dive.dive_num` | No `LIS` line. |
| -1 | `FloatId.*`, `Gps` integers, `Sbd` integers, `Surface.pres_correction_num_*` | Unknown or not set. |
| -9.0 | `Gps.hdop` | Not set. |
| -9 | `Gps.cep_km` | Invalid fix. |
| 99.99999, 999.99999 | `Gps.lat`, `Gps.lon` | Invalid fix (decoder value). |
| -9.99 | `Engineering*.vac0`, `vac1` | Not reported. |
| `""` | engineering string attributes, `MissionParameters` strings | Not reported. |
| -1 | `Pump.n` | `PostProcess()` was not run. |

Listing fill values are not removed in per-dive files, except where noted: temperature ≤ −5 °C and salinity ≤ 0 PSU in drift and profile, surface temperature ≤ −9.99 and salinity ≤ 0, and the wave spectral fill. A pressure of -9.999 or a vacuum of 655.35 therefore appears as a number. `xmtps-aggregate-json.py` removes these (section 4.2).

### 3.17 Older files with the same `json_vers`

On 2026-10-30, 33,721 of the 55,003 production per-dive files (61 %) were written before 2026-10-24 and lack the attributes added since, although they say `"json_vers":"0.11"`. Example: `11145/dives/11145_0424.json`. Differences:

| Class | Missing in older files |
|---|---|
| `Profile` | `saln_corr` |
| `Surface` | `pres_correction`, `pres_correction_valid`, `pres_correction_num_samples`, `pres_correction_num_dives` |
| `MissionParameters` | `cfgver`, `entries` (`parameters` is present, usually empty) |
| `Engineering` | `ice`, `bit`, `errors`, `extra`; each of `ascent`, `descent`, `drift`, `surface` has only `dt_start`, `vac0`, `vac1` |
| `Drift` | measured sample times (all times are estimated) |

All 16,654 production files with a `wave` member have `declination` and `direction_ref`. `xmtps-list2json.py` rewrites a file only when its listing is newer (or with `-f`), so older files stay until the listing changes or the float is reprocessed with `-f`.

## 4. Aggregate files: `<serial>_<name>.json`

### 4.1 Encoding

Each file is one pandas DataFrame written with `DataFrame.to_json(path, orient='split', index=False)`:

```
{"columns":["dive","pres","temp","tcnd","saln","saln_corr","dens","sigma0"],
 "data":[[0,262.6,19.405,19.445,36.426,null,27.127,26.022], ...]}
```

- The object has exactly two members, `columns` (list of names) and `data` (list of rows, each a list in column order).
- Missing values are `null`. The files are strict JSON.
- Floats are written with at most 10 decimals (pandas `double_precision` default), for example `-13.4777720084`.
- A float column that holds whole numbers is still written with `.0` (for example `dt_epoch` `1755544649.0` in `_ascent`). An int column is written without it.
- There is no version member.

### 4.2 Common rules

- **Dives included.** Unless a file says otherwise, rows come from dives ≥ 0 only (pre-mission dives are skipped).
- **Row order.** Rows follow the order in which the dive files were read (section 2.3: −1, −2, …, 0, 1, 2, …) unless the file is sorted.
- **Separator rows.** `_ascent`, `_descent`, `_drift` and `_surface` end each dive with one separator row, so line plots break between dives. In it `dive` is set, `phase_code` is -1, all numbers are `null`, and `dt_str` is the string `"nan"`.
- **Fill values.** Before any file is written, `RemoveFillValues` replaces listing fill values with `null` in numeric columns. A rule is (file, column regular expression, fill values); the first rule whose column pattern matches applies, and a cell matches within 1e-6. String columns, `_mission` and files without rules are not filtered.

| Files | Columns (full match) | Fill values |
|---|---|---|
| `ascent`, `descent`, `drift`, `profile`, `pump`, `surface` | `pres`, `temp`, `saln`, `tcnd`, `saln_corr`, `pres_corr` | -9.999 |
| `ascent`, `descent` | `rate` | -9.99 |
| `pump` | `vac` | -9.99, 655.35 |
| `pump` | `time` | 65535 |
| `engineering` | `*_vac0`, `*_vac1` | -9.99, 655.35 |
| `engineering_data` | `*.vac0`, `*.vac1`, `*_inhg` | -9.99, 655.35 |
| `engineering_data` | `*_dbar`, `*_degc`, `*_psu`, `*.test_temp` | -9.999 |
| `engineering_data` | `*_counts` | -99999 |
| `engineering_data` | every other column except `dive` | 65535, 6553.5, 655.35, 65.535, 4294967295, -32768, -99999, -9.999, -9.99 |
| `gps` | `snr_min`, `snr_avg`, `snr_max` | 255 |
| `gps`, `sbd` | `cep_km` | 999999 |
| `wave_spectra` | `freq` | -99.0 |
| `wave_spectra` | `*_g` | -1000.0, -999.99999 |
| `wave_spectra` | `Szz`, `Syy`, `Sxx`, `Cxy`, `Qzx`, `Qzy` | -9.99, -9.999 |

### 4.3 Which files exist

| File | Written when | Rows |
|---|---|---|
| `_ascent`, `_descent`, `_drift`, `_profile`, `_pump`, `_sbd`, `_surface`, `_gps`, `_engineering`, `_profile_position`, `_iridium_position`, `_displacement_submerged`, `_displacement_surface`, `_cycle_timing` | always (possibly with no rows) | see each section |
| `_mission` | some dive has `mission.fwver` or `mission.parameters` | dive with mission data |
| `_engineering_data` | always when the float has a dive | dive |
| `_engineering_errors` | some dive has an `errors` entry | error line |
| `_wave`, `_wave_spectra` | some dive ≥ 0 has a `Wave` with `slen` ≥ 1 (both are left out if either is empty) | dive, frequency bin |

An existing file that is no longer produced (for example `_wave` after the last wave dive is removed) is not deleted.

Production on 2026-10-30: float 9303 (WIMS) has all 19 table files; float 11186 (ALTO) has the same set without `_wave` and `_wave_spectra`.

### 4.4 `_ascent`, `_descent`

One row per sample of `Dive.ascent` / `Dive.descent` (including the inserted midpoints, section 3.6), then a separator row per dive.

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | Dive number. |
| `pres` | float | dBar | `pres`. |
| `dt_epoch` | float | s | Sample time. |
| `dt_ref_delta_sec` | float | s | Seconds after the dive's first sample. |
| `dt_str` | string | | Sample time; `"nan"` in separator rows. |
| `phase_code` | int | | Phase code (midpoints 10/12/13/-1; separator -1). |
| `rate` | float | dBar/s × 100 | The per-dive `rate` multiplied by 100 (numerically cm/s for 1 dBar ≈ 1 m [inferred]). Positive when rising. |

### 4.5 `_drift`

One row per drift sample of dives with at least 3 samples, then a separator row.

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | |
| `pres`, `temp`, `saln` | float | dBar, °C, PSU | |
| `dens`, `sigma0` | float | kg/m³ − 1000 | |
| `dt_epoch` | float | s | Measured or estimated sample time (section 3.7); `null` if none. |
| `dt_ref_delta_sec` | float | s | |
| `dt_str` | string | | `"nan"` if no time and in separator rows. |

### 4.6 `_profile`

One row per profile bin; no separator rows. Dives are skipped when the float did not dive ("ghost" profile): the maximum ascent pressure or the maximum descent pressure is below 3 dBar.

| Column | Type | Units |
|---|---|---|
| `dive` | int | |
| `pres` | float | dBar |
| `temp` | float | °C |
| `tcnd` | float | °C |
| `saln` | float | PSU |
| `saln_corr` | float | PSU; `null` when not sent or for older dive files |
| `dens` | float | kg/m³ − 1000 |
| `sigma0` | float | kg/m³ − 1000 |

### 4.7 `_surface`

One row per surface sample, then a separator row.

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | |
| `dt_epoch` | float | s | |
| `dt_str` | string | | |
| `pres` | float | dBar | |
| `temp` | float | °C | |
| `pres_corr` | float | dBar | The dive's `Surface.pres_correction` if `pres_correction_valid`, else `null`. Repeated on every row of the dive. |
| `dt_ref_delta_sec` | float | s | |

`dt_ref_delta_sec` is the last column: it is not in the initial empty frame, so pandas appends it. Surface salinity is not aggregated.

### 4.8 `_pump`

One row per pump run. Columns `dive`, `pres` (dBar), `time` (s), `volt` (V), `curr` (A), `curr_adj` (A), `vac` (inHg, `vac1`), `power` (W), `energy` (J), all from `Dive.pump` (section 3.10). All floats except `dive`.

### 4.9 `_gps`

One row per valid GPS fix (`Position.is_valid`) of every dive, including negative dives. Sorted by `dt_epoch`.

| Column | Type | Units |
|---|---|---|
| `dive` | int | |
| `dt_epoch` | int | s |
| `dt_str` | string | |
| `lat`, `lon` | float | deg |
| `cep_km` | float | km (section 3.11) |
| `idx` | int | GPS message ID |
| `ttf_sec` | int | s |
| `nsat` | int | |
| `snr_min`, `snr_avg`, `snr_max` | int (`null` for 255) | |
| `hdop` | float | |

### 4.10 `_sbd`

One row per SBD message of every dive, including negative dives and messages with invalid positions. Not sorted.

| Column | Type | Units |
|---|---|---|
| `dive` | int | |
| `dt_epoch` | int | s |
| `dt_str` | string | |
| `lat`, `lon` | float | deg |
| `cep_km` | float | km (`null` for 999999) |
| `size_b` | float | bytes |
| `momsn`, `mtmsn`, `pid` | int | |

### 4.11 `_engineering` and `_engineering_data`

`_engineering`: one row per dive, all dives including negative ones, in read order.

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | |
| `<phase>_dt_epoch` | int | s | `engineering.<phase>.dt_start`; -1 if unset. |
| `<phase>_dt_str` | string | | `""` if unset. |
| `<phase>_vac0`, `<phase>_vac1` | float | inHg | Case vacuum at start and end of the phase; `null` if not reported. |

`<phase>` is `ascent`, `descent`, `drift`, `surface`, in that column order.

`_engineering_data`: one row per dive (all dives including negative ones, in read order) with every engineering value:

- Column `dive`, then one column `<section>.<attribute>` for each attribute of each section object, sections in the order `descent`, `drift`, `ascent`, `surface`, `ice`, `bit`, attributes in class order (sections 3.14.3–3.14.8). Then `<section>.<column>` for each `extra` entry.
- A `DateTime` attribute becomes its `dt_str` string. Unset times and empty strings become `null`.
- Columns that are `null` in every row are dropped, so the column set depends on the float. Columns can still be all `null` after the fill-value rules run, because fill removal happens after the drop.
- Numbers that the listing gave as integers become floats when the column also has `null` (for example `"surface.num_sbd": 12.0`).
- Hex words and status strings (`*.version`, `bit.results`, `bit.*_errors`, `bit.ctd_status`, `bit.sw_version`) stay strings.

Production floats 9303 and 11186 both have the same 112 columns: `dive`; 9 `descent.*` (`dt_start`, `vac0`, `vac1`, `version`, `seeks`, `seek_pump_adj_s`, `oil_valve_open_pos_surf_v`, `oil_valve_close_pos_fall_pump_v`, `air_valve_mid_pos_surf_v`); 12 `drift.*` (`dt_start` to `mean_saln_counts`); 5 `ascent.*` (`dt_start`, `vac0`, `vac1`, `version`, `fall_start_pres_dbar`); 38 `surface.*` (from `dt_start` to `num_errors`, without `ctd_status`, `case_vac_inhg`, `system_voltage_v`, `system_current_a`); and 47 `bit.*` (the version 9–11 attributes). Older firmware adds the version 0x10 columns; floats with ice messages add `ice.*`.

`xmtps-generate-startup-page.py` reads the rows whose `bit.version` or `bit.results` is set (not `null`, `NaN`, `""` or an unset value 255, 65535, 4294967295, 655.35, 65.535, 6553.5, 4294967.295, −327.68).

### 4.12 `_engineering_errors`

One row per `errors` entry (section 3.14.9) of every dive, in read order. Columns: `dive` (int), then the entry keys in first-seen order, normally `section` (string), `idx` (int), `dt_str` (string; `null` for surface 0x10 errors), `code` (string).

### 4.13 `_mission`

One row per dive (including negative dives) whose `mission.fwver` or `mission.parameters` is not empty, in read order.

| Column | Type | Contents |
|---|---|---|
| `dive` | int | |
| `fwver` | string | Firmware word, e.g. `"0x0A0C0001"`. |
| `cfgver` | string | Configuration message version, e.g. `"0x18"`; `""` for older files. |
| `ctdtype` | string | `"RBR"`, `"SBE"` or `""`. |
| `<name>` | float (or string) | One column per mission parameter name, in first-seen order, from `mission.parameters`. `null` where the dive did not report it. |

This file is not fill-filtered: unset parameters keep their raw values (255, 65535, 4294967295, -32768, 655.35, 65.535, ...). Readers must check for them; `xmtps-generate-mission-page.py` uses its `UNSET_VALUES`. The parameter set depends on the firmware and float type: 9303 (WIMS, 10.10) has 190 columns, and 11186 (ALTO, 10.12) has the same set without the wave parameter `WST` (189). The mission page takes the last row with any non-null parameter as the float's current configuration.

Example (float 11186, trimmed):

```
{"columns":["dive","fwver","cfgver","ctdtype","MSN","APT","BTM","MDS",...],
 "data":[[-1,"0x0A0C0001","0x18","RBR",-32768.0,255.0,255.0,65535.0,...], ...,
         [113,"0x0A0C0001","0x18","RBR",113.0,1.0,1.0,2100.0,...]]}
```

### 4.14 `_profile_position`

One row per dive that has a position or a surface time, sorted by `dt_epoch`. Built in three passes (`AggregateDiveData`):

1. **GPS, same dive.** The earliest valid fix of the dive with `idx` 0 or 2 (end of dive). `src` = `"gps"`.
2. **GPS, next dive.** Otherwise the earliest valid fix of dive + 1 with `idx` 1 (start of the next dive, at the same surfacing). `src` = `"gps"`.
3. **No position.** Otherwise, if `engineering.surface.dt_start` is set, a row with that time and `null` position. `src` = `"none"`. A dive with neither gets no row.

Then the Iridium composites (4.15) fill rows without a position: each composite is assigned to the row nearest in time within 3600 s (`CPOS_SPAN_SECONDS`). If the composite is valid and its `cep_km` ≤ 2.0 (`CPOS_MAX_CEP_KM`), every row of that dive with a `null` `lat` or `lon` gets the composite's time, position and CEP, and `src` = `"iridium"`. The first composite (in creation order) that qualifies wins.

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | |
| `dt_epoch` | int | s | Fix time, composite time, or surface start time. |
| `dt_str` | string | | |
| `lat`, `lon` | float | deg | `null` for `src` `"none"`. |
| `cep_km` | float | km | `null` for `"none"`. |
| `src` | string | | `"gps"`, `"iridium"` or `"none"`. |

All dives are considered, but negative dives usually get no row (their fixes have `idx` 5 and they have no surface engineering time); production 9303 has 484 rows for dives 0–483.

### 4.15 `_iridium_position`

One row per composite Iridium position, sorted by `dt_epoch`. A composite combines the SBD positions of one transmission session; it is not one per dive.

Grouping: the valid SBD messages (`Position.is_valid`, so `cep_km` > 0) of all dives, including negative ones, are taken in read order. A message joins an existing composite when its time is strictly within ±1800 s (`CPOS_SPAN_SECONDS` / 2) of the midpoint of that composite's earliest and latest message time; otherwise it starts a new composite.

Combination (`FloatClasses.CompositePosition.compute`), over the n valid positions with CEPs cᵢ (km) and weights wᵢ = 1/cᵢ²:

- position: the weighted mean of the unit vectors (x, y, z) = (cos lon cos lat, sin lon cos lat, sin lat), converted back to latitude and longitude and rounded to 4 decimals (correct across the dateline);
- time: Σ wᵢ tᵢ / Σ wᵢ, rounded to whole seconds;
- CEP: `cep_km = sqrt(1 / Σ(1/cᵢ²) + 1.55²)`, rounded to 3 decimals. The first term is the CEP of independent estimates. The second (`BIAS_CEP_KM` = 1.55 km) is a GPS-calibrated error term. It was fitted on 2026-10-27 against interpolated GPS fixes (40,518 composites, 234 floats), so that the error is at most `cep_km` in 50 % of composites (comment in `FloatClasses.py`).

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | float | | Dive of the nearest profile position within 3600 s; `null` if none (for example startup sessions). Written as `113.0`. |
| `dt_epoch`, `dt_str` | int, string | s | Weighted mean time. |
| `dt_min_epoch`, `dt_min_str` | int, string | s | Earliest message time. |
| `dt_max_epoch`, `dt_max_str` | int, string | s | Latest message time. |
| `lat`, `lon` | float | deg | |
| `cep_km` | float | km | As above; at least 1.55. |
| `num_pos` | int | | Number of valid messages used. |

Production 9303: 495 rows for 484 deployed dives plus startup sessions.

### 4.16 `_displacement_submerged`, `_displacement_surface`

One row per dive number from 1 to the highest dive number, including numbers with no dive file. For each dive N, using the dive's GPS fixes as stored (including invalid ones):

- gps1 = the last fix of dive N with `idx` 1 (start of dive N);
- gps2 = the last fix of dive N with `idx` 2 (end of dive N);
- gps3 = the first fix of dive N + 1 with `idx` 1 (start of dive N + 1).

`_displacement_submerged` is the vector gps1 → gps2 if both exist and gps2 is later. `_displacement_surface` is gps2 → gps3 if both exist and gps3 is later.

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | |
| `azi` | float | deg | Initial azimuth at pos1, clockwise from north, [0, 360), 1 decimal (WGS84 geodesic, geographiclib). |
| `dist_m` | float | m | Geodesic distance, rounded to 1 m. |
| `duration_s` | float | s | pos2 time − pos1 time. |
| `speed_mps` | float | m/s | `dist_m / duration_s`, 4 decimals. |
| `pos1_dt_epoch`, `pos1_dt_str` | int, string | | Time of pos1; -1 and `""` if none. |
| `pos1_lat`, `pos1_lon` | float | deg | |
| `pos2_dt_epoch`, `pos2_dt_str`, `pos2_lat`, `pos2_lon` | | | Same for pos2. |

The four results are `null` when either fix is invalid. In that case the `pos1_*`/`pos2_*` columns still hold the invalid fix as stored, for example `-1, "", 99.99999, 999.99999` (production 9303, dive 1). When a fix is missing, the row has -1, `""` and `null`.

### 4.17 `_cycle_timing`

One row per dive (including negative dives), sorted by dive. Phase start times come from `_engineering` (`engineering.<phase>.dt_start`).

| Column | Type | Units | Contents |
|---|---|---|---|
| `dive` | int | | |
| `descent_dt_epoch`, `descent_dt_str` | int, string | s | Start of descent; -1, `""` if unset. |
| `descent_duration_s` | float | s | drift start − descent start, if 0 < d < 864000 (10 days); else `null`. |
| `drift_dt_epoch`, `drift_dt_str` | | | Start of drift. |
| `drift_duration_s` | float | s | ascent start − drift start, if 0 < d < 3456000 (40 days). |
| `ascent_dt_epoch`, `ascent_dt_str` | | | Start of ascent. |
| `ascent_duration_s` | float | s | surface start − ascent start, if 0 < d < 864000. |
| `surface_dt_epoch`, `surface_dt_str` | | | Start of the surface phase. |
| `surface_duration_s` | float | s | next dive's descent start − surface start, if 0 < d < 864000. `null` for the latest dive. |
| `total_duration_s` | float | s | Sum of the durations that are not `null`; 0.0 when all are. |

Column order in the file: `dive`, then `ascent_*`, `descent_*`, `drift_*`, `surface_*` (each `dt_epoch`, `dt_str`, `duration_s`), then `total_duration_s`.

### 4.18 `_wave`

One row per dive ≥ 0 with a `Wave` of `slen` ≥ 1, in read order. Values from `Dive.wave` (section 3.15).

| Column | Type | Units |
|---|---|---|
| `dive` | int | |
| `mwd` | int | deg, toward, [0, 360); 0 when rejected |
| `declination` | float | deg, east positive; `null` if not applied |
| `direction_ref` | string | `"true"` or `"magnetic"` |
| `period_peak`, `period_avg`, `period_zero` | float | s |
| `swh` | float | m |

### 4.19 `_wave_spectra`

One row per frequency bin of each `_wave` dive (245 per dive), sorted by `dive` then `freq`. Columns: `dive` (int); `freq` (Hz); `Szz_g`, `Syy_g`, `Sxx_g`, `Cxy_g`, `Qzx_g`, `Qzy_g` (interpolated acceleration spectra, g²/Hz); `Szz`, `Syy`, `Sxx`, `Cxy`, `Qzx`, `Qzy` (displacement spectra, m²/Hz); `a0`, `a1`, `a2`, `b1`, `b2`, `r1`, `r2` (float); `theta1`, `theta2` (int, deg). The `raw_*` spectra, `wavenumber` and `moment` are not aggregated.

## 5. Float summary: `<serial>_summary.json`

### 5.1 Encoding

`jsonpickle.encode(summary, unpicklable=True, keys=True, indent=2, separators=(',', ':'))`. The top level is a plain JSON object (a Python dict, so no `py/object`). It contains jsonpickle objects for `id` and the `Gps`/`Sbd` positions.

When the same `Gps` or `Sbd` object fills two slots (for example the deployment SBD message is also the latest one), the second occurrence is written as a reference `{"py/id": n}`. Six production summaries had one on 2026-10-30 (for example `10008_summary.json`, where `latest.sbd` is `{"py/id":7}` and is the same object as `deployment.sbd`). As observed with jsonpickle 4.1.4, n counts the JSON objects (class objects and nested dicts) in document order, starting at 1 with the first object inside the top-level dict [inferred]. Readers that do not use jsonpickle must resolve these references.

### 5.2 Fields

| Field | Type | Units | Contents |
|---|---|---|---|
| `json_vers` | string | | `"0.11"`. |
| `id` | `FloatId` | | Serial; DAC and WMO IDs from the last dive file read. |
| `age` | int | s | Latest SBD time − deployment SBD time, or − deployment GPS time if there is no deployment SBD. -999 if neither, or if there is no latest SBD. |
| `fwver` | string | | `mission.fwver` of the last dive (in read order) that has one; `""` if none. |
| `ctdtype` | string | | Likewise for `mission.ctdtype`. |
| `profdepthsum` | float | dBar | Sum over dives ≥ 0 that are in `_profile` of the dive's maximum profile pressure, 1 decimal. 0.0 if none. The pages treat a float with `profdepthsum` > 0 and `latest.cycle` ≥ 1 as deployed. |
| `type` | string | | `"ALTO"` if the serial is above 10000, else `"ALAMO"`. |
| `startup` | object | | `gps`, `sbd` at startup (below). |
| `deployment` | object | | `gps`, `sbd` at deployment. |
| `latest` | object | | `gps`, `sbd`, `cycle`, `duration`, `nsbd`, `depth`. |

Position selection (over valid GPS fixes; over all SBD messages):

| Slot | `gps` | `sbd` |
|---|---|---|
| `startup` | The latest fix of any negative dive. | The latest message of any negative dive. |
| `deployment` | The earliest fix of a dive ≥ 0 that is later than the startup fix. | The earliest message of a dive ≥ 0 that is more than 3600 s after the startup message. |
| `latest` | The latest fix of a dive ≥ 0. | The latest message of a dive ≥ 0. |

Each slot holds a complete `Gps` (section 3.11) or `Sbd` (section 3.12) object. An unfilled slot is a default object: times -1 and `""`, `lat`/`lon`/`cep_km` `NaN`, integers -1, `hdop` -9.0.

`latest` scalars:

| Field | Type | Units | Contents |
|---|---|---|---|
| `cycle` | int | | The highest dive number of the float (`dives[-1]`); can be negative before deployment. |
| `duration` | int | s | Total duration of the latest dive (`_cycle_timing.total_duration_s`) plus the most recent valid surface duration of an earlier dive. Set only if the latest dive has valid descent and ascent durations; else -999. |
| `nsbd` | int | | Number of SBD messages of the latest dive (0 if none). -999 only if aggregating that dive's SBD messages failed. |
| `depth` | float | dBar | Maximum pressure of the latest dive over ascent, descent, drift and profile, 1 decimal. -999.0 if none. Fill values are not removed first (`RemoveFillValues` runs after this). |

Example (float 9303, trimmed):

```
{
  "json_vers":"0.11",
  "id":{"py/object":"FloatClasses.FloatId","whoi":9303,"dac":-1,"wmo":-1},
  "age":36867924,
  "fwver":"0x0A0A0031",
  "ctdtype":"RBR",
  "profdepthsum":139163.4,
  "type":"ALAMO",
  "startup":{"gps":{"py/object":"FloatClasses.Gps","dt_epoch":1755527580,...},"sbd":{...}},
  "deployment":{"gps":{...},"sbd":{...}},
  "latest":{"gps":{...},"sbd":{...},"cycle":483,"duration":108051,"nsbd":12,"depth":291.9}
}
```

## 6. `dives/.lastprocessed`

- Location: in each directory that `xmtps-aggregate-json.py` processes, in production `<serial>/dives/.lastprocessed`.
- Content: one number, no newline, the Python `str()` of a float: the newest modification time (Unix seconds) of the JSON files read from that directory in the last run. Example: `1793249363.0885403`.
- Written by `WriteLastProcessedFiles` after all aggregate, summary and plot files of the run have been written. A run that raises before then (for example `-H`, or a failure writing the aggregate files) writes none, so the directory is processed again next time.
- Read by `ProcessDirectory`: if the file exists, the directory has at least one included file, and `-f` is not given, the directory is skipped unless some included file is newer than the stored time. An unreadable file is logged and the directory processed. If no file was read, nothing is written.

## 7. Reading the files

### 7.1 Python

Per-dive files, as the tools do (needs `bin/py` on `sys.path` so that `FloatClasses` can be imported):

```python
import jsonpickle, sys
sys.path.insert(0, '/data1/alamo/bin/py')
dive = jsonpickle.decode(open('9303_0483.json').read(), keys=True)
if dive.json_vers != '0.11':
    raise ValueError('unsupported version')
saln_corr = getattr(dive.profile, 'saln_corr', None)   # absent in older 0.11 files
```

Without `FloatClasses`, the plain `json` module reads the same file into nested dicts (it accepts `NaN`). The `py/object` members then stay as strings.

```python
import json
d = json.load(open('9303_0483.json'))
pres = d['profile']['pres']
```

Aggregate files:

```python
import pandas as pd
df = pd.read_json('9303_profile.json', orient='split')
```

or, as the page generators do, `json.load` and `dict(zip(j['columns'], row))` per row.

Summary files: `jsonpickle.decode(text, keys=True)`, which also resolves `py/id` references. `json.load` works too, but leaves `py/id` references unresolved.

### 7.2 Other languages

| File | Issue | What to do |
|---|---|---|
| Per-dive | Bare `NaN` tokens (not valid JSON). | Use a parser that accepts them, or replace the tokens before parsing: `NaN` never appears inside a string in these files, so a regular expression such as `(?<=[\[,:])NaN(?=[,\]}])` → `null` is enough. Handle `Infinity`/`-Infinity` the same way to be safe. |
| Per-dive | `py/object` members. | Ignore them, or use them to tell classes apart. |
| Per-dive | `py/reduce` numpy encodings (failed wave post-processing). | Treat that `wave` member as unusable. |
| Per-dive | Missing members in older 0.11 files (section 3.17). | Treat every member added in section 3.17 as optional. |
| Aggregate | None: strict JSON. | Map `columns` to the row arrays. Treat `"nan"` in `dt_str` as missing. |
| Summary | `py/id` references. | Keep a list of the JSON objects in document order (section 5.1) and replace `{"py/id": n}` with the n-th; or compare the times of the slots. |
| All | Unset times are -1 and `""`, not null. | Check `dt_epoch > 0`. |

JavaScript `JSON.parse` rejects the per-dive files without the replacement above; it reads the aggregate files and summaries directly.

## 8. Open questions / discrepancies

Code, README and production were compared on 2026-10-30.

1. **`json_vers` not bumped.** Attributes were added to `Profile`, `Surface`, `MissionParameters` and `Engineering` (2026-10-24) and to `Wave` (2026-10-26) without changing `"0.11"`. 61 % of the production per-dive files are in the older layout (section 3.17). The README says all scripts ignore files with any other version, which is true but does not help here.
2. **README, `_iridium_position`: "one row per dive".** The file has one row per composite (transmission session). Startup sessions have `dive` `null`, and a dive can have more than one row (9303: 495 rows for 484 dives). `dive` is written as a float.
3. **README, `_surface` column order.** The README lists `dt_ref_delta_sec` before `pres_corr`; the files have it last (section 4.7).
4. **README does not mention the units of the aggregate `rate`.** The aggregate multiplies the per-dive dBar/s value by 100. The `(-9.99,)` fill rule for `rate` in `FILL_VALUES` cannot match a scaled computed rate and appears to be left over.
5. **README, summary: "Unset numbers are -999".** Only `age`, `latest.cycle`/`duration`/`nsbd`/`depth` use -999. Unset `Gps`/`Sbd` slots use -1, -9.0 and `NaN`.
6. **Summary `py/id` references** (section 5.1) are not mentioned in the README. They break readers that expect a full object in every slot.
7. **Displacement tables keep invalid fixes.** When gps1/gps2/gps3 exist but are invalid, the vector is `null` but `pos1_*`/`pos2_*` hold the stored values, including the decoder's 99.99999/999.99999 and time -1 (9303 dive 1). `FILL_VALUES` has no rule for these files, and `RemoveFillValues` runs before they are built.
8. **Composite grouping can add a message to several composites.** In `AggregateDiveData` the loop over existing composites uses `continue` where `break` was probably meant, so a message whose time falls within ±1800 s of two composites' midpoints is added to both. Effect not measured.
9. **Failed wave post-processing.** `9237/dives/9237_0000.json` has a `WVS` line with `slen` 1. `Wave.PostProcess()` raises, the arrays are pickled as numpy `py/reduce` objects, and the drift sample times are not set because `Dive.PostProcess()` stops at the wave step. The file is still written.
10. **A listing without a `LIS` line** produces `-1/dives/-001_-999.json` (present in production). `update-plots-service.sh` does not aggregate it, because its include pattern requires a numeric float directory.
11. **`Drift.dt_ref` type varies.** It is a `DateTime`, except when `n` is 0 and the dive has ascent and descent samples, in which case `EstimateDatetimes` sets it to `[]`. A `PresTimeSeries` with exactly one sample keeps a `dt` member and empty `rate`/`dt_epoch`/`dt_str`.
12. **Docstrings.** `FloatClasses.Drift` and `FloatClasses.Profile` describe `dens` as "in-situ density in kg/m^3". The value is density minus 1000 (as `RhoLUT.py` and the README say). In `_cycle_timing` the code comments the 864000 s surface limit as "1 day"; it is 10 days.
13. **`example/json/` is stale.** `example/json/dives/9303_0440.json` (2026-10-26) has no `wave.declination` or `wave.direction_ref`, and its `raw_freq` starts at 0.02 Hz instead of 0.0. Production files written since 2026-10-28 have both members and start at 0.0.
14. **Dead code in `xmtps-aggregate-json.py`.** The initial empty `engineering` frame lists `'ascent_vac1' 'descent_dt_epoch'` without a comma (three such pairs), and `float_aggregate_data['profile'].sort_values('dive', ignore_index=True)` discards its result. Neither affects the output: the frame is replaced, and the profile rows are already in dive order.
15. **`xmtps-list2json.py`, `WriteJsonDiveDataFile`.** The error message for a failed write uses `path`, which is not a local variable. It works only because the main loop defines a global `path`.
16. **`profile_position` for dives without engineering.** A dive with no valid GPS fix and no `engineering.surface.dt_start` gets no row, so no Iridium composite can be assigned to it. This may be intended (there is no time to match), but it means the position table can skip dives that have SBD positions.
17. **Mission page "current configuration".** `xmtps-generate-mission-page.py` takes the last row of `_mission` with any parameter. Rows are in read order, so for a float with only negative dives the "latest" row is the most negative dive, not −1. The page lists only deployed floats, so this does not show today.
