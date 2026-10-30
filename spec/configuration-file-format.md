# XMTPS Configuration File Format

This document specifies the configuration file of the XMTPS decoder daemon (`xmtps`) and the command-line options that share its option set. An operator writes the file by hand; the daemon reads it once at startup with the Boost.Program_options INI-style parser. The systemd unit files that start the daemon and the plot service are covered briefly at the end, because they carry the command line.

Status: compiled on 2026-10-30 from the source at commit `5e77db6` (build 3341) and the production host `argus.whoi.edu`. The production configuration file itself is readable only by the `argo` account, so its contents were not read; production values given here are inferred from the logs and the output tree, and are marked [inferred].

## Contents

1. [How options are read](#1-how-options-are-read)
2. [File syntax](#2-file-syntax)
3. [Value types](#3-value-types)
4. [Option reference](#4-option-reference)
5. [Regular-expression options](#5-regular-expression-options)
6. [Error behavior](#6-error-behavior)
7. [Production configuration](#7-production-configuration)
8. [systemd unit files](#8-systemd-unit-files)
9. [Open questions / discrepancies](#9-open-questions--discrepancies)

## 1. How options are read

The option set is defined in two functions in `src/Main.cpp`:

- `LocalConfiguration::GenerateCommandLineOptionsDescription()` defines the command-line-only options (section 4.7).
- `LocalConfiguration::GenerateConfigurationFileOptionsDescription()` defines every option that may appear in the configuration file (sections 4.1 to 4.6).

Parsing is done by `LocalConfiguration::ParseProgramOptions()` in `src/config/LocalConfiguration.h`. The order is:

1. The startup banner is printed to standard output.
2. The command line is parsed against both option sets. Every configuration-file option can therefore also be given on the command line.
3. `--help` prints both option sets with their defaults and exits. `--version` exits at once; the version is in the banner already printed. Neither reads the configuration file.
4. The log level is set from `-d`, `-v` or `-q` (section 4.7).
5. The configuration file named by `-c`/`--config` (default `../cfg/options.cfg`) is opened and parsed against the configuration-file option set only.
6. The run-time lock (PID file) is acquired. See [log-and-lock-files.md](log-and-lock-files.md).

Precedence, from highest to lowest:

1. A value given on the command line.
2. A value given in the configuration file.
3. The built-in default (the `Default` columns below).

This follows from the Boost `store()` rule: a value that is already stored and not defaulted is never replaced by a later `store()`. The command line is stored first, so the file only fills in options that the command line did not give.

The file is read once. Changing it has no effect until the daemon is restarted.

## 2. File syntax

The parser is `boost::program_options::parse_config_file(stream, desc, false)`. Its rules, as implemented by Boost's `config_file_iterator`, are:

| Element | Rule |
|---|---|
| Line | One option or one section header per line. There is no line continuation. |
| Comment | `#` starts a comment anywhere on the line, including inside a value. Everything from the first `#` to the end of the line is removed before the line is parsed. |
| Whitespace | Spaces, tabs, CR and LF are trimmed from both ends of the line, of the option name and of the value. Inner whitespace in a value is kept. CRLF line endings are therefore accepted. |
| Blank line | Ignored. |
| Section header | A line that starts with `[` and ends with `]`. The text between the brackets becomes a prefix for the option names that follow: `[mail]` makes `host` mean `mail.host`. A `.` is appended to the prefix unless it already ends in one. The text is not trimmed, so `[ mail ]` gives the prefix ` mail .` and every option under it is unknown. |
| Option | `name = value`. The line is split at the first `=`. The full option name is the section prefix plus `name`. |
| Other line | A non-blank line that is neither a section header nor contains `=` is a syntax error (`invalid_config_file_syntax`, "unrecognized line"). |
| Option names | Case-sensitive. They must match an option of the configuration-file option set exactly (`maxFetch`, not `maxfetch`). |
| Quoting and escaping | None. Quotes are kept as part of a string value. Backslashes are passed through unchanged, which is what regular-expression values need (section 5). |
| Repeated option | An option given twice in the file is an error (`multiple_occurrences`) for every option in the file, since none of them is a list. |

Consequences:

- Options without a section (section 4.1) must come before the first section header. There is no way to return to the top level once a section has started: `maxFetch` written after `[dir]` means `dir.maxFetch`, which is unknown.
- A dotted name may be split between header and key in any way. `[gen]` + `raw.format`, `[gen.raw]` + `format`, and a top-level `gen.raw.format` (before any header) all name the same option.
- A value cannot contain `#`. This matters for passwords and regular expressions (section 9, item 1).
- An empty value (`name =`) is allowed. It is an empty string for string options, `true` for Boolean options (Boost treats an empty Boolean as true), and an error for numeric options.

Example (the repository's development file `cfg/options.cfg`, which git ignores; passwords replaced by `********`):

```ini
#--------------------------------------------------------------------------------------------------
# General Configuration Options.
#--------------------------------------------------------------------------------------------------
maxFetch                = 16
nProcThrd               = 1
nDecoThrd               = 1
pid-file                = /var/lock/xmtps/lock.pid

[dir]
kml                     = /home/aekholm/temp/xmtps/kml
lis                     = /home/aekholm/temp/xmtps/list
log                     = /home/aekholm/temp/xmtps/log
#nc                      = /home/aekholm/temp/xmtps/netcdf
meta                    = /data1/alamo/meta/dac
phy                     = /home/aekholm/temp/xmtps/phy
raw                     = /home/aekholm/temp/xmtps/raw
mail                    = /home/aekholm/temp/xmtps/raw/incoming
mail.processed          = /home/aekholm/temp/xmtps/raw/processed
sbd                     = /home/aekholm/temp/xmtps/sbd
stat                    = /home/aekholm/temp/xmtps/stat

[gen]
kml                     = false
lis                     = true
phy                     = true
raw                     = true
raw.format              = 1
sbd                     = true
stat                    = true

[poc]
lis                     = true
phy                     = false

[mail]
host                    = maila.whoi.edu
fetchMode				= 0
folder                  = INBOX
moveMessages            = false
protocol                = imap
user                    = alamo
password                = ********
validSbdFilenameRegex   = (\d+)_(\d{6})\.sbd
validSenderRegex        = (sbdservice@sbd\.iridium\.com|service@sbd\.pac\.disa\.mil)
validSubjectRegex       = SBD Msg From Unit: (\d+)
xDlyS                   = 300

[sql]
host                    = 127.0.0.1
port                    = 3306
protocol                = tcp
schema                  = xmtps_alamo
user                    = xmtps
password                = ********
```

The `fetchMode` line uses tabs before the `=`; this is valid. The commented-out `nc` lines name options that no longer exist (the NetCDF options are commented out in `src/Main.cpp`); uncommenting them would make the file fail to load.

## 3. Value types

Each option is bound to a C++ variable. The type decides how the value text is converted (Boost `validate()` / `lexical_cast`).

| Type | Accepted text | Notes |
|---|---|---|
| `bool` | `1`, `true`, `yes`, `on`, or empty for true; `0`, `false`, `no`, `off` for false. Case-insensitive. | Anything else is an `invalid_bool_value` error. |
| `bool_switch` (command line only) | No value. Present means true. | Used by `-D` and `-R`. |
| `int` | Decimal integer. | |
| `unsigned int`, `size_t` | Decimal integer. | A leading `-` is not rejected by `lexical_cast`; it wraps to a large value [inferred]. Do not use negative values. |
| `path` | A file-system path. | Relative paths are resolved against the process working directory, not against the configuration file's directory. A path containing spaces fails to convert, because `boost::filesystem::path` reads with `operator>>` [inferred]. |
| `string` | The rest of the line after trimming. | Used for host names, user names, passwords and regular expressions. |
| `vector<unsigned int>` (command line only) | One integer per occurrence, or several with `multitoken` (`-r`). | Values accumulate across occurrences. |

## 4. Option reference

In the tables, "File" is how the option is written in the configuration file and "Command line" is the equivalent command-line form. Every file option is also accepted on the command line as `--<full name> VALUE` (or `--<full name>=VALUE`).

### 4.1 General options (no section)

| File | Command line | Type | Default | Description |
|---|---|---|---|---|
| `maxFetch` | `-m N`, `--maxFetch N` | `size_t` | `127` | Maximum number of mail messages fetched from the server, or RAW mail files read from `dir.mail`, per batch. |
| `msgStartIdx` | `-M N`, `--msgStartIdx N` | `unsigned int` | `1` | Index of the first message to fetch from the mail server. Used only when `mail.fetchMode = 0`. |
| `nProcThrd` | `-n N`, `--nProcThrd N` | `unsigned int` | `1` | Number of mail processing threads (stage 2). |
| `nDecoThrd` | `-N N`, `--nDecoThrd N` | `unsigned int` | `1` | Number of telemetry decoder threads (stage 3). |
| `pid-file` | `-p PATH`, `--pid-file PATH` | `path` | `/var/lock/xmtps/lock.pid` | Run-time lock (PID) file. Its parent directory is created if missing. See [log-and-lock-files.md](log-and-lock-files.md). |

### 4.2 `[dir]` — directories

All are `path` values. Relative defaults are relative to the working directory.

| File (`[dir]` key) | Command line | Default | Description |
|---|---|---|---|
| `kml` | `--dir.kml PATH` | `kml` | KML/KMZ output. See [kml-file-format.md](kml-file-format.md). |
| `lis` | `--dir.lis PATH` | `lis` | Listing files, in per-float subdirectories. See [listing-file-format.md](listing-file-format.md). |
| `log` | `--dir.log PATH` | `log` | Daemon log files. Must exist; it is not created. See [log-and-lock-files.md](log-and-lock-files.md). |
| `meta` | `--dir.meta PATH` | `meta` | Float metadata (`.meta`) files. Must exist. See [float-metadata-format.md](float-metadata-format.md). |
| `phy` | `--dir.phy PATH` | `phy` | PHY listing files. See [phy-file-format.md](phy-file-format.md). |
| `raw` | `--dir.raw PATH` | `raw` | RAW mail message output files. See [raw-file-format.md](raw-file-format.md). |
| `mail` | `--dir.mail PATH` | `raw/incoming` | RAW mail input files (one e-mail per file), read in name order when `mail.fetchMode = 1`. See [iridium-mail-format.md](iridium-mail-format.md). |
| `mail.processed` | `--dir.mail.processed PATH` | `raw/processed` | An input file is moved here once queued; created if missing. If set empty, input files are deleted instead. |
| `sbd` | `--dir.sbd PATH` | `sbd` | Binary SBD attachment files. See [sbd-file-format.md](sbd-file-format.md). |
| `stat` | `--dir.stat PATH` | `stat` | Status files. See [status-file-format.md](status-file-format.md). |

Output directories and their per-float subdirectories are created when a file is first written to them (`FileSystemUtils::GetDirectoryPathFromString(..., true)` in `src/utility/FileSystemUtils.h`). `dir.log` and `dir.meta` are checked at startup, and a missing one stops the daemon (section 6).

### 4.3 `[gen]` — output file generation

| File (`[gen]` key) | Command line | Type | Default | Description |
|---|---|---|---|---|
| `kml` | `--gen.kml 0\|1` | `bool` | `true` | Write KML files. |
| `lis` | `--gen.lis 0\|1` | `bool` | `true` | Write listing files. |
| `phy` | `--gen.phy 0\|1` | `bool` | `true` | Write PHY files. A PHY file is written only if the float also has PHY generation enabled individually (`--phy-enable`, or the per-float flag stored in the database) and has a metadata file (`XFloatDiveData` PHY writer in `src/data/XFloat.cpp`). |
| `raw` | `--gen.raw 0\|1` | `bool` | `true` | Write RAW mail message files. |
| `raw.format` | `--gen.raw.format 0\|1` | `int` | `1` | RAW file format: `0` standard, `1` extended (`XMAIL_RAW_FORMAT_*` in `src/data/XFloatMailMessage.h`). Any other value writes only the file banner. |
| `sbd` | `--gen.sbd 0\|1` | `bool` | `false` | Write binary SBD files. |
| `stat` | `--gen.stat 0\|1` | `bool` | `false` | Write status files. |

### 4.4 `[poc]` — surface pressure offset correction

| File (`[poc]` key) | Command line | Type | Default | Description |
|---|---|---|---|---|
| `lis` | `--poc.lis 0\|1` | `bool` | `true` | Apply the surface pressure offset correction to pressures in listing files. |
| `phy` | `--poc.phy 0\|1` | `bool` | `true` | Apply it to pressures in PHY files. |

The correction is applied only to floats whose sensor type needs it (README.md, "Surface pressure offset correction"). The WMO instrument type from the metadata file is one input to that decision ([float-metadata-format.md](float-metadata-format.md)).

### 4.5 `[mail]` — mail server and message validation

| File (`[mail]` key) | Command line | Type | Default | Description |
|---|---|---|---|---|
| `host` | `--mail.host HOST` | `string` | `maila.whoi.edu` | Mail server host name or IP address. |
| `protocol` | `--mail.protocol P` | `string` | `imap` | VMime service protocol name. |
| `user` | `--mail.user U` | `string` | `alamo` | Mail account user name. |
| `password` | `--mail.password P` | `string` | none (empty) | Mail account password. |
| `folder` | `--mail.folder F` | `string` | empty | Folder to read; empty means the server's default folder. |
| `fetchMode` | `--mail.fetchMode 0\|1` | `int` | `0` | `0`: fetch from the mail server. `1`: read RAW mail files (`*.raw`, `*.txt`) from `dir.mail`; the other server options are then unused and no connection is made. Any other value: the fetch thread starts but fetches nothing. |
| `moveMessages` | `--mail.moveMessages 0\|1` | `bool` | `false` | After processing, move each server message to the folder `INBOX.<YYYYMM>` of its header date. Server mode only. With `sql.enable = 0` and `fetchMode = 0`, `1` stops startup (section 6). |
| `timeout` | `--mail.timeout SEC` | `unsigned int` | `120` | Seconds without data after which a mail server operation is canceled; the connection is reopened at the next check. |
| `xDlyS` | `--mail.xDlyS SEC` | `unsigned int` | `60` | Seconds to wait before checking again when no new messages were found. |
| `validSenderRegex` | `--mail.validSenderRegex RE` | `string` | `(sbdservice@sbd.iridium.com\|service@sbd.pac.disa.mil)` | Senders whose messages are valid. Section 5. |
| `validSubjectRegex` | `--mail.validSubjectRegex RE` | `string` | `SBD Msg From Unit: (\d+)` | Valid subjects; capture group 1 is the IMEI. Section 5. |
| `validSbdFilenameRegex` | `--mail.validSbdFilenameRegex RE` | `string` | `(\d+)_(\d{6})\.sbd` | Valid SBD attachment file names. Section 5. |

The mail message formats are in [iridium-mail-format.md](iridium-mail-format.md).

### 4.6 `[sql]` — MySQL server

| File (`[sql]` key) | Command line | Type | Default | Description |
|---|---|---|---|---|
| `enable` | `--sql.enable 0\|1` | `bool` | `true` | Use the database. `0`: no connection is made, nothing is loaded or saved, and the other `[sql]` options are unused. Floats are built in memory from the messages read in the run; `-r` and `-R` stop startup, and `-A` and the reprocessing implied by `--phy-enable` are skipped (`XFloatMailRetrievalService::CheckSqlDependentOptions()` in `src/stage1_MailRetrievalThread.cpp`). See [database-schema.md](database-schema.md). |
| `host` | `--sql.host HOST` | `string` | `127.0.0.1` | Server host name or IP address. |
| `port` | `--sql.port N` | `unsigned int` | `3306` | Server port. |
| `protocol` | `--sql.protocol P` | `string` | `tcp` | Connection protocol. The connection URL is `<protocol>://<host>:<port>` (`LocalMysqlConfiguration::getSqlConnection()` in `src/config/LocalMysqlConfiguration.cpp`). |
| `schema` | `--sql.schema S` | `string` | `float_db` | Database schema. Production uses `xmtps_alamo`. |
| `user` | `--sql.user U` | `string` | none (empty) | Account user name. |
| `password` | `--sql.password P` | `string` | none (empty) | Account password. |

The schema is described in [database-schema.md](database-schema.md).

### 4.7 Command-line-only options

These are not accepted in the configuration file; writing them there is an unknown-option error.

| Option | Type | Default | Description |
|---|---|---|---|
| `-c PATH`, `--config PATH` | `path` | `../cfg/options.cfg` | Configuration file. Relative to the working directory. |
| `-D`, `--daemon` | switch | off | Run until a termination signal or loss of the run-time lock. Without it, the daemon starts the pipeline, waits about 10 seconds (`XFLT_DAEMON_THREAD_SPINUP_DELAY_SEC`), then shuts down, draining what is queued. With `mail.fetchMode = 1` it waits instead until `dir.mail` has no `.raw`/`.txt` file left, then shuts down the same way and writes the KML and status files once more. A one-shot run with `mail.fetchMode = 0` does not wait for a full mail fetch. |
| `-h`, `--help` | | | Print all options with defaults, then exit. |
| `-V`, `--version` | | | Exit after the banner (which shows `v1.0.<build>b`, or `a` for debug builds). |
| `-d`, `--debug` | | | Log level `LOG_DBG3`. Debug and trace messages are compiled only into debug builds (`BUILD_DEBUG`); in a release build `-d` logs the same as `-v`. |
| `-v`, `--verbose` | | | Log level `LOG_INF`: adds informational messages. Used if `-d` is not given. |
| `-q`, `--quiet` | | | Log level `LOG_WRN`: warnings and errors only. Used if neither `-d` nor `-v` is given. |
| `-r ID ...`, `--reprocess ID ...` | `vector<unsigned int>`, multitoken | none | Reprocess all stored mail messages of these instrument serial numbers at startup. Takes several IDs and may be repeated. |
| `-R`, `--reprocess-all` | switch | off | Reprocess the stored messages of every cached instrument at startup. |
| `-A 0\|1`, `--reprocess-active 0\|1` | `bool` | `1` | Reprocess every instrument that reported in the last 90 days at startup. It takes a value; `-A` alone is an error. |
| `--phy-enable ID` | `vector<unsigned int>` | none | Enable PHY generation for this instrument. One ID per occurrence; repeat the option for more. |
| `--phy-disable ID` | `vector<unsigned int>` | none | Disable PHY generation for this instrument. One ID per occurrence. |

With no log option the level is `LOG_LOG` (`DEFAULT_LOG_LEVEL` in `src/utility/Log.h`). Log levels are described in [log-and-lock-files.md](log-and-lock-files.md).

## 5. Regular-expression options

The three `mail.valid*Regex` options are compiled with `std::regex` (libstdc++) each time a message is checked (`src/data/XFloatMailMessage.h`):

| Option | Grammar | Function | Applied to | Use |
|---|---|---|---|---|
| `validSenderRegex` | POSIX extended (`regex::extended`) | `regex_match` (whole string) | The bare e-mail address of the `From` header (VMime `mailbox::getEmail()`), not the display name | Message is valid only if it matches. |
| `validSubjectRegex` | ECMAScript (default) | `regex_search` | The decoded `Subject` header | Capture group 1 is parsed as the IMEI. |
| `validSubjectRegex` | POSIX extended | `regex_match` (whole string) | The decoded `Subject` header | Message is valid only if it matches. |
| `validSbdFilenameRegex` | ECMAScript (default) | `regex_search` (substring) | Each `application/*` attachment's file name | The first matching attachment is the SBD data. |

Rules that follow:

- `validSubjectRegex` must be valid in both grammars, must match the whole subject, and must put the IMEI in group 1.
- The sender expression must match the whole address. An unescaped `.` matches any character; the built-in default leaves the dots unescaped, and the repository file escapes them (`\.`). Both work.
- Backslashes are written once in the file (`\d`, `\.`); the file parser does no escape processing. The doubled backslashes in `src/data/XFloatMailMessage.h` (`"(\\d+)"`) are C++ string-literal escapes for the same text.
- `\d` is used in the POSIX-extended matches in production (`SBD Msg From Unit: (\d+)`) and matches digits there. POSIX leaves `\d` undefined in ERE; `[0-9]` is the portable spelling [inferred].
- A value cannot contain `#` (section 2), not even as `[#]` or `\#`, since the parser cuts the line at the first `#` before anything else. An expression that needs `#` can only be given on the command line.
- Trailing spaces are trimmed. The repository file has trailing spaces after `validSubjectRegex`; they are removed by the parser and do no harm.
- An invalid expression throws `std::regex_error` when a message is checked, not at startup. The error code is logged (`regex_error caught: error_...`, `CATCH_BLOCK` in `src/utility/Log.h`) and the message fails header parsing.

## 6. Error behavior

| Condition | Behavior |
|---|---|
| Configuration file missing | `Configuration file not found!` |
| Configuration file unreadable | `Failed to read configuration file: <path>` |
| Unknown option in the file (`allow_unregistered = false`) | Boost `unknown_option` ("unrecognised option '<name>'"). |
| Line without `=` that is not a section header | Boost `invalid_config_file_syntax`. |
| Option repeated in the file | Boost `multiple_occurrences`. |
| Bad value (non-numeric integer, bad Boolean) | Boost `invalid_option_value` / `invalid_bool_value`. |
| Unknown or malformed command-line option | Boost `unknown_option` or similar, before the file is read. |
| PID file already exists | `PID file already exists: <path>` with the PID found in it. See [log-and-lock-files.md](log-and-lock-files.md). |
| `dir.log` not a directory | `Invalid path to log directory:<path>` |
| `dir.meta` not a directory | `Invalid metadata path: <path>` |
| `sql.enable = 0` with `-r` or `-R` | `Reprocessing (-r, -R) needs the SQL database, but it is disabled (sql.enable = 0)!` The PID file is removed first. |
| `sql.enable = 0` with `mail.moveMessages = 1` and `mail.fetchMode = 0` | `mail.moveMessages = 1 needs the SQL database, but it is disabled (sql.enable = 0): ...` The PID file is removed first. |

All of these happen inside functions marked `ATTR_NO_EXCEPTIONS` (`noexcept`). The exception is wrapped by `CATCH_BLOCK` into a `util::log::MessageException` with a "Caused by" chain and rethrown, and leaving the `noexcept` function calls `std::terminate()`. The process aborts (SIGABRT) by design. The libstdc++ terminate handler prints `terminate called after throwing an instance of ...` and the exception's `what()` text, which contains the cause, to standard error [inferred].

Where the message goes:

- Errors before the logger is initialized (configuration and PID-file errors) reach only standard output and standard error. Boost.Log has no sink yet, so `lerr()` output goes to Boost's default sink on `std::clog` [inferred].
- Under the production systemd unit both streams go to `/dev/null` (`StandardOutput=null`, `StandardError=null`), so a configuration error leaves no message on disk. It shows only as a failed start in the systemd journal. Run the `ExecStart` command by hand to see it.
- Errors after the logger starts (`dir.meta`, database) are also written to the log files.

There are no warnings for options that are set but unused (for example the `[mail]` server options with `mail.fetchMode = 1`).

## 7. Production configuration

| Item | Value |
|---|---|
| Host | `argus.whoi.edu` |
| File | `/data1/alamo/etc/xmtps.cfg`, given by `--config` in `xmtps-alamo.service` |
| Owner and mode | `argo:floatgroup`, `0400` (`-r--------`). It holds the mail and SQL passwords. |
| Command line | `/data1/alamo/bin/xmtps -v --config /data1/alamo/etc/xmtps.cfg --daemon --pid-file /data1/alamo/var/lock/xmtps.pid` |
| Working directory | `/` (systemd default; the unit sets no `WorkingDirectory=`) [inferred]. Relative defaults such as `dir.log = log` would resolve under `/`, so production must set every directory absolutely. |

Values in effect, inferred from the paths in the 2026-10-29 log and from the repository's `cfg/options.cfg` and README.md [inferred]:

| Option | Value |
|---|---|
| `dir.log` | `/data1/alamo/var/log/xmtps` |
| `dir.meta` | `/data1/alamo/meta/dac` |
| `dir.lis` | `/data1/alamo/data/list` |
| `dir.raw` | `/data1/alamo/data/raw` |
| `dir.sbd` | `/data1/alamo/data/sbd/incoming` |
| `dir.stat` | `/data1/alamo/data/stat` |
| `dir.kml` | `/data1/alamo/data/kml` |
| `gen.kml`, `gen.sbd`, `gen.stat` | on (KML, SBD and status files are written) |
| `mail.fetchMode` | `0` (the log shows `Fetched N new message(s) from mail server...`) |
| `sql.schema` | `xmtps_alamo` |

## 8. systemd unit files

The repository keeps two units in `bin/`. The installed copies in `/etc/systemd/system/` on `argus` are identical to them (checked 2026-10-30).

`bin/xmtps-alamo.service` (decoder):

| Key | Value | Effect |
|---|---|---|
| `Type` | `simple` | `ExecStart` is the daemon process; it does not fork. |
| `User` / `Group` | `argo` / `floatgroup` | Owner of the log, lock and output files. |
| `ExecStart` | see section 7 | Supplies `-v`, `--config`, `--daemon` and `--pid-file`. |
| `PIDFile` | `/data1/alamo/var/lock/xmtps.pid` | Same path as `--pid-file`. systemd does not write this file, but removes it after the service stops if it is still there [inferred from systemd documentation]. This clears a lock left by a crash (see [log-and-lock-files.md](log-and-lock-files.md)). |
| `Restart` / `RestartSec` | `always` / `30` | A failed start, for example a configuration error or MySQL being down, is retried every 30 seconds. |
| `TimeoutStopSec` | `300` | Time allowed for the orderly shutdown (draining queued messages) before SIGKILL. |
| `StandardOutput` / `StandardError` | `null` | Console output, including startup errors, is discarded. |

`bin/xmtps-alamo-plot.service` (Python plot service):

| Key | Value | Effect |
|---|---|---|
| `Type` | `simple` | |
| `User` / `Group` | `aekholm` / `floatgroup` | |
| `ExecStart` | `/data1/alamo/bin/py/update-plots-service.sh` | Loop script; its paths are hard-coded in the script, not configurable. |
| `PIDFile` | `/data1/alamo/var/lock/xmtps-plot.pid` | Never written by the script; the file does not exist on `argus`. |
| `Restart` / `RestartSec` | `always` / `30` | |
| `StandardOutput` | not set | Output goes to the journal. See [log-and-lock-files.md](log-and-lock-files.md). |

After a unit file changes, `sudo systemctl daemon-reload` is needed. The `init.d/` scripts in the working copy are older SysV equivalents; git ignores them.

## 9. Open questions / discrepancies

1. **`#` in values.** README.md says the mail user name and password "may contain any characters" because they are percent-encoded in the server URL. In the configuration file a `#` starts a comment and truncates the value, and leading and trailing whitespace is removed. A password containing `#` can only be passed on the command line, where it is visible in the process list. The same limit applies to the SQL password and to the regular expressions.
2. **`\d` in POSIX-extended regular expressions.** `validSubjectRegex` (whole-match check) and the metadata-file patterns in `src/data/XFloatMetadata.cpp` use `\d` with `regex::extended`. POSIX leaves this undefined. The production libstdc++ (GCC 15.2) treats it as a digit class, as production shows. Another standard library may treat it as a literal `d` or reject it.
3. **`LocalMysqlConfiguration::protocol_` initializer.** In `src/config/LocalMysqlConfiguration.cpp` the static `protocol_` is initialized with `CFG_MYSQL_DFLT_SCHEMA` (`"float_db"`) instead of `CFG_MYSQL_DFLT_PROTOCOL` (`"tcp"`). The option default (`tcp`) overwrites it during parsing, so it has no effect. It is a latent typo.
4. **Production configuration not verified.** The production file is mode `0400` for `argo` and was not read. Section 7 values are inferred.
5. **Running process differs from the unit file.** On 2026-10-30 the running daemon (PID 3241667, started 2026-10-28 22:08) had `-r` followed by 32 serial numbers appended to the unit's command line, and systemd reported that the unit file had changed on disk. The unit was apparently edited for a reprocessing run and then restored without a `daemon-reload` [inferred]. The next restart uses whatever `daemon-reload` last loaded.
