# RAW Mail Message File Format

This document specifies the RAW files that XMTPS writes, one per Iridium SBD e-mail, under `<dir.raw>/<serial>/`. The mail processing thread (stage 2) writes them from the parsed e-mail; people read them to inspect individual messages, and in the standard format they can be fed back to the decoder as mail input files. The format is ASCII text, selected by `gen.raw.format`: standard (0) or extended (1, the default and the production format).

## Contents

1. [Overview](#1-overview)
2. [File names and directories](#2-file-names-and-directories)
3. [Standard format (`gen.raw.format = 0`)](#3-standard-format-genrawformat--0)
4. [Extended format (`gen.raw.format = 1`)](#4-extended-format-genrawformat--1)
5. [Common parts](#5-common-parts)
6. [When files are written and rewritten](#6-when-files-are-written-and-rewritten)
7. [Open questions / discrepancies](#7-open-questions--discrepancies)

Related specifications: [iridium-mail-format.md](iridium-mail-format.md) (the e-mail each file is made from, and the meaning of every field), [sbd-file-format.md](sbd-file-format.md) (the binary file written alongside), [xmessage-format.md](xmessage-format.md) (the bytes in the hex dump), [database-schema.md](database-schema.md) (`XFloatMail.raw_file_local_path`) and [configuration-file-format.md](configuration-file-format.md).

## 1. Overview

- Writer: `XFloatMailMessage::WriteRawFile` and `operator<<(ostream&, const XFloatMailMessage&)` in `src/data/XFloatMailMessage.cpp`, called from `XFloatMailMessage::WriteFiles` (`src/data/XFloatMailMessage.h`) by `XFloatMailProcessingService::ProcessMailMsg` (`src/stage2_MailProcessingThread.cpp`).
- One file per e-mail that passes stage 2: header valid and serial in 1000–11999 ([iridium-mail-format.md section 9](iridium-mail-format.md#9-stage-2-processing-and-rejection)). This includes incomplete transfers. Rejected e-mails get no file.
- Options (`src/Main.cpp`):

| Option | Default | Meaning |
|---|---|---|
| `gen.raw` | `1` | Write RAW files. |
| `gen.raw.format` | `1` | `0` standard, `1` extended. Any other value writes the splash header only. |
| `dir.raw` | `raw` | Root directory. Production: `/data1/alamo/data/raw` (from the paths in the production log). |

- Encoding: ASCII. Lines end in LF; in the standard format, the embedded e-mail keeps its own line endings [inferred].
- The relative path is stored in `XFloatMail.raw_file_local_path`.
- RAW files contain the full 15-digit IMEI. The examples here and in `example/raw/` mask it to the last 7 digits.

## 2. File names and directories

The directory is `<dir.raw>/<serial>/`, created if missing. The name depends on whether the SBD data is valid ([iridium-mail-format.md section 6](iridium-mail-format.md#6-sbd-attachment)).

| Case | Name (Boost format) | Example |
|---|---|---|
| SBD data valid | `raw%04d-%05d-%04d-%03d.txt` (serial, MOMSN, dive, packet) | `raw9303-05814-0440-000.txt` |
| SBD data invalid | `raw%04d-%05d-incomplete-%s.txt` (serial, MOMSN, header date) | `raw11730-00038-incomplete-20260812T021246.txt` |

| Field | Format | Notes |
|---|---|---|
| serial | `%04d` | At least 4 digits; serials ≥ 10000 have 5 (`raw11730-...`). The directory name uses the same format. |
| MOMSN | `%05d` | Zero-padded to 5 digits; 6 or more digits when the MOMSN is ≥ 100000. |
| dive | `%04d` | Width 4 including the sign: dive −1 (start-up) is `-001`, so the name has a double hyphen: `raw9303-00000--001-000.txt`. Dive 0 is `0000`. |
| packet | `%03d` | Packet ID 0–255. |
| header date | ISO 8601 basic, `YYYYMMDDTHHMMSS` | The e-mail `Date` header in UTC (`to_iso_string`). Distinguishes failed sessions that repeat a MOMSN. |

The MOMSN and the date in incomplete names keep files apart when the modem repeats a MOMSN after a failed session. Before 2020-01-15 files were named differently (`CHANGELOG.md`): `raw<serial>-<MOMSN %06d>-<dive %04d>.txt` with no packet ID (for example `10083/raw10083-004431-0002.txt`), and `raw<serial>-<MOMSN %06d>-incomplete-<date>.txt`. Production still has thousands of these (for example 2727 in `10083/`), often next to a current-name file for the same message written when the float was reprocessed in 2020: `raw9035-001668-incomplete-20140912T154249.txt` and `raw9035-01668-incomplete-20140912T154249.txt`.

Production on argus (2026-10-30): 468 float directories, plus an empty `incoming` directory (the default `dir.mail` location, not used in fetch mode 0); float 9303 alone has 6457 RAW files for 6328 SBD files, the difference being incomplete transfers. About 30,600 incomplete files in all.

## 3. Standard format (`gen.raw.format = 0`)

The standard format is a comment header followed by the original e-mail. It was the format read by the SOLO-II decoding software (`src/data/XFloatMailMessage.h`, `XMAIL_RAW_FORMAT_STANDARD`). A search of production (`grep -rl '^# WHOI:'` over `/data1/alamo/data/raw`, stopped after 500 s, so not the whole tree) found no file in this format; see section 7.

### 3.1 Layout

| Lines | Content |
|---|---|
| 1–25 | Splash header (5.1). |
| 26–28 | Section header `Processing Details` (5.2). |
| 29 | `#` |
| 30–36 | Processing details, each prefixed `# ` (table below). |
| 37 | `#` |
| 38–40 | Section header `Start of Message`. |
| 41 | Empty. |
| 42– | The e-mail text (`msg_raw_text`), unchanged, to the end of the file. |

Processing details lines: the same labels and values as in the extended format (4.2), with `# ` in front, for example `# WHOI:            9303`.

The e-mail text is the message as fetched from the IMAP server: all header fields (including relay headers), the MIME parts, and the SBD attachment in base64. For a message read from a mail input file, it is the message as regenerated by VMime ([iridium-mail-format.md 13.2](iridium-mail-format.md#132-file-format)).

### 3.2 Example

Constructed from the code and the example e-mail (no production file exists); splash header and most of the e-mail abbreviated:

```
#===================================================================================================
#        _       ____  ______  ____   ________            __     ______
...
#===================================================================================================
#
# This data was produced and distributed as part of projects that were planned and executed by or
...
#
#===================================================================================================
# Processing Details
#===================================================================================================
#
# WHOI:            9303
# IMEI:            XXXXXXXX2418110
# Dive:            440
# Message UID:     1211371
# Message ID:      39e31b$csupha@UFOR19PA23.eemsg.mail.mil
# Processed:       2026-08-27T16:34:16Z
# Valid:           yes
#
#===================================================================================================
# Start of Message
#===================================================================================================

Message-Id: <39e31b$csupha@UFOR19PA23.eemsg.mail.mil>
Date: 27 Aug 2026 16:32:39 +0000
...
Subject: SBD Msg From Unit: XXXXXXXX2418110
...
--SBD.Boundary.605592468--
```

### 3.3 Reading it back

A standard-format file is a valid mail input file for `mail.fetchMode = 1`: the parser skips leading lines that are empty or start with `#`, and parses the rest as the e-mail ([iridium-mail-format.md section 13](iridium-mail-format.md#13-mail-input-files-fetch-mode-1)). The file must be named `.txt` or `.raw`, which RAW files are.

## 4. Extended format (`gen.raw.format = 1`)

The extended format lists the parsed fields in four sections and gives the SBD data as a hex dump. It does not contain the e-mail itself, so it cannot be read back as mail input.

### 4.1 Layout

| Section | Header text | Present |
|---|---|---|
| Splash header | (5.1) | always |
| 4.2 | `Processing Details` | always |
| 4.3 | `Message Header` | always |
| 4.4 | `Iridium Session Details` | always |
| 4.5 | `Iridium Short-Burst Data (SBD) Message` | only if the e-mail had an SBD attachment (`sbd_data` not empty), valid or not |

Each section is a three-line section header (5.2), an empty line, the `Label: value` lines, and an empty line. The last section ends with the hex dump and a newline instead.

Each data line is a label and a value. The label, including its colon, is left-aligned and padded with spaces to 17 characters, so values start in column 18. Values are written with default C++ stream formatting unless stated.

### 4.2 Processing Details

| Label | Field | Format | Notes |
|---|---|---|---|
| `WHOI:` | `id_instrument` | integer | Float serial number ([iridium-mail-format.md section 8](iridium-mail-format.md#8-serial-number-resolution)). |
| `IMEI:` | `id_imei` | integer, 15 digits | From the subject. −1 if not found. |
| `Dive:` | `id_dive` | integer | From the packet header; −999 if the SBD data is invalid. |
| `Message UID:` | `id_msg_uid` | integer | IMAP UID; −1 for mail input files. |
| `Message ID:` | `id_msg` | text | `Message-ID` header without angle brackets. |
| `Processed:` | current time | ISO 8601 `YYYY-MM-DDTHH:MM:SSZ`, UTC | When the file was written. |
| `Valid:` | `msg_is_valid` | `yes`/`no` | Header, body and SBD data all valid ([iridium-mail-format.md section 7](iridium-mail-format.md#7-validity-flags)). |

### 4.3 Message Header

| Label | Field | Format | Notes |
|---|---|---|---|
| `From:` | `msg_header_from` | text | Sender address only. |
| `Subject:` | `msg_header_subject` | text | |
| `Date:` | `msg_header_datetime` | ISO 8601 `YYYY-MM-DDTHH:MM:SSZ` | `Date` header converted to UTC. `0000-00-00T00:00:00Z` if missing. |

### 4.4 Iridium Session Details

| Label | Field | Format | Notes |
|---|---|---|---|
| `MOMSN:` | `id_sbd_momsn` | integer | −1 if not found. |
| `MTMSN:` | `id_sbd_mtmsn` | integer | −1 if not found. |
| `Time of Session:` | `iridium_position_datetime` | ISO 8601 `YYYY-MM-DDTHH:MM:SSZ` | `0000-00-00T00:00:00Z` if not parsed. |
| `Session Status:` | `sbd_session_status_code`, `sbd_session_status_string` | `<code> - <text>` | The code is an integer without zero padding: the e-mail's `00 - Transfer OK` becomes `0 - Transfer OK`. `-1 - ` if the line was missing. |
| `Message Size:` | `sbd_file_size_bytes` | integer, bytes | From the e-mail body, not the attachment length. −1 if not found. |
| `Unit Location:` | `iridium_position_latitude`, `iridium_position_longitude` | ISO 6709 `%+.5f%+.5f` | For example `+29.54123-68.74308`. `+99.99999+999.99999` if not given. Older builds printed 6 decimals (`+99.999990+999.999990`). |
| `CEPradius:` | `iridium_position_cep_radius` | integer, km | 999999 if not given. |
| `Position Valid:` | `iridium_position_is_valid` | `yes`/`no` | Latitude within ±90 and longitude within ±180. |

### 4.5 Iridium Short-Burst Data (SBD) Message

| Label | Field | Format | Notes |
|---|---|---|---|
| `Filename:` | `sbd_file_name` | text | The attachment name, `<IMEI>_<MOMSN, 6 digits>.sbd`. |
| `Local Path:` | `sbd_file_local_path` | Boost path, in double quotes | Path of the SBD file relative to `dir.sbd`. `""` when the file is written from mail; see 6.2. |
| `Hex:` | `sbd_data` | hex dump below | The label is followed by trailing spaces, then an empty line. |

**Hex dump.** The SBD bytes (the X packet, [xmessage-format.md A.1.2](xmessage-format.md#a12-x-packet-x-nn-mm-dd-p-data--c-c-)), 32 bytes per line:

```
(000) 0x 58011C24 5701B800 010018FE 11958523 D6F17A0B 0981030A 20000400 00002B3B
(032) 0x 40105D32 217D4200 001000FA 00D2A003 EC0158A0 06DC018E 2007E602 22A009C5
...
(288) 0x 36333E
```

- Each line starts with the decimal offset of its first byte, zero-padded to 3 digits, in parentheses, then ` 0x `.
- Bytes are two uppercase hex digits, in groups of 4 bytes separated by one space.
- The last line holds the remaining bytes; the last group may be short.
- The file ends with a newline after the last line.
- An SBD message is at most 340 bytes, so offsets fit in 3 digits.

Unlike the `SHX` lines of listing files ([listing-file-format.md](listing-file-format.md)), the dump has no ASCII column.

### 4.6 Example

`example/raw/raw9303-05814-0440-000.txt` (production file, IMEI masked), without the splash header:

```
#===================================================================================================
# Processing Details
#===================================================================================================

WHOI:            9303
IMEI:            XXXXXXXX2418110
Dive:            440
Message UID:     1211371
Message ID:      39e31b$csupha@UFOR19PA23.eemsg.mail.mil
Processed:       2026-08-27T16:34:16Z
Valid:           yes

#===================================================================================================
# Message Header
#===================================================================================================

From:            service@sbd.pac.disa.mil
Subject:         SBD Msg From Unit: XXXXXXXX2418110
Date:            2026-08-27T16:32:39Z

#===================================================================================================
# Iridium Session Details
#===================================================================================================

MOMSN:           5814
MTMSN:           0
Time of Session: 2026-08-27T16:32:35Z
Session Status:  0 - Transfer OK
Message Size:    291
Unit Location:   +29.54123-68.74308
CEPradius:       3
Position Valid:  yes

#===================================================================================================
# Iridium Short-Burst Data (SBD) Message
#===================================================================================================

Filename:        XXXXXXXX2418110_005814.sbd
Local Path:      ""
Hex:             

(000) 0x 58011C24 5701B800 010018FE 11958523 D6F17A0B 0981030A 20000400 00002B3B
...
(288) 0x 36333E
```

An incomplete transfer (production, `11730/raw11730-00038-incomplete-20260812T021246.txt`, IMEI masked) has no SBD section:

```
WHOI:            11730
IMEI:            XXXXXXXX8509990
Dive:            -999
Message UID:     1208819
Message ID:      4fxdtjxn0t-1@istsbdmailsv01.sbd.iridium.com
Processed:       2026-08-12T02:13:13Z
Valid:           no
...
MOMSN:           38
MTMSN:           0
Time of Session: 2026-08-12T02:12:30Z
Session Status:  13 - Incomplete Transfer
Message Size:    0
Unit Location:   +8.79329-47.00685
CEPradius:       5
Position Valid:  yes
```

An e-mail with an attachment but an unaccepted status (for example status 2 under builds before status 2 was accepted) has an `incomplete` name, `Dive: -999`, `Valid: no`, and the SBD section with the full hex dump.

## 5. Common parts

### 5.1 Splash header

Both formats start with the splash header `SPLASH_LIS` (`src/config/LocalConfiguration.cpp`), the same one as listing files: a `#===` line, the WHOI Float Group banner, a credit paragraph, the XMTPS version and build time (`version 0.6.<build><variant> (built <ISO time>)`), the maintainer, and a closing `#` line. All 25 lines start with `#`. The text, including the version line, depends on the build that wrote the file.

### 5.2 Section headers

Each section header is three lines (macro `XMSG_HEADER2`, `src/data/xmsg/common/XDefinitions.h`):

```
#===================================================================================================
# <Section name>
#===================================================================================================
```

The rule lines are `#` followed by 99 `=` (100 characters).

## 6. When files are written and rewritten

### 6.1 Written once

- A RAW file is written when stage 2 processes the message, before the message is saved to the database and before it is decoded.
- If a file already exists at the path, it is left unchanged. This holds for both formats and for reprocessing (`-r`, `-R`, `-A`): RAW files are never rewritten or updated.
- `Processed:` is therefore the time of the first write. To regenerate a file, delete it and reprocess the float; stage 2 then writes it from the database row [inferred: reprocessed messages pass through `ProcessMailMsg`]. Production has such regenerated files, for example a message from 2018 with `Processed: 2020-03-31T22:16:26Z`.
- Changing `gen.raw.format` affects only files written afterward.
- If the write fails, stage 2 logs the error and the message is dropped (`Failed to write mail message files!`); it is not saved to the database.

### 6.2 `Local Path` is empty on first write

`WriteFiles` writes the RAW file before the SBD file, and the SBD file's path (`sbd_file_local_path`) is set only when the SBD file is written. So for a message processed from mail, `Local Path:` is always `""`. A RAW file regenerated during reprocessing shows the path, because the message is rebuilt from the database, which stores it. Production confirms this: about 156,000 RAW files show a path, for example `9035/raw9035-04952-0000-000.txt` (message of 2014-11-25, `Processed: 2020-03-31T19:42:01Z`, `Local Path: "9035/9035-004952-0000.sbd"`), while none of the 6457 files of float 9303 does.

## 7. Open questions / discrepancies

1. **Standard format in production.** No production RAW file in the standard format was found (the search did not cover the whole tree in its time limit; one file seen in a partial tally has an unprefixed `Session Status: 00 - Transfer OK` line, which could be embedded e-mail text, but it was not located). The standard layout in section 3 is from the code only. Its byte-exact output (line endings of the embedded e-mail, whether `msg_raw_text` ends with a newline) has not been checked.
2. **Format description in the code.** `src/data/XFloatMailMessage.h` describes the extended format as the standard format with the attachment "encoded in hex, rather than base-64". In fact the extended format omits the e-mail (headers and body) entirely and lists parsed fields instead; it cannot be read back as mail input, while the standard format can.
3. **`Local Path` is empty on first write** (6.2) and filled only in files regenerated from the database, so the same field means different things in different files. It looks intended to show the SBD file path, but the call order in `WriteFiles` prevents that.
4. **Old file names.** Files from before the 2020-01-15 and 2020-03-31 renames remain in production next to regenerated files with current names (section 2), so a message can have two RAW files.
5. **Invalid `gen.raw.format`.** A value other than 0 or 1 writes a file with only the splash header; no warning is given.
