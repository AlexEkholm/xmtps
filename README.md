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

Options can be given on the command line or in a config file (`--config PATH`). Config-file
options are grouped into sections; run `xmtps --help` for the full list with defaults.

| Section | Options |
|---|---|
| (general) | `maxFetch`, `msgStartIdx`, `nProcThrd`, `nDecoThrd`, `pid-file` |
| `[dir]` | `kml`, `lis`, `log`, `meta`, `phy`, `raw`, `mail`, `mail.processed`, `sbd`, `stat` |
| `[gen]` | `kml`, `lis`, `phy`, `raw`, `raw.format`, `sbd`, `stat` — which output files to write |
| `[poc]` | `lis`, `phy` — apply the surface pressure offset correction in listing / PHY files |
| `[mail]` | `host`, `protocol`, `user`, `password`, `folder`, `fetchMode` (0 = IMAP server, 1 = file system), `moveMessages`, `xDlyS` (seconds between mail checks), `validSenderRegex`, `validSubjectRegex`, `validSbdFilenameRegex` |
| `[sql]` | `host`, `port`, `protocol`, `schema`, `user`, `password` |

Example (credentials omitted):

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

Notes:

- `gen.stat` controls status files and defaults to `false`; enable it explicitly if you want them.
- The mail user and password may contain any characters; they are percent-encoded when the server
  URL is built.
- Keep real configuration files, with their credentials, out of version control (`cfg/` is
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

Useful command-line options:

| Option | Effect |
|---|---|
| `-D`, `--daemon` | Run until a termination signal; otherwise process once and exit. |
| `-r ID`, `--reprocess ID` | Reprocess all stored messages for an instrument at startup (repeatable). |
| `-A 0\|1`, `--reprocess-active` | Reprocess floats that reported in the last 90 days. |
| `-R`, `--reprocess-all` | Reprocess every stored message. |
| `--phy-enable ID`, `--phy-disable ID` | Turn PHY generation on or off for an instrument. |
| `-v`, `-q`, `-d` | Verbose, quiet or debug logging. |

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

## Author and license

Written and maintained by Alexander K. Ekholm, WHOI Float Group, Department of Physical
Oceanography, Woods Hole Oceanographic Institution. Released under the GNU General Public License.
