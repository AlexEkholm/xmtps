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
| 2. Mail processing | `src/stage2_MailProcessingThread.*` | Validates the header and SBD attachment, resolves the float's identifiers, writes the RAW/SBD files, and saves the message to MySQL. |
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

The build number embedded in the binary comes from `build/buildNo.txt`, which
`build/incrementBuild.sh` bumps as a build step.

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
| `mail.moveMessages` | `0` | After processing, move server messages to the folder `INBOX.<YYYYMM>` for their header date. |
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

Decoded data, messages, floats and dives are also stored in the MySQL schema set by `sql.schema`.

## Deployment

The `bin/*.service` files are systemd units:

- `xmtps-alamo.service` runs the decoder daemon with `Restart=always` and `RestartSec=30`, so a
  startup failure (for example MySQL being unavailable) keeps retrying without hitting systemd's
  start limit.
- `xmtps-alamo-plot.service` runs `bin/py/update-plots-service.sh`.

After installing or changing a unit file, run `sudo systemctl daemon-reload`.

## Python tools (`bin/py`)

Requires Python 3 with `numpy`, `scipy`, `pandas`, `plotly` (plus `kaleido` for PNG export),
`jsonpickle`, `geographiclib`, `pytz`, `python-dateutil`, `Pillow` and `yattag`.

| Script | Purpose |
|---|---|
| `xmtps-list2json.py` | Converts listing files into per-dive JSON (`<out>/<serial>/dives/<serial>_<dive>.json`), adding density and σ₀ from `RhoLUT.py` and wave statistics from `FloatClasses.py`. |
| `xmtps-aggregate-json.py` | Aggregates a float's dive JSON into per-float JSON files (`<serial>_profile.json`, `_wave_spectra.json`, `_summary.json`, ...) and, with `-p DIR`, writes the plot images and the float's HTML page. |
| `xmtps-generate-summary-page.py` | Builds the fleet summary page from the per-float `_summary.json` files. |
| `update-plots-service.sh` | Loop used by the plot service: regenerates JSON and plots for floats with recent listings, then the summary page. |

All three scripts share JSON version `0.11`. They skip inputs that haven't changed since the last
run (`.lastprocessed` per dives directory, which is written only after all output succeeds). Use
`-f` to force regeneration, for example after changing `RhoLUT.py` or `FloatClasses.py`.

Wave directional coefficients use the displacement (heave/north/east) formulation:
`a1 = Qzx/√(Szz·(Sxx+Syy))`, `b1 = Qzy/√(Szz·(Sxx+Syy))`, `a2 = (Sxx−Syy)/(Sxx+Syy)`,
`b2 = 2·Cxy/(Sxx+Syy)`, with `r1 = √(a1²+b1²)` and `r2 = √(a2²+b2²)`.

## Changelog

### 2026-10-24

- Added this README.

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
- Metadata: launch times at hour or minute 0 are accepted, a bad field no longer stops parsing of
  the rest of the file, launch positions accept any whitespace, and DAC ID 9999 placeholders are
  ignored.
- The run-time PID lock is created exclusively and removed only by its owner.
- The post-processing thread keeps going when one float's KML or status file fails.
- A float's record is saved before its dives (fixes foreign-key failures for new floats), and the
  shutdown save continues past a failing float.
- Added ALTO WMO instrument types 873, 875 and 876.
- Fixed ice engineering (v0x11) messages being rejected whenever they reported error codes.
- Identical retransmitted SBD packets are no longer decoded twice.
- Stopped tracking the Debug/Release binaries; build number 3287.

Python tools:

- Updated to JSON version 0.11.
- Fixed the RhoLUT maximum temperature (39 °C, was 34 °C).
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
