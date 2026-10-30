# PHY Listing File Format Specification

A PHY file is a short text listing of one completed profile of one ALAMO or ALTO float: identifiers, metadata, one profile position and the profile's pressure, temperature and (if measured) salinity. The XMTPS telemetry decoder (stage 3) writes it when a dive is complete. It was the file uploaded to the Argo data center at AOML for GTS distribution; in production no float has PHY generation enabled any more, and the last PHY files were written on 2021-01-29.

Status: compiled on 2026-10-30 from the decoder source and the production files on argus (`/data1/alamo/data/phy`).

## Contents

- [1. Configuration](#1-configuration)
- [2. Naming and location](#2-naming-and-location)
- [3. When the file is written](#3-when-the-file-is-written)
- [4. File layout](#4-file-layout)
- [5. Header lines](#5-header-lines)
- [6. Position block](#6-position-block)
- [7. Profile data lines](#7-profile-data-lines)
- [8. Surface pressure offset correction (`poc.phy`)](#8-surface-pressure-offset-correction-pocphy)
- [9. Differences from the intermediate listing](#9-differences-from-the-intermediate-listing)
- [10. Readers](#10-readers)
- [11. Production state](#11-production-state)
- [12. Open questions / discrepancies](#12-open-questions--discrepancies)

## 1. Configuration

Three switches must all be on for a PHY file to be written. The options are parsed in `src/Main.cpp`; see also [configuration-file-format.md](configuration-file-format.md).

| Option | Kind | Default | Effect |
|---|---|---|---|
| `dir.phy` | config file `[dir]` | `phy` | Root directory of the PHY files (`XFloatDiveData::phy_directory_path_`). |
| `gen.phy` | config file `[gen]` | `1` | Global switch (`XFloatDiveData::phy_file_generation_is_enabled_`). |
| `poc.phy` | config file `[poc]` | `1` | Apply the surface pressure offset correction to PHY pressures (section 8). |
| `--phy-enable ID` | command line, repeatable | none | Turn the per-float flag on for instrument `ID`, and reprocess that instrument at startup. |
| `--phy-disable ID` | command line, repeatable | none | Turn the per-float flag off for instrument `ID`. |

The per-float flag is `XFloat::phy_file_generation_is_enabled_`. It is persisted in the `phy_file_generation_is_enabled` column of the instrument table (`sql/xmtps-schema.sql`, default `b'0'`; see [database-schema.md](database-schema.md)). A new float starts with the flag off, so PHY generation is opt-in per float even though `gen.phy` defaults to `1`.

`--phy-enable` and `--phy-disable` are applied once, at startup, by `XFloat::StartupTasks()` (`src/data/XFloat.cpp`). Each listed float's flag is set and the float is saved to the database, so the setting survives restarts. Enable is applied before disable, so an ID given to both ends up disabled. The IDs given to `--phy-enable` are also added to the startup reprocessing list (`src/stage1_MailRetrievalThread.cpp`), so their existing dives are rebuilt and their PHY files written.

The current per-float flag is visible as `PHY_GENERATION_ENABLED` in the float's status file ([status-file-format.md](status-file-format.md)).

## 2. Naming and location

```
<dir.phy>/<serial>/<dac>_<serial>_<dive>.phy
```

The path is built in `XFloatDiveData::WritePhyFile()` (`src/data/XFloat.cpp`):

| Part | Format | Source |
|---|---|---|
| `<serial>` directory | `%04d` | WHOI instrument serial number. Created if missing. |
| `<dac>` | `%04d` | DAC "internal ID number" from the float's `.meta` file. |
| `<serial>` | `%04d` | WHOI instrument serial number. |
| `<dive>` | `%04d` | Dive (cycle) number. |

Example: `/data1/alamo/data/phy/9273/6087_9273_0391.phy`.

The `uploaded/` subdirectory in each float directory is not written by XMTPS. It is created by the upload script (section 10).

## 3. When the file is written

The telemetry decoder thread calls `XFloatTelemetryDecoderService::ProcessDiveData()` (`src/stage3_TelemetryDecoderThread.h`) after every decoded SBD message. When `XFloatDiveData::CheckCompleteness()` reports the dive complete, it calls `WritePhyFile()`, then `WriteListingFile()`, then saves the dive to the database.

A dive is complete when all of these hold (`src/data/XFloatDiveData.cpp`):

- complete binned or unbinned (high-resolution) profile data;
- known sensor gains and offsets (the dive's Argo message 0xF0 has arrived);
- contiguous packet IDs starting at 0;
- a valid profile position (section 6).

`WritePhyFile()` then returns without writing unless all of these hold:

- `gen.phy` is on;
- the float's per-float flag is on;
- the float has metadata (a `.meta` file was loaded; see [float-metadata-format.md](float-metadata-format.md));
- the dive is complete.

The file is rewritten (truncated and written again) every time another message for an already complete dive arrives, and every time the dive is reprocessed. Nothing deletes a PHY file.

## 4. File layout

The file is plain ASCII with LF line ends. It has three parts: header lines, a position block, and profile data lines. Real example (production, float 9273, dive 391, trimmed):

```
INTERNAL ID NUMBER                      6087
WMO ID NUMBER                           4903307
PROFILE NUMBER                          391
INSTRUMENT TYPE                         ALAMOIR_TS1
WMO INSTRUMENT TYPE (TABLE 1770)        867
WMO RECORDER TYPE (TABLE 4770)          64
POSITIONING_SYSTEM                      GPS
PI                                      Elizabeth Sanabia
NUMBER OF COLUMNS                       3
1. COLUMN                               PRESSURE (DBAR)
2. COLUMN                               TEMPERATURE-90 (DEG C)
3. COLUMN                               SALINITY (PSU)
================================================================
   LATITUDE  LONGITUDE YEAR/MO/DY HR:MN:SC XMITS SAT POSITION-QC
  +24.73798  -55.94179 2020/10/19 16:32:00     8   4           0
================================================================
  250.26  19.008  36.714
  249.02  19.004  36.702
  246.90  19.024  36.704
...
    2.46  27.547  36.943
    1.78  27.545  36.943
```

A temperature-only float (WMO type 866) has two columns (production, float 9038, dive 156, trimmed):

```
NUMBER OF COLUMNS                       2
1. COLUMN                               PRESSURE (DBAR)
2. COLUMN                               TEMPERATURE-90 (DEG C)
================================================================
   LATITUDE  LONGITUDE YEAR/MO/DY HR:MN:SC XMITS SAT POSITION-QC
  +21.30465  -62.86169 2014/09/07 13:31:00    10  11           0
================================================================
  577.88  11.992
  575.00  11.938
...
    1.84  28.776
```

The last data line ends with a newline. There is no trailer.

## 5. Header lines

Each header line is a key left-justified in 39 characters, one space, and the value (`"%-39s %s"`, or `"%-39s %d"` for integers). The value therefore starts in column 41. This is the same layout as the `.meta` file. Built by `XFloatDiveData::GeneratePhyString()` (`src/data/XFloat.cpp`) with the `XMSG_PHY` macro (`src/data/xmsg/common/XDefinitions.h`).

| # | Key | Value | Source |
|---|---|---|---|
| 1 | `INTERNAL ID NUMBER` | integer | DAC ID (`id_dac`), from the `.meta` `internal ID number`. |
| 2 | `WMO ID NUMBER` | integer | WMO ID (`id_wmo`). |
| 3 | `PROFILE NUMBER` | integer | Dive number. Not zero-padded. |
| 4 | `INSTRUMENT TYPE` | string | `.meta` field `instrument type`, verbatim (e.g. `ALAMOIR_TS1`, `ALAMOIR_T1`). Empty if the `.meta` file lacks it. |
| 5 | `WMO INSTRUMENT TYPE (TABLE 1770)` | integer | `.meta` `WMO instrument type`. |
| 6 | `WMO RECORDER TYPE (TABLE 4770)` | integer | `.meta` `WMO recorder type` (64 for all production floats). |
| 7 | `POSITIONING_SYSTEM` | `GPS` | Constant. |
| 8 | `PI` | string | `.meta` `PI`, verbatim. |
| 9 | `NUMBER OF COLUMNS` | `2` or `3` | From the WMO instrument type, see below. |
| 10 | `1. COLUMN` | `PRESSURE (DBAR)` | Constant. |
| 11 | `2. COLUMN` | `TEMPERATURE-90 (DEG C)` | Constant. |
| 12 | `3. COLUMN` | `SALINITY (PSU)` | Only for 3-column files. |

The column count depends only on the WMO instrument type in the `.meta` file:

| WMO instrument type | Float | Columns |
|---|---|---|
| 866 | ALAMO, temperature only | 2 |
| 867 | ALAMO, RBR CTD | 3 |
| 868 | ALAMO, SBE CTD | 3 |
| 873 | ALTO, unspecified CTD | 3 |
| 875 | ALTO, RBR CTD | 3 |
| 876 | ALTO, SBE CTD | 3 |

Any other WMO instrument type, and any float whose Argo instrument type is not ALAMO (SOLO-II and Deep Argo branches are empty), throws an exception. The header column count does not control the data lines: those have three columns whenever the profile has salinity samples (section 7).

## 6. Position block

Built by `XGPS::GeneratePhyString()` (`src/data/xmsg/XGPS.h`). Four lines:

```
================================================================
   LATITUDE  LONGITUDE YEAR/MO/DY HR:MN:SC XMITS SAT POSITION-QC
  +24.73798  -55.94179 2020/10/19 16:32:00     8   4           0
================================================================
```

The rule lines are 64 `=` characters. The value line is formatted `" %+10.5f %+10.5f "`, then the date and time, then `"%5d %3d %11d"`:

| Column | Format | Units | Meaning |
|---|---|---|---|
| `LATITUDE` | `%+10.5f` | degrees north | GPS latitude. |
| `LONGITUDE` | `%+10.5f` | degrees east | GPS longitude. |
| `YEAR/MO/DY HR:MN:SC` | `%04d/%02d/%02d %02d:%02d:%02d` | UTC | GPS fix time. `9999/99/99 99:99:99` if unknown (cannot occur, see below). |
| `XMITS` | `%5d` | count | Number of SBD mail messages assigned to the dive. |
| `SAT` | `%3d` | count | Number of GPS satellites in the fix. |
| `POSITION-QC` | `%11d` | flag | `0` if the fix is valid, `9` otherwise. |

The fix is chosen by `XFloatDiveData::GetGpsProfilePosition()` (`src/data/XFloatDiveData.cpp`):

1. The dive's end-of-dive fix (X-message 0x02), if it is a valid profile position.
2. Otherwise the first valid start-of-dive fix (0x01) of the next dive, read from the database.
3. For dive 0, the diagnostic-dive fix (0x00) also counts.

`XFloatDiveData::GpsPositionIsValidProfilePosition()` (`src/data/XFloat.cpp`) requires a valid position, a known time not in the future, a time not after the dive's last Iridium message (types 0x00 and 0x02), and a time not before the float's start of mission. Abort (0x03) and built-in-test (0x05) fixes never qualify. Because a dive is only complete with a valid profile position, `POSITION-QC` is always `0` and the date is never the fill value in practice; all production files have `0`.

## 7. Profile data lines

The data lines follow the second rule line, one sample per line. The profile comes from the binned profile (X-messages 0x1n/0x2n/0x3n) if it is complete, otherwise from the high-resolution (unbinned) profile (0x90–0xB7). See [xmessage-format.md](xmessage-format.md) A.3.

Values are converted from counts with the dive's gains and offsets (`value = counts / gain - offset`) and then range-checked in `src/data/xmsg/AbstractXSensorDataComposite.h`:

| Column | Format | Units | Valid range | Fill |
|---|---|---|---|---|
| Pressure | `%8.2f` | dbar | −5 to 2610 (after the offset correction) | −9.999, printed as `  -10.00` |
| Temperature | `%7.3f` | °C (ITS-90) | −5 to 40 | `-9.999` |
| Salinity | `%7.3f` | PSU | 2 to 41 | `-9.999` |

Columns are separated by one space. The pressure has the surface pressure offset subtracted when the correction applies (section 8).

The selection and order of the lines depend on the profile type and on whether it has salinity:

| Profile | Salinity samples | Lines written | Order |
|---|---|---|---|
| Binned (`XCompositeProfile.h`) | yes | Samples whose uncorrected pressure is greater than 1 dbar. | Deepest first: if sample 0 is deeper than sample 1 the stored order is kept, otherwise it is reversed. |
| Binned | no | All samples, including fill values and pressures at or above 1 dbar. | Stored order. |
| Unbinned (`XCompositeHiResProfile.h`) | yes or no | All samples. | Stored order. |

A real temperature-only example near the surface (float 9085, dive 246):

```
    0.17  -9.999
```

A dive can be complete and still produce no data lines. 18 production files end right after the second rule line.

## 8. Surface pressure offset correction (`poc.phy`)

When `poc.phy` is on and the dive's pressure offset is valid (`is_pres_offs_valid()`), the offset `pres_offs()` is subtracted from every pressure in the data lines. Otherwise the correction is 0.

The offset is computed by `XFloatDiveData` from the minimum surface pressures of this dive and up to N preceding dives, only for floats whose CTD needs it (RBR sensors, or WMO types 866, 867 and 875 when the mission configuration gives no sensor type; SBE sensors correct onboard). The listing describes it in its `PRC` line; see [listing-file-format.md](listing-file-format.md).

The 1 dbar cut for binned CTD profiles uses the uncorrected pressure. The PHY file does not say whether the correction was applied or how large it was.

## 9. Differences from the intermediate listing

The intermediate listing (`<dir.lis>/<serial>/lis<serial>-<dive>.txt`, see [listing-file-format.md](listing-file-format.md)) and the PHY file are written from the same dive object, in the same call, with the same gains, offsets and profile data.

| | Intermediate listing | PHY file |
|---|---|---|
| Written | After every decoded message, complete or not. | Only for complete dives, and only with the per-float flag on and metadata present. |
| Switches | `gen.lis`, `poc.lis` | `gen.phy`, per-float flag, `poc.phy` |
| Name | `lis<serial>-<dive>.txt` | `<dac>_<serial>_<dive>.phy` |
| Content | Every decoded message: GPS fixes, profiles, drift, time series, engineering, mission, waves, pressure-correction statistics, hex dumps. | Metadata header, one position, one profile. |
| Line format | Tagged lines (`GPS`, `PRO`, `PRU`, `PRC`, ...) with serial and dive on every line, `#` comment headers. | Untagged; key/value header, then bare numbers. |
| Positions | All GPS fixes of the dive. | One chosen profile position (section 6). |
| Profile | Binned and unbinned profiles, all samples. | One profile (binned preferred); CTD binned profiles cut at 1 dbar and ordered deepest first. |
| Pressure correction | Applied per `poc.lis`, and documented in the `PRC` line. | Applied per `poc.phy`, not documented in the file. |
| Metadata | None from the `.meta` file. | DAC ID, WMO ID, instrument type, WMO types, PI. |

## 10. Readers

XMTPS does not read PHY files back. The Python tools in `bin/py` do not read them.

On argus, `/data1/alamo/bin/upload-phy.sh` (not in this repository) uploaded them. For each `<dir.phy>/<serial>/` directory it copies every `NNNN_NNNN_NNNN.phy` file that is not yet in `<serial>/uploaded/` into that directory, and uploads the copy by `lftp` to the AOML Argo real-time FTP area (`whoi_storm`) and to a second FTP host. Its last log is `upload-phy_2021-03-01.log`. A file in `uploaded/` is therefore a copy of what was sent; a later rewrite of the original is not sent again.

## 11. Production state

On argus (checked 2026-10-30):

- `/data1/alamo/data/phy` has 83 float directories, 28,645 PHY files at the top level and 28,645 copies under `uploaded/`.
- The newest PHY file is dated 2021-01-29 (a reprocessing run); files by year: 2014 to 2021.
- All 467 status files show `PHY_GENERATION_ENABLED 0`, so no float writes PHY files now.
- No top-level PHY file is empty, but 18 of the copies under `uploaded/` are 0 bytes, so those 18 files were uploaded empty (item 2 of section 12).
- 18 top-level PHY files have a header and position but no data lines.

## 12. Open questions / discrepancies

1. **Index mismatch in reversed binned profiles (bug).** In `XCompositeProfile::GeneratePhyString()`, when the stored order is shallow-first, the loop tests the pressure of sample `i` against 1 dbar but prints sample `mx - i`. The cut then removes the deepest samples instead of the near-surface ones, and near-surface samples at or above 1 dbar are printed. Profiles stored deepest-first, which are all production examples checked, are not affected. [inferred: no production file of a shallow-first profile was found to confirm the effect]
2. **Empty files on error (bug).** `WritePhyFile()` opens the output stream before calling `GeneratePhyString()`. If generation throws (unsupported WMO instrument type, non-ALAMO instrument type), the file is left truncated to 0 bytes. The file is also not written atomically, so a reader can see it empty or partly written. In production no top-level PHY file is empty, but 18 copies under `uploaded/` are 0 bytes: the upload script copied, and sent, those files while they were empty. [inferred: whether they were caught mid-write or after a failed generation cannot be told from the files]
3. **Pressure fill prints as `-10.00`.** The fill value −9.999 printed with `%8.2f` rounds to `-10.00`, while temperature and salinity show `-9.999`.
4. **Header column count versus data columns.** The `NUMBER OF COLUMNS` header comes from the WMO instrument type, the data line width from whether the profile has salinity samples. A `.meta` file with a wrong WMO type gives a header that disagrees with the data. [inferred]
5. **Ordering test uses samples 0 and 1 only.** If sample 0 or 1 is a fill value (−9.999), the order is misdetected. [inferred]
6. **Opt-in per float.** README lists `gen.phy` default `1` without saying that the per-float flag defaults to off; with the defaults, no PHY file is written until `--phy-enable` is given.
7. **Startup reprocessing comment.** The comment on the mail retrieval thread says floats "with PHY generation enabled" are reprocessed at startup; the code reprocesses only the IDs given to `--phy-enable` on that run.
8. **Production configuration not verified.** `/data1/alamo/etc/xmtps.cfg` is not readable by the checking account, so the production values of `gen.phy` and `poc.phy` are unknown.
