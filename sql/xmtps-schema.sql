-- =================================================================================================
-- XMTPS database schema (MySQL 8, InnoDB)
-- =================================================================================================
--
-- Creates the five tables XMTPS stores its state in (see "SQL backend" in README.md):
--
--   XFloat           one row per float: identifiers, gains/offsets, latest positions
--   XFloatMetadata   one row per float: deployment metadata from its .meta file
--   XFloatDive       one row per dive cycle
--   XGPS             one row per GPS fix of a dive
--   XFloatMail       one row per Iridium SBD mail message received (used for reprocessing)
--
-- Usage, into an existing, empty database (production: xmtps_alamo; see the sql.schema option):
--
--   mysql -u <user> -p <database> < sql/xmtps-schema.sql
--
-- or, to create the database too:
--
--   mysql -u <user> -p -e "CREATE DATABASE xmtps_alamo DEFAULT CHARACTER SET utf8mb4"
--   mysql -u <user> -p xmtps_alamo < sql/xmtps-schema.sql
--
-- The tables are created only if they do not exist, so running the script against an existing
-- database changes nothing.
--
-- XMTPS writes unset date/times as zero dates, so the server's sql_mode must not include
-- NO_ZERO_DATE or NO_ZERO_IN_DATE (MySQL 8 has them by default). Production uses:
--
--   sql-mode = "STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION"
--
-- Source: the production schema (mysqldump of xmtps_alamo, MySQL 8.4.11, 2026-09-08), with the
-- index on XFloatMail.iridium_position_datetime added on 2026-10-25 for the startup query that
-- selects the floats active in the last 90 days.
-- =================================================================================================

