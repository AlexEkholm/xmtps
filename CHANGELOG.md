# Changelog

Changes to XMTPS, its Python tools and its documentation, newest first. See
[README.md](README.md) for how the current version works.

## 2026-10-27

Decoder daemon:

- An extended record message (0xFF) whose size field is under 6 bytes is rejected as too short;
  its record was taken to end before it started.

Python tools:

- Mission parameters page: phase-dependent parameters (drift and profile pressure, fall mode,
  drift duration, first descent pump) are shown for every mission phase (P0–P2), with statistics
  and colours per phase. They were shown for the reported phase `MPS` only, ignoring that a float
  moves to phase `MNS<MPS>` once its cycle count `MPC` reaches `MNM<MPS>`: the current phase now
  takes this into account and is marked bold and underlined, and phases the mission does not use
  are grey and left out of the comparisons.
- Startup page: the status is Started (not yet deployed) or Deployed, with the startup number
  (minus the dive number of the latest BIT; startup messages have negative dive numbers,
  decremented at each start) and, once deployed, the cycles. The startup time is the start time
  reported in the latest BIT (the first SBD time for BIT formats without times); `--days` and the
  sort order use it. Duration (the test duration, min) replaces BIT Time, and Pressure (the CTD
  pressure after calibration, `presPostCal`/`presPostReset`) replaces P Offset. The air and oil
  pump columns add `currAvg`, `currMax`, `12VCurrMax`, `batMax` and `powerMax` (BIT formats 9-11;
  format 8 reports only the maximum currents and battery voltage). The Dive column is removed,
  floats without BIT messages show "-", and the table heading is "Recent Startups". A BIT start
  time that cannot be parsed counts as missing instead of dropping the float from the page.
- Fill values (`xmtps-aggregate-json.py`, `RemoveFillValues`): the listing fill values no longer
  reach the plots or the aggregate JSON files. Once a float's dives are aggregated, and before
  anything is derived from them, a fill value becomes NaN in the columns where it cannot be a
  real value (the `FILL_VALUES` table):
  - pressure, temperature and salinity (time series, engineering `_dbar`/`_degc`/`_psu`):
    -9.999; ascent/descent rates: -9.99; vacuums (`vac0`/`vac1`, `_inhg`, pump `vac`): -9.99 and
    655.35; engineering counts: -99999;
  - other engineering values (e.g. `APRunT`, `AirBladMxP`, `seekPumpAdjT`, the BIT air and oil
    pump fields): 65535, 6553.5, 655.35, 65.535, 4294967295, -32768, -99999, -9.999 and -9.99;
    pump durations: 65535;
  - GPS SNR: 255; GPS and SBD CEP: 999999 (`IRI_POS_INVALID_CEP`);
  - wave spectra: frequency -99.0, `*_g` columns -1000 and -999.99999, the other spectra -9.99
    and -9.999.

  The same numbers are kept where they are real: in production, profile pressures of 655.35 dBar
  (1 dBar high-resolution profiles) and descent time offsets of 6553.5 and 65535 s are real
  samples. The mission table is not filtered: its 65535s mark an unset configuration, which the
  plots that use it already skip. The case vacuum plot now keeps a real 0.00 inHg (it dropped
  values of 0 or below).
- Float pages: the first pump duration plot is the first plot of the "Hydraulic System" section.
  It is generated separately from the case vacuum plot, so a failure in one no longer loses the
  other, and floats whose pump records have no dive numbers skip it.

Documentation:
- The changelog moved from `README.md` to `CHANGELOG.md`, and the README's Python tool
  descriptions are shorter.

## 2026-10-26

Decoder daemon (version 1.0):

- The program version is 1.0 (from 0.6); builds are numbered `1.0.<build>`.
- Faster shutdown: the mail processing, decoding and post-processing queues hold at most 128
  messages (they held 8,192 to 65,536), so fewer queued messages are processed before the daemon
  exits. The mail retrieval thread stops queueing messages once shutdown starts; mail files and
  inbox messages not yet queued are left for the next run, and reprocessing stops (the messages
  stay in the database). With 1,461 messages being reprocessed, shutdown took 8.5 s instead of
  29.9 s.
- The daemon no longer shuts down from inside the signal handler. The handler ran the whole
  shutdown (logging, stopping the threads, saving to MySQL) in whichever thread the signal
  interrupted; if that thread held a lock, such as the logger's, the handler deadlocked on it.
  On 2026-10-26 a `systemctl restart` during reprocessing hung that way: no "Caught SIGTERM" was
  logged, and systemd killed the daemon after `TimeoutStopSec` (300 s) without restarting it. The
  handler now only records the signal; the daemon thread sees it within 100 ms and shuts down
  from its main loop. A SIGABRT now ends the process at once instead of attempting a shutdown.
- A packet with a bad checksum or fewer than 8 bytes is logged and discarded; before, it
  terminated the daemon.
- The surface pressure offset counts dives with at least 5 surface pressure samples (it was 10).
  On 3,812 production dives, offsets from the first 5 samples were within 0.085 dBar of those
  from all samples for 95% of the 4-dive offsets; below 5, outliers are no longer rejected
  reliably. The limit of 10 had left the offset uncorrected for 27 RBR floats that send 8 or 9
  samples per dive.
- X-message 0xFF, logged as unknown and dropped until now (several hundred warnings a day), is
  decoded: its records are listed on `XFF` lines under "Extended Record Messages (0xFF)". The
  format is inferred from the 6,262 0xFF blobs in the production RAW files (spec C.6): a record
  type and version, then event records with a time and value (0xCA, likely surface pressure),
  controller reset records (0xD1), the float serial number with configuration bytes (0xAE, at
  startup), RBR CTD commands and replies (0xDB) and processor fault dumps (0xFA, ARM Cortex-M
  fault registers: a controller crash).
