# Float Metadata File Format

This document specifies the float metadata (`.meta`) files that the XMTPS decoder reads at startup. The Float Group writes them by hand, one per float, in the plain-text layout of the older Argo/SOLO metadata files; the decoder reads a subset of the fields and ignores the rest. Other tools (the Argo DAC submissions, MATLAB summaries in the same directory) also use these files [inferred].

Status: compiled on 2026-10-30 from `src/data/XFloatMetadata.cpp`, `src/data/XFloatMetadata.h` and `src/data/XFloat.cpp` at commit `8643acf`, and from the 299 entries in the production directory `/data1/alamo/meta/dac` on `argus.whoi.edu`.

## Contents

1. [Location and file names](#1-location-and-file-names)
2. [Line syntax](#2-line-syntax)
3. [Fields the decoder reads](#3-fields-the-decoder-reads)
4. [Fields the decoder ignores](#4-fields-the-decoder-ignores)
5. [Placeholder files and acceptance rules](#5-placeholder-files-and-acceptance-rules)
6. [Error handling](#6-error-handling)
7. [What the metadata is used for](#7-what-the-metadata-is-used-for)
8. [Examples](#8-examples)
9. [Open questions / discrepancies](#9-open-questions--discrepancies)

## 1. Location and file names

The directory is the `dir.meta` option (default `meta`; production `/data1/alamo/meta/dac` [inferred], see [configuration-file-format.md](configuration-file-format.md)). It must exist; otherwise startup fails with `Invalid metadata path: <path>`.

`XFloatMetadata::ListMetadataFiles()` lists the regular files directly in that directory (no subdirectories) and keeps those whose full path matches the POSIX-extended expression:

```
.*/[0-9]{4,5}_[0-9]{4,6}\.meta
```

That is, the file name is `<DAC-ID>_<serial>.meta`:

| Part | Form | Example |
|---|---|---|
| DAC ID | 4 or 5 digits | `6033`, `10293`, `9999` |
| `_` | literal | |
| Float serial number | 4 to 6 digits, zero-padding allowed | `9078`, `011682` |
| `.meta` | literal, lower case | |

Consequences:

- The name only selects the file. The DAC ID and serial number that the decoder uses come from the file's contents (`internal ID number`, `float serial number`), not from the name.
- Names such as `alamo_template.meta`, `NEWTemplate_6XXX_9XXX.meta`, `6062_9130.meta.sb-a0222412-1hD9O1` and the macOS resource files `._6008_9030.meta` do not match and are skipped.
- Files in subdirectories (`defunct/`, `recycled_ids/` in production) are not read.
- The order in which files are read is the directory order, which is unspecified.

Files are read once, at startup (`XFloat::LoadMetadataFromFiles()`, called from `Initialize()` in `src/Main.cpp`). Changes take effect at the next restart.

## 2. Line syntax

The file is read line by line by `operator>>(istream&, XFloatMetadata&)` in `src/data/XFloatMetadata.cpp`:

1. A line shorter than 40 characters is skipped. This covers blank lines and short separators; the `========================================` separator line is exactly 40 characters and becomes a field named `========================================` with an empty value, which is then skipped by rule 3.
2. Characters 1 to 40 are the field name, and characters 41 to the end of the line are the value. Both are trimmed of leading and trailing whitespace (`boost::algorithm::trim`, which also removes a trailing CR, so CRLF line endings work).
3. A line with an empty value is skipped.
4. The field name is compared to a fixed list of patterns (section 3) with `regex_match` (whole name, case-sensitive). The first pattern that matches decides how the value is parsed.
5. A field name that matches no pattern is collected as unsupported; one warning lists all of them at the end of the file (section 6).

Layout rules that follow:

| Rule | Detail |
|---|---|
| Column 41 | The value must start at column 41 or later. Production files pad the field name with spaces to exactly 40 columns. |
| Characters, not display columns | The split counts bytes. A tab counts as one column, and a multi-byte UTF-8 character counts as two or more. Use spaces and plain ASCII in the first 40 columns. |
| Long names | A field name longer than 40 characters is cut at column 40, and the rest becomes part of the value. No field the decoder reads is that long. |
| Case | Field names are case-sensitive. `deployment cruise ID` does not match `deployment cruise id` (section 9, item 1). |
| Duplicates | A repeated field overwrites the earlier value. |
| Encoding | Plain ASCII text. Rich Text Format (RTF) files are not converted: every RTF line ends with `\`, so numeric fields fail to parse (section 6). |

## 3. Fields the decoder reads

The patterns are POSIX extended regular expressions matched against the whole trimmed field name. A `.*` suffix allows any units note after the name, such as `(table 1770)`.

| Field name pattern | Example field name in production | Value format | Stored as | Default if absent |
|---|---|---|---|---|
| `internal ID number` | `internal ID number` | Decimal integer | DAC ID (`id_dac`) | `-1` (unknown) |
| `WMO ID number` | `WMO ID number` | Decimal integer | WMO ID (`id_wmo`) | `-1` |
| `WMO instrument type.*` | `WMO instrument type (table 1770)` | Decimal integer | `wmo_instrument_type` | `-1` (`XFLT_ID_UNKNOWN`) |
| `WMO recorder type.*` | `WMO recorder type (table 4770)` | Decimal integer | `wmo_recorder_type` | `-1` |
| `launch time.*` | `launch time [dd mm yyyy hh mm (Z)]` | `dd mm yyyy hh mm`, UTC (section 3.1) | Deployment date/time | not-a-date-time |
| `status of launch time` | `status of launch time` | Free text | `deployment_datetime_status` | `unknown` |
| `launch position.*` | `launch position [lat latm lon lonm]` | `lat latm lon lonm` (section 3.2) | Deployment latitude, longitude, validity | invalid position |
| `status of launch position` | `status of launch position` | Free text | `deployment_position_status` | `unknown` |
| `float manufacturer` | `float manufacturer` | Free text | `manufacturer` | `unknown` |
| `float serial number` | `float serial number` | Decimal integer; leading zeros allowed (`011011` is 11011) | Instrument serial number (`id_instrument`) | `-1` |
| `PI` | `PI` | Free text | `principal_investigators` | `Steven Jayne, Breck Owens, P.E. Robbins` |
| `principal investigator address` | same | Free text | `principal_investigators_address` | `WHOI, Woods Hole, MA` |
| `originating country` | same | Free text | `principal_investigators_originating_country` | `USA` |
| `float deployer` | same | Free text | `deployer` | `unknown` |
| `float deployer address` | same | Free text | `deployer_address` | `unknown` |
| `deployment type` | same | Free text | `deployment_type` | `unknown` |
| `deployment platform` | same | Free text | `deployment_platform` | `unknown` |
| `deployment cruise id` | `deployment cruise id` (older files only) | Free text | `deployment_cruise_id` | `unknown` |
| `comment` | same | Free text | `comment` | empty |
| `instrument type` | same | Free text, e.g. `ALAMOIR_T1`, `ALTOIR_TS12` | `instrument_type` | empty |

Defaults are set by the `XFloatMetadata(const path&)` constructor in `src/data/XFloatMetadata.h`. Integer values are parsed with `boost::lexical_cast<int>`, which accepts only an optional sign and digits; `n/a`, `-999.` or a trailing `\` fail (section 6). Free-text values are stored as written, including `n/a`.

The decoder never reads the IMEI from the file. `XFloat::set_metadata()` (`src/data/XFloat.h`) copies the IMEI that the float already has from its mail messages into the metadata object.

### 3.1 Launch time

The value must match `(\d+) (\d+) (\d+) (\d+) (\d+)` as a whole: five unsigned integers separated by exactly one space each, in the order day, month, year, hour, minute. The time is UTC; the seconds are zero.

| Check | Result |
|---|---|
| Value does not match the pattern (two spaces, a tab, a decimal point, a trailing `\`) | Warning `Failed to match instrument deployment date/time value string: <value>`; date/time set to not-a-date-time. |
| Matches, and 2000 < year < 2100, 1 ≤ month ≤ 12, 1 ≤ day ≤ 31, 0 ≤ hour ≤ 23, 0 ≤ minute ≤ 59 | Date/time set. |
| Matches, but a value is out of those ranges (for example the placeholder `99 99 9999 99 99`) | Date/time set to not-a-date-time, with no message. |
| In range, but not a real date (for example `31 02 2025 00 00`) | `boost::gregorian` throws; error logged (section 6); date/time not set. |

The year must be 2001 to 2099; year 2000 is rejected.

### 3.2 Launch position

The value must match, as a whole:

```
([+-]?\d*\.?\d*)[ \t]+([+-]?\d*\.?\d*)[ \t]+([+-]?\d*\.?\d*)[ \t]+([+-]?\d*\.?\d*)
```

That is four numbers (optional sign, digits, optional decimal point) separated by spaces or tabs: latitude degrees, latitude minutes, longitude degrees, longitude minutes. The decoder computes:

```
latitude  = lat + latm / 60
longitude = lon + lonm / 60
```

- The minutes are added with their own sign, so they must carry the same sign as the degrees. Production writes `-43. -59.941 33. 6.197` for 43° 59.941′ S, 33° 6.197′ E (latitude −43.99902). A positive minute value with negative degrees, as in `-43. 59.941`, gives −42.00098 with no warning.
- Degrees may be written with a trailing decimal point (`-43.`) and minutes as decimals.
- Longitude is −180 to 180. A position is valid only if −90 ≤ latitude ≤ 90 and −180 ≤ longitude ≤ 180 (`PositionUtils::IsPositionValid()` in `src/utility/PositionUtils.h`). Longitudes written 0 to 360 are therefore invalid east of 180.

| Check | Result |
|---|---|
| Matches and position valid | Latitude and longitude set; position valid. |
| Matches but out of range (the placeholder `99. 99. 999. 999.` gives latitude 100.65) | Latitude 99.99999, longitude 999.99999, position invalid. No message. |
| Does not match (for example `ll. 39.996667 63. 59.165000`, or a trailing `.` or `\`) | Warning `Failed to match instrument deployment location value string: <value>`; position invalid. |
| Matches, but a number is only `.`, `+` or `-` | `bad_lexical_cast`; error logged (section 6). |

## 4. Fields the decoder ignores

Every line whose field name matches none of the patterns in section 3 is ignored and listed in the warning `Encountered <n> unsupported metadata fields: "<field> = <value>" ...`. Production files carry between 1 and about 104 such fields. The common ones are:

| Group | Field names (examples) |
|---|---|
| Transmission | `transmission ID number`, `transmission type`, `IRIDIUM program number`, `transmission IMEI number`, `transmission repetition rate [sec]` |
| Mission timing | `start time [dd mm yyyy hh mm (Z)]`, `status of start time`, `delay of first down time [hours]`, `cycle time [days]`, `descent time [hours]`, `neutral depth time [days]`, `ascent time [hours]`, `surface time [hours]`, `clock drift [hours/hours]`, `last cycle` |
| Calibration | `calib eq 1 for pressure`, `calib coef for pressure` (and for temperature, salinity, CPU voltage, pump voltage), `conductivity calibration date`, `temperature calibration date`, `pressure calibration date` |
| Sampling | `nominal drift pressure [dbar]`, `cycles for drift pressure`, `nominal profile pressure [dbar]`, `cycles for profile pressure`, `Profile Sampling Method`, `profile at deployment`, `pump type` |
| Sensors (Argo names) | `CTD_TEMP SENSOR_MAKER`, `CTD_TEMP SENSOR_MODEL`, `CTD_TEMP SENSOR_SERIAL_NO`, `TEMP PARAMETER_UNITS`, `TEMP PRED_CAL_EQ`, `TEMP PRED_CAL_COEF`, `TEMP PRED_CAL_COM`, and the same for `CNDC`/`PSAL`, `PRES` and `TEMP_CNDC` |
| Sensors (older names) | `temperature sensor type`, `... manufacturer`, `... serial number` (also conductivity and pressure) |
| Hardware | `CPU serial number`, `ROM version`, `manual/decoder version`, `battery type`, `battery manufacturer and model`, `CPU battery serial number`, `pump battery serial number`, `initial battery voltage`, `customisation` |
| Argo reference tables | `ref table PLATFORM_TYPE`, `ref table PROJECT_NAME`, `ref table DEPLOYMENT_PLATFORM_ID`, `ref table ... SENSOR_TYPE` |
| Project | `Project Name` |
| Case variants | `deployment cruise ID` (see section 9, item 1) |

## 5. Placeholder files and acceptance rules

A metadata object is used only if its DAC ID (from `internal ID number`) satisfies `0 < DAC ID < 99999` and `DAC ID != 9999` (`XFloat::LoadMetadataFromFiles()` in `src/data/XFloat.cpp`). Otherwise it is dropped without a message.

Placeholders mark floats that have no DAC ID yet. Production uses two conventions:

| Convention | Seen in production | Effect |
|---|---|---|
| File name `9999_<serial>.meta`, `internal ID number -99`, `WMO ID number -1` | 55 of the 66 `9999_*` files (current template) | Dropped: DAC ID < 1. |
| File name `9999_<serial>.meta`, `internal ID number -1` | 2 files (`9999_9162`, `9999_9181`) | Dropped: DAC ID < 1. |
| File name `9999_<serial>.meta`, saved as RTF | 8 files (`9999_9122`, `9999_9123`, `9999_9237` to `9999_9243`) | Dropped: the first field line carries the RTF prefix `\f0\fs22 \cf0 ` and its name no longer matches, so the DAC ID stays −1. |

Only the `internal ID number` decides, not the file name. Production has one counter-example: `9999_9244.meta` contains `internal ID number 6088` and is used (logged as `Float[09244|06088|4903308] ... Loaded instrument metadata.`). A file with a valid name but `internal ID number -99` would be dropped.

For an accepted file:

1. The float object for `float serial number` is looked up, or created if it is not yet known (`XFloat::GetFloat()`). A file without a parsable serial number attaches to a float with serial −1 [inferred].
2. The metadata object is attached to the float, and the float's DAC ID and WMO ID are set from it (`XFloat::set_metadata()`).
3. The float, including its metadata row (table `XFloatMetadata`, see [database-schema.md](database-schema.md)), is saved to the database.
4. `Loaded instrument metadata.` is logged with the float's IDs.

If two accepted files carry the same serial number, the one read last wins; the order is the directory order [inferred].

A float without an accepted metadata file is still decoded. Its DAC ID and WMO ID stay as stored in the database (or −1), and its status file shows `unknown` for the deployment fields.

## 6. Error handling

A problem in one field never stops the file or the other files. All messages are written by the daemon thread (`daemon`) during startup, before the worker threads start. See [log-and-lock-files.md](log-and-lock-files.md) for the log format.

| Problem | Level | Message | Effect |
|---|---|---|---|
| Integer field not an integer (`n/a`, `-999.`, RTF `\`) | error | `Failed to extract metadata from field: '<field>' = '<value>'`, followed by a cause chain ending in `bad lexical cast` | Field skipped; default kept. |
| Launch time does not match the pattern | warning | `Failed to match instrument deployment date/time value string: <value>` | Date/time not-a-date-time. |
| Launch time not a real date | error | `Failed to extract metadata from field: 'launch time ...' = '<value>'` with the cause | Date/time not set. |
| Launch position does not match the pattern | warning | `Failed to match instrument deployment location value string: <value>` | Position invalid. |
| Unknown fields | warning | `Encountered <n> unsupported metadata fields: "<field> = <value>" ...` (one line per file) | Ignored. |
| File unreadable or vanished | error | `Failed to extract metadata from file: <path>!` | File skipped. |
| `dir.meta` missing | error, then abort | `Invalid metadata path: <path>` | Startup fails (see [configuration-file-format.md](configuration-file-format.md), section 6). |

Real example from the 2026-10-28 error log on `argus`, for the RTF placeholder `9999_9122.meta` (multi-line, as written):

```
2026-10-28T00:26:15.134567       daemon   error -------------------------- ---------- ---------------------- ---------- Failed to extract metadata from field: 'WMO ID number' = '4902953\'
../src/data/XFloatMetadata.cpp(417): Throw in std::istream& argo::iridium::xfloat::operator>>(std::istream&, XFloatMetadata&)
	Caused by: 
../src/data/XFloatMetadata.cpp(236): Throw in function operator>>
Dynamic exception type: boost::wrapexcept<util::log::MessageException>
std::exception::what: Failed to parse WMO ID!
	Caused by:
Throw location unknown (consider using BOOST_THROW_EXCEPTION)
Dynamic exception type: boost::wrapexcept<boost::bad_lexical_cast>
std::exception::what: bad lexical cast: source type value could not be interpreted as target
```

Each daemon start logs 32 such error entries (2026-10-28: 128 entries over 4 starts), nearly all from the 8 RTF placeholder files. They are harmless, since those files are dropped anyway.

## 7. What the metadata is used for

| Use | Fields | Source |
|---|---|---|
| Float DAC ID and WMO ID in listings, PHY files, status files, logs | `internal ID number`, `WMO ID number` | `XFloat::set_metadata()` |
| Surface pressure offset correction, when the dive's mission message does not give the sensor type: 866, 867, 875 corrected; 868, 873, 876, −1 not corrected; other values not corrected, with the warning `Unknown WMO instrument type encountered` | `WMO instrument type` | `src/data/XFloat.cpp` (pressure offset computation) |
| Lower time limit for the GPS fixes and mail messages used for a float (`datetime > launch time`) | `launch time` | `src/data/XFloat.cpp` |
| Deployment placemark in the KML file, only if the position is valid | `launch position` | `XFloat` KML writer; see [kml-file-format.md](kml-file-format.md) |
| Status file `DEPLOYMENT_*`, `DEPLOYER*`, `WMO_INSTRUMENT_TYPE`, `WMO_RECORDER_TYPE` lines | launch, deployer, deployment and WMO fields | `XFloat::GenerateStatusString()`; see [status-file-format.md](status-file-format.md) |
| PHY file header (`INSTRUMENT TYPE`, `WMO INSTRUMENT TYPE (TABLE 1770)`, `WMO RECORDER TYPE (TABLE 4770)`, `PI`, ...). A PHY file is written only for floats with metadata. | `instrument type`, WMO types, `PI` | `src/data/XFloat.cpp`; see [phy-file-format.md](phy-file-format.md) |
| Database table `XFloatMetadata` | all stored fields, plus `local_path` (the file path) and the IMEI | `XFloatMetadata::sql_table_description_` |

## 8. Examples

Production file `6033_9078.meta` (ALAMO, air-deployed, launch position unknown), complete:

```
internal ID number                      6033
transmission ID number                  9078
transmission type                       IRIDIUM
instrument type                         ALAMOIR_T1
IRIDIUM program number                  n/a
WMO ID number                           4902257
WMO instrument type (table 1770)        866
WMO recorder type (table 4770)          64
launch time [dd mm yyyy hh mm (Z)]      30 08 2016 00 00 
status of launch time                   as recorded
launch position [lat latm lon lonm]     99. 99. 999. 999.
status of launch position               as recorded
cycle time [days]                       -999.
descent time [hours]                    -999.
neutral depth time [days]               -999.
ascent time [hours]                     -999.
surface time [hours]                    -999.
clock drift [hours/hours]               0
transmission repetition rate [sec]      -999.
last cycle                              n/a
========================================
calib eq 1 for pressure                 n/a
calib coef for pressure                 n/a
calib eq 1 for temperature              n/a
calib coef for temperature              n/a
calib eq 1 for cpu voltage              n/a
calib coef for cpu voltage              n/a
calib eq 1 for pump voltage             n/a
calib coef for pump voltage             n/a
========================================
conductivity calibration date           n/a
temperature calibration date            n/a
pressure calibration date               n/a
float manufacturer                      MRV
float serial number                     9078
PI                                      Steven Jayne, Breck Owens, P.E. Robbins
principal investigator address          WHOI, Woods Hole, MA
originating country                     USA
float deployer                          Jayne
float deployer address                  n/a
deployment type                         Airborne
deployment platform                     USAFR 53rd WRS C-130
deployment cruise id                    n/a
profile at deployment                   n/a
nominal drift pressure [dbar]           1300
cycles for drift pressure               1
pump type                               n/a
nominal profile pressure [dbar]         1200
cycles for profile pressure             1
Profile Sampling Method                 continuous
temperature sensor type                 n/a
temperature sensor manufacturer         n/a
temperature sensor serial number        n/a
pressure sensor type                    n/a
pressure sensor manufacturer            n/a
pressure sensor serial number           n/a
ROM version                             n/a
battery type                            n/a
initial battery voltage [volt]          n/a
comment                                 Pacific
```

The decoder reads from it: DAC ID 6033, WMO ID 4902257, WMO instrument type 866, recorder type 64, launch time 2016-08-30 00:00 UTC (the trailing space is trimmed), an invalid launch position (99 + 99/60 > 90), serial 9078, and the free-text fields. The resulting status file lines are:

```
DEPLOYMENT_DATETIME                     2016-08-30T00:00:00
DEPLOYMENT_DATETIME_STATUS              as recorded
DEPLOYMENT_POSITION                     +99.99999+999.99999
DEPLOYMENT_POSITION_STATUS              as recorded
DEPLOYMENT_CRUISE_ID                    n/a
DEPLOYMENT_PLATFORM                     USAFR 53rd WRS C-130
DEPLOYMENT_TYPE                         Airborne
DEPLOYER                                Jayne
DEPLOYER_ADDRESS                        n/a
```

Production file `10293_011682.meta` (ALTO with RBR CTD, ship-deployed), first part, with the IMEI masked; the remaining 70 lines are sensor, hardware and reference-table fields of the kinds listed in section 4:

```
internal ID number                      10293
transmission ID number                  011682
transmission type                       IRIDIUM
instrument type                         ALTOIR_TS12
IRIDIUM program number                  n/a
WMO ID number                           1902778
WMO instrument type (table 1770)        875
WMO recorder type (table 4770)          64
start time [dd mm yyyy hh mm (Z)]       10 04 2025 07 43
status of start time                    estimated
launch time [dd mm yyyy hh mm (Z)]      21 04 2025 16 12
status of launch time                   estimated
launch position [lat latm lon lonm]     -43. -59.941 33. 6.197
status of launch position               estimated
delay of first down time [hours]        00
cycle time [days]                       10.0
...
float manufacturer                      MRV
float serial number                     11682
Project Name                            Argo WHOI
PI                                      Susan WIJFFELS, Steven JAYNE, Pelle ROBBINS
principal investigator address          WHOI, Woods Hole, MA
originating country                     USA
float deployer                          unknown
float deployer address                  SAEON
deployment type                         RV
deployment platform                     SA Agulhas II
deployment cruise ID                    Marion Island-2025
...
transmission IMEI number                3005340651XXXXX
...
comment                                 n/a
```

The decoder reads launch time 2025-04-21 16:12 UTC and launch position −43.99902, 33.10328 (status file `DEPLOYMENT_POSITION -43.99902+33.10328`). It ignores `deployment cruise ID` because of its upper-case `ID`, so the status file shows `DEPLOYMENT_CRUISE_ID unknown`. The file produces the warning `Encountered 96 unsupported metadata fields: "transmission ID number = 011682" ...`.

A placeholder (`9999_011181.meta`, first lines):

```
internal ID number                      -99
transmission ID number                  011181
transmission type                       IRIDIUM
instrument type                         ALTOIR_TS12
IRIDIUM program number                  n/a
WMO ID number                           -1
WMO instrument type (table 1770)        875
WMO recorder type (table 4770)          64
start time [dd mm yyyy hh mm (Z)]       99 99 9999 99 99
status of start time                    n/a
launch time [dd mm yyyy hh mm (Z)]      24 09 2026 03 35
status of launch time                   as reported (around 11:35 UTC+0800)
```

It is parsed and then dropped, because its DAC ID is −99.

## 9. Open questions / discrepancies

1. **`deployment cruise ID` is ignored.** The decoder pattern is `deployment cruise id` (lower case). 63 production files use `deployment cruise ID`, 228 use `deployment cruise id`. For the 63, the cruise ID is never read and status files show `unknown` (seen for float 11682). Either the pattern should be case-insensitive or the files should be changed.
2. **README says DAC ID 9999 marks a placeholder.** True for the file names, but the current placeholder template writes `internal ID number -99`, and only the internal ID decides. The code comment in `XFloat::LoadMetadataFromFiles()` mentions both 9999 and −99; README.md mentions only 9999.
3. **Invalid-longitude constant.** In the branch where the launch position does not match the pattern, the longitude is set to `XFLT_POS_INVALID_LON`, which is defined as `IRI_POS_INVALID_LAT` (99.99999) in `src/data/XFloatDefinitions.h`; `IRI_POS_INVALID_LON` in `src/data/IridiumDefinitions.h` is also defined as `UTIL_POS_INVALID_LAT`. The out-of-range branch uses the correct `UTIL_POS_INVALID_LON` (999.99999). The position is flagged invalid in both branches, so the effect is limited to the stored longitude value (99.99999 instead of 999.99999).
4. **DAC IDs of 10000 and above in log prefixes.** The log formatter shows the DAC ID only if 1000 ≤ DAC < 10000 (`src/utility/Log.h`). Floats with five-digit DAC IDs (10293 and up) appear as `Float[11682|?????|1902778]` although their metadata loaded correctly. See [log-and-lock-files.md](log-and-lock-files.md).
5. **File name and contents disagree in production.** `6062_9130.meta` contains `float serial number 9119`, so its metadata is attached to float 9119 (logged as `Float[09119|06062|4902960]`); float 9130 has no metadata file. `6112_9298.meta` contains `internal ID number 6122`. The decoder uses the contents in both cases; whether the files or the names are wrong was not checked.
6. **Non-ASCII files in production.** Besides the 8 RTF placeholders, `6011_9034.meta` contains NUL bytes (line 61); `file` reports it as `data`. Its lines before the NULs parse normally [inferred]. `SylviaCole_template.meta` is RTF but does not match the file-name pattern.
7. **Launch time year range.** The check `yr > 2000` rejects a launch in the year 2000. No ALAMO/ALTO float was launched then, so this has no effect.
8. **`\d` in POSIX-extended patterns.** The launch time and launch position patterns use `\d` with `regex::extended`, which POSIX leaves undefined. The production libstdc++ (GCC 15.2) treats it as a digit class; production status files confirm that both values parse. See [configuration-file-format.md](configuration-file-format.md), section 9, item 2.
