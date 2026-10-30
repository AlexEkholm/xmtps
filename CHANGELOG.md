# Changelog

Changes to XMTPS, its Python tools and its documentation, newest first. See
[README.md](README.md) for how the current version works.

## 2026-10-30

Documentation:

- The X-message specification is also given as plain text, JSON, YAML and PDF
  (`spec/xmessage-format.{txt,json,yaml,tex,pdf}`). They are generated from
  `spec/xmessage-format.md` by `spec/build-formats.py`, which needs only the Python standard
  library, plus XeLaTeX for the PDF. The JSON and YAML hold the document tree: nested sections
  with number, title and id, and paragraph, list, table, code and block-quote blocks; text keeps
  its inline markdown. Two malformed table rows in the markdown were fixed: 0xF0 offset 34 had an
  extra cell, and the 0xC5 parameter-list row had unescaped `|` inside code.

Python tools:

- Trajectory plots: the bathymetry line comes from ETOPO 2022 (60 arc-second surface elevation,
  `/data1/etopo2022/`) instead of ETOPO2v2c (2 arc-minute), labelled "Bathymetry (ETOPO 2022)".
  The reader takes either grid's coordinate names (`lon`/`lat` or `x`/`y`); `--etopo` is a new
  alias of `-b/--bathymetry`.

## 2026-10-29

Python tools:

- WIMS wave spectra (`Wave.PostProcess`): the low-frequency response correction coefficients are
  [f0, w] = [0.01445, 0.10457] Hz, the fit of WIMS user manual rev. 3.15, made without floats 9298
  and 9310 (was [0.03189, 0.07842] Hz, fitted with them). The taper now reaches 1 at 0.119 Hz
  instead of 0.110 Hz and is larger below about 0.09 Hz: 0.05 instead of 0 at 0.030 Hz, 0.26 instead of
  0.13 at 0.050 Hz.
- `xmtps-aggregate-json.py`: listing fill values (e.g. pressure -9.999, rate -9.99, `cep_km`
  999999) are removed again under pandas 3. pandas 3 no longer ignores empty frames when it
  chooses the dtypes of a concatenation, so every aggregate column appended to its empty initial
  frame became `object` and `RemoveFillValues` skipped it; rows are now appended with
  `AppendRows`, which keeps the dive data's dtypes. Production (pandas 3.0.3) has been writing the
  fill values into the aggregate JSON files and plots.
- Python 3.14 and current packages (pandas 3.1, plotly 7.1, kaleido 1.5, jsonpickle 4.1.4): the
  tools run without warnings. Files are opened with an explicit UTF-8 encoding; jsonpickle is
  called with `keys=True` (the jsonpickle 5 default; output is unchanged because all keys are
  strings); `inplace=True` sort and reset calls are replaced by assignments; the
  `mode.chained_assignment` option is set only under pandas 2. With the same inputs, 3.12 (pandas
  2.3) and 3.14 (pandas 3.1) write identical JSON and the same plots.

## 2026-10-29

Decoder daemon:

- WIMS configuration parameter messages (0xC5) are decoded (`XWimsParameters`) and listed as one
  `WIP` line per parameter in a new "WIMS Configuration Parameters (0xC5)" section. A WIMS float
  sends one in dive 0 and after a `WCG -1` upload: a version (1: 42 parameters, 2: 43), a part
  index, and one 16-bit value per parameter in the float's `WCG`/`WCS` index order. The format and
  most names are inferred from the 54 messages in production (spec C.7); before, 0xC5 was logged
  as an unknown X message.

## 2026-10-28

Decoder daemon:

- WIMS wave spectra (`XWave`): the effective spectral sensitivity scale factor uses the Tukey
  window mean square and the true effective sample rate (WIMS user manual rev. 3.15), and is
  listed with one decimal on the `WVS` line. Bins that were not received hold the fill value
  -999.99999.
- Whether a float sends every other spectral bin is resolved from the dive's or float's firmware
  version, or from the spectra when the version is unknown, independent of message order.

Python tools:

- WIMS wave spectra (`Wave.PostProcess`): the noise-floor mask (bins where Szz is below
  0.0005 g^2/Hz) and the low-frequency cosine taper are applied to all six spectra, not to Szz
  only. Applied to Szz alone, they inflated r1 (bin values up to about 18 in simulated seas) and
  the (Sxx + Syy) / Szz ratio wherever the taper is below 1 or Szz was zeroed. Bin r1 is now at
  most 0.99 and the ratio averages 1.00 in simulated deep-water seas. `swh` and the periods are
  unchanged; `mwd` changes by up to 2°. Sxx, Syy, and Cxy (displacement) are now tapered and
  masked too; the acceleration spectra (`*_g`) are unchanged.