- Wave spectra: bin k has frequency k·fs/N, where bin 0 is the DC bin of the WIMS FFT; it was
  (k+1)·fs/N. WIMS firmware (v1.x and v2.x up to 2.01.00614b) copies FFT bins 0 to N/2-1 and
  labels them one bin high, and the decoder (`XWave.cpp`, `XWave.h` `FixFuckup9000()`) and
  `FloatClasses.Wave` inherited that label. For the Hurricane Erin floats (9302, 9303, 9308 and
  9309, which send every other bin) the error was 0.0195 Hz; shifting the spectra against NDBC
  buoy 41047 gives a best-match offset of +0.0225 Hz (per-pair median +0.0155 Hz). The
  displacement listing values (`wvm`) of the DC bin are the fill value, instead of a division
  by zero. Wave data processed before this change must be reprocessed.
- Wave spectra truncated to a range of frequencies (newer sensor firmware) are decoded without
  error: only the bins in each message are read. Before, each truncated message logged "wave
  sample index exceeds message size" (about 3,700 per 10 minutes while reprocessing); the bins
  were decoded correctly. Bins past N/2 are skipped with a warning instead of discarding the
  message.
- Listings: `CMD` lines list the command's parameter name in uppercase (`DFT 0 100`), as the
  mission parameter listings name them. The fall and rise time series (`f`, `r`) no longer list
  temperature and salinity columns: only the 9-byte packing format carries them, and only 9035
  and 9050 used it. The surface series (`sfc`) keeps them. `xmtps-list2json.py` reads the phase
  code of either layout.
- Build: `makefile.targets` makes the objects that print the build number and build date depend
  on `build/buildNo.txt`. They were only rebuilt when their sources changed, so `xmtps --version`
  and the file headers could show an older build (build 3318 reported 3312).

Database and operations:

- `XFloatMail` table, migrated in production on 2026-10-26 with `bin/migrate-xfloatmail.sh`
  (about 21 GB to about 8–9 GB):
  - The mail body text and raw header text are no longer stored (`msg_body_text`,
    `msg_header_raw_text`): they are only used while a mail is parsed, and both are part of
    `msg_raw_text`. The unused `id_dac` and `id_wmo` columns are dropped.
  - The primary key starts with `id_imei` instead of `id_dive`, so each float's messages are
    stored together instead of spread over the table. New indexes match the daemon's queries:
    `inst_dive_idx` (`id_instrument`, `id_dive`) for loading a dive and reprocessing a float,
    and `active_idx` (`iridium_position_datetime`, `msg_is_valid`, `id_instrument`) for the
    startup query of the floats active in the last 90 days, which it answers from the index
    alone (0.3 s instead of 2.0–2.4 s). The indexes they replace are dropped.
- `bin/move-mysql-datadir.sh` moves the MySQL data directory to `/data1`: the database (23 GB,
  almost all `XFloatMail`) had filled the 49 GB root filesystem to 94% (46% after the move).
- `xmtps-alamo.service`: `TimeoutStopSec=300` (systemd's default was 90 s).
- `bin/py/update-plots-service.sh` is updated to the version running on argus (run-time limit,
  run time logged). It passes the run time to the summary page (`--runtime`) and then writes the
  mission parameters and startup pages, creating `plots2/engineering/`.

Python tools:

- Float summary page footer, one item per line, for example: "Page last updated
  2026-10-26T17:40Z (105 minutes ago, runtime: 3 minutes)", "Python utility version 0.11",
  "Decoder status: active (uptime: 3 hours, 4 minutes)", "XMTPS version 1.0.3318b (built
  2026-10-26)". The runtime is that of the JSON and plot updates before the page is generated.
  The decoder's status and uptime come from `systemctl show xmtps-alamo` and its version from
  `xmtps --version`; the time since the update is filled in by the browser
  (`bin/py/PageStatus.py`). The individual float pages are unchanged.
- Mission parameters page (`bin/py/xmtps-generate-mission-page.py`, new), written to
  `plots2/engineering/params.html` (published at
  `http://argus.whoi.edu/etcetc/alamo/engineering/params.html`):
  - One table per float type (ALAMO, including WIMS and MM2 floats, and ALTO) with the firmware
    and the most recently reported mission parameters of every deployed float (with a profile)
    that has sent any: drift and profile pressures, timings, descent and ascent pumps, seek
    modes, surface pumps, ice detection, `PPE` and the WIMS timeout. Floats need not send their
    mission parameters every dive, so the page uses each float's latest dive that has them and
    shows which dive that is. A status column (ACTIVE, OVERDUE, ARCHIVED) is as on the summary
    page. `--days` can limit it to recently reporting floats.
  - A colour scale shows how abnormal each parameter value is within its group: how rare the
    value is compared with the most common one (first descent pump duration: its distance from
    the group median, white up to 25% and full colour from 200%). Values above the most common
    value go from white through yellow to red, values below it from white to blue, so values
    close to typical stay nearly white. Out-of-range values and drift or profile pressures
    deeper than the maximum pressure are marked dark red as errors. Unset values (sent before a
    mission is loaded) are shown as "unknown", without colour.
  - Each table's footer shows the group's most common values and their typical range (10th to
    90th percentile). Each table has a search field.
- Float startup page (`bin/py/xmtps-generate-startup-page.py`, new), written to
  `plots2/engineering/startup.html` (published at
  `http://argus.whoi.edu/etcetc/alamo/engineering/startup.html`): the floats that started up in
  the last 180 days, including those still in pre-deployment tests, with the results of their
  most recent built-in test (failed tests and subsystem error flags decoded by name, and the main
  measurements). The table has a search field, and the BIT firmware is shown as on the summary
  page (v10.12.0), with the firmware word and BIT format below.
- Air pump plots (`WriteAirPumpPlots`), in a new "Air Pump" section of each float page, for
  floats whose surface engineering data has air pump fields:
  - `<serial>_air_pump_duration`: the air pump run time at the surface (`APRunT`) of each dive,
    as bars. The hover text also gives the mission's air-assist duration setting (`SAO`); it is
    not drawn, since it is about ten times the run times (900 or 1080 s against 30–100 s).
  - `<serial>_air_pump_pres`: the maximum air bladder pressure of each dive (`AirBladMxP`, PSI),
    with the mission's air-assist minimum and maximum pressures (`SAN`, `SAX`) as dashed steps,
    carried forward from each mission message to the following dives. Unset configurations
    (`SAM` 255, sent before a mission is loaded) are skipped. No message reports a measured
    minimum air pressure; the minimum shown is the configured `SAN`.
  - Both also mark the air pump test of BIT messages (duration and maximum pressure).
