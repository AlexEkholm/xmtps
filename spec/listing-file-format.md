# Intermediate Listing File Format

This document specifies the intermediate listing files `lis<serial>-<dive>.txt`. The XMTPS telemetry decoder (stage 3) writes one per float and dive cycle: a plain-text record of everything decoded from that dive's SBD messages. People read them, and `bin/py/xmtps-list2json.py` parses them into the per-dive JSON files that the plots and web pages are built from.

Status: written on 2026-10-30 from the source tree (version 1.0, after build 3336) and checked against the production listings on argus (`/data1/alamo/data/list`, 86,109 files; the newest written by build 1.0.3332b). Where this document says "the code", it means the source tree. Production listings come from many builds (section 15); the code and the production files disagree in a few places (section 17).

## Contents

1. [Overview](#1-overview)
2. [File names, location and writing](#2-file-names-location-and-writing)
3. [Lexical conventions](#3-lexical-conventions)
4. [Section order](#4-section-order)
5. [Header block: title, completeness, `XSV`, `LIS`](#5-header-block-title-completeness-xsv-lis)
6. [Iridium SBD messages: `SBD`](#6-iridium-sbd-messages-sbd)
7. [Position data: `APS`, `GPS`](#7-position-data-aps-gps)
8. [Dive data](#8-dive-data)
9. [Engineering data: `E05` to `E0C`, `e0A`, `e0c`](#9-engineering-data-e05-to-e0c-e0a-e0c)
10. [Command echo-back: `CMD`](#10-command-echo-back-cmd)
11. [Extended records: `XFF`](#11-extended-records-xff)
12. [WIMS configuration parameters: `WIP`](#12-wims-configuration-parameters-wip)
13. [Mission configuration: `MIS`, `m`, `mis`](#13-mission-configuration-mis-m-mis)
14. [SBD hex dump: `SHX`, `shx`](#14-sbd-hex-dump-shx-shx)
15. [Older listings in production](#15-older-listings-in-production)
16. [How `xmtps-list2json.py` reads a listing](#16-how-xmtps-list2jsonpy-reads-a-listing)
17. [Open questions / discrepancies](#17-open-questions--discrepancies)

Related specifications: [xmessage-format.md](xmessage-format.md) (the binary meaning of every decoded field; its section numbers, such as A.2 or B.6, are cited throughout), [iridium-mail-format.md](iridium-mail-format.md) (the e-mail fields on `SBD` lines), [raw-file-format.md](raw-file-format.md) and [sbd-file-format.md](sbd-file-format.md) (the same messages as files), [phy-file-format.md](phy-file-format.md) (the other per-dive file stage 3 writes), [configuration-file-format.md](configuration-file-format.md) (`dir.lis`, `gen.lis`, `poc.lis`), [float-metadata-format.md](float-metadata-format.md) (DAC and WMO IDs), [json-file-format.md](json-file-format.md) (what the parser makes of a listing) and [database-schema.md](database-schema.md) (the stored messages a listing is rebuilt from).

## 1. Overview

- Writer: `XFloatDiveData::WriteListingFile()` and `XFloatDiveData::GenerateListingString()` in `src/data/XFloat.cpp`. Each section's lines come from the `GenerateListingString()` (and similar) members of the message classes in `src/data/xmsg/`, `src/data/XFloatMailMessage.h` and `XFloatDiveData::GeneratePressureCorrectionListingString()` in `src/data/XFloatDiveData.cpp`.
- Caller: `XFloatTelemetryDecoderService::ProcessDiveData()` in `src/stage3_TelemetryDecoderThread.h`.
- Reader: `ProcessListingFile()` in `bin/py/xmtps-list2json.py` (section 16). It writes `<output>/<serial>/dives/<serial>_<dive>.json` ([json-file-format.md](json-file-format.md)). `PreviousValidFix()` in the same script also reads the `GPS` lines of earlier listings of the same float.
- One listing holds one dive cycle of one float. It has every decoded message of the dive: SBD session metadata, GPS fixes, the Argo summary, the time series and profiles in physical units, engineering messages, mission configuration, command echoes, and a hex dump of each SBD message.
- The format is line-oriented and column-aligned. It is meant to be read by people and split on whitespace by programs. It is not a self-describing table format: the comment lines above each group of data lines name the columns.

## 2. File names, location and writing

### 2.1 Path

```
<dir.lis>/<serial>/lis<serial>-<dive>.txt
```

- `dir.lis` is the listing root directory (default `lis`; production `/data1/alamo/data/list`). See [configuration-file-format.md](configuration-file-format.md).
- The per-float directory is `printf("%s/%04d", dir.lis, serial)` (`XFloat::GetListingDirectoryPath()`, `src/data/XFloat.cpp`). It is created if it does not exist.
- The file name is `printf("/lis%04d-%04d.txt", serial, dive)` (`XFloatDiveData::WriteListingFile()`).
  - The serial number is the float's WHOI instrument ID (the `mm` field of the X packet, [A.1.2](xmessage-format.md#a12-x-packet-x-nn-mm-dd-p-data--c-c-)). All production serials have four or five digits, so `%04d` adds no padding in practice.
  - The dive number is the packet's signed `dd` field. `%04d` pads it to four characters, and the sign counts as one of them. So dive 7 is `0007`, dive -1 is `-001`, dive -12 is `-012`, and a dive of 10000 or more would use five or more digits.
  - Pre-mission dives (start-up and test cycles) are negative and give names with a double hyphen: `lis10108--001.txt`, `lis11700--012.txt`.

### 2.2 When the file is written

- The decoder thread calls `WriteListingFile()` after each mail message whose X-message data has been decoded into the dive (`ProcessDiveData()`):
  - if the dive is complete (section 5.2), after writing the PHY file and before saving the dive to the database;
  - otherwise, before saving the dive to the database.
- `WriteListingFile()` does nothing when:
  - listing generation is disabled (`gen.lis = 0`), or
  - the dive's "listing file updated" flag is set. Every `Assign…()` member that adds a message to the dive clears the flag (`src/data/XFloatDiveData.h`, `src/data/XFloat.cpp`), and a successful write sets it.
- The whole file is regenerated from the dive's in-memory state and written with `std::ofstream`, which truncates the existing file. The listing therefore always reflects all messages received so far, not just the latest one.
- The write is not atomic: a reader that opens the file while it is being written can see a truncated file [inferred from the code; no temporary file or rename is used].
- Reprocessing (`-r`, `-R`, `-A`; see [configuration-file-format.md](configuration-file-format.md)) rebuilds the dive from the stored messages in the database and rewrites the listing in the format of the running build. Listings of floats that are not reprocessed keep the format of the build that last wrote them (section 15).
- Errors while writing are caught and logged (`Failed to write listing file!`). A section that fails to generate is logged and left out (`CATCH_NOTHROW`), and the rest of the file is still written.

### 2.3 Encoding and size

- Encoding: ASCII. Lines end in LF (`std::endl`). There is no byte-order mark. Binary bytes from SBD messages appear only as hex (section 14) or as `\xNN` escapes (sections 10 and 11).
- Exceptions:
  - The `mis` group headers of the 2.0.62 to 4.6.61 mission configuration layouts contain the unit `(°/sec/FSR)` in UTF-8 (`src/data/xmsg/XMfaMissionCfg.cpp`).
  - Surface engineering (0xEA) version 0x10 writes its error count as a raw byte, often NUL (section 9.7, section 17).
  - Listings from builds before 2015 also have UTF-8 box-drawing characters in a splash banner (section 15).
- The file usually ends with a blank line (the last line of the hex dump section is followed by an empty line).
- Size in production: median 104 KB, largest 2.9 MB; 528 of 86,109 files exceed 400 KB. The example `example/listing/lis9303-0440.txt` is 123 KB and 1,403 lines, most of them the profile, wave spectra and hex dump.

## 3. Lexical conventions

### 3.1 Line classes

| Class | Recognized by | Meaning |
|---|---|---|
| Comment | First character `#` | Banners, section titles, column-name and unit headers, explanatory notes, the hex dump's character rows. Not data. |
| Data | First character a letter | A tag, then whitespace-separated fields. |
| Blank | Empty line | Ends a top-level section. |

- On a data line, a `#` starts a trailing comment: a phase-code name, a sensor-type name, or a parameter description. The comment is for people. The parser uses it in only two places: the position of `#` locates the phase code on `f`/`r` lines, and the BIT `sensorTypeName` is taken from it (section 16).
- `XFF` and `CMD` lines can contain `#` and `;` inside their text field (for example a CTD reply). Only the first `#` of a line is significant for the parser, and those tags are not parsed.

### 3.2 Tags

- The tag is the first whitespace-delimited token of a data line. Tags are case-sensitive: `F` and `f`, `SFC` and `sfc`, `E0A` and `e0A` are different line types.
- Convention: an upper-case tag introduces a group (count, start time, header values) and the lower-case tag of the same name holds its rows (`PRO`/`pro`, `F`/`f`, `VLV`/`vlv`, `SHX`/`shx`). Engineering lines use `E0x` for values and `e0x` for error entries.
- Every data line except `XSV` continues with the float serial number and the dive number, as decimal integers. Their widths vary by tag: most use `%5d %4d`, `SFD` uses `%4d %4d`, and `E07`, `E08`, `E0A`, `E0C`, `e0A` and `e0c` zero-pad the serial (`%05d`, e.g. `09303`). Readers must split on whitespace, not on column positions.
- Many groups then have an index column: `idx` (message part index within a multi-part X-message series), `midx` (minor index: line number within a multi-line message or group), or `sampleIdx` (sample index).

### 3.3 Banners and headings

| Comment line | Produced by | Use |
|---|---|---|
| `#` + 99 `=` | `XMSG_HEADER`, `XMSG_HEADER2` (`src/data/xmsg/common/XDefinitions.h`) | Above and below a top-level section title. |
| `# <Title>` between two `=` rules | same | Top-level section: `Iridium Short-Burst Data (SBD) Message(s)`, `Position Data`, `Dive Data`, `Engineering Data`, … (section 4). |
| `#` + 99 `-` | `XMSG_SECTION`, `XMSG_SECTION_BREAK` | Above and below a subsection title, and alone between repeated groups (e.g. between the `F` groups of a multi-part fall series, and every 50 rows of `pro`, `pru` and `d`). |
| `# <Title>` between two `-` rules | `XMSG_SECTION` | Subsection: `Argo Data`, `Fall Time-Series Data`, `Engineering (Surface)`, … |
| `# --- <Name> ----…` | inline in the message classes | Sub-group heading inside an engineering message or mission configuration, e.g. `# --- Air Pump & Bladder ---`. Some sub-groups have an unnamed `# -----…` rule instead. |
| `#      SN dive …` | inline | Column-name header for the data lines that follow. |
| `#      …  (dBar)  (degC) …` | inline | Units header, right-aligned under the column names, directly below the name header. Absent where no column has a unit. |
| `#` alone | inline | Spacer (between the `A` groups, under the `MIS` and BIT headers). |
| `#   (Incomplete … Data)` | inline | The series has missing parts; its rows are omitted (sections 8.4 to 8.6). |

The column headers are exact text. Units are in parentheses in headers and in square brackets in the `units` field of `m` lines.

### 3.4 Value formats

| Kind | Format | Example | Unset or invalid |
|---|---|---|---|
| Date/time (UTC) | ISO 8601 extended, `to_iso_extended_string(t) + "Z"` (`DateUtils::GenerateIso8601String`, `src/utility/DateUtils.h`) | `2026-10-19T14:36:17Z` | `0000-00-00T00:00:00Z` |
| Date/time with microseconds (`APS` receive lines only) | `…T00:01:51.212319Z` | `2026-09-27T00:01:51.212319Z` | `0000-00-00T00:00:00.000000Z` |
| Relative time | `%+d` seconds from the group's start time, in a 20-character field | `+10801` | — |
| Position | Latitude and longitude run together, `%+.5f%+.5f` (`GenerateIso6709String`, `src/utility/PositionUtils.h`) | `+28.41912-67.55337` | — |
| Pressure (dBar) | `XMsgFormatPressure()`: two decimals (`src/data/xmsg/common/XDefinitions.h`) | `290.48` | `-9.999` |
| Temperature (°C), salinity (PSU) | `%.3f` in the column's width | `19.070` | `-9.999` |
| Hex code | `0x%08X`, `0x%02X`, `0x%04X` | `0x00000114` | — |
| Integers | `%d` in the column's width | | `-1` for unknown IDs, printed zero-padded where the format pads (`-001`, `-000001`) |
| Wave spectra | `%+11.5f` (`wvs`), `%+11.4f` (`wvm`) | `+0.00089` | `-999.99999` (`wvs`); prints as `-1000.0000` in `wvm` |
| Case vacuum not measured | `%.2f` of 65535 / 100 | `655.35` | — |

- Sensor values are computed from counts with the dive's Argo gains and offsets (`value = counts / gain − offset`, [A.1.5](xmessage-format.md#a15-counts-to-units-conversion)) and range-checked. Pressure must be within −5 … 2610 dBar, temperature −5 … 40 °C, salinity 2 … 41 PSU (`XMSG_SENS_MIN_*`, `XMSG_SENS_MAX_*`). A value outside its range, or a value whose gain is 0 or unknown, is listed as `-9.999`.
- Without an Argo message (0xF0) in the dive, the gains are unknown and every converted sensor value is `-9.999`; the header says `Sensor Gains & Offsets Known: no`.
- Pressures in the fall, drift, profile, valve, pump and rise sections are corrected for the surface pressure offset when the correction applies (section 8.2). Engineering-message pressures (`E08`, `E09`, `E0A`, `E0C`, BIT) and the surface series (`SFC`, `sfc`, `SFD`) are never corrected.
- The pressure fill test in `XMsgFormatPressure()` is a ±0.0005 window around −9.999, so a corrected pressure that happens to equal −9.999 would also print as the fill value.
- IMEIs are listed as their last seven digits, zero-padded to 15 characters (`printf("%015d", imei % 10000000)`), on purpose (README, "Listing files").

## 4. Section order

`GenerateListingString()` writes the sections in this fixed order. Sections whose condition is false are left out entirely, banner included.

| # | Top-level banner (`=`) | Subsections (`-`) | Condition | Tags |
|---|---|---|---|---|
| 1 | `Intermediate Listing for Float #<serial>, Dive #<dive>` | `Dive Data Completeness`, `XMTPS Software Version`, `Listing Details` | always | `XSV`, `LIS` |
| 2 | `Iridium Short-Burst Data (SBD) Message(s)` | — | the dive has at least one mail message | `SBD` |
| 3 | `Position Data` | `Acoustic Position` (if any 0xCA blob), `GPS` (if any GPS fix) | the dive has a GPS fix or a mail message (`HasPositionData()`) | `APS`, `GPS` |
| 4 | `Dive Data` | `Argo Data`, `Pressure Correction`, `Fall Time-Series Data`, `Drift Data`, `Profile Data (Binned) - Sensor Columns`, `Profile Data (Binned)`, `Profile Data (Unbinned)`, `Valve Data`, `Pump Data`, `Rise Time-Series Data`, `Surface Time-Series Data`, `Surface Time-Series Data - Derived Fields`, `Ocean Surface Wave-Field Data` | drift data, a complete binned or unbinned profile, fall or rise data, surface data, or wave data (`HasDiveData()`) | `A`, `PRC`, `F`/`f`, `D`/`d`, `PRD`/`prd`, `PRO`/`pro`, `PRU`/`pru`, `VLV`/`vlv`, `PMP`/`pmp`, `R`/`r`, `SFC`/`sfc`, `SFD`, `WVS`/`wvs`/`wvm` |
| 5 | `Engineering Data` | `Engineering (Abort)`, `(BIT failed)`, `(BIT passed)`, `(Descent)`, `(Diagnostic Dive)`, `(Park/Drift)`, `(Ascent)`, `(Ice)`, `(Standard Dive)`, `(Surface)` | any engineering message except ice (`HasEngineeringData()`; section 17) | `E05`, `E06`, `E07`, `E08`, `E09`, `E0C`, `e0c`, `E0A`, `e0A` |
| 6 | `Ground Station Upload Command Echo-Back Messages` | — | at least one 0xDE blob | `CMD` |
| 7 | `Extended Record Messages (0xFF)` | — | at least one 0xFF blob (build 3321 and later) | `XFF` |
| 8 | `WIMS Configuration Parameters (0xC5)` | — | at least one 0xC5 blob (build 3336 and later) | `WIP` |
| 9 | `Mission Configuration` | `# --- <group> ---` sub-groups | always | `MIS`, `m`, `mis` |
| 10 | `SBD Message Hex Data` | — | the dive has at least one mail message | `SHX`, `shx` |

Within section 4, each subsection is printed only when its data exists:

| Subsection | Condition |
|---|---|
| Argo Data | the dive has an Argo message (0xF0) |
| Pressure Correction | always, when section 4 is printed |
| Fall Time-Series Data | any fall message (0x4n) |
| Drift Data | any drift message (0x98–0xBF) |
| Profile Data (Binned) - Sensor Columns, Profile Data (Binned) | any binned profile message (0x1n–0x3n), complete or not |
| Profile Data (Unbinned) | any high-resolution profile message (0x90–0xB7) |
| Valve Data | any valve message (0xEB) |
| Pump Data | any pump message (0x6n) |
| Rise Time-Series Data | any rise message (0x5n) |
| Surface Time-Series Data and its Derived Fields | any surface message (0x8n) |
| Ocean Surface Wave-Field Data | a wave message with a non-zero segment length (`HasWaveData()`) |

Section 4 needs one of the data types in its condition. An Argo message, valve or pump data, or an incomplete profile alone does not make the section appear: until other data arrives, those messages are decoded but not listed (section 17).

Each top-level section ends with a blank line. Inside section 5 the subsections follow the fixed order above; the SOLO-II subsections `Engineering (Abort)`, `(Diagnostic Dive)` and `(Standard Dive)` print only their banners (section 9.9).

## 5. Header block: title, completeness, `XSV`, `LIS`

### 5.1 Title and file details

```
#===================================================================================================
# Intermediate Listing for Float #9303, Dive #483
#===================================================================================================
#                        Created: 2026-10-28T23:20:04Z
#                     Local Path: "/data1/alamo/data/list/9303/lis9303-0483.txt"
```

- `Created`: the UTC time the file was generated (`GenerateCurrentUtcTimeIso8601String()`), not a data time.
- `Local Path`: the path the file was written to, printed by `boost::filesystem::path`'s stream operator, so in double quotes. It is `dir.lis` joined with the relative name, absolute only if `dir.lis` is absolute. The line is omitted if the path is not yet set, which does not happen when the file is written [inferred].
- Both lines use `# %30s: %s`.

### 5.2 Dive Data Completeness

```
#---------------------------------------------------------------------------------------------------
# Dive Data Completeness
#---------------------------------------------------------------------------------------------------
#                  Dive Complete: yes
#          Complete Profile Data: yes
#              Contiguous MOMSNs: yes
#                Contiguous PIDs: yes
#   Sensor Gains & Offsets Known: yes
#                 Valid Position: yes
```

All six lines are comments (`# %30s: %s`, value `yes` or `no`). Nothing parses them.

| Item | Source (`src/data/XFloatDiveData.cpp`) | `yes` when |
|---|---|---|
| Dive Complete | `is_complete()`, set by `CheckCompleteness()` | Complete Profile Data, Contiguous PIDs, Sensor Gains & Offsets Known and Valid Position are all `yes`. Contiguous MOMSNs is not required. |
| Complete Profile Data | `HasCompleteProfileData()` | the binned or the unbinned profile has all its parts for pressure, temperature and salinity. |
| Contiguous MOMSNs | `HasContiguousMomsns()` | the dive's MOMSNs form an unbroken sequence (a single message counts). Informational only. |
| Contiguous PIDs | `HasContiguousPids()` | packet IDs 0 … max are all present. |
| Sensor Gains & Offsets Known | `SensorGainsAndOffsetsAreKnown()` | the dive has an Argo message (0xF0). |
| Valid Position | `HasValidPosition()` | the end-of-dive GPS fix (or the next dive's start-of-dive fix) passes `GpsPositionIsValidProfilePosition()` (`src/data/XFloat.cpp`). Iridium positions never count. |

"Dive Complete" is the value stored when the listing is written. A dive that is complete triggers the PHY file, KML and NetCDF updates (see [phy-file-format.md](phy-file-format.md)).

### 5.3 `XSV`: software version

Subsection `XMTPS Software Version`. One line, always.

```
#       xmessage-version        xmtps-version     build-configuration                build-date/time
XSV                  2.2            1.0.3332b                 RELEASE         2026-10-28T15:00+00:00
```

Format `XSV %20s %20s %23s %30s` (`src/data/XFloat.cpp`). There are no serial or dive columns.

| Column | Format | Meaning |
|---|---|---|
| xmessage-version | `%20s` | `PROG_XVERS` (`src/config/LocalConfiguration.h`), the X-message format version the decoder targets. Constant `2.2`. |
| xmtps-version | `%20s` | `PROG_VERS`: `<major>.<minor>.<build>` plus `a` for a debug build or `b` for a release build. `1.0.<build>` since 2026-10-26; `0.6.<build>` before. |
| build-configuration | `%23s` | `DEBUG` or `RELEASE`. |
| build-date/time | `%30s` | `BUILD_DATETIME`, set by the build: local ISO 8601 date and time to the minute with UTC offset. |

Parser: ignored.

### 5.4 `LIS`: listing details

Subsection `Listing Details`. One line, always, followed by a blank line.

```
#      SN dive DAC-ID            IMEI  WMO-ID
LIS  9303  483   -001 000000002418110 -000001
```

Format `LIS %5d %4d   %04d %015d %07d`.

| Column | Format | Meaning |
|---|---|---|
| SN | `%5d` | Float serial number. |
| dive | `%4d` | Dive number. |
| DAC-ID | `%04d` | Argo data assembly center ID from the float record (`.meta` file; [float-metadata-format.md](float-metadata-format.md)); `-001` if unknown. |
| IMEI | `%015d` | Last seven digits of the Iridium IMEI, zero-padded. |
| WMO-ID | `%07d` | WMO platform number; `-000001` if unknown. |

Parser: reads SN (`x[1]`), dive (`x[2]`), DAC-ID (`x[3]`) and WMO-ID (`x[5]`); the IMEI is ignored. Reading `LIS` also triggers the up-to-date check: without `--force`, the listing is skipped if its JSON file is newer (or exists at all, with `--skip`).

## 6. Iridium SBD messages: `SBD`

Top-level section `Iridium Short-Burst Data (SBD) Message(s)`. One line per mail message assigned to the dive, in ascending MOMSN order (`XFloatDiveData::AssignMailMessage`, `src/data/XFloatMailMessage.cpp`). Retransmissions (same SBD bytes under a new MOMSN) are listed too, so a packet ID can appear twice. Writer: `XFloatMailMessage::GenerateListingComment()` and `GenerateListingString()` (`src/data/XFloatMailMessage.h`).

```
#      SN dive size            IMEI MOMSN MTMSN PID            date/time            position  CEP
#               (b)                                                                          (km)
SBD  9303  483  287 000000002418110  6328     0  11 2026-10-19T14:46:44Z  +28.44964-67.49324   22
```

Format `SBD %5d %4d %4d %015d %5d %5d %3d %20s %19s %4d`.

| Column | Format | Units | Meaning |
|---|---|---|---|
| SN | `%5d` | | Serial number. |
| dive | `%4d` | | Dive number (from the X packet header). |
| size | `%4d` | bytes | Size of the SBD attachment (the X packet). |
| IMEI | `%015d` | | Last seven digits of the IMEI. |
| MOMSN | `%5d` | | Mobile-originated message sequence number. |
| MTMSN | `%5d` | | Mobile-terminated message sequence number (0 if none). |
| PID | `%3d` | | Packet ID `p` within the dive ([A.1.2](xmessage-format.md#a12-x-packet-x-nn-mm-dd-p-data--c-c-)). |
| date/time | `%20s` | UTC | The mail's `Time of Session (UTC)`. |
| position | `%19s` | degrees | The mail's Iridium unit location (`Lat`, `Long`). |
| CEP | `%4d` | km | The mail's `CEPradius`. |

The e-mail fields are specified in [iridium-mail-format.md](iridium-mail-format.md). Parser: reads size, MOMSN, MTMSN, PID, date/time, position and CEP (`x[3]`, `x[5]` to `x[10]`).

## 7. Position data: `APS`, `GPS`

Top-level section `Position Data`. It is printed when the dive has a GPS fix or a mail message, so in practice always; a dive without GPS fixes or acoustic data gets only the banner and a blank line. The acoustic subsection comes first.

### 7.1 `APS`: acoustic positioning (0xCA)

Subsection `Acoustic Position`, printed when the dive has any 0xCA blob (marginal-ice-zone floats; build 3249 and later). Writer: `XAcousticPosition::GenerateListingString()` (`src/data/xmsg/XAcousticPosition.h`). Binary format: [C.4](xmessage-format.md#c4-marginal-ice-zone-acoustic-messages-0xca).

Before listing, `AssignWindowIndices()` numbers the windows 0, 1, … in the order received (not in time order) and gives each receive record the index of the first window that contains its time of arrival.

```
# --- Acoustic Listening Windows -------------------------------------------------------------------
#      SN dive midx widx      start-date/time        end-date/time
#                                       (UTC)                (UTC)
APS  9213    1    3    0 2026-09-26T23:57:54Z 2026-09-27T00:30:49Z 
APS  9213    1    3    1 2026-09-26T19:58:08Z 2026-09-26T20:20:02Z 
# --- Acoustic Communication Cycle Receive Statistics ----------------------------------------------
#      SN dive midx widx                   date/time    src MFDPwr PSKErr  SNRIn SNROut
#                                              (UTC)          (dB)          (dB)   (dB)
APS  9213    1    5    0 2026-09-27T00:01:51.212319Z     -1     45      4  -99.9   -9.9
```

Window lines, format `APS %5d %4d %4d %4d %20s %20s ` (with a trailing space):

| Column | Format | Units | Meaning |
|---|---|---|---|
| SN, dive | `%5d %4d` | | |
| midx | `%4d` | | Subtype, always 3 (listening window). |
| widx | `%4d` | | Window index. |
| start-date/time, end-date/time | `%20s` | UTC | Window start and end. |

Receive lines (CACST records), format `APS %5d %4d %4d %4d %27s %6d %6d %6d %6.1f %6.1f`:

| Column | Format | Units | Meaning |
|---|---|---|---|
| SN, dive | `%5d %4d` | | |
| midx | `%4d` | | Subtype, always 5. |
| widx | `%4d` | | Index of the window that contains the record; `-1` if none. |
| date/time | `%27s` | UTC | Time of arrival with microseconds. |
| src | `%6d` | | Source ID. |
| MFDPwr | `%6d` | dB | Matched-filter detector power. |
| PSKErr | `%6d` | | PSK error code. |
| SNRIn, SNROut | `%6.1f` | dB | Input and output SNR. |

Variants: before build 3309 the SNROut column repeated SNRIn. Parser: not read (logged as an unknown line at debug level).

### 7.2 `GPS`: GPS fixes (0x00–0x05)

Subsection `GPS`, printed when the dive has at least one GPS message. Writer: `XGPS::GenerateListingComment()` and `GenerateListingString()` (`src/data/xmsg/XGPS.h`). Binary format: [A.2](xmessage-format.md#a2-gps-position--0x000x05). One line per GPS message, ordered by message type (`AssignGps`, `src/data/XFloat.cpp`).

```
#      SN dive idx            date/time            position   TTF nSat   SNR   SNR   SNR HDOP stat
#                                                            (sec)      (min) (avg) (max)
GPS  9303  483   1 2026-10-18T08:48:00Z  +28.27064-67.21926     0    5     0     0     0  1.4    0
GPS  9303  483   2 2026-10-19T14:41:00Z  +28.41912-67.55337   280    4     0     0     0  4.5    0
```

Format `GPS %5d %4d %3s %20s %19s %5d %4d %5d %5d %5d %4.1f %4d`.

| Column | Format | Units | Meaning |
|---|---|---|---|
| SN, dive | `%5d %4d` | | |
| idx | `%3s` | | The GPS message type, not a running index: 0 after the diagnostic dive, 1 start of dive, 2 end of dive, 3 after a mission abort, 5 during the built-in test. |
| date/time | `%20s` | UTC | Fix time. |
| position | `%19s` | degrees | Fix latitude and longitude. |
| TTF | `%5d` | s | Time to fix. |
| nSat | `%4d` | | Number of satellites. |
| SNR (min), SNR (avg), SNR (max) | `%5d` each | | Signal-to-noise ratios. 0 when the firmware does not send them. |
| HDOP | `%4.1f` | | Horizontal dilution of precision. |
| stat | `%4d` | | Always `0`: the code prints a constant. |

The type-1 fix is taken just before this dive's descent and the type-2 fix after its ascent, so a normal dive lists one of each. Parser: reads every column except `stat` (`x[3]` to `x[11]`) and estimates a position uncertainty from them; `PreviousValidFix()` reads the same columns from earlier dives' listings.

## 8. Dive data

Top-level section `Dive Data` (condition in section 4). Its subsections, in order, are 8.1 to 8.13.

### 8.1 `A`: Argo data (0xF0)

Subsection `Argo Data`, printed when the dive has an Argo message. Writer: `XArgo::GenerateListingString()` (`src/data/xmsg/XArgo.h`). Binary format: [A.6](xmessage-format.md#a6-argo-data--0xf0).

Six lines, `midx` 0 to 5, each with its own header and a `#` spacer between them. All lines start with `A   %5d %4d %4.1f %3d %4d`:

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| xVer | `%4.1f` | Argo message version as a decimal (2.1 to 2.4). |
| typ | `%3d` | Argo instrument type: 3 = ALAMO. |
| midx | `%4d` | Line index 0–5. |

Remaining columns by `midx`:

| midx | Columns (format) | Units |
|---|---|---|
| 0 | missPhase (`%10d`), profPres (`%9d`), parkPres (`%9d`), maxAscT (`%9d`), maxDesc2ParkT (`%14d`), maxDesc2ProfT (`%14d`) | —, dBar, dBar, min, min, s (as the header says) |
| 1 | tarParkT (`%10d`), nSeek (`%7d`), seekIntervalT (`%14d`), tarAscRate (`%11d`), maxSurfT (`%9d`) | raw value in units of 5 min (the units line has no entry for it; [A.6.1](xmessage-format.md#a61-v22--v23--v24--66-bytes-mrv-firmware-105-argo-packet-version-0x24-mo)), —, min, cm/s, s |
| 2 | start-of-descent, start-of-drift (`%24s` each) | UTC |
| 3 | start-of-ascent, start-of-surface (`%24s` each) | UTC |
| 4 | pGain, pOff, tGain, tOff, sGain (`%11s`), sOff (`%10s`) | 1/dBar, dBar, 1/degC, degC, 1/PSU, PSU |
| 5 | pGainSurf, pOffSurf, tGainSurf, tOffSurf, sGainSurf (`%11d`), sOffSurf (`%10d`) | as midx 4, for the surface time series |

```
A    9303  483  2.4   3    0          0       500       300        30            196              1
A    9303  483  2.4   3    4          25          10        1000           5        1000          1
```

Variants:

- Only instrument type 3 is listed. For type 0 (SOLO-II) or 1 (Deep Argo) the subsection is empty. For an unknown type the single line `A (unknown float type)` is printed.
- Before build 3309, `nSeek` was read as one byte.
- The gains and offsets on `midx` 4 and 5 are the values used for every conversion in this listing (section 3.4).

Parser: `A` lines are skipped.

### 8.2 `PRC`: pressure correction

Subsection `Pressure Correction`, printed whenever section 4 is. Writer: `GenerateListingString()` (the note) and `XFloatDiveData::GeneratePressureCorrectionListingString()` (`src/data/XFloatDiveData.cpp`). The method is described in README, "Surface pressure offset correction".

A comment note comes first. When the correction is applied to this listing (`is_pres_offs_valid()` and `poc.lis = 1`):

```
# presOffset has been subtracted from the fall (F/f), drift (D/d), profile (PRO/pro, PRD/prd),
# valve (VLV/vlv), pump (PMP/pmp) and rise (R/r) pressures in this listing:
#     pres = rawPres - presOffset
# To recover the raw pressures, add presOffset back: rawPres = pres + presOffset.
# The surface pressures (SFC/sfc, SFD) are not corrected; they are what presOffset is
# computed from. A corrected pressure outside the valid range is listed as -9.999 and
# cannot be recovered.
```

Otherwise the single comment `# No pressure correction has been applied in this listing: all pressures are raw.`

Then one data line:

```
#      SN dive midx nSam nDive valid      presOffset  sfcPmin sfcPmean  sfcPmax  sfcPstd
#                                             (dBar)   (dBar)   (dBar)   (dBar)   (dBar)
PRC  9303  483    0   79     4     1           -0.04    -0.05    -0.01     0.06    0.034
```

Format `PRC %5d %4d %4s %4d %5d %5d %15s %8s %8s %8s %8s`.

| Column | Format | Units | Meaning | Fill |
|---|---|---|---|---|
| SN, dive | `%5d %4d` | | | |
| midx | `%4s` | | Always 0. | |
| nSam | `%4d` | | Surface pressure samples (after outlier removal) summed over the dives the offset is computed from. | `-1` when no offset was computed |
| nDive | `%5d` | | Number of dives (this one and up to three before it) the offset is computed from. | `-1` |
| valid | `%5d` | | 1 if an offset was computed, else 0. No offset is computed, and 0 is listed, for SBE floats (CTD type from the mission configuration, else the `.meta` WMO instrument type) or floats of unknown type (`XFloatDiveData::ComputePressureOffsetFromSurfaceTimeSeries`, `src/data/XFloat.cpp`). | |
| presOffset | `%15s` | dBar | Mean of the per-dive minimum surface pressures. Subtracted from the pressures listed in section 8 when `valid` is 1 and `poc.lis` is 1. | `0.00` when not valid |
| sfcPmin | `%8s` | dBar | This dive's uncorrected minimum surface pressure, outliers excluded (the value the offset uses). | `-9.999` |
| sfcPmean, sfcPmax | `%8s` | dBar | This dive's uncorrected mean and maximum surface pressure. | `-9.999` |
| sfcPstd | `%8s` | dBar | Square root of the surface pressure variance, `%.3f`. | `-9.999` |

Variants: before build 3310 (2026-10-25) the line ended at `presOffset`; before 2026-10-24, pressures had three decimals.

Parser: reads nSam, nDive, valid and presOffset (`x[4]` to `x[7]`); the surface statistics are ignored.

### 8.3 `F`, `f`: fall (descent) time series (0x40–0x4F)

Subsection `Fall Time-Series Data`. Writer: `XCompositeFall::GenerateListingString()` and `XFall::GenerateListingString()` (`src/data/xmsg/XCompositeFall.h`, `XFall.h`), sample text from `XSensorTripletCodeCounts::GenerateListingString()` (`src/data/xmsg/common/XSensorTripletCodeCounts.h`). Binary format: [A.5.1](xmessage-format.md#a51-fall-0x400x4f-rise-0x500x5f-and-surface-0x800x8f-time-series); phase codes [A.5.2](xmessage-format.md#a52-phase-codes-4-bit-shared-by-fallrisesurfacepumpvalve).

Each fall message (part) gives one `F` line followed by its `f` lines. From the second part on, a `#---` rule and the column header are repeated before the `F` line.

```
#      SN dive idx nSam                 time      pres phaseCode   phaseCodeName
#                                  (UTC/+sec)    (dBar)
F    9303  483   0   17 2026-10-18T08:49:01Z
f    9303  483   0    0                   +0      0.00         1 # start of descent
f    9303  483   0    1                 +222     30.44        10 # descending
```

`F` line, format `F   %5d %4d %3d %4d %20s`:

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| idx | `%3d` | Part index (low nibble of the X-message ID). |
| nSam | `%4d` | Number of samples in this part. |
| time | `%20s` | Start time of this part (UTC). |

`f` line, format `f   %5d %4d %3d %4d %+20d %9s %9d # %s`:

| Column | Format | Units | Meaning |
|---|---|---|---|
| SN, dive | `%5d %4d` | | |
| idx | `%3d` | | Part index (same as the `F` line). |
| nSam | `%4d` | | Here: sample index within the part, from 0. |
| time | `%+20d` | s | Offset from the part's start time. |
| pres | `%9s` | dBar | Pressure, offset-corrected when the correction applies; `-9.999` if invalid. |
| phaseCode | `%9d` | | 4-bit phase code. |
| `# phaseCodeName` | `%s` | | Name of the phase code (`XPresCodeCounts::phase_code_names_`, `src/data/xmsg/common/XPresCodeCounts.cpp`): `unknown`, `start of descent`, `first descent pump`, `seek to drift pres`, `start of drift`, `seek during drift`, `start of descent to profile`, `start of ascent`, `end of ascent`, `ice turn-around`, `descending`, `drift/park`, `descending to profile`, `ascending`, `surface`, `invalid phase code` for codes 0–15. |

Variant: listings written before version 1.0 (2026-10-26; README gives build 3321) have `temp` (`%7.3f`, degC) and `saln` (`%7.3f`, PSU) columns between `pres` and `phaseCode`, with `(degC)  (PSU)` in the units header. Only floats 9035 and 9050 sent them; for every other float they were `-9.999`.

Parser: `F` gives the reference time (`x[5]`); each `f` gives the time offset (`x[5]`), pressure (`x[6]`) and the phase code, taken as the field just before `#` so that both layouts work.

### 8.4 `D`, `d`: drift data (0x98–0xBF, 0x78–0x7F)

Subsection `Drift Data`, printed when the dive has any drift sensor message. Writer: `XCompositeDrift::GenerateListingString()` (`src/data/xmsg/XCompositeDrift.h`). Binary formats: [A.3](xmessage-format.md#a3-sensor-series-binned-profile-0x1n0x2n0x3n-high-resolution-profile-0x900xb7-drift-0x980xbf) (values), [A.4.1](xmessage-format.md#a41-drift-sample-times--0x780x7f--format-inferred-i-decoder-d-xdrifttimeh) (sample times). Two layouts exist.

**With sample times** (the float sends drift time messages 0x78–0x7F, and they are complete: contiguous indices and one time per sample):

```
#      SN dive nSam                 time      pres    temp    saln
#                             (UTC/+sec)    (dBar)  (degC)   (PSU)
D    9303  483    8 2026-10-18T12:57:53Z
d    9303  483    0                   +1    284.56  19.080  36.689
d    9303  483    1               +10801    291.20  19.109  36.696
```

| Line | Format | Columns |
|---|---|---|
| `D` | `D   %5d %4d %4d %20s` | SN, dive, nSam (number of samples), time: the drift start time from the Argo message (`start-of-drift`, `A` `midx` 2), or `0000-00-00T00:00:00Z` without one. |
| `d` | `d   %5d %4d %4d %+20d %9s %7.3f %7.3f` | SN, dive, sample index (under the `nSam` header), time offset in s from the `D` time, pres (dBar), temp (degC), saln (PSU). |

**Without sample times** (most floats):

```
#      SN dive  numSamples
D   11333   74         407
#---------------------------------------------------------------------------------------------------
#      SN dive   sampleIdx        pres        temp        saln
#                                (dBar)      (degC)      (PSU)
d   11333   74           0     1000.23       4.168      34.506
```

| Line | Format | Columns |
|---|---|---|
| `D` | `D   %5d %4d %11d` | SN, dive, numSamples. |
| `d` | `d   %5d %4d %11d %11s %11.3f %11.3f` | SN, dive, sampleIdx, pres (dBar), temp (degC), saln (PSU). The rule and column header are repeated before every 50th row. |

- If the drift data is incomplete (a part missing for pressure, temperature or salinity), the `D` line is followed by `#   (Incomplete Drift Data)` and no `d` lines.
- If time messages were received but are incomplete, the no-times layout is used and `#   (Incomplete Drift Time Data)` follows the `D` line.
- Variants: the times layout exists from 2026-10-24 on. Before then pressures had three decimals.

Parser: `D` gives the sample count (`x[3]`) and, if present, the reference time (`x[4]`). `d` lines are matched to the last column header (`ColumnValues()`): `pres`, `temp`, `saln`, and in the times layout `nSam` (index) and `time`. Without a matching header the fallback is `x[4]`, `x[5]`, `x[6]`.

### 8.5 `PRD`, `prd`: binned profile column descriptions

Subsection `Profile Data (Binned) - Sensor Columns`, printed when the dive has binned profile data (2026-10-24 and later). Writer: `XCompositeProfile::GenerateColumnDescriptionString()` (`src/data/xmsg/XCompositeProfile.h`).

```
#      SN dive  numColumns
PRD  9303  483           5
#---------------------------------------------------------------------------------------------------
#      SN dive      column    units  description
prd  9303  483   sampleIdx        -  # Bin index, in the order the float reports the bins.
prd  9303  483        pres     dBar  # Bin pressure, less the surface pressure offset (see Pressure
#                                      Correction).
prd  9303  483       temp1     degC  # CTD temperature.
prd  9303  483       temp2     degC  # Auxiliary temperature channel.
prd  9303  483        saln      PSU  # Salinity reported by the CTD.
#   Values outside the valid range are listed as -9.999.
```

| Line | Format | Columns |
|---|---|---|
| `PRD` | `PRD %5d %4d %11d` | SN, dive, numColumns: the number of `prd` lines, which equals the number of fields after the dive number on each `pro` line. |
| `prd` | `prd %5d %4d %11s %8s  # %s` | SN, dive, column name, units (`-` if none), `#` description. |

- Descriptions are wrapped at 99 characters; continuation lines are comments indented to the description column.
- The columns described are exactly those of this dive's `pro` lines, in order: `sampleIdx`, `pres`, then `temp1` and `temp2` when the float sends an auxiliary temperature channel ([A.4.3](xmessage-format.md#a43-auxiliary-temperature-channel-format-nibble-3-on-0x2n)) or `temp` otherwise, `saln`, and `salnCorr` ("Dynamically corrected salinity, computed on the float (mission parameter PPE).") when the dynamically corrected salinity channel is complete ([A.4.2](xmessage-format.md#a42-dynamically-corrected-salinity--0xdc--format-inferred-i-decoder-d-xprofilesalinitycorrectedh)).
- The table is printed even when the profile is incomplete.

Parser: `PRD` and `prd` are skipped; the parser reads the `pro` column header instead.

### 8.6 `PRO`, `pro`: binned profile (0x10–0x3F, 0xDC)

Subsection `Profile Data (Binned)`. Writer: `XCompositeProfile::GenerateListingString()`. Binary format: [A.3](xmessage-format.md#a3-sensor-series-binned-profile-0x1n0x2n0x3n-high-resolution-profile-0x900xb7-drift-0x980xbf).

```
#      SN dive  numSamples
PRO  9303  483         290
#---------------------------------------------------------------------------------------------------
#      SN dive   sampleIdx        pres       temp1       temp2        saln
#                               (dBar)      (degC)      (degC)       (PSU)
pro  9303  483           0      290.32      19.070      19.063      36.689
```

| Line | Format | Columns |
|---|---|---|
| `PRO` | `PRO %5d %4d %11d` | SN, dive, numSamples (number of bins). |
| `pro`, standard | `pro %5d %4d %11d %11s %11.3f %11.3f` | SN, dive, sampleIdx, pres (dBar), temp (degC), saln (PSU). |
| `pro`, auxiliary temperature | `pro %5d %4d %11d %11s %11.3f %11.3f %11.3f` | SN, dive, sampleIdx, pres, temp1 (CTD, degC), temp2 (auxiliary channel, degC), saln. |
| `salnCorr` column | ` %11.3f` appended | Dynamically corrected salinity (PSU), when complete. |

- Bins are listed in the order the float sends them, normally deepest first. `sampleIdx` counts from 0.
- The rule and the column header (with a `salnCorr` / `(PSU)` column when present) are printed before bins 0, 50, 100, …; the units header is part of each repetition.
- Incomplete data: `#   (Incomplete Binned Profile Data)` after `PRO`, and no `pro` lines. If the corrected-salinity channel was received but is incomplete, the comment `#   (Incomplete Corrected Salinity Data)` is printed and the column is left out.
- Variants: `salnCorr` since 2026-10-24; three-decimal pressures before then.

Example with corrected salinity (float 11730, 10.12.x with `PPE` = 2):

```
#      SN dive   sampleIdx        pres       temp1       temp2        saln    salnCorr
#                               (dBar)      (degC)      (degC)       (PSU)       (PSU)
pro 11730    1           0      664.13       6.496       6.492      34.661      34.661
```

Parser: `PRO` gives the bin count (`x[3]`). Each `pro` line is matched to the last column header; the fallback without a header is by field count (7 fields standard, 8 auxiliary). `temp2` becomes the JSON `tcnd`, `salnCorr` `saln_corr`.

### 8.7 `PRU`, `pru`: unbinned (high-resolution) profile (0x90–0xB7)

Subsection `Profile Data (Unbinned)`, printed when the dive has any high-resolution profile message (few floats send them, e.g. 9038). Writer: `XCompositeHiResProfile::GenerateListingString()` (`src/data/xmsg/XCompositeHiResProfile.h`).

```
#      SN dive  numSamples
PRU  9038   22          25
#---------------------------------------------------------------------------------------------------
#      SN dive   sampleIdx        pres        temp        saln
#                                (dBar)      (degC)      (PSU)
pru  9038   22           0      37.687      28.175      -9.999
```

| Line | Format | Columns |
|---|---|---|
| `PRU` | `PRU %5d %4d %11d` | SN, dive, numSamples. |
| `pru` | `pru %5d %4d %11d %11s %11.3f %11.3f` | SN, dive, sampleIdx, pres (dBar), temp (degC), saln (PSU). Header repeated every 50 rows. |

Incomplete data: `#   (Incomplete Unbinned Profile Data)` and no `pru` lines. The production example above is from build 3067, hence three-decimal pressures.

Parser: not read (logged as unknown lines). Unbinned profiles do not reach the JSON files.

### 8.8 `VLV`, `vlv`: valve operations (0xEB)

Subsection `Valve Data`. Writer: `XCompositeValve::GenerateListingString()`, `XValve::GenerateListingString()`, `XValveCounts::GenerateListingString()` (`src/data/xmsg/XCompositeValve.h`, `XValve.h`, `common/XValveCounts.h`). Binary format: [A.5.4](xmessage-format.md#a54-valve--0xeb). Multiple messages are separated by a rule and a repeated header.

```
#      SN dive idx nSam       pres  valveTime       vac0       vac1
#                            (dBar)      (sec)     (inHg)     (inHg)
VLV  9303  483   0    1
vlv  9303  483   0    0       0.04        405     655.35       5.55
```

| Line | Format | Columns |
|---|---|---|
| `VLV` | `VLV %5d %4d %3d %4d` | SN, dive, idx (message index), nSam (number of valve actions). |
| `vlv` | `vlv %5d %4d %3d %4d %10s %10d %10.2f %10.2f` | SN, dive, idx, action index (under `nSam`), pres (dBar, offset-corrected), valveTime (s), vac0 and vac1 (case vacuum before and after, inHg). |

`vac0` is `655.35` (65535 / 100, not sent) on current firmware; the message carries only the vacuum after the operation ([A.5.4](xmessage-format.md#a54-valve--0xeb)). Parser: `VLV` and `vlv` are skipped.

### 8.9 `PMP`, `pmp`: pump operations (0x60–0x6F)

Subsection `Pump Data`. Writer: `XCompositePump`, `XPump::GenerateListingString()`, `XPumpCounts::GenerateListingString()` (`src/data/xmsg/XPump.h`, `common/XPumpCounts.h`). Binary format: [A.5.3](xmessage-format.md#a53-pump--0x600x6f). Multiple messages are separated by a rule and a repeated header.

```
#      SN dive idx nSam       pres      pumpT      pumpV      pumpI       vac0       vac1
#                            (dBar)      (sec)        (V)        (A)     (inHg)     (inHg)
PMP  9303  483   0    6
pmp  9303  483   0    0      70.68         93       7.31      0.217     655.35       5.77
```

| Line | Format | Columns |
|---|---|---|
| `PMP` | `PMP %5d %4d %3d %4d` | SN, dive, idx (part index), nSam (number of pump actions). |
| `pmp` | `pmp %5d %4d %3d %4d %10s %10d %10.2f %10.3f %10.2f %10.2f` | SN, dive, idx, action index, pres (dBar, offset-corrected), pumpT (s), pumpV (V), pumpI (A), vac0, vac1 (inHg; vac0 `655.35` when not sent). |

Parser: `PMP` is skipped. `pmp` gives pressure (`x[5]`), time (`x[6]`), voltage (`x[7]`), current (`x[8]`) and vac1 (`x[10]`) by position.

### 8.10 `R`, `r`: rise (ascent) time series (0x50–0x5F)

Subsection `Rise Time-Series Data`. Same layout, variants and parsing as `F`/`f` (section 8.3), with tags `R` and `r`. Writer: `XCompositeRise`, `XRise::GenerateListingString()` (`src/data/xmsg/XCompositeRise.h`, `XRise.h`).

```
#      SN dive idx nSam                 time      pres phaseCode   phaseCodeName
#                                  (UTC/+sec)    (dBar)
R    9303  483   0   11 2026-10-19T13:57:58Z
r    9303  483   0    0                   +0    290.48         7 # start of ascent
r    9303  483   0    1                 +274    259.00        13 # ascending
```

### 8.11 `SFC`, `sfc`: surface time series (0x80–0x8F)

Subsection `Surface Time-Series Data` (its banner is printed by `XCompositeSurfaceData::GenerateListingString()`, `src/data/xmsg/XCompositeSurfaceData.h`). Binary format: [A.5.1](xmessage-format.md#a51-fall-0x400x4f-rise-0x500x5f-and-surface-0x800x8f-time-series). The surface series keeps the temperature and salinity columns. Its values are converted with the surface gains and offsets (`A` `midx` 5) and are never offset-corrected. Parts follow each other without a repeated header.

```
#      SN dive idx nSam                 time      pres    temp    saln phaseCode   phaseCodeName
#                                  (UTC/+sec)    (dBar)  (degC)  (PSU)
SFC  9303  483   0   21 2026-10-19T14:36:43Z
sfc  9303  483   0    0                   +0      0.13  29.218  36.727        14 # surface
```

| Line | Format | Columns |
|---|---|---|
| `SFC` | `SFC %5d %4d %3d %4d %20s` | SN, dive, idx (part), nSam, start time (UTC). |
| `sfc` | `sfc %5d %4d %3d %4d %+20d %9s %7.3f %7.3f %9d # %s` | SN, dive, idx, sample index, time offset (s), pres (dBar), temp (degC), saln (PSU), phaseCode, `#` phase name. |

Parser: `SFC` gives the reference time (`x[5]`); `sfc` gives offset, pres, temp and saln (`x[5]` to `x[8]`) by position.

### 8.12 `SFD`: surface derived fields

Subsection `Surface Time-Series Data - Derived Fields`, always printed after the surface series. Writer: `XCompositeSurfaceData::GenerateDerivedFieldsListingString()`.

```
#     SN dive midx nSam    presValid         presMin        presMean         presMax         presVar
#                                             (dBar)          (dBar)          (dBar)        (dBar^2)
SFD 9303  483    0   20            1           -0.05           -0.01            0.06           0.001
```

Format `SFD %4d %4d %4d %4d %12d %15s %15s %15s %15.3f`.

| Column | Format | Units | Meaning | Fill |
|---|---|---|---|---|
| SN | `%4d` | | Serial (one column narrower than elsewhere). | |
| dive | `%4d` | | | |
| midx | `%4d` | | Always 0. | |
| nSam | `%4d` | | Surface pressure samples used: valid samples within 2 standard deviations of the mean. | `-1` |
| presValid | `%12d` | | 1 if the statistics were computed. | |
| presMin, presMean, presMax | `%15s` | dBar | Statistics of the used samples, uncorrected. | `-9.999` |
| presVar | `%15.3f` | dBar² | Variance of the used samples (divided by gain² since 2026-10-24). | `-9.999` when the surface pressure gain is 0 or no statistics were computed |

Parser: skipped.

### 8.13 `WVS`, `wvs`, `wvm`: ocean surface wave spectra (0xC1, 0xC3)

Subsection `Ocean Surface Wave-Field Data`, printed when the dive has wave data with a non-zero segment length (WIMS floats). Writer: `XWave::GenerateListingString()` (`src/data/xmsg/XWave.cpp`). Binary format and the meaning of every column: [C.3](xmessage-format.md#c3-ocean-surface-waves-0xc1-0xc3), listing details [C.3.11](xmessage-format.md#c311-listing-output-wvs-wvs-wvm).

```
#      SN dive vers slen     fsam fbase fdiv     assf  again       essf
#                            (Hz)  (Hz)      (LSB/g^2)   (LSB/(g^2/Hz))
WVS  9303  483 0x02  256   5.0000   45    18     2048  4.000    35000.0
#---------------------------------------------------------------------------------------------------
#      SN dive  idx     freq         Szz         Syy         Sxx         Cxy         Qzx         Qzy
#                       (Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)
wvs  9303  483    0   0.0000    +0.00006    +0.00040    +0.00089    +0.00023    +0.00000    +0.00000
...
#---------------------------------------------------------------------------------------------------
#      SN dive  idx     freq         Szz         Syy         Sxx         Cxy         Qzx         Qzy
#                       (Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)
wvm  9303  483    0   0.0000  -1000.0000  -1000.0000  -1000.0000  -1000.0000  -1000.0000  -1000.0000
wvm  9303  483    1   0.0195     +0.0000   +642.1102   +884.4159   +133.2681    +12.1153     +0.0000
```

`WVS`, format `WVS %5d %4d 0x%02X %4d %8.4f %4d %5d %8d %6.3f %10.1f`:

| Column | Format | Units | Meaning |
|---|---|---|---|
| SN, dive | `%5d %4d` | | |
| vers | `0x%02X` | | Wave message version. |
| slen | `%4d` | | Segment length N. |
| fsam | `%8.4f` | Hz | Effective sample rate (doubled by the every-other-bin correction, [C.3.8](xmessage-format.md#c38-every-other-value-correction-wims-firmware)). |
| fbase | `%4d` | Hz | Base sample rate. |
| fdiv | `%5d` | | Rate divider + 1. |
| assf | `%8d` | LSB/g² | Accelerometer scale factor. |
| again | `%6.3f` | | Accelerometer gain (version 2). |
| essf | `%10.1f` | LSB/(g²/Hz) | Effective spectral scale factor. |

`wvs`, format `wvs %5d %4d %4d %8.4f` + six `%+11.5f`; `wvm`, format `wvm %5d %4d %4d %8.4f` + six `%+11.4f`:

| Column | Meaning |
|---|---|
| idx | Bin index 0 … N/2 − 1. Bin k has frequency k·fs/N; bin 0 is DC. |
| freq | Bin frequency (Hz); `-99.0000` for a bin not received. |
| Szz, Syy, Sxx, Cxy, Qzx, Qzy | Auto-spectra, co-spectrum and quadrature spectra, always in this order: acceleration in g²/Hz (`wvs`) or displacement in m²/Hz (`wvm`). |

- Fill: a component not received is `-999.99999` in `wvs`; in `wvm` it prints as `-1000.0000` (the same fill rounded by `%+11.4f`). The DC bin of `wvm` is the fill value. `wvm` values are clipped to ±999.9999 (`XWaveSpectralSample`, `src/data/xmsg/common/XWaveSpectralSample.h`).
- Only message versions 1 and 2 are listed; a dive whose wave data is all version 0 gets the subsection banner only.
- Variants (see [C.3.11](xmessage-format.md#c311-listing-output-wvs-wvs-wvm) and CHANGELOG.md): `essf` was `%8d` before build 3332; builds 3234–3289 printed `wvm` columns in the wrong order under this header; before 2026-10-26 bin k was labeled (k+1)·fs/N.
- Raw (non-spectral) wave mode is not listed (the code is commented out).

Parser: `WVS` gives slen (`x[4]`) and fsam (`x[5]`). `wvs` gives the six spectra (`x[5]` to `x[10]`) indexed by `x[3]`; the fill value becomes NaN. The frequency column is not read (the tool recomputes it), and `wvm` is ignored.

## 9. Engineering data: `E05` to `E0C`, `e0A`, `e0c`

Top-level section `Engineering Data`. One subsection per engineering message the dive has, in the order of section 4. Binary formats and the meaning of each field are in part B of the X-message specification; the listing tables there ([B.2.10](xmessage-format.md#b210-listing-output), [B.3.3](xmessage-format.md#b33-decoding-notes-and-listing), [B.4.4](xmessage-format.md#b44-decoding-notes-and-listing), [B.5.4](xmessage-format.md#b54-decoding-notes-and-listing), [B.6.8](xmessage-format.md#b68-listing-output-e0a-e0a), [B.7.4](xmessage-format.md#b74-decoding-notes-and-listing-e0c-e0c)) list the columns of every message version.

### 9.1 Common layout

- Tag: `E0` + the low hex digit of the X-message ID (`E05` = 0xE5, `E06` = 0xE6, `E07` = 0xE7, `E08` = 0xE8, `E09` = 0xE9, `E0A` = 0xEA, `E0C` = 0xEC). Error entries use the lower-case `e0A` and `e0c`.
- Each message is a sequence of lines numbered by `midx` from 0. Every line has its own column-name header (`#      SN dive midx …`), usually a units header, and often a `# --- <Name> ---` sub-group heading.
- Fields after `midx`: `midx` 0 starts with `engVer` (`0x%-02X`, the message version) for E07 to E0C, and with `BITVer` for E05/E06. Times are ISO 8601. Voltages and currents have three decimals, case vacuum two.
- Sensor values (drift averages, first and last profile samples, BIT readings) are converted with the profile gains and offsets and range-checked, but not offset-corrected. Each is listed in physical units and, where the message sends counts, again in counts under the same column name with a `(counts)` unit.
- Error lists: a count line (`nErr`), then a header `#      SN dive  idx            date-time        err-code` and one line per error: `e0A %05d %4d %4d %20s      0x%08X` (or `e0c`), giving the error index, its time and its code. Error codes are listed without names; see [B.9](xmessage-format.md#b9-system-error-codes).
- A message version the code does not list makes the listing code throw; the subsection then has its banner only and an error is logged.

### 9.2 `E05`, `E06`: built-in test passed / failed (0xE5, 0xE6)

Subsections `Engineering (BIT passed)` and `Engineering (BIT failed)`. Writers: `XEngBIT::GenerateListingString()` dispatching to `XAlamoBit` (formats 6–8, `src/data/xmsg/XAlamoBit.h`) or `XAlamoBit2` (formats 9–11, `src/data/xmsg/XAlamoBit2.h`). The tag is `E05` when the test passed and `E06` when it failed. Binary format: [B.2](xmessage-format.md#b2-built-in-test-messages-0xe5-passed-and-0xe6-failed).

Formats 6–8 (`E0%c %5d %4d %4d …`): `midx` 0 has BITVer (`%15d`), SWVer (`0x%08X`) and BITResults (`0x%08X`). Format 6 has only `midx` 0; formats 7 and 8 continue to `midx` 6 and 8 with the readings in [B.2.10](xmessage-format.md#b210-listing-output) (pressure, temperature and salinity in units and counts; CTD status; vacuum and battery; air valve, air pump, oil valve and oil pump tests under `# --- Air Valve (AV) Tests ---` etc.).

```
#      SN dive midx          BITVer           SWVer      BITResults
E06 11145   -1    0               8      0x09000000      0x00000010
```

Formats 9–11 (`E0%c %5d %4d %4d %6d …`): 14 lines, `midx` 0–13, each with `BITVer` after `midx`:

```
#      SN dive midx BITVer         FWVer    BITResults    sensorType   sensorTypeName
#
E05 11700  -12    0     11    0x0A0C0001    0x00000000             0 # RBR
# --- Date/Time ------------------------------------------------------------------------------------
#      SN dive midx BITVer                   startTime                     endTime      duration
#                                                                                          (sec)
E05 11700  -12    1     11        2026-09-29T03:13:51Z        2026-09-29T03:39:16Z          1525
# --- CTD ------------------------------------------------------------------------------------------
#      SN dive midx BITVer        errors        status    presPreCal    presOffset   presPostCal
#                                                             (dBar)        (dBar)        (dBar)
E05 11700  -12    2     11    0x00000000    0x73746F70          0.16          0.00          0.16
```

| midx | Sub-group | Columns |
|---|---|---|
| 0 | — | FWVer (`0x%08X`), BITResults (`0x%08X`), sensorType (`%13d`: 0 RBR, 1 SBE), `# sensorTypeName` (`RBR`, `SBE`, `unknown`) |
| 1 | Date/Time | startTime, endTime (`%27s`), duration (s) |
| 2 | CTD | errors, status (hex), presPreCal, presOffset, presPostCal (dBar) |
| 3 | (unnamed rule) | temp (degC), saln (PSU) |
| 4 | Case Vacuum | errors (hex), caseVacStart, caseVacEnd (inHg) |
| 5 | Battery Voltage | errors (hex), batVoltStart, batVoltEnd (V) |
| 6 | Air Valve Positions | open, mid, closed (V) |
| 7 | Air Valve Calibration Positions | open, mid, closed (V) |
| 8 | Air Pump | errors (hex), duration (s), cycles, presMax (PSI) |
| 9 | (unnamed rule) | currAvg, currMax, 12VCurrMax (A), batMax (V), powerMax (W) |
| 10 | Oil Valve Positions | open, closed (V) |
| 11 | Oil Valve Calibration Positions | open, closed (V) |
| 12 | Oil Pump | errors (hex), duration (s) |
| 13 | (unnamed rule) | currAvg, currMax, 12VCurrMax (A), batMax (V), powerMax (W) |

Column names repeat between sub-groups (`errors`, `open`, `currAvg`), so a reader must track the latest `# --- <Name> ---` heading; the unnamed rules continue the previous group. Parser: header-based (section 16.3), with keys such as `Air Pump/currAvg`.

### 9.3 `E07`: descent (0xE7)

Subsection `Engineering (Descent)`. Writer: `XEngDescent::GenerateListingString()` (`src/data/xmsg/XEngDescent.h`). Binary format and listing: [B.3](xmessage-format.md#b3-descent-engineering-0xe7).

| Version | midx | Columns (format) |
|---|---|---|
| 0x10 | 0 | engVer, start-of-descent (`%20s`), pumpT (`%5d` s), pumpI, sysI (`%6.3f` A), sysV (`%6.3f` V), vac (`%6.2f` inHg), pumpAdj (`%+7d` s) |
| 0x11 | 0 | engVer, start-of-descent, vac0, vac1 (`%9.2f` inHg), seeks (`%15d`), seekPumpAdjT (`%+15d` s) |
| 0x11 | 1 (`Valve Position Measurements`) | OilValveOpenPosSurf, OilValveClosePosFallPump (`%24.3f` V), AirValveMidPosSurf (`%29.3f` V) |

```
E07 09303  483    0   0x11 2026-10-18T08:49:00Z     10.04      6.10               1              +0
E07 09303  483    1                    1.735                    0.701                         0.643
```

### 9.4 `E08`: drift/park (0xE8)

Subsection `Engineering (Park/Drift)`. Writer: `XEngDrift::GenerateListingString()` (`src/data/xmsg/XEngDrift.h`). Binary format: [B.4](xmessage-format.md#b4-driftpark-engineering-0xe8).

| Version | midx | Columns (format) |
|---|---|---|
| 0x10 | 0 | engVer, start-of-drift, meanP (`%7s` dBar), meanT (`%7.3f` degC), start-of-fall (`%20s`), fallP (`%7s` dBar) |
| 0x11, 0x12 | 0 | engVer, start-of-drift, vac0, vac1 (inHg), seeks, seekPumpAdjT (`%+15d` s) |
| 0x11, 0x12 | 1 (`Drift Sensor Averages`) | pres (`%12s` dBar), pres (`%12d` counts), temp (`%12.3f` degC), temp (counts), saln (`%12.3f` PSU), saln (counts) |

```
E08 09303  483    0   0x12 2026-10-18T12:57:55Z      6.09      6.13               0              +0
E08 09303  483    1          289.04         7476       19.056        24056       36.688        37688
```

### 9.5 `E09`: ascent (0xE9)

Subsection `Engineering (Ascent)`. Writer: `XEngAscent::GenerateListingString()` (`src/data/xmsg/XEngAscent.h`). Binary format: [B.5](xmessage-format.md#b5-ascent-engineering-0xe9). `E09` lines pad the serial with spaces (`%5d`), unlike E07, E08, E0A and E0C.

| Version | midx | Columns (format) |
|---|---|---|
| 0x10 | 0 | engVer, start-of-ascent, sysI (`%9.3f` A), pumpT (`%9d` s) |
| 0x11 | 0 | engVer, start-of-ascent, vac0, vac1 (`%9.2f` inHg) |
| 0x12 | 0 | as 0x11 |
| 0x12 | 1 (no sub-group heading) | start-of-fall (`%20s`), fallP (`%9s` dBar) |

```
E09  9303  483    0   0x12 2026-10-19T13:57:58Z      6.12      5.84
E09  9303  483    1 0000-00-00T00:00:00Z      0.00
```

### 9.6 `E0C`, `e0c`: ice detection (0xEC)

Subsection `Engineering (Ice)`. Writer: `XEngIce::GenerateListingString()` (`src/data/xmsg/XEngIce.h`). Binary format: [B.7](xmessage-format.md#b7-ice-detection-engineering-0xec).

| midx | Sub-group | Columns (format) |
|---|---|---|
| 0 | — | engVer, mode (`%8d`) |
| 1 | Status | ice-status, urgency, surfAttempts, numStoredSBD (`%15d`), lastXmit (`%15d` s) |
| 2 | — | minPres (`%15s` dBar), testTemp (`%15.3d`, raw value; the header says degC), testSalinity (`%15.3d`, PSU) |
| 3 (0x11 only) | Errors | nErr (`%15d`), then `e0c` lines |

```
E0C 09213    8    2            0.16               0             -10
e0c 11700    1    0 2026-09-30T12:23:48Z      0x00000615
```

`%15.3d` on a floating-point value prints three significant digits, so the salinity fill `-9.999` appears as `-10` ([B.7.4](xmessage-format.md#b74-decoding-notes-and-listing-e0c-e0c)). An ice message alone does not make the Engineering Data section appear (section 17).

### 9.7 `E0A`, `e0A`: surface (0xEA)

Subsection `Engineering (Surface)`. Writer: `XEngSurface::GenerateListingString()` (`src/data/xmsg/XEngSurface.h`). Binary format: [B.6](xmessage-format.md#b6-surface-engineering-0xea). The `midx` layout of every version is the table in [B.6.8](xmessage-format.md#b68-listing-output-e0a-e0a). Version 0x14, the current one:

| midx | Sub-group | Columns (format) | Units |
|---|---|---|---|
| 0 | — | engVer, start-of-surface (`%20s`), vac0, vac1 (`%9.2f`), nSBD, nPrevAttempt (`%15d`) | inHg |
| 1 | Air Pump & Bladder | APCur (`%15.3f` A), APRunT (`%15d` s), AirBladMxP (`%15.3f` PSI) | |
| 2 | Air Valve Position | AVOpenStart, AVMidEnd, ACVpenCal, AVMidCal, AVCloseCal (`%15.3f`) | V |
| 3 | Oil Valve Position | OVOpenAirPmp, OVCloseEnd, OVOpenCal, OVClosCal (`%15.3f`) | V |
| 4 | Surface Pressure | presOffset (`%15s`): the float's own surface pressure offset reading, not the XMTPS offset of `PRC` | dBar |
| 5 | Profile - First Sample | firstPres (dBar, counts), firstTemp (degC, counts), firstSaln (PSU, counts) | |
| 6 | Profile - Last Sample | lastPres, lastTemp, lastSaln, each in units and counts | |
| 7 | Profile - Meta | numBins, numEmptyBins, numRawSamples (`%15d`) | |
| 8 | Real-Time Clock | gpsOffset (`%15d`) | s |
| 9 | Surface Sampling | start, end (`%20s`) | UTC |
| 10 | Errors | nErr (`%15d`), then `e0A` lines | |

```
E0A 09303  483    0   0x14 2026-10-19T14:36:17Z      5.84      9.94              12              13
E0A 09303  483    9 2026-10-19T14:36:18Z 2026-10-19T14:38:36Z
E0A 09303  483   10               3
#      SN dive  idx            date-time        err-code
e0A 09303  483    0 2026-10-19T14:15:58Z      0x00000114
```

Version differences, in short (details in B.6.8):

- 0x10: a single `midx` 0 line with engVer, start-of-surface, CTD-status (`0x%08X`), nSBD, nAtmpt, vac, sysV, sysI, nErr; then `e0A` lines with `0000-00-00T00:00:00Z` times. The `nErr` column is printed from a `uint8_t`, so boost::format writes the raw byte (NUL for 0) instead of a number (section 17).
- 0x11: `midx` 0–3 as above (`midx` 1 uses `%20` widths), then the error count on an `e0A` line at `midx` 4.
- 0x12: adds Surface Pressure at `midx` 4; error count on an `e0A` line at `midx` 5.
- 0x13: adds first/last sample, meta and clock (`midx` 5–8); error count on an `E0A` line at `midx` 9.
- 0x14: adds Surface Sampling (`midx` 9); error count at `midx` 10.
- The header label `ACVpenCal` is a typo for the air valve open calibration position; the parser keys on the typo.

### 9.8 Engineering parser notes

`xmtps-list2json.py` parses every `E0x`/`e0x` line through the latest engineering header and units line (section 16.3). It does not depend on positions, so new columns are kept (in `engineering.extra`) without code changes. Lines whose field count does not match the header are dropped with a debug message.

### 9.9 SOLO-II engineering subsections

0xE0 (diagnostic dive), 0xE2 (standard dive) and 0xE3 (abort) are validated but not decoded ([B.1](xmessage-format.md#b1-solo-ii-heritage-engineering-messages-0xe0-0xe2-0xe3)). If such a message is assigned to the dive, its subsection (`Engineering (Diagnostic Dive)`, `(Standard Dive)`, `(Abort)`) is printed with no lines: their `GenerateListingString()` members are empty (`src/data/xmsg/XEngDiagnostic.h`, `XEngStandard.h`, `XEngAbort.h`). BIT messages in a format neither BIT class handles also give an empty subsection.

## 10. Command echo-back: `CMD`

Top-level section `Ground Station Upload Command Echo-Back Messages`, printed when the dive has at least one 0xDE blob. Writer: `XMissionCmdEcho::GenerateListingString()` (`src/data/xmsg/XMissionCmdEcho.h`). Binary format and decoding: [C.2](xmessage-format.md#c2-mission-command-echo-0xde).

```
#      SN dive eidx cidx  command
CMD  9245  287    0    0  WCS 36 98
CMD  9245  287    0    1  WCG -1
CMD  9245  287    0    2  WCV
CMD  9245  287    0    3  MIC 1
```

Format `CMD %5d %4d %4d %4d  %s`.

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| eidx | `%4d` | Index of the echo-back message (0xDE blob) within the dive, in decoding order. |
| cidx | `%4d` | Index of the command within the echo. |
| command | `%s` | The command text, the rest of the line. The parameter name (text before the first space) is upper-cased; the rest is as received. Non-printable bytes appear as `\xNN`. |

- The text is the upload split at `;`; the closing `MIC 1` is listed like any other command.
- Variants: before build 3310 the section was printed with its header only (the echo was not decoded); before 2026-10-26 the parameter name kept the upload's case.

Parser: not read.

## 11. Extended records: `XFF`

Top-level section `Extended Record Messages (0xFF)`, printed when the dive has at least one 0xFF blob (build 3321 and later, per [C.6](xmessage-format.md#c6-extended-records-0xff); before, 0xFF was logged as unknown and dropped). Writer: `XExtendedRecord::GenerateListingString()` (`src/data/xmsg/XExtendedRecord.h`). Format inferred: [C.6](xmessage-format.md#c6-extended-records-0xff).

```
#      SN dive ridx type  ver  record
XFF  9308   -7    0 0xAE 0x11  serial 9308  config 0x02 0x02 0x01
XFF  9300    0    0 0xCA 0x20  event 0x004002  time 2021-07-30T20:56:08Z  value 0.1091
XFF  9300    0    1 0xD1 0x10  counter 9  time 2021-07-30T20:56:01Z  count 1  time +215s  value 0
```

Format `XFF %5d %4d %4d 0x%02X 0x%02X  %s`.

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| ridx | `%4d` | Record index within the dive. |
| type | `0x%02X` | Record type (blob byte 3). |
| ver | `0x%02X` | Record version (byte 4). For type 0xDB this byte is the high byte of the command length. |
| record | `%s` | Decoded record, the rest of the line (below). |

| Type (version) | `record` text |
|---|---|
| 0xCA (0x20, 11 bytes) | `event 0x%06X  time <t>  value %.4f` |
| 0xD1 (0x10, 16 bytes) | `counter %d  time <t>  count %d  time <t>  value %u` |
| 0xAE (0x10, 0x11) | `serial %d  config` + ` 0x%02X` per configuration byte |
| 0xDB | `command "<text>"  reply "<text>"` |
| 0xFA | `text "<text>"`, or `(empty)` if all NUL |
| other | `hex` + ` %02X` per record byte |

- `<t>` is an ISO 8601 time when the value is UNIX seconds on or after 2000-01-01; smaller values are seconds since controller start, printed `+<n>s`.
- `<text>` is on one line: CR and LF as `\r` and `\n`, `"` and `\` escaped with `\`, other non-printable bytes as `\xNN`, trailing NULs dropped. The text can contain `#` and `;`.

Parser: not read.

## 12. WIMS configuration parameters: `WIP`

Top-level section `WIMS Configuration Parameters (0xC5)`, printed when the dive has at least one 0xC5 blob. Added in build 3336 (2026-10-29); production listings, last written by build 3332, have no `WIP` lines yet. Writer: `XWimsParameters::GenerateListingString()` (`src/data/xmsg/XWimsParameters.h`). Format and parameter names, mostly inferred: [C.7](xmessage-format.md#c7-wims-configuration-parameters-0xc5).

```
#      SN dive midx  ver part pidx  name         value     hex
WIP  9297   15    0    2    0    2  cal.acofx     -213  0xFF2B
WIP  9297   15    0    2    0   25  -             2658  0x0A62
```

Format `WIP %5d %4d %4d %4d %4d %4d  %-11s %6d  0x%04X`.

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| midx | `%4d` | Index of the 0xC5 message within the dive. |
| ver | `%4d` | Message version (1: 42 parameters, 2: 43). |
| part | `%4d` | Part index (byte 4). |
| pidx | `%4d` | Parameter index (the float's `WCG`/`WCS` index). |
| name | `%-11s` | WIMS parameter name; a trailing `?` marks an inferred name; `-` if unknown. |
| value | `%6d` | Value, signed for calibration coefficients, unsigned otherwise. |
| hex | `0x%04X` | The raw 16-bit value. |

Example source: [C.7](xmessage-format.md#c7-wims-configuration-parameters-0xc5) (decoded from RAW files; not yet in a production listing). Parser: not read.

## 13. Mission configuration: `MIS`, `m`, `mis`

Top-level section `Mission Configuration`, always printed. Writer: `XMfaMissionCfg::GenerateListingString()` and its per-layout `GenerateListingStringFw<version>()` functions (`src/data/xmsg/XMfaMissionCfg.cpp`), from the dive's mission configuration messages (0xD0, 0xD1). Binary format, supported firmware and the full list of parameters: [C.1](xmessage-format.md#c1-mission-configuration-0xd0-0xd7); listing styles: [C.1.11](xmessage-format.md#c111-listing-output).

### 13.1 No mission configuration

If the dive has no mission configuration message (firmware word still 0xFFFFFFFF), the section is:

```
# Incomplete mission parameters.
```

Most dives look like this: floats send their mission configuration only in some dives (pre-mission dives, dive 0, after an upload, or with `APT`).

For a firmware word the code does not support, the release build throws (`Unsupported ALAMO Mission Parameters message version`), and the section has its banner only.

### 13.2 `MIS` and `m` (firmware 4.5.63, 5.0.61 and later)

```
#      SN dive        CfgMsgVer       FWVer sensor-type        sensor-type-name
#
MIS 11700  -12             0x18  0x0A0C0001           0      # RBR
# --- General --------------------------------------------------------------------------------------
#      SN dive idx param      value      units          desc
#      -- ---- --- -----      -----      -----          ----
m   11700  -12   0 MSN            0      [-]          # dive cycle number
m   11700  -12   3 MDS         2100      [dBar]       # maximum depth
```

`MIS`, format `MIS %5d %4d             0x%02X  0x%08X %11d      # %s`:

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| CfgMsgVer | `0x%02X` | Mission parameters message version (D0 offset 8). |
| FWVer | `0x%08X` | Firmware version word `0xMMmmppbb` ([How to read](xmessage-format.md#how-to-read-this-document)). |
| sensor-type | `%11d` | CTD type: 0 RBR, 1 SBE; other values are listed as is. |
| `# sensor-type-name` | `%s` | `RBR`, `SBE` or `unknown`. |

`m`, format `m   %5d %4d ` + `%3d %-5s <value> %-12s # %s`:

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| idx | `%3d` | Running index of `m` lines in listing order, from 0. Not the parameter's position in the message. |
| param | `%-5s` | Parameter mnemonic, upper case, with a phase digit where the parameter has one (`DFT0`, `CPP1`). |
| value | `%10d` (integers), `%12.1f`, `%13.2f`, `%14.3f`, `%15.4f` (scaled values) | Parameter value in the units of the next column. |
| units | `%-12s` | Units in square brackets (`[dBar]`, `[sec]`, `[-]`, `[LSB@12V]`). |
| `# desc` | `%s` | Description. |

- Parameters are grouped under `# --- <Group> ---` headings (General, Calibration Coefficients, Start-of-Mission Trigger, Diagnostic Dive, …), each followed by the two-line header shown above. The groups and their order depend on the firmware layout ([C.1.11](xmessage-format.md#c111-listing-output)). A 10.12.0.x listing has 177 `m` lines.
- `CPN`/`CPP` binning values depend on the sensor type (scaled for RBR, raw dBar for SBE).

Parser: `MIS` gives `cfgver`, `fwver` and `ctdtype` (found by locating the two hex fields, so the older form with an index also works). Each `m` line gives name (`x[4]`), value (`x[5]`), units (`x[6]` without brackets) and index (`x[3]`), stored with the current sub-group name in `mission.entries` and by name in `mission.parameters`; the description is not kept.

### 13.3 `mis` groups (firmware 2.0.62 to 4.6.61)

Older firmware layouts list the parameters in fixed-column groups, one `mis` line per group, with a minor index:

```
#      SN dive  idx        CfgMsgVer       FWVer sensor-type        sensor-type-name
#
MIS 10108    3    0             0x13  0x04066100           1      # SBE
# --- General --------------------------------------------------------------------------------------
#      SN dive  idx midx         MSN         MDS         APT        XAS
#                                          (dBar)                       
mis 10108    3    0    0           2         500           1          2
```

- Layouts 2.0.62–2.5.62, 3.x and 4.1.61 (`Fw02006200`, `Fw02006900`): no `MIS` line; a header line `mis %5d %4d %4d       0x%08X` (SN, dive, idx = 0, firmware word), then the groups.
- Layouts 4.2.62, 4.3.61, 4.4.61, 4.6.61 (`Fw04026200`): `MIS %5d %4d %4d             0x%02X  0x%08X %11d      # %s`, with an `idx` column (always 0), then the groups.
- Group lines: `mis %5d %4d %4d %4d` (SN, dive, idx, midx) followed by up to six `%11d`/`%11.Nf` values whose names and units are in the two header lines above each group.

Parser: `MIS` is read as in 13.2; `mis` lines are not read.

## 14. SBD hex dump: `SHX`, `shx`

Top-level section `SBD Message Hex Data`, printed when the dive has a mail message, always last. One block per mail message, in MOMSN order. Writer: `XFloatMailMessage::GenerateSbdHexListingString()` (`src/data/XFloatMailMessage.h`). The bytes are the X packet ([A.1.2](xmessage-format.md#a12-x-packet-x-nn-mm-dd-p-data--c-c-)), the same as the SBD file ([sbd-file-format.md](sbd-file-format.md)).

```
#      SN dive   size MOMSN MTMSN PID
#                 (b)
SHX  9303  483    287  6328     0  11
#---------------------------------------------------------------------------------------------------
#      SN dive MOMSN idx PID               | 0x  0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F
#                                          |    00 01 02 03  04 05 06 07  08 09 10 11  12 13 14 15
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
shx  9303  483  6328   0  11 0x0000 (0000) | 0x 58 01 18 24  57 01 E3 0B  80 20 C5 32  68 E7 7B 00
#                                          |     X  .  .  $   W  .  .  .   .     .  2   h  .  {  .
...
#---------------------------------------------------------------------------------------------------
```

`SHX`, format `SHX %5d %4d %6d %5d %5d %3d`: SN, dive, size (bytes), MOMSN, MTMSN, PID.

`shx`, format `shx %5d %4d %5d %3d %3d 0x%04X (%04u) | 0x` followed by up to 16 bytes as ` %02X`, with an extra space before bytes 4, 8 and 12:

| Column | Format | Meaning |
|---|---|---|
| SN, dive | `%5d %4d` | |
| MOMSN | `%5d` | MOMSN of the message. |
| idx | `%3d` | Line index within the message, from 0. |
| PID | `%3d` | Packet ID. |
| offset | `0x%04X (%04u)` | Offset of the line's first byte, hex and decimal. |
| `| 0x` | literal | Separator. |
| bytes | ` %02X` × ≤16 | Packet bytes. |

Each `shx` line is followed by a comment line showing the same bytes as characters (printable ASCII as is, others as `.`), aligned under the hex. The second header line (decimal byte positions) was added after build 3310.

Parser: `SHX` and `shx` are skipped.

## 15. Older listings in production

Production listings are not regenerated when the decoder changes, only when their float is reprocessed. On 2026-10-30 the 86,109 files on argus came from 67 builds, from 0.04.2230a (2014) to 1.0.3332b; 31,742 are from 0.6.3067b (2022) and 18,349 from 1.0.3332b. Readers of old listings meet these differences (from CHANGELOG.md and the production files):

| Feature | Old form | Changed |
|---|---|---|
| Version line | `X    2.1    0.04.2230a      DEBUG 2014-08-29T11:18-04:00` (tag `X`, four fields) | 2014 (`XSV`) |
| File start | A UTF-8 box-drawing "XMTPS" splash banner before the title; title `Intermediate Listing for Float 9035: Dive 1`; `Local Path` unquoted and relative | 2014 |
| IMEI | Full 15-digit IMEI on `LIS`/`SBD` | 2017-08-02 (last seven digits) |
| `SBD` position | Six decimals | early builds |
| Surface pressure series | `SFP` / `sfp` lines under "Surface Pressure Time-Series Data", pressure and phase code only (9035, 2014) | replaced by `SFC`/`sfc` |
| Mission configuration (2014) | `M SN dive idx` header and `m` lines with fixed-column groups (`m SN dive idx midx MSN DTD MDS …`) | replaced by `mis` and the current `MIS`/`m` |
| Iridium position | `IRI` line under "Position Data / Iridium Position": weighted-average time and position, nPos, variance (km²), stdError (km), satTime (s) | removed 2026-10-27 |
| Unbinned profile | A different line tag | `pru` since 2017 (CHANGELOG.md; the older tag is not recorded) |
| Pressures | Three decimals | two decimals since 2026-10-24 |
| `PRD`/`prd`, drift times, `salnCorr` | absent | 2026-10-24 |
| `PRC` | Ends at `presOffset` | sfc statistics added 2026-10-25/26 (after build 3310) |
| `f`, `r` | `temp` and `saln` columns before `phaseCode` | removed in version 1.0 (2026-10-26) |
| `CMD` | Section banner only | decoded since build 3310 |
| `XFF` | absent (0xFF dropped) | build 3321 |
| `WIP` | absent | build 3336 |
| `APS` SNROut | Repeats SNRIn | fixed in build 3309 |
| `wvm` | Columns in xx, xy, yy, zz order under the zz-first header | builds 3234–3289 |
| `E0A` 0x10 | NUL or control character in the `nErr` column (3,085 files, all 2016–2023) | still present in the code (section 17) |
| `XSV` version | `0.6.<build>` | `1.0.<build>` since 2026-10-26 |

The example in the repository, `example/listing/lis9303-0440.txt`, was written by 0.6.3310a: it has `f`/`r` temperature and salinity columns, a `PRC` line without the surface statistics, and no decimal-position row in the hex dump headers.

## 16. How `xmtps-list2json.py` reads a listing

### 16.1 General

- `ProcessListingFile()` (`bin/py/xmtps-list2json.py`) reads the whole file as UTF-8 (strict), strips LF, skips blank lines, and dispatches each line by a regular expression on its tag. Every data line is split on whitespace; most tags are read by field position.
- Comment lines are not stored, but four kinds set parser state:
  1. Data-table column headers matching `^#\s+SN\s+dive\s+(sampleIdx|idx\s+nSam|nSam)\s`: the names are kept for `pro` and `d` lines (`ColumnValues()`), which are read by name.
  2. Engineering headers matching `^#\s+SN\s+dive\s+(midx|idx)\s`, and the comment line right after them as the units line (unless it is a rule).
  3. Sub-group headings `# --- <Name> ---`.
  4. `# Engineering…` and `# Mission Configuration` titles reset the sub-group name.
- A line that raises an exception is logged as an error and skipped (`-H` makes it fatal). Lines with an unhandled tag are logged at debug level as unknown.

### 16.2 Tags

| Tag | Read | Fields used |
|---|---|---|
| `XSV` | skipped | — |
| `LIS` | yes | SN, dive, DAC-ID, WMO-ID; triggers the up-to-date check |
| `SBD` | yes | size, MOMSN, MTMSN, PID, date/time, position, CEP |
| `GPS` | yes | idx, date/time, position, TTF, nSat, SNR ×3, HDOP |
| `APS` | no (unknown) | — |
| `IRI` (old) | skipped | — |
| `A` | skipped | — |
| `PRC` | yes | nSam, nDive, valid, presOffset |
| `F`, `R`, `SFC` | yes | start time |
| `f`, `r` | yes | time offset, pres, phase code (field before `#`) |
| `sfc` | yes | time offset, pres, temp, saln |
| `D` | yes | nSam, drift start time if present |
| `d` | yes, by header name | pres, temp, saln, time, index |
| `PRD`, `prd` | skipped | — |
| `PRO` | yes | numSamples |
| `pro` | yes, by header name | pres, temp/temp1, temp2, saln, salnCorr |
| `PRU`, `pru` | no (unknown) | — |
| `VLV`, `vlv` | skipped | — |
| `PMP` | skipped | — |
| `pmp` | yes | pres, pumpT, pumpV, pumpI, vac1 |
| `SFD` | skipped | — |
| `WVS` | yes | slen, fsam |
| `wvs` | yes | idx, six spectra (fill → NaN) |
| `wvm` | skipped | — |
| `E05`–`E0C`, `e0A`, `e0c` | yes, by header name | all columns (section 16.3) |
| `CMD`, `XFF`, `WIP` | no (unknown) | — |
| `MIS` | yes | CfgMsgVer, FWVer, sensor type |
| `m` | yes | idx, name, value, units, sub-group |
| `mis` | no (unknown) | — |
| `SHX`, `shx` | skipped | — |
| `X`, `SFP`, `sfp`, `M` (2014) | no (unknown) | — |

So the machine-relied-upon parts of the format are: the tag names; the field positions of `LIS`, `SBD`, `GPS`, `PRC`, `F`/`f`, `R`/`r`, `SFC`/`sfc`, `D`, `PRO`, `pmp`, `WVS`, `wvs`, `MIS`, `m`; the column-header text above `pro` and `d` lines; and the header, units and sub-group comment lines above engineering lines.

### 16.3 Engineering lines

`ParseEngineeringLine()`:

- Field names are the header tokens after `SN dive midx` (or `idx`). Units are matched to names by the right edge of their text in the units line (within 2 characters), with parentheses removed (`HeaderFields()`).
- Values are the line's fields after `midx`. If the line has a `#` comment and one field fewer than the header, the comment is the last value (BIT `sensorTypeName`).
- Each value goes to the attribute given by `ENG_COLUMNS` under the key `<sub-group>/<name>`, then `<name>|<unit>` (e.g. `pres|counts`), then `<name>`. Unmatched columns go to `engineering.extra[<section>]`.
- `e0A`/`e0c` lines become entries of `engineering.errors` with `section`, `idx`, `dt_str` and `code`.
- The section is chosen by tag: E05/E06 `bit`, E07 `descent`, E08 `drift`, E09 `ascent`, E0A `surface`, E0C `ice`.

## 17. Open questions / discrepancies

Found while writing this document (code = source tree as of 2026-10-30; production = argus listings):

1. **E0A version 0x10 `nErr` is written as a raw byte.** `XEngSurface::GenerateListingString()` passes `num_errors()` (a `uint8_t`) to a `%4d` field without an `(int)` cast; boost::format streams it as a character, so 0 errors writes a NUL and 2 errors a 0x02 byte. Versions 0x11–0x14 cast to `int`. In production, 3,085 listings (floats 9004–9043, written 2016–2023) contain NUL bytes for this reason, which makes `grep` treat them as binary. The parser stores the byte as a string in `surface.num_errors`. Suggested fix: `% (int) num_errors()`.
2. **An ice-detection message alone does not produce an Engineering Data section.** `HasEngineeringData()` (`src/data/XFloatDiveData.cpp`) tests every engineering message except `xmsg_engineering_data_ice_`, although `GenerateListingString()` has an `Engineering (Ice)` subsection. A dive whose only engineering message is 0xEC (possible during ice avoidance, when the float stays under ice and sends little) would not list it [inferred; not observed]. `HasEngineeringData()` also does not count valve data, which is listed under Dive Data, so that is consistent.
3. **Argo, valve and pump data are not listed until other dive data arrives.** `HasDiveData()` requires drift, a complete profile, fall/rise, surface or wave data. A partial dive with only an Argo message, valve or pump packets, or an incomplete profile gets no Dive Data section (and no `PRC` line), although the README says the listing "always reflects the messages received so far". By the time a dive is complete this does not matter.
4. **`prd` pressure description is fixed text.** The `pres` description always says "Bin pressure, less the surface pressure offset (see Pressure Correction)", including in listings where no correction was applied (SBE floats, `poc.lis = 0`, or no valid offset). The Pressure Correction note is correct; the `prd` text is not.
5. **README "Layout rules" overstate the time-series rule.** The README says `D`/`d` give an absolute start time and `+seconds` per row; that holds only for floats that send drift sample times. The README line-type table also describes `D`/`d` as "time, pressure, temperature, salinity" without noting the no-times layout. It also says every data line continues with serial and dive, which `XSV` does not.
6. **README gives build 3321 for the `f`/`r` temperature/salinity removal.** CHANGELOG.md puts it under 2026-10-26 (version 1.0); production has no 1.0 build between 3316 and 3327 to confirm the exact build. The difference matters only for reading old listings, which the parser handles either way.
7. **The repository example listing is out of date.** `example/listing/lis9303-0440.txt` (0.6.3310a) differs from current output in the `PRC` line, the `f`/`r` columns, the `XSV` version and the `shx` header. The README's own excerpts (from `lis9287-0325.txt`) already show the current `f` layout.
8. **Serial-number padding is inconsistent.** `E07`, `E08`, `E0A`, `E0C`, `e0A`, `e0c` use `%05d`; `E05`, `E06`, `E09` and every other tag use `%5d`; `SFD` uses `%4d`. Harmless for whitespace splitting, but a column-position reader would break.
9. **Header typo `ACVpenCal`** (E0A `midx` 2) should be `AVOpenCal`. The parser's `ENG_COLUMNS` keys on the typo, so fixing the header requires changing the parser at the same time.
10. **`E0C` and BIT format 8 use `%15.3d` on floating-point values**, which prints three significant digits (`-10` for the fill `-9.999`, [B.7.4](xmessage-format.md#b74-decoding-notes-and-listing-e0c-e0c), [B.2.10](xmessage-format.md#b210-listing-output)). `testTemp` is the raw value under a `(degC)` header.
11. **`GPS` `stat` column is a constant 0**, and `idx` is the message type, not an index. Both are as coded; the header names suggest otherwise.
12. **SOLO-II engineering messages give empty subsections** (section 9.9). If such a message is ever decoded, the listing shows a banner with no data.
13. **The parser ignores several listed data types**: `PRU`/`pru` (unbinned profiles), `APS`, `CMD`, `XFF`, `WIP` and `mis` are logged as unknown lines at debug level. The parser's docstring lists the skipped tags but not these. Unbinned profiles in particular never reach the JSON files.
14. **`Local Path` documentation.** `XFloatDiveData.h` describes the listing path as relative to the listing directory; it is `dir.lis` joined with the file name, absolute in production.
15. **MOMSN wrap-around.** SBD lines and hex blocks are sorted by MOMSN; a dive whose messages straddle the 65535 → 0 wrap would list them out of order [inferred; not observed].
16. **No `WIP` lines in production yet.** The production decoder is build 3332; `WIP` needs build 3336. The `WIP` example here is from the X-message specification, not from a listing.
