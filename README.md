# XMTPS — X-Message Telemetry Processing Service

XMTPS is the WHOI Float Group daemon that receives Iridium Short Burst Data (SBD) telemetry from
ALAMO and ALTO profiling floats, decodes it, stores it in MySQL, and writes the listing, PHY, KML,
status and RAW files used downstream. A set of Python tools in `bin/py` turns the listing files
into JSON and generates the per-float plot pages and the fleet summary page.

## How it works

Iridium delivers each SBD message as an email with a binary `.sbd` attachment. XMTPS processes
them in a four-stage pipeline of worker threads, connected by bounded queues:

| Stage | Source | Role |
|---|---|---|
| 1. Mail retrieval | `src/stage1_MailRetrievalThread.*` | Fetches mail from the IMAP server (or RAW mail files from a directory) and queues it. Optionally reprocesses stored messages from the database at startup. |
| 2. Mail processing | `src/stage2_MailProcessingThread.*` | Validates the header and SBD attachment, resolves the float's identifiers, writes the RAW/SBD files, saves the message to MySQL (holding it while the SQL server is unavailable), and then queues it for decoding. |
| 3. Telemetry decoding | `src/stage3_TelemetryDecoderThread.*` | Decodes the SBD data into X-messages, assigns them to the float's dive, and writes listing/PHY files and dive records when a dive is complete. |
| 4. Post-processing | `src/stage4b_PostProcessingThread.*` | Periodically writes each float's KML and status files and the master KML file. |

The main thread (`src/Main.cpp`) parses the configuration, takes the run-time PID lock, loads the
cached float data and metadata, starts the workers, and shuts them down in pipeline order when it
receives a signal. Each stage drains its queue before exiting.

### Source layout

```
src/
  Main.cpp, stage*_*.cpp/.h   daemon entry point and pipeline stages
  config/                     command-line/config-file options, MySQL connection settings
  data/                       floats (XFloat), dives (XFloatDiveData), mail messages, metadata
  data/xmsg/                  X-message decoders (GPS, profiles, drift, engineering, ice, wave, ...)
  utility/                    logging, MySQL/SQL table helpers, KML, queues, file-system helpers
bin/
  py/                         JSON conversion, plotting and summary-page tools
  xmtps-alamo.service         systemd unit for the decoder daemon
  xmtps-alamo-plot.service    systemd unit for the plot-generation loop
build/                        build-number script and counter
```

## Building

XMTPS is a C++11 project built with Eclipse CDT managed builds (`.cproject`, `Debug/` and
`Release/` makefiles) and g++ on Linux. It links against:

- Boost: `chrono`, `date_time`, `filesystem`, `iostreams`, `log`, `log_setup`,
  `program_options`, `regex`, `thread`