- WIMS wave spectra: the low-frequency response correction is a cosine taper with
  [f0, w] = [0.03189, 0.07842] Hz, fitted to the Hurricane Erin and Lowell intercomparisons.
  `xmtps-list2json.py` reads the fill value of bins not received as NaN. A dive without a valid
  position uses the magnetic declination at the float's most recent valid fix after deployment.

## 2026-10-27

Decoder daemon:

- An extended record message (0xFF) with a size field under 6 bytes is rejected as too short.
- The composite Iridium position (the listing's "Iridium Position" section, `IRI` line, and
  `XCompositeIridiumPosition`) is removed. Its listed uncertainty was not calibrated (against GPS
  the error was within its `stdError` in 7% of dives); the Python tools compute their own,
  GPS-calibrated composite from the `SBD` lines. `xmtps-list2json.py` still skips `IRI` lines in
  older listings.

Python tools:

- Composite Iridium positions: `cep_km` adds a GPS-calibrated error term,
  `sqrt(1 / sum(CEP^-2) + 1.55^2)` (`CompositePosition.BIAS_CEP_KM`). Without it the CEP was
  overconfident: against GPS the error (about 2 km whatever the number of positions) was within
  it in 25% of composites instead of 50%. Composites fill the profile position of a dive without
  a GPS fix up to a CEP of 2 km (`CPOS_MAX_CEP_KM`, was 1 km), about half of those dives.

- Mission parameters page: phase-dependent parameters (drift and profile pressure, fall mode,
  drift duration, first descent pump) are shown per mission phase (P0–P2), with per-phase
  statistics and colors. They were shown only for the reported phase `MPS`, ignoring the switch
  to `MNS<MPS>` once `MPC` reaches `MNM<MPS>`. The current phase is bold and underlined; unused
  phases are gray and not compared.
- Startup page: status is Started or Deployed, with the startup number and, once deployed, the
  cycles. The startup time is the latest BIT's start time (else the first SBD time), used by
  `--days` and the sort order. Duration replaces BIT Time, Pressure
  (`presPostCal`/`presPostReset`) replaces P Offset, and the air and oil pump columns add
  `currAvg`, `currMax`, `12VCurrMax`, `batMax` and `powerMax` (BIT formats 9-11). The Dive column
  is removed, floats without BIT messages show "-", the heading is "Recent Startups", and an
  unparseable BIT start time no longer drops the float.
- Fill values (`xmtps-aggregate-json.py`, `RemoveFillValues`): listing fill values become NaN,
  per the `FILL_VALUES` table, in columns where they cannot be real, so they no longer reach the
  plots or aggregate JSON. Values that can be real there (e.g. 655.35 dBar profile pressures) are
  kept, and the mission table is not filtered. The case vacuum plot keeps a real 0.00 inHg.
- Float pages: the first pump duration plot opens the "Hydraulic System" section. It is
  generated separately from the case vacuum plot, so a failure in one no longer loses the other;
  floats whose pump records have no dive numbers skip it.

Documentation:
- The changelog moved from `README.md` to `CHANGELOG.md`; the README's Python tool descriptions
  are shorter.

## 2026-10-26

Decoder daemon (version 1.0):

- The program version is 1.0 (from 0.6); builds are numbered `1.0.<build>`.
- Faster shutdown: the mail processing, decoding and post-processing queues hold at most 128
  messages, and once shutdown starts, mail retrieval stops queueing (the rest is left for the
  next run) and reprocessing stops.
- The signal handler only records the signal, and the daemon thread shuts down from its main
  loop; shutting down inside the handler could deadlock (a `systemctl restart` hung on
  2026-10-26). A SIGABRT ends the process at once.
- A packet with a bad checksum or under 8 bytes is logged and discarded instead of terminating
  the daemon.
- The surface pressure offset counts dives with at least 5 surface pressure samples (was 10,
  which left RBR floats sending 8 or 9 uncorrected).
- X-message 0xFF, until now logged as unknown and dropped, is decoded and listed on `XFF` lines
  under "Extended Record Messages (0xFF)". Its format is inferred (spec C.6): event (0xCA),
  controller reset (0xD1), serial number and configuration (0xAE), RBR CTD command (0xDB) and
  processor fault (0xFA) records.
- Wave spectra: bin k has frequency k·fs/N (bin 0 is DC); it was (k+1)·fs/N, a mislabeling from
  the WIMS firmware (v1.x and v2.x up to 2.01.00614b) carried into `XWave.cpp`, `XWave.h`
  `FixFuckup9000()` and `FloatClasses.Wave`. The DC bin's `wvm` values are the fill value instead
  of a division by zero. Wave data processed earlier must be reprocessed.
- Wave spectra truncated to a frequency range (newer sensor firmware) no longer log "wave sample
  index exceeds message size". Bins past N/2 are skipped with a warning instead of discarding the
  message.
- Listings: `CMD` lines give the parameter name in uppercase (`DFT 0 100`). The fall and rise
  series (`f`, `r`) drop the temperature and salinity columns (sent only by 9035 and 9050); `sfc`
  keeps them. `xmtps-list2json.py` reads either layout.
