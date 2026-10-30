# Iridium SBD Notification E-mail Format

This document specifies the e-mail that the Iridium gateway sends for each mobile-originated (MO) Short Burst Data (SBD) message from an ALAMO/ALTO float, and how XMTPS reads it. The gateway writes the e-mail; the XMTPS mail retrieval thread (stage 1) fetches it from the IMAP server, or reads it from a mail file in `dir.mail`, and the mail processing thread (stage 2) validates it, resolves the float, writes the RAW and SBD files and saves it to the database.

## Contents

1. [Overview](#1-overview)
2. [Example](#2-example)
3. [Header fields](#3-header-fields)
4. [MIME structure](#4-mime-structure)
5. [Body text fields](#5-body-text-fields)
6. [SBD attachment](#6-sbd-attachment)
7. [Validity flags](#7-validity-flags)
8. [Serial number resolution](#8-serial-number-resolution)
9. [Stage 2 processing and rejection](#9-stage-2-processing-and-rejection)
10. [Duplicates and retransmissions](#10-duplicates-and-retransmissions)
11. [Fetching from the IMAP server (fetch mode 0)](#11-fetching-from-the-imap-server-fetch-mode-0)
12. [Moving processed mail to `INBOX.<YYYYMM>`](#12-moving-processed-mail-to-inboxyyyymm)
13. [Mail input files (fetch mode 1)](#13-mail-input-files-fetch-mode-1)
14. [Reprocessing from the database](#14-reprocessing-from-the-database)
15. [Mobile-terminated (outgoing) messages](#15-mobile-terminated-outgoing-messages)
16. [Open questions / discrepancies](#16-open-questions--discrepancies)

Related specifications: [xmessage-format.md](xmessage-format.md) (the attachment bytes), [raw-file-format.md](raw-file-format.md) and [sbd-file-format.md](sbd-file-format.md) (the files written from each e-mail), [database-schema.md](database-schema.md) (table `XFloatMail`), [configuration-file-format.md](configuration-file-format.md) (the `mail.*`, `dir.*` and `gen.*` options) and [listing-file-format.md](listing-file-format.md) (the `SBD` and `SHX` lines made from each e-mail).

## 1. Overview

- One e-mail carries one SBD message. An SBD message is one X-message packet of at most 340 bytes ([xmessage-format.md A.1.1](xmessage-format.md#a11-transport-iridium-sbd-and-the-mail-gateway)).
- The e-mail has three parts that XMTPS reads:
  - the header (sender, subject with the IMEI, date, Message-ID);
  - a `text/plain` part with the Iridium session details (MOMSN, MTMSN, session time and status, size, Iridium position);
  - an `application/*` attachment with the SBD bytes, base64-encoded.
- Parsing is done with VMime in `src/data/XFloatMailMessage.h` (`ParseMailMsg`, `ParseMailMsgHeader`, `ParseMailMsgBody`, `ParseMailMsgSbdAttachment`). The parser is the same for both fetch modes.
- Two gateways have been seen in production: the commercial Iridium gateway (sender `sbdservice@sbd.iridium.com`) and the US DoD gateway (sender `service@sbd.pac.disa.mil`). Their e-mails have the same layout.

## 2. Example

The e-mail for float 9303, dive 440, packet 0 (MOMSN 5814), from `example/mail/mail9303-05814-0440-000.eml`. The IMEI is masked to its last 7 digits and the relay headers are removed (see `example/README.md`); the body is unchanged.

```
Message-Id: <39e31b$csupha@UFOR19PA23.eemsg.mail.mil>
Date: 27 Aug 2026 16:32:39 +0000
Reply-To: <service@sbd.pac.disa.mil>
From: service@sbd.pac.disa.mil
To: alamo@whoi.edu
Subject: SBD Msg From Unit: XXXXXXXX2418110
MIME-Version: 1.0
Content-Type: multipart/mixed;boundary="SBD.Boundary.605592468"

SBM Message
--SBD.Boundary.605592468
Content-Type: text/plain;charset=US-ASCII
Content-Disposition: inline
Content-Transfer-Encoding: 7bit

MOMSN: 5814
MTMSN: 0
Time of Session (UTC): Thu Aug 27 16:32:35 2026
Session Status: 00 - Transfer OK


Message Size (bytes): 291

Unit Location: Lat = 29.54123 Long = -68.74308
CEPradius = 3



--SBD.Boundary.605592468
Content-Type: application/x-zip-compressed; name="SBMmessage.sbd"
Content-Disposition: attachment; filename="XXXXXXXX2418110_005814.sbd"
Content-Transfer-Encoding: base64

WAEcJFcBuAABABj+EZWFI9bxegsJgQMKIAAEAAAAKztAEF0yIX1CAAAQAPoA0qAD7AFYoAbc
...
MCAAAAAAAAAAAAAAAAAAADskNjM+
--SBD.Boundary.605592468--
```

Because the IMEI in the example is masked with `X`, its subject does not match the subject regex, and the decoder would discard it as having an invalid header (section 3.1). Replace the `X`s with digits to use it as a test input; the serial (9303) then comes from the packet header (section 8).

## 3. Header fields

`ParseMailMsgHeader` reads these header fields. All other header fields (`To`, `Reply-To`, `Received`, ...) are ignored, but stay in the stored raw text (`msg_raw_text`).

| Header | Parsed into | How | Used for |
|---|---|---|---|
| `From` | `msg_header_from` | The e-mail address only (`mailbox::getEmail()`); a display name is dropped. | Sender validation (3.1). |
| `Subject` | `msg_header_subject`, `id_imei` | Decoded to UTF-8. `regex_search` with `mail.validSubjectRegex`; capture group 1 is read as an integer and becomes the IMEI. | Subject validation (3.1), IMEI. |
| `Date` | `msg_header_datetime` | RFC 5322 date, converted to UTC by subtracting its zone offset. | Monthly mail folder (section 12), name of RAW files for incomplete messages, start-of-mission filter in stage 3. |
| `Message-ID` | `id_msg` | The id without the angle brackets (`left@right`). | Duplicate detection (section 10); part of the database primary key. |

The IMAP UID (fetch mode 0 only) is stored as `id_msg_uid`. It is not a header field; see section 11.

Before 2026-10-23 the `Date` zone offset was not applied correctly (`CHANGELOG.md`). All gateway dates seen in production are `+0000`, so this made no difference in practice [inferred].

### 3.1 Header validation

The header is valid (`msg_header_is_valid`) when both of these hold:

| Option | Default | Match |
|---|---|---|
| `mail.validSenderRegex` | `(sbdservice@sbd.iridium.com\|service@sbd.pac.disa.mil)` | `regex_match` (whole address), POSIX extended grammar, case-sensitive, against the `From` address. |
| `mail.validSubjectRegex` | `SBD Msg From Unit: (\d+)` | `regex_match` (whole subject), POSIX extended grammar, against the subject. |

The defaults are `XMAIL_DFLT_MSG_HEADER_VALID_SNDR_REGEX` and `XMAIL_DFLT_MSG_HEADER_VALID_SUBJ_REGEX` in `src/data/XFloatMailMessage.h`; the options are defined in `src/Main.cpp`. The subject regex is used twice with different grammars: ECMAScript `regex_search` to extract the IMEI, then POSIX extended `regex_match` to validate. See section 16, item 1.

A subject from a float is `SBD Msg From Unit: ` followed by the modem's 15-digit IMEI (shown masked in the examples here). Other gateway e-mails (for example notices about mobile-terminated messages) do not match and are discarded in stage 2 [inferred].

## 4. MIME structure

| Level | Content type | Notes |
|---|---|---|
| Message | `multipart/mixed` | Boundary `SBD.Boundary.<n>`. A preamble line `SBM Message` precedes the first boundary; it is not a MIME part and is not read. |
| Part 1 | `text/plain;charset=US-ASCII`, `Content-Disposition: inline`, 7bit | The session details (section 5). |
| Part 2 | `application/x-zip-compressed; name="SBMmessage.sbd"`, `Content-Disposition: attachment; filename="<IMEI>_<MOMSN>.sbd"`, base64 | The SBD message. The bytes are not a zip file despite the content type; they are the raw X packet. Absent when the session carried no data (`Message Size (bytes): 0`) [inferred from production RAW files]. |

- The body text is the concatenation of all text parts that VMime's `messageParser` reports (`getTextPartAt`), in order.
- Attachments are the parts that `messageParser` reports as attachments (`getAttachmentAt`). Section 6 says which one is used.

## 5. Body text fields

`ParseMailMsgBody` searches the body text with one `regex_search` (ECMAScript grammar) per field. Order and spacing between lines do not matter; each regex matches the first occurrence anywhere in the text. A field that is not found keeps its fill value.

| Field | Regex | Stored as | Fill value if absent | Notes |
|---|---|---|---|---|
| MOMSN | `MOMSN: (\d+)` | `id_sbd_momsn` (int) | −1 | Mobile-originated message sequence number. Incremented by the modem for each successful MO transfer; not incremented for a failed session, so failed sessions can repeat a MOMSN. |
| MTMSN | `MTMSN: (\d+)` | `id_sbd_mtmsn` (int) | −1 | Mobile-terminated sequence number of an MT message delivered in the same session; 0 when none [inferred]. |
| Time of Session | `Time of Session \(UTC\): ([a-zA-Z]{3} ([a-zA-Z]{3})[ ]+(\d{1,2}) (\d{2}:\d{2}:\d{2}) (\d{4}))` | `iridium_position_datetime` (UTC) | not-a-date-time | `ctime`-style: weekday, month name, day (space-padded), `HH:MM:SS`, year. The weekday is ignored. Rebuilt as `YYYY-Mon-DD HH:MM:SS` and read with the Boost `ptime` stream parser. |
| Session Status | `Session Status: (\d{2}) - ([^\n]+)` | `sbd_session_status_code` (int), `sbd_session_status_string` | −1, empty | The code must have exactly two digits. The text is the rest of the line. |
| Message Size | `Message Size \(bytes\): (\d+)` | `sbd_file_size_bytes` (int) | −1 | Size of the SBD message as reported by the gateway. Not compared with the attachment length. This is the size shown on listing `SBD` lines. |
| Unit Location | `Unit Location: Lat = (-?\d+\.\d+) Long = (-?\d+\.\d+)` | `iridium_position_latitude`, `iridium_position_longitude` (degrees) | 99.99999, 999.99999 | Both numbers need a decimal point. The position is valid if −90 ≤ lat ≤ 90 and −180 ≤ lon ≤ 180 (`IsPositionValid`, `src/utility/PositionUtils.h`). |
| CEPradius | `CEPradius = (\d+)` | `iridium_position_cep_radius` (int, km) | 999999 | Circular error probable radius of the Iridium position. |

`ParseMailMsgBody` also calls `regex_replace` to remove blank lines from the body text, but discards the result, so the text is searched as received (section 16, item 4). This is harmless because every field is found with `regex_search`.

The body is valid (`msg_body_is_valid`) when MOMSN and MTMSN were found and the session time was parsed.

### 5.1 Session status codes

Codes and texts seen in production RAW files (`/data1/alamo/data/raw` on argus, 2014–2026):

| Code | Text in the e-mail | Meaning | SBD data accepted |
|---|---|---|---|
| 00 | `Transfer OK` | Successful MO transfer. | yes |
| 02 | `Transfer OK Bad Location` | Successful transfer; the Iridium location is of unacceptable quality. | yes |
| 13 | `Incomplete Transfer` | The session did not complete (RF link lost). Size 0 and no attachment in the production files examined. | no |
| 14 | `SBD Protocol Error` | Protocol anomaly. | no |

Iridium documents further codes (01 MT message too large, 10 call timeout, 12 MO message too large, 15 access denied) [inferred: from Iridium's SBD documentation, not seen in production]. XMTPS does not interpret the code beyond the check in section 6: any code other than 0 or 2, or a missing status line, makes the SBD data invalid.

## 6. SBD attachment

`ParseMailMsgSbdAttachment` walks the attachments in order and uses the first one that passes both checks:

1. Its media type is `application/*` (any subtype).
2. Its name matches `mail.validSbdFilenameRegex` (default `(\d+)_(\d{6})\.sbd`, `XMAIL_DFLT_SBD_FILE_VALID_NAME_REGEX`) with `regex_search` (ECMAScript), so the regex may match part of the name.

The name is VMime's attachment name, which is taken from `Content-Disposition: filename` (production RAW files show `<IMEI>_<MOMSN>.sbd`, not the `name="SBMmessage.sbd"` of `Content-Type`). The gateway writes the name as `<IMEI>_<MOMSN, 6 digits zero-padded>.sbd`, for example `XXXXXXXX2418110_005814.sbd`. The two capture groups are not used: the IMEI comes from the subject and the MOMSN from the body.

The attachment is decoded from base64, and the bytes are stored in `sbd_data`. The name is stored in `sbd_file_name`. Later attachments are ignored.

The SBD data is valid (`sbd_data_is_valid`) when:

- the attachment has at least 8 bytes (`XMSG_SBD_MIN_BYTES`, `src/data/xmsg/common/XDefinitions.h`; enforced since 2026-10-23), and
- the session status code is 0 or 2.

When the SBD data is valid, the packet header gives (big-endian, see [xmessage-format.md A.1.2](xmessage-format.md#a12-x-packet-x-nn-mm-dd-p-data--c-c-)):

| Packet bytes | Field set |
|---|---|
| 3–4 (u16) | `id_instrument`, the float serial number |
| 5–6 (i16) | `id_dive`, the dive number (−1 at start-up) |
| 7 (u8) | `id_sbd_packet`, the packet ID |

When the SBD data is invalid these fields keep their fill values (`id_dive` = −999, `id_sbd_packet` = −1, serial unknown). The decoder does not check the `X` start byte or the checksum here; stage 3 does (xmessage-format.md A.1.2).

## 7. Validity flags

| Flag | True when | Effect |
|---|---|---|
| `msg_header_is_valid` | Sender and subject regexes match (3.1). | Required: stage 2 discards the message otherwise. |
| `msg_body_is_valid` | MOMSN, MTMSN and session time found (5). | Stored only; no decision depends on it. |
| `sbd_data_is_valid` | Attachment present, named right, ≥ 8 bytes, status 0 or 2 (6). | Required for decoding and for the SBD file; selects the RAW file name. |
| `msg_is_valid` | All three above. | Stored; printed as `Valid:` in RAW files. |

All four are columns of `XFloatMail` (see [database-schema.md](database-schema.md)).

## 8. Serial number resolution

The float serial number (`id_instrument`, "WHOI ID") is found as follows:

1. If the SBD data is valid, the serial is bytes 3–4 of the packet (section 6). The IMEI is not used for this. A float therefore keeps its serial even if its modem (IMEI) changes.
2. Otherwise (incomplete transfer, no attachment, bad name, short attachment), stage 2 looks the IMEI up in the float table (`src/stage2_MailProcessingThread.cpp`, `ProcessMailMsg`):

   ```sql
   SELECT id_instrument FROM XFloat WHERE id_imei = <IMEI> ORDER BY id_instrument DESC LIMIT 1
   ```

   If several floats have the IMEI, the highest serial wins. If none has it, or the subject gave no IMEI (−1), the result is 0.

   With `sql.enable = 0` (no database; see [database-schema.md](database-schema.md), section 10.6) the serial is instead taken from the first float already in memory with the same IMEI, that is a float whose valid packet was read earlier in the same run. If there is none, the serial stays unknown.
3. Stage 2 then requires 1000 ≤ serial ≤ 11999. A message outside the range is discarded with the warning `Ignoring mail message with invalid serial number.` It is not saved and no files are written.

The float table learns IMEIs from decoded messages: when stage 3 decodes a message, it sets the float's `id_imei` to the message's IMEI if they differ (`XFloatDiveData::DecodeMailMessageData`, `src/data/XFloat.cpp`). An incomplete message from a float that has never sent a valid packet is therefore discarded.

The DAC and WMO identifiers of the message are copied from the float (`XFloat::GetFloat`).

## 9. Stage 2 processing and rejection

`XFloatMailProcessingService::ProcessMailMsg` handles each parsed e-mail in this order:

1. Header invalid: discard (`Ignoring mail message with invalid header.`). Nothing is saved or written.
2. Resolve the serial (section 8); discard if out of range.
3. Write the RAW file ([raw-file-format.md](raw-file-format.md)) and, if the SBD data is valid, the SBD file ([sbd-file-format.md](sbd-file-format.md)).
4. Save the message to `XFloatMail` (`INSERT ... ON DUPLICATE KEY UPDATE`).
5. If the SBD data is valid, queue it for decoding (stage 3). Otherwise log `Ignoring mail message with invalid binary SBD attachment.`

So a message with a valid header and a known float but invalid SBD data (an incomplete transfer) is still kept: it gets a RAW file named `...-incomplete-...` and a database row, but no SBD file and no decoding.

If saving fails because the SQL server is unavailable, the message is held and retried every 10 s (`XFLT_SQL_RETRY_MS`), and no new mail is fetched until the server is back. Any other failure drops the message (it is logged). With `sql.enable = 0` step 4 does nothing.

## 10. Duplicates and retransmissions

| Case | Handling | Source |
|---|---|---|
| The same e-mail processed again (re-fetch, restart, reprocessing). | The database row is updated in place: the primary key is (`id_imei`, `id_dive`, `id_sbd_momsn`, `id_sbd_packet`, `id_msg`). The RAW and SBD files already exist and are not rewritten. | `sql/xmtps-schema.sql`, `src/utility/SqlTableDescription.h`, `WriteRawFile`, `WriteSbdFile` |
| An e-mail with a Message-ID already assigned to the dive. | Ignored by the dive (`Ignoring mail message with duplicate ID`); not decoded again. | `XFloatDiveData::AssignMailMessage` (`src/data/XFloatMailMessage.cpp`) |
| A packet resent unchanged under a new MOMSN (identical SBD bytes). | Assigned to the dive (its Iridium position counts) but its blobs are not decoded again. It gets its own RAW and SBD files, named by its own MOMSN. | `XFloatDiveData::AssignMailMessage` |
| Several e-mails with the same MOMSN (failed sessions do not advance the MOMSN). | Kept apart: incomplete ones are named by MOMSN and header date; complete ones by MOMSN, dive and packet. | `WriteRawFile`, `CHANGELOG.md` (2020-01-15) |

Dive data is keyed by (serial, dive) from the packet header, not by anything in the e-mail.

## 11. Fetching from the IMAP server (fetch mode 0)

`mail.fetchMode = 0` (the default). `XFloatMailRetrievalService::FetchMailMessagesFromServer` (`src/stage1_MailRetrievalThread.cpp`):

- Connects to `<mail.protocol>://<user>:<password>@<mail.host>` (user and password percent-encoded) and opens `mail.folder` read-write; an empty `mail.folder` opens the server's default folder.
- Fetches by message sequence number, not by UID: messages `msgStartIdx` (default 1) to `msgStartIdx + maxFetch − 1`, capped at the folder's message count. It fetches the envelope, full header, content info, structure and UID, then extracts the whole message.
- Stores the full raw message text (`msg->extract()`) as `msg_raw_text` and the IMAP UID as `id_msg_uid`.
- Advances the start index past the batch. When a batch returns messages it fetches again at once; when there are none it disconnects and waits `mail.xDlyS` seconds (default 60).
- Does not fetch while the SQL server is unavailable, so the mail stays on the server.
- A message whose parsing throws is logged (`Failed to process fetched message!`) and skipped; it stays in the inbox and is not moved.
- Does not set flags on messages or delete them, unless `mail.moveMessages` is set (section 12). Whether the server marks fetched messages as seen depends on how VMime fetches the body [not checked].

With `mail.moveMessages = false` the start index begins at `msgStartIdx` on every start, so all messages still in the inbox are fetched and processed again after a restart. Section 10 says why this is harmless.

## 12. Moving processed mail to `INBOX.<YYYYMM>`

With `mail.moveMessages = true` (default false), and only in fetch mode 0:

- Stage 1 records each fetched message's IMAP UID and destination folder before queuing it. The folder name is the first six characters of the header date in ISO 8601 basic format, that is `YYYYMM` of the `Date` header in UTC.
- Stage 2 reports each message as processed when it has been saved, or rejected (invalid header, serial out of range, or any other failure while the SQL server is available).
- At the start of the next fetch, stage 1 moves the processed messages: for each month it opens the subfolder `<YYYYMM>` of the open inbox folder (creating it if needed), copies the messages there by UID, deletes them from the inbox and expunges. With `mail.folder = INBOX` this is `INBOX.<YYYYMM>` on a server whose hierarchy separator is `.` [inferred: the separator is the server's].
- The start index is lowered by the number of messages removed, so no message is skipped.
- A message whose processing was interrupted (SQL outage, shutdown) is never reported as processed, so it stays in the inbox and is fetched again at the next start.
- If a move fails, its messages stay pending and are retried after the next fetch. A copy that succeeded before a failed delete is copied again, so the month folder can get a duplicate. A timeout aborts the fetch and reconnects.

## 13. Mail input files (fetch mode 1)

`mail.fetchMode = 1` reads e-mails from files instead of the IMAP server. `XFloatMailRetrievalService::FetchMailMessagesFromFileSystem` and `ListRawMailFiles` (`src/stage1_MailRetrievalThread.cpp`), parser `operator>>(istream&, XFloatMailMessage&)` (`src/data/XFloatMailMessage.cpp`).

### 13.1 Which files are read

- Directory: `dir.mail` (default `raw/incoming`). Only regular files directly in it; subdirectories are not searched.
- Name: the full path must match `.*/.*\.(raw|txt)` (POSIX extended, whole path), that is end in `.raw` or `.txt`. Lock files (`.lck`) and failed files (`.err`) are not read.
- At most `maxFetch` files per batch (default 127), in name order (byte-wise sort of the full path), so files named in transmission order (for example by MOMSN) are processed in the order the messages were sent.
- No files are read while the SQL server is unavailable (with `sql.enable = 0` there is no server to wait for).

### 13.2 File format

A mail input file is one RFC 5322 / MIME message, as in section 2, optionally preceded by comment lines:

1. Leading lines that are empty or start with `#` are skipped (regex `^#.*|^$`). This skips the comment header of a standard-format RAW file.
2. Everything from the first other line to the end of the file is parsed as the e-mail: header fields, a blank line, then the MIME body.

So valid input files are:

- a plain `.eml` export of the gateway e-mail, renamed to `.txt` or `.raw`;
- a standard-format RAW file (`gen.raw.format = 0`, [raw-file-format.md](raw-file-format.md#3-standard-format-genrawformat--0)), whose comment header is skipped and whose `Start of Message` section is the original e-mail.

An extended-format RAW file (`gen.raw.format = 1`, the default) is not valid input: it does not contain the e-mail. Its first non-comment line (`WHOI: ...`) is read as a header field, there is no `From` or `Subject`, and the message fails header validation in stage 2 [inferred]. The code has a `TODO Determine if mail message is extended format.` at this point.

Line endings may be LF or CRLF [inferred: VMime accepts both].

Fields that differ from fetch mode 0:

| Field | Fetch mode 0 | Fetch mode 1 |
|---|---|---|
| `id_msg_uid` | IMAP UID | −1 (not set) |
| `msg_raw_text` | The message as received (`extract()`) | The message as regenerated by VMime from the parsed file (`generate()`), so folding and encoding may differ from the file [inferred] |

### 13.3 Locking, deletion and `.err` files

For each file `<name>`:

1. If `<name>.lck` exists, the file is skipped with an error that names the PID in the lock file. The lock is not removed, so the file is skipped on every pass until someone removes the lock.
2. Otherwise `<name>.lck` is created with the daemon's PID.
3. The file is parsed and the message is queued for stage 2.
4. On success the input file is moved to `dir.mail.processed` (default `raw/processed`, created if missing): it is copied there under the same name and then removed from `dir.mail`. If a file of that name is already in `dir.mail.processed`, that file is kept and the new one is just removed. If the copy throws, the input file is renamed to `<name>.processed` instead, so it is not read again. If `dir.mail.processed` is set empty, the input file is deleted.
5. If reading or parsing throws, the file is renamed to `<name>.err` so it is not read again, and a warning is logged.
6. The lock file is removed.

If termination is signaled while waiting for room in the queue, the lock is removed and the file is left in place for the next run.

The file leaves `dir.mail` as soon as it is queued, before stage 2 validates and saves it. A file that parses but is rejected in stage 2 (invalid header, unknown serial) is therefore moved to `dir.mail.processed` like a good one (or deleted, if `dir.mail.processed` is empty), with only a log line to show it was rejected. A file is only renamed `.err` when the MIME parse itself throws.

In a one-shot run (no `-D`) with `mail.fetchMode = 1`, the program keeps running until `dir.mail` has no more `.raw`/`.txt` files, then drains the pipeline, writes the KML and status files once more, and exits. It logs `All raw mail input files read from: <dir>` when it finds the directory empty (see [log-and-lock-files.md](log-and-lock-files.md), section 6).

## 14. Reprocessing from the database

At startup (`-r ID ...`, `-R`, `-A`, and floats with PHY generation enabled) stage 1 reads stored messages from `XFloatMail` instead of mail: the rows of each float, after its start of mission, ordered by MOMSN. They are rebuilt from the database columns (not reparsed from `msg_raw_text`) and go through stage 2 and 3 as above. See [database-schema.md](database-schema.md).

## 15. Mobile-terminated (outgoing) messages

XMTPS does not send, read or track mobile-terminated (MT) messages (commands to floats):

- No source file in `src/` or `bin/py/` refers to `/data1/alamo/data/sbd/outgoing`. On argus that directory holds per-float command files (`<serial>-<YYYYMMDDTHHMMSS>-<n>.sbd`, `.sbd.raw`, `.log`) written by the external `alamo-cmd` upload tool (`upload-alamo-cmd`). XMTPS neither reads nor writes it.
- From the MO e-mail, XMTPS keeps only the MTMSN (section 5), stores it, and prints it on listing `SBD` lines and in RAW files. It does not match it against sent commands.
- Gateway e-mails about MT messages, if any arrive in the inbox, fail the subject regex and are discarded in stage 2 [inferred].
- The float's echo of a received command is an MO X-message (0xDE, [xmessage-format.md C.2](xmessage-format.md#c2-mission-command-echo-0xde)), decoded like any other packet.

## 16. Open questions / discrepancies

1. **`\d` in a POSIX extended regex.** The default subject regex `SBD Msg From Unit: (\d+)` (also used in `README.md` and `cfg/options.cfg`) is validated with `regex::extended`. POSIX has no `\d`: libstdc++ (GCC 15 on argus, `bits/regex_scanner.tcc`) throws `error_escape` for it when `__STRICT_ANSI__` is defined, which `--std=c++11` (the production build flag) defines, and otherwise reads it as a literal `d`. Either way no Iridium subject would validate. Production e-mails do validate (RAW files say `Valid: yes`, and the logs have no `regex_error`), so the production configuration (`/data1/alamo/etc/xmtps.cfg`, not readable to this review) presumably uses a different subject regex. Not verified. `[0-9]+` works in both grammars. The same applies to a sender regex with `\.` only in that `\.` is a valid POSIX escape, so the sender regex is fine.
2. **Status 02 accepted.** The member documentation of `sbd_data_is_valid_` in `src/data/XFloatMailMessage.h` says the status must be zero; the code accepts 0 and 2. Production has incomplete RAW files with status 2 and a full attachment that were written by older builds (for example a 2018 message processed in 2020), so older builds accepted only 0.
3. **Discarded `regex_replace`.** `ParseMailMsgBody` calls `regex_replace(msg_body_text_, ...)` without using the result. No effect on parsing.
4. **Stale lock files block input files.** A `.lck` file left by a crash makes its input file be skipped forever, with an error each pass; it is not renamed `.err`.
5. **Header comments out of date.** `src/stage1_MailRetrievalThread.h` says messages are fetched starting from the maximum UID stored in the database; the code fetches by sequence number from `msgStartIdx`, and `is_reprocess_all_` no longer affects which mail is fetched. The member documentation of `msg_header_is_valid_` swaps SUBJECT and FROM.
6. **Message Size before 2026-10-23.** `CHANGELOG.md` says "Message Size" has been parsed since 2026-10-23. The change (commit 77b4911) only removed the `^…$` anchors from the size regex. RAW files written before it (2015–2026) show real sizes, not −1, so the anchored regex did match with the production library; the changelog entry overstates the change.
7. **Move to `dir.mail.processed` keeps the older file.** `MoveFileToDirectory` (`src/utility/FileSystemUtils.h`) does not overwrite an existing file of the same name in the destination, and the caller then removes the input file, so a second input file with a reused name is lost from disk (its message is still processed).