- Engineering plots (`WriteEngineeringPlots`), in the "Hydraulic System" section:
  - `<serial>_phase_vac`: the case vacuum at the start (`vac0`) and end (`vac1`) of the descent,
    park/drift, ascent and surface phases (the `E07` to `E0A` engineering lines), with the eight
    phase points on the x axis ("Descent start", "Descent end", ... "Surface end"). Each dive's
    start and end of a phase are circle markers joined by a line, with a gap between phases;
    dives are colored from blue (first dives) to red, with the latest dive in black.
  - `<serial>_pump_first_duration`: the duration of each dive's first pump (the first `pmp`
    record, the first descent pump) as bars, with the mission's first descent pump duration
    (`DFT<phase>`, carried forward to the following dives) as a dashed step, and the descent and
    drift seek pump time adjustments (`seekPumpAdjT`) on a second y axis.

  The new plots by dive open at dive 0 (pre-mission dives are off the default x range but can be
  panned to), and their pressure and vacuum axes start at 0.
- `xmtps-list2json.py`: the first line of BIT formats 9-11 (`BITVer FWVer BITResults sensorType
  sensorTypeName`) was skipped, because its last column is listed as a comment (`# RBR`), so the
  BIT results mask, firmware and sensor type of every float with BIT formats 9-11 were missing
  from the JSON. An engineering line one value short of its header now takes the comment as its
  last value. Existing dive JSON files are only fixed when their listings are converted again
  (`-f`).
- Wave spectra (`FloatClasses.py`):
  - New low-frequency response correction coefficients, [0.09681, 0.05256, 0.01988, 0.03908],
    fitted against NDBC 41047 with the corrected bin frequencies (see Decoder daemon); see the
    WIMS user manual. With the corrected frequencies and new coefficients, `swh` is within 1% of
    the buoy on average (908 colocated pairs).
  - Wave directions (`theta1`, `theta2`, `mwd`) are corrected for the magnetic declination and
    referenced to true north. The declination is computed with the IGRF (package `ppigrf`) at
    the dive's surface GPS fix (idx 0 or 2, else the first valid fix) and stored as
    `declination`; `direction_ref` is `true`. Without `ppigrf` or a valid fix, the directions
    stay magnetic (`direction_ref` `magnetic`). The WIMS firmware is unchanged. `ppigrf` must be
    installed in the argus venv (`pip install ppigrf`). Validated against NDBC 41047 and the
    A-DWSD drifters: the correction reduced the mean direction bias from +25° to +13° (NDBC) and
    from +9° to -3° (A-DWSD); see the WIMS user manual.
  - The principal wave direction `theta2` was computed as atan2(a2, b2), which is not a
    direction: waves along north-south gave 270°. It is now the principal axis atan2(b2, -a2)/2,
    taken at the end of the axis (that angle or that angle + 180°) nearest `theta1`, in
    [0, 360). a2 and b2 alone cannot tell a direction from its opposite. Checked by running
    synthetic spectra for waves toward 0° to 345°, in 15° steps and with the WIMS firmware's
    cross-spectra sign conventions, through `Wave.PostProcess()`; `theta1` and `mwd` were
    already correct. With two opposing wave systems, or when `theta1` is poorly defined, the end
    of the axis chosen can be wrong.
  - Directions (`theta1`, `theta2`, `mwd`) are documented as toward which the waves propagate
    (not from); the wave plot titles say "(Toward)". Directions that round to 360° are now 0°.

## 2026-10-25

Decoder daemon:

- X-message packets are checked against their checksum (the two characters between `$` and
  `>`); a packet that fails is discarded and logged. All 1,351,052 packets in the production RAW
  files pass.
- The command echo-back message (0xDE), which floats send after a command upload, is decoded:
  each echoed command is listed on a `CMD` line under "Ground Station Upload Command Echo-Back
  Messages" (before, the section was printed empty).
- Mail server operations time out (`mail.timeout`, default 120 s without data): a stalled
  connection is closed and reopened at the next check instead of blocking the decoder, and a
  shutdown during a stall no longer waits for systemd to kill the daemon.
- The `PRC` line also lists the dive's uncorrected surface pressure minimum (outliers excluded),
  mean, maximum and standard deviation (`sfcPmin`, `sfcPmean`, `sfcPmax`, `sfcPstd`).
- Surface pressure offset correction: dives without a mission configuration message use the
  sensor type of the float's last one, so floats that send it only on some dives (most hurricane
  floats) are corrected on every dive even without a `.meta` file. The offset is also recomputed
  when a mission configuration message arrives after the dive's surface data.
- Decoder fixes found while writing the X-message specification:
  - Argo data (0xF0): the number of seeks (`nSeek` in the `A` lines) is read as two bytes; it was
    read as one, so it listed 0 instead of 1.
  - The `APS` acoustic lines list the output SNR in the SNROut column (it repeated SNRIn).
  - Mission configuration listings: the `DRT` line shows `DRT` (it showed `DRA`), and the `PPC0`
    and `PPC1` values are printed under their own labels (tau and alpha were swapped).
  - Surface engineering (0xEA) versions 0x13 and 0x14 read the number of raw profile samples
    before the number of empty bins, as all the format documents give them (they were swapped);
    the version 0x10 error list heading no longer fails to format.
  - Mission configuration and BIT messages too short for their layout are rejected instead of
    being read past their end; other size mismatches still only log a warning. The BIT 9/10 size
    warning gives the size it checks (256, not 260).
- Startup: the active-float query selects only `DISTINCT id_instrument` instead of whole
  `XFloatMail` rows (with their SBD data); it took 88 s instead of 248 s at the 2026-10-25
  restart, still as a table scan. `XFloatMail.iridium_position_datetime` has since been indexed,
  which lets the query read only the recent rows.