- Build: `makefile.targets` makes the build number and date objects depend on
  `build/buildNo.txt`; `xmtps --version` and file headers could show an older build.

Database and operations:

- `XFloatMail`, migrated in production on 2026-10-26:
  `msg_body_text` and `msg_header_raw_text` (both in `msg_raw_text`) are no longer stored, and
  `id_dac` and `id_wmo` are dropped. The primary key starts with `id_imei` instead of `id_dive`,
  and the new indexes `inst_dive_idx` (`id_instrument`, `id_dive`) and `active_idx`
  (`iridium_position_datetime`, `msg_is_valid`, `id_instrument`) replace the old ones.
- The MySQL data directory moved to `/data1`; the database had
  filled the root filesystem.
- `xmtps-alamo.service`: `TimeoutStopSec=300` (was systemd's default, 90 s).
- `bin/py/update-plots-service.sh` matches the version on argus (run-time limit, run time
  logged), passes the run time to the summary page (`--runtime`), and writes the mission
  parameters and startup pages, creating `plots2/engineering/`.

Python tools:

- Float summary page footer, one item per line: update time and age, runtime, Python utility
  version, decoder status and uptime (`systemctl show xmtps-alamo`) and XMTPS version
  (`xmtps --version`), via `bin/py/PageStatus.py`.
- Mission parameters page (`bin/py/xmtps-generate-mission-page.py`, new), written to
  `plots2/engineering/params.html` (published at
  `http://argus.whoi.edu/etcetc/alamo/engineering/params.html`): one table per float type (ALAMO,
  including WIMS and MM2, and ALTO) with each deployed float's firmware, status and latest
  reported mission parameters. A color scale marks values unusual for their group, errors are
  dark red and unset values "unknown"; table footers give the group's typical values, and each
  table has a search field. `--days` limits it to recently reporting floats.
- Float startup page (`bin/py/xmtps-generate-startup-page.py`, new), written to
  `plots2/engineering/startup.html` (published at
  `http://argus.whoi.edu/etcetc/alamo/engineering/startup.html`): floats started up in the last
  180 days with their latest built-in test results (failed tests and error flags by name, main
  measurements) and BIT firmware, with a search field.
- Air pump plots (`WriteAirPumpPlots`), in a new "Air Pump" section: `<serial>_air_pump_duration`
  (`APRunT` per dive, `SAO` in the hover text) and `<serial>_air_pump_pres` (`AirBladMxP`, with
  `SAN` and `SAX` as dashed steps; `SAM` 255 is skipped). Both mark the BIT air pump test.
- Engineering plots (`WriteEngineeringPlots`), in "Hydraulic System": `<serial>_phase_vac` (case
  vacuum `vac0`/`vac1` at the start and end of each phase, from `E07` to `E0A`) and
  `<serial>_pump_first_duration` (first descent pump duration, `DFT<phase>` as a dashed step,
  `seekPumpAdjT` on a second axis). The new plots by dive open at dive 0, and their pressure and
  vacuum axes start at 0.
- `xmtps-list2json.py`: the first line of BIT formats 9-11 (`BITVer FWVer BITResults sensorType
  sensorTypeName`) was skipped because of its trailing comment (`# RBR`), losing the BIT results
  mask, firmware and sensor type. An engineering line one value short of its header now takes the
  comment as its last value. Existing dive JSON files need converting again (`-f`).
- Wave spectra (`FloatClasses.py`):
  - New low-frequency response correction coefficients, [0.09681, 0.05256, 0.01988, 0.03908],
    for the corrected bin frequencies (see the WIMS user manual).
  - Wave directions (`theta1`, `theta2`, `mwd`) are corrected for magnetic declination (IGRF via
    `ppigrf`, at the surface GPS fix, stored as `declination`) and referenced to true north
    (`direction_ref` `true`); without `ppigrf` or a fix they stay magnetic. Install `ppigrf` in
    the argus venv (`pip install ppigrf`).
  - `theta2` was atan2(a2, b2), not a direction; it is now the principal axis atan2(b2, -a2)/2,
    at the end nearest `theta1`, in [0, 360). With opposing wave systems or a poorly defined
    `theta1` the end can be wrong.
  - Directions are documented as toward which waves propagate; plot titles say "(Toward)".
    Directions that round to 360° are 0°.

## 2026-10-25

Decoder daemon:

- X-message packets are checked against their checksum (the two characters between `$` and
  `>`); a failing packet is logged and discarded.
- The command echo-back message (0xDE) is decoded and listed on `CMD` lines under "Ground
  Station Upload Command Echo-Back Messages" (the section was empty).
- Mail server operations time out (`mail.timeout`, default 120 s): a stalled connection is
  reopened at the next check instead of blocking the decoder or shutdown.
- The `PRC` line also lists the uncorrected surface pressure statistics (`sfcPmin`, `sfcPmean`,
  `sfcPmax`, `sfcPstd`).
- Surface pressure offset correction: dives without a mission configuration message use the
  sensor type of the float's last one, so floats sending it only on some dives are corrected on
  every dive. The offset is recomputed when a mission configuration message arrives late.
- Decoder fixes found while writing the X-message specification:
  - Argo data (0xF0): `nSeek` in the `A` lines is read as two bytes, not one.
  - `APS` lines list the output SNR in SNROut (it repeated SNRIn).
  - Mission configuration listings: the `DRT` line shows `DRT` (not `DRA`), and `PPC0`/`PPC1`
    are under their own labels (tau and alpha were swapped).
  - Surface engineering (0xEA) v0x13 and v0x14 read the raw profile sample count and empty bin
    count in the right order; the v0x10 error list heading formats correctly.
  - Mission configuration and BIT messages too short for their layout are rejected instead of
    read past their end. The BIT 9/10 size warning gives 256, not 260.
- Startup: the active-float query selects only `DISTINCT id_instrument` instead of whole
  `XFloatMail` rows; `XFloatMail.iridium_position_datetime` is now indexed.
- Mail is no longer lost when the SQL server is unavailable: an unsaved message is retried every
  10 s, mail is not fetched during the outage, and with `mail.moveMessages` a message leaves the
  inbox only once processed. Messages unsaved at shutdown are fetched again at the next start.
- `XMfaMissionCfg::DecodeXMsgData` and `GenerateListingString` are split into one function per
  firmware layout (`DecodeXMsgDataFw<version>`, `GenerateListingStringFw<version>`, plus
  `…Unsupported` and `GenerateListingStringIncomplete`); listings are unchanged.

- Mission configuration: 25 more firmware versions are decoded with existing layouts instead of
  reported as unsupported: 10.2.3.1, 10.4.x, 10.6.0.11, 10.6.1.32 and 10.6.4.1 (10.5/10.6
  layout); 10.7.0.1, 10.7.1.4, 10.7.2.11, 10.8.0.1, 10.9.x (except 10.9.1.31), 10.10.0.x,
  10.12.0.4, 10.12.0.34, 10.13.0.13 and 10.14.0.28 (10.7/10.8); 10.7.1.31 and 10.9.1.31 (WIMS);
  7.3.4.0 (7.2). Affected active floats include 11531, 11533, 11041, 11498-11500, 11060, 11678,
  11700-11712, 11773-11776 and the 9287-9306 wave floats.
- Shutdown: saving the float cache no longer fails after a long uptime; the daemon thread's idle
  connection, dropped by the server's timeout, is checked and replaced first.
- Surface pressure offset correction: whether a float is corrected comes from the sensor type in
  its mission configuration message (RBR yes, SBE no), else the `.meta` file's WMO instrument
  type; before, only the `.meta` file decided, so most RBR floats were uncorrected. The offset is
  the mean of each dive's outlier-free minimum surface pressure (was the mean of the surface
  means), over dives with at least 10 samples, and the surface time series is no longer
  corrected in the listing. The Pressure Correction section says whether the correction was
  applied and how to recover the raw pressures.

