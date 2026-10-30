# Binary SBD File Format

This document specifies the binary SBD files that XMTPS writes, one per accepted Iridium SBD message, under `<dir.sbd>/<serial>/`. The mail processing thread (stage 2) writes each file from the e-mail attachment when `gen.sbd` is on; XMTPS itself never reads them back, and they are kept for other tools and for inspection. Each file holds exactly the bytes the float sent: one X-message packet.

## Contents

1. [Overview](#1-overview)
2. [File names and directories](#2-file-names-and-directories)
3. [Content](#3-content)
4. [When files are written](#4-when-files-are-written)
5. [Open questions / discrepancies](#5-open-questions--discrepancies)

Related specifications: [xmessage-format.md](xmessage-format.md) (the packet bytes), [iridium-mail-format.md](iridium-mail-format.md) (the e-mail the bytes come from), [raw-file-format.md](raw-file-format.md) (the text file written for the same message) and [configuration-file-format.md](configuration-file-format.md).

## 1. Overview

- Writer: `XFloatMailMessage::WriteSbdFile` (`src/data/XFloatMailMessage.h`), called from `XFloatMailMessage::WriteFiles` by `XFloatMailProcessingService::ProcessMailMsg` (`src/stage2_MailProcessingThread.cpp`), right after the RAW file.
- Options (`src/Main.cpp`):

| Option | Default | Meaning |
|---|---|---|
| `gen.sbd` | `0` | Write SBD files. Off by default; on in production. |
| `dir.sbd` | `sbd` | Root directory. Production: `/data1/alamo/data/sbd/incoming` [inferred from the files on argus; the production configuration is not readable]. |

- The relative path is stored in `XFloatMail.sbd_file_local_path` ([database-schema.md](database-schema.md)). The database also stores the bytes (`XFloatMail.sbd_data`), so the files are not needed for reprocessing.
- `/data1/alamo/data/sbd/outgoing` on argus is a different directory, for commands sent to floats. It is written by the external `alamo-cmd` tool, and XMTPS neither reads nor writes it ([iridium-mail-format.md section 15](iridium-mail-format.md#15-mobile-terminated-outgoing-messages)).

## 2. File names and directories

```
<dir.sbd>/<serial>/<serial>-<MOMSN>-<dive>.sbd
```

Boost format `%04d/%04d-%06d-%04d.sbd` (serial, serial, MOMSN, dive). The directory `<dir.sbd>/<serial>` is created if missing.

| Field | Format | Notes |
|---|---|---|
| serial | `%04d` | Float serial number from packet bytes 3–4. At least 4 digits; 5 for serials ≥ 10000. |
| MOMSN | `%06d` | Zero-padded to 6 digits (the RAW file name uses 5). Matches the MOMSN in the attachment name `<IMEI>_<MOMSN>.sbd`. |
| dive | `%04d` | From packet bytes 5–6. Width 4 including the sign: dive −1 is `-001`, so the name has a double hyphen: `9303-000000--001.sbd`. |

The packet ID is not in the name. Each MOMSN is one SBD message, so the name is unique for accepted messages. A packet retransmitted unchanged under a new MOMSN gets its own file.

Examples from float 9303 in production and in `example/sbd/`:

```
9303/9303-000000--001.sbd      dive -1 (start-up)
9303/9303-000001-0000.sbd      dive 0
9303/9303-005814-0440.sbd      dive 440, MOMSN 5814 (packet 0)
```

On argus (2026-10-30), float 9303 has 6328 SBD files and 6457 RAW files; the difference is the incomplete transfers, which get a RAW file but no SBD file.

## 3. Content

- The file is the decoded (base64) e-mail attachment, byte for byte. There is no header, trailer, length field or padding added by XMTPS.
- The file size equals the attachment length. It normally equals the `Message Size (bytes)` of the e-mail body, which is not checked.
- Size: at least 8 bytes (shorter attachments are not accepted) and at most 340 bytes (the SBD limit).
- The content is one X-message packet, `X nn mm dd p <blobs> $cc>`. Its layout, checksum and blobs are specified in [xmessage-format.md, section A.1](xmessage-format.md#a1-packet-framing-and-general-conventions); they are not repeated here.
- The bytes are written as received. The file is written before stage 3 checks the checksum or decodes the blobs, so a packet that fails decoding still has a file.

Start of `example/sbd/9303-005814-0440.sbd` (291 bytes), with the packet header fields used for the file name:

```
00000000: 5801 1c24 5701 b800 0100 18fe 1195 8523  X..$W..........#
          ^  ^    ^    ^    ^
          |  |    |    |    p  = 0x00   packet 0
          |  |    |    dd    = 0x01B8   dive 440
          |  |    mm         = 0x2457   serial 9303
          |  nn              = 0x011C   (284 = 291 - 7)
          'X'
...
00000120: 3633 3e                                  63>
```

The same bytes appear as the hex dump in the extended RAW file ([raw-file-format.md 4.5](raw-file-format.md#45-iridium-short-burst-data-sbd-message)) and as `SHX`/`shx` lines in the listing ([listing-file-format.md](listing-file-format.md)).

## 4. When files are written

An SBD file is written when all of these hold:

1. `gen.sbd` is on.
2. The e-mail passed stage 2: valid header, serial in 1000–11999 ([iridium-mail-format.md section 9](iridium-mail-format.md#9-stage-2-processing-and-rejection)).
3. The SBD data is valid: an `application/*` attachment whose name matches `mail.validSbdFilenameRegex`, at least 8 bytes, session status 0 or 2 ([iridium-mail-format.md section 6](iridium-mail-format.md#6-sbd-attachment)).
4. No file exists at the path yet.

Existing files are never overwritten, including during reprocessing (`-r`, `-R`, `-A`). Deleting a file and reprocessing the float writes it again from the bytes stored in the database [inferred: reprocessed messages pass through `ProcessMailMsg`].

The file is written with a plain `ofstream` and is not written atomically (no temporary file and rename) [read from code]. If writing fails, stage 2 logs the error and drops the message without saving it to the database.

## 5. Open questions / discrepancies

1. **Location in the documentation.** `README.md` and `example/README.md` give the location as `<dir.sbd>/incoming/<serial>/...`. The code writes `<dir.sbd>/<serial>/...`; the `incoming` part comes from the production value of `dir.sbd` (`/data1/alamo/data/sbd/incoming`) [inferred]. With the default `dir.sbd = sbd` there is no `incoming` directory.
2. **Doc comment of `sbd_file_generation_is_enabled_`.** `src/data/XFloatMailMessage.h` says the default is FALSE; `XMAIL_DFLT_SBD_FILE_GENERATRION_ENABLE` is `true`, but the option's default in `src/Main.cpp` is `false`, which is what applies. The option text says the files go to "the mail message directory", but they go to `dir.sbd`.
3. **MOMSN width differs from the RAW file name** (6 digits here, 5 in RAW names), so the two names for the same message are not built the same way.
