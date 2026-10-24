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

## Database schema

XMTPS stores its state in five InnoDB tables in the MySQL schema named by `sql.schema` (production:
`xmtps_alamo`). Each table is mapped to one C++ class through a static
`util::mysql::SqlTableDescription`. The description lists the column names and the getter and setter
used to read and write each column, and generates the `SELECT`, `DELETE` and
`INSERT ... ON DUPLICATE KEY UPDATE` statements. Saving an object therefore inserts a new row or
overwrites the existing row with the same primary key.

| Table | Class | Primary key | One row per | Written by |
|---|---|---|---|---|
| `XFloat` | `XFloat` | `id_instrument` | instrument | startup, stage 3 and shutdown |
| `XFloatMetadata` | `XFloatMetadata` | `id_instrument` | instrument, from its `.meta` file | saved with `XFloat` |
| `XFloatDive` | `XFloatDiveData` | `id_instrument`, `id_dive` | dive cycle | stage 3 |
| `XGPS` | `XGPS` | `id_instrument`, `id_dive`, `datetime` | GPS fix | stage 3, saved with the dive |
| `XFloatMail` | `XFloatMailMessage` | `id_dive`, `id_imei`, `id_sbd_momsn`, `id_sbd_packet`, `id_msg` | mail message (one SBD packet) | stage 2 |

Relationships (foreign keys):

```
XFloat (id_instrument)
 ├── XFloatMetadata.id_instrument               fk_XFloatMetadata_1
 └── XFloatDive.id_instrument                   fk_XFloatDive_1
      └── XGPS (id_instrument, id_dive)         fk_XGPS_1
XFloatMail                                      no foreign key; linked by id_instrument / id_imei
```

Because of these keys a float's `XFloat` row must exist before its dives are saved, and a dive's row
before its GPS fixes; `XFloatDiveData::SaveObjectToDatabase()` saves the parent float first.
`XFloatMail` has no foreign key, so a message can be stored before its float is known (stage 2).

Common identifier columns:

| Column | Meaning |
|---|---|
| `id_instrument` | Float serial number (the instrument ID used throughout XMTPS). |
| `id_imei` | IMEI of the float's Iridium modem. |
| `id_wmo` | WMO platform number (`-1` if not assigned). |
| `id_dac` | Argo data assembly center ID. |
| `id_dive` | Dive (profile) number. |

Unset values use sentinel defaults rather than `NULL`: `-1` for IDs, `1400-01-01 00:00:00` for
dates, `99.99999`/`999.99999` for latitude/longitude, and `9999` for gains and offsets.

### `XFloat`

The current state of each float, reloaded at startup so that processing resumes where it left off.

- Identifiers: `id_instrument`, `id_imei`, `id_wmo`, `id_dac`, and `argo_instrument_type` (Argo
  instrument type code).
- Calibration: `gain_*` and `offset_*` for pressure, salinity and temperature, with separate
  `*_surface` values for the surface sample.
- Latest positions: `latest_gps_position_*` and `latest_iridium_position_*` (date/time, dive number,
  latitude, longitude, and Iridium CEP radius), used for the KML and status files.
- Iridium sequence numbers: `latest_sbd_momsn` and `latest_sbd_mtmsn`, the most recent
  mobile-originated and mobile-terminated message sequence numbers.
- Output switches: `phy_file_generation_is_enabled` and `netcdf_file_generation_is_enabled`.
- Mission: `start_of_mission_datetime`, and `xmsg_version_major`/`xmsg_version_minor` (the
  X-Message format version the float reports).
- `latest_iridium_startup_position_datetime` and `latest_iridium_startup_sbd_momsn` exist in the
  production table but are not read or written by the current code.

### `XFloatMetadata`

Deployment metadata parsed from the float's `.meta` file (see [Float metadata](#float-metadata)).

- Deployment: `deployment_datetime`, `deployment_latitude`/`deployment_longitude`,
  `deployment_position_valid`, the `deployment_*_status` fields, `deployment_platform`,
  `deployment_cruise_id` and `deployment_type`.
- People: `deployer`, `deployer_address`, `principal_investigators`,
  `principal_investigators_address`, `principal_investigators_originating_country`.
- Instrument: `manufacturer`, `wmo_instrument_type` (for example 873, 875 or 876 for ALTO floats),
  and `wmo_recorder_type`.
- Other: `comment`, and `local_path`, the path of the `.meta` file.

### `XFloatDive`

One dive cycle, built from all the SBD messages received for it.

- `complete`: the dive has a complete profile, known sensor gains and offsets, contiguous
  message IDs and a valid position.
- `num_sbd_msgs`: the number of SBD messages received for the dive.
- `iridium_position_datetime_first`/`_last`: the first and last Iridium fix times for the dive.
- `intermediate_listing_file_path` and `phy_file_path`: the output files written for the dive.
- `surface_pres_*`: statistics of the surface pressure samples (min, max, mean, variance, valid).
- `pres_offs*`: the pressure offset applied to the profile, with the number of dives and samples it
  was computed from and whether it is valid.

### `XGPS`

One GPS fix reported in an X-Message.

- `datetime`, `latitude`, `longitude` and `valid`.
- `id_type`: the GPS message type.
- Fix quality: `hdop`, `num_satellites`, `snr_min`, `snr_mean`, `snr_max`, and `time_to_fix`.

### `XFloatMail`

Every Iridium SBD mail message received, so that a float can be reprocessed from the database with
`-r`/`--reprocess`.

- Keys: `id_imei`, `id_sbd_momsn`, `id_sbd_packet` and `id_dive` identify the packet; `id_msg` is
  the mail Message-ID; `id_msg_uid` is the IMAP UID; `id_sbd_mtmsn` is the MTMSN.
- Mail: `msg_header_datetime`, `msg_header_from`, `msg_header_subject`, `msg_header_raw_text`,
  `msg_body_text` and `msg_raw_text`, each with an `*_is_valid` flag, and `msg_is_valid` for the
  whole message.
- Iridium session: `iridium_position_*` (latitude, longitude, CEP radius, date/time, valid), and
  `sbd_session_status_code`/`sbd_session_status_string`.
- Attachment: `sbd_data` (the raw SBD bytes), `sbd_data_is_valid`, `sbd_file_name` and
  `sbd_file_size_bytes`.
- Files written: `sbd_file_local_path` and `raw_file_local_path`.
- `id_dac` and `id_wmo` exist in the production table but are not written by the current code, so
  they keep their default of `-1`.

Reprocessing (`-r ID`) deletes the float's `XGPS` and `XFloatDive` rows and rebuilds them from its
`XFloatMail` rows. The full `CREATE TABLE` statements are in the Doxygen documentation (the
"Libraries and Database Schema" page in `doc/xmtps.pdf`).

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