Build:

- The Release makefiles are regenerated from the Debug ones with `-O2 -g` (they referenced
  missing Boost sources and did not link the libraries). `XMfaMissionCfg.cpp` is built with
  `-O1`; GCC did not finish it at `-O2`.
- The build number is incremented per commit (`build/hooks/pre-commit`), not per build.

Python tools:

- The average surface pressure plot shows each dive's offset correction (`PRC` line) as a blue
  dashed line, parsed by `xmtps-list2json.py` (`Surface.pres_correction`, ...) and carried into
  the aggregated surface table (`pres_corr`) by `xmtps-aggregate-json.py`.
- The README links the published plot pages at argo.whoi.edu/alamo.
- Trajectory plot bathymetry is interpolated bilinearly between ETOPO2 cells (it was the
  nearest, up to a third of a cell off). ETOPO2 is read from `/data1/etopo2/` by default;
  `--etopo2` is an alias of `-b/--bathymetry`.
- Docstrings for all Python tool modules, classes and functions; no code changed.

Documentation:

- The README describes the listing file format, and the SQL backend in general rather than table
  by table.
- The README documents the Python tools' JSON files: per-dive jsonpickle files, aggregate tables,
  the float summary and `.lastprocessed`.
- `sql/xmtps-schema.sql` creates the database tables (the production schema, with the new
  `XFloatMail.iridium_position_datetime` index).
- The README has a table of contents.
- The README lists the compiler, library, MySQL and Python package versions production uses.
- New `spec/xmessage-format.md`: a full ALAMO/ALTO X-message format specification, noting where
  its sources disagree.
- Changelog entries for the versions before the git repository (2014-09 to 2026-07).

## 2026-10-24