- Mail messages are no longer lost when the SQL server is unavailable. The mail processing
  thread writes a message's files and saves it before queueing it for decoding; if that fails
  and the SQL server cannot be reached (`SELECT 1`), the message is held and retried every
  10 s instead of being dropped (logged once when the outage starts and ends). The mail
  retrieval thread does not fetch while the SQL server is unavailable, so mail stays on the
  server, and with `mail.moveMessages` a message is moved out of the inbox only after it has
  been processed, not when it is fetched. Messages still unsaved at shutdown stay on the mail
  server and are fetched again at the next start.
- `XMfaMissionCfg::DecodeXMsgData` (6,504 lines) and `GenerateListingString` (5,584 lines) are
  split into one member function per firmware layout (`DecodeXMsgDataFw<version>`,
  `GenerateListingStringFw<version>`, plus `…Unsupported` and `GenerateListingStringIncomplete`);
  the two functions only dispatch on the firmware version. Decoding all 211,007 production
  mission messages (78,694 dives, 93 firmware versions) gives byte-identical listings before and
  after.

- Mission configuration: 25 more firmware versions are decoded instead of being reported as
  unsupported, mapped to existing layouts that were identified from production messages:
  10.2.3.1, 10.4.x, 10.6.0.11, 10.6.1.32 and 10.6.4.1 use the 10.5/10.6 layout; 10.7.0.1,
  10.7.1.4, 10.7.2.11, 10.8.0.1, 10.9.x (except 10.9.1.31), 10.10.0.x, 10.12.0.4, 10.12.0.34,
  10.13.0.13 and 10.14.0.28 the 10.7/10.8 layout; 10.7.1.31 and 10.9.1.31 the WIMS layout;
  7.3.4.0 the 7.2 layout. This covers 3,191 stored dives, including the active floats 11531,
  11533, 11041, 11498-11500, 11060, 11678, 11700-11712, 11773-11776 and the 9287-9306 wave
  floats. Checked on all 211,007 production mission messages: only those dives change, the
  calibration constants decode to their usual values, MSN equals the dive number, and the
  decoded drift target agrees with the measured drift pressure on 94% of 660 dives (the rest
  are float behaviour, e.g. 11678 holding a constant pressure above its target).
- Shutdown: saving the float cache no longer fails after a long uptime. The daemon thread's
  connection is idle from startup to shutdown and was dropped by the server's idle timeout, so
  every save failed (seen at the 2026-10-25 restart after 10.8 h); the connection is now checked
  and replaced first, and if the SQL server is down one warning is logged instead.
- Surface pressure offset correction: whether a float is corrected now comes from the sensor
  type in its mission configuration message (RBR corrected, SBE not), with the `.meta` file's WMO
  instrument type as the fallback; before, only the `.meta` file decided, so RBR floats without
  one (all but 11678) were not corrected. The offset is the mean of each dive's minimum surface
  pressure after outliers are discarded (it was the mean of the surface means), and the surface
  time series is no longer corrected in the listing. Only dives with at least 10 surface pressure
  samples count towards the offset, and the listing's Pressure Correction section notes whether
  the correction was applied and how to recover the raw pressures. Tested on 979 messages of
  11186 (RBR, now corrected by 0.12-0.20 dBar), 11498 (SBE, unchanged), 11678 (offset 0.42-0.52
  instead of 0.45-0.61 dBar) and 9293 (no sensor type, unchanged), and on all 2718 messages of
  9294 (238 of 246 dives corrected, by about the minimum surface pressure).

Build:

- The Release makefiles are regenerated from the Debug ones with `-O2 -g` (they referenced Boost
  sources that no longer exist and linked without the libraries). `XMfaMissionCfg.cpp` is built
  with `-O1`: GCC did not finish it at `-O2` within an hour.
- The build number is incremented with each commit (`build/hooks/pre-commit`) instead of with
  each build.

Python tools:

- The average surface pressure plot shows each dive's surface pressure offset correction (the
  listing's `PRC` line, when valid) as a blue dashed line. `xmtps-list2json.py` parses the `PRC`
  line into the dive JSON (`Surface.pres_correction`, ...) and `xmtps-aggregate-json.py` carries
  it into the aggregated surface table (`pres_corr`). Older dive JSON files simply have no
  correction until they are rewritten.
- The README links the published plot pages at argo.whoi.edu/alamo.
- Bathymetry in the trajectory plots is interpolated bilinearly between the ETOPO2 grid cells
  (it was the nearest cell, and the grid spacing taken from two float32 coordinates put it up to
  a third of a cell off). ETOPO2 is read from `/data1/etopo2/` by default; `--etopo2` is an
  alias of `-b/--bathymetry`.
- Docstrings for all modules, classes and functions of the Python tools (module docstrings with
  usage examples replace the editor templates; inaccurate ones corrected); no code changed.

Documentation:

- The README describes the listing file format (layout rules, every line type, and examples)
  and describes the SQL backend in general instead of listing its tables and columns.
- The README documents the JSON files written by the Python tools: the per-dive jsonpickle
  files, the aggregate tables, the float summary and `.lastprocessed`.
- `sql/xmtps-schema.sql` creates the database tables (the production schema, with the new
  `XFloatMail.iridium_position_datetime` index); it is referenced from the SQL backend section.
- The README has a table of contents.
- The README lists the versions of the compiler, libraries, MySQL server and Python packages
  production uses.
- New `spec/xmessage-format.md`: a full specification of the ALAMO/ALTO X-message format
  (packet framing, every message type and version, mission configuration layouts, error codes),
  compiled from the SOLO-II and ALAMO format documents, MRV's firmware definitions, the decoder
  source and production packets, with the discrepancies between them.
- Changelog entries for the versions before the git repository (2014-09 to 2026-07),
  reconstructed from the old repository and the source snapshots.

## 2026-10-24

Decoder daemon:

- Wave: the v2 spectral decoder no longer reads a sample past the last bin (the `;` terminator
  plus one byte beyond the message). For floats sending 51-sample packets (9287, 9291) this
  spurious bin failed the 0.3 g²/Hz check, so no wave products were produced; 128-sample packets
  decode as before.
