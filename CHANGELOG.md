# Changelog

Changes to XMTPS, its Python tools and its documentation, newest first. See
[README.md](README.md) for how the current version works.

## 2026-11-08

Python tools:

- Remaining-cycles plot: usable energy 68 % of nominal, range 50-95 % (was 80 %, 65-95 %).
  Backtested on the 92 floats that ran out of battery, the old values over-predicted the cycles
  left by 1.4-1.9 times through most of a float's life; the new ones are unbiased (1.04-1.07)
  and about three quarters of the floats end inside the range.
- Remaining-cycles plot: a falling pump voltage is marked (red dashed line and label from the dive
  where the highest pump voltage stays below 95 % of its early-life median for 3 dives; the title
  ends "Pump voltage falling"). On the floats that ran out of battery it came a median 60 dives,
  16 % of their dives, before the end, and caught the abrupt failures the energy estimate misses.

## 2026-11-07

Decoder:

- Empty Iridium messages and failed transfers are listed. The gateway sends a mail without an SBD
  attachment for an empty upload (session status 0 or 2) and for a failed transfer (any other
  status, mostly 13, Incomplete Transfer). These were kept only as a RAW `-incomplete-` file and a
  database row. Two new listing lines, `SBE` (empty) and `SBF` (failed), list them after the `SBT`
  lines: MOMSN, MTMSN, session status, time, position, CEP and the status text. A failed transfer
  goes with the dive of its retry (the message with the same MOMSN), an empty upload with the dive
  of the nearest message in time, within 24 hours. In production about 95% of these mails are
  failed transfers.

Python tools:

- `SBE` and `SBF` lines are read into the dive JSON as `sbd_empty` and `sbd_failed` (new class
  `SbdSession`), and aggregated into the new `<serial>_sbd_session.json` (column `kind`: `empty`
  or `failed`). They are not in `_sbd`, so the message counts, sizes, positions and energy are
  unchanged.
- Plots: the satcom plot marks empty uploads (yellow) and failed transfers (red) in the colors of
  fmtps, under the messages with data and larger, so that a failed transfer shows as a red ring
  around its retry a few minutes later. The "Iridium TX" markers are now purple, as in fmtps, so
  that red only means failed.

- Plots: the remaining-cycles hover label shows the expected, minimum and maximum cycles
  left after the dive, in one label on the estimate line.

## 2026-11-06

Decoder:

- Micro-ALTO microstructure data. X-message 0xF1, which the decoder ignored as a "test pattern", is
  MRV's guest file fragment: micro-ALTO floats (11491, 11492, 11494, 11495) send in it the files
  their Rockland MAPLe board writes during the ascent. New class `XMaple` (`src/data/xmsg/XMaple.h`)
  rebuilds the files from their fragments (encodings 0 and 1, any packet order, retransmissions
  ignored) and decodes them: signal means (first differences summed), band-averaged spectra
  (little-endian float32, or 8-bit with `%scaleoff`: 10^(b/scale - offset)) and motor events. The
  listing has a new section "Microstructure Data (MAPLe, 0xF1)" with `MSF` (files), `MSC`
  (channels), `MSB` (bands), `msm` (signal mean), `msv` (spectra) and `MSX` (motor events) lines.
  Record times are the record middles, from the MAPLe start time (UTC) and effective sample rate.
  The SBD summary (`SBT`) names 0xF1 "guest file fragments (MAPLe)".

Python tools:

- Plots: a watchdog on the PNG export. A Kaleido call that waits on its headless Chrome longer
  than `--image-timeout` (default 120 s) plus 10 s an image counts as hung. The process kills the
  Chrome it started (not other floats'), starts a new browser and retries the images one at a time.
  After 3 restarts, the rest of the images fail. On 2026-11-06, 9044, 9045, 9207, 9304 and 11558
  hung this way until the one-hour limit of the plot service, and each was fine when rerun. A
  simulated hang on 11558 cost 115 s, after which all 243 images were written.
- Energy estimate: battery age at each dive's first SBD message, not its last. A float resends a
  lost packet of an earlier dive at a later surfacing; on 11213 dive 48 resent packets of dives 40
  and 43, which then took the controller drain and self-discharge of eight days (21 kJ) and left
  dives 41-48 with none.
- GPS: a fix that puts the float more than 2 m/s away from the surfacings before and after it,
  over the time submerged, is rejected (`RejectGpsSubmergedSpeed`): it stays in `_gps` with the new
  columns `valid` false and `qc_reject` "submerged_speed", and is left out of the profile positions,
  displacements, position plots and summary fixes. One bad fix is rejected at a time, so the good
  fixes around it stay. Rejects 100 fixes on 21 floats, 60 of them on 10110, 10114 and 11013, which
  cycled on a ship in May 2018. The displacements now skip invalid fixes instead of giving NaN.
- Energy estimate: floats whose dive numbers restarted after a reset in the field (9113, 9223, 9226,
  9245) took the reset's startup as the pack's start, so their dives before the reset got no
  controller drain or self-discharge. The first dive is now the earliest in time, and the dives
  share out the battery age in time order (9113: 141 to 222 days, 0.76 to 1.01 MJ).
- Energy estimate: new component `maple_j` (`maple_s`) for the Rockland MAPLe microstructure
  system of the Micro-ALTO floats (ALTO 11491-11496, or firmware ending in 0x33): 0.3 W over each
  dive's ascent, when it logs. No published power; 11491 and 11495, which ran out of battery,
  bound it to at most about 0.5-0.8 W. Adds about 0.3-0.4 MJ to each of those two floats.
- `xmtps-aggregate-json.py`: micro-ALTO float pages get a "Microstructure Data" section with two
  plots of the latest dive with MAPLe spectra: `microstructure_profile` (signal variance against
  pressure, and a pressure-by-frequency image of each channel's band-averaged spectrum) and
  `microstructure_shear_spectra` (the shear spectra on log-log axes, one line per record, colored
  by pressure). Values are in counts. New aggregate files `_microstructure` and
  `_microstructure_bands`.
- `xmtps-list2json.py` reads the microstructure lines into a new per-dive `microstructure` member
  (`FloatClasses.Microstructure`, `MicrostructureChannel`): files, channel setup, bands, record
  times and pressures (interpolated from the ascent), signal means, shear masks, spectra and motor
  events. Absent for other floats; `json_vers` is unchanged.
- `xmtps-aggregate-json.py`, `spec/json-file-format.md`: the low startup voltages of ALTO 11020-11034,
  the 2017-2018 SBE ALTOs and ALAMO 9179, 9181 and 9207 are put down to the diode on their VOP11
  controller boards (MRV, 2018), not to inline diodes in the packs; wording only, no change in
  the energy estimate.
- `xmtps-aggregate-json.py`, float pages: new plot `energy_remaining_cycles`, the cycles left if the
  float stays on its current mission, with a range: usable energy 65-95 % (central 80 %) of the
  pack's nominal energy, the energy and duration of a cycle the medians of the last 10 dives, and
  the controller drain +-30 %. The pies `energy_by_component` and `satcom_sbd_data_type` are now single
  horizontal stacked bars, drawn wide and short (new size class `figWideBar`).
- Energy estimate: floats deployed (first positive dive) from 2024-01-01 count as having the fixed
  controller board (no Bluetooth fault), whatever the date of their first SBD message
  (`ENERGY_BLUETOOTH_FIXED_DEPLOYED`).
- Energy estimate: pack overrides for 10111 and 11015 (Tadiran behind diodes; read as alkaline, they
  used 120 % and 156 % of an alkaline pack, like the rest of the 2017-2018 ALTO batch) and 9032 (Saft,
  like its 2014-2016 batch; its startup voltage reads about 10 times too high).
- `xmtps-aggregate-json.py`: plot groups skip data they cannot use instead of failing with an
  ERROR (about 730 lines in the last full rebuild). Ascent, descent, surface, drift, displacement,
  profile position and contour plots return early, with a warning, when no rows are left after
  filtering; contours need at least 2 usable profiles (one profile left the grids 1-D). Floats
  without salinity (the early ALAMOs had RBR temperature and pressure sensors only) now get their
  temperature profile, drift and contour plots; the salinity, density, sigma0 and T-S figures are
  left out (new helpers `RangePercentile`, `DropFigures`). Wave aggregation keeps a missing mean
  wave direction (`mwd` `NaN`) and leaves spectrum arrays whose length differs from the frequencies
  empty (`NaN`), instead of dropping the dive's wave data (9108, 9237); the wave waterfalls are
  left out with fewer than 2 wave dives (a single column drew a zero-width image: 9108, 9113, 9307).
  A failing plot group now logs the exception type and message; the surface displacement message
  no longer says "submerged displacement plotes". The drift temperature range no longer needs sigma0.
- `xmtps-aggregate-json.py`: ascent and descent records whose arrays are shorter than their sample
  count (one-sample ascents without times or rate, 9031, 9032, 9035, 9036) are padded instead of
  dropping the dive's data (new helper `FitLength`); surface plots leave out the temperature
  figures when a float has no surface temperature instead of failing; wave plots are skipped when
  no spectra are left after filtering (9046); profile plots are skipped when no profile with
  temperature is left (9091, 9212).
- `xmtps-aggregate-json.py`: plots of floats with a single dive no longer divide by zero in the
  dive color ramp (10008, 10270); the satellite-count and HDOP axes allow an empty GPS table, and ascent and
  descent plots allow no rate values (11332).

Documentation:

- `spec/xmessage-format.md` C.8 (0xF1 blob layout, MAPLe file IDs and file formats, checked on
  11491, 11492, 11494, 11495 and MRV's 11344 demonstration data), `spec/listing-file-format.md`
  section 13 (sections 13-17 renumbered 14-18), `spec/json-file-format.md` 3.16 (3.16-3.17
  renumbered 3.17-3.18), README listing tags.

## 2026-11-05

Decoder:

- Startup messages (negative dive numbers) are no longer dropped by the start-of-mission and
  deployment-time filters. Reprocessing a float (`-r`, `-R`, `-A`) now selects all of its startup
  messages and regenerates every startup listing file (`lis<serial>--NNN.txt`); before, only the
  startup session after the newest built-in-test fix was kept (11273: `--007` was rewritten,
  `--001` to `--006` were not). Changed in `XFloatDiveData::DecodeMailMessageData()`,
  `XFloatDiveData::GetDiveDataMailMessagesFromDatabase()` and the reprocessing query in
  `stage1_MailRetrievalThread.cpp`.
- Each startup session keeps its own listing file. A restarted float numbers its startup dives
  from -1 again, and the sessions used to merge into one dive and one listing (11017's `-001`
  holds messages from several startups). A startup message whose dive number does not
  decrement from the previous startup message's starts a new session, if its dive already has
  messages at least 1 h older (without a previous startup dive in the run, a gap of more than
  12 h). The earlier listing is renamed `lis<serial>-<dive>_<YYYYMMDD>T<HHMMSS>.txt` (UTC time
  of its first message) and the newest session keeps the unchanged name. Reloading a startup
  dive from the database keeps only its latest session (split at gaps of more than 12 h).
  `xmtps-list2json.py` skips the renamed listings. New: `XFloatDiveData::IsNewStartupSession()`,
  `ArchiveStartupListingFile()`, `XFloat::StartNewStartupSession()`,
  `XFloat::last_startup_dive()`.

Python tools:

- Energy estimate: ALAMO packs are taken as 24 cells, like the 24-cell Saft ALAMOs (was 16): Tadiran
  24 TL-6920 C cells, 734 Wh / 2.6 MJ (was 1.8 MJ); alkaline 24 D cells, 540 Wh / 1.9 MJ. The
  depleted 2020-2024 RBR ALAMOs now come out at 72 % of nominal (was 107 %). Only `pack_wh` and
  the self-discharge term of ALAMOs with Tadiran or alkaline packs change; their energy
  aggregates must be regenerated (`xmtps-aggregate-json.py -f`).
- Energy estimate: pack overrides for floats that used more than their pack's nominal energy
  before depleting: ALTO 10110, 10112, 10113, 10115, 10116 and 11013 (2017-2018, read as alkaline,
  150-195 %) are Tadiran, as with the diode Tadiran ALTOs 11020-11034; ALAMO 9033 (startup
  13.77 V, read as alkaline, 113 %) is Saft.
- Energy estimate: pack energies from public datasheets. The Tadiran TL-69xx cells are 3.9 V
  PulsesPlus (Li-SO2Cl2) cells; ALAMO Tadiran is 24 TL-6920 C cells at 7.5 Ah x 3.9 V, 2.5 MJ
  (was 8.5 Ah x 3.6 V, 2.6 MJ). Alkaline cells are the same size as the lithium cells: ALAMO
  alkaline is 24 Procell C (PC1400, 8.5 Ah), 1.1 MJ (was 24 D cells, 1.9 MJ). New overrides for
  floats that used more than an alkaline C-cell pack: 9062 is Saft; 9179, 9181 and 9207 are Tadiran
  (behind diodes, startup 11.4-11.9 V).
- Energy estimate: the controller drain is split for the Bluetooth power fault of VOP11 and early
  VOP12 controller boards, which leave the Bluetooth module powered (MRV, 2023: about 2 mA sleep
  current), also in storage. Every float gets 20 mW over each cycle (from 11342, a long-lived ALTO
  with a fixed board); a faulty board 15 mW more, over each cycle and over the storage before
  deployment (35 mW in all, as before). A board is faulty when the float's earliest SBD message is
  before 2022-07-01, except ALAMOs refitted with new boards and batteries in 2023 (9255-9257, 9289,
  9291-9293, 9296, 9297, 9299), whose battery age counts from the refit; 9280 (first message 2020,
  deployed 2024, startup voltage unchanged) is taken as refitted in December 2022. New `_energy`
  column `bluetooth_fault`.
- Oil pump power and energy are computed from the battery voltage and current: power = `volt` ×
  `curr` (was `volt` × `curr_adj`, with `curr_adj` = `curr` × 12 / `volt` / 0.85). The pumps run
  from a 12 V rail made by a buck-boost converter from the batteries, but `pumpI` is the battery
  current, so the 12 V / 85 % conversion did not apply. Pump energy rises by about 6 % (11273:
  498.6 to 528.1 kJ). `curr_adj` is dropped from the dive and `_pump` JSON, and the pump current
  plots show `curr`. Floats must be reprocessed (`xmtps-list2json.py -f`, then
  `xmtps-aggregate-json.py -f`) for their JSON and plots to change.
- Energy estimate: new WIMS component (`wims_j`, `wims_s` in `_energy`) for floats with wave
  data: the WIMS running power, 0.0456 W at 15 V (WIMS user manual rev. 3.22, Table 3), for one
  run of 590 s per dive with wave data; the power while not sampling is negligible. Both energy
  plots show it as "WIMS"; floats without WIMS are unchanged. `xmtps-list2json.py` keeps the
  `WVS` `fbase` and `fdiv` in the dive JSON (`Wave.fbase`, `Wave.fdiv`), and the `_wave`
  aggregate gains `slen` and `fe_hz` (the true sample rate; for older dive JSON, `fsam` halved
  for the every-other-bin firmware).
- Energy estimate: pack overrides for floats whose startup voltage misleads the pack rule: 9109
  (Saft, read as alkaline at 13.6-13.7 V), and 9322, 9323, 11046, 11048, 11058 (Tadiran; no startup
  voltage received before dive 1). An override now applies even without a startup voltage.
- Profile positions and bathymetry plot (`position`): the depth colorbar has the shallowest
  depth (and land, 0 m) at the top and the deepest at the bottom. The map is drawn as negative
  depth; the colorbar labels and the hover show positive depths.
- Energy estimate: controller drain raised from 30 to 35 mW (about 2.4 mA at 15 V), one value for
  ALAMO and ALTO, which share the controller. Re-estimated with the datasheet per-dive terms and
  self-discharge: the long-lived ALTO groups give 35-38 mW for a usable pack energy of 5.9 MJ
  (from 11491 and 11495).
- Energy estimate: Iridium and GPS powers from the datasheets of the fitted parts, replacing the
  SOLO-II fit. Iridium 9602 (Product Developers Guide rev 5): SBD transfer average 190 mA at
  5.0 V, 0.95 W (was 0.70 W) over the session time. u-blox NEO-M8N (NEO-M8 data sheet R14,
  confirmed by an ALTO checklog): acquisition 32 mA at 3.0 V, 0.096 W (was 0.27 W) over the time
  to fix. Regulator losses are not counted. 11273: Iridium 30.1 to 40.8 kJ, GPS 14.4 to 5.1 kJ.
- `update-plots-service.sh`: the listing pattern now starts `(90|91|92|93|94|10|11)` (was
  `(908|909|91|...)`), so the older ALAMOs 9003-9079 are converted to JSON and plotted too. A
  test conversion of 9005, 9023, 9035 and 9061 wrote JSON for every listing (a few line and
  post-processing errors on 9023 and 9035). Stray directories (4790, 6796, 7127, 9511, 9672)
  stay excluded, and so do the older ALAMOs without usable profile data (`NOPROF`: 9004, 9005,
  9011, 9012, 9024, 9025, 9027, 9037, 9038, 9042, 9047, 9050, 9056, 9059, 9063), found by
  converting 9003-9079 to a scratch directory. The rest have temperature profiles; only 9003 also
  has valid salinity.
- Energy estimate: CTD energies from the manufacturers' documentation, replacing the SOLO-II
  fit (0.25 J a scan, 3.3 J a spot sample, × 0.2 for RBR). Sea-Bird SBE 41CP: 22 mA at 12 V with
  the pump running (hardware reference 41528F), so 0.264 J a 1 Hz profile scan and 0.66 J a spot
  sample (drift, surface), taken as the 2.5 s fast-pump start of `pumpon` since the manuals give no
  spot-sample pump time. RBR argo3: 24 mJ a sample, profile scan or spot sample alike. 11273
  (RBR): CTD profile 47.1 to 22.6 kJ, spot samples 4.1 to 0.15 kJ.
- Energy estimate: new controller component (`controller_j`), a constant 30 mW (about 2 mA at
  15 V) over each dive cycle (`self_discharge_s`), not over storage before deployment; shown as
  "Controller" in both energy plots. The figure comes from the floats that ran out of battery:
  usable pack energy (from short-lived floats) minus the estimate, over the lifetime of
  long-lived ones (ALTO 22-46 mW, ALAMO about 29 mW), and matches the SOLO-II CPU's about 2 mA.
  It adds about 2.4 kJ per daily cycle; 11273's total rises from 706 to 997 kJ.
- Energy estimate: new battery self-discharge component (`self_discharge_j`, with
  `battery_age_s`, `self_discharge_s`, `storage_s`, `battery_start_v`, `pack_type`, `pack_wh`
  and `pack_rate_per_yr`), shown as "Self-discharge" in both energy plots:
  - Pack: the chemistry from the voltage of the latest startup before the first dive, the
    startups in time order by their first SBD message (dive numbers are not in time order):
    Tadiran >= 14.6 V, Saft >= 13.8 V, else alkaline Procell D. ALTO 11017 and 11020-11034
    (Tadiran packs, 11020-11034 behind inline diodes) are set to Tadiran. Energy by chemistry
    and float type: ALTO Tadiran 6.7 MJ, ALAMO Tadiran 1.8 MJ, Saft 2.4 MJ, alkaline 32 (ALTO)
    or 16 (ALAMO) D cells. Rates: 1 %/yr lithium, 3 %/yr alkaline.
  - Battery age: from the pack's first startup (for a repacked float, the first startup of the
    latest run at the new pack's voltage level; without a startup before the first dive, the
    float's earliest SBD message). Each dive with a positive number gets the age gained since
    the previous one.
  - The self-discharge before the first dive's cycle (storage and tests) is a separate
    component, `storage_j` ("Self-discharge (storage)"), counted in the totals and the pie but
    not in the per-cycle bars.
- Energy estimate: pump records with an unusable current (`curr` <= 0, >= 100 or missing) are
  left out of the pump and internal-resistance terms, as the pump plots leave them out; both now
  use `PumpCurrentInvalid()`.
- Energy plot (`energy_per_cycle`): the title no longer adds "(RBR CTD: CTD x0.2)" for floats with
  an RBR CTD. The CTD estimates are still reduced as before.
- Energy plot (`energy_per_cycle`): the dive total is on its own line at the top of the hover label
  instead of being repeated after each component.
- New plot `energy_by_component` ("Estimated Total Energy"), in the float page's Engineering
  section: a pie of the float's estimated energy use by component over the same dives as
  `energy_per_cycle`, slices labeled with the component and kJ.
- Both energy plots count only dives with a positive dive number; dive 0 and the startup dives
  are left out.
- Float page: the four pump vacuum plots (`pump_vac`, `pump_vac_avg`, `pump_vac_v_time`,
  `pump_vac_v_pres`) moved from the Hydraulic System section to the Engineering section, after
  Case Vacuum through the Dive Cycle.
- Case Vacuum through the Dive Cycle (`phase_vac`): dashed grey lines at 3 and 7 inHg in the
  background, without hover.
- GPS CNO plot (`satcom_gps_cno`): not made for floats whose GPS reports a carrier-to-noise ratio
  of 0 in every fix the plot shows; their float page shows the All Positions map in its place.
- Total Pump Energy and Total Pump Duration per Cycle (`pump_energy_sum`, `pump_time_sum`): the
  bars are all grey (operations still separated by thin white lines) and there is no legend; the
  hover label ends with a `Total:` line.
- PNG export: images that Kaleido fails to render within a batch are now retried one at a time
  and their render error is logged. Kaleido 1.x does not raise for them, so the retry never ran and
  the figure was dropped with a misleading "No such file" error; now an image counts as written
  only if its file exists and is newer than the batch.
- SBD data pie chart (`satcom_sbd_data_type`): slices are labeled with their data size (e.g.
  `117.0 kB`) instead of the percentage, which is still in the hover. Only dives with a positive
  dive number count; dive 0 and the startup dives (negative numbers) are left out.

Documentation:

- New [spec/mrv-firmware-changelog.md](spec/mrv-firmware-changelog.md): the MRV firmware changelogs
  (VOP 8.0.0 – 10.13.0 and MFA 1.5.a – 4.5.d), collected from `Dropbox/mrv/firmware`, with notes
  on which source files the other copies duplicate.
- spec/xmessage-format.md C.2: the retransmission upload commands `TDD <dive>` and
  `TDP <dive> <packet>`, with their production use and the firmware changes that affect them.
- spec/listing-file-format.md: the `SBT` ratio column was rawBytes / bytes in builds 3352 and
  3353 and is bytes / rawBytes from build 3354; the change was intended.
- spec/plot-and-page-files.md: the bathymetry map's hover (`2743m`, `0m` on land) and plain gray
  land are described as the code draws them.

## 2026-11-04

Decoder:

- Listing files have a new subsection, "SBD Data by Message Type", after the SBD message lines.
  It has one `SBT` line per X-message type in the dive's SBD data (retransmissions counted once):
  number of blobs, bytes, percentage of the total and, for the compressed sensor series (binned,
  high-resolution and drift profiles, drift times, corrected salinity), the number of values, their
  size as plain counts (3 bytes each, 2 for the first-difference encoding, plus the 4-byte blob
  frame) and the compression ratio (bytes sent as a fraction of that size, two decimals); other
  types have ratio 1.00. A `frame` line counts the 12-byte
  packet header and trailer, an `unparsed` line any incomplete blob, and a `total` line the whole.
  `xmtps-list2json.py` reads them (below).

Python tools:

- `xmtps-list2json.py` reads the `SBT` lines into a new per-dive `sbd_types` list (`SbdType`), and
  `xmtps-aggregate-json.py` aggregates them into `<serial>_sbd_type.json`.
- The SBD Data Size plot is a stacked bar chart by X-message type, as in the `SBT` lines, with
  any retransmitted bytes on top. Dives listed before build 3352 show one grey "not broken down"
  bar.
- New plot `satcom_sbd_data_type`: a pie chart of the float's total SBD data by message type.
- New aggregate `<serial>_energy.json` and plot `energy_per_cycle` (new "Engineering" section of the
  float page, which also holds the case vacuum plot `phase_vac`, moved from "Hydraulic System"): estimated energy use per dive, stacked by component (oil pump descent and ascent,
  air pump, CTD profile, drift and surface samples, Iridium, GPS, battery internal resistance).
  The constants were fitted to the WHOI SOLO-II energy plots against the SOLO-II listing files:
  0.25 J per CTD scan, 3.3 J per spot sample, 0.70 W Iridium and 0.27 W GPS. For RBR CTDs the
  CTD estimates are reduced by 80%.
- Float plot pages: the footer is the shared one of the fleet pages, without the decoder status:
  page update time (and time since, in the browser) with the run time of that float's plots, the
  Python utility version and the XMTPS version.
- Float summary page: links to the Float Startup and Mission Parameters pages under the title.
- `xmtps-aggregate-json.py` writes each plot group's PNG images in one batch, spread over 4
  browser tabs (`--image-tabs`). A full rebuild of float 9305's plots on argus took 125 s instead
  of 239 s; the images are unchanged.
- Float pages: the Profile Positions plot (`position`) is now "Profile Positions and Bathymetry":
  the profiles over ETOPO 2022 depth with hillshade, isobaths and gray land, and the depth under
  the mouse anywhere on the map ("12m"; "0m" on land). Its PNG images have no color bar or axis labels. The previous map is
  kept as `position_geo` ("Profile Positions (Map)") in the figure table, and is shown instead
  when the bathymetry file can't be read. With `-d`, the time to build and
  write it, and the time of each plot group, are logged.
- The Total Pump Duration and Total Pump Energy per Cycle plots are stacked by pump operation
  (first, second, ... of the dive).
- First Pump Duration plot: the configured DFT line is drawn only at dives that transmitted their
  mission configuration, instead of carrying the last value forward. A float with `APT` = 0
  (9305 from dive 189) showed a stale DFT for over 100 dives while its first pump changed.

- WIMS wave spectra (`Wave.PostProcess`): the low-frequency response correction coefficients are
  [f0, w] = [0.02622, 0.08951] Hz, from WIMS user manual rev. 3.22 (was [0.02595, 0.08834] Hz).
  Same fit and pair selection as before (same storm quadrant, A-DWSD outliers excluded), but with
  the storm center and direction of motion from a cubic spline through the best track (11338
  pairs). The taper now reaches 1 at 0.116 Hz instead of 0.114 Hz and is at most 0.016 smaller
  (0.164 instead of 0.172 at 0.050 Hz, 0.656 instead of 0.672 at 0.080 Hz).

## 2026-11-03

Python tools:

- `xmtps-generate-summary-page.py` also writes the summary page's table as `summary.json` next to
  `index.html`: one record per row, with numbers as numbers, ISO 8601 times and `null` for
  missing values (README, [JSON files](README.md#fleet-summary-table-summaryjson)). The page is
  unchanged.

## 2026-11-02

Documentation:

- New [bin/README.md](bin/README.md), describing the two systemd units and the production layout
  under `/data1/alamo/bin`, and [bin/py/README.md](bin/py/README.md), describing every script and
  module in `bin/py`: the data flow from listings to JSON, plots and pages, what each tool reads and
  writes, the plot service loop (`update-plots-service.sh`), the shared modules (`FloatClasses.py`,
  `RhoLUT.py`, `PageStatus.py`). The main README links to both.
- bin/py/README.md is now the reference for the Python tools' command-line options: the shared
  conventions, a usage line, option table and examples for each tool, and the commands the plot
  service runs. The main README's option tables moved there; they had omitted
  `xmtps-aggregate-json.py -b/--bathymetry`.
- Removed unused scripts: `bin/migrate-xfloatmail.sh` and `bin/move-mysql-datadir.sh` (one-off
  database migration and MySQL data directory move, both done), and `bin/py/update-plots.sh`,
  `update-plots2.sh`, `update-plots3.sh` (old debugging, service and rebuild loops) and
  `cp-files.sh` (copied edited scripts from the home directory). Nothing ran them; production keeps
  its own copies of the `update-plots*.sh` scripts.

## 2026-10-31

Python tools:

- WIMS wave spectra (`Wave.PostProcess`): the low-frequency response correction coefficients are
  [f0, w] = [0.02595, 0.08834] Hz (was [0.01445, 0.10457] Hz). Same fit (Erin and Lowell
  colocations up to 0.15 Hz, without floats 9298 and 9310), but using only pairs whose WIMS and
  reference positions are in the same storm quadrant within 600 km of the center, or both more
  than 600 km from it, and excluding A-DWSD reference outliers (spectra with zero energy, or Hs
  more than a factor exp(0.5) from the median of the same drifter within ±3 h); 11335 of 12028
  pairs. The taper now reaches 1 at 0.114 Hz instead of 0.119 Hz. It is up to 0.087 smaller below
  0.089 Hz (0.005 instead of 0.054 at 0.030 Hz, 0.172 instead of 0.259 at 0.050 Hz) and up to
  0.017 larger above. Commit 6e5fc69 briefly used an intermediate fit without the outlier
  exclusion, [0.01778, 0.09946] Hz.

## 2026-10-30

Documentation:

- `spec/` now specifies every file XMTPS and its Python tools read or write, indexed in
  [spec/README.md](spec/README.md): Iridium mail, configuration, float metadata, database, RAW,
  SBD, listing, PHY, KML, status, log and lock files, JSON, and plots and pages. Each ends with the
  discrepancies found between code, README and production.
- The X-message specification is also given as plain text, JSON, YAML and PDF
  (`spec/xmessage-format.{txt,json,yaml,tex,pdf}`). They are generated from
  `spec/xmessage-format.md` by `spec/build-formats.py`, which needs only the Python standard
  library, plus XeLaTeX for the PDF. The JSON and YAML hold the document tree: nested sections
  with number, title and id, and paragraph, list, table, code and block-quote blocks; text keeps
  its inline markdown. Two malformed table rows in the markdown were fixed: 0xF0 offset 34 had an
  extra cell, and the 0xC5 parameter-list row had unescaped `|` inside code.

Decoder:

- XMTPS can run without the database and without the mail server, decoding a directory of
  saved Iridium emails (see README, "Without the mail or SQL server"):
  - New option `sql.enable` (default `1`). With `sql.enable = 0` no connection is made and
    nothing is loaded or saved; floats are built in memory from the messages read in the run.
    `-r`/`-R`, and `mail.moveMessages = 1` with the mail server, are rejected at startup (the
    run-time lock is released); `-A` and the reprocessing implied by `--phy-enable` are skipped.
    The next dive's start-of-dive GPS fix and the KML positions come from the dives read in the
    run, and a message without a serial number is matched by IMEI to a float read in the run.
  - File-system fetch mode (`mail.fetchMode = 1`) reads the files in name order and moves each
    to `dir.mail.processed` once queued (previously it was deleted; set the option empty to
    delete). It needs no mail server settings.
  - Without `-D`, a file-system run reads every file in `dir.mail`, finishes processing them,
    writes the KML and status files once more, and exits, instead of exiting after a few
    seconds.
  - Tested on argus with 161 emails rebuilt from float 9303's production RAW files (dives 157
    and 430–441): the listings of dives 432–441 match production except dive 439, which lacks a
    duplicate gateway email that has no RAW file; dives 430 and 431 lack their earlier dives for
    the pressure offset.

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
