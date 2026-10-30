# XMTPS Database Schema Specification

This document specifies the MySQL database in which XMTPS keeps its state: floats, their deployment metadata, dive cycles, GPS fixes and every Iridium SBD mail message received. It gives each table's columns, keys and C++ mapping, says which code reads and writes each table and when, and describes the reprocessing flows in terms of table operations.

Status: compiled on 2026-10-30 from `sql/xmtps-schema.sql` (the production schema, MySQL 8.4.11, with the XFloatMail changes of 2026-10-26), the table descriptions in `src/data/*.cpp` and `src/data/xmsg/XGPS.cpp`, the SQL helpers in `src/utility/` and `src/config/`, and README.md "SQL backend". The live database on argus was not inspected (no read access without a password).

## Contents

- [1. Overview](#1-overview)
- [2. Server requirements](#2-server-requirements)
- [3. Access layer](#3-access-layer)
- [4. Table relationships](#4-table-relationships)
- [5. Table `XFloat`](#5-table-xfloat)
- [6. Table `XFloatMetadata`](#6-table-xfloatmetadata)
- [7. Table `XFloatDive`](#7-table-xfloatdive)
- [8. Table `XGPS`](#8-table-xgps)
- [9. Table `XFloatMail`](#9-table-xfloatmail)
- [10. Table operations during normal operation](#10-table-operations-during-normal-operation)
- [11. Reprocessing flows](#11-reprocessing-flows)
- [12. Example queries](#12-example-queries)
- [13. Creating and migrating the schema](#13-creating-and-migrating-the-schema)
- [14. Open questions / discrepancies](#14-open-questions--discrepancies)

## 1. Overview

The database holds five InnoDB tables. Production uses the schema `xmtps_alamo` on argus.

| Table | One row per | C++ class | Read by the daemon | Written by the daemon |
|---|---|---|---|---|
| `XFloat` | float | `XFloat` (`src/data/XFloat.*`) | at startup; IMEI lookup in stage 2 | many times (section 10) |
| `XFloatMetadata` | float with a `.meta` file | `XFloatMetadata` (`src/data/XFloatMetadata.*`) | never | with every `XFloat` save |
| `XFloatDive` | dive cycle of a float | `XFloatDiveData` (`src/data/XFloatDiveData.*`, `src/data/XFloat.cpp`) | when a dive not in memory is needed | after each decoded message |
| `XGPS` | GPS fix of a dive | `x::XGPS` (`src/data/xmsg/XGPS.*`) | KML trajectories; profile position fallback | with every `XFloatDive` save |
| `XFloatMail` | Iridium SBD mail message | `XFloatMailMessage` (`src/data/XFloatMailMessage.*`) | reprocessing; dive reload | once per message, before decoding |

`XFloatMail` is the archive: it holds every saved message with its raw SBD data, so that all other dive-level tables can be rebuilt from it. `XFloat` is the daemon's persistent per-float cache. `XFloatDive` and `XGPS` are derived data; reprocessing deletes and rebuilds them. `XFloatMetadata` mirrors the `.meta` files (see [float-metadata-format.md](float-metadata-format.md)) and is written only.

The Python tools in `bin/py/` do not use the database. They read the listing files (see [listing-file-format.md](listing-file-format.md)).

## 2. Server requirements

### 2.1 Version and engine

- MySQL 8. Production runs MySQL 8.4.11 (README.md, "Deployment").
- All tables use `ENGINE=InnoDB`. The foreign keys need InnoDB.

### 2.2 Character set

- Every table is created with `DEFAULT CHARSET=utf8mb3` (`sql/xmtps-schema.sql`).
- The README and the schema script create the database itself with `DEFAULT CHARACTER SET utf8mb4`. The table-level `utf8mb3` overrides it for all five tables.
- The connection character set is not set by the code (`src/utility/MysqlUtils.h`). It is the Connector/C++ default [inferred].
- In practice all stored text is ASCII: e-mail headers, the Iridium body text, file paths and `.meta` fields. A 4-byte UTF-8 character (for example an emoji in a `.meta` comment) would be rejected by a `utf8mb3` column under strict mode [inferred].

### 2.3 Required `sql_mode`

XMTPS writes every unset date/time as the zero date `'0000-00-00 00:00:00'` (section 3.4). MySQL 8 includes `NO_ZERO_DATE` and `NO_ZERO_IN_DATE` in its default `sql_mode`. Together with `STRICT_TRANS_TABLES` these make such inserts fail. The server's global `sql_mode` must therefore exclude both. Production uses:

```
sql-mode = "STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION"
```

The daemon does not set a session `sql_mode`; it relies on the server's global setting.

`STRICT_TRANS_TABLES` stays on. Any value that does not fit its column (out-of-range integer, too-long string) makes the whole `INSERT` fail rather than being truncated. Section 14 lists columns where this can happen.

### 2.4 Time zone

All `DATETIME` values are UTC. They are written as plain date/time strings with no zone, so the server's time zone does not affect stored values. It does affect `CURDATE()` in the active-float query (section 11.3).

## 3. Access layer

### 3.1 Connection

The connection settings are command-line/configuration-file options defined in `src/Main.cpp` and held in static members of `util::mysql::LocalMysqlConfiguration` (`src/config/LocalMysqlConfiguration.*`). See [configuration-file-format.md](configuration-file-format.md).

| Option | Default | Use |
|---|---|---|
| `sql.enable` | `1` | `0` runs without the database (section 10.6) |
| `sql.host` | `127.0.0.1` | server host |
| `sql.port` | `3306` | server port |
| `sql.protocol` | `tcp` | URL scheme |
| `sql.schema` | `float_db` | database (production: `xmtps_alamo`) |
| `sql.user` | none | account |
| `sql.password` | none | password |

- The client library is MySQL Connector/C++ (legacy `sql::` API, `mysqlcppconn`).
- The connection URL is `<protocol>://<host>:<port>`. The schema is selected with `Connection::setSchema()`.
- Each thread has its own connection, held in a `boost::thread_specific_ptr` (`src/utility/MysqlUtils.*`). It is opened on first use and reopened if closed.
- Autocommit is on. The daemon uses no explicit transactions. Each statement commits on its own, so a dive row and its GPS rows are not saved atomically.
- `LocalMysqlConfiguration::isSqlServerAvailable()` runs `SELECT 1;`. On failure it closes the thread's connection and tries once more on a new one.
- Worker threads close their connection when idle (`CloseSqlConnection()` in stages 1–4), so idle connections are not dropped by the server's timeout.

### 3.2 Table descriptions

Each persistent class has a static `util::mysql::SqlTableDescription<T>` (`src/utility/SqlTableDescription.h`). It is constructed with the table name and an ordered list of column descriptions (`src/utility/SqlTableColumnDescription.h`). Each column description gives:

- the column name;
- a C++ type (one of the `Column_*` templates, created with the `sql_col*` macros);
- an accessor and a mutator of the class;
- an `is_primary_key` flag.

The column order in the C++ list need not match the table. All statements name their columns.

`SqlTableDescription<T>` generates these statements:

| Operation | Statement | Used by |
|---|---|---|
| Save | `INSERT INTO <table>(<all columns>) VALUES (?,…,?) ON DUPLICATE KEY UPDATE c=VALUES(c), …` for every column not flagged as primary key | `SaveObjectToDatabase(obj)` |
| Select | `SELECT <all columns> FROM <table> [WHERE …] [GROUP BY …] [ORDER BY …] [LIMIT n [OFFSET m]];` | `SelectObjectsFromDatabase()`, `SelectObjectFromDatabase()` (LIMIT 1), `SelectAllObjectsFromDatabase()` |
| Select one value | `SELECT <column> FROM <table> … LIMIT 1;` | `SelectValueFromDatabase<TT>()` |
| Delete | `DELETE FROM <table> [WHERE …];` | `DeleteObjectsFromDatabase(where)` |

Save is an upsert. A new key inserts a row. An existing key (primary key, or any unique index; the tables have no other unique indexes) overwrites every non-key column with the object's current values. A save always writes every mapped column. Columns that are in the table but not in the C++ list keep their default on insert and their old value on update.

Statements are prepared statements. Values of the save are bound parameters. `WHERE` clauses are built by string concatenation, but only from integers and formatted dates, never from mail content.

A failing save is retried twice on the same prepared statement before the exception propagates (`SqlTableDescription::SaveObjectToDatabase`).

`DeleteObjectsFromDatabase("")` deletes every row of the table. No caller passes an empty condition.

### 3.3 Column type mapping

| Macro | C++ type | Bound with | Read with | SQL types it is used for |
|---|---|---|---|---|
| `sql_colb` | `bool` | `setBoolean` | `getBoolean` | `bit(1)` |
| `sql_cold` | `double` | `setDouble` | `getDouble` | `double` |
| `sql_colf` | `float` | `setDouble` | `getDouble` | `float` |
| `sql_coli` | `int` | `setInt` | `getInt` | `smallint`, `int` |
| `sql_ci16` | `int16_t` | `setInt` | `getInt` | `smallint` |
| `sql_coll` | `long` | `setInt64` | `getInt64` | `bigint`, `int` |
| `sql_cu16` | `uint16_t` | `setInt` | `getInt` | `smallint`, `smallint unsigned` |
| `sql_cou8` | `uint8_t` | `setInt` | `getInt` | `smallint`, `smallint unsigned` |
| `sql_cols` | `std::string` | `setString` | `getString` | `varchar`, `text` |
| `sql_colp` | `boost::filesystem::path` | `setString(path.string())` | `getString` | `varchar` |
| `sql_colt` | `boost::posix_time::ptime` | `setDateTime` (section 3.4) | `getString` (section 3.4) | `datetime` |
| `sql_colv` | `std::vector<uint8_t>` | `setString` of the raw bytes (section 3.5) | `HEX(col)` then hex-decoded | `blob` |

A `NULL` column reads as `""`, `0`, `0.0` or `false` (Connector/C++ behavior) [inferred]. The daemon itself never writes `NULL`: every mapped column always gets a value.

### 3.4 Date/time encoding

- Write (`Column_posix_time::SetPreparedStatementValueFromObject`):
  - an unset time (`not_a_date_time`) is written as `'0000-00-00 00:00:00'`;
  - any other time is written as `boost::posix_time::to_iso_string(t)`, the ISO 8601 basic format `YYYYMMDDTHHMMSS` (for example `20261025T120307`). MySQL accepts this as a `DATETIME` literal [inferred: production rows hold valid dates]. All times the daemon stores have whole seconds.
- Read (`Column_posix_time::SetObjectValueFromResultSet`):
  - the column is read as a string;
  - `''` (also `NULL`) and `'0000-00-00 00:00:00'` become `not_a_date_time`;
  - anything else is parsed with `boost::posix_time::time_from_string` (`YYYY-MM-DD HH:MM:SS`).
- The schema defaults of `'1400-01-01 00:00:00'` are never written by the daemon, because a save always sets every mapped column. A row that does hold `1400-01-01` (created outside the daemon, or an unmapped column) is read as the real date 1400-01-01, not as unset.
- In hand-written queries, treat both `'0000-00-00 00:00:00'` and `'1400-01-01 00:00:00'` as "unset".
- `WHERE` conditions built by the daemon compare `DATETIME` columns with strings in two formats:
  - `bpt::to_iso_string()` → `'20261025T120307'` (`XFloat::GetGpsPositions`, `XFloatDiveData::GetDiveDataMailMessagesFromDatabase`);
  - `DateUtils::GenerateIso8601String(t, true)` → `'2026-10-25T12:03:07Z'` (reprocessing, section 11.1). See section 14 on the trailing `Z`.

### 3.5 BLOB encoding (`XFloatMail.sbd_data`)

`XFloatMail.sbd_data` is the only BLOB. It is the only `sql_colv` column.

- Write (`Column_vector_uint8_t::SetPreparedStatementValueFromObject`): the bytes of the attachment are copied unchanged into a `std::string` and bound with `setString`. The column stores the attachment's exact bytes, with no header, length prefix or encoding.
- Read: the select list uses `HEX(sbd_data) AS sbd_data` (`SqlTableColumnDescriptionBase::column_select_name`). The hex string is decoded two characters per byte. Hex digits may be either case.
- The bytes are one ALAMO/ALTO X packet, at most 340 bytes. Its layout is specified in [xmessage-format.md](xmessage-format.md) A.1.2. The header fields that the mail parser copies into columns are:

| Offset | Size | Type | Field | Copied to column |
|---|---|---|---|---|
| 0 | 1 | char | `X` (0x58) | — |
| 1 | 2 | u16 BE | `nn`, length | — |
| 3 | 2 | u16 BE | float serial number | `id_instrument` |
| 5 | 2 | i16 BE | dive number | `id_dive` |
| 7 | 1 | u8 | packet ID | `id_sbd_packet` |
| 8… | | | blobs, then `$cc>` | — |

The copy happens only when `sbd_data_is_valid` is true (at least 8 bytes and session status 0 or 2; section 9.3). The same bytes are written to the `.sbd` file (see [sbd-file-format.md](sbd-file-format.md)).

No other column holds serialized structures. Decoded X-messages are not stored; they are rebuilt from `sbd_data` when needed.

## 4. Table relationships

```
XFloat (id_instrument)
 ├── XFloatMetadata (id_instrument)              FK fk_XFloatMetadata_1 → XFloat
 └── XFloatDive (id_instrument, id_dive)          FK fk_XFloatDive_1 → XFloat
      └── XGPS (id_instrument, id_dive, datetime) FK fk_XGPS_1 → XFloatDive

XFloatMail (id_imei, id_dive, id_sbd_momsn, id_sbd_packet, id_msg)   no foreign keys
     joined by convention on id_instrument (and id_dive) or id_imei
```

- The foreign keys have no `ON DELETE` or `ON UPDATE` action, so they restrict. A parent row cannot be deleted while children exist.
- Save order follows the keys:
  - `XFloat::SaveObjectToDatabase` saves the `XFloat` row, then its `XFloatMetadata` row.
  - `XFloatDiveData::SaveObjectToDatabase` saves the parent `XFloat` row (and so its metadata), then the `XFloatDive` row, then the dive's `XGPS` rows.
- Delete order is the reverse: `XGPS` rows, then `XFloatDive` rows (section 11.1).
- `XFloatMail` has no foreign key. A message is saved before its float may have a row in `XFloat`.

Identifier conventions used across tables:

| Column | Meaning | Unknown value |
|---|---|---|
| `id_instrument` | WHOI float serial number. Stage 2 accepts only 1000–11999. | -1 |
| `id_dive` | Dive (cycle) number from the packet header: -1 = start-up, 0 = test dive, then +1 per dive | -999 in C++ (`XFLT_DIVE_UNKNOWN`); schema default -99 in `XFloatMail` |
| `id_imei` | Iridium modem IMEI (15 digits) | -1 |
| `id_wmo` | WMO platform number from the `.meta` file | -1 |
| `id_dac` | Argo DAC identifier from the `.meta` file | -1 |

## 5. Table `XFloat`

**Purpose.** One row per float. It is the persistent part of the daemon's in-memory float cache (`XFloat::id2float_map_`): identifiers, the sensor gains and offsets from the latest decoded Argo message (0xF0), the latest GPS and Iridium positions, the start-of-mission time, and per-float output switches.

**Primary key.** `id_instrument`.

**Indexes.** `id_dac_idx (id_dac)`, `id_wmo_idx (id_wmo)`, `id_imei_idx (id_imei)`. Stage 2 uses `id_imei_idx` for its IMEI lookup.

**Foreign keys.** None out. `XFloatMetadata` and `XFloatDive` reference it.

**Table description.** `XFloat::sql_table_description_` in `src/data/XFloat.cpp`.

### 5.1 Columns

"Code default" is the value the C++ constructor gives a new float, and so the value written for a float that has not yet received the data.

| Column | SQL type | Null / schema default | Code default | Units | Meaning | C++ (macro, accessor) |
|---|---|---|---|---|---|---|
| `id_instrument` | `smallint` | NOT NULL | — | — | Float serial number. **PK.** | `sql_coli`, `id_instrument` |
| `argo_instrument_type` | `smallint unsigned` | NULL / 99 | 99 | — | Instrument type from the Argo message: 0 SOLO-II, 1 Deep Argo, 3 ALAMO, 99 unknown (`XArgoInstrumentType` in `src/data/xmsg/XArgo.h`). | `sql_cou8`, `argo_instrument_type` |
| `id_dac` | `smallint` | NOT NULL / -1 | -1 | — | Argo DAC ID, copied from the `.meta` file when the metadata is loaded. | `sql_coli`, `id_dac` |
| `id_imei` | `bigint` | NOT NULL / -1 | -1 | — | IMEI of the float's modem, set from each decoded message (the latest one wins). | `sql_coll`, `id_imei` |
| `id_wmo` | `int` | NOT NULL / -1 | -1 | — | WMO number, copied from the `.meta` file. | `sql_coli`, `id_wmo` |
| `gain_pressure` | `smallint unsigned` | NOT NULL / 9999 | 9999 | counts per dbar | Profile pressure gain. 9999 = unknown. | `sql_cu16`, `gain_pressure` |
| `gain_pressure_surface` | `smallint unsigned` | NOT NULL / 9999 | 9999 | counts per dbar | Surface pressure gain. | `sql_cu16`, `gain_pressure_surface` |
| `gain_salinity` | `smallint unsigned` | NOT NULL / 9999 | 9999 | counts per PSU | Profile salinity gain. | `sql_cu16`, `gain_salinity` |
| `gain_salinity_surface` | `smallint unsigned` | NOT NULL / 9999 | 9999 | counts per PSU | Surface salinity gain. | `sql_cu16`, `gain_salinity_surface` |
| `gain_temperature` | `smallint unsigned` | NOT NULL / 9999 | 9999 | counts per °C | Profile temperature gain. | `sql_cu16`, `gain_temperature` |
| `gain_temperature_surface` | `smallint unsigned` | NOT NULL / 9999 | 9999 | counts per °C | Surface temperature gain. | `sql_cu16`, `gain_temperature_surface` |
| `offset_pressure` | `smallint unsigned` | NULL / 9999 | 9999 | dbar | Profile pressure offset. 9999 = unknown. | `sql_cu16`, `offset_pressure` |
| `offset_pressure_surface` | `smallint unsigned` | NULL / 9999 | 9999 | dbar | Surface pressure offset. | `sql_cu16`, `offset_pressure_surface` |
| `offset_salinity` | `smallint unsigned` | NULL / 9999 | 9999 | PSU | Profile salinity offset. | `sql_cu16`, `offset_salinity` |
| `offset_salinity_surface` | `smallint unsigned` | NULL / **999** | 9999 | PSU | Surface salinity offset. | `sql_cu16`, `offset_salinity_surface` |
| `offset_temperature` | `smallint unsigned` | NULL / 9999 | 9999 | °C | Profile temperature offset. | `sql_cu16`, `offset_temperature` |
| `offset_temperature_surface` | `smallint unsigned` | NULL / 9999 | 9999 | °C | Surface temperature offset. | `sql_cu16`, `offset_temperature_surface` |
| `latest_gps_position_datetime` | `datetime` | NOT NULL / 1400-01-01 | zero date | UTC | Time of the newest valid GPS fix of any dive. | `sql_colt`, `latest_gps_position_datetime` |
| `latest_gps_position_dive_number` | `smallint` | NOT NULL / -999 | -999 | — | Dive of that fix. | `sql_coli`, `latest_gps_position_dive_number` |
| `latest_gps_position_latitude` | `double` | NOT NULL / 99.99999 | 99.99999 | deg N | Latitude of that fix. 99.99999 = none. | `sql_cold`, `latest_gps_position_latitude` |
| `latest_gps_position_longitude` | `double` | NOT NULL / 999.99999 | **99.99999** | deg E | Longitude of that fix. See section 14 for the code default. | `sql_cold`, `latest_gps_position_longitude` |
| `latest_iridium_position_datetime` | `datetime` | NOT NULL / 1400-01-01 | zero date | UTC | Iridium "Time of Session" of the newest message with a valid Iridium position. | `sql_colt`, `latest_iridium_position_datetime` |
| `latest_iridium_position_cep_radius` | `int` | NOT NULL / -1 | -1 | km | CEP radius of that Iridium position. | `sql_coli`, `latest_iridium_position_cep_radius` |
| `latest_iridium_position_dive_number` | `smallint` | NOT NULL / -999 | -999 | — | Dive of that message. | `sql_coli`, `latest_iridium_position_dive_number` |
| `latest_iridium_position_latitude` | `double` | NOT NULL / 99.99999 | 99.99999 | deg N | Iridium latitude. | `sql_cold`, `latest_iridium_position_latitude` |
| `latest_iridium_position_longitude` | `double` | NOT NULL / 999.99999 | **99.99999** | deg E | Iridium longitude. | `sql_cold`, `latest_iridium_position_longitude` |
| `latest_iridium_startup_position_datetime` | `datetime` | NOT NULL / 1400-01-01 | — | UTC | **Not mapped.** The C++ member exists but is not persisted; the column keeps its default. | — |
| `latest_iridium_startup_sbd_momsn` | `int` | NOT NULL / -1 | — | — | **Not mapped.** As above. | — |
| `latest_sbd_momsn` | `int` | NOT NULL / -1 | -1 | — | MOMSN of the message that set the latest Iridium position. | `sql_coli`, `latest_sbd_momsn` |
| `latest_sbd_mtmsn` | `int` | NOT NULL / -1 | -1 | — | MTMSN of that message. | `sql_coli`, `latest_sbd_mtmsn` |
| `netcdf_file_generation_is_enabled` | `bit(1)` | NOT NULL / 0 | 0 | — | Per-float NetCDF switch. The daemon never changes it; it writes back the loaded value. | `sql_colb`, `netcdf_file_generation_is_enabled` |
| `phy_file_generation_is_enabled` | `bit(1)` | NULL / 0 | 0 | — | Per-float PHY switch. Set by `--phy-enable`/`--phy-disable`. A PHY file also needs `.meta` metadata and a complete dive (see [phy-file-format.md](phy-file-format.md)). | `sql_colb`, `phy_file_generation_is_enabled` |
| `start_of_mission_datetime` | `datetime` | NOT NULL / 1400-01-01 | zero date | UTC | Time of the newest built-in-test GPS fix (X-message 0x05). Messages sent more than 15 min before it are ignored (section 10.3). | `sql_colt`, `start_of_mission_datetime` |
| `xmsg_version_major` | `smallint unsigned` | NULL / 255 | 255 | — | X-format major version from the Argo message. 255 = unknown. | `sql_cou8`, `xmsg_version_major` |
| `xmsg_version_minor` | `smallint unsigned` | NULL / 255 | 255 | — | X-format minor version. | `sql_cou8`, `xmsg_version_minor` |

Gains and offsets are the raw u16 values of the Argo message 0xF0. A decoded value is `counts / gain − offset` ([xmessage-format.md](xmessage-format.md) A.1.5). The units above follow from that formula [inferred].

### 5.2 Reads

- **Startup.** `XFloat::LoadLocalCacheFromDatabase()` (`src/Main.cpp`) runs `SELECT <all columns> FROM XFloat;` and fills the in-memory cache. `XFloat::GetFloat()` repeats this if the cache is empty.
- **IMEI lookup.** Stage 2 (`src/stage2_MailProcessingThread.cpp`) needs a serial number for a message whose SBD attachment is invalid (the serial normally comes from the attachment). It runs:

  ```sql
  SELECT id_instrument FROM XFloat WHERE id_imei = <imei> ORDER BY id_instrument DESC LIMIT 1;
  ```

  If the IMEI was used by several floats, the highest serial number wins. If no row matches, the result is 0, which fails the 1000–11999 check, and the message is dropped without being saved.

### 5.3 Writes

Every save is a full-row upsert of the in-memory `XFloat` object, followed by an upsert of its `XFloatMetadata` row if the float has metadata. Saves happen:

1. at startup, for each `.meta` file with a usable DAC ID (`XFloat::LoadMetadataFromFiles`);
2. at startup, for each `--phy-enable`/`--phy-disable` serial number (`XFloat::StartupTasks`);
3. when an Argo message (0xF0) is assigned to a dive (`XFloatDiveData::AssignArgoData`): gains, offsets, instrument type and X-format version;
4. when a built-in-test GPS fix moves the start of mission forward (`XFloatDiveData::AssignGps`);
5. before every `XFloatDive` save (`XFloatDiveData::SaveObjectToDatabase`), so the dive's foreign key is satisfied. This also persists the latest GPS and Iridium positions, which are updated in memory as messages are decoded;
6. at shutdown, for every cached float (`XFloat::SaveLocalCacheToDatabase`, `src/Main.cpp`). It is skipped if the SQL server is unavailable.

A float gets its first row the first time one of these saves runs for it. Rows are never deleted by the daemon.

The "latest" columns only move forward in time. `start_of_mission_datetime` moves to any later built-in-test fix. None of these are reset by reprocessing (section 11).

## 6. Table `XFloatMetadata`

**Purpose.** One row per float that has a `.meta` file in `dir.meta` with a usable DAC ID. It mirrors the deployment metadata used for PHY files, NetCDF files and the decoder's pressure-offset decision. The file format is specified in [float-metadata-format.md](float-metadata-format.md).

**Primary key.** `id_instrument`.

**Foreign key.** `fk_XFloatMetadata_1 (id_instrument) → XFloat (id_instrument)`.

**Table description.** `XFloatMetadata::sql_table_description_` in `src/data/XFloatMetadata.cpp`.

### 6.1 Columns

| Column | SQL type | Null / schema default | Units | Meaning | C++ (macro, accessor) |
|---|---|---|---|---|---|
| `id_instrument` | `smallint` | NOT NULL | — | Float serial number. **PK, FK.** | `sql_coli`, `id_instrument` |
| `id_dac` | `smallint` | NOT NULL / -1 | — | Argo DAC ID. | `sql_coli`, `id_dac` |
| `id_imei` | `bigint` | NOT NULL / -1 | — | IMEI. Copied from the `XFloat` object when the metadata is attached at startup, not from the file. | `sql_coll`, `id_imei` |
| `id_wmo` | `int` | NOT NULL / -1 | — | WMO platform number. | `sql_coli`, `id_wmo` |
| `deployer` | `varchar(255)` | NULL | — | Deployer name(s). | `sql_cols`, `deployer` |
| `deployer_address` | `varchar(255)` | NULL | — | Deployer address. | `sql_cols`, `deployer_address` |
| `deployment_cruise_id` | `varchar(255)` | NULL | — | Deployment cruise ID. | `sql_cols`, `deployment_cruise_id` |
| `deployment_datetime_status` | `varchar(45)` | NULL | — | Status text of the deployment time. | `sql_cols`, `deployment_datetime_status` |
| `deployment_datetime` | `datetime` | NULL / 1400-01-01 | UTC | Deployment time; zero date if absent or unparseable. | `sql_colt`, `deployment_datetime` |
| `deployment_latitude` | `double` | NOT NULL / 99.99999 | deg N | Deployment latitude; 99.99999 if invalid. | `sql_cold`, `deployment_latitude` |
| `deployment_longitude` | `double` | NOT NULL / 999.99999 | deg E | Deployment longitude; 999.99999 or 99.99999 if invalid (section 14). | `sql_cold`, `deployment_longitude` |
| `deployment_platform` | `varchar(255)` | NULL | — | Ship or platform. | `sql_cols`, `deployment_platform` |
| `deployment_position_status` | `varchar(255)` | NULL | — | Status text of the deployment position. | `sql_cols`, `deployment_position_status` |
| `deployment_position_valid` | `bit(1)` | NOT NULL / 0 | — | 1 if the deployment position parsed. | `sql_colb`, `is_deployment_position_valid` |
| `deployment_type` | `varchar(255)` | NULL | — | Deployment type. | `sql_cols`, `deployment_type` |
| `manufacturer` | `varchar(45)` | NULL | — | Manufacturer. | `sql_cols`, `manufacturer` |
| `comment` | `text` | NULL | — | Free-text comment. | `sql_cols`, `comment` |
| `local_path` | `varchar(255)` | NULL | — | Path of the `.meta` file the row was read from. | `sql_cols`, `local_path` |
| `principal_investigators` | `varchar(255)` | NULL | — | PI name(s) (`PI` field). | `sql_cols`, `principal_investigators` |
| `principal_investigators_address` | `varchar(255)` | NULL / `WHOI, Woods Hole, MA` | — | PI address. | `sql_cols`, `principal_investigators_address` |
| `principal_investigators_originating_country` | `varchar(255)` | NULL / `USA` | — | PI country. | `sql_cols`, `principal_investigators_originating_country` |
| `wmo_instrument_type` | `smallint` | NOT NULL / -1 | — | WMO instrument type (for example 866, 867, 868, 873, 875, 876). The decoder uses it to decide on the surface pressure-offset correction when the mission configuration does not give the sensor type. | `sql_coli`, `wmo_instrument_type` |
| `wmo_recorder_type` | `smallint` | NOT NULL / -1 | — | WMO recorder type. | `sql_coli`, `wmo_recorder_type` |

Because a save writes every mapped column, an absent `.meta` field is stored as an empty string, not `NULL`. The schema defaults for the PI address and country apply only to rows inserted by other means.

### 6.2 Reads and writes

- **Write.** At startup, `XFloat::LoadMetadataFromFiles()` reads every `.meta` file. It keeps those whose DAC ID is > 0, ≠ 9999 and < 99999, attaches each to its `XFloat` object (copying `id_dac` and `id_wmo` to the float and the float's `id_imei` to the metadata) and saves both. The row is saved again with every later `XFloat` save.
- **Read.** The daemon never reads this table. Metadata always comes from the `.meta` files at startup.
- **Delete.** Never. A float whose `.meta` file is removed keeps its row.

## 7. Table `XFloatDive`

**Purpose.** One row per dive cycle of a float: message count, Iridium time span, completeness, output file paths, and the surface pressure statistics and pressure-offset correction computed by the decoder.

**Primary key.** `(id_instrument, id_dive)`.

**Foreign key.** `fk_XFloatDive_1 (id_instrument) → XFloat (id_instrument)`.

**Table description.** `XFloatDiveData::sql_table_description_` in `src/data/XFloatDiveData.cpp`. The C++ list flags `id_dive` and `id_instrument` as primary-key columns, matching the table.

### 7.1 Columns

| Column | SQL type | Null / schema default | Code default | Units | Meaning | C++ (macro, accessor) |
|---|---|---|---|---|---|---|
| `id_instrument` | `smallint` | NOT NULL | — | — | Float serial number. **PK, FK.** | `sql_coli`, `id_instrument` |
| `id_dive` | `smallint` | NOT NULL | — | — | Dive number. **PK.** | `sql_coli`, `id_dive` |
| `id_dac` | `smallint` | NOT NULL / -1 | from float | — | DAC ID, copied from the float when unknown. | `sql_coli`, `id_dac` |
| `id_imei` | `bigint` | NOT NULL / -1 | from float | — | IMEI, copied from the float when unknown. | `sql_coll`, `id_imei` |
| `id_wmo` | `int` | NOT NULL / -1 | from float | — | WMO number, copied from the float when unknown. | `sql_coli`, `id_wmo` |
| `complete` | `bit(1)` | NOT NULL / 0 | 0 | — | 1 once the dive has complete profile data, known gains and offsets, contiguous packet IDs from 0, and a valid profile position (`XFloatDiveData::CheckCompleteness`). | `sql_colb`, `is_complete` |
| `intermediate_listing_file_path` | `varchar(255)` | NULL | `''` | — | Path of the listing file, `<dir.lis>/<serial>/lis<serial>-<dive>.txt` (four-digit fields). See [listing-file-format.md](listing-file-format.md). | `sql_colp`, `intermediate_listing_file_path` |
| `iridium_position_datetime_first` | `datetime` | NULL / 1400-01-01 | zero date | UTC | Earliest Iridium "Time of Session" of the dive's messages. | `sql_colt`, `iridium_position_datetime_first` |
| `iridium_position_datetime_last` | `datetime` | NULL / 1400-01-01 | zero date | UTC | Latest Iridium "Time of Session" of the dive's messages. | `sql_colt`, `iridium_position_datetime_last` |
| `num_sbd_msgs` | `smallint unsigned` | NOT NULL / 0 | 0 | — | Number of messages assigned to the dive. Retransmissions under a new MOMSN count; duplicate Message-IDs do not. | `sql_cu16`, `num_sbd_msgs` |
| `phy_file_path` | `varchar(255)` | NULL | `''` | — | Path of the PHY file, `<dir.phy>/<serial>/<dac>_<serial>_<dive>.phy`; empty if none was written. See [phy-file-format.md](phy-file-format.md). | `sql_colp`, `phy_file_path` |
| `surface_pres_max` | `float` | NULL / -9.999 | -9.999 | dbar | Maximum surface pressure reported by the float. -9.999 = none. | `sql_colf`, `surface_pres_max` |
| `surface_pres_mean` | `float` | NULL / -9.999 | -9.999 | dbar | Mean surface pressure. | `sql_colf`, `surface_pres_mean` |
| `surface_pres_min` | `float` | NULL / -9.999 | -9.999 | dbar | Minimum surface pressure sample after outliers are discarded. | `sql_colf`, `surface_pres_min` |
| `surface_pres_variance` | `float` | NULL / -9.999 | -9.999 | dbar² | Surface pressure variance. | `sql_colf`, `surface_pres_variance` |
| `surface_pres_valid` | `bit(1)` | NULL / 0 | 0 | — | 1 if the gains are known, the mean is within ±5 dbar, the surface time series is complete and the dive has a valid GPS position. | `sql_colb`, `is_surface_pres_valid` |
| `pres_offs` | `float` | NOT NULL / 0 | 0 | dbar | Surface pressure offset: the mean of `surface_pres_min` over this dive and up to 3 preceding dives with valid surface data and at least the minimum sample count. Computed only for RBR sensors. | `sql_colf`, `pres_offs` |
| `pres_offs_num_dives` | `smallint` | NOT NULL / -1 | -1 | — | Number of dives in that mean. | `sql_coli`, `pres_offs_num_dives` |
| `pres_offs_num_samples` | `smallint` | NOT NULL / -1 | -1 | — | Total surface samples of those dives. | `sql_coli`, `pres_offs_num_samples` |
| `pres_offs_valid` | `bit(1)` | NOT NULL / 0 | 0 | — | 1 if `pres_offs` was computed. The listing and PHY outputs then apply it. | `sql_colb`, `is_pres_offs_valid` |

The surface sample count (`surface_pres_num_samples`) is a C++ member but is not stored.

### 7.2 Reads

- `XFloatDiveData::GetDiveDataObjectFromDatabase()` (`src/data/XFloat.cpp`), called by `XFloat::GetDiveData()` when a dive is not in memory:

  ```sql
  SELECT <all columns> FROM XFloatDive WHERE id_instrument = <serial> AND id_dive = <dive> LIMIT 1;
  ```

  If a row exists, the dive's messages are read from `XFloatMail` and decoded again (section 10.4). The row's own values are only a starting point: decoding resets and recomputes the Iridium times, message count, surface statistics, offset and completeness.
- The pressure-offset computation loads up to three preceding dives this way (`ComputePressureOffsetFromSurfaceTimeSeries`).

### 7.3 Writes and deletes

- **Write.** `XFloatDiveData::SaveObjectToDatabase()` upserts the row after every decoded message of the dive. It runs from `ProcessDiveData` in stage 3 (`src/stage3_TelemetryDecoderThread.h`), both while the dive is incomplete and once it is complete. Duplicate messages also trigger a save.
- **Delete.** `XFloatDiveData::DeleteFromDatabase(serial)` runs `DELETE FROM XGPS WHERE id_instrument = <serial>;` then `DELETE FROM XFloatDive WHERE id_instrument = <serial>;` at the start of reprocessing (section 11).

## 8. Table `XGPS`

**Purpose.** One row per GPS fix (X-messages 0x00–0x05) of a dive. It is used for KML trajectories and as the fallback profile position.

**Primary key.** `(id_instrument, id_dive, datetime)`.

**Indexes.** `id_instrument (id_instrument)`.

**Foreign key.** `fk_XGPS_1 (id_instrument, id_dive) → XFloatDive (id_instrument, id_dive)`.

**Table description.** `XGPS::sql_table_description_` in `src/data/xmsg/XGPS.cpp`. The C++ list flags `id_instrument`, `id_dive` and **`id_type`** as primary-key columns. The table's key uses `datetime`, not `id_type`. See section 8.3.

### 8.1 Columns

The X-message fields are specified in [xmessage-format.md](xmessage-format.md) A.2.

| Column | SQL type | Null / schema default | Units | Meaning | C++ (macro, accessor) |
|---|---|---|---|---|---|
| `id_instrument` | `smallint` | NOT NULL | — | Float serial number. **PK, FK.** | `sql_cu16`, `id_instrument` |
| `id_dive` | `smallint` | NOT NULL | — | Dive number. **PK, FK.** | `sql_ci16`, `id_dive` |
| `datetime` | `datetime` | NOT NULL / 1400-01-01 | UTC | Fix time from GPS week, day, hour and minute. Seconds are always 0. Zero date if the fix is invalid. **PK.** | `sql_colt`, `position_datetime` |
| `id_type` | `smallint` | NULL | — | X-message ID: 0 diagnostic dive, 1 start of dive, 2 end of dive, 3 after mission abort, 5 built-in test. | `sql_cou8`, `id_type` (= `id_xmsg_type`) |
| `hdop` | `float` | NULL | — | Horizontal dilution of precision (`u8 / 10`). | `sql_colf`, `horizontal_dilution_of_precision` |
| `num_satellites` | `smallint unsigned` | NULL | — | Satellites used. | `sql_cou8`, `num_satellites` |
| `snr_min` | `smallint unsigned` | NULL | — | Minimum SNR. | `sql_cou8`, `signal_to_noise_min` |
| `snr_mean` | `smallint unsigned` | NULL | — | Mean SNR. | `sql_cou8`, `signal_to_noise_mean` |
| `snr_max` | `smallint unsigned` | NULL | — | Maximum SNR. | `sql_cou8`, `signal_to_noise_max` |
| `time_to_fix` | `smallint unsigned` | NULL | s | Time to acquire the fix (`u8 × 10`). | `sql_cu16`, `time_to_acquire_fix` |
| `latitude` | `double` | NOT NULL / 99.99999 | deg N | Latitude. 99.99999 if invalid. | `sql_cold`, `position_latitude` |
| `longitude` | `double` | NOT NULL / 999.99999 | deg E | Longitude. 999.99999 if the fix had no position; **99.99999** if the decoder invalidated it (section 14). | `sql_cold`, `position_longitude` |
| `valid` | `bit(1)` | NOT NULL / 0 | — | 1 if the fix is valid. | `sql_colb`, `position_is_valid` |

A fix that the decoder decodes as valid is invalidated when it is later than the dive's last Iridium time or later than the current time (`XFloatDiveData::AssignGps`). Its time is then cleared to the zero date and its position to 99.99999/99.99999.

### 8.2 Reads and writes

- **Write.** Each of the dive's fixes is upserted after the `XFloatDive` row, in every `XFloatDiveData::SaveObjectToDatabase()` call.
- **Read, KML.** `XFloat::GetGpsPositions()` (stage 4, KML and trajectory output; see [kml-file-format.md](kml-file-format.md)):

  ```sql
  SELECT <all columns> FROM XGPS
  WHERE id_instrument = <serial> AND valid = 1 AND id_type <= 3
    [AND datetime > '<start of mission, YYYYMMDDTHHMMSS>']
    [AND datetime > '<deployment time, YYYYMMDDTHHMMSS>']
  ORDER BY datetime ASC;
  ```

- **Read, profile position.** If a dive has no usable end-of-dive fix, `XFloatDiveData` looks for the next dive's start-of-dive fix (`src/data/XFloatDiveData.cpp`):

  ```sql
  SELECT <all columns> FROM XGPS
  WHERE id_instrument = <serial> AND id_dive = <dive + 1> AND id_type = 1 AND valid = 1;
  ```

- **Delete.** `XGPS::DeleteFromDatabase(serial)` runs `DELETE FROM XGPS WHERE id_instrument = <serial>;` at the start of reprocessing. The per-dive overload `DeleteFromDatabase(serial, dive)` builds `id_dive == <dive>`, which MySQL rejects. It is not called.

### 8.3 Effect of the key mismatch

The upsert matches rows on the table's key `(id_instrument, id_dive, datetime)`. The C++ flags only decide which columns the `ON DUPLICATE KEY UPDATE` clause leaves out. As a result:

- `id_type` is never updated on an existing row; `datetime` is (with the same value).
- Two fixes of one dive with the same minute share a row. The later save overwrites the position and quality fields but keeps the first fix's `id_type`.
- All invalid fixes of a dive have the zero date, so they share a single row [inferred].

## 9. Table `XFloatMail`

**Purpose.** One row per Iridium SBD mail message saved by stage 2: the mail header fields, the Iridium session details from the body, the raw SBD attachment, the full raw mail text, and the paths of the RAW and SBD files written for it. It is the source for all reprocessing. The mail format is specified in [iridium-mail-format.md](iridium-mail-format.md).

**Primary key.** `(id_imei, id_dive, id_sbd_momsn, id_sbd_packet, id_msg)`. It starts with `id_imei` so that each float's messages are stored together. The same columns formed the key before 2026-10-26, so the same rows are duplicates.

**Indexes.**

| Index | Columns | Used by |
|---|---|---|
| `id_sbd_momsn_idx` | `id_sbd_momsn` | not used by the daemon's queries [inferred] |
| `inst_dive_idx` | `id_instrument, id_dive` | a dive's messages (section 10.4); a float's messages for reprocessing (section 11.1) |
| `active_idx` | `iridium_position_datetime, msg_is_valid, id_instrument` | the active-float query (section 11.3), answered from the index alone |

**Foreign keys.** None.

**Table description.** `XFloatMailMessage::sql_table_description_` in `src/data/XFloatMailMessage.cpp`. Its primary-key flags match the table.

### 9.1 Columns

| Column | SQL type | Null / schema default | Code default | Units | Meaning | C++ (macro, accessor) |
|---|---|---|---|---|---|---|
| `id_imei` | `bigint unsigned` | NOT NULL | -1 | — | IMEI from the subject `SBD Msg From Unit: <IMEI>`. **PK.** | `sql_coll`, `id_imei` |
| `id_dive` | `smallint` | NOT NULL / -99 | -999 | — | Dive number from SBD bytes 5–6. **PK.** | `sql_coli`, `id_dive` |
| `id_sbd_momsn` | `smallint` | NOT NULL / -1 | -1 | — | MOMSN from the body (`MOMSN: n`). **PK.** | `sql_coli`, `id_sbd_momsn` |
| `id_sbd_packet` | `int` | NOT NULL / -1 | -1 | — | Packet ID from SBD byte 7. **PK.** | `sql_coli`, `id_sbd_packet` |
| `id_msg` | `varchar(128)` | NOT NULL | `''` | — | The `Message-ID` header without angle brackets [inferred]. **PK.** | `sql_cols`, `id_msg` |
| `id_instrument` | `smallint` | NOT NULL / -1 | -1 | — | Float serial number from SBD bytes 3–4, or from the IMEI lookup (section 5.2). | `sql_coli`, `id_instrument` |
| `id_sbd_mtmsn` | `int` | NOT NULL / -1 | -1 | — | MTMSN from the body. | `sql_coli`, `id_sbd_mtmsn` |
| `id_msg_uid` | `int` | NOT NULL / -1 | -1 | — | IMAP UID of the message in the inbox when fetched; -1 for mail read from files. | `sql_coll`, `id_msg_uid` |
| `iridium_position_cep_radius` | `int` | NULL | 999999 | km | `CEPradius = n` from the body. 999999 = none. | `sql_coli`, `iridium_position_cep_radius` |
| `iridium_position_datetime` | `datetime` | NULL | zero date | UTC | `Time of Session (UTC)` from the body. | `sql_colt`, `iridium_position_datetime` |
| `iridium_position_is_valid` | `bit(1)` | NOT NULL / 0 | 0 | — | 1 if the `Unit Location` latitude and longitude form a valid position (`Position::IsPositionValid`). | `sql_colb`, `iridium_position_is_valid` |
| `iridium_position_latitude` | `double` | NULL | 99.99999 | deg N | `Unit Location: Lat = …`. | `sql_cold`, `iridium_position_latitude` |
| `iridium_position_longitude` | `double` | NULL | 999.99999 | deg E | `Long = …`. | `sql_cold`, `iridium_position_longitude` |
| `msg_body_is_valid` | `bit(1)` | NOT NULL / 0 | 0 | — | 1 if the body gave MOMSN, MTMSN and the session time. | `sql_colb`, `msg_body_is_valid` |
| `msg_header_datetime` | `datetime` | NULL | zero date | UTC | `Date` header, converted to UTC. | `sql_colt`, `msg_header_datetime` |
| `msg_header_from` | `varchar(128)` | NULL | `''` | — | E-mail address of the `From` header. | `sql_cols`, `msg_header_from` |
| `msg_header_subject` | `varchar(128)` | NULL | `''` | — | `Subject` header. | `sql_cols`, `msg_header_subject` |
| `msg_header_is_valid` | `bit(1)` | NOT NULL / 0 | 0 | — | 1 if sender and subject match the configured patterns. Only such messages are saved, so stored rows have 1. | `sql_colb`, `msg_header_is_valid` |
| `msg_raw_text` | `text` | NULL | `''` | — | The complete mail as text (section 9.2). | `sql_cols`, `msg_raw_text` |
| `msg_is_valid` | `bit(1)` | NOT NULL / 0 | 0 | — | `msg_header_is_valid AND msg_body_is_valid AND sbd_data_is_valid`. | `sql_colb`, `msg_is_valid` |
| `sbd_data` | `blob` | NULL | empty | bytes | The SBD attachment, byte for byte (section 3.5). | `sql_colv`, `sbd_data` |
| `sbd_data_is_valid` | `bit(1)` | NOT NULL / 0 | 0 | — | 1 if the attachment has at least 8 bytes and the session status is 0 or 2. | `sql_colb`, `sbd_data_is_valid` |
| `sbd_file_local_path` | `varchar(256)` | NULL | `''` | — | Path of the `.sbd` file written for the message; empty if SBD files are disabled. See [sbd-file-format.md](sbd-file-format.md). | `sql_colp`, `sbd_file_local_path` |
| `raw_file_local_path` | `varchar(256)` | NULL | `''` | — | Path of the RAW mail file, `<dir.raw>/<serial>/raw<serial>-<momsn>-<dive>-<packet>.txt` (or `…-incomplete-…`). See [raw-file-format.md](raw-file-format.md). | `sql_colp`, `raw_file_local_path` |
| `sbd_file_name` | `varchar(128)` | NULL | `''` | — | Attachment file name, `<IMEI>_<MOMSN, 6 digits>.sbd`. | `sql_cols`, `sbd_file_name` |
| `sbd_file_size_bytes` | `int` | NULL | -1 | bytes | `Message Size (bytes)` from the body. | `sql_coli`, `sbd_file_size_bytes` |
| `sbd_session_status_code` | `int` | NULL | -1 | — | Two-digit code of `Session Status: NN - text`. | `sql_coli`, `sbd_session_status_code` |
| `sbd_session_status_string` | `varchar(64)` | NULL | `''` | — | Text after the code. | `sql_cols`, `sbd_session_status_string` |

The mail body text and the raw header text are parsed but not stored. Both are contained in `msg_raw_text`. Columns `id_dac`, `id_wmo`, `msg_body_text` and `msg_header_raw_text` were dropped on 2026-10-26 (`bin/migrate-xfloatmail.sh`).

### 9.2 `msg_raw_text`

- Server mode (IMAP): the message exactly as the server returns it (`vmime::net::message::extract`): RFC 5322 headers, a blank line, and the MIME body with the text part and the base64-encoded `.sbd` attachment.
- File mode (`dir.mail`): the message as VMime regenerates it from the parsed file (`vmime::message::generate`). It may differ from the input file in line folding or header order [inferred].
- It is stored as `TEXT` in `utf8mb3`, so at most 65,535 bytes. A typical Iridium mail is a few kilobytes.
- Reprocessing does not parse `msg_raw_text`. It rebuilds message objects from the other columns. The text is kept for reference and for the RAW file.

### 9.3 Reads and writes

- **Write.** Stage 2, `XFloatMailProcessingService::ProcessMailMsg()` (`src/stage2_MailProcessingThread.cpp`), for each message:
  1. Messages with an invalid header are dropped and not saved.
  2. An unknown serial number is looked up by IMEI (section 5.2). Messages whose serial number is outside 1000–11999 are dropped and not saved.
  3. The RAW and SBD files are written. Their paths are set on the object.
  4. The row is upserted. This happens **before** decoding, so a message is decoded only once it is saved.
  5. If `sbd_data_is_valid`, the message is queued for stage 3.

  If the save fails and the server is unreachable, the message is held and retried every 10 s, and stage 1 fetches no new mail until the server is back. If the save fails while the server is reachable, the message is dropped (and, with `mail.moveMessages`, moved out of the inbox).
- Reprocessed messages pass through stage 2 again, so their rows are upserted again with the same values (section 11).
- **Read.** See sections 10.4 and 11.
- **Delete.** Never. Rows are permanent.

## 10. Table operations during normal operation

### 10.1 Startup (`src/Main.cpp`)

1. `SELECT … FROM XFloat;` loads all floats (section 5.2).
2. For each usable `.meta` file: upsert `XFloat`, then upsert `XFloatMetadata` (section 6.2).
3. For each `--phy-enable`/`--phy-disable` serial number: upsert `XFloat`, then `XFloatMetadata` if present.
4. Stage 1 runs the reprocessing flows (section 11), then starts fetching mail.

### 10.2 Each new message

1. Stage 2 upserts its `XFloatMail` row (section 9.3).
2. Stage 3 finds the dive in memory, or loads it from `XFloatDive` and `XFloatMail` (section 10.4), or creates it.
3. Stage 3 decodes the message. Depending on its content it may upsert `XFloat` (Argo message; built-in-test GPS fix).
4. Stage 3 writes the listing file (and, if the dive is complete, the PHY file), then saves the dive: upsert `XFloat` + `XFloatMetadata`, upsert `XFloatDive`, upsert each of the dive's `XGPS` rows.

### 10.3 Message filters that depend on the database

`XFloatDiveData::DecodeMailMessageData()` ignores a message if:

- its Iridium session time, or else its `Date` header, is more than 15 minutes before `XFloat.start_of_mission_datetime`; or
- its Iridium session time is more than 15 minutes before the deployment time (the in-memory value from the `.meta` file, also stored in `XFloatMetadata.deployment_datetime`).

Ignored messages stay in `XFloatMail`.

### 10.4 Reloading a dive that is not in memory

When stage 3 needs a dive that is not cached, and an `XFloatDive` row exists for it, `XFloat::GetDiveData()` reads the dive's messages with `XFloatDiveData::GetDiveDataMailMessagesFromDatabase()`:

```sql
SELECT <all columns> FROM XFloatMail
WHERE id_instrument = <serial> AND id_dive = <dive> AND msg_is_valid = 1
  [AND iridium_position_datetime >= '<start of mission − 15 min, YYYYMMDDTHHMMSS>']
  [AND iridium_position_datetime >= '<deployment time − 15 min, YYYYMMDDTHHMMSS>'];
```

It resets the dive's Iridium times and message count and decodes each message again. This happens, for example, when a float that was not reprocessed at startup sends another packet of an earlier dive, or when the pressure-offset computation needs a preceding dive.

### 10.5 Shutdown

`XFloat::SaveLocalCacheToDatabase()` upserts every cached float and its metadata, one float at a time, so one failure does not stop the rest. It does nothing if the server is unavailable.

### 10.6 Running without the database (`sql.enable = 0`)

With `sql.enable = 0` ([configuration-file-format.md](configuration-file-format.md), section 4.6) the daemon makes no connection and none of the statements in sections 10.1–10.5 and 11 run. This is meant for decoding a directory of saved e-mails (`mail.fetchMode = 1`, [iridium-mail-format.md](iridium-mail-format.md), section 13).

- **Access layer.** `SqlTableDescription` saves and deletes do nothing, and selects return no rows (`src/utility/SqlTableDescription.h`). `LocalMysqlConfiguration::getSqlConnection()` throws, so a single-value select (`SelectValueFromDatabase`) fails. `isSqlServerAvailable()` returns true without connecting, so stages 1 and 2 never wait for the server. `XFloat::LoadLocalCacheFromDatabase()` and `SaveLocalCacheToDatabase()` return at once.
- **Floats and dives.** The float cache starts empty and is filled from the `.meta` files and the messages read in the run. A dive that is not in memory is created empty (section 10.4 finds no row), so the pressure offset uses only preceding dives read in the same run, and a float's sensor type is known only once a mission configuration message has been read.
- **GPS reads.** The profile position's fallback (the next dive's start-of-dive fix, section 8.2) and the KML positions come from the dives in memory (`XFloatDiveData::GetGpsProfilePosition()`, `XFloat::GetGpsPositions()`), with the same validity, type and date filters as the queries.
- **Serial number.** A message whose subject gives no serial number gets it from a float in memory with the same IMEI instead of section 5.2's query; otherwise it is dropped.
- **Message UID.** No row is inserted, so every message keeps the UID −1, which RAW files and the log show (`??????`).
- **Reprocessing.** `-r` and `-R` stop startup; `-A` and the reprocessing implied by `--phy-enable` are skipped (section 11).

## 11. Reprocessing flows

Reprocessing rebuilds a float's dive data from its stored messages. It runs once, in the stage 1 thread, before mail fetching starts (`XFloatMailRetrievalService::operator()`, `src/stage1_MailRetrievalThread.cpp`).

### 11.1 Per-float procedure

For each serial number in the reprocessing list:

1. Delete the float's derived rows:

   ```sql
   DELETE FROM XGPS       WHERE id_instrument = <serial>;
   DELETE FROM XFloatDive WHERE id_instrument = <serial>;
   ```

2. Select all its stored messages, in MOMSN order:

   ```sql
   SELECT id_dive, id_imei, id_sbd_momsn, id_sbd_packet, id_msg, id_instrument, id_msg_uid, …,
          HEX(sbd_data) AS sbd_data, …
   FROM XFloatMail
   WHERE id_instrument = <serial>
     [AND iridium_position_datetime > '<start of mission, YYYY-MM-DDTHH:MM:SSZ>']
   ORDER BY id_sbd_momsn;
   ```

3. Push each message into the stage 2 queue. They then follow the normal path (section 10.2): the `XFloatMail` row is upserted again, the RAW/SBD files are written if missing, and stage 3 rebuilds the `XFloatDive` and `XGPS` rows.

Not deleted or reset:

- `XFloatMail` rows. They are the input.
- The `XFloat` row. Its gains and offsets end up as those of the last Argo message decoded. Its latest-position columns and `start_of_mission_datetime` keep any later values they already had.
- `XFloatMetadata`.

After the list is done, stage 1 waits up to 30 s (for a non-daemon run to be stopped), then starts fetching mail.

### 11.2 Building the reprocessing list

The list is built from the options in this order. Duplicate serial numbers are not removed (section 14).

| Option | Adds | Notes |
|---|---|---|
| `-r`, `--reprocess ID …` | the given serial numbers | always |
| `-R`, `--reprocess-all` | every float in the cache, which is every `XFloat` row plus any float created by the startup metadata/PHY steps | when given, `--phy-enable` and `-A` add nothing |
| `--phy-enable ID` | the given serial numbers | only without `-R` |
| `-A`, `--reprocess-active 0\|1` (default 1) | floats with a valid message in the last 90 days (section 11.3) | only without `-R` |

### 11.3 Active-float query (`-A`)

```sql
SELECT DISTINCT id_instrument FROM XFloatMail
WHERE msg_is_valid = 1 AND iridium_position_datetime >= (CURDATE() - INTERVAL 90 day)
ORDER BY id_instrument;
```

`XFloatMailMessage::SelectInstrumentIdsFromDatabase()` selects only the serial number, so `active_idx` answers the query from the index. `CURDATE()` uses the server's time zone.

## 12. Example queries

All examples are read-only. Replace `11186` and `107` with the float serial number and dive.

**All messages of a dive, in packet order, with the packet bytes as hex:**

```sql
SELECT id_sbd_packet, id_sbd_momsn, iridium_position_datetime,
       sbd_session_status_code, LENGTH(sbd_data) AS bytes, HEX(sbd_data) AS sbd_hex
FROM XFloatMail
WHERE id_instrument = 11186 AND id_dive = 107 AND msg_is_valid = 1
ORDER BY id_sbd_packet, id_sbd_momsn;
```

Packets with the same `id_sbd_packet` and identical `sbd_hex` are retransmissions.

**Missing packet IDs of a dive** (the highest ID is normally sent first, so `MAX + 1` is the expected count):

```sql
SELECT MAX(id_sbd_packet) + 1 AS expected, COUNT(DISTINCT id_sbd_packet) AS received
FROM XFloatMail
WHERE id_instrument = 11186 AND id_dive = 107 AND msg_is_valid = 1;
```

**Header fields decoded from the stored bytes** (offsets from section 3.5):

```sql
SELECT id_sbd_momsn,
       CONV(HEX(SUBSTRING(sbd_data, 4, 2)), 16, 10) AS serial,
       CONV(HEX(SUBSTRING(sbd_data, 6, 2)), 16, 10) AS dive_u16,   -- 65535 = dive -1
       ASCII(SUBSTRING(sbd_data, 8, 1))             AS packet
FROM XFloatMail
WHERE id_instrument = 11186 AND id_dive = 107;
```

**Last known position of a float, from the cache row:**

```sql
SELECT id_instrument,
       latest_gps_position_datetime, latest_gps_position_dive_number,
       latest_gps_position_latitude, latest_gps_position_longitude,
       latest_iridium_position_datetime, latest_iridium_position_latitude,
       latest_iridium_position_longitude, latest_iridium_position_cep_radius
FROM XFloat
WHERE id_instrument = 11186;
```

The `XFloat` row is updated on every dive save and at shutdown, so it may lag the in-memory state by one save.

**Last valid surface GPS fix of a float, from the fixes:**

```sql
SELECT id_dive, id_type, datetime, latitude, longitude, hdop, num_satellites
FROM XGPS
WHERE id_instrument = 11186 AND valid = 1 AND id_type <= 3
ORDER BY datetime DESC
LIMIT 1;
```

**Last Iridium position of a float, from the messages:**

```sql
SELECT id_dive, id_sbd_momsn, iridium_position_datetime,
       iridium_position_latitude, iridium_position_longitude, iridium_position_cep_radius
FROM XFloatMail
WHERE id_instrument = 11186 AND iridium_position_is_valid = 1
ORDER BY iridium_position_datetime DESC
LIMIT 1;
```

**Dives of a float and their state:**

```sql
SELECT id_dive, complete, num_sbd_msgs, iridium_position_datetime_first,
       iridium_position_datetime_last, surface_pres_valid, pres_offs_valid, pres_offs,
       phy_file_path
FROM XFloatDive
WHERE id_instrument = 11186
ORDER BY id_dive;
```

**Floats that reported in the last 90 days** (same as `-A`):

```sql
SELECT DISTINCT id_instrument FROM XFloatMail
WHERE msg_is_valid = 1 AND iridium_position_datetime >= (CURDATE() - INTERVAL 90 day)
ORDER BY id_instrument;
```

**All floats with metadata, with unset times shown as NULL:**

```sql
SELECT f.id_instrument, f.id_wmo, f.id_imei, m.wmo_instrument_type,
       NULLIF(NULLIF(m.deployment_datetime, '0000-00-00 00:00:00'), '1400-01-01 00:00:00')
           AS deployed,
       NULLIF(NULLIF(f.start_of_mission_datetime, '0000-00-00 00:00:00'), '1400-01-01 00:00:00')
           AS mission_start
FROM XFloat f LEFT JOIN XFloatMetadata m USING (id_instrument)
ORDER BY f.id_instrument;
```

Comparing with `'0000-00-00 00:00:00'` needs the same `sql_mode` as the daemon (section 2.3).

**Extract one message's SBD bytes to a file** (shell; the account needs `SELECT` on the schema):

```
mysql -N -B -e "SELECT HEX(sbd_data) FROM XFloatMail
                WHERE id_instrument = 11186 AND id_dive = 107 AND id_sbd_packet = 0
                ORDER BY id_sbd_momsn LIMIT 1" xmtps_alamo | xxd -r -p > packet.sbd
```

## 13. Creating and migrating the schema

- `sql/xmtps-schema.sql` creates all five tables with `CREATE TABLE IF NOT EXISTS`, in foreign-key order. Running it on an existing database changes nothing. It is the production schema from a `mysqldump` of 2026-09-08, plus the later index changes. Create the database first:

  ```
  mysql -u <user> -p -e "CREATE DATABASE xmtps_alamo DEFAULT CHARACTER SET utf8mb4"
  mysql -u <user> -p xmtps_alamo < sql/xmtps-schema.sql
  ```

- The daemon does not create or alter tables. It needs `SELECT`, `INSERT`, `UPDATE` and `DELETE` on the five tables [inferred].
- `bin/migrate-xfloatmail.sh` applies the 2026-10-26 `XFloatMail` change to an older database: it drops `id_dac`, `id_wmo`, `msg_body_text` and `msg_header_raw_text`, reorders the primary key to start with `id_imei`, and replaces the old indexes with `inst_dive_idx` and `active_idx`. It checks the current layout first and refuses to run while the deployed binary still writes the dropped columns.
- `bin/move-mysql-datadir.sh` moves the MySQL data directory on argus to `/data1/mysql`. It does not change the schema.
- `sql/model/*.mwb` are MySQL Workbench models (versions 01–04, with backups). They are historical. Model 04 defines `BEFORE INSERT`/`BEFORE UPDATE` triggers on `XFloatMail` and `XFloatMetadata` that create a matching `XFloat` row. `sql/xmtps-schema.sql`, taken from the production dump, has no triggers, so production has none [inferred]. The `'1400-01-01'` and other column defaults date from that design, in which rows were created with only key columns.

## 14. Open questions / discrepancies

**Bugs and risks found in the code and schema**

1. **`XFloatMail.id_sbd_momsn` is `smallint` (max 32,767).** An Iridium MOMSN is a 16-bit counter (0–65,535). Under `STRICT_TRANS_TABLES`, saving a message with MOMSN ≥ 32,768 fails with "Out of range value". The SQL server is still reachable, so stage 2 drops the message (section 9.3) and it is lost to the database. The column should be `smallint unsigned` or `int`, like `XFloat.latest_sbd_momsn`. Whether any float has reached that MOMSN was not checked.
2. **Invalid longitude constant.** `IRI_POS_INVALID_LON` is defined as `UTIL_POS_INVALID_LAT` (`src/data/IridiumDefinitions.h`), and `XFLT_POS_INVALID_LON` uses it (`src/data/XFloatDefinitions.h`). The "invalid" longitude written by `XFloat` (`latest_*_longitude` before any fix), by invalidated `XGPS` fixes, and by one `.meta` parsing branch is therefore 99.99999, not the schema default 999.99999. 99.99999 is a valid longitude. A query that tests `longitude = 999.99999` misses these rows; test the `valid` flag or the latitude instead.
3. **`XGPS` key mismatch.** The C++ description flags `id_type` as a key column; the table's key uses `datetime` (section 8.3). Fixes with the same minute in one dive share a row, `id_type` is never updated, and all invalid fixes of a dive (zero date) collapse into one row.
4. **Reprocessing time filter.** Reprocessing selects messages with `iridium_position_datetime > '<start of mission>'`. Normal decoding and the dive reload accept messages from 15 minutes before the start of mission (`>=`, `− 15 min`). So reprocessing drops messages that live processing kept. The reprocessing literal is `'YYYY-MM-DDTHH:MM:SSZ'`. MySQL does not document a trailing `Z` in `DATETIME` literals; the comparison presumably works with a "truncated incorrect datetime value" warning [inferred, not tested].
5. **Reprocessing does not filter on `msg_is_valid`.** It pushes every stored message of the float to stage 2, which drops those whose SBD data is invalid. The dive reload (section 10.4) filters on `msg_is_valid = 1`. The results are the same, but reprocessing does extra work.
6. **Duplicate serial numbers in the reprocessing list.** `-r` IDs, `--phy-enable` IDs, the active floats (`-A`, on by default) and all floats (`-R`) are concatenated without removing duplicates. A float given with `-r` that is also active is reprocessed twice. The second pass deletes the float's `XFloatDive`/`XGPS` rows while the first pass may still be decoding. The rows are written again because duplicate messages still trigger a dive save, but the order is timing-dependent [inferred].
7. **Reprocessing ignores `id_imei`.** Messages are ordered by `id_sbd_momsn` only. If a float's modem was replaced, the two IMEIs' MOMSN sequences interleave. Decoding is keyed by dive and packet, so the effect is limited to which Argo message's gains end up in `XFloat` and the order of retransmission detection [inferred].
8. **`XFloat` is not reset by reprocessing.** Latest positions and the start of mission are only moved forward. A bad value (for example a wrong built-in-test fix time) survives reprocessing and must be corrected in the table by hand.
9. **IMEI lookup returning no row.** `SelectValueFromDatabase<int>` returns 0 when no `XFloat` row has the IMEI. Such a message (invalid SBD data and unknown IMEI) fails the serial-number range check and is not saved, although the README says every message received is stored.
10. **`id_msg` length.** `varchar(128)` is part of the key. A `Message-ID` longer than 128 characters fails the save under strict mode. Iridium's IDs are much shorter.
11. **Empty-hex read.** `Column_vector_uint8_t::SetObjectValueFromResultSet` loops to `end() - 1`. For an empty or `NULL` `sbd_data` this compares an iterator before `begin()` (undefined behavior, harmless in practice).
12. **`Column_char` does not compile if used.** It passes `SqlTableColumnDescription<C, double>` arguments to a `<C, char>` base. No table uses it.
13. **`sql.protocol` initializer.** `LocalMysqlConfiguration::protocol_` is statically initialized to `CFG_MYSQL_DFLT_SCHEMA` (`"float_db"`) instead of `CFG_MYSQL_DFLT_PROTOCOL`. The option parser's default `tcp` overwrites it, so it has no effect.

**Schema, code and README disagreements**

14. `XFloat.offset_salinity_surface` defaults to 999. All other gain and offset columns default to 9999 (the code's "invalid" value). It looks like a typo; the daemon always writes 9999.
15. `XFloat.latest_iridium_startup_position_datetime` and `latest_iridium_startup_sbd_momsn` exist in the table and as C++ members but are not mapped. They always hold their defaults.
16. `XFloatMail.id_imei` is `bigint unsigned`; `id_imei` in the other tables is signed `bigint` with default -1. An unknown IMEI (-1) cannot be stored in `XFloatMail`; this does not happen because such messages have an invalid header and are not saved.
17. `XFloatMail.id_dive` defaults to -99; the code's unknown dive is -999. The daemon always writes the column, so the default is unused.
18. `id_msg_uid` is mapped as `long` (`sql_coll`) but the column is `int`. IMAP UIDs are 32-bit unsigned, so UIDs above 2,147,483,647 would not fit. Not expected in practice.
19. `XFloatMetadata` is written but never read. The README's "reloaded at startup" applies to `XFloat` only; metadata is always re-read from the `.meta` files.
20. The README says the database is created with `utf8mb4`, but every table is `utf8mb3` (section 2.2).
21. The surface pressure sample count used by the pressure-offset computation is not stored. It is recomputed when a dive is reloaded, because the dive's messages are decoded again.
22. `XGPS.datetime` has no seconds (the GPS message sends none), so its precision is one minute.