- [VMime](https://www.vmime.org/) (IMAP and MIME parsing)
- MySQL Connector/C++ (`mysqlcppconn`)
- libkml (`kmlbase`, `kmldom`, `kmlengine`)
- pthreads

The build number embedded in the binary comes from `build/buildNo.txt`. It is incremented
with each commit by the `build/hooks/pre-commit` hook (enable it in a clone with
`git config core.hooksPath build/hooks`); building no longer changes it, so a binary reports
the build number of the commit it was built from.

The code uses POSIX APIs (`open`/`O_EXCL`, `getpid`, signals), so it builds on Linux only.

## Configuration

### How options are read

- Every option can be given on the command line. The options in the "Configuration file
  options" tables below can also be set in the configuration file.
- The configuration file (`-c PATH`, default `../cfg/options.cfg`, relative to the working
  directory) must exist; the program stops if it is missing or contains an unknown option.
- Command-line values take precedence: the configuration file is read after the command line and
  only fills in options that were not given there.
- In the configuration file, options in a table's section are written as `name = value` under a
  `[section]` header (for example `host` under `[mail]`); on the command line they are written
  with the section as a prefix (for example `--mail.host maila.whoi.edu`).
- Boolean values are written as `0`/`1` or `false`/`true`, except the switches `-D` and `-R`,
  which take no value.
- `xmtps --help` prints all options with their defaults and exits.

### Command-line only options

| Option | Default | Description |
|---|---|---|
| `-c`, `--config PATH` | `../cfg/options.cfg` | Path to the configuration file. |
| `-D`, `--daemon` | off | Run as a daemon: keep running until a termination signal or until the run-time lock is lost. Without it, the program starts the pipeline, waits briefly, and exits. |
| `-h`, `--help` | | Print all options with their defaults, then exit. |
| `-V`, `--version` | | Print the version banner, then exit. |
| `-d`, `--debug` | | Log at debug level. |
| `-v`, `--verbose` | | Log informational messages as well (used if `-d` is not given). |
| `-q`, `--quiet` | | Log only warnings and errors (used if neither `-d` nor `-v` is given). |
| `-r`, `--reprocess ID ...` | none | Reprocess all stored mail messages for the given instrument serial numbers before normal operation: their dive data is deleted and rebuilt from the database. Repeatable, and accepts several IDs. |
| `-R`, `--reprocess-all` | off | Reprocess the stored mail messages of every cached instrument at startup. |
| `-A`, `--reprocess-active 0\|1` | `1` | Reprocess every instrument that reported in the last 90 days at startup. **On by default.** |
| `--phy-enable ID` | none | Enable PHY file generation for an instrument (repeatable). The instrument is also reprocessed at startup. |
| `--phy-disable ID` | none | Disable PHY file generation for an instrument (repeatable). |

With no logging option, the normal log level is used: operational messages, warnings and errors.

### Configuration file options

General:

| Option | Default | Description |
|---|---|---|
| `-m`, `maxFetch N` | `127` | Maximum number of mail messages fetched from the server, or files read from `dir.mail`, per batch. |
| `-M`, `msgStartIdx N` | `1` | Index of the first message on the mail server to fetch (server mode only). |
| `-n`, `nProcThrd N` | `1` | Number of mail processing threads. |
| `-N`, `nDecoThrd N` | `1` | Number of telemetry decoder threads. |
| `-p`, `pid-file PATH` | `/var/lock/xmtps/lock.pid` | Run-time lock (PID) file. Only one instance can hold it. |

`[dir]` — directories (relative paths are relative to the working directory):

| Option | Default | Description |
|---|---|---|
| `dir.kml` | `kml` | KML files. |
| `dir.lis` | `lis` | Intermediate listing files. |
| `dir.log` | `log` | Log files. |
| `dir.meta` | `meta` | Float metadata (`.meta`) files. |
| `dir.phy` | `phy` | PHY listing files. |
| `dir.raw` | `raw` | RAW mail message output files. |
| `dir.mail` | `raw/incoming` | RAW mail input files (file-system fetch mode). |
| `dir.mail.processed` | `raw/processed` | Processed RAW mail input files. Accepted but not currently used. |
| `dir.sbd` | `sbd` | Binary SBD message files. |
| `dir.stat` | `stat` | Status files. |

`[gen]` — which output files to write:

| Option | Default | Description |
|---|---|---|
| `gen.kml` | `1` | Google Earth KML files. |
| `gen.lis` | `1` | Intermediate listing files. |
| `gen.phy` | `1` | PHY listing files. |
| `gen.raw` | `1` | RAW mail message files. |
| `gen.raw.format` | `1` | RAW file format: `0` standard, `1` extended (adds headers, Iridium session details and an SBD hex dump). |
| `gen.sbd` | `0` | Binary SBD message files. |
| `gen.stat` | `0` | Status files. |

`[poc]` — surface pressure offset correction (applied only for sensor types that need it; see
[Float metadata](#float-metadata)):

| Option | Default | Description |
|---|---|---|
| `poc.lis` | `1` | Apply the correction in listing files. |
| `poc.phy` | `1` | Apply the correction in PHY files. |

`[mail]` — mail server and message validation:

| Option | Default | Description |
|---|---|---|
| `mail.host` | `maila.whoi.edu` | Mail server host name or IP address. |
| `mail.protocol` | `imap` | Mail server protocol. |
| `mail.user` | `alamo` | Mail account user name. |
| `mail.password` | none | Mail account password. |
| `mail.folder` | empty (server default) | Folder to read messages from, e.g. `INBOX`. |
| `mail.fetchMode` | `0` | `0` fetch from the mail server; `1` read RAW mail files from `dir.mail` (renamed to `.err` if they can't be parsed). |
| `mail.moveMessages` | `0` | Move server messages to the folder `INBOX.<YYYYMM>` for their header date once they are processed (saved to the SQL database or rejected); a message whose processing is interrupted stays in the inbox. |
| `mail.xDlyS` | `60` | Seconds to wait before checking again when no new messages are found. |
| `mail.validSenderRegex` | `(sbdservice@sbd.iridium.com\|service@sbd.pac.disa.mil)` | Senders whose messages are accepted. |
| `mail.validSubjectRegex` | `SBD Msg From Unit: (\d+)` | Accepted subjects; the captured number is the IMEI. |
| `mail.validSbdFilenameRegex` | `(\d+)_(\d{6})\.sbd` | Accepted SBD attachment file names. |

The mail user and password may contain any characters; they are percent-encoded when the server
URL is built.

`[sql]` — MySQL server:

| Option | Default | Description |
|---|---|---|
| `sql.host` | `127.0.0.1` | Server host name or IP address. |
| `sql.port` | `3306` | Server port. |
| `sql.protocol` | `tcp` | Connection protocol. |
| `sql.schema` | `float_db` | Database schema name. |
| `sql.user` | none | Account user name. |
| `sql.password` | none | Account password. |

### Example configuration file

Credentials omitted:

```ini
maxFetch                = 16
nProcThrd               = 1
nDecoThrd               = 1

[dir]
meta                    = /data1/alamo/meta/dac
lis                     = /data1/alamo/data/list
raw                     = /data1/alamo/data/raw

[gen]
kml                     = true
stat                    = true

[mail]
host                    = maila.whoi.edu
protocol                = imap
folder                  = INBOX
fetchMode               = 0
validSenderRegex        = (sbdservice@sbd\.iridium\.com|service@sbd\.pac\.disa\.mil)
validSubjectRegex       = SBD Msg From Unit: (\d+)

[sql]
host                    = 127.0.0.1
port                    = 3306
schema                  = xmtps_alamo
```

Keep real configuration files, with their credentials, out of version control (`cfg/` is
ignored).

### Float metadata

Each float's metadata is read at startup from `<dir.meta>/<DAC-ID>_<serial>.meta`, a plain-text
file with the field name in the first 40 columns and the value after it. Files must be plain ASCII
text (not RTF). The daemon uses, among others, the WMO ID, WMO instrument type, launch time
(`dd mm yyyy hh mm`) and launch position (`lat latm lon lonm`, with the minutes carrying the same
sign as the degrees). A field that fails to parse is logged and skipped. DAC ID `9999` marks a
placeholder file and is ignored.

Supported WMO instrument types: 866, 867, 868 (ALAMO) and 873, 875, 876 (ALTO). The surface
pressure offset correction is applied for RBR conductivity sensors (867, 875) and for floats
without one (866), and is skipped for SBE sensors (868, 876) and for 873.

## Running

```
xmtps --config /data1/alamo/etc/xmtps.cfg --daemon --pid-file /data1/alamo/var/lock/xmtps.pid
```

See [Configuration](#configuration) for every option. Note that `--reprocess-active` is on by
default, so each start reprocesses the floats active in the last 90 days; pass `-A 0` to skip it.

The PID file is created exclusively at startup, so only one instance can run at a time. It is
removed on shutdown only by the process that owns it.

Logs are written to `<dir.log>/YYYY-MM-DD.log`, with warnings and errors also in
`<dir.log>/YYYY-MM-DD.error.log`.

### Output files

| Output | Location |
|---|---|
| RAW mail | `<dir.raw>/<serial>/raw<serial>-<MOMSN>-<dive>-<packet>.txt` |
| Listings | `<dir.lis>/<serial>/lis<serial>-<dive>.txt` |
| PHY, KML, status, SBD | under `dir.phy`, `dir.kml`, `dir.stat`, `dir.sbd` |

Decoded data, messages, floats and dives are also stored in the MySQL database (see
[SQL backend](#sql-backend)).

## Listing files

A listing file is the decoder's human- and machine-readable record of one dive: everything decoded
from the dive's SBD messages, written to `<dir.lis>/<serial>/lis<serial>-<dive>.txt` (the dive
number has four digits; pre-mission dives are negative, e.g. `lis10108--001.txt`). The file is
rewritten each time a message of the dive is decoded, so it always reflects the messages received
so far. The Python tools (`xmtps-list2json.py`) parse these files.

### Layout rules

- Lines starting with `#` are comments: banners, section titles, column-name and unit headers.
  Anything after a `#` on a data line is also a comment (for example a phase name or a parameter
  description) and is not meant to be machine-readable.
- Every data line starts with a tag naming its type, followed by the float serial number and the
  dive number; the remaining fields are separated by whitespace. The comment lines just above a
  group of data lines give the column names and, on a second line in parentheses, the units.
- An upper-case tag introduces a group and a lower-case tag of the same name holds its rows, e.g.
  `PRO` (sample count) and `pro` (one line per profile bin), or `D` (drift start time) and `d`
  (drift samples).
- Time-series groups (`F`/`f`, `D`/`d`, `R`/`r`, `SFC`/`sfc`) give an absolute start time
  (ISO 8601, UTC) on the group line and the time of each row as `+seconds` from that start.
- Engineering and mission messages that span several lines number them with a minor index,
  `midx`; `# --- Name ---` comment lines name the sub-groups.
- Times are UTC (`2026-10-25T12:03:07Z`); an unset time is `0000-00-00T00:00:00Z`. Positions are
  written as signed latitude and longitude run together (`+12.73113-163.48779`). Missing or invalid
  sensor values are `-9.999` (pressure, temperature and salinity, with pressure to two decimals);
  other fields have their own fill values, for example `-1` for unknown IDs (`-001`, `-000001`)
  and `±999.9999` in the wave spectra.
- IMEIs are reduced to their last seven digits (`000000002214760`) on purpose.

### Line types

| Tag | Section | Contents |
|---|---|---|
| `XSV` | XMTPS software version | X-message format version, XMTPS version, build configuration and date. |
| `LIS` | Listing details | Serial number, dive, DAC ID, IMEI, WMO ID. |
| `SBD` | SBD messages | One line per message: size, IMEI, MOMSN, MTMSN, packet ID, session time and position, CEP. |
| `GPS` | GPS | GPS fixes: time, position, time to fix, satellites, SNR, HDOP, status. |
| `IRI` | Iridium position | Position estimated from the Iridium sessions: weighted time and position, variance, error. |
| `A` | Argo data | Mission phase, pressures, durations, cycle timing and sensor gains and offsets (`midx` 0–5). |
| `PRC` | Pressure correction | Surface pressure offset applied to the profile, and the dives and samples it comes from. |
| `F` / `f` | Fall time series | Descent: time, pressure, temperature, salinity and phase code (name after `#`). |
| `D` / `d` | Drift data | Drift/park samples: time, pressure, temperature, salinity. |
| `PRD` / `prd` | Profile sensor columns | Name, units and description of each `pro` column. |
| `PRO` / `pro` | Profile data (binned) | One line per pressure bin; the columns are given by `prd`. |
| `PRU` / `pru` | Profile data (unbinned) | Raw profile samples, for floats that send them. |
| `VLV` / `vlv` | Valve data | Valve actions: pressure, duration, case vacuum before and after. |
| `PMP` / `pmp` | Pump data | Pump actions: pressure, duration, voltage, current, case vacuum. |
| `R` / `r` | Rise time series | Ascent, in the same layout as the fall time series. |
| `SFC` / `sfc` | Surface time series | Surface samples, in the same layout. |
| `SFD` | Surface derived fields | Surface pressure statistics: valid count, minimum, mean, maximum, variance. |
| `WVS` / `wvs` / `wvm` | Ocean surface waves | Spectrum parameters, then acceleration spectra (`wvs`, g²/Hz) and displacement spectra (`wvm`, m²/Hz). |
| `E05` / `E06` | Engineering (BIT passed / failed) | Built-in test results. |
| `E07`, `E08`, `E09`, `E0A`, `E0C` | Engineering | Descent, park/drift, ascent, surface and ice engineering messages (`midx` lines). |
| `e0A`, `e0c` | Engineering errors | Error codes with their time, one line each. |
| `APS` | Acoustic position | Acoustic listening windows and receive statistics. |
| `MIS` / `m` | Mission configuration | Configuration message and firmware version, then one parameter per `m` line: name, value, units, `# description`. |
| `mis` | Mission configuration (older firmware) | The same parameters in fixed-column groups. |
| `SHX` / `shx` | SBD message hex data | Each SBD message as a hex dump, 16 bytes per line. |

### Examples

The excerpts below are from `lis9287-0325.txt` and `lis11186-0107.txt` (some lines and repeated
rows left out, marked `...`).

Start of the file, the version and the SBD messages:

```
#===================================================================================================
# Intermediate Listing for Float #9287, Dive #325
#===================================================================================================
#                        Created: 2026-10-25T14:05:48Z
#---------------------------------------------------------------------------------------------------
# Dive Data Completeness
#---------------------------------------------------------------------------------------------------
#                  Dive Complete: yes
#          Complete Profile Data: yes
...
#       xmessage-version        xmtps-version     build-configuration                build-date/time
XSV                  2.2            0.6.3298a                   DEBUG         2026-10-25T14:00+00:00
#      SN dive DAC-ID            IMEI  WMO-ID
LIS  9287  325   -001 000000002214760 -000001
...
#      SN dive size            IMEI MOMSN MTMSN PID            date/time            position  CEP
#               (b)                                                                          (km)
SBD  9287  325  192 000000002214760  3638     0   9 2026-10-25T12:03:07Z +12.73113-163.48779    3
SBD  9287  325  275 000000002214760  3639     0   8 2026-10-25T12:03:20Z +12.73113-163.48779    3
```

A time series: the `F` line gives the number of samples and the start time, and each `f` line the
offset in seconds, the sample and the phase code:

```
#      SN dive idx nSam                 time      pres    temp    saln phaseCode   phaseCodeName
#                                  (UTC/+sec)    (dBar)  (degC)  (PSU)
F    9287  325   0   16 2026-10-24T07:37:09Z
f    9287  325   0    0                   +0      0.12  -9.999  -9.999         1 # start of descent
f    9287  325   0    1                 +293     30.08  -9.999  -9.999        10 # descending
f    9287  325   0    2                 +387     43.52  -9.999  -9.999         2 # first descent pump
```

The binned profile, with its column descriptions:

```
#      SN dive      column    units  description
prd  9287  325   sampleIdx        -  # Bin index, in the order the float reports the bins.
prd  9287  325        pres     dBar  # Bin pressure, less the surface pressure offset (see Pressure
#                                      Correction).
prd  9287  325       temp1     degC  # CTD temperature.
...
#      SN dive  numSamples
PRO  9287  325         278
#      SN dive   sampleIdx        pres       temp1       temp2        saln
#                               (dBar)      (degC)      (degC)       (PSU)
pro  9287  325           0      278.04       9.817       9.806      34.605
pro  9287  325           1      277.60       9.817       9.806      34.604
```

An engineering message over two lines (`midx` 0 and 1), with a sub-group heading:

```
#      SN dive midx engVer       start-of-drift      vac0      vac1           seeks    seekPumpAdjT
#                                                  (inHg)    (inHg)                          (+sec)
E08 09287  325    0   0x12 2026-10-24T11:05:29Z      6.65      6.69               0              +0
# --- Drift Sensor Averages ------------------------------------------------------------------------
#      SN dive midx            pres         pres         temp         temp         saln         saln
#                            (dBar)      (counts)       (degC)     (counts)        (PSU)     (counts)
E08 09287  325    1          274.72         7118        9.744        14744       34.523        35523
```

Mission parameters (`lis11186-0107.txt`):

```
#      SN dive        CfgMsgVer       FWVer sensor-type        sensor-type-name
MIS 11186  107             0x18  0x0A0C0001           0      # RBR
# --- General --------------------------------------------------------------------------------------
#      SN dive idx param      value      units          desc
#      -- ---- --- -----      -----      -----          ----
m   11186  107   0 MSN          107      [-]          # dive cycle number
m   11186  107   3 MDS         2100      [dBar]       # maximum depth
```

The hex dump of one SBD message:

```
#      SN dive   size MOMSN MTMSN PID
#                 (b)
SHX  9287  325    192  3638     0   9
#      SN dive MOMSN idx PID               | 0x  0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F
shx  9287  325  3638   0   9 0x0000 (0000) | 0x 58 00 B9 24  47 01 45 09  60 20 5C 20  05 3A 00 50
#                                          |     X  .  .  $   G  .  E  .   `     \      .  :  .  P
```

## SQL backend

XMTPS keeps its state in a MySQL database (InnoDB tables in the schema named by `sql.schema`;
production: `xmtps_alamo`), so that it can be restarted and floats can be reprocessed:

- **Floats**: each float's identifiers, sensor gains and offsets, latest positions and mission
  start, plus the deployment metadata read from its `.meta` file. They are reloaded at startup so
  processing resumes where it left off.
- **Dives and GPS fixes**: the state of each dive cycle and its GPS fixes, rebuilt as messages are
  decoded.
- **Mail messages**: every Iridium SBD message received, with its mail header, Iridium session
  details and the raw SBD data. Reprocessing (`-r`, `-R`, and `-A` at startup) deletes a float's
  dive records and decodes its stored messages again.

Each table is mapped to a C++ class through `util::mysql::SqlTableDescription`, which generates
the `SELECT`, `DELETE` and `INSERT ... ON DUPLICATE KEY UPDATE` statements; saving an object
inserts a row or overwrites the row with the same key. Each worker thread uses its own connection.

The mail processing thread saves each message before it is decoded. While the SQL server is
unavailable, messages are held and retried every 10 s and no new mail is fetched (see
`mail.moveMessages`), so an outage delays processing but does not lose messages.

[`sql/xmtps-schema.sql`](sql/xmtps-schema.sql) creates the tables (the production schema,
including its indexes and foreign keys) in an existing database:

```
mysql -u <user> -p -e "CREATE DATABASE xmtps_alamo DEFAULT CHARACTER SET utf8mb4"
mysql -u <user> -p xmtps_alamo < sql/xmtps-schema.sql
```

It only creates tables that do not exist yet. The server's `sql_mode` must allow zero dates
(production uses `STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION`), because
XMTPS writes unset date/times as zero dates.

## Deployment

The `bin/*.service` files are systemd units:

- `xmtps-alamo.service` runs the decoder daemon with `Restart=always` and `RestartSec=30`, so a
  startup failure (for example MySQL being unavailable) keeps retrying without hitting systemd's
  start limit.
- `xmtps-alamo-plot.service` runs `bin/py/update-plots-service.sh`, also with `Restart=always` and
  `RestartSec=30`.

The installed copies live in `/etc/systemd/system/`. After installing or changing a unit file, run
`sudo systemctl daemon-reload`.

## Python tools (`bin/py`)

Requires Python 3 with `numpy`, `scipy`, `pandas`, `plotly` (plus `kaleido` for PNG export),
`jsonpickle`, `geographiclib`, `pytz`, `python-dateutil`, `Pillow` and `yattag`.

| Script | Purpose |
|---|---|
| `xmtps-list2json.py` | Converts listing files into per-dive JSON (`<out>/<serial>/dives/<serial>_<dive>.json`), adding density and σ₀ from `RhoLUT.py` and wave statistics from `FloatClasses.py`. |
| `xmtps-aggregate-json.py` | Aggregates a float's dive JSON into per-float JSON files (`<serial>_profile.json`, `_wave_spectra.json`, `_summary.json`, ...) and, with `-p DIR`, writes the plot images and the float's HTML page. |
| `xmtps-generate-summary-page.py` | Builds the fleet summary page from the per-float `_summary.json` files. |
| `update-plots-service.sh` | Loop used by the plot service: regenerates JSON and plots for floats with recent listings, then the summary page. |

All three scripts share JSON version `0.11`; JSON files with a different version are ignored.

### Command-line options

Each script takes one or more input paths (directories and/or files) as positional arguments.
Paths matching an include pattern are tested with a regular-expression search on the full path
(using `/` separators), so a pattern only needs to match part of it.

Options common to all three scripts:

| Option | Default | Description |
|---|---|---|
| `-d`, `--debug` | | Print debug output. |
| `-v`, `--verbose` | | Print informational output (used if `-d` is not given). |
| `-q`, `--quiet` | | Print only errors (used if neither `-d` nor `-v` is given). |
| `-H`, `--halt-on-error` | off | Stop at the first error instead of logging it and continuing. |
| `-i`, `--include REGEX` | all files | Process only files whose path matches `REGEX`. Repeatable; a file is processed if it matches any pattern. |
| `-o`, `--output DIR` | `.` | Output directory (see each script). |

With no logging option, only warnings and errors are printed.

`xmtps-list2json.py [options] PATH ...` — converts listing files (`PATH` may be listing files or
directories containing them) into per-dive JSON files in `<output>/<serial>/dives/`:

| Option | Default | Description |
|---|---|---|
| `-r`, `--recursive` | off | Descend into subdirectories of the input directories. |
| `-x`, `--exclude REGEX` | none | Skip directories and files whose path matches `REGEX`. Repeatable. |
| `-f`, `--force` | off | Rewrite every dive JSON file, even if it is newer than its listing file. |
| `-s`, `--skip` | off | Skip listing files whose dive JSON already exists, even if the listing is newer. |
| `-S`, `--strip-path REGEX` | `.*/alamo/` | Remove the matching part of the listing file path before storing it in the JSON. |

Without `-f` or `-s`, a listing file is converted only if its dive JSON doesn't exist yet or is
older than the listing file.

`xmtps-aggregate-json.py [options] PATH ...` — reads dive JSON files (`PATH` is usually a float's
`dives` directory) and writes the per-float aggregate JSON files to `<output>/<serial>/`:

| Option | Default | Description |
|---|---|---|
| `-p`, `--plots DIR` | none | Also write the plot images, plot HTML files and the float's `index.html` page to `DIR/<serial>/`. `DIR` must exist. |
| `-r`, `--recursive` | off | Descend into subdirectories of the input directories. |
| `-x`, `--exclude REGEX` | none | Skip subdirectories whose path matches `REGEX` (applies to directories only). Repeatable. |
| `-f`, `--force` | off | Process every directory, ignoring its `.lastprocessed` file. |
| `-S`, `--strip-path REGEX` | `.*/alamo/` | Remove the matching part of file paths before storing them in the JSON and the plot pages. |
| `-D`, `--display-figures` | off | Also open each figure in a web browser as it is generated (interactive use). |
| `-s`, `--skip` | | Accepted but has no effect in this script. |

Without `-f`, a directory is skipped when none of its included files is newer than the time in its
`.lastprocessed` file. That file is written only after all aggregate JSON and plot output has been
written, so a failed or interrupted run is retried next time. Use `-f` after changing `RhoLUT.py`
or `FloatClasses.py`.

`xmtps-generate-summary-page.py [options] PATH ...` — reads the per-float `<serial>_summary.json`
files (searching input directories recursively) and writes the fleet summary page to
`<output>/index.html`. `DIR` for `-o` must exist. It takes only the common options above, and
always rebuilds the page from all matching summary files.

Example (as run by `update-plots-service.sh` for one float):

```
xmtps-list2json.py -i "/data1/alamo/data/list/[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" \
    -x "/\..*" -o /data1/alamo/data/json /data1/alamo/data/list/9308
xmtps-aggregate-json.py -i "/data1/alamo/data/json/[0-9]+/dives/[0-9]+_-?[0-9]+.json" \
    -o /data1/alamo/data/json -p /data1/alamo/data/plots2 /data1/alamo/data/json/9308/dives
xmtps-generate-summary-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2 /data1/alamo/data/json
```

### JSON files

The Python tools write three kinds of JSON file under the output directory (production:
`/data1/alamo/data/json/<serial>/`). Times are given twice throughout: `dt_epoch` (Unix seconds,
UTC) and `dt_str` (ISO 8601, e.g. `2026-10-25T11:05:32Z`). Pressure is in dBar, temperature in
°C, salinity in PSU; `dens` and `sigma0` are in-situ density and potential density referenced to
the surface, minus 1000 kg/m³, from `RhoLUT.py`.

#### Per-dive files: `dives/<serial>_<dive>.json`

Written by `xmtps-list2json.py`, one per listing file (pre-mission dives are negative, e.g.
`9213_-003.json`). Each file is a `FloatClasses.Dive` object serialized with
[jsonpickle](https://jsonpickle.github.io/): every object carries a `py/object` tag naming its
class, and its attributes are written by name, so the attribute names in `FloatClasses.py` are
part of the format (`json_vers`, currently `0.11`, identifies it). Missing values are written as
`NaN`, which Python's `json` module reads but strict JSON parsers do not.

```
{"py/object": "FloatClasses.Dive", "json_vers": "0.11",
 "id": {"py/object": "FloatClasses.FloatId", "whoi": 9287, "dac": -1, "wmo": -1},
 "list_file": {"py/object": "FloatClasses.File", "path": "data/list/9287/lis9287-0325.txt",
               "mtime": {"py/object": "FloatClasses.DateTime", "dt_epoch": 1792937148.35,
                         "dt_str": "2026-10-25T14:05:48Z"}},
 "dive_num": 325,
 "profile": {"py/object": "FloatClasses.Profile", "pres": [278.04, 277.6, ...], ..., "n": 278},
 ...}
```

| Attribute | Class | Contents |
|---|---|---|
| `id` | `FloatId` | Serial number (`whoi`), DAC and WMO IDs (`-1` if unknown). |
| `list_file` | `File` | Path and modification time of the listing the file was made from. |
| `dive_num` | | Dive number. |
| `descent`, `ascent` | `Descent`, `Ascent` | Fall and rise time series: `pres`, `dt_epoch`/`dt_str`, `dt_ref` (the series start) and `dt_ref_delta_sec`, `rate` (dBar/s), `phase_code`. |
| `drift` | `Drift` | Drift samples: `pres`, `temp`, `saln`, `dens`, `sigma0` and their times (measured when the float sends drift times, otherwise estimated between descent and ascent). |
| `profile` | `Profile` | Binned profile: `pres`, `temp`, `tcnd` (second temperature channel), `saln`, `saln_corr` (dynamically corrected salinity, NaN if not sent), `dens`, `sigma0`. |
| `surface` | `Surface` | Surface time series: `pres`, `temp`, `saln` and times. |
| `pump` | `Pump` | Pump actions: `pres`, `time` (s), `volt`, `curr`, `curr_adj`, `vac`, `power`, `energy`. |
| `gps`, `sbd` | lists of `Gps`, `Sbd` | GPS fixes and the dive's SBD messages (position, CEP, MOMSN, packet ID, size). |
| `wave` | `Wave` | Wave spectra (`Szz_g` … in g²/Hz, `Szz` … in m²/Hz), directional moments and derived values: significant wave height `swh` (m), periods (s), mean direction `mwd` (°). |
| `mission` | `MissionParameters` | Firmware and configuration versions, CTD type, `parameters` (`{name: value}`) and `entries` (section, index, name, value, units of every `m` line). |
| `engineering` | `Engineering` | One object per engineering section (`descent`, `drift`, `ascent`, `surface`, `ice`, `bit`) with an attribute per parameter, `errors` (error codes) and `extra` (columns not mapped to an attribute). |

#### Aggregate files: `<serial>_<name>.json`

Written by `xmtps-aggregate-json.py`: one table per float, made by concatenating all its dives.
Each file is a pandas DataFrame in "split" orientation, without an index: a list of `columns`
and a list of `data` rows. Missing values are `null`; every table has a `dive` column.

```
{"columns": ["dive", "pres", "temp", "tcnd", "saln", "saln_corr", "dens", "sigma0"],
 "data": [[0, 131.52, 20.056, 19.863, 34.627, null, 25.035, 24.473], ...]}
```

Read one with `pandas.read_json(path, orient="split")`.

| File | One row per | Columns |
|---|---|---|
| `_profile` | profile bin | `pres`, `temp`, `tcnd`, `saln`, `saln_corr`, `dens`, `sigma0` |
| `_descent`, `_ascent` | time-series sample | `pres`, `dt_epoch`, `dt_ref_delta_sec`, `dt_str`, `phase_code`, `rate` |
| `_drift` | drift sample | `pres`, `temp`, `saln`, `dens`, `sigma0`, `dt_epoch`, `dt_ref_delta_sec`, `dt_str` |
| `_surface` | surface sample | `dt_epoch`, `dt_str`, `pres`, `temp`, `dt_ref_delta_sec` |
| `_pump` | pump action | `pres`, `time`, `volt`, `curr`, `curr_adj`, `vac`, `power`, `energy` |
| `_gps` | GPS fix | `dt_epoch`, `dt_str`, `lat`, `lon`, `cep_km`, `idx`, `ttf_sec`, `nsat`, `snr_min`, `snr_avg`, `snr_max`, `hdop` |
| `_sbd` | SBD message | `dt_epoch`, `dt_str`, `lat`, `lon`, `cep_km`, `size_b`, `momsn`, `mtmsn`, `pid` |
| `_iridium_position` | dive | Iridium position composited from the dive's messages: time (with its range `dt_min_*`/`dt_max_*`), `lat`, `lon`, `cep_km`, `num_pos` |
| `_profile_position` | dive | Position assigned to the profile: time, `lat`, `lon`, `cep_km`, `src` (`gps` or `iridium`) |
| `_displacement_surface`, `_displacement_submerged` | dive | Displacement between two positions: `azi` (°), `dist_m`, `duration_s`, `speed_mps`, and `pos1_*`/`pos2_*` |
| `_cycle_timing` | dive | Start time and duration of descent, drift, ascent and surface, and `total_duration_s` |
| `_wave` | dive | `swh`, `mwd`, `period_peak`, `period_avg`, `period_zero` |
| `_wave_spectra` | spectral bin | `freq`, acceleration (`*_g`, g²/Hz) and displacement spectra (m²/Hz), `a0`–`b2`, `r1`, `r2`, `theta1`, `theta2` |
| `_engineering` | dive | Start time and case vacuum of each engineering section (`descent_*`, `drift_*`, `ascent_*`, `surface_*`) |
| `_engineering_data` | dive | Every engineering parameter, as `section.attribute` columns (e.g. `drift.mean_pres_dbar`); columns no dive reports are left out |
| `_engineering_errors` | error | `section`, `idx`, `dt_str`, `code` |
| `_mission` | dive with mission data | `fwver`, `cfgver`, `ctdtype` and one column per mission parameter (`MSN`, `DTD0`, …) |

#### Float summary: `<serial>_summary.json`

Also written by `xmtps-aggregate-json.py`, and read by `xmtps-generate-summary-page.py` for the
summary page. It is a jsonpickle object (indented) with the float's `json_vers`, `id`, `fwver`,
`ctdtype` and `type` (`ALTO` for serial numbers above 10000, otherwise `ALAMO`); `age`, the time
in seconds from deployment to the latest SBD message; `profdepthsum`, the sum of each dive's
deepest pressure (dBar); and the `gps` and `sbd` positions at `startup`, `deployment` and
`latest`, where `latest` also has `cycle` (the last dive), `duration` (s), `nsbd` (its number of
SBD messages) and `depth` (the deepest pressure of the dive, dBar). Unset numbers are `-999`.

#### `dives/.lastprocessed`

The modification time (Unix seconds) of the newest dive JSON file read by the last aggregate
run; it is written once the run has written its output. `xmtps-aggregate-json.py` skips a float
when none of its dive files is newer, unless `-f` is given.

## Changelog

### 2026-10-25

Decoder daemon:

- Startup: the active-float query selects only `DISTINCT id_instrument` instead of whole
  `XFloatMail` rows (with their SBD data); it took 88 s instead of 248 s at the 2026-10-25
  restart, still as a table scan. `XFloatMail.iridium_position_datetime` has since been indexed,
  which lets the query read only the recent rows.
- Mail messages are no longer lost when the SQL server is unavailable. The mail processing
  thread writes a message's files and saves it before queueing it for decoding; if that fails
  and the SQL server cannot be reached (`SELECT 1`), the message is held and retried every
  10 s instead of being dropped (logged once when the outage starts and ends). The mail
  retrieval thread does not fetch while the SQL server is unavailable, so mail stays on the
  server, and with `mail.moveMessages` a message is moved out of the inbox only after it has
  been processed, not when it is fetched. Messages still unsaved at shutdown stay on the mail
  server and are fetched again at the next start.
- `XMfaMissionCfg::DecodeXMsgData` (6,504 lines) and `GenerateListingString` (5,584 lines) are
  split into one member function per firmware layout (`DecodeXMsgDataFw<version>`,
  `GenerateListingStringFw<version>`, plus `…Unsupported` and `GenerateListingStringIncomplete`);
  the two functions only dispatch on the firmware version. Decoding all 211,007 production
  mission messages (78,694 dives, 93 firmware versions) gives byte-identical listings before and
  after.

- Mission configuration: 25 more firmware versions are decoded instead of being reported as
  unsupported, mapped to existing layouts that were identified from production messages:
  10.2.3.1, 10.4.x, 10.6.0.11, 10.6.1.32 and 10.6.4.1 use the 10.5/10.6 layout; 10.7.0.1,
  10.7.1.4, 10.7.2.11, 10.8.0.1, 10.9.x (except 10.9.1.31), 10.10.0.x, 10.12.0.4, 10.12.0.34,
  10.13.0.13 and 10.14.0.28 the 10.7/10.8 layout; 10.7.1.31 and 10.9.1.31 the WIMS layout;
  7.3.4.0 the 7.2 layout. This covers 3,191 stored dives, including the active floats 11531,
  11533, 11041, 11498-11500, 11060, 11678, 11700-11712, 11773-11776 and the 9287-9306 wave
  floats. Checked on all 211,007 production mission messages: only those dives change, the
  calibration constants decode to their usual values, MSN equals the dive number, and the
  decoded drift target agrees with the measured drift pressure on 94% of 660 dives (the rest
  are float behaviour, e.g. 11678 holding a constant pressure above its target).
- Shutdown: saving the float cache no longer fails after a long uptime. The daemon thread's
  connection is idle from startup to shutdown and was dropped by the server's idle timeout, so
  every save failed (seen at the 2026-10-25 restart after 10.8 h); the connection is now checked
  and replaced first, and if the SQL server is down one warning is logged instead.

Build:

- The Release makefiles are regenerated from the Debug ones with `-O2 -g` (they referenced Boost
  sources that no longer exist and linked without the libraries). `XMfaMissionCfg.cpp` is built
  with `-O1`: GCC did not finish it at `-O2` within an hour.
- The build number is incremented with each commit (`build/hooks/pre-commit`) instead of with
  each build.

Python tools:

- Bathymetry in the trajectory plots is interpolated bilinearly between the ETOPO2 grid cells
  (it was the nearest cell, and the grid spacing taken from two float32 coordinates put it up to
  a third of a cell off). ETOPO2 is read from `/data1/etopo2/` by default; `--etopo2` is an
  alias of `-b/--bathymetry`.
- Docstrings for all modules, classes and functions of the Python tools (module docstrings with
  usage examples replace the editor templates; inaccurate ones corrected); no code changed.

Documentation:

- The README describes the listing file format (layout rules, every line type, and examples)
  and describes the SQL backend in general instead of listing its tables and columns.
- The README documents the JSON files written by the Python tools: the per-dive jsonpickle
  files, the aggregate tables, the float summary and `.lastprocessed`.
- `sql/xmtps-schema.sql` creates the database tables (the production schema, with the new
  `XFloatMail.iridium_position_datetime` index); it is referenced from the SQL backend section.

### 2026-10-24

Decoder daemon:

- Wave: the v2 spectral decoder no longer reads a sample past the last bin (the `;` terminator
  plus one byte beyond the message). For floats sending 51-sample packets (9287, 9291) this
  spurious bin failed the 0.3 g²/Hz check, so no wave products were produced; 128-sample packets
  decode as before.
- Wave: bin frequencies now include the packet's first-sample index, and `wvm` (m²/Hz) columns
  use the same order as `wvs` again (zz, yy, xx, xy, zx, zy), matching their header. Builds
  3234–3287 printed them as xx, xy, yy, zz under that header.
- New: dynamically corrected salinity (X-message 0xDC), sent by floats with dynamic CTD
  correction enabled (PPE = 2), is decoded as a second binned-profile salinity channel and listed
  as an extra `salnCorr` column on the `pro` lines. The format was inferred from production
  packets: a salinity message with four extra bytes (0x00, part index, 0x01, 0x00) after the size
  field.
- New: drift sample times (X-messages 0x78–0x7F) are decoded. For floats that send them, the
  drift section uses the fall/rise layout: `D SN dive nSam driftStart`, then `d` lines with the
  time (`+seconds` since the drift start, `(UTC/+sec)`) before pressure, temperature and
  salinity. The format (inferred) matches the drift pressure/temperature/salinity messages.
- New listing section "Profile Data (Binned) - Sensor Columns", before the binned profile, lists
  each profile column present for the dive as a `PRD`/`prd` table (column name and units, with
  the description after `#`, wrapped to keep lines under 100 characters).
- Pressures are listed with two decimal places (previously three) in every section; the fill
  value is still `-9.999`, and column widths are unchanged. The surface pressure variance keeps
  three decimals.
- Apart from pressure decimals, listings for floats that send neither 0xDC nor 0x78 are
  unchanged.
- Mission configuration message 1 for 10.8.0.4/10.12.0.x accepts 88 bytes (with CTD correction
  parameters) as well as 71.
- Fixed mission configuration offsets: `ddt` in five firmware layouts, `descend_start_time` for
  2.0.69–2.5.62, `isa` overlapping `isd` from 5.0.61 on, and `sao` read across the message
  terminator.
- Fixed BIT v8 temperature and air-valve calibration offsets, ice engineering test
  salinity/temperature range checks (salinity always read 2.000), surface engineering v0x10/v0x12
  bounds, and the XArgo v2.1 surface salinity offset.
- Surface pressure variance is now divided by gain² (it was 1/gain too large).
- Added length checks for short or malformed X-messages, bounded curvature sub-blocks, accepted
  phase code 15, and cleared auxiliary temperature data on dive reset.

Python tools:

- `xmtps-list2json.py` reads `pro` and `d` lines by the column names in their header, so it
  handles the new `salnCorr` column and the drift-times layout as well as the aux-temperature
  format. Drift sample times are the `D` line's drift start plus each `d` line's `time` offset.
- Measured drift sample times are used when a listing has them; otherwise drift times are still
  estimated between the end of descent and the start of ascent.
- Profiles carry `saln_corr` (dynamically corrected salinity) in the dive JSON and the aggregated
  profile table; it is NaN for floats that don't send it.
- Mission parameters: every `m` line is kept with its section, index, value and units
  (`mission.entries` in the dive JSON; the `#` description is not kept);
  `mission.parameters` keeps non-numeric values
  instead of failing on them. The `MIS` line also gives `cfgver`, and the firmware version is
  read correctly from older listings that have a minor index on that line.
- Engineering data: the Engineering classes in `FloatClasses.py` now define every parameter of
  every descent, drift, ascent, surface, ice and BIT message version as an explicit attribute
  named with its units (e.g. `drift.mean_pres_dbar`, `surface.prof_num_bins`,
  `bit.air_pump_current_max_a`). Each `E0x` line is parsed by its column header and mapped to
  these attributes through the `ENG_COLUMNS` table; columns not in the table go into
  `engineering.extra`, and the `e0x` error lines go into `engineering.errors`. Unreported
  parameters keep NaN, '' or an unset time. The old fixed-position E07–E0A parsing was removed.
- `xmtps-aggregate-json.py` also writes `<id>_mission.json` (one row per dive with mission
  parameters), `<id>_engineering_data.json` (one row per dive, `section.attribute` columns,
  dropping columns no dive reports) and `<id>_engineering_errors.json`.
- Trajectory plots show, behind the other traces and for dives 1 and later, the target drift
  pressure as a thin dashed light grey step line (`DTD<n>` for the current mission phase `MPS`, or
  `DTD`, changing at the start of descent of each dive; once a phase's cycle count is used up,
  `MPC` ≥ `MNM<n>`, the dive drifts in the next phase `MNS<n>`, since the reported phase is from
  before the switch) and the bathymetry at the GPS fixes as a thin black line ("Bathymetry
  (ETOPO2)" in the legend). Depth comes from NOAA ETOPO2v2c, interpolated bilinearly between the
  2′ grid cells and converted to pressure with Saunders (1981). The pressure axis range is
  unchanged, so a seafloor deeper than the float's data is off the plot. The file is read from
  `/data1/etopo2/ETOPO2v2c_f4.nc` by default (`-b/--bathymetry/--etopo2 FILE`); without it the
  bathymetry line is left out with a warning.
- `xmtps-aggregate-json.py` plotting is split into one function per plot group
  (`WriteGeoPlots` … `WriteWavePlots`, run in order from `PLOT_WRITERS`); the plot output is
  unchanged.
- Plot images are written through one persistent kaleido/Chromium browser instead of a new
  browser per image (2.2 s → 0.6 s per image): a full aggregate of 9213 takes 47 s instead of
  309 s, and of 11186 103 s instead of 382 s, with identical PNG and HTML output. The browser
  is started only when there are floats to plot; if it can't start, images are written one
  browser at a time as before.

Operations:

- Both systemd units restart after 30 s (`RestartSec=30`); the repository copies now match the
  installed units (decoder runs with `-v`; plot PID file name).

Documentation:

- Added this README, documented all command-line, configuration and Python tool options and the
  SQL schema, fixed stale Doxygen comments, and added the Doxygen PDF manual.

### 2026-10-23

Decoder daemon:

- Fixed the mail retrieval thread busy-looping on an unparseable mail file (now renamed to
  `.err`), and replaying a whole batch when one server message failed.
- Worker threads now drain their queues on shutdown, and termination flags are atomic.
- Fixed the latest Iridium position being compared against the GPS time instead of the Iridium
  time.
- Fixed composite Iridium positions: latitude/longitude were swapped in the variance calculation,
  and longitudes are now averaged correctly across ±180°.
- Mail parsing: "Message Size" is now parsed, the Date header timezone is converted to UTC
  correctly, and SBD attachments must be at least 8 bytes.
- The mail fetch limit is applied after filtering for `.raw`/`.txt` files, and the mail user and
  password are percent-encoded in the server URL.
- Status files now follow `gen.stat` instead of `gen.kml`; archived floats go into the KML
  "archive" folder; floats with a single GPS fix get a latest-position marker.
- Metadata: launch times at hour or minute 0 are accepted and hour 24 is rejected, a bad field no
  longer stops parsing of the rest of the file, launch positions accept any whitespace, and DAC ID
  9999 placeholders are ignored.
- Added mission configuration support for firmware 10.12.1.31 (WIMS).
- The run-time PID lock is created exclusively and removed only by its owner.
- The post-processing thread keeps going when one float's KML or status file fails.
- A float's record is saved before its dives (fixes foreign-key failures for new floats), and the
  shutdown save continues past a failing float.
- Added ALTO WMO instrument types 873, 875 and 876.
- Fixed ice engineering (v0x11) messages being rejected whenever they reported error codes.
- Identical retransmitted SBD packets are no longer decoded twice.
- Stopped tracking the Debug/Release binaries; build number 3287.

Python tools:

- Updated to JSON version 0.11: descent/ascent phase codes in `xmtps-list2json.py`, shared logger
  setup, and a reworked summary page (elapsed time, page-update stamp, auto refresh).
- Fixed the RhoLUT maximum temperature (39 °C, was 34 °C); pressures below the table now use its
  minimum pressure instead of 0.
- Fixed drift fill values, a crash on 0 V pump records, and `.lastprocessed` being written before
  output succeeded.
- Wave directional coefficients now use the displacement formulation; `r1` and `r2` are distinct
  and lie in [0, 1].
- Fixed the drift σ₀ average plot title, wave plots not being linked when a float has one wave
  dive, and plot path handling.

Operations:

- The decoder service waits 30 s between restarts (`RestartSec=30`).

### 2026-08-11

- The post-processing thread closes its SQL connection after each pass, avoiding "lost connection
  to MySQL server" errors.

### 2026-08-06

- Added mission parameter support for firmware 0x0A0C0001 (10.12.0.1, RBR).

### 2026-08-05

- Initial commit.

## Author and license

Written and maintained by Alexander K. Ekholm, WHOI Float Group, Department of Physical
Oceanography, Woods Hole Oceanographic Institution. Released under the GNU General Public License.