Decoder daemon:

- Wave: the v2 spectral decoder no longer reads a sample past the last bin, which made 51-sample
  packets (9287, 9291) fail the 0.3 g²/Hz check and produce no wave products.
- Wave: bin frequencies include the packet's first-sample index, and `wvm` (m²/Hz) columns are
  in `wvs` order again (zz, yy, xx, xy, zx, zy), matching their header. Builds 3234–3289 printed
  them as xx, xy, yy, zz under that header.
- New: dynamically corrected salinity (X-message 0xDC, floats with `PPE` = 2) is decoded and
  listed as a `salnCorr` column on `pro` lines (format inferred).
- New: drift sample times (X-messages 0x78–0x7F, format inferred) are decoded. For floats that
  send them, the drift section uses the fall/rise layout: `D SN dive nSam driftStart`, then `d`
  lines with a `+seconds` time before pressure, temperature and salinity.
- New listing section "Profile Data (Binned) - Sensor Columns": a `PRD`/`prd` table of the
  dive's profile columns with units and descriptions.
- Pressures are listed with two decimals (was three); the fill value is still `-9.999`, and the
  surface pressure variance keeps three.
- Apart from pressure decimals, listings for floats that send neither 0xDC nor 0x78 are
  unchanged.
- Mission configuration message 1 for 10.8.0.4/10.12.0.x accepts 88 bytes (with CTD correction
  parameters) as well as 71.
- Fixed mission configuration offsets: `ddt` in five layouts, `descend_start_time` for
  2.0.69–2.5.62, `isa` overlapping `isd` from 5.0.61 on, and `sao` read across the terminator.
- Fixed BIT v8 temperature and air-valve calibration offsets, ice engineering test
  salinity/temperature range checks (salinity always read 2.000), surface engineering v0x10/v0x12
  bounds, and the XArgo v2.1 surface salinity offset.
- Surface pressure variance is divided by gain² (it was 1/gain too large).
- Added length checks for short or malformed X-messages, bounded curvature sub-blocks, accepted
  phase code 15, and cleared auxiliary temperature data on dive reset.

Python tools:

- `xmtps-list2json.py` reads `pro` and `d` lines by header column names, handling `salnCorr`,
  the drift-times layout and the aux-temperature format.
