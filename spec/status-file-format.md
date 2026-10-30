# Status File Format Specification

A status file is a small key/value text file that summarizes one float as the XMTPS decoder knows it: identifiers, deployment metadata, the latest GPS and Iridium positions, the latest SBD message numbers and the X-message version. The post-processing thread (stage 4) writes one per float. No program in this repository or in the production tool set on argus reads them; in particular, the decoder status, uptime and version in the web page footers do not come from these files.

Status: compiled on 2026-10-30 from the decoder source and the production files on argus (`/data1/alamo/data/stat`).

## Contents

- [1. Configuration](#1-configuration)
- [2. Naming and location](#2-naming-and-location)
- [3. When the file is written](#3-when-the-file-is-written)
- [4. Line format](#4-line-format)
- [5. Fields](#5-fields)
- [6. Fill values](#6-fill-values)
- [7. Examples](#7-examples)
- [8. Readers](#8-readers)
- [9. Production state](#9-production-state)
- [10. Open questions / discrepancies](#10-open-questions--discrepancies)

## 1. Configuration

The options are parsed in `src/Main.cpp`; see also [configuration-file-format.md](configuration-file-format.md).

| Option | Default | Effect |
|---|---|---|
| `dir.stat` | `stat` | Directory of the status files (`XFloat::status_directory_path_`). Created if missing. |
| `gen.stat` | `0` | Write status files (`XFloat::status_file_generation_is_enabled_`). Off by default; production sets it on. |

There is no per-float switch.

## 2. Naming and location

```
<dir.stat>/stat<serial>.txt
```

`<serial>` is the WHOI instrument serial number, zero-padded to four digits (`/stat%04d.txt`), for example `stat9281.txt` or `stat11191.txt`. The directory is flat. Built in `XFloat::WriteStatusFile()` (`src/data/XFloat.cpp`).

## 3. When the file is written

The post-processing thread, `XFloatPostProcessingService::operator()()` (`src/stage4b_PostProcessingThread.h`), runs a pass about once a minute: it sleeps 60 seconds, then for every float in the decoder's float cache calls `WriteKmlFile()` ([kml-file-format.md](kml-file-format.md)) and then `WriteStatusFile()`. A failure for one float is logged and the pass goes on.

`WriteStatusFile()` writes only if `gen.stat` is on and the float's `status_file_updated_` flag is false; it then sets the flag. The flag is false:

- for every float when the decoder starts, so every cached float's status file is rewritten in the first pass after startup;
- after every SBD message decoded for the float. `ProcessDiveData()` (`src/stage3_TelemetryDecoderThread.h`) clears the flag whether or not the message completes a dive.

So a status file is rewritten within about a minute of each message from the float, at most once per pass. Production log, 2026-10-30:

```
2026-10-30T14:35:00.238845 post-process info Float[11191|...] ... Writing status file: "/data1/alamo/data/stat/stat11191.txt"...
2026-10-30T14:36:00.308401 post-process info Float[11191|...] ... Writing KML file: "/data1/alamo/data/kml/11191.kmz"...
2026-10-30T14:36:00.346598 post-process info Float[11191|...] ... Writing status file: "/data1/alamo/data/stat/stat11191.txt"...
```

The file is truncated and written in place with `std::ofstream`; it is not written to a temporary file and renamed. Nothing deletes status files.

## 4. Line format

Plain ASCII, LF line ends, one field per line, always the same 27 lines in the same order. Each line is the key left-justified in 39 characters, one space, and the value (`"%-39s %s"`, `src/data/XFloat.cpp` `XFloat::GenerateStatusString()`). The value starts in column 41. Values may contain spaces (for example `DEPLOYER`); a reader should take everything from column 41 to the end of the line, or split once on the first run of spaces. The last line ends with a newline.

Times are ISO 8601 extended format without a time zone designator (`2026-10-30T14:27:00`); they are UTC. Positions are ISO 6709 strings, latitude then longitude, each `%+.5f` (`+14.92970+116.66385`), from `GenerateIso6709String()` (`src/utility/PositionUtils.h`).

## 5. Fields

| # | Key | Value | Source |
|---|---|---|---|
| 1 | `ID_INST` | integer | WHOI instrument serial number. |
| 2 | `ID_DAC` | integer | DAC "internal ID number", from the `.meta` file ([float-metadata-format.md](float-metadata-format.md)) via the instrument table. |
| 3 | `ID_WMO` | integer | WMO ID, from the `.meta` file via the instrument table. |
| 4 | `ID_IMEI` | integer, 15 digits | Iridium modem IMEI. Updated from each new message's IMEI. |
| 5 | `PHY_GENERATION_ENABLED` | `0` or `1` | The per-float PHY flag ([phy-file-format.md](phy-file-format.md)). |
| 6 | `DEPLOYMENT_DATETIME` | ISO time | `.meta` `launch time`. |
| 7 | `DEPLOYMENT_DATETIME_STATUS` | text | `.meta` `status of launch time`, verbatim (`as recorded`, `approximated`, `estimated`, ...). |
| 8 | `DEPLOYMENT_POSITION` | ISO 6709 | `.meta` `launch position`. |
| 9 | `DEPLOYMENT_POSITION_STATUS` | text | `.meta` `status of launch position`, verbatim. |
| 10 | `DEPLOYMENT_CRUISE_ID` | text | `.meta` `deployment cruise id`. |
| 11 | `DEPLOYMENT_PLATFORM` | text | `.meta` `deployment platform` (ship or aircraft). |
| 12 | `DEPLOYMENT_TYPE` | text | `.meta` `deployment type` (`RV`, `Airborne`, `Air deployed`, `USCGC`, ...). |
| 13 | `DEPLOYER` | text | `.meta` `float deployer`. |
| 14 | `DEPLOYER_ADDRESS` | text | `.meta` `float deployer address`. |
| 15 | `LATEST_GPS_DIVE` | integer | Dive number of the latest GPS fix. |
| 16 | `LATEST_GPS_DATETIME` | ISO time | Time of the latest GPS fix. |
| 17 | `LATEST_GPS_POSITION` | ISO 6709 | Position of the latest GPS fix. |
| 18 | `LATEST_IRIDIUM_DIVE` | integer | Dive number of the SBD message with the latest Iridium position. |
| 19 | `LATEST_IRIDIUM_DATETIME` | ISO time | Time of that Iridium position (the gateway's session time). |
| 20 | `LATEST_IRIDIUM_POSITION` | ISO 6709 | That Iridium position. |
| 21 | `LATEST_SBD_MOMSN` | integer | MOMSN of that SBD message. |
| 22 | `LATEST_SBD_MTMSN` | integer | MTMSN of that SBD message. |
| 23 | `START_OF_MISS_DATETIME` | ISO time | Start of mission: the time of the latest built-in-test GPS fix (X-message 0x05). |
| 24 | `WMO_INSTRUMENT_TYPE` | integer | `.meta` `WMO instrument type` (866, 867, 868, 873, 875, 876). |
| 25 | `WMO_RECORDER_TYPE` | integer | `.meta` `WMO recorder type` (64). |
| 26 | `XMSG_VERSION_MAJOR` | integer | X-format major version, from the latest Argo message 0xF0 decoded for the float. |
| 27 | `XMSG_VERSION_MINOR` | integer | X-format minor version, from the same message. |

Fields 6 to 14, 24 and 25 come from the float's metadata object, which exists only if a `.meta` file with a usable DAC ID was loaded at startup. All other fields come from the float object, which is persisted in the instrument table ([database-schema.md](database-schema.md)).

How the "latest" fields are kept (`src/data/XFloat.cpp`):

- **GPS (15–17).** Updated when a decoded GPS fix of any type (0x00, 0x01, 0x02, 0x03 or 0x05) has a valid position and is later than the stored one. Built-in-test fixes count, so a float that has only done its startup test shows that fix with dive −1.
- **Iridium and SBD (18–22).** Updated, all five together, when an SBD message has a valid Iridium position whose time is later than the stored one. `LATEST_SBD_MOMSN` is therefore the MOMSN of the message with the latest Iridium position time, not necessarily the highest MOMSN received.
- **Start of mission (23).** Updated when a built-in-test GPS fix is later than the stored value.

## 6. Fill values

| Field | Fill | Meaning |
|---|---|---|
| `ID_DAC`, `ID_WMO` | `-1` | Not known (no `.meta` file, or never set). |
| `ID_IMEI` | `-1` | Not known. [inferred: not seen in production] |
| Any `*_DATETIME` | `not-a-date-time` | Unknown time (Boost `to_iso_extended_string()` of `not_a_date_time`). |
| `DEPLOYMENT_*` text fields, `DEPLOYER*`, `WMO_*_TYPE` | `unknown` | No metadata object. |
| `DEPLOYMENT_POSITION` | `+99.999999+999.999999` | No metadata object (literal string). |
| `DEPLOYMENT_POSITION` | `+99.99999+999.99999` or `+99.99999+99.99999` | Metadata present but the launch position could not be parsed (see section 10). |
| `DEPLOYMENT_DATETIME_STATUS`, `DEPLOYMENT_POSITION_STATUS` | empty | Metadata present but the field missing from the `.meta` file. [inferred] |
| `WMO_INSTRUMENT_TYPE`, `WMO_RECORDER_TYPE` | `-1` | Metadata present but the field missing. [inferred] |
| `LATEST_GPS_DIVE`, `LATEST_IRIDIUM_DIVE` | `-999` | No fix or position yet. |
| `LATEST_GPS_POSITION`, `LATEST_IRIDIUM_POSITION` | `+99.99999+99.99999` | No fix or position yet. |
| `LATEST_SBD_MOMSN`, `LATEST_SBD_MTMSN` | `-1` | Not known. |
| `XMSG_VERSION_MAJOR`, `XMSG_VERSION_MINOR` | `255` | No Argo message 0xF0 decoded yet. |

A `LATEST_GPS_DIVE` of `-1` is not a fill value: it is the dive number the float gives its built-in-test GPS fix. `LATEST_IRIDIUM_DIVE` `0` is a real dive 0.

## 7. Examples

A float with metadata (production `stat11014.txt`, IMEI masked):

```
ID_INST                                 11014
ID_DAC                                  7547
ID_WMO                                  4903038
ID_IMEI                                 XXXXXXXX4235120
PHY_GENERATION_ENABLED                  0
DEPLOYMENT_DATETIME                     2018-09-06T01:55:00
DEPLOYMENT_DATETIME_STATUS              as recorded
DEPLOYMENT_POSITION                     +73.19517-159.81635
DEPLOYMENT_POSITION_STATUS              as recorded
DEPLOYMENT_CRUISE_ID                    SKQ-2018-19
DEPLOYMENT_PLATFORM                     Sikuliaq
DEPLOYMENT_TYPE                         RV
DEPLOYER                                Peacock / Boury
DEPLOYER_ADDRESS                        MIT
LATEST_GPS_DIVE                         333
LATEST_GPS_DATETIME                     2018-10-04T10:40:00
LATEST_GPS_POSITION                     +76.14653-163.40539
LATEST_IRIDIUM_DIVE                     333
LATEST_IRIDIUM_DATETIME                 2018-10-04T11:13:36
LATEST_IRIDIUM_POSITION                 +76.17979-163.41572
LATEST_SBD_MOMSN                        4348
LATEST_SBD_MTMSN                        0
START_OF_MISS_DATETIME                  2018-08-30T00:09:00
WMO_INSTRUMENT_TYPE                     873
WMO_RECORDER_TYPE                       64
XMSG_VERSION_MAJOR                      2
XMSG_VERSION_MINOR                      2
```

An active float without a `.meta` file (production `stat11191.txt`, written 2026-10-30, IMEI masked):

```
ID_INST                                 11191
ID_DAC                                  -1
ID_WMO                                  -1
ID_IMEI                                 XXXXXXXX8847460
PHY_GENERATION_ENABLED                  0
DEPLOYMENT_DATETIME                     not-a-date-time
DEPLOYMENT_DATETIME_STATUS              unknown
DEPLOYMENT_POSITION                     +99.999999+999.999999
DEPLOYMENT_POSITION_STATUS              unknown
DEPLOYMENT_CRUISE_ID                    unknown
DEPLOYMENT_PLATFORM                     unknown
DEPLOYMENT_TYPE                         unknown
DEPLOYER                                unknown
DEPLOYER_ADDRESS                        unknown
LATEST_GPS_DIVE                         39
LATEST_GPS_DATETIME                     2026-10-30T14:27:00
LATEST_GPS_POSITION                     +14.92970+116.66385
LATEST_IRIDIUM_DIVE                     39
LATEST_IRIDIUM_DATETIME                 2026-10-30T14:35:17
LATEST_IRIDIUM_POSITION                 +14.89751+116.61496
LATEST_SBD_MOMSN                        702
LATEST_SBD_MTMSN                        0
START_OF_MISS_DATETIME                  2026-09-22T02:00:00
WMO_INSTRUMENT_TYPE                     unknown
WMO_RECORDER_TYPE                       unknown
XMSG_VERSION_MAJOR                      2
XMSG_VERSION_MINOR                      4
```

A float that only ran its built-in test (production `stat11057.txt`, excerpt):

```
LATEST_GPS_DIVE                         -1
LATEST_GPS_DATETIME                     2019-09-17T19:56:00
LATEST_GPS_POSITION                     +78.89865-141.79337
LATEST_IRIDIUM_DIVE                     0
LATEST_IRIDIUM_DATETIME                 2019-09-25T01:50:17
LATEST_IRIDIUM_POSITION                 +75.52672-144.16235
START_OF_MISS_DATETIME                  2019-09-17T19:56:00
```

## 8. Readers

No reader was found:

- Nothing in this repository reads `stat*.txt` (`src/`, `bin/py/`, the shell scripts in `bin/`).
- Nothing in `/data1/alamo/bin` or `/data1/alamo/matlab` on argus reads them.

The decoder status, uptime and version shown in the footer of the summary, mission and startup pages come from `bin/py/PageStatus.py`, which does not read any file written by XMTPS:

| Footer item | Source (`bin/py/PageStatus.py`) |
|---|---|
| Decoder status and uptime | `systemctl show xmtps-alamo --timestamp=unix --property=ActiveState,ActiveEnterTimestamp,StateChangeTimestamp` (`DecoderStatus()`). `active` gives the uptime since `ActiveEnterTimestamp`; another state gives the time since `StateChangeTimestamp`. |
| XMTPS version | `/data1/alamo/bin/xmtps --version`, parsed for `v<version> (built <date>` (`DecoderVersion()`). |
| Python utility version | The tools' JSON format version (`JSON_VERSION`). |

Any item that cannot be determined is shown as `unknown`. `/data1/alamo/bin/check-xmtps-status.sh` on argus also uses `systemctl status xmtps-alamo`, not the status files. See [plot-and-page-files.md](plot-and-page-files.md) and [log-and-lock-files.md](log-and-lock-files.md).

## 9. Production state

On argus (checked 2026-10-30), `/data1/alamo/data/stat` has 467 status files:

- 424 rewritten since the decoder restart on 2026-10-28 22:08 UTC, one per float in the float cache (the master KML file has the same 424 floats);
- 43 last written in 2020 (1) or 2021 (42), for floats no longer in the float cache [inferred];
- all 467 have `PHY_GENERATION_ENABLED 0`;
- 249 have no metadata (`unknown` deployment fields), and 235 have `ID_DAC -1`;
- 50 have `XMSG_VERSION_MAJOR 255` (no Argo message 0xF0 decoded).

## 10. Open questions / discrepancies

1. **Longitude fill equals the latitude fill (bug).** `XFLT_POS_INVALID_LON` (`src/data/XFloatDefinitions.h`) and `IRI_POS_INVALID_LON` (`src/data/IridiumDefinitions.h`) are both defined as the latitude fill (`UTIL_POS_INVALID_LAT`, 99.99999) instead of `UTIL_POS_INVALID_LON` (999.99999). Unknown positions therefore print as `+99.99999+99.99999`; 12 GPS and 28 Iridium production entries show it.
2. **Three different deployment position fills.** No metadata gives the literal `+99.999999+999.999999` (six decimals). A launch position that does not parse gives `+99.99999+999.99999` or `+99.99999+99.99999`, depending on the branch in `src/data/XFloatMetadata.cpp` (one uses `UTIL_POS_INVALID_LON`, the other `XFLT_POS_INVALID_LON`).
3. **Mixed fills for "unknown".** Missing metadata text prints `unknown`, missing numbers print `-1`, `-999` or `255`, and missing times `not-a-date-time`. There is no single fill convention.
4. **"Latest GPS" includes test and abort fixes.** `LATEST_GPS_*` follows any valid GPS fix, including built-in-test (dive −1) and abort fixes, unlike the PHY profile position and the KML surface positions.
5. **Option help text.** The `--gen.stat` help in `src/Main.cpp` says status files are written "to the mail message directory"; they are written to `dir.stat`.
6. **No readers.** The status files appear unused. The CHANGELOG records that status files were added in 2015-08 and later made to follow `gen.stat` instead of `gen.kml`, but no current consumer was found. [inferred: an external consumer on another host cannot be ruled out]
7. **Not a decoder status.** Despite the name, these are per-float summaries. XMTPS writes no file that reports its own running state; the run-time lock (PID) file is described in [log-and-lock-files.md](log-and-lock-files.md).
