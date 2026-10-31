# XMTPS — X-Message Telemetry Processing Service

XMTPS is the WHOI Float Group daemon that receives Iridium Short Burst Data (SBD) telemetry from
ALAMO and ALTO profiling floats, decodes it, stores it in MySQL, and writes the listing, PHY, KML,
status and RAW files used downstream. A set of Python tools in `bin/py` turns the listing files
into JSON and generates the per-float plot pages and the fleet summary page, which are published
at [argo.whoi.edu/alamo](https://argo.whoi.edu/alamo/) (one page per float at
`https://argo.whoi.edu/alamo/<serial>/`, e.g. [9294](https://argo.whoi.edu/alamo/9294/)).

## Contents

- [How it works](#how-it-works)
  - [Source layout](#source-layout)
- [Building](#building)
  - [Versions](#versions)
- [Configuration](#configuration)
  - [How options are read](#how-options-are-read)
  - [Command-line only options](#command-line-only-options)
  - [Configuration file options](#configuration-file-options)
  - [Example configuration file](#example-configuration-file)
  - [Float metadata](#float-metadata)
  - [Surface pressure offset correction](#surface-pressure-offset-correction)
- [Running](#running)
  - [Without the mail or SQL server](#without-the-mail-or-sql-server)
  - [Output files](#output-files)
- [Listing files](#listing-files)
  - [Layout rules](#layout-rules)
  - [Line types](#line-types)
  - [Examples](#examples)
- [SQL backend](#sql-backend)
- [Deployment](#deployment)
- [Python tools (`bin/py`)](#python-tools-binpy)
  - [Command-line options](#command-line-options)
  - [JSON files](#json-files)
    - [Per-dive files: `dives/<serial>_<dive>.json`](#per-dive-files-divesserial_divejson)
    - [Aggregate files: `<serial>_<name>.json`](#aggregate-files-serial_namejson)
    - [Float summary: `<serial>_summary.json`](#float-summary-serial_summaryjson)
    - [`dives/.lastprocessed`](#diveslastprocessed)
- [Changelog](#changelog)
- [Author and license](#author-and-license)

## How it works

Iridium delivers each SBD message as an email with a binary `.sbd` attachment. The binary
X-message format the floats send is specified in
[spec/xmessage-format.md](spec/xmessage-format.md), which is also given as plain text, JSON,
YAML and a PDF built from LaTeX (`spec/xmessage-format.{txt,json,yaml,tex,pdf}`). The markdown
is the source of truth: after changing it, regenerate the other formats with
`python spec/build-formats.py` (standard library only; the PDF needs XeLaTeX). Every other file
XMTPS and its Python tools read or write is specified in [spec/](spec/README.md). XMTPS processes them in a four-stage
pipeline of worker threads, connected by bounded queues:

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
  data/xmsg/                  X-message decoders (GPS, profiles, drift, engineering, ice, wave, ...);
                              the format is specified in spec/xmessage-format.md
  utility/                    logging, MySQL/SQL table helpers, KML, queues, file-system helpers
bin/
  py/                         JSON conversion, plotting and summary-page tools
  xmtps-alamo.service         systemd unit for the decoder daemon
  xmtps-alamo-plot.service    systemd unit for the plot-generation loop
build/                        build-number script and counter
example/                      real example files from one dive (see example/README.md)
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

### Versions

The versions production (`argus.whoi.edu`) builds and runs with, as of 2026-10-25. The C++
libraries are the Ubuntu packages; other versions may work but are not tested.

| Component | Version | Notes |
|---|---|---|
| OS | Ubuntu 26.04.1 LTS (kernel 7.0) | |
| Compiler | g++ 15.2.0, GNU Make 4.4.1 | compiled as C++11 (`--std=c++11`) |
| Boost | 1.90.0 (`libboost-dev` 1.90.0.1ubuntu3) | linked shared (`BOOST_ALL_DYN_LINK`); the binary loads `chrono`, `filesystem`, `log`, `program_options` and `thread` |
| VMime | 0.9.2 (`libvmime-dev` 0.9.2-9build1) | TLS through GnuTLS (`libgnutls.so.30`) |
| MySQL Connector/C++ | 1.1.12 (`libmysqlcppconn-dev` 1.1.12-4.1ubuntu4, `libmysqlcppconn.so.7`) | legacy JDBC-style API (`sql::Connection`); uses `libmysqlclient.so.24` and OpenSSL 3 |
| libkml | 1.3.0 (`libkml-dev` 1.3.0-13) | |
| MySQL server | 8.4.11 | `sql_mode` must allow zero dates (see [SQL backend](#sql-backend)) |
| Python | 3.14.4 (`/usr/bin/python3.14`), in the venv `/data1/alamo/bin/py/.venv` | the update-plots service activates it for every tool |
| numpy | 2.5.4 | |
| ppigrf | 2.1.0 | IGRF geomagnetic model for the wave direction declination correction; without it, wave directions stay referenced to magnetic north |
| scipy | 1.18.1 | `scipy.interpolate` (`RhoLUT.py`), `scipy.io.netcdf_file` (ETOPO 2022) |
| pandas | 3.1.0 | |
| plotly | 7.1.0 | embeds plotly.js 4.1.1 |
| kaleido | 1.5.0 (with `choreographer` 1.4.0, `logistro` 2.0.1) | PNG export; drives Google Chrome for Testing 142.0.7444.175, which choreographer downloaded to `~/.local/share/choreographer` |
| jsonpickle | 4.1.4 | |
| geographiclib | 2.1 | |
| Pillow | 13.0.0 | |
| yattag | 1.16.1 | |
| python-dateutil | 3.0.0 | |
| pytz | 2026.6 | |
| orjson, simplejson | 3.13.0, 4.2.0 | used by plotly's JSON encoder (orjson) and by choreographer (simplejson) |
| ETOPO 2022 | ETOPO 2022 v1, 60 arc-second surface elevation (NOAA NCEI), `/data1/etopo2022/` | bathymetry in the trajectory plots. `ETOPO_2022_v1_60s_N90W180_surface.nc` is NOAA's netCDF-4 file; the plots read `ETOPO_2022_v1_60s_N90W180_surface_classic.nc`, made from it with `nccopy -k 64-bit-offset`, because `scipy.io.netcdf_file` reads only classic netCDF. ETOPO2v2c (`/data1/etopo2/ETOPO2v2c_f4.nc`) still works with `-b`. |

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
| `-D`, `--daemon` | off | Run as a daemon: keep running until a termination signal or until the run-time lock is lost. Without it, the program starts the pipeline, waits briefly, and exits; when reading RAW mail files (`mail.fetchMode = 1`) it first reads every file in `dir.mail`, then finishes processing them and exits (see [Without the mail or SQL server](#without-the-mail-or-sql-server)). |
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
| `dir.mail` | `raw/incoming` | RAW mail input files (file-system fetch mode): `*.raw` or `*.txt` files, each holding one email as received (headers and MIME body). |
| `dir.mail.processed` | `raw/processed` | RAW mail input files are moved here once queued for processing (a file of the same name already here is kept). If set empty, they are deleted instead. |
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
[Surface pressure offset correction](#surface-pressure-offset-correction)):

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
| `mail.fetchMode` | `0` | `0` fetch from the mail server; `1` read RAW mail files from `dir.mail` (renamed to `.err` if they can't be parsed), with no mail server settings needed and no connection made. |
| `mail.moveMessages` | `0` | Move server messages to the folder `INBOX.<YYYYMM>` for their header date once they are processed (saved to the SQL database or rejected); a message whose processing is interrupted stays in the inbox. |
| `mail.timeout` | `120` | Seconds without data from the mail server after which an operation (connect, login, fetch, copy, delete, expunge) is canceled; the connection is closed and reopened at the next check, and messages not yet moved are moved then. |
| `mail.xDlyS` | `60` | Seconds to wait before checking again when no new messages are found. |
| `mail.validSenderRegex` | `(sbdservice@sbd.iridium.com\|service@sbd.pac.disa.mil)` | Senders whose messages are accepted. |
| `mail.validSubjectRegex` | `SBD Msg From Unit: (\d+)` | Accepted subjects; the captured number is the IMEI. |
| `mail.validSbdFilenameRegex` | `(\d+)_(\d{6})\.sbd` | Accepted SBD attachment file names. |

The mail user and password may contain any characters; they are percent-encoded when the server
URL is built.

`[sql]` — MySQL server:

| Option | Default | Description |
|---|---|---|
| `sql.enable` | `1` | Use the database. `0` runs without it: no connection is made, nothing is loaded or saved, and the other `sql.*` options are ignored (see [Without the mail or SQL server](#without-the-mail-or-sql-server)). |
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

Supported WMO instrument types: 866, 867, 868 (ALAMO) and 873, 875, 876 (ALTO). The instrument
type decides whether the surface pressure offset correction is applied when the dive's mission
configuration message does not give the sensor type (see below).

### Surface pressure offset correction

Floats with an RBR CTD report pressure without a surface offset correction, so XMTPS computes
the offset from the pressure the float measures while it is at the surface and subtracts it from
the dive's pressures. SBE CTDs correct pressure onboard and are not corrected again.

- **Which floats**: the sensor type comes from the dive's mission configuration message (the
  `MIS` line: 0 = RBR, corrected; 1 = SBE, not corrected), when its firmware layout is
  supported. Floats need not send that message every dive, so a dive without one uses the sensor
  type of the float's last mission configuration message that gave it. Otherwise the WMO
  instrument type in the float's `.meta` file decides: 866, 867 and 875 are corrected; 868, 873,
  876, unknown types and floats without a `.meta` file are not.
- **Offset**: for each dive, the minimum surface pressure sample after outliers are discarded
  (samples farther from the median than 3 × 1.4826 × the median absolute deviation, or than
  0.1 dBar if that is larger). The offset is the mean of these minima over the dive and up to
  three preceding dives whose surface data is valid (a complete surface series, a GPS fix, and a
  mean surface pressure within ±5 dBar) and that have at least 5 surface pressure samples, so
  that the minimum is not taken from too few samples.
- **Applied to**: the fall, drift, profile, valve, pump and rise pressures in the listing. The
  surface time series (`SFC`, `sfc`, `SFD`) is listed uncorrected, since it is what
  the offset is computed from. The `PRC` line gives the offset (`presOffset`), whether it is
  valid, and the number of samples and dives it comes from, followed by this dive's uncorrected
  surface pressure minimum (`sfcPmin`, outliers excluded, as used for the offset), mean, maximum
  and standard deviation (-9.999 when the dive has no usable surface data). A note above it says whether this
  listing's pressures are corrected: if so, `pres = rawPres - presOffset`, so the raw pressures
  are recovered as `rawPres = pres + presOffset` (except samples listed as -9.999, which were out
  of range after the correction).

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

### Without the mail or SQL server

XMTPS can decode a directory of saved emails with neither a mail server nor a database, for
example to test a change or to decode messages away from the production host:

- `mail.fetchMode = 1` reads the emails from files in `dir.mail` instead of the mail server. Each
  `*.txt` or `*.raw` file holds one email as received from the Iridium gateway, headers and MIME
  body with the base64 SBD attachment (like [`example/mail/`](example/mail/)). Leading `#` and
  blank lines are skipped, so standard-format RAW files (`gen.raw.format = 0`) can be read back;
  the extended-format ones (the default) don't contain the email. Files are read in name order,
  so name them in the order the messages were sent (for example by MOMSN): the decoder carries
  state such as the sensor type from one dive to the next. No `mail.*` server option is needed.
  A file is moved to `dir.mail.processed` once queued, or renamed to `.err` if it can't be
  parsed, so it is not read again.
- `sql.enable = 0` turns the database off: nothing is loaded from or saved to it, and no
  connection is made. Each float is built in memory from the messages read in this run, so give
  it every message of the dives to decode, plus the three dives before each (their surface
  pressure goes into the [pressure offset](#surface-pressure-offset-correction)) and the next
  dive (its start-of-dive GPS fix is the profile position when the dive has no good end-of-dive
  fix). A float whose dives don't all send a mission configuration message also needs the
  last dive that did, for its sensor type. KML files show only the positions read in this run,
  and RAW files give `Message UID: -1`, as the UID comes from the database. Reprocessing needs
  the stored messages, so `-r` and `-R` are rejected, and `-A` and the reprocessing implied by
  `--phy-enable` are skipped. `mail.moveMessages = 1` with the mail server is rejected, since
  messages would leave the inbox without being saved. A message whose subject gives no serial
  number is matched to a float read in this run by its IMEI, or dropped.
- Without `-D`, a run with `mail.fetchMode = 1` reads every file in `dir.mail`, finishes
  processing them (listing, RAW, SBD, PHY, KML and status files), and exits.

Example: decode the emails in `./mail-in` into `./out`:

```ini
# nosql.cfg
[dir]
meta                    = /data1/alamo/meta/dac
mail                    = mail-in
mail.processed          = out/mail-done
lis                     = out/list
raw                     = out/raw
sbd                     = out/sbd
kml                     = out/kml
stat                    = out/stat
phy                     = out/phy
log                     = out/log

[mail]
fetchMode               = 1

[sql]
enable                  = 0
```

```
mkdir -p out/log
xmtps -v --config nosql.cfg --pid-file out/xmtps.pid
```

The log directory must exist; the others are created as needed.

### Output files

| Output | Location |
|---|---|
| RAW mail | `<dir.raw>/<serial>/raw<serial>-<MOMSN>-<dive>-<packet>.txt` |
| Listings | `<dir.lis>/<serial>/lis<serial>-<dive>.txt` |
| PHY, KML, status, SBD | under `dir.phy`, `dir.kml`, `dir.stat`, `dir.sbd` |

The [`example/`](example/) folder has a real example of each file, all from float 9303's dive
440: the Iridium [email](example/mail/), the [SBD](example/sbd/) attachments, the
[RAW](example/raw/) files, the [listing](example/listing/lis9303-0440.txt), and the
[JSON](example/json/) and [plot](example/plots/) files the Python tools make from it (see
[example/README.md](example/README.md)).

Decoded data, messages, floats and dives are also stored in the MySQL database, unless
`sql.enable = 0` (see [SQL backend](#sql-backend)).

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
| `A` | Argo data | Mission phase, pressures, durations, cycle timing and sensor gains and offsets (`midx` 0–5). |
| `PRC` | Pressure correction | Surface pressure offset (`presOffset`), whether it is valid, and the dives and samples it comes from, then the dive's uncorrected surface pressure min (outliers excluded), mean, max and standard deviation; a note above it says whether it was subtracted from this listing's pressures and how to recover the raw ones (see [Surface pressure offset correction](#surface-pressure-offset-correction)). |
| `F` / `f` | Fall time series | Descent: time, pressure and phase code (name after `#`); listings before build 3321 also have temperature and salinity columns before the phase code. |
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
| `CMD` | Command echo-back | One line per command the float echoed back after a command upload (X-message 0xDE): SN, dive, echo index, command index and the command text, with the parameter name in uppercase, e.g. `DFT 0 100`. |
| `XFF` | Extended records | One line per X-message 0xFF record (format inferred; spec C.6): SN, dive, record index, record type and version, and the decoded record: event code, time and value (0xCA), reset counter and times (0xD1), serial number and configuration bytes (0xAE), CTD command and reply (0xDB), processor fault dump text (0xFA), or the bytes in hex. |
| `WIP` | WIMS configuration parameters | One line per WIMS parameter in an X-message 0xC5, which a WIMS float sends in dive 0 and after a `WCG -1` upload (format and most names inferred; spec C.7): SN, dive, message index, version, part, parameter index, name (`?` when inferred, `-` when unknown), value and hex. |
| `SHX` / `shx` | SBD message hex data | Each SBD message as a hex dump, 16 bytes per line. |

### Examples

A complete listing file is in [example/listing/lis9303-0440.txt](example/listing/lis9303-0440.txt).
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
offset in seconds, the pressure and the phase code. The fall and rise series (`f`, `r`) list no
temperature or salinity; the surface series (`sfc`) adds them after the pressure:

```
#      SN dive idx nSam                 time      pres phaseCode   phaseCodeName
#                                  (UTC/+sec)    (dBar)
F    9287  325   0   16 2026-10-24T07:37:09Z
f    9287  325   0    0                   +0      0.12         1 # start of descent
f    9287  325   0    1                 +293     30.08        10 # descending
f    9287  325   0    2                 +387     43.52         2 # first descent pump
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
  start limit. `TimeoutStopSec=300` gives the daemon 5 minutes to finish the messages still
  queued before systemd kills it.
- `xmtps-alamo-plot.service` runs `bin/py/update-plots-service.sh`, also with `Restart=always` and
  `RestartSec=30`.

The installed copies live in `/etc/systemd/system/`. After installing or changing a unit file, run
`sudo systemctl daemon-reload`.

## Python tools (`bin/py`)

These tools turn the listing files into JSON and generate the plot pages published at
[argo.whoi.edu/alamo](https://argo.whoi.edu/alamo/).

Requires Python 3.12 or later (tested with 3.12 and 3.14; pandas 2.3 and 3.x) with `numpy`, `scipy`, `pandas`, `plotly` (plus `kaleido` for PNG export),
`jsonpickle`, `geographiclib`, `pytz`, `python-dateutil`, `Pillow` and `yattag`; see
[Versions](#versions) for the versions production uses.

| Script | Purpose |
|---|---|
| `xmtps-list2json.py` | Listing files → per-dive JSON, with density and σ₀ (`RhoLUT.py`) and wave statistics (`FloatClasses.py`). |
| `xmtps-aggregate-json.py` | Per-dive JSON → per-float aggregate JSON and, with `-p`, the float's plots and page. |
| `xmtps-generate-summary-page.py` | Fleet summary page (`index.html`). |
| `xmtps-generate-mission-page.py` | Mission parameters page (`engineering/params.html`). |
| `xmtps-generate-startup-page.py` | Float startup page (`engineering/startup.html`). |
| `update-plots-service.sh` | Plot service loop: runs the two converters for floats with recent listings, then rebuilds the three pages. |

All scripts use JSON version `0.11` and ignore JSON files with any other version.

### Command-line options

Each script takes one or more input paths (directories and/or files). Include patterns are
regular expressions searched in the full path (with `/` separators), so they need only match part
of it. Options common to all scripts:

| Option | Default | Description |
|---|---|---|
| `-d`, `--debug` | | Print debug output. |
| `-v`, `--verbose` | | Print informational output (used if `-d` is not given). |
| `-q`, `--quiet` | | Print only errors (used if neither `-d` nor `-v` is given). |
| `-H`, `--halt-on-error` | off | Stop at the first error instead of logging it and continuing. |
| `-i`, `--include REGEX` | all files | Process only files whose path matches `REGEX`. Repeatable; a file is processed if it matches any pattern. |
| `-o`, `--output DIR` | `.` | Output directory (see each script). |

With no logging option, only warnings and errors are printed.

`xmtps-list2json.py [options] PATH ...` — converts listing files into per-dive JSON files in
`<output>/<serial>/dives/`. A listing is converted only if its JSON file is missing or older,
unless `-f` or `-s` is given.

| Option | Default | Description |
|---|---|---|
| `-r`, `--recursive` | off | Descend into subdirectories of the input directories. |
| `-x`, `--exclude REGEX` | none | Skip directories and files whose path matches `REGEX`. Repeatable. |
| `-f`, `--force` | off | Rewrite every dive JSON file. |
| `-s`, `--skip` | off | Skip listings whose dive JSON already exists, even if the listing is newer. |
| `-S`, `--strip-path REGEX` | `.*/alamo/` | Remove the matching part of the listing path before storing it in the JSON. |

`xmtps-aggregate-json.py [options] PATH ...` — reads a float's dive JSON files (`PATH` is usually
its `dives` directory) and writes the aggregate JSON files to `<output>/<serial>/`. Listing fill
values (`-9.999`, `65535`, ... per column; see `FILL_VALUES`) become NaN before anything is
plotted or saved. A directory is skipped unless one of its files is newer than its
`.lastprocessed` time; that file is written only after all output, so a failed run is retried.
Use `-f` after changing `RhoLUT.py` or `FloatClasses.py`.

| Option | Default | Description |
|---|---|---|
| `-p`, `--plots DIR` | none | Also write the plots and the float's `index.html` to `DIR/<serial>/`. `DIR` must exist. |
| `-r`, `--recursive` | off | Descend into subdirectories of the input directories. |
| `-x`, `--exclude REGEX` | none | Skip subdirectories whose path matches `REGEX`. Repeatable. |
| `-f`, `--force` | off | Process every directory, ignoring `.lastprocessed`. |
| `-S`, `--strip-path REGEX` | `.*/alamo/` | Remove the matching part of file paths stored in the JSON and plot pages. |
| `-D`, `--display-figures` | off | Also open each figure in a web browser (interactive use). |
| `-s`, `--skip` | | Accepted; no effect. |

`xmtps-generate-summary-page.py [options] PATH ...` — rebuilds `<output>/index.html` from all
`<serial>_summary.json` files under `PATH` (searched recursively).

The three page generators take `--runtime SECONDS`, the duration of the JSON and plot updates,
shown in the page footer; the summary page footer also shows the decoder's status, uptime and
version. The mission parameters and startup pages also take `--plots-url URL`, where the float
pages and status icons are (default `..`; the service passes `/alamo`), and `--days DAYS`. In
production they are written to `/data1/alamo/data/plots2/engineering/` (created by
`update-plots-service.sh`) and published at `http://argus.whoi.edu/etcetc/alamo/engineering/`.
Their tables have a search field, like the summary page.

`xmtps-generate-mission-page.py [options] PATH ...` — writes `params.html` from each float's
`<serial>_summary.json` and `<serial>_mission.json`:

- **Rows:** every deployed float (one with a completed profile) that has sent mission parameters;
  with `--days`, only those that reported within `DAYS` days. Each row shows the parameters from
  the float's latest dive that has any, with its firmware, last report, status (ACTIVE within 30
  days, OVERDUE within 180, otherwise ARCHIVED) and the dive.
- **Tables:** one per float type, ALAMO (including WIMS and MM2) and ALTO; floats are compared only
  within their table.
- **Columns:** phase `MPS`; pressures `DTD`, `PFD`/`PFM`, `MDS`, `CTF`, `ASD`; timing `PTO`, `SGT`,
  `SET`, `APT`; descent `DFT`, `DCM`, `PSM`; ascent `ARR`, `APM`; surface `SSP`, `SAM`, `SAO`; ice
  `ICM`, `ICT`; `PPE` and `WST`/`WMT`.
- **Phases:** `DTD`, `PFD`, `PFM`, `PTO` and `DFT` have a column per phase, P0–P2 (firmware without
  phases uses P0). The current phase is `MPS`, or `MNS<MPS>` once `MPC` reaches `MNM<MPS>`; its
  values are bold and underlined, and the Phase column shows `MPC`/`MNM`. Phases the mission
  never reaches, or with `MNM` 0, are gray and left out of the statistics.
- **Colors:** in groups of three or more floats, each value is colored by how rare it is in its
  group (1 − its floats / the most common value's floats): yellow to red above the most common
  value, blue below. `DFT` is colored by its distance from the group median instead (white
  within 25%, full color at 200%). The footer shows the most common value (`DFT`: the median) and
  the 10th–90th percentile range. Unset values (255, 65535, ...) are shown as "unknown" and not
  counted; impossible values (phase outside 0–2, drift pressure outside 0–7000 dBar, drift or
  profile pressure deeper than `MDS`) are dark red. Hovering over a cell shows the parameter, its
  value and how many floats share it.

`xmtps-generate-startup-page.py [options] PATH ...` — writes `startup.html` from each float's
`<serial>_summary.json` and `<serial>_engineering_data.json`, listing the floats that started up
in the last `--days` days (default 180), latest first:

- **Startup:** each startup's messages have the next negative dive number (-1, -2, ...), so the
  startup number is minus the dive of the latest BIT, and the startup time is the time that BIT
  reports (or the first SBD message, for BIT formats without times).
- **Columns:** status (Started or Deployed, with the startup number and cycles); startup,
  deployment and last report times; BITs sent and failed; and the latest BIT: duration, failed
  tests (spec B.2.6), subsystem error flags, firmware and BIT format, CTD pressure after
  calibration with temperature and salinity, vacuum and battery voltage (start / end), and for the
  air and oil pumps the duration and current, voltage and power (formats 9-11 report all of them,
  format 8 only the maximum currents and battery voltage).
- **Colors:** passed BITs green, failed BITs and error flags dark red, started floats yellow.
  The table is sortable by any column.

Example (as run by `update-plots-service.sh` for one float):

```
xmtps-list2json.py -i "/data1/alamo/data/list/[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" \
    -x "/\..*" -o /data1/alamo/data/json /data1/alamo/data/list/9308
xmtps-aggregate-json.py -i "/data1/alamo/data/json/[0-9]+/dives/[0-9]+_-?[0-9]+.json" \
    -o /data1/alamo/data/json -p /data1/alamo/data/plots2 /data1/alamo/data/json/9308/dives
xmtps-generate-summary-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2 --runtime 185 /data1/alamo/data/json
xmtps-generate-mission-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2/engineering --plots-url /alamo --runtime 185 /data1/alamo/data/json
xmtps-generate-startup-page.py -i "/data1/alamo/data/json/[0-9]+/[0-9]+_summary.json" \
    -o /data1/alamo/data/plots2/engineering --plots-url /alamo --runtime 185 /data1/alamo/data/json
```

### JSON files

The Python tools write three kinds of JSON file under the output directory (production:
`/data1/alamo/data/json/<serial>/`); [example/json/](example/json/) has one of each, and
[example/plots/](example/plots/) some of the plots made from them. Times are given twice throughout: `dt_epoch` (Unix seconds,
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
| `surface` | `Surface` | Surface time series: `pres`, `temp`, `saln` and times; and the surface pressure offset correction from the `PRC` line: `pres_correction` (dBar, NaN without a `PRC` line), `pres_correction_valid`, `pres_correction_num_samples`, `pres_correction_num_dives`. |
| `pump` | `Pump` | Pump actions: `pres`, `time` (s), `volt`, `curr`, `curr_adj`, `vac`, `power`, `energy`. |
| `gps`, `sbd` | lists of `Gps`, `Sbd` | GPS fixes and the dive's SBD messages (position, CEP, MOMSN, packet ID, size). |
| `wave` | `Wave` | Wave spectra (`Szz_g` … in g²/Hz, `Szz` … in m²/Hz), directional moments and derived values: significant wave height `swh` (m), periods (s), mean direction `mwd` (°). Bins not received are NaN. The displacement spectra have the noise-floor mask (bins where `Szz_g` is below 0.0005 g²/Hz are zeroed) and the low-frequency response correction (cosine taper, [f0, w] = [0.02595, 0.08834] Hz) applied alike to all six spectra. Directions are clockwise from true north (magnetic north when the declination is unavailable) and give the direction the waves propagate toward, not from. A dive without a valid GPS fix uses the declination at the float's most recent valid fix after deployment. |
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
| `_surface` | surface sample | `dt_epoch`, `dt_str`, `pres`, `temp`, `dt_ref_delta_sec`, `pres_corr` (the dive's valid pressure correction, else NaN) |
| `_pump` | pump action | `pres`, `time`, `volt`, `curr`, `curr_adj`, `vac`, `power`, `energy` |
| `_gps` | GPS fix | `dt_epoch`, `dt_str`, `lat`, `lon`, `cep_km`, `idx`, `ttf_sec`, `nsat`, `snr_min`, `snr_avg`, `snr_max`, `hdop` |
| `_sbd` | SBD message | `dt_epoch`, `dt_str`, `lat`, `lon`, `cep_km`, `size_b`, `momsn`, `mtmsn`, `pid` |
| `_iridium_position` | dive | Iridium position composited from the dive's messages (CEP⁻²-weighted): time (with its range `dt_min_*`/`dt_max_*`), `lat`, `lon`, `cep_km` (GPS-calibrated: `sqrt(1 / sum(CEP^-2) + 1.55^2)`), `num_pos` |
| `_profile_position` | dive | Position assigned to the profile: time, `lat`, `lon`, `cep_km`, `src` (`gps`; `iridium`, the composite, for a dive without a GPS fix when its `cep_km` is at most 2 km; or `none`) |
| `_displacement_surface`, `_displacement_submerged` | dive | Displacement between two positions: `azi` (°), `dist_m`, `duration_s`, `speed_mps`, and `pos1_*`/`pos2_*` |
| `_cycle_timing` | dive | Start time and duration of descent, drift, ascent and surface, and `total_duration_s` |
| `_wave` | dive | `swh`, `mwd`, `declination` (°, east positive; NaN if not applied), `direction_ref` (`true` or `magnetic`), `period_peak`, `period_avg`, `period_zero` |
| `_wave_spectra` | spectral bin | `freq`, acceleration (`*_g`, g²/Hz) and displacement spectra (m²/Hz), `a0`–`b2`, `r1`, `r2`, `theta1` (mean direction toward, [0, 360)), `theta2` (principal direction toward, [0, 360), the end of the principal axis nearest `theta1`) |
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

See [CHANGELOG.md](CHANGELOG.md).

## Author and license

Written and maintained by Alexander K. Ekholm, WHOI Float Group, Department of Physical
Oceanography, Woods Hole Oceanographic Institution. Released under the GNU General Public License.