- Wave: bin frequencies now include the packet's first-sample index, and `wvm` (m²/Hz) columns
  use the same order as `wvs` again (zz, yy, xx, xy, zx, zy), matching their header. Builds
  3234–3287 printed them as xx, xy, yy, zz under that header.
- New: dynamically corrected salinity (X-message 0xDC), sent by floats with dynamic CTD
  correction enabled (PPE = 2), is decoded as a second binned-profile salinity channel and listed
  as an extra `salnCorr` column on the `pro` lines. The format was inferred from production
  packets: a salinity message with four extra bytes (0x00, part index, 0x01, 0x00) after the size
  field.
- New: drift sample times (X-messages 0x78–0x7F) are decoded. For floats that send them, the
  drift section uses the fall/rise layout: `D SN dive nSam driftStart`, then `d` lines with the
  time (`+seconds` since the drift start, `(UTC/+sec)`) before pressure, temperature and
  salinity. The format (inferred) matches the drift pressure/temperature/salinity messages.
- New listing section "Profile Data (Binned) - Sensor Columns", before the binned profile, lists
  each profile column present for the dive as a `PRD`/`prd` table (column name and units, with
  the description after `#`, wrapped to keep lines under 100 characters).
- Pressures are listed with two decimal places (previously three) in every section; the fill
  value is still `-9.999`, and column widths are unchanged. The surface pressure variance keeps
  three decimals.
- Apart from pressure decimals, listings for floats that send neither 0xDC nor 0x78 are
  unchanged.
- Mission configuration message 1 for 10.8.0.4/10.12.0.x accepts 88 bytes (with CTD correction
  parameters) as well as 71.
- Fixed mission configuration offsets: `ddt` in five firmware layouts, `descend_start_time` for
  2.0.69–2.5.62, `isa` overlapping `isd` from 5.0.61 on, and `sao` read across the message
  terminator.
- Fixed BIT v8 temperature and air-valve calibration offsets, ice engineering test
  salinity/temperature range checks (salinity always read 2.000), surface engineering v0x10/v0x12
  bounds, and the XArgo v2.1 surface salinity offset.
- Surface pressure variance is now divided by gain² (it was 1/gain too large).
- Added length checks for short or malformed X-messages, bounded curvature sub-blocks, accepted
  phase code 15, and cleared auxiliary temperature data on dive reset.

Python tools:

- `xmtps-list2json.py` reads `pro` and `d` lines by the column names in their header, so it
  handles the new `salnCorr` column and the drift-times layout as well as the aux-temperature
  format. Drift sample times are the `D` line's drift start plus each `d` line's `time` offset.
- Measured drift sample times are used when a listing has them; otherwise drift times are still
  estimated between the end of descent and the start of ascent.
- Profiles carry `saln_corr` (dynamically corrected salinity) in the dive JSON and the aggregated
  profile table; it is NaN for floats that don't send it.
- Mission parameters: every `m` line is kept with its section, index, value and units
  (`mission.entries` in the dive JSON; the `#` description is not kept);
  `mission.parameters` keeps non-numeric values
  instead of failing on them. The `MIS` line also gives `cfgver`, and the firmware version is
  read correctly from older listings that have a minor index on that line.
- Engineering data: the Engineering classes in `FloatClasses.py` now define every parameter of
  every descent, drift, ascent, surface, ice and BIT message version as an explicit attribute
  named with its units (e.g. `drift.mean_pres_dbar`, `surface.prof_num_bins`,
  `bit.air_pump_current_max_a`). Each `E0x` line is parsed by its column header and mapped to
  these attributes through the `ENG_COLUMNS` table; columns not in the table go into
  `engineering.extra`, and the `e0x` error lines go into `engineering.errors`. Unreported
  parameters keep NaN, '' or an unset time. The old fixed-position E07–E0A parsing was removed.
- `xmtps-aggregate-json.py` also writes `<id>_mission.json` (one row per dive with mission
  parameters), `<id>_engineering_data.json` (one row per dive, `section.attribute` columns,
  dropping columns no dive reports) and `<id>_engineering_errors.json`.
