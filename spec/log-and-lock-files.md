# XMTPS Log, Lock and Run-Time Files

This document specifies the files the XMTPS decoder daemon creates and uses while it runs, other than its data products: the daily log files, the run-time lock (PID) file, and the lock and error files of the RAW-mail input mode. The daemon (`xmtps`) writes all of them; operators and monitoring scripts read them. The output of the Python plot service, which goes to the systemd journal rather than to files, is covered at the end.

Status: compiled on 2026-10-30 from `src/Main.cpp`, `src/utility/Log.h`, `src/utility/Log.cpp`, `src/config/LocalConfiguration.h`, `src/utility/SignalHandler.h`, `src/stage1_MailRetrievalThread.cpp` and `bin/py/update-plots-service.sh` at commit `5e77db6`, and from the production host `argus.whoi.edu`.

## Contents

1. [Overview of run-time files](#1-overview-of-run-time-files)
2. [Daemon log files](#2-daemon-log-files)
3. [Console output](#3-console-output)
4. [Run-time lock (PID) file](#4-run-time-lock-pid-file)
5. [Signals and shutdown](#5-signals-and-shutdown)
6. [RAW mail input lock and error files](#6-raw-mail-input-lock-and-error-files)
7. [Plot service output](#7-plot-service-output)
8. [Open questions / discrepancies](#8-open-questions--discrepancies)

## 1. Overview of run-time files

| File | Path | Written by | Read by | Section |
|---|---|---|---|---|
| Daily log | `<dir.log>/YYYY-MM-DD.log` | daemon | operators | 2 |
| Daily error log | `<dir.log>/YYYY-MM-DD.error.log` | daemon | operators | 2 |
| PID file | `pid-file` option (default `/var/lock/xmtps/lock.pid`; production `/data1/alamo/var/lock/xmtps.pid`) | daemon | daemon, systemd, operators | 4 |
| RAW mail input lock | `<dir.mail>/<name>.lck` | daemon (fetch mode 1 only) | daemon | 6 |
| Rejected RAW mail input | `<dir.mail>/<name>.err` | daemon (fetch mode 1 only) | operators | 6 |
| Configuration file | `--config` | operator | daemon, at startup | [configuration-file-format.md](configuration-file-format.md) |
| Float metadata | `<dir.meta>/<DAC-ID>_<serial>.meta` | operator | daemon, at startup | [float-metadata-format.md](float-metadata-format.md) |

The data products (RAW, SBD, listing, PHY, KML and status files, and the database) are specified in [raw-file-format.md](raw-file-format.md), [sbd-file-format.md](sbd-file-format.md), [listing-file-format.md](listing-file-format.md), [phy-file-format.md](phy-file-format.md), [kml-file-format.md](kml-file-format.md), [status-file-format.md](status-file-format.md) and [database-schema.md](database-schema.md). The daemon writes no temporary or cache files besides those listed [inferred from a search for file writes in `src/`].

## 2. Daemon log files

### 2.1 Names, location and rotation

`Log::InitializeLog()` in `src/utility/Log.h` adds two Boost.Log text-file sinks and one console sink:

| Sink | File name pattern | Records |
|---|---|---|
| Main log | `<dir.log>/%Y-%m-%d.log` | All records that pass the log level (section 2.4). |
| Error log | `<dir.log>/%Y-%m-%d.error.log` | Records of severity `warning`, `error` and `fatal` only. They also appear in the main log. |
| Console | standard output | Same records as the main log (section 3). |

Rules:

| Property | Behavior |
|---|---|
| Directory | `dir.log` must exist and be a directory; it is not created. Otherwise startup fails with `Invalid path to log directory:<path>`. Production: `/data1/alamo/var/log/xmtps`. |
| Date in the name | The local date when the file is opened. `argus` runs in UTC, so production names are UTC dates. |
| Rotation | At local midnight (`rotation_at_time_point(0, 0, 0)`). There is no size limit; size-based rotation is commented out. |
| Open mode | Append (`ios::out \| ios::app`). A restart on the same day appends to that day's files. |
| Retention | None. The daemon never deletes or compresses old logs. Production holds 7190 files (3623 days, from 2016-08-08) totaling 31 GB. |
| Ownership | The daemon's user. Production: `argo:floatgroup`, mode `0644`. |
| Encoding | Bytes as logged. Messages may contain raw bytes from decoded telemetry, including NUL (for example `Invalid accelerometer sensitivity scale factor: ^@`), so use `grep -a`. |
| Buffering | File sinks do not flush each record. The daemon thread flushes all sinks about every 30 seconds (`XFLT_DAEMON_FLUSH_LOG_SEC`) and at shutdown. The newest records can lag by up to 30 seconds. |

Typical sizes in production: about 0.4 MB per day for the main log and 10 to 20 kB for the error log in normal operation. A day with restarts is much larger, because each start reprocesses the floats active in the last 90 days (`-A` is on by default): 2026-10-28 had four starts and a 400 MB main log.

### 2.2 Line format

Each record starts a new line:

```
<timestamp> <thread> <severity> <context><message>
```

| Field | Width | Format | Source |
|---|---|---|---|
| Timestamp | 26 | `YYYY-MM-DDTHH:MM:SS.ffffff`, local time (UTC on `argus`), microseconds. Followed by one space. | `TimeStamp` attribute (`add_common_attributes()`), format `%Y-%m-%dT%H:%M:%S.%f` |
| Thread | 12 | Thread name, right-aligned and space-padded (section 2.5). | `ThreadName` attribute |
| Severity | 7 | `  fatal`, `  error`, `warning`, `   info`, `  debug`, `  trace`, right-aligned. | `Severity` attribute (Boost trivial severity) |
| Context | 72 | Float, dive, SBD and X-message identifiers, or dashes (section 2.3). | `WhoiID`, `DacID`, `WmoID`, `DiveID`, `MsgUID`, `MomsnID`, `PcktID`, `XmsgID` attributes |
| Message | rest | Free text. Leading and trailing whitespace is trimmed. It may contain newlines (section 2.6). | Log call |

Fields are separated by single spaces. The context block ends with a space, so the message begins at column 121 (1-based).

### 2.3 Context block

The context block is built from scoped thread attributes that the pipeline sets while it works on a float, dive or message. It has four parts, each followed by one space:

| Part | Width | When the attribute is set | When not set |
|---|---|---|---|
| `Float[<serial>\|<DAC>\|<WMO>]` | 26 | `Float[` + serial zero-padded to 5 digits + `\|` + DAC ID zero-padded to 5 digits if 1000 ≤ DAC < 10000, else `?????` + `\|` + WMO ID as 7 digits if 1000000 ≤ WMO < 10000000, else `???????` + `]` | 26 dashes; the remaining parts are then dashes too |
| `Dive[<dive>]` | 10 | Dive number right-aligned in 4 characters if −1 ≤ dive < 10000, else `????`. Shown if `DiveID` or `MsgUID` is set. | 10 dashes |
| `SBD[<uid>\|<momsn>\|<packet>]` | 22 | Message UID (6 digits, if 0 ≤ UID < 1000000, else `??????`), MOMSN (6 digits, same range), packet index (3 digits, if 0 ≤ packet < 256, else `???`). Shown if `MsgUID` is set. | 22 dashes |
| `Xmsg[0x<ID>]` | 10 | X-message blob ID in upper-case hex (see [xmessage-format.md](xmessage-format.md)). Shown while a blob is decoded. | 10 dashes |

The ranges are checked, the padding is not: a serial number above 99999 would widen the block. Examples from production:

```
-------------------------- ---------- ---------------------- ----------
Float[09453|06139|6903434] ---------- ---------------------- ----------
Float[09453|06139|6903434] Dive[ 339] ---------------------- ----------
Float[11190|?????|???????] Dive[  31] SBD[??????|000620|014] ----------
Float[11190|?????|???????] Dive[  31] SBD[??????|000621|013] Xmsg[0x80]
```

Notes:

- `?????`/`???????` in the float part mean the DAC or WMO ID is unknown (no metadata file, or a placeholder), or out of range. Five-digit DAC IDs (10000 and up) are also shown as `?????` (section 8, item 1).
- A message UID is the database key of the mail message. It is unknown (−1, shown as `??????`) until the message is first saved, so new messages show `??????` and reprocessed ones show their UID [inferred from `src/stage2_MailProcessingThread.cpp`].
- Dive −1 is the pre-mission dive.
- Log files written before about 2017 use 25 dashes instead of 26 in the first part.

### 2.4 Severities and log levels

The code logs through macros in `src/utility/Log.h`. Each has a level; a record is written only if its level is at or below the reporting level set on the command line.

| Level | Value | Macro | Written as | Default (`LOG_LOG`) | `-v` (`LOG_INF`) | `-q` (`LOG_WRN`) | `-d` (`LOG_DBG3`) |
|---|---|---|---|---|---|---|---|
| `LOG_FAT` | 0 | `lfat()` | `fatal` | yes | yes | yes | yes |
| `LOG_ERR` | 1 | `lerr()` | `error` | yes | yes | yes | yes |
| `LOG_WRN` | 2 | `lwrn()` | `warning` | yes | yes | yes | yes |
| `LOG_LOG` | 3 | `lout()`, `llog()` | `info` | yes | yes | no | yes |
| `LOG_INF` | 4 | `linf()` | `info` | no | yes | no | yes |
| `LOG_DBG` | 5 | `ldbg()` | `debug` | no | no | no | debug builds only |
| `LOG_DBG1`–`LOG_DBG3` | 6–8 | `dbg1()`–`dbg3()` | `trace` | no | no | no | debug builds only |

- `LOG_LOG` and `LOG_INF` are both written as `info`; the file does not show which one a record came from.
- `debug` and `trace` records are compiled only into debug builds (`BUILD_DEBUG`, version suffix `a`). In a release build (suffix `b`, as in production) `-d` gives the same output as `-v`.
- Production runs with `-v`.
- `fatal` is not used in practice: no `fatal` record appears in the production logs checked.
- The error log takes `warning` and above, whatever the reporting level.

### 2.5 Thread names

| Name | Thread | Count | Source |
|---|---|---|---|
| `daemon` | Main thread: startup, metadata loading, lock checks, log flushing, shutdown | 1 | `main()` in `src/Main.cpp` |
| `mail-fetch` | Stage 1: mail retrieval, startup reprocessing | 1 | `src/stage1_MailRetrievalThread.cpp` |
| `mail-process` | Stage 2: header checks, RAW and SBD file writing | `nProcThrd` | `src/stage2_MailProcessingThread.cpp` |
| `sbd-decode` | Stage 3: X-message decoding, listing and PHY files | `nDecoThrd` | `src/stage3_TelemetryDecoderThread.h` |
| `post-process` | Stage 4b: KML and status files | 1 | `src/stage4b_PostProcessingThread.h` |

Several threads of one stage share a name. A `ThreadNum` attribute is declared in `src/utility/Log.h` but never set or shown.

### 2.6 Multi-line records

Records logged by the exception macros (`CATCH_BLOCK`, `CATCH_NOTHROW` in `src/utility/Log.h`) span several lines. The first line has the normal prefix; the following lines have none. They give the source file and line, the function, and a `Caused by:` chain with Boost diagnostic information, ending at the original exception. Debug builds also log hex dumps over several lines. A parser should treat any line that does not start with a timestamp as a continuation of the previous record.

Example (production error log, 2026-10-28):

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

When the cause is a `std::regex_error`, a separate `error` record `regex_error caught: error_<code>` precedes it.

### 2.7 Notable messages

Startup (thread `daemon`), in order:

```
2026-10-28T22:08:06.404915       daemon    info -------------------------- ---------- ---------------------- ---------- Registering system signal handlers...
2026-10-28T22:08:06.404994       daemon    info -------------------------- ---------- ---------------------- ---------- Done. Successfully registered system signal handlers.
2026-10-28T22:08:06.405005       daemon    info -------------------------- ---------- ---------------------- ---------- Loading persistent instrument data from the SQL database...
2026-10-28T22:08:06.435107       daemon    info Float[02987|?????|???????] ---------- ---------------------- ---------- Loaded instrument data from database.
...
2026-10-28T22:08:06.437235       daemon    info -------------------------- ---------- ---------------------- ---------- Done. Successfully loaded persistent instrument data.
2026-10-28T22:08:06.437248       daemon    info -------------------------- ---------- ---------------------- ---------- Loading metadata from the local metadata files...
2026-10-28T22:08:06.456338       daemon warning -------------------------- ---------- ---------------------- ---------- Encountered 96 unsupported metadata fields: "transmission ID number = 011337" ...
...
2026-10-28T22:08:07.229441       daemon    info Float[11052|08005|4903230] ---------- ---------------------- ---------- Loaded instrument metadata.
2026-10-28T22:08:07.229474       daemon    info -------------------------- ---------- ---------------------- ---------- Done. Successfully loaded metadata from the local metadata files.
2026-10-28T22:08:07.229560       daemon    info -------------------------- ---------- ---------------------- ---------- Starting worker threads...
2026-10-28T22:08:07.229994       daemon    info -------------------------- ---------- ---------------------- ---------- Done. All worker threads successfully started.
2026-10-28T22:08:07.230242   mail-fetch    info -------------------------- ---------- ---------------------- ---------- Reprocessing active floats...
2026-10-28T22:08:07.401938   mail-fetch    info -------------------------- ---------- ---------------------- ---------- Found 71 floats reporting in the last 90 days.
```

The configuration, lock and logger steps come before this and are not logged to file (section 3). A start is best found by `Registering system signal handlers...`.

Shutdown (thread `daemon`):

```
2026-10-28T22:08:05.295386       daemon    info -------------------------- ---------- ---------------------- ---------- Caught SIGTERM! Shutting down...
2026-10-28T22:08:05.295424       daemon    info -------------------------- ---------- ---------------------- ---------- Terminating worker threads...
2026-10-28T22:08:05.571114       daemon    info -------------------------- ---------- ---------------------- ---------- Done. All worker threads successfully terminated.
2026-10-28T22:08:05.571167       daemon    info -------------------------- ---------- ---------------------- ---------- Saving local cache of instrument data to database...
2026-10-28T22:08:06.132158       daemon    info -------------------------- ---------- ---------------------- ---------- Done. Local data saved successfully.
2026-10-28T22:08:06.132212       daemon    info -------------------------- ---------- ---------------------- ---------- Releasing run-time lock...
2026-10-28T22:08:06.132451       daemon    info -------------------------- ---------- ---------------------- ---------- Done. Run-time lock successfully removed.
```

Most frequent records in normal operation (2026-10-29, with `-v`):

| Thread | Severity | Message | Meaning |
|---|---|---|---|
| `mail-fetch` | info | `Fetched N new message(s) from mail server...` | One fetch batch. |
| `mail-process` | info | `Processing message...` | A message passed the header checks. |
| `mail-process` | info | `Writing RAW file: "<path>"...`, `Writing SBD file: "<path>"...` | RAW and SBD outputs. A RAW file named `raw<serial>-<MOMSN>-incomplete-<date-time>.txt` is a message without valid SBD data. |
| `mail-process` | warning | `Ignoring mail message with invalid header.` | Sender or subject did not match the `mail.valid*Regex` options. |
| `mail-process` | warning | `Ignoring mail message with invalid serial number.` | Serial number outside 1000..11999, for example 65535. |
| `mail-process` | warning | `Ignoring mail message with invalid binary SBD attachment.` | No valid SBD attachment, or an unsuccessful Iridium session status. |
| `sbd-decode` | info | `Decoding message data...`, then `Decoding <kind> message...` per blob | One per SBD message, and one per X-message blob (with `Xmsg[0x..]`). |
| `sbd-decode` | info | `Dive complete.` | All packets of a dive have arrived. |
| `sbd-decode` | info | `Writing listing file: "<path>"...` | Listing output. |
| `sbd-decode` | warning | `Pressure offset not computed. Zero valid dives found.` | No dive with usable surface data for the pressure offset. |
| `post-process` | info | `Writing KML file: "<path>"...`, `Writing KML master file: "<path>"...`, `Writing status file: "<path>"...` | KML and status outputs. |
| `daemon` | error | `Daemon thread encountered fatal exception!` | The main loop ended on an exception, usually a failed lock check (section 4); followed by shutdown. |

## 3. Console output

The daemon also writes to standard output:

1. Before the logger exists, plain lines without the record prefix:

   ```
   ====================================================================================================

     X-Message Mail-Telemetry Processing Service
     v1.0.<build>b (built <date-time>)
     Alexander K. Ekholm, Research Engineer, Float Group
     Physical Oceanography
     Woods Hole Oceanographic Institution
     Woods Hole, MA, USA

   ====================================================================================================

   Loading configuration...
   Done.
   Acquiring run-time lock... Done. Run-time lock successfully acquired. PID is <pid>.
   Initializing logger... Done.
   ```

2. After that, every record of the main log, in the same format (console sink).

Errors before the logger exists (configuration or lock problems) go to standard error through Boost.Log's default sink and the C++ terminate handler [inferred]; see [configuration-file-format.md](configuration-file-format.md), section 6. The production unit sets `StandardOutput=null` and `StandardError=null`, so all console output, including those errors, is discarded. Only the log files remain.

## 4. Run-time lock (PID) file

The lock prevents two daemons from running at once. It is implemented in `LocalConfiguration::AcquireRuntimeLock()`, `CheckRuntimeLock()` and `ReleaseRuntimeLock()` in `src/config/LocalConfiguration.h`.

| Property | Value |
|---|---|
| Path | `pid-file` option (`-p`): default `/var/lock/xmtps/lock.pid`; production `/data1/alamo/var/lock/xmtps.pid`. |
| Directory | Created if missing. |
| Content | The daemon's process ID as decimal ASCII digits, with no newline (production: 7 bytes, `3241667`). |
| Mode | `0644` before the umask; production `-rw-r--r-- argo floatgroup`. |
| Created | After the configuration is parsed and before the logger starts, with `open(O_WRONLY \| O_CREAT \| O_EXCL)`. Two daemons starting together cannot both succeed. |

Acquisition:

1. If the file does not exist, it is created and the PID written. If the write is short, the file is removed and startup fails (`Failed to write PID file: <path>!`).
2. If the file exists, startup fails with `PID file already exists: <path>` and `PID: <pid in the file>`. The daemon does not check whether that process is still alive.
3. The new file is checked at once (step "Check", below).

Check: the daemon thread calls `CheckRuntimeLock()` every 100 ms (`XFLT_THREAD_SLEEP_MS`). The check fails if the file is missing (`PID file does not exist: <path>!`) or if the first integer in it differs from the daemon's PID (`PID mismatch (expected <pid>, got <pid>)!`). A failed check ends the main loop with `Daemon thread encountered fatal exception!`, and the daemon shuts down in order (section 5). Deleting or overwriting the PID file therefore stops the daemon within about 100 ms. Under the production unit (`Restart=always`), systemd starts it again 30 seconds later [inferred].

Release: at shutdown, `ReleaseRuntimeLock()` removes the file only if it holds the daemon's own PID. Otherwise it logs `Not removing PID file owned by another process (PID <pid>): <path>` as a warning and leaves the file.

Stale lock: if the daemon dies without shutting down (SIGKILL, or an abort during startup after the lock is taken, such as a missing log directory), the file stays and every later start fails until it is removed. The production unit names the same path in `PIDFile=`, and systemd removes that file after the service stops [inferred from systemd documentation], so under systemd the next restart succeeds. When running the daemon by hand, remove a stale file manually after checking that the PID in it is not an `xmtps` process.

## 5. Signals and shutdown

`SignalHandler::RegisterSignalHandlers()` (`src/utility/SignalHandler.h`) installs one handler for SIGABRT, SIGINT, SIGQUIT, SIGTERM and SIGTSTP. The handler only records the signal. The daemon thread sees it within 100 ms and calls `SignalHandler::ExitHandler()` (`src/Main.cpp`), which:

1. Logs `Caught <SIGNAME>! Shutting down...`. If the main loop ended for another reason (lost lock, exception, end of a non-daemon run), it logs `Caught SIGTERM! Shutting down...` although no signal arrived.
2. Stops the worker threads in pipeline order, letting each stage finish the messages already queued (`Terminating worker threads...`).
3. Saves the cached float data to the database (`Saving local cache of instrument data to database...`), or warns `SQL server unavailable: local cache of instrument data not saved.` With `sql.enable = 0` this step is skipped without a message. In a non-daemon run, the post-processing thread first writes the KML and status files once more, after the earlier stages have finished.
4. Releases the PID file (section 4).
5. Flushes the logs and exits with status 0.

SIGTSTP (Ctrl-Z) therefore shuts the daemon down instead of suspending it. SIGKILL skips all of this. The production unit allows 300 seconds (`TimeoutStopSec`) for these steps before systemd sends SIGKILL.

## 6. RAW mail input lock and error files

With `mail.fetchMode = 1` the daemon reads RAW mail files from `dir.mail` instead of the mail server (`XFloatMailRetrievalService::FetchMailMessagesFromFileSystem()` in `src/stage1_MailRetrievalThread.cpp`). Production uses mode 0, so these files do not occur there.

| File | Meaning |
|---|---|
| `<dir.mail>/<name>.raw` or `<name>.txt` | Input. Only regular files whose path matches `.*/.*\.(raw\|txt)` are read, in name order, at most `maxFetch` per batch. Each holds one e-mail as received from the Iridium gateway, not the RAW output format; leading `#` comment lines and blank lines are skipped ([iridium-mail-format.md](iridium-mail-format.md), section 13). |
| `<dir.mail.processed>/<name>.raw` or `<name>.txt` | The input file, moved here once queued. |
| `<dir.mail>/<name>.raw.processed` | The input file renamed after it was queued but could not be moved to `dir.mail.processed`, so it is not read again. Logged as a warning. |
| `<dir.mail>/<name>.raw.lck` | Created before the input file is read; contains the daemon's PID as decimal ASCII with no newline. Removed when the file is done. |
| `<dir.mail>/<name>.raw.err` | The input file renamed after it failed to parse, so it is not read again. Logged as the warning `Renaming unprocessable raw mail input file: <path>.err`. |

Processing of one input file:

1. If `<name>.lck` exists, the file is skipped with the error `Lock file already exists: <path>` and the PID in it. It is retried at every fetch, so a stale `.lck` blocks the file until it is removed by hand.
2. Otherwise `<name>.lck` is created.
3. The file is parsed and queued for processing, then moved to `dir.mail.processed` (copied, then removed; a file of the same name already there is kept). If `dir.mail.processed` is empty, the file is deleted. If the move fails, the file is renamed to `<name>.processed`.
4. If parsing fails, the file is renamed to `<name>.err`.
5. `<name>.lck` is removed.
6. If the daemon is terminating when the file would be queued, the lock is removed and the file is left for the next run.

When a fetch finds no `.raw`/`.txt` file left in `dir.mail`, the info record `All raw mail input files read from: <dir>` is logged once. A non-daemon run (no `-D`) then shuts down (section 5), so it processes every input file before it exits; with `-D` the daemon keeps checking `dir.mail` every `mail.xDlyS` seconds.

## 7. Plot service output

The plot service (`bin/py/update-plots-service.sh`, run by `xmtps-alamo-plot.service`; see [plot-and-page-files.md](plot-and-page-files.md)) writes no log files. Its standard output and standard error go to the systemd journal, under the unit `xmtps-alamo-plot` and the identifier `update-plots-service.sh`:

```
journalctl -u xmtps-alamo-plot
```

| Source | Format | Example |
|---|---|---|
| Loop script, at the start and end of each pass (every 5 minutes plus run time) | `<date -uIseconds> --- start ---` and `<date -uIseconds> --- done, run time = <seconds> ---` | `2026-10-30T18:18:24+00:00 --- start ---` |
| Loop script, when a pass exceeds 3600 seconds | `Run time exceeded. Killing children...` | |
| `xmtps-list2json.py`, `xmtps-aggregate-json.py` (run with `-v`) | Python `logging`: `%(asctime)s %(levelname)7s %(message)s`, time `%Y%m%dT%H%M%S` (local time, UTC on `argus`) | `20261030T143512    INFO Aggregating data for float 11191...` |
| `xmtps-generate-summary-page.py`, `-mission-page.py`, `-startup-page.py` (run without `-v`) | Python `logging` without a configured handler: warnings and errors only, message text only [inferred] | |

Retention is that of the journal. The unit's `PIDFile=/data1/alamo/var/lock/xmtps-plot.pid` is never written (section 8, item 5).

## 8. Open questions / discrepancies

1. **DAC IDs of 10000 and above show as `?????`.** The log formatter accepts DAC IDs only in 1000..9999 (`expr::is_in_range(attr_dac, 1000, 10000)` in `src/utility/Log.h`). Production has DAC IDs up to 10762, logged for example as `Float[11682|?????|1902778]` although the metadata loaded. Widening the range to 99999 would fix this; the 5-digit field already fits.
2. **Message UID usually unknown.** New messages are logged before they are saved, so `SBD[??????|...]` is the normal case and the UID is visible only when reprocessing. The range check (< 1000000) would also hide UIDs from one million up [inferred].
3. **No log retention.** The daemon never removes old logs (31 GB since 2016). A reprocessing start adds about 100 MB. There is no logrotate rule for `/data1/alamo/var/log/xmtps` [inferred; not checked in `/etc/logrotate.d`].
4. **Startup errors are lost under systemd.** Configuration and lock errors happen before the file logger exists and go only to standard output and error, which the unit discards. A failing start is visible only as repeated restarts in the journal (not readable by ordinary users on `argus`).
5. **Plot unit `PIDFile`.** `xmtps-alamo-plot.service` names `/data1/alamo/var/lock/xmtps-plot.pid`, which nothing writes. For `Type=simple` systemd does not need it; the line can be removed.
6. **Shutdown message names SIGTERM without a signal.** A shutdown caused by a lost lock or an exception is logged as `Caught SIGTERM!`. The preceding `Daemon thread encountered fatal exception!` record identifies these cases. The normal end of a non-daemon run is logged the same way, after `All raw mail input files read from: <dir>` in fetch mode 1.
7. **`.lck` content and check.** The input lock is created with a plain `ofstream`, not exclusively, and its PID is not checked for liveness. This is safe with one daemon per input directory, which the PID file guarantees.