CREATE TABLE IF NOT EXISTS `XFloat` (
  `id_instrument` smallint NOT NULL,
  `argo_instrument_type` smallint unsigned DEFAULT '99',
  `id_dac` smallint NOT NULL DEFAULT '-1',
  `id_imei` bigint NOT NULL DEFAULT '-1',
  `id_wmo` int NOT NULL DEFAULT '-1',
  `gain_pressure` smallint unsigned NOT NULL DEFAULT '9999',
  `gain_pressure_surface` smallint unsigned NOT NULL DEFAULT '9999',
  `gain_salinity` smallint unsigned NOT NULL DEFAULT '9999',
  `gain_salinity_surface` smallint unsigned NOT NULL DEFAULT '9999',
  `gain_temperature` smallint unsigned NOT NULL DEFAULT '9999',
  `gain_temperature_surface` smallint unsigned NOT NULL DEFAULT '9999',
  `latest_gps_position_datetime` datetime NOT NULL DEFAULT '1400-01-01 00:00:00',
  `latest_gps_position_dive_number` smallint NOT NULL DEFAULT '-999',
  `latest_gps_position_latitude` double NOT NULL DEFAULT '99.99999',
  `latest_gps_position_longitude` double NOT NULL DEFAULT '999.99999',
  `latest_iridium_position_datetime` datetime NOT NULL DEFAULT '1400-01-01 00:00:00',
  `latest_iridium_position_cep_radius` int NOT NULL DEFAULT '-1',
  `latest_iridium_position_dive_number` smallint NOT NULL DEFAULT '-999',
  `latest_iridium_position_latitude` double NOT NULL DEFAULT '99.99999',
  `latest_iridium_position_longitude` double NOT NULL DEFAULT '999.99999',
  `latest_iridium_startup_position_datetime` datetime NOT NULL DEFAULT '1400-01-01 00:00:00',
  `latest_iridium_startup_sbd_momsn` int NOT NULL DEFAULT '-1',
  `latest_sbd_momsn` int NOT NULL DEFAULT '-1',
  `latest_sbd_mtmsn` int NOT NULL DEFAULT '-1',
  `offset_pressure` smallint unsigned DEFAULT '9999',
  `offset_pressure_surface` smallint unsigned DEFAULT '9999',
  `offset_salinity` smallint unsigned DEFAULT '9999',
  `offset_salinity_surface` smallint unsigned DEFAULT '999',
  `offset_temperature` smallint unsigned DEFAULT '9999',
  `offset_temperature_surface` smallint unsigned DEFAULT '9999',
  `netcdf_file_generation_is_enabled` bit(1) NOT NULL DEFAULT b'0',
  `phy_file_generation_is_enabled` bit(1) DEFAULT b'0',
  `start_of_mission_datetime` datetime NOT NULL DEFAULT '1400-01-01 00:00:00',
  `xmsg_version_major` smallint unsigned DEFAULT '255',
  `xmsg_version_minor` smallint unsigned DEFAULT '255',
  PRIMARY KEY (`id_instrument`),
  KEY `id_dac_idx` (`id_dac`),
  KEY `id_wmo_idx` (`id_wmo`),
  KEY `id_imei_idx` (`id_imei`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

CREATE TABLE IF NOT EXISTS `XFloatMetadata` (
  `id_dac` smallint NOT NULL DEFAULT '-1',
  `id_imei` bigint NOT NULL DEFAULT '-1',
  `id_instrument` smallint NOT NULL,
  `id_wmo` int NOT NULL DEFAULT '-1',
  `deployer` varchar(255) DEFAULT NULL,
  `deployer_address` varchar(255) DEFAULT NULL,
  `deployment_cruise_id` varchar(255) DEFAULT NULL,
  `deployment_datetime_status` varchar(45) DEFAULT NULL,
  `deployment_datetime` datetime DEFAULT '1400-01-01 00:00:00',
  `deployment_latitude` double NOT NULL DEFAULT '99.99999',
  `deployment_longitude` double NOT NULL DEFAULT '999.99999',
  `deployment_platform` varchar(255) DEFAULT NULL,
  `deployment_position_status` varchar(255) DEFAULT NULL,
  `deployment_position_valid` bit(1) NOT NULL DEFAULT b'0',
  `deployment_type` varchar(255) DEFAULT NULL,
  `manufacturer` varchar(45) DEFAULT NULL,
  `comment` text,
  `local_path` varchar(255) DEFAULT NULL,
  `principal_investigators` varchar(255) DEFAULT NULL,
  `principal_investigators_address` varchar(255) DEFAULT 'WHOI, Woods Hole, MA',
  `principal_investigators_originating_country` varchar(255) DEFAULT 'USA',
  `wmo_instrument_type` smallint NOT NULL DEFAULT '-1',
  `wmo_recorder_type` smallint NOT NULL DEFAULT '-1',
  PRIMARY KEY (`id_instrument`),
  CONSTRAINT `fk_XFloatMetadata_1` FOREIGN KEY (`id_instrument`) REFERENCES `XFloat` (`id_instrument`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

CREATE TABLE IF NOT EXISTS `XFloatDive` (
  `id_instrument` smallint NOT NULL,
  `id_dive` smallint NOT NULL,
  `id_dac` smallint NOT NULL DEFAULT '-1',
  `id_imei` bigint NOT NULL DEFAULT '-1',
  `id_wmo` int NOT NULL DEFAULT '-1',
  `complete` bit(1) NOT NULL DEFAULT b'0',
  `intermediate_listing_file_path` varchar(255) DEFAULT NULL,
  `iridium_position_datetime_first` datetime DEFAULT '1400-01-01 00:00:00',
  `iridium_position_datetime_last` datetime DEFAULT '1400-01-01 00:00:00',
  `num_sbd_msgs` smallint unsigned NOT NULL DEFAULT '0',
  `phy_file_path` varchar(255) DEFAULT NULL,
  `surface_pres_max` float DEFAULT '-9.999',
  `surface_pres_mean` float DEFAULT '-9.999',
  `surface_pres_min` float DEFAULT '-9.999',
  `surface_pres_valid` bit(1) DEFAULT b'0',
  `surface_pres_variance` float DEFAULT '-9.999',
  `pres_offs` float NOT NULL DEFAULT '0',
  `pres_offs_num_dives` smallint NOT NULL DEFAULT '-1',
  `pres_offs_num_samples` smallint NOT NULL DEFAULT '-1',
  `pres_offs_valid` bit(1) NOT NULL DEFAULT b'0',
  PRIMARY KEY (`id_instrument`,`id_dive`),
  CONSTRAINT `fk_XFloatDive_1` FOREIGN KEY (`id_instrument`) REFERENCES `XFloat` (`id_instrument`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

CREATE TABLE IF NOT EXISTS `XGPS` (
  `id_instrument` smallint NOT NULL,
  `id_dive` smallint NOT NULL,
  `datetime` datetime NOT NULL DEFAULT '1400-01-01 00:00:00',
  `id_type` smallint DEFAULT NULL,
  `hdop` float DEFAULT NULL,
  `num_satellites` smallint unsigned DEFAULT NULL,
  `snr_min` smallint unsigned DEFAULT NULL,
  `snr_mean` smallint unsigned DEFAULT NULL,
  `snr_max` smallint unsigned DEFAULT NULL,
  `time_to_fix` smallint unsigned DEFAULT NULL,
  `latitude` double NOT NULL DEFAULT '99.99999',
  `longitude` double NOT NULL DEFAULT '999.99999',
  `valid` bit(1) NOT NULL DEFAULT b'0',
  PRIMARY KEY (`id_instrument`,`id_dive`,`datetime`),
  KEY `id_instrument` (`id_instrument`),
  CONSTRAINT `fk_XGPS_1` FOREIGN KEY (`id_instrument`, `id_dive`) REFERENCES `XFloatDive` (`id_instrument`, `id_dive`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

CREATE TABLE IF NOT EXISTS `XFloatMail` (
  `id_dive` smallint NOT NULL DEFAULT '-99',
  `id_imei` bigint unsigned NOT NULL,
  `id_sbd_momsn` smallint NOT NULL DEFAULT '-1',
  `id_sbd_packet` int NOT NULL DEFAULT '-1',
  `id_msg` varchar(128) NOT NULL,
  `id_dac` smallint NOT NULL DEFAULT '-1',
  `id_instrument` smallint NOT NULL DEFAULT '-1',
  `id_sbd_mtmsn` int NOT NULL DEFAULT '-1',
  `id_msg_uid` int NOT NULL DEFAULT '-1',
  `id_wmo` int NOT NULL DEFAULT '-1',
  `iridium_position_cep_radius` int DEFAULT NULL,
  `iridium_position_datetime` datetime DEFAULT NULL,
  `iridium_position_is_valid` bit(1) NOT NULL DEFAULT b'0',
  `iridium_position_latitude` double DEFAULT NULL,
  `iridium_position_longitude` double DEFAULT NULL,
  `msg_body_text` varchar(512) DEFAULT NULL,
  `msg_body_is_valid` bit(1) NOT NULL DEFAULT b'0',
  `msg_header_datetime` datetime DEFAULT NULL,
  `msg_header_from` varchar(128) DEFAULT NULL,
  `msg_header_subject` varchar(128) DEFAULT NULL,
  `msg_header_is_valid` bit(1) NOT NULL DEFAULT b'0',
  `msg_header_raw_text` text,
  `msg_raw_text` text,
  `msg_is_valid` bit(1) NOT NULL DEFAULT b'0',
  `sbd_data` blob,
  `sbd_data_is_valid` bit(1) NOT NULL DEFAULT b'0',
  `sbd_file_local_path` varchar(256) DEFAULT NULL,
  `raw_file_local_path` varchar(256) DEFAULT NULL,
  `sbd_file_name` varchar(128) DEFAULT NULL,
  `sbd_file_size_bytes` int DEFAULT NULL,
  `sbd_session_status_code` int DEFAULT NULL,
  `sbd_session_status_string` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`id_dive`,`id_imei`,`id_sbd_momsn`,`id_sbd_packet`,`id_msg`),
  KEY `id_dac_idx` (`id_dac`),
  KEY `id_wmo_idx` (`id_wmo`),
  KEY `id_dive_idx` (`id_dive`),
  KEY `id_sbd_momsn_idx` (`id_sbd_momsn`),
  KEY `id_instrument` (`id_instrument`),
  KEY `iridium_position_datetime_idx` (`iridium_position_datetime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