- Measured drift sample times (`D` line drift start plus each `d` line's offset) are used when
  present; otherwise they are estimated as before.
- Profiles carry `saln_corr` in the dive JSON and aggregated profile table (NaN if not sent).
- Mission parameters: every `m` line is kept (`mission.entries`), `mission.parameters` keeps
  non-numeric values instead of failing, the `MIS` line also gives `cfgver`, and older listings
  with a minor index on that line give the right firmware version.
- Engineering data: the Engineering classes in `FloatClasses.py` define every parameter of every
  descent, drift, ascent, surface, ice and BIT message version as an attribute named with its
  units (e.g. `drift.mean_pres_dbar`), mapped from `E0x` line headers through `ENG_COLUMNS`;
  other columns go into `engineering.extra` and `e0x` lines into `engineering.errors`. The old
  fixed-position E07–E0A parsing was removed.
- `xmtps-aggregate-json.py` also writes `<id>_mission.json`, `<id>_engineering_data.json` and
  `<id>_engineering_errors.json`.
- Trajectory plots show the target drift pressure (`DTD<n>` for phase `MPS`, or `DTD`; phase
  `MNS<n>` once `MPC` ≥ `MNM<n>`) as a dashed gray step line, and the bathymetry at the GPS fixes
  ("Bathymetry (ETOPO2)") from NOAA ETOPO2v2c, converted to pressure with Saunders (1981). The
  file is `/data1/etopo2/ETOPO2v2c_f4.nc` by default (`-b/--bathymetry/--etopo2 FILE`); without
  it the line is left out with a warning.
- `xmtps-aggregate-json.py` plotting is split into one function per plot group (`WriteGeoPlots`
  … `WriteWavePlots`, run from `PLOT_WRITERS`); output is unchanged.
- Plot images are written through one persistent kaleido/Chromium browser instead of one per
  image, falling back to the old way if it can't start.

Operations:

- Both systemd units restart after 30 s (`RestartSec=30`); the repository copies match the
  installed units (decoder runs with `-v`; plot PID file name).

Documentation:

- Added this README, documented all command-line, configuration and Python tool options and the
  SQL schema, fixed stale Doxygen comments, and added the Doxygen PDF manual.

## 2026-10-23

Decoder daemon:

- Fixed the mail retrieval thread busy-looping on an unparseable mail file (now renamed `.err`)
  and replaying a whole batch when one server message failed.
- Worker threads drain their queues on shutdown; termination flags are atomic.
- Fixed the latest Iridium position being compared against the GPS time instead of the Iridium
  time.
- Fixed composite Iridium positions: latitude/longitude were swapped in the variance, and
  longitudes are averaged correctly across ±180°.
- Mail parsing: "Message Size" is parsed, the Date header timezone is converted to UTC correctly,
  and SBD attachments must be at least 8 bytes.
- The mail fetch limit applies after filtering for `.raw`/`.txt` files; the mail user and
  password are percent-encoded in the server URL.
- Status files follow `gen.stat` instead of `gen.kml`; archived floats go into the KML "archive"
  folder; floats with a single GPS fix get a latest-position marker.
- Metadata: launch times at hour or minute 0 are accepted and hour 24 rejected, a bad field no
  longer stops parsing, launch positions accept any whitespace, and DAC ID 9999 placeholders are
  ignored.
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
- Fixed the RhoLUT maximum temperature (39 °C, was 34 °C); pressures below the table use its
  minimum pressure instead of 0.
- Fixed drift fill values, a crash on 0 V pump records, and `.lastprocessed` being written before
  output succeeded.
- Wave directional coefficients use the displacement formulation; `r1` and `r2` are distinct and
  in [0, 1].
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

Reconstructed on 2026-10-25 from the old repository's history (`git.backup/edu.whoi.aekholm` on argus, 2014-09-08 to 2021-01-29, build 2975) and from source snapshots on argus (`/data1/alamo/src/xmtps.backup*`) and in Dropbox (builds 2988, 3143, 3150, 3194, 3219, 3225, 3234, 3238, 3249 and 3254; 3254 is identical to the initial commit, build 3255).

Release dates are the build dates in the listings' `XSV` line. Changes between two snapshots are listed under the production builds between them.

### 0.6.3252 (2026-07-01); source build 3254

- Build only: `boost::lock_guard`/`boost::recursive_mutex` replaced with the `std::` equivalents.

### 0.6.3248 (2026-05-29) and 0.6.3249 debug (2026-06-24); source build 3249

- Decodes the marginal-ice-zone acoustic messages, whose type ID moved from 0xC0 to 0xCA (`XAcousticPosition.h`): subtype 3 listening windows and subtype 5 CACST receive records, with each ping assigned to its listening window.
- New `APS` listing lines under "Position Data / Acoustic Position". The SNROut column repeats SNRIn (a bug). Acoustic data no longer counts as position data for a dive.
- Mission configuration: firmware 0x0A0C0032 (ALAMO 10.12.0.32) is decoded with the 10.8.0.4 layout.
- Fall/rise listing header units read "(UTC/+sec)" instead of "(start-time/+sec)".

### 0.6.3235 debug (2025-10-08) and 0.6.3237 (2025-11-07); source build 3238

- Wave spectra: a correction for firmware 0x0A060231, 0x0A080031, 0x0A090131 and 0x0A0A0031, which send only every other spectral value; it doubles the effective sample rate and recomputes the frequencies.
- Wave frequency bins are back to `fs·(i+1)/seglen`.
- `.meta` files with 5-digit DAC IDs are accepted.
- Fixed an inverted condition: the start-of-mission filter on fetched messages was applied only when the start-of-mission time was invalid.

### 0.6.3233 (2025-09-05) and 0.6.3232 (2025-09-30); source build 3234

- Wave spectra rework (`XWave.cpp`, `XWaveSpectralSample.h`):
  - Version-2 spectral scale factors changed to {8192, 2048, 512, 128}, as for version 1, halving `wvs`/`wvm` values.
  - Fixed the fill-value test in the `wvm` accessors, which all tested the xx component.
  - Frequency bins changed to `fs·i/seglen` (reverted in 0.6.3237).
  - Only spectral versions 1 and 2 are listed.
- Regression: `wvm` columns were printed as xx, xy, yy, zz, zx, zy under the zz, yy, xx, xy, zx, zy header. Builds 3234 to 3289 have mislabeled `wvm` columns; fixed on 2026-10-24.

### 0.6.3222 (2025-02-14), 0.6.3224 (2025-07-15) and 0.6.3226 (2025-08-22); source build 3225

- BIT message version 11 is listed (listing it failed before). Messages of 256 or 264 bytes are accepted; the extra 8 bytes are not decoded.
- Mission configuration: firmware 0x0A0A0031 (WIMS 10.10.0.31) is supported.
- KML: the latest-position placemark description is in a `<p>` element, without the trailing instrument ID.

### 0.6.3219 (2024-09-25); source build 3219

- Mission configuration: firmware 0x0A070101 (10.7.1.1) is supported, with the 10.8 message-0 layout and an 88-byte message 1. Its shared listing block with 10.8.0.4 no longer prints the dropped BIT thresholds (PNB, PNC, PXC).

### 0.6.3217 (2024-09-11); source build 3194

- New `mail.fetchMode` option: 0 fetches from the IMAP server (default), 1 reads `*.raw` and `*.txt` mail files from `dir.mail` under a PID lock and deletes them once parsed. New `dir.mail` and `dir.mail.processed` (not used yet) options.
- Stored mail files can be parsed back into messages (`XFloatMailMessage` `operator>>`).
- The `dir.*` options got defaults (`kml`, `lis`, `log`, `meta`, `raw`, `sbd`, `stat`, `phy`). The maximum fetch count went from 128 to 127.
- BIT message version 11 (firmware 10.8) is decoded: a 32-bit traveler error field at offset 50 moves all later fields. The CTD status is read as a 32-bit integer in BIT 9 and 10.
- Mission configuration: firmware 0x0A080004 (10.8.0.4, SBE) and 0x0A080031 (10.8.0.31, WIMS) are supported: a 327-byte message 0 without PNB/PNC/PXC, and a 71- or 90-byte message 1 with `WST` and the dynamic salinity correction parameters.
- Known problems at this build: the 10.8.0.4 parser read beyond the 71-byte message 1, BIT v11 messages could not be listed, and the dynamic salinity correction lines were commented out of the 10.5 to 10.6.3 listings.

### 0.6.3193 (2024-09-03); source build 3150

- Mission configuration: firmware 0x0A060231 (10.6.2.31, WIMS) is supported, with a 95-byte message 1 and a "Surface - Wave-Field Measurement" listing section (`WST`) plus the dynamic salinity correction parameters.
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
  - New firmware: 0x08010001 (2021 hurricane floats) and 0x0A020201 with the 0x0800xxxx layout; 0x08010000, 0x08020000, 0x09000000, 0x0A000000, 0x0A000100, 0x0A020000 to 0x0A020200, 0x0A030000 and 0x0A030100 with a 76-byte message 1; 10.5.0.0 to 10.5.0.4, 10.6.0.0 to 10.6.0.4, 10.6.1.0 to 10.6.1.4, 10.6.1.33 (Micro-ALTO) and 10.6.2.0 to 10.6.2.4 with a 93-byte message 1.
  - New "Profile - Dynamic Salinity Correction" listing section, with `PPE` and `PPC0` to `PPC3` (tau and alpha printed against each other's labels).
- **Firmware version tracking:** only mission configuration message 0 updates a float's firmware version (an always-true comparison let any message do it).
- **BIT version 2 decoder** (`XEngBIT.h`, `XAlamoBit2.h`): BIT versions 6 to 8 use the old decoder (6 newly accepted); 9 (firmware 10.5) and 10 (10.6) use a new 256-byte decoder. `E05`/`E06` lines got a `BITVer` column, and serial numbers are not zero-padded.
- **Wave message:** byte 3 is the message version; version 2 adds an accelerometer gain at offset 14 (effective scale `assf·gain²/64`, shown in the `WVS` header); `wvs` values are scaled; new `wvm` lines give displacement spectra in m²/Hz.
- **`.meta` files:** 4- to 6-digit serial numbers are accepted in the filename, and the DAC and WMO IDs are copied into the float record.
- **Sensor data:** values left over after curvature unpacking log a warning instead of failing the message.
- **Reprocessing and threads:** startup reprocessing covers floats heard from in the last 90 days (was 180); the daemon thread spin-up delay is 10 s (was 30 s).
- **Options:** `dir.nc`, `gen.nc` and `poc.nc` were removed, the default `mail.user` is `alamo`, and help text was added for several options.
- **Repository additions:** the systemd units `xmtps-alamo.service` and `xmtps-alamo-plot.service`, the Python plotting tools in `bin/py`, and build notes for Ubuntu 20.04 and g++-10.

### Build 2988 (2021-03); Dropbox snapshot

- Ported from `boost::shared_ptr` to `std::shared_ptr`, and to the VMime 0.9.2 API.
- Removed NetCDF output (the writer thread, `NetcdfUtils` and `-lnetcdf_c++4`).
- Built with the system `g++` instead of `g++-4.7`. Sample configuration in `cfg/options.cfg`.

### 0.6.2971 to 0.6.2975 (2020-08-10 to 2021-01-29)

- Mission parameter support for firmware 0x08000101 (2020-08-10), 0x08000300 (2020-11-23) and 8.0.5 (2021-01-29).
- Fixed the listing not being updated when wave spectra were the last message received in real time (2020-09-01).

### 0.6.2923 to 0.6.2962 (2020-01-15 to 2020-07-29)

- Firmware 0x08000001 (2020-07-24) added directional, spectral wave data, with decoding fixes on 2020-07-29.
- Mission parameter support for firmware 0x07020200, 0x07020400, 0x07020800, 0x07020900 and 0x07030500 (2020-03 to 2020-06).
- RAW file names changed (2020-01-15) so that messages with duplicate MOMSNs are not lost; fixed RAW file names for incomplete messages (2020-03-31).

### 0.6.2869 to 0.6.2910 (2019-08-13 to 2019-09-17)

- Wave data decoding, and a hex dump of each message at the end of the listing (2019-08-13).
- Fixed mission configuration decoding for firmware 6.0.x, 6.4.9 and 6.5.w.
- Added firmware 0x06057700 and 0x06010300 (2019 ALTO RBR/O2 test float).
- Fixed the surface engineering message v0x14 and mission message 0xD1 for 0x06050100.

### 0.6.2803 to 0.6.2805 (2018-10-31 to 2019-06-28)

- Support for the firmware 6.0 message changes (2018-10-29 to 2018-10-31) to mission parameters, ascent engineering, Argo data and drift engineering; firmware 0x06000100 (2019-03-19) and 0x06050100 (2019-05-30).
- Fixed the "start-of-drift" label in the drift engineering listing (2019-03-29).
- PHY files support WMO instrument type 868 (ALAMO with an SBE CTD), and samples are ordered by pressure (2019-06).
- Partial spectral wave decoding (2019-06-21).

### 0.6.2764 to 0.6.2775 (2017-10-03 to 2018-05-23)

- MFA mission configuration decoding (2017-10-16): phase-specific fall-to-profile parameters, fixes for firmware 0x05006100, and an updated listing format.
- Fixed surface engineering error code decoding (2017-11-03).
- Ice engineering message v0x11 (2018-05-16).
- Active floats (a valid SBD message in the last 180 days) are reprocessed at startup by default (2018-05-23).

### 0.6.2592 to 0.6.2633 (2017-03-03 to 2017-08-17)

- **Mission configuration support:** ALAMO software 0x04016100 (2016 NASCar floats), 0x04056300, 0x04066100 and 0x04046100 (2017 hurricane floats); fixed decoding for 0x03006500.
- **Surface pressure offset correction:** fixed bad offsets from spurious surface pressures; no longer computed for SBE floats (2017-04-21).
- **Surface data:** surface engineering message v0x13, and fixes to derived surface fields when a dive has several surface messages.
- **Listing format:** auxiliary temperature data is part of the profile section; IMEIs are truncated for security (2017-08-02); "counts" fields were removed from the composite sections; unbinned profile lines became `pru`.
- **PHY files:** written for ALAMO floats with salinity, excluding binned samples above 1 dBar (2017-08).
- **NetCDF:** JULD from the start-of-ascent time, writer thread fixes, and unbinned profiles left out for now.
- **GPS and KML:** GPS messages with future dates are rejected, and KML files link to argo.whoi.edu.
- **Configuration:** the valid DAC ID range changed, and per-instrument log files were disabled.

### 0.6.1555 to 0.6.2457 (2016-02-14 to 2016-12-29)

- **Mail and SQL server connections:** better error handling (2016-02-14).
- **New messages for firmware 0x02006900** (2016-03-24): engineering surface v0x12, valve data, mission configuration, and ice detection v0x10.
- **New output:** NetCDF files (2016-04-11), and per-instrument options to enable or disable PHY files (2016-06-01).
- **Surface pressure offset correction** (2016-06-23 to 06-27): computed from the surface pressures of the previous N dives and applied to the listing, PHY and NetCDF files.
- **Fixes:** curvature unpacking for packing factors above 3; missing data in some listings.
- **Valid instrument IDs:** 1000 to 10999.
- **Mail options** (2016-08): move processed messages out of the inbox folder, the starting message index, more robust mail retrieval, and handling of unsupported `.meta` fields.
- **Master KML:** active (last 30 days) and archived floats in separate folders.
- **Mission parameter messages:** firmware 0x03016700 (Solo-II hybrid test float 10083) and 0x0302100 (2016 NASCar floats); reprocessing no longer fetches mail from before the start of the mission.

### 0.6.1451 (2015-09-22) and earlier (2014-09-08 to 2015-09-22)

- **2014-09:** first versions on argus: graceful shutdown from the signal handler, the init.d script, zero decoder threads to fetch mail only, and listing, pump-data, PHY and gain/offset fixes; version 0.6 (2014-10-20).
- **2015-01 to 2015-03:** X-message format 2.2: RAW mail files in standard or extended format, the Argo data message, the 9-byte surface time series with temperature and salinity, and curvature packing for profile and drift data. Reprocessing of one instrument by ID (2015-05-11).
- **2015-06 to 2015-07:** ALAMO messages: BIT v7, engineering descent, drift and ascent v0x11, engineering surface (0xEA), mission configuration for software 0x02002300, and the new pump data format. A FIFO of RAW file paths for the S2A decoder, and reworked sensor data decoding for curvature-packed messages of differing lengths.
- **2015-07 to 2015-09:** fixed swapped drift-engineering temperature and salinity and the descent first pump-time adjustment; KML (2015-08-11) and status files (2015-08-21); stored messages are reprocessed when a dive is loaded from the database; the start-of-dive GPS fix can stand in for the previous dive's profile position; firmware 0x02006700 and 0x02006800.