- Trajectory plots show, behind the other traces and for dives 1 and later, the target drift
  pressure as a thin dashed light grey step line (`DTD<n>` for the current mission phase `MPS`, or
  `DTD`, changing at the start of descent of each dive; once a phase's cycle count is used up,
  `MPC` ≥ `MNM<n>`, the dive drifts in the next phase `MNS<n>`, since the reported phase is from
  before the switch) and the bathymetry at the GPS fixes as a thin black line ("Bathymetry
  (ETOPO2)" in the legend). Depth comes from NOAA ETOPO2v2c, interpolated bilinearly between the
  2′ grid cells and converted to pressure with Saunders (1981). The pressure axis range is
  unchanged, so a seafloor deeper than the float's data is off the plot. The file is read from
  `/data1/etopo2/ETOPO2v2c_f4.nc` by default (`-b/--bathymetry/--etopo2 FILE`); without it the
  bathymetry line is left out with a warning.
- `xmtps-aggregate-json.py` plotting is split into one function per plot group
  (`WriteGeoPlots` … `WriteWavePlots`, run in order from `PLOT_WRITERS`); the plot output is
  unchanged.
- Plot images are written through one persistent kaleido/Chromium browser instead of a new
  browser per image (2.2 s → 0.6 s per image): a full aggregate of 9213 takes 47 s instead of
  309 s, and of 11186 103 s instead of 382 s, with identical PNG and HTML output. The browser
  is started only when there are floats to plot; if it can't start, images are written one
  browser at a time as before.

Operations:

- Both systemd units restart after 30 s (`RestartSec=30`); the repository copies now match the
  installed units (decoder runs with `-v`; plot PID file name).

Documentation:

- Added this README, documented all command-line, configuration and Python tool options and the
  SQL schema, fixed stale Doxygen comments, and added the Doxygen PDF manual.

## 2026-10-23

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
- Metadata: launch times at hour or minute 0 are accepted and hour 24 is rejected, a bad field no
  longer stops parsing of the rest of the file, launch positions accept any whitespace, and DAC ID
  9999 placeholders are ignored.
- Added mission configuration support for firmware 10.12.1.31 (WIMS).
- The run-time PID lock is created exclusively and removed only by its owner.
- The post-processing thread keeps going when one float's KML or status file fails.
- A float's record is saved before its dives (fixes foreign-key failures for new floats), and the
  shutdown save continues past a failing float.
- Added ALTO WMO instrument types 873, 875 and 876.
- Fixed ice engineering (v0x11) messages being rejected whenever they reported error codes.
- Identical retransmitted SBD packets are no longer decoded twice.
- Stopped tracking the Debug/Release binaries; build number 3287.

Python tools:

- Updated to JSON version 0.11: descent/ascent phase codes in `xmtps-list2json.py`, shared logger
  setup, and a reworked summary page (elapsed time, page-update stamp, auto refresh).
- Fixed the RhoLUT maximum temperature (39 °C, was 34 °C); pressures below the table now use its
  minimum pressure instead of 0.
- Fixed drift fill values, a crash on 0 V pump records, and `.lastprocessed` being written before
  output succeeded.
- Wave directional coefficients now use the displacement formulation; `r1` and `r2` are distinct
  and lie in [0, 1].
- Fixed the drift σ₀ average plot title, wave plots not being linked when a float has one wave
  dive, and plot path handling.

Operations:

- The decoder service waits 30 s between restarts (`RestartSec=30`).

## 2026-08-11

- The post-processing thread closes its SQL connection after each pass, avoiding "lost connection
  to MySQL server" errors.

## 2026-08-06

- Added mission parameter support for firmware 0x0A0C0001 (10.12.0.1, RBR).

## 2026-08-05

- Initial commit.

## Before the git repository (2014-09 to 2026-07)

These entries were reconstructed on 2026-10-25. Two sources were used:

- **2021-01 and earlier:** the commit history of the old repository (`git.backup/edu.whoi.aekholm` on argus, 146 commits from 2014-09-08 to 2021-01-29, build 2975).
- **2021-03 to 2026-07:** the source snapshots on argus (`/data1/alamo/src/xmtps.backup*`) and in Dropbox. These are builds 2988, 3143, 3150, 3194, 3219, 3225, 3234, 3238, 3249 and 3254. Build 3254 is identical to the initial commit (build 3255).

Release dates are the build dates the deployed decoder wrote into the listings' `XSV` line. Changes between two snapshots are listed under the production builds that fall between them, because builds without a snapshot can't be told apart.

### 0.6.3252 (2026-07-01); source build 3254

- Build only: `boost::lock_guard`/`boost::recursive_mutex` replaced with the `std::` equivalents. No change to output.

### 0.6.3248 (2026-05-29) and 0.6.3249 debug (2026-06-24); source build 3249

- Decodes the marginal-ice-zone acoustic messages, whose type ID moved from 0xC0 to 0xCA (`XAcousticPosition.h`).
  - Subtype 3 is a listening window, 37 bytes: start and end UNIX times, with pressure, temperature and salinity at each end.
  - Subtype 5 is a CACST receive record, 33 bytes: counter, arrival time (s and µs), matched-filter power, source ID, PSK error code, and input and output SNR in 0.1 dB.
  - Each received ping is assigned to the listening window that contains it.
- New `APS` listing lines under "Position Data / Acoustic Position", with one block for listening windows and one for receive statistics.
  - The SNROut column repeats SNRIn, a bug that is still present.
  - Acoustic data no longer counts as position data for a dive.
- Mission configuration: firmware 0x0A0C0032 (ALAMO 10.12.0.32) is decoded with the 10.8.0.4 layout.
- Fall/rise listing header units read "(UTC/+sec)" instead of "(start-time/+sec)".

### 0.6.3235 debug (2025-10-08) and 0.6.3237 (2025-11-07); source build 3238

- Wave spectra: added a correction for firmware 0x0A060231, 0x0A080031, 0x0A090131 and 0x0A0A0031. These versions send only every other spectral value, with zeros in the second half. The correction doubles the effective sample rate and recomputes the frequencies of the first half.
- Wave frequency bins are back to `fs·(i+1)/seglen`.
- `.meta` files with 5-digit DAC IDs are accepted: the filename pattern became `[0-9]{4,5}_...` and the DAC ID limit rose from 10000 to 99999.
- Fixed an inverted condition: the start-of-mission filter on fetched messages was only applied when the start-of-mission time was invalid.

### 0.6.3233 (2025-09-05) and 0.6.3232 (2025-09-30); source build 3234

- Wave spectra rework (`XWave.cpp`, `XWaveSpectralSample.h`):
  - Version-2 spectral scale factors changed from {4096, 1024, 256, 64} to {8192, 2048, 512, 128}, the same as version 1. This halves `wvs`/`wvm` values compared with earlier builds.
  - Fixed the fill-value test in the `wvm` accessors, which all tested the xx component.
  - Frequency bins changed to `fs·i/seglen`, so the first bin was 0 Hz (reverted in 0.6.3237).
  - Only spectral versions 1 and 2 are listed.
- Regression: `wvm` columns were printed as xx, xy, yy, zz, zx, zy under the zz, yy, xx, xy, zx, zy header. Builds 3234 to 3289 have mislabelled `wvm` columns; fixed on 2026-10-24.

### 0.6.3222 (2025-02-14), 0.6.3224 (2025-07-15) and 0.6.3226 (2025-08-22); source build 3225

- BIT message version 11 is listed. Before this, v11 messages were decoded but listing them failed. Messages of 256 or 264 bytes are accepted; the extra 8 bytes are not decoded.
- Mission configuration: firmware 0x0A0A0031 (WIMS 10.10.0.31) is supported, alongside 0x0A080031.
- KML: the latest-position placemark description is in a `<p>` element, without the trailing instrument ID.

### 0.6.3219 (2024-09-25); source build 3219

- Mission configuration: firmware 0x0A070101 (10.7.1.1) is supported, with the 10.8 message-0 layout and an 88-byte message 1.
- 10.7.1.1 and 10.8.0.4 share a listing block. It no longer prints the BIT thresholds (PNB, PNC, PXC) that this layout dropped.

### 0.6.3217 (2024-09-11); source build 3194

- New `mail.fetchMode` option: 0 fetches from the IMAP server (the default), 1 reads mail files from the local filesystem.
  - In filesystem mode, `*.raw` and `*.txt` files are read from `dir.mail` under a PID lock file, parsed, and deleted once parsed.
  - New `dir.mail` and `dir.mail.processed` options. `dir.mail.processed` was not used yet.
- Stored mail files can be parsed back into messages (`XFloatMailMessage` `operator>>`).
- The `dir.*` options got defaults (`kml`, `lis`, `log`, `meta`, `raw`, `sbd`, `stat`, `phy`). The maximum fetch count went from 128 to 127.
- BIT message version 11 (firmware 10.8) is decoded.
  - It adds a 32-bit traveler error field at offset 50, which moves all later fields.
  - The CTD status is now read as a 32-bit integer in BIT 9 and 10.
- Mission configuration: firmware 0x0A080004 (10.8.0.4, SBE) and 0x0A080031 (10.8.0.31, WIMS) are supported.
  - New layout: message 0 is 327 bytes, without PNB/PNC/PXC.
  - Message 1 is 71 or 90 bytes, with the WIMS timeout (`WST`) and the dynamic salinity correction parameters.
- Known problems at this build:
  - The 10.8.0.4 parser read beyond the 71-byte message 1.
  - BIT v11 messages could not be listed.
  - The dynamic salinity correction lines were commented out of the 10.5 to 10.6.3 listings.

### 0.6.3193 (2024-09-03); source build 3150

- Mission configuration: firmware 0x0A060231 (10.6.2.31, WIMS) is supported.
  - Its message 1 is 95 bytes, with a WIMS timeout field.
  - Its listing has a "Surface - Wave-Field Measurement" section (`WST`) and the dynamic salinity correction parameters.
- Firmware 0x0A060233, 0x0A060300 to 0x0A060304 and 0x0A060333 are accepted with the 10.6.2 layout.

### 0.6.3006 debug (2021-03-29) to 0.6.3149 (2023-08-23); source build 3143

The deployed releases in this period were:

| Release | Build date |
|---|---|
| 0.6.3006 (debug) | 2021-03-29 |
| 0.6.3008 to 0.6.3011 | 2021-04-09 to 2021-04-14 |
| 0.6.3048 | 2021-07-13 |
| 0.6.3061 | 2021-07-28 |
| 0.6.3064 | 2021-11-22 |
| 0.6.3067 | 2022-01-04 |
| 0.6.3071 | 2022-07-06 |
| 0.6.3078 | 2022-10-04 |
| 0.6.3081 | 2023-01-26 |
| 0.6.3139 | 2023-05-05 |
| 0.6.3142 | 2023-06-22 |
| 0.6.3149 | 2023-08-23 |

Together they made these changes:

- **Mission configuration** (`XMfaMissionCfg.cpp`):
  - New firmware support:
    - 0x08010001 (2021 hurricane floats) and 0x0A020201, with the 0x0800xxxx layout.
    - 0x08010000, 0x08020000, 0x09000000, 0x0A000000, 0x0A000100, 0x0A020000 to 0x0A020200, 0x0A030000 and 0x0A030100, with a 76-byte message 1.
    - 10.5.0.0 to 10.5.0.4, 10.6.0.0 to 10.6.0.4, 10.6.1.0 to 10.6.1.4, 10.6.1.33 (Micro-ALTO) and 10.6.2.0 to 10.6.2.4, with a 93-byte message 1.
  - New "Profile - Dynamic Salinity Correction" listing section, with `PPE` and `PPC0` to `PPC3`. The tau and alpha values were printed against each other's labels.
- **Firmware version tracking:** only mission configuration message 0 updates a float's firmware version. Before, any message did, because of an always-true comparison.
- **BIT version 2 decoder** (`XEngBIT.h`, `XAlamoBit2.h`):
  - BIT versions 6 to 8 use the old decoder (version 6 newly accepted).
  - Versions 9 (firmware 10.5) and 10 (10.6) use a new 256-byte decoder with IEEE float fields and UNIX times.
  - `E05`/`E06` lines got a `BITVer` column, and serial numbers are no longer zero-padded.
- **Wave message:**
  - Byte 3 is read as the message version.
  - Version 2 adds an accelerometer gain at offset 14, and the effective scale is `assf·gain²/64`.
  - The `WVS` header shows the version, gain and effective scale.
  - `wvs` values are scaled.
  - New `wvm` lines give displacement spectra in m²/Hz.
- **`.meta` files:** serial numbers of 4 to 6 digits are accepted in the filename, and the DAC and WMO IDs are copied into the float record.
- **Sensor data:** values left over after curvature unpacking log a warning instead of failing the message.
- **Reprocessing and threads:**
  - Reprocessing at startup covers floats heard from in the last 90 days, down from 180.
  - The daemon thread spin-up delay dropped from 30 s to 10 s.
- **Options:**
  - `dir.nc`, `gen.nc` and `poc.nc` were removed.
  - The default `mail.user` is `alamo`.
  - Help text was added for several options.
- **Repository additions:**
  - The systemd units `xmtps-alamo.service` and `xmtps-alamo-plot.service`.
  - The Python plotting tools in `bin/py`.
  - Build notes for Ubuntu 20.04 and g++-10.

### Build 2988 (2021-03); Dropbox snapshot

- Ported from `boost::shared_ptr` to `std::shared_ptr` throughout, and to the VMime 0.9.2 API.
- Removed NetCDF output: the NetCDF writer thread and `NetcdfUtils` were deleted, and `-lnetcdf_c++4` was dropped from the link.
- Built with the system `g++` instead of `g++-4.7`. Sample configuration in `cfg/options.cfg`.

### 0.6.2971 to 0.6.2975 (2020-08-10 to 2021-01-29)

- Mission parameter support for firmware 0x08000101 (2020-08-10), 0x08000300 (2020-11-23) and 8.0.5 (2021-01-29).
- Fixed the listing not being updated when wave spectra were the last message received in real time (2020-09-01).

### 0.6.2923 to 0.6.2962 (2020-01-15 to 2020-07-29)

- Firmware 0x08000001 (2020-07-24) added directional, spectral wave data. Wave decoding fixes followed on 2020-07-29.
- Mission parameter support for firmware 0x07020200, 0x07020400, 0x07020800, 0x07020900 and 0x07030500 (2020-03 to 2020-06).
- RAW file names changed (2020-01-15) so that messages with duplicate MOMSNs are not lost. Fixed RAW file names for incomplete messages (2020-03-31).

### 0.6.2869 to 0.6.2910 (2019-08-13 to 2019-09-17)

- Wave data decoding, and the hex dump of each message at the end of the listing (2019-08-13).
- Fixed mission configuration decoding for firmware 6.0.x, 6.4.9 and 6.5.w.
- Added firmware 0x06057700 and 0x06010300 (2019 ALTO RBR/O2 test float).
- Fixed the surface engineering message v0x14 and mission message 0xD1 for 0x06050100.

### 0.6.2803 to 0.6.2805 (2018-10-31 to 2019-06-28)

- Support for the firmware 6.0 message changes (2018-10-29 to 2018-10-31):
  - mission parameters, ascent engineering and Argo data;
  - the drift engineering message;
  - firmware 0x06000100 (2019-03-19) and 0x06050100 (2019-05-30).
- Fixed the "start-of-drift" label in the drift engineering listing (2019-03-29).
- PHY files:
  - support WMO instrument type 868 (ALAMO with an SBE CTD);
  - samples are ordered by pressure (2019-06).
- Partial spectral wave decoding (2019-06-21).

### 0.6.2764 to 0.6.2775 (2017-10-03 to 2018-05-23)

- MFA mission configuration decoding (2017-10-16):
  - phase-specific fall-to-profile parameters;
  - fixes for firmware 0x05006100;
  - an updated listing format.
- Fixed surface engineering error code decoding (2017-11-03).
- Ice engineering message v0x11 (2018-05-16).
- Active floats are reprocessed at startup by default (2018-05-23). Active means a valid SBD message in the last 180 days.

### 0.6.2592 to 0.6.2633 (2017-03-03 to 2017-08-17)

- **Mission configuration support:**
  - ALAMO software 0x04016100 (2016 NASCar floats), 0x04056300 and 0x04066100;
  - 0x04046100 (2017 hurricane floats);
  - fixed decoding for 0x03006500.
- **Surface pressure offset correction:**
  - fixed bad offsets caused by spurious surface pressures;
  - the correction is no longer computed for SBE floats (2017-04-21).
- **Surface data:** surface engineering message v0x13 support, and fixes to the derived surface fields when a dive has several surface messages.
- **Listing format:**
  - Auxiliary temperature channel data is part of the profile section.
  - IMEIs in listings are truncated for security (2017-08-02).
  - "counts" fields were removed from the composite sections.
  - Unbinned profile lines became `pru`.
- **PHY files:** written for ALAMO floats with salinity, excluding binned samples above 1 dBar (2017-08).
- **NetCDF:**
  - JULD is taken from the start-of-ascent time;
  - fixes to the writer thread;
  - unbinned profiles were left out for now.
- **GPS and KML:** GPS messages with future dates are rejected, and KML files link to argo.whoi.edu.
- **Configuration:** the valid DAC ID range changed, and per-instrument log files were disabled.

### 0.6.1555 to 0.6.2457 (2016-02-14 to 2016-12-29)

- **Mail and SQL server connections:** better error handling (2016-02-14).
- **New messages for firmware 0x02006900** (2016-03-24): engineering surface v0x12, valve data, mission configuration, and ice detection v0x10.
- **New output:** NetCDF files (2016-04-11), and options to enable or disable PHY files per instrument (2016-06-01).
- **Surface pressure offset correction** (2016-06-23 to 06-27): computed from the surface pressures of the previous N dives, and applied to the listing, PHY and NetCDF files.
- **Fixes:**
  - curvature unpacking for packing factors above 3;
  - missing data in some listings.
- **Valid instrument IDs:** 1000 to 10999.
- **Mail options** (2016-08):
  - move processed messages out of the inbox folder;
  - the starting message index;
  - more robust mail retrieval;
  - unsupported `.meta` fields are handled.
- **Master KML:** active (last 30 days) and archived floats in separate folders.
- **Mission parameter messages:**
  - firmware 0x03016700 (Solo-II hybrid test float 10083) and 0x0302100 (2016 NASCar floats);
  - reprocessing no longer fetches mail from before the start of the mission.

### 0.6.1451 (2015-09-22) and earlier (2014-09-08 to 2015-09-22)

- **2014-09:** first versions on argus.
  - The signal handler gives a graceful shutdown.
  - The init.d script.
  - Zero decoder threads can run, to fetch mail only.
  - Listing, pump-data, PHY and gain/offset fixes; version 0.6 (2014-10-20).
- **2015-01 to 2015-03:** X-message format 2.2.
  - RAW mail files in standard or extended format.
  - The Argo data message.
  - The 9-byte surface time series with temperature and salinity.
  - Curvature packing for profile and drift data.
  - Reprocessing of one instrument by ID (2015-05-11).
- **2015-06 to 2015-07:** ALAMO messages.
  - BIT v7.
  - Engineering descent, drift and ascent v0x11.
  - Engineering surface (0xEA).
  - Mission configuration for software 0x02002300.
  - The new pump data format.
  - A FIFO of RAW file paths for the S2A decoder.
  - Reworked sensor data decoding for curvature-packed messages of differing lengths.
- **2015-07 to 2015-09:** output and fixes.
  - Fixed swapped drift-engineering temperature and salinity, and the descent first pump-time adjustment.
  - KML (2015-08-11) and status files (2015-08-21).
  - Stored messages are reprocessed when a dive is loaded from the database.
  - The start-of-dive GPS fix can stand in for the previous dive's profile position.
  - Firmware 0x02006700 and 0x02006800.
