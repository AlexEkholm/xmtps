# Plot and Page File Formats

This document specifies every file the XMTPS Python plot tools (`bin/py/`) write, other than the JSON files: the per-float plot files and float page, the fleet summary page and the two engineering pages. It also specifies the non-JSON files and system information they read (bathymetry, decoder status, geomagnetic model), the plot service that runs them, and where the pages are published.

Status: compiled on 2026-10-30 from the source in `bin/py/` and `README.md`, a local run of `bin/py/xmtps-aggregate-json.py` on float 9303's production dive files, and a read-only inspection of production on `argus.whoi.edu`.

## Contents

- [1. Overview](#1-overview)
- [2. Per-float plot files](#2-per-float-plot-files)
- [3. Plot catalog](#3-plot-catalog)
- [4. Float page: `<plots>/<serial>/index.html`](#4-float-page-plotsserialindexhtml)
- [5. Fleet summary page: `<plots>/index.html`](#5-fleet-summary-page-plotsindexhtml)
- [6. Mission parameters page: `engineering/params.html`](#6-mission-parameters-page-engineeringparamshtml)
- [7. Startup page: `engineering/startup.html`](#7-startup-page-engineeringstartuphtml)
- [8. Page footer and decoder status](#8-page-footer-and-decoder-status)
- [9. Static assets and external scripts](#9-static-assets-and-external-scripts)
- [10. Non-JSON inputs](#10-non-json-inputs)
- [11. The plot service](#11-the-plot-service)
- [12. Publishing locations](#12-publishing-locations)
- [13. Open questions / discrepancies](#13-open-questions--discrepancies)

Related specifications: [json-file-format.md](json-file-format.md) (the per-dive, aggregate and summary JSON files these tools read; column lists are there, not here), [listing-file-format.md](listing-file-format.md) (the listings `xmtps-list2json.py` reads), [log-and-lock-files.md](log-and-lock-files.md), [status-file-format.md](status-file-format.md) and [xmessage-format.md](xmessage-format.md) (the BIT and engineering fields shown on the startup page).

## 1. Overview

The plot tools form a chain. Each run of the plot service (section 11) runs it once.

| Step | Tool | Reads | Writes |
|---|---|---|---|
| 1 | `bin/py/xmtps-list2json.py` | listing files | per-dive JSON |
| 2 | `bin/py/xmtps-aggregate-json.py -p <plots>` | per-dive JSON, bathymetry netCDF | aggregate JSON, `<plots>/<serial>/` plot files and `index.html` |
| 3 | `bin/py/xmtps-generate-summary-page.py` | `<serial>_summary.json`, systemd, `xmtps --version` | `<plots>/index.html`, `<plots>/summary.json` |
| 4 | `bin/py/xmtps-generate-mission-page.py` | `<serial>_summary.json`, `<serial>_mission.json`, systemd, `xmtps --version` | `<plots>/engineering/params.html` |
| 5 | `bin/py/xmtps-generate-startup-page.py` | `<serial>_summary.json`, `<serial>_engineering_data.json`, systemd, `xmtps --version` | `<plots>/engineering/startup.html` |
| 6 | `bin/py/xmtps-generate-energy-page.py` | `<serial>_summary.json`, `<serial>_energy.json`, `<serial>_pump.json`, systemd, `xmtps --version` | `<plots>/engineering/energy.html` |

Production paths:

| Item | Path |
|---|---|
| Plot root `<plots>` | `/data1/alamo/data/plots2/` (also reachable as `/data1/alamo/www`, a symbolic link to `data/plots2/`) |
| JSON root | `/data1/alamo/data/json/` |
| Listing root | `/data1/alamo/data/list/` |
| Tools | `/data1/alamo/bin/py/` (the scripts are symbolic links into the source checkout `/data1/alamo/src/xmtps/bin/py/`) |
| Python | the venv `/data1/alamo/bin/py/.venv` (Python 3.14.4) |

Top level of `<plots>` in production (2026-10-30):

| Entry | Written by | Contents |
|---|---|---|
| `<serial>/` (459 directories) | `xmtps-aggregate-json.py` | one float's plot files and page (sections 2–4) |
| `index.html` | `xmtps-generate-summary-page.py` | fleet summary page (section 5) |
| `summary.json` | `xmtps-generate-summary-page.py` | the summary page's table as JSON (since 2026-11-03; [json-file-format.md](json-file-format.md) section 5.3) |
| `engineering/` | `update-plots-service.sh` (`mkdir -p`) | `params.html`, `startup.html` (sections 6, 7) |
| `img/` | installed by hand (2022-10-03; `s3.png` 2026-11-08) | status icons `s0.png` to `s3.png` (section 9) |
| `js/` | installed by hand (2025-11-18) | `datatables.min.js`, `datatables.min.css`; no page uses them (section 9) |

## 2. Per-float plot files

### 2.1 Directory and file names

`xmtps-aggregate-json.py` writes the plots only with `-p DIR`. `DIR` must exist; the float directory `DIR/<serial>/` is created if missing (`WritePlotFiles`). A float gets a plot directory only if it has at least one SBD message.

Each figure has an ID (`id_str`, e.g. `profile_temp`). `WritePlotImageFiles` writes four files per figure:

| File | Format | Contents |
|---|---|---|
| `<serial>_<id>.html` | HTML | interactive Plotly figure, with legend |
| `<serial>_<id>.png` | PNG | full-size image, without legend |
| `<serial>_<id>_small.png` | PNG | small image, without legend, shown on the float page |
| `<serial>_<id>_thumb.png` | PNG | thumbnail made from the small image, shown in the float page's figure table |

`<serial>` is the WHOI serial number without padding (`9303`, `11010`). Example for float 9303:

```
9303_profile_temp.html
9303_profile_temp.png
9303_profile_temp_small.png
9303_profile_temp_thumb.png
```

The figure metadata also holds `fpath_svg` and `fpath_svg_small` paths, but no SVG file is written. Production directories still hold SVG files from older code (section 2.6).

The directory also holds the float page, `index.html` (section 4). No other file is written there.

### 2.2 Image sizes

Each figure is drawn at one of eight size classes (`opts` in `bin/py/xmtps-aggregate-json.py`). The size class sets the PNG sizes and the HTML plot size. The class is chosen per figure (section 3).

| Size class | Full PNG (px) | Small PNG (px) | Thumbnail (px) | HTML plot size | Float page width |
|---|---|---|---|---|---|
| `fig` | 900 × 900 | 390 × 415 | 150 × 160 | fixed 900 × 900 | half |
| `fig2` | 900 × 900 | 390 × 415 | 150 × 160 | fills the browser window | half |
| `figWide` | 1600 × 900 | 780 × 415 | 150 × 80 | fills the browser window | full |
| `figWideS` | 1600 × 900 | 780 × 315 | 150 × 61 | fills the browser window | full |
| `figWideXS` | 1600 × 900 | 780 × 215 | 150 × 41 | fills the browser window | full |
| `figWideBar` | 1600 × 300 | 780 × 115 | 150 × 32 | fills the browser window | full |
| `figWideL` | 1600 × 900 | 780 × 515 | 150 × 99 | fills the browser window | full |
| `figWideXL` | 1600 × 900 | 780 × 615 | 150 × 118 | fills the browser window | full |
| `figWideXXL` | 1600 × 900 | 780 × 715 | 150 × 138 | fills the browser window | full |

Sizes are width × height. All production PNGs of float 9303 have exactly these sizes (checked 2026-10-30). The full and small images are rendered separately at their own sizes, so a wide small image has a different aspect ratio from its full image. The thumbnail is the small PNG scaled to 150 px wide with Pillow's `thumbnail` (bicubic), keeping its aspect ratio.

### 2.3 Rendering

Order of writing, all from the same Plotly figure object (`WritePlotImageFiles`):

1. HTML, with the legend shown (unless the figure sets `showlegend=False`) and the size class's HTML size.
2. Small PNG, without legend, with margins `l=15, r=15, b=15, t=40`.
3. Thumbnail, from the small PNG.
4. Full PNG, without legend.

PNGs are written by Plotly's `write_image` with kaleido at scale 1, so the pixel size is the requested size. Kaleido drives a headless Chrome (production: Chrome for Testing 142, see README "Versions"). The tool starts one kaleido server for the whole run (`kaleido.start_sync_server`, with `--image-tabs` tabs, default 4) instead of one browser per image. Every export call runs under a watchdog (`--image-timeout`, default 120 s plus 10 s an image). When a call takes longer, the tool kills the Chrome processes it started, replaces the kaleido server and retries the images one at a time, with at most 3 restarts a run. `WritePlotImageFiles` writes the HTML at once and queues the two PNGs; after each plot group, `FlushImageQueue` renders all the group's PNGs in one `plotly.io.write_images` batch spread over the tabs, then writes the thumbnails and records the file sizes. Kaleido does not report a figure that fails within the batch, so afterwards every image whose file is missing or older than the batch is retried alone (logged as a warning: "N of M images not written in one batch"); a figure whose images still can't be written is logged with Kaleido's error and left off the float page. The small and full-size PNG use the same layout (no legend, the small margins) and differ only in size.

The HTML file is a complete page written by `write_html(include_plotlyjs='cdn')`:

- `<meta charset="utf-8" />`; the plot `div` is 900 × 900 px for `fig`, otherwise 100% of the window.
- plotly.js is loaded from the Plotly CDN with a subresource-integrity hash. Production (plotly 7.1.0) loads `https://cdn.plot.ly/plotly-4.1.1.min.js`. The page needs internet access to render.
- Plotly config: `displaylogo: false`, mode bar without the `select2d` and `lasso2d` buttons.
- The figure data are embedded in the page. Files are large: in production, `9303_trajectory.html` is 2.4 MB and `9303_position_all.html` 1.6 MB.

The figure title is `<serial>: <title>`. The float page shows the title without the `<serial>: ` prefix.

### 2.4 Common plot conventions

These hold for all plot groups unless section 3 says otherwise.

- **Dive list.** `dives` is the sorted list of distinct dive numbers in the float's SBD table, including negative (startup) dives.
- **Dive colors.** Per-dive traces are colored along an HSV ramp: hue 0.666 (blue) for the first dive in `dives` down to hue 0 (red) for the last, saturation 0.8, value 1, each RGB component rounded to a multiple of 5 (e.g. `rgb(50, 90, 255)` … `rgb(255, 50, 50)`). The latest dive is drawn in black and wider (line width 2 instead of 1.2). Wave plots index the ramp by dive number from 0 to the largest dive, with that dive black.
- **Per-dive traces** are named by dive number, so the HTML legend lists dives. Hover text shows the dive, the time and the values.
- **Statistics plots** (`*_avg`): one point per dive; a light band for min./max. (`rgb(194,204,204)`), a darker band for ±1σ (`rgb(123,123,123)`) and the mean in black.
- **Count and sum bar charts** use light gray bars (`rgb(220,220,220)`), one per dive.
- **Pressure axes** are reversed (surface at the top).
- **Backgrounds** are white; hover labels use Consolas 14 pt.
- **Fill values** of the listings are NaN before plotting (README "Command-line options"; `FILL_VALUES`), so they are not drawn.

### 2.5 When a plot is written

Each plot group (`PLOT_WRITERS`) runs in its own `try` block and returns no figures when its data is missing (for example no `wave` data, no `pump` data). A failure in one group is logged (`ERROR Failed to generate ... plots for float <serial>`) and the other groups still run, unless `-H` is given. A figure whose files cannot all be written is left out of the float page.

A float whose SBD table has a single dive gets only `cycle_timing`, `trajectory` and `phase_vac`. The dive color ramp divides by `len(dives) - 1`, which is zero, so most groups fail. Production floats 10008 and 10270 show this (section 13).

### 2.6 Stale files in production

The tool never deletes plot files. Production directories hold files the current code no longer writes:

| Stale files | Last written | Origin |
|---|---|---|
| `*.svg`, `*_small.svg`, `*_thumb.svg` | 2025-10-28 (9303); some 2022 | SVG export, since disabled |
| `<serial>_drift_density.*`, `<serial>_drift_pressure.*` | — | older names of `drift_dens`, `drift_pres` |
| `<serial>_satcom_iridium_fix_cep.*` | — | replaced by `satcom_iridium_fix_cep_avg` |
| `<serial>_wave_spectra.*`, `<serial>_wave_spectra_linear.*`, `<serial>_wave_waterfall.*`, `<serial>_wave_waterfall_linear.*` | — | older wave plots, replaced by the `_accel_`/`_disp_` variants |

In production, `9303/` holds 587 files: 99 figures × 4 files, `index.html`, and 190 stale SVG files. The float page links only to current files.

## 3. Plot catalog

The tables list every figure the current code writes, by plot group in `PLOT_WRITERS` order. A local run on float 9303 (a WIMS float with air pump, 483 dives) wrote all 99 of them. Columns:

- **ID**: the `<id>` of the file names.
- **Title**: the figure title (after `<serial>: `).
- **Size**: size class (section 2.2).
- **Page**: the float page section that shows the small image (section 4); "table" means only the Figure Files table lists it.
- **Contents**: axes and traces.

Units in axis titles are as the code writes them; Plotly renders `<sup>`/`<sub>`.

### 3.1 Position (`WriteGeoPlots`, category `position`)

Input: `profile_position`, `gps`, `iridium_position`, `sbd`. Written only if there is `profile_position` data. All four are maps (`Scattergeo`) with land in gray, a dashed gray graticule line for the tropics and polar circles and a darker equator line.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `position_globe` | Profile Positions | `fig` | top | Orthographic globe centered on the float's mean profile position. Black line "Trajectory" through the profile positions; one circle per dive in the dive colors, the latest a star. |
| `position` | Profile Positions and Bathymetry | `fig` | top | `PositionBathymetryFigure`: plain lon/lat axes, latitude scaled by 1/cos(mid-latitude), over a region square in km (the profiles plus 0.6°). Layers: the bathymetry grid (`-b`, section 10.1) as a depth heatmap in whole meters (light to dark blue at sea, plain gray on land, where the depth is 0) whose hover gives the depth at any point (`%{z}m`: "2743m", or "0m" on land); a hillshade (sun at 315°, 45°, 3× vertical exaggeration) at 35% opacity; isobaths every 500 m (1000 m if deeper than 6000 m) and the coastline, unlabeled lines computed once (`ContourLines`); black track; one marker per profile in the dive colors, the latest a star, whose hover adds the grid depth there. Grids over about 600 cells a side are thinned. The PNG images leave out the color bar and the axis tick labels and titles (`static_bare`); the HTML keeps them. Not written if the bathymetry file can't be read; the page then shows `position_geo`. With `-d` the aggregator logs the time to build it and to write its files, and the time of each plot group (DEBUG). |
| `position_geo` | Profile Positions (Map) | `fig` | table | The previous `position` plot (until 2026-11-04): as `position_globe`, Winkel tripel projection zoomed to the float's area. |
| `position_all` | All Positions | `fig` | table | Every position: startup GPS (black star; a startup from a reset in the field, json-file-format.md section 5, is a red "Field reset GPS" star), GPS before descent (×), GPS beacon (star), GPS after ascent (cross), each Iridium message position, and the composite Iridium position of each dive, all in the dive colors; profile positions as large open circles. |
| `position_gps` | GPS Positions | `fig` | Satcom | GPS fixes only: startup GPS (field resets in red), before descent, beacon, after ascent. |

### 3.2 Displacement (`WriteSubmergedDisplacementPlots`, `WriteSurfaceDisplacementPlots`, category `displacement`)

Input: `displacement_submerged`, `displacement_surface`. Polar plots, north up, angle clockwise; one marker per dive in the dive colors.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `displacement_submerged` | Submerged Dispacement | `fig` | table | Azimuth and distance between surfacings; radial axis "Distance [km]". |
| `displacement_submerged_velocity` | Submerged Velocity | `fig` | top | Azimuth and mean speed while submerged; radial axis "Speed [cm s⁻¹]". |
| `displacement_surface` | Surface Dispacement | `fig` | table | Azimuth and distance drifted at the surface; radial axis "Distance [m]". |
| `displacement_surface_velocity` | Surface Velocity | `fig` | top | Azimuth and mean surface drift speed; radial axis "Speed [cm s⁻¹]". |

The titles are spelled "Dispacement" in the code and in production.

### 3.3 Trajectory (`WriteTrajectoryPlots`, category `trajectory`)

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `trajectory` | Trajectory | `figWideL` if the deepest pressure is over 500 dBar, else `figWide` | top | x: time (UTC); y: "Pressure [dBar]", reversed, from the deepest pressure to −10. Lines: Descent (green `rgb(69,255,69)`), Drift (blue `rgb(69,69,255)`), Ascent (red `rgb(255,69,69)`), Surface (gray, lines and markers). Markers at the surface: GPS (magenta `rgb(255,69,255)`), Iridium (cyan `rgb(69,255,255)`). Stepped gray line "Drift Target" (the mission's `DTD<phase>`). Black line "Bathymetry (ETOPO 2022)": the seafloor pressure at each GPS fix (section 10.1); left out if the bathymetry file cannot be read. Only dives 1 and later. |

The function also sets out-of-range pressures to NaN in the aggregate data, which the later plot groups then see (docstring of `WriteTrajectoryPlots`).

### 3.4 Cycle timing (`WriteCycleTimingPlots`, category `cycle_timing`)

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `cycle_timing` | Dive Cycle Duration | `figWideS` | top | Stacked bars per dive; x: "Dive"; y: "Duration [hours]". Descent (`rgb(99,222,99)`), Drift (`rgb(99,99,222)`), Ascent (`rgb(222,99,99)`), Surface (`rgb(122,122,122)`), and a transparent "Dive" bar that carries the total for the hover text [inferred]. |

### 3.5 Profiles (`WriteProfilePlots`, category `profile`)

Input: `profile`, `profile_position` (times). One line per dive in the dive colors.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `profile_temp` | Temperature Profiles | `fig` | Profile | x: "Temperature [°C]"; y: "Pressure [dBar]". |
| `profile_saln` | Salinity Profiles | `fig` | Profile | x: "Salinity [PSU]"; y: pressure. |
| `profile_dens` | In-situ Density Profiles | `fig` | table | x: "In-situ Density Anomaly, ρ - 1000 [kg m⁻³]"; y: pressure. |
| `profile_sigma0` | Potential Density Profiles | `fig` | Profile | x: "Potential Density Anomaly, σ₀ [kg m⁻³]"; y: pressure. |
| `profile_ts` | T-S Profiles | `fig` | Profile | x: "Salinity [PSU]"; y: "Temperature [°C]". |
| `profile_num_bins` | No. Profile Bins | `figWideXS` | Profile | Bars; x: "Dive"; y: "Bins". |

### 3.6 Profile contours (`WriteContourPlots`, category `profile_contour`)

The profiles are regridded onto a 1-dBar pressure grid against profile time. Each figure is one `contour` trace with a horizontal color bar on top; x: time; y: "Pressure [dBar]", reversed. Size `figWideXL` if the deepest pressure is over 300 dBar, else `figWideL`.

| ID | Title | Page | Color scale | Color bar |
|---|---|---|---|---|
| `profile_contour_temp` | Temperature Contour | Profile | `thermal` | Temperature [°C] |
| `profile_contour_saln` | Salinity Contour | Profile | `haline` | Salinity [PSU] |
| `profile_contour_dens` | In-situ Density Contour | table | `dense` | In-situ Density Anomaly, ρ - 1000 [kg m⁻³] |
| `profile_contour_sigma0` | Potential Density Contour | Profile | `dense` | Potential Density, σ₀ [kg m⁻³] |

### 3.7 Ascent and descent (`WriteAscentPlots`, `WriteDescentPlots`, categories `ascent`, `descent`)

Input: `ascent`, `descent`. The descent figures are the same as the ascent figures with `descent` in the ID and title. One line per dive in the dive colors; time is from the start of the series.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `ascent` | Ascent | `fig` | Ascent | x: "Time [minutes]"; y: "Pressure [dBar]". |
| `ascent_rate_v_pres` | Ascent Rate | `fig` | Ascent | x: "Vertical Rate [mBar s⁻¹]"; y: pressure; gray zero line. |
| `ascent_rate_v_time` | Ascent Rate | `fig` | Ascent | x: "Time [minutes]"; y: "Vertical Rate [mBar s⁻¹]". |
| `ascent_rate_avg` | Average Ascent Rate | `fig2` | Ascent | Statistics per dive (section 2.4); y: "Vertical Rate [mBar s⁻¹]". |
| `ascent_sample_count` | No. Ascent Samples | `figWideXS` | Ascent | Bars; y: "Samples". |
| `descent` … `descent_sample_count` | Descent, Descent Rate, Average Descent Rate, No. Descent Samples | as above | Descent | As above. |

### 3.8 Drift (`WriteDriftPlots`, category `drift`)

Input: `drift`. Time series plots have x "Time [minutes]" from the start of the drift and one line per dive with drift data.

| ID | Title | Size | Page | y axis |
|---|---|---|---|---|
| `drift_pres` | Drift Pressure | `fig` | Drift | Pressure [dBar] |
| `drift_temp` | Drift Temperature | `fig` | Drift | Temperature [°C] |
| `drift_saln` | Drift Salinity | `fig` | Drift | Salinity [PSU] |
| `drift_dens` | Drift In-Situ Density | `fig` | Drift | In-situ Density Anomaly, ρ - 1000 [kg m⁻³] |
| `drift_sigma0` | Drift Potential Density | `fig` | table | Potential Density Anomaly, σ₀ [kg m⁻³] |
| `drift_pres_avg` | Average Drift Pressure | `fig2` | Drift | as `drift_pres`; statistics per dive |
| `drift_temp_avg` | Average Drift Temperature | `fig2` | Drift | as `drift_temp` |
| `drift_saln_avg` | Average Drift Salinity | `fig2` | Drift | as `drift_saln` |
| `drift_dens_avg` | Average Drift In-Situ Density | `fig2` | Drift | as `drift_dens` |
| `drift_sigma0_avg` | Average Drift Potential Density | `fig2` | table | "In-situ Density Anomaly, ρ - 1000 [kg m⁻³]" (wrong label, section 13) |
| `drift_sample_count` | No. Drift Samples | `figWideXS` | Drift | Samples (bars) |

### 3.9 Hydraulic system (`WritePumpPlots`, `WriteEngineeringPlots`, category `pump`)

Input: `pump` (`WritePumpPlots`); `engineering_data`, `pump` and `mission` (`WriteEngineeringPlots`). Per-pump plots have one line (or marker set) per dive; "Pump #" is the pump action's index within the dive.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `pump_pres` | Pump Pressure | `fig` | Hydraulic | x: "Pump #"; y: "Pressure [dBar]". |
| `pump_power_v_pres` | Pump Power vs. Pressure | `fig` | Hydraulic | x: "Pressure [dBar]"; y: "Power [W]"; markers. |
| `pump_volt` | Pump Battery Voltage | `fig` | Hydraulic | x: "Pump #"; y: "Voltage [V]". |
| `pump_curr` | Pump Battery Current | `fig` | Hydraulic | x: "Pump #"; y: "Current [A]". |
| `pump_power` | Pump Power | `fig` | table | x: "Pump #"; y: "Power [W]". |
| `pump_energy` | Pump Energy | `fig` | Hydraulic | x: "Pump #"; y: "Energy [J]". |
| `pump_vac` | Internal Vacuum vs. Pump | `fig` | Hydraulic | x: "Pump #"; y: "Vacuum [inHg]". |
| `pump_vac_v_pres` | Internal Vac. vs. Pump Pressure | `fig` | Hydraulic | x: "Vacuum [inHg]"; y: "Pressure [dBar]". |
| `pump_vac_v_time` | Internal Vacuum vs. Pump Time | `fig` | Hydraulic | x: "Cumulative Pump Duration [seconds]"; y: "Vacuum [inHg]". |
| `pump_vac_avg` | Average Internal Vacuum (Pump) | `fig2` | Hydraulic | Statistics per dive; y: "Vacuum [inHg]". |
| `pump_volt_avg` | Average Pump Battery Voltage | `fig2` | Hydraulic | Statistics per dive; y: "Voltage [V]". |
| `pump_curr_avg` | Average Pump Battery Current | `fig2` | Hydraulic | Statistics per dive; y: "Current [A]". |
| `pump_power_avg` | Average Pump Power | `fig2` | table | Statistics per dive; y: "Power [W]". |
| `pump_energy_sum` | Total Pump Energy per Cycle | `figWideXS` | Hydraulic | Bars stacked by pump operation (1 = the dive's first, in listing order), so a bar is the dive total. Each segment is colored by the pressure of its pump operation in the blues of `position` (`DEPTH_BLUES`): light blue at the surface to dark blue at the float's deepest pump pressure, rounded up to 100 dbar; gray if the pressure is invalid; faint gray outlines (none above 150 dives). Color bar "Pump pressure [dbar]" with the surface at the top; no legend. Hover (x unified): each operation's value and pressure, and the dive total. y: "Energy [kJ]". |
| `pump_time_sum` | Total Pump Duration per Cycle | `figWideXS` | Hydraulic | Bars stacked by pump operation, as `pump_energy_sum`; y: "Duration [seconds]". |
| `pump_energy_v_max_pres` | Total Pump Energy vs. Max. Pres. | `fig` | Hydraulic | One marker per dive; x: "Max. Pressure [dBar]"; y: "Energy [kJ]". |
| `phase_vac` | Case Vacuum through the Dive Cycle | `figWide` | Engineering | Case vacuum at the start and end of descent, drift, ascent and surface; x: "Phase" (eight points, "Descent start" … "Surface end"); y: "Vacuum [inHg]"; one line per dive, the latest black. Dashed gray lines at 3 and 7 inHg (`CASE_VAC_LIMITS_INHG`, layout shapes below the traces, no hover). |
| `pump_first_duration` | First Pump Duration and Seek Adjustment | `figWideS` | Hydraulic | Bars "first pump" (duration of each dive's first pump record), dashed blue step "DFT (configured)" (DFT of the dive's mission phase, drawn across the bar of each dive that transmitted its mission configuration and broken at the others: the float sends it only after an upload or when `APT` = 1, and can change its first pump time itself between transmissions), and on a second y axis "Seek adjustment [seconds]" the lines "descent seek adj." (black) and "drift seek adj." (red); x: "Dive"; y: "First pump [seconds]". |

### 3.10 Air pump (`WriteAirPumpPlots`, category `air`)

Input: `engineering_data`, `mission`. Written only if some dive has air pump data.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `air_pump_duration` | Air Pump Duration | `figWideXS` | Air Pump | Gray bars "surface" (`APRunT` per dive); orange markers "BIT test" (`rgb(255,140,0)`); x: "Dive"; y: "Duration [seconds]". The configured `SAO` is in the hover text only. |
| `air_pump_pres` | Air Bladder Pressure | `figWideXS` | Air Pump | Black "surface max." (`AirBladMxP`), red step "SAX (max.)", blue step "SAN (min.)", orange "BIT test" markers; y: "Pressure [PSI]". |

### 3.10.1 Energy (`WriteEnergyPlots`, category `energy`)

Input: `energy` (the `_energy` aggregate, [json-file-format.md 4.17.1](json-file-format.md#4171-_energy)). Written when some dive has a positive total. Added 2026-11-04.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `energy_per_cycle` | Estimated Energy Use per Cycle, Total = … kJ | `figWideS` | Engineering | Dives with a positive dive number only (dive 0 and the startup dives are left out). Bars per dive, stacked bottom up: Pump-descent, Pump-ascent, Air pump, CTD-profile, CTD-drift, CTD-surface, Iridium, GPS, WIMS, Microstructure (MAPLe), Internal Res., Controller, Self-discharge (colors `ENERGY_COLORS`). A component that is 0 in every dive is left out, so WIMS appears only for floats with wave data and Microstructure (MAPLe) only for Micro-ALTOs. Self-discharge (storage), the self-discharge before deployment, is left out of the bars, the title total and the hover total (`ENERGY_PIE_ONLY`); the pie includes it. The hover (`x unified`) gives the dive total on its own line (an invisible `Total` marker trace at the top of each bar), then each component. x: "Dive"; y: "Energy [kJ]". |
| `energy_by_component` | Estimated Total Energy | `figWideBar` | Engineering | One horizontal bar stacked by component (`ShareBarFigure`), each part's length its share (%) of the float total over the same dives as `energy_per_cycle` (colors `ENERGY_COLORS`; components that are 0 in every dive left out); x: "Share [%]". Parts of at least 5% are labeled, horizontally, with the component and kJ; the hover gives the kJ and the share. The subtitle gives the total kJ, the number of dives and their range. A pie before 2026-11-06. |
| `energy_remaining_cycles` | Estimated Remaining Cycles on the Current Mission: N (low–high), about … | `figWideS` | Engineering | After each dive from the third: the cycles left if the float stays on its current mission, (usable energy − energy used so far, storage included) / energy per cycle (`RemainingCycles`). Usable energy is 68% of the pack's nominal energy (`pack_wh`), range 50–95% (`ENERGY_USABLE_FRACTION`, from the floats that ran out of battery; 80%, range 65–95%, before 2026-11-08); the energy and duration of a cycle are the medians of the last 10 dives (`ENERGY_REMAINING_RECENT_DIVES`). The range (shaded) pairs the low usable share with the controller energy +30% and the high share with −30% (`ENERGY_CONTROLLER_UNCERTAINTY`). The title gives the latest estimate and its time (years, or days below a year); the subtitle the share used so far, the pack, and the cycle energy and duration. The hover label on the line gives the dive and the expected, minimum and maximum cycles. A falling pump voltage (`PumpVoltageFallDive`: from the first dive after dive 10 that starts 3 dives in a row whose highest `_pump` voltage is below 95% of its median over dives 1–10; `ENERGY_VOLTAGE_FALL_FRACTION`, `ENERGY_VOLTAGE_FALL_DIVES`, `ENERGY_VOLTAGE_EARLY_DIVES`) is marked by a red dashed line at that dive, labeled with the dive and the early and latest voltages, and the title ends "Pump voltage falling" (since 2026-11-08). Only written when the pack is known. x: "Dive"; y: "Remaining cycles". Since 2026-11-06. |

### 3.11 Surface (`WriteSurfacePlots`, category `surface`)

Input: `surface`. Time series plots have x "Time [seconds]" and one line per dive.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `surface_pres` | Surface Pressure | `fig` | Surface | y: "Pressure [dBar]". |
| `surface_temp` | Surface Temperature | `fig` | Surface | y: "Temperature [°C]". |
| `surface_pres_avg` | Average Surface Pressure | `fig2` | Surface | Statistics per dive, plus blue "pres. correction" (the dive's valid surface pressure offset correction). |
| `surface_temp_avg` | Average Surface Temperature | `fig2` | Surface | Statistics per dive. |
| `surface_sample_count` | No. Surface Samples | `figWideXS` | Surface | Bars; y: "Samples". |

### 3.12 Satcom (`WriteSatcomPlots`, category `satcom`)

Input: `sbd`, `sbd_session`, `gps`, `iridium_position`, summary. Default size `fig2`. "before descent" markers are red (`rgba(255,69,69,0.7)`), "after ascent" blue (`rgba(69,69,255,0.7)`).

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `satcom` | Satcom | `figWideXL` | Satcom | x: date (UTC); y: time of day (UTC), plotted on the dummy date 2021-02-12 with a range from 23:00 the day before to 01:00 the day after. Markers, in the colors of fmtps: "Iridium empty" (empty uploads from `_sbd_session`, yellow `rgba(255,220,0,0.8)`), "Iridium failed" (failed transfers, red `rgba(255,69,69,0.7)`), "Iridium TX" (SBD messages with MTMSN 0, purple `rgba(160,69,220,0.7)`; red until 2026-11-07), "Iridium TX+RX" (MTMSN > 0, i.e. a mobile-terminated message was received, blue `rgba(69,69,255,0.7)`), "GPS" (green `rgb(69,255,69)`). Behind them all, "Surface start": a white circle (size 16, black outline) at the start of each surface phase, `surface.dt_start` of `_engineering_data` (since 2026-11-09); hover gives the dive and time. All the Iridium markers, empty and failed included, are size 16 (the empty and failed ones were 24 until 2026-11-09) and GPS size 8, all with a black 0.5 px outline. The empty and failed markers are drawn before the messages with data, so a failed transfer, minutes before its retry, is mostly hidden under it. Their hover gives the dive, time, position ± CEP, MOMSN and session status. |
| `satcom_gps_ttf` | GPS Time-to-fix (TTF) | `fig2` | Satcom | x: "Dive"; y: "TTF [seconds]". |
| `satcom_gps_cno` | GPS Carrier-to-Noise (CNO) Ratio | `fig2` | Satcom | y: "CNO [dBHz]"; average with min./max. lines. Not written when every fix it shows (`idx` 0–2) has a CNO of 0 (unset); the float page then shows `position_all` in its place. |
| `satcom_gps_nsat` | GPS No. Satellites | `fig2` | Satcom | y: "No. Satellites". |
| `satcom_gps_hdop` | GPS Horiz. Dilution of Precision | `fig2` | Satcom | y: "HDOP". |
| `satcom_gps_fix_cep` | GPS Fix Uncertainty Est. | `fig2` | Satcom | y: "Uncertainty [m]". |
| `satcom_sbd_count` | No. SBD Messages | `figWideXS` | Satcom | Bars; y: "No. Messages". |
| `satcom_sbd_data_size` | SBD Data Size | `figWideXS` | Satcom | Bars stacked by X-message type, from `_sbd_type` (the `SBT` lines), in ID order then `frame` and `unparsed`; fixed color per type (`SBD_TYPE_COLORS`). Bytes of the dive's `SBD` messages beyond its `SBT` total are a `retransmitted` segment. A dive without `SBT` lines (listed before build 3352) is one gray `not broken down` bar of its `SBD` sizes. y: "Size [bytes]". |
| `satcom_sbd_data_type` | SBD Data by Message Type | `figWideBar` | Satcom | One horizontal bar stacked by message type (`ShareBarFigure`), each part's length its share (%) of the total SBD bytes over the dives with a positive dive number and `SBT` lines (dive 0 and the startup dives are left out), plus a `retransmitted` part; colors as `satcom_sbd_data_size`; x: "Share [%]". Parts of at least 5% are labeled, horizontally, with the type and data size (bytes below 10 kB, kB below 10 MB, else MB); the hover gives the bytes and the share. The subtitle gives the bytes, the number of dives and their range. Only written when some dive has `SBT` lines. A pie before 2026-11-06. |
| `satcom_sbd_data_size_avg` | SBD Message Average Size | `fig2` | Satcom | Statistics per dive; y: "Size [bytes]", 0–350. |
| `satcom_iridium_fix_cep_avg` | Iridium Fix Uncertainty | `fig2` | Satcom | Statistics of the message CEPs per dive, plus "composite" markers (the dive's composite CEP); y: "Uncertainty [km]", log scale. |

### 3.13 Waves (`WriteWavePlots`, category `wave`)

Input: `wave`, `wave_spectra`, `profile_position`. Written only for floats with wave data (WIMS floats). Default size `figWide`. Spectra plots have x "Frequency [Hz]" (0.03–0.53) and one line per dive. "accel" plots show the acceleration spectra (g² Hz⁻¹); "disp" plots ("Heave" in the titles) show the displacement spectra (m² Hz⁻¹). IDs without `_linear` have a log y axis; `_linear` IDs a linear one.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `wave_mwm` | Significant Wave Height, Average Period, and Average Direction (Toward) | `figWideXXL` | Wave | Polar plot: angle = mean direction (toward), radius = "Average Period [s]", marker size = significant wave height; one marker per dive in the dive colors; a white "SWH" size key (10 m, 5 m, 2, 1). |
| `wave_swh` | Significant Wave Height | `figWide` | Wave | x: date; y: "H<sub>m0</sub> [m]"; markers in the dive colors. |
| `wave_period` | Average Wave Period | `figWide` | Wave | x: date; y: "Period [s]"; "Average" in the dive colors, "Dominant" and "Zero-crossing" gray. |
| `wave_direction_mean` | Wave Mean Direction Spectra (Toward) | `figWideXL` | Wave | Heatmap: x: date; y: "Frequency [Hz]"; color: mean direction, cyclic color scale. |
| `wave_spectra_accel_Szz`, `_linear` | Wave Vertical Acceleration Spectra (S<sub>ZZ</sub>) | `figWideL` | table | y: "Power Spectral Density (S<sub>ZZ</sub>) [g² Hz⁻¹]". |
| `wave_spectra_accel_Sxx`, `_linear` | Wave East-West Horizontal Acceleration Spectra (S<sub>XX</sub>) | `figWideL` | table | y: S<sub>XX</sub> [g² Hz⁻¹]. |
| `wave_spectra_accel_Syy`, `_linear` | Wave North-South Horizontal Acceleration Spectra (S<sub>YY</sub>) | `figWideL` | table | y: S<sub>YY</sub> [g² Hz⁻¹]. |
| `wave_spectra_accel_Cxy_linear` | Wave Horizontal Acceleration Co-Spectra (C<sub>XY</sub>) | `figWideL` | table | y: "Co-Spectral Density (C<sub>XY</sub>) [g² Hz⁻¹]", symmetric about 0. |
| `wave_spectra_disp_Szz` | Wave Vertical Heave Spectra (S<sub>ZZ</sub>) | `figWideL` | Wave | y: S<sub>ZZ</sub> [m² Hz⁻¹], log. Background: light gray lines of constant acceleration spectral density (5 × 10ⁿ g² Hz⁻¹ converted to displacement, S × g² / (2πf)⁴) labeled "5g²Hz⁻¹", "0.5", "0.05", and a shaded region below 0.0005 g² Hz⁻¹ (the noise-floor mask). |
| `wave_spectra_disp_Szz_linear` | as above | `figWideL` | table | linear y. |
| `wave_spectra_disp_Sxx`, `_linear`; `wave_spectra_disp_Syy`, `_linear` | Wave East-West / North-South Horizontal Heave Spectra | `figWideL` | table | y: S<sub>XX</sub>, S<sub>YY</sub> [m² Hz⁻¹]. |
| `wave_spectra_disp_Cxy_linear` | Wave Horizontal Heave Co-Spectra (C<sub>XY</sub>) | `figWideL` | table | y axis titled "Co-Spectral Density (X<sub>XY</sub>) [m² Hz⁻¹]" (typo, section 13). |
| `wave_waterfall_accel_Szz`, `_linear` | Wave Vertical Acceleration Spectra | `figWideXL` | table | Heatmap: x: date; y: "Frequency [Hz]"; color: S<sub>ZZ</sub> [g² Hz⁻¹], log or linear. |
| `wave_waterfall_disp_Szz` | Wave Vertical Heave Spectra | `figWideXL` | Wave | Heatmap as above, S<sub>ZZ</sub> [m² Hz⁻¹], log color. |
| `wave_waterfall_disp_Szz_linear` | Wave Vertical Heave Spectra | `figWideXL` | table | linear color. |

Wave directions are toward, clockwise from true north when the IGRF declination is available (section 10.3).

### 3.13.1 Microstructure (`WriteMicrostructurePlots`, category `microstructure`)

Input: `microstructure`, `microstructure_bands` (the MAPLe band-averaged spectra of micro-ALTO floats, [json-file-format.md](json-file-format.md) 4.20). Written only for floats with microstructure data (11491-11495). Both figures show the latest dive with a finite spectrum value and a pressure, named in the title with its record times (UTC). Values are in counts, as the MAPLe board sends them: nothing is converted to shear, temperature gradient or dissipation. Records rejected while the pump ran are gaps.

| ID | Title | Size | Page | Contents |
|---|---|---|---|---|
| `microstructure_profile` | Microstructure, Dive N, start to end UTC | `figWideL` | Microstructure | Left: "Signal variance [counts²]" (log x) against "Pressure [dBar]" (reversed), one line per channel (blue: channel 0, orange: channel 1); the x range covers the variances from 10 to 10¹¹ counts² (a failing probe can send 10⁻⁶ or 10¹²), padded by 0.2 decades; channel names in the panel's lower right corner. Then one heatmap per channel: x "&lt;channel&gt; spectrum: frequency [Hz]" (log, band centers), y pressure, color log₁₀ of the band-averaged spectrum [counts² Hz⁻¹], scale `amp` from 1 to 8, one color bar. |
| `microstructure_shear_spectra` | Shear Spectra, Dive N, start to end UTC | `figWideL` | Microstructure | One panel per shear channel (`shear`, `shear1`, `shear2`; left out without one): band-averaged spectra on log-log axes, x "&lt;channel&gt; spectra: frequency [Hz]", y "Spectrum [counts² Hz⁻¹]", one line per record colored by pressure (`Blues`, light = shallow), with a pressure color bar. |

## 4. Float page: `<plots>/<serial>/index.html`

Written by `WriteHtmlFile` in `bin/py/xmtps-aggregate-json.py` after the float's plots, from the in-memory figure metadata. The page is regenerated on every run that processes the float; the figure metadata is not saved anywhere else.

Structure:

```
<!DOCTYPE html><html>
<head><title>9303</title><style>...</style></head>
<body>
  <div class="nav"> fixed navigation links </div>
  <div id="top" class="plots-top-div"><div class="plots-div">
    small images (top group)
    <h2 id="prof">Profile Data</h2> ...
    <h2 id="wave">Wave Data</h2> ...            (only with wave data)
    <h2 id="microstructure">Microstructure Data</h2> ...   (only with microstructure data)
    <h2 id="drift">Drift Data</h2> ...
    <h2 id="surf">Surface Data</h2> ...
    <h2 id="ascent">Ascent</h2> ...
    <h2 id="descent">Descent</h2> ...
    <h2 id="pump">Hydraulic System</h2> ...
    <h2 id="air">Air Pump</h2> ...              (only with air pump figures)
    <h2 id="sat">Satcom</h2> ...
    <h2 id="figs">Figure Files</h2> <table class="figt"> ... </table>
  </div></div>
  <div class="footer"><i>Page last updated 2026-11-04T19:38Z <span class="page-updated" ...>(runtime: 50 seconds)</span><br />Python utility version 0.11<br />XMTPS version 1.0.3351b (built 2026-11-04)<script>...</script></i></div>
</body></html>
```

- **Head.** The title is the serial number. Inline CSS only; no external stylesheet or script; no `<meta charset>`.
- **Navigation.** A fixed pane (125 px) with links to Top, Profile Data, Wave Data (with wave data), Microstructure Data (with microstructure data), Drift Data, Surface Data, Ascent, Descent, Hydraulic System, Air Pump (with air pump figures), Satcom, Figure Files, and "Back" to the absolute URL `/alamo/`.
- **Images.** Each shown figure is `<a href="<serial>_<id>.html"><img class="<size class>" src="<serial>_<id>_small.png" title="<title>" alt="<title>"></a>`. Images of class `fig`/`fig2` have `max-width: 50%` (two per row); wide classes `max-width: 100%`. The content column is at most 805 px wide (`figWide` small width + 25). A `<br>` ends each row.
- **Missing data.** A section without data shows "No profile data.", "No drift data.", "No surface data.", "No ascent data.", "No descent data." or "No hydraulic system data.".

Figures shown in each section, in order (rows separated by `/`):

| Section | Figures |
|---|---|
| top (no heading) | `position_globe`, `position` / `displacement_submerged_velocity`, `displacement_surface_velocity` / `trajectory` / `cycle_timing` |
| Profile Data | `profile_temp`, `profile_saln` / `profile_ts`, `profile_sigma0` / `profile_contour_temp` / `profile_contour_saln` / `profile_contour_sigma0` / `profile_num_bins` |
| Wave Data | `wave_mwm` / `wave_swh` / `wave_period` / `wave_spectra_disp_Szz` / `wave_waterfall_disp_Szz` / `wave_direction_mean` |
| Microstructure Data | `microstructure_profile` / `microstructure_shear_spectra` |
| Drift Data | `drift_pres`, `drift_pres_avg` / `drift_temp`, `drift_temp_avg` / `drift_saln`, `drift_saln_avg` / `drift_dens`, `drift_dens_avg` / `drift_sample_count` |
| Surface Data | `surface_pres`, `surface_pres_avg` / `surface_temp`, `surface_temp_avg` / `surface_sample_count` |
| Ascent | `ascent`, `ascent_rate_v_pres` / `ascent_rate_v_time`, `ascent_rate_avg` / `ascent_sample_count` |
| Descent | `descent`, `descent_rate_v_pres` / `descent_rate_v_time`, `descent_rate_avg` / `descent_sample_count` |
| Hydraulic System | `pump_first_duration` / `pump_pres`, `pump_power_v_pres` / `pump_volt`, `pump_volt_avg` / `pump_curr`, `pump_curr_avg` / `pump_energy`, `pump_energy_v_max_pres` / `pump_time_sum` / `pump_energy_sum` / `pump_vac`, `pump_vac_avg` / `pump_vac_v_time`, `pump_vac_v_pres` |
| Air Pump | `air_pump_duration`, `air_pump_pres` |
| Engineering (only if one of them is written) | `energy_per_cycle` / `energy_by_component` / `energy_remaining_cycles` / `phase_vac` (`ENGINEERING_PAGE_ROWS`; the energy plots were last until 2026-11-09, and the pump vacuum plots moved to Hydraulic System on 2026-11-09) |
| Satcom | `satcom` / `position_gps`, `satcom_gps_cno` (or `position_all`) / `satcom_gps_hdop`, `satcom_gps_nsat` / `satcom_gps_ttf`, `satcom_gps_fix_cep` / `satcom_iridium_fix_cep_avg`, `satcom_sbd_data_size_avg` / `satcom_sbd_count` / `satcom_sbd_data_size` / `satcom_sbd_data_type` |

The Hydraulic System section is shown only if the float has `pump` data, so a float without it shows `pump_first_duration` only in the figure table.

**Figure Files table** (`table.figt`): one row per written figure, sorted by ID.

| Column | Header | Contents |
|---|---|---|
| 1 | (blank) | row number |
| 2 | Title | figure title (HTML, not escaped) |
| 3 | Category | plot category (`position`, `displacement`, `trajectory`, `cycle_timing`, `profile`, `profile_contour`, `ascent`, `descent`, `drift`, `pump`, `air`, `energy`, `surface`, `satcom`, `wave`) |
| 4 | (blank) | the thumbnail, linking to the HTML plot |
| 5 | Files | the ID, then links: `.html (<size> kB)`, `.png [<w>x<h>] (<size> kB)` for the small PNG, `.png [<w>x<h>] (<size> kB)` for the full PNG |

**Footer**: the shared footer of section 8 without the decoder status line, shown in the browser as "Page last updated 2026-11-04T19:38Z (just now, runtime: 50 seconds)", "Python utility version 0.11", "XMTPS version 1.0.3351b (built 2026-11-04)". The runtime is the time `xmtps-aggregate-json.py` spent writing this float's plots and page, not the whole run. Before 2026-11-04 it was one line, `Page last updated <time>Z - version <json_vers>`.

## 5. Fleet summary page: `<plots>/index.html`

Written by `bin/py/xmtps-generate-summary-page.py` from all `<serial>_summary.json` files under the input paths (searched recursively, filtered by `-i`; files with a `json_vers` other than `0.11` are ignored). The page is rebuilt from scratch on every run. The fields read are described in [json-file-format.md](json-file-format.md). The same run writes the table, one record per row, to `<plots>/summary.json` (json-file-format.md section 5.3).

### 5.1 Head

- `<title>Float Summary</title>`; no `<meta charset>`.
- DataTables 1.12.1 with jQuery 3.6.0 from `https://cdn.datatables.net/v/dt/jq-3.6.0/dt-1.12.1/datatables.min.js` and `.../dt-1.12.1/datatables.min.css`.
- Inline script: elapsed times and auto refresh (5.4).
- DataTables setup for `#stable`: sorted by column 8 (Last Report) descending, 10 rows per page, page lengths 10, 20, 50, 100, ALL.
- Inline CSS (`#top` max width 1100 px).

### 5.2 Body

1. Heading `ALAMO/ALTO Float Summary`, and under it the links to the table pages, in `<div id="tlinks">` (`WritePageLinks` in `bin/py/PageStatus.py`, since 2026-11-09; `TABLE_PAGES`): "Summary" (this page, as plain text), "Startups" (`engineering/startup.html`), "Parameters" (`engineering/params.html`) and "Energy" (`engineering/energy.html`), separated by " | ". The paths are relative and assume the production layout of section 11, with the other pages in `engineering/` below the summary page. The startup, mission and energy pages have the same links under their headings, built from `--plots-url` (`/alamo/` and `/alamo/engineering/...` in production).
2. Map (`div#pit`, 390 × 420 px): a Plotly figure embedded as a `div` (`to_html(full_html=False, include_plotlyjs='cdn', div_id='plot')`), so it also loads plotly.js from the Plotly CDN. Orthographic globe centered on 30° N, 45° W; 15°/30° graticule; dashed lines at the tropics and polar circles; equator line. One marker per float at its latest valid GPS position (`latest.gps`), colored by status (`FloatStatus`, as in the table): orange `rgb(255,168,64)` size 6 PREDEPLOYMENT (last seen at WHOI), green `rgb(123,255,123)` size 6 under 30 days since the latest SBD message, blue `rgb(123,123,255)` size 5 under 180 days, red `rgb(255,123,123)` size 4 otherwise. Hover shows the serial. Mode bar hidden. The map includes floats the table leaves out (it does not check the cycle or profiled distance).
3. Table `#stable` (class `stripe`), one row per float, most recent report first. A float is left out if `latest.cycle` < 1 or its `profdepthsum` is not positive. Production on 2026-10-30: 250 rows (40 ACTIVE, 11 OVERDUE, 199 ARCHIVED).
4. Footer (section 8) on the left, and an "Auto Refresh" checkbox on the right.

### 5.3 Table columns

Two header rows: `ID` (rowspan 2); `Summary` (6 columns); `Most Recent Cycle` (4 columns).

| # | Header | Contents |
|---|---|---|
| 0 | ID | serial, linking to the relative URL `<serial>` (the float page directory), and under it the WMO ID in small text if it is above 1 (a separate WMO column until 2026-11-08) |
| 1 | Type | `ALAMO` or `ALTO`, and in small text the CTD type, with `/WIMS` added (`RBR/WIMS`, `SBE/WIMS`, or `WIMS` alone without a CTD type) when the float's `<serial>_wave.json` next to its summary has a measurement with `slen` > 1 (`HasWimsData`; since 2026-11-08) |
| 2 | Firmware | `v<MM>.<m>.<p>` from `fwver` (major two-digit), with the raw `fwver` in small text; `-` if `fwver` has fewer than 10 characters |
| 3 | Deployed [UTC] | date of the deployment GPS fix (`YYYY-MM-DD`), or `-`; attribute `epoch` for the elapsed-time script |
| 4 | Age [days] | `age` / 86400, no decimals; `-` if not positive |
| 5 | Distance Profiled [km] | `profdepthsum` / 1000, one decimal |
| 6 | Status | status icon and text (`FloatStatus` in `bin/py/FloatStatus.py`): `img/s3.png` PREDEPLOYMENT (orange) when the last known position is at WHOI; otherwise from days since the latest SBD message: `img/s2.png` ACTIVE (< 30), `img/s1.png` OVERDUE (< 180), `img/s0.png` ARCHIVED. Under it, in small gray text, the last known location (`LastLocation`, same module): "WHOI" within 25 km of Woods Hole (41.524° N, 70.671° W), else the sea or ocean of the position from `bin/py/SeaNames.py` (since 2026-11-08). The position is the later valid one of `latest.gps` and `latest.sbd` (Iridium). The sea is the smallest Natural Earth marine area (`ne_10m_geography_marine_polys`, public domain, converted to `bin/py/data/marine-areas.json`: 292 named oceans, seas, gulfs, bays, straits and others, coordinates rounded to 0.01°) that contains it, else the area with the nearest vertex (a position on land or between areas). |
| 7 | No. | latest cycle, or `-` if 9999 or more |
| 8 | Last Report [UTC] | time of the latest SBD message (`YYYY-MM-DD HH:MM`); attribute `epoch` |
| 9 | Depth [dBar] | deepest pressure of the latest dive: one decimal under 100, none otherwise; `-` if not in (0, 3000) |
| 10 | Duration [hours] | duration of the latest cycle, one decimal; `-` if not positive |

The Deployed and Last Report cells carry the same `id` (`deployed`, `last-report`) in every row; the script finds them with `querySelectorAll('#deployed')`.

### 5.4 Page script

- **Elapsed times.** On load, each Deployed and Last Report cell gets a `<br><small>` with "just now" (< 2 min), "N minutes ago" (< 3 h), "N hours ago" (< 4 days), "N days ago" (< 2 years) or "N years ago". Cells with negative or over-30-year offsets are left alone.
- **Auto refresh.** URL parameter `r` (seconds): absent or < 1 disables it; 1 means 60 s; values under 10 become 10. The checkbox sets `r=1` (reload every 60 s) or removes `r`. The URL is updated with `history.pushState`.
- **Page size.** URL parameter `n` (since 2026-11-09): the table's page size, 10, 20, 50, 100 or `all`. Absent means 10; any other value gives 10 and is removed from the URL. Choosing a page size in the table writes it to `n`, or removes `n` for 10, with `history.replaceState` (no history entries); other parameters such as `r` are kept, so an auto refresh keeps the page size.
- **Search.** URL parameter `s` (since 2026-11-10): the table's search text, as typed into its search field (DataTables' search across all columns). On load it fills the search field; whatever is typed is written back to `s`, or `s` is removed when the field is empty, with `history.replaceState`. Example: `?s=ACTIVE&n=50`.
- **Shared script.** The auto refresh, page size and search are `TABLE_URL_SCRIPT` in `bin/py/PageStatus.py` (since 2026-11-10), used by all four table pages: `TABLE_PAGING` starts the DataTable from `n` and `s`, and `BindTableUrl` writes changes back. The auto refresh works only on pages with the Auto Refresh checkbox (`WriteAutoRefresh`): the summary and startup pages; elsewhere `r` is ignored.
- **Footer.** The footer's elapsed-time script (section 8).

## 6. Mission parameters page: `engineering/params.html`

Written by `bin/py/xmtps-generate-mission-page.py`. It reads each float's `<serial>_summary.json` and the `<serial>_mission.json` next to it ([json-file-format.md](json-file-format.md)). README "Python tools" describes the rows, columns, phases and colors in full; this section gives the file structure.

- **Head.** `<meta charset="utf-8">`, title "Float Mission Parameters", DataTables 1.12.1 from the CDN (as 5.1), inline CSS (`#top` max width 1600 px). The `table.mtable` is a DataTable sorted by column 3 (Last Report) descending, with the summary page's paging (`TABLE_PAGING` in `bin/py/PageStatus.py`: 10 rows, choices 10, 20, 50, 100 and ALL; until 2026-11-09 without paging or info line), with the search field; the page size and search text follow the URL parameters `n` and `s` (section 5.4; since 2026-11-10). An inline script sets `MCFG` (`ScoringConfig`: each data column's format, the `MEDIAN_COLUMNS` tolerances by column index, `MIN_GROUP_SIZE`, `MIN_SCORE`, `TYPICAL_RANGE_PERCENTILE`) and runs `SCORING_SCRIPT`, which scores the values (below).
- **Heading** `ALAMO/ALTO Mission Parameters`, the links to the table pages (`WritePageLinks`, as 5.2), and an introduction (`div#intro`) with a color key: `mcur` (current phase: bold, underlined), `munused` (light gray), `mscale` (yellow to red gradient), `mscalelow` (white to blue gradient), `merr` (dark red `#b71c1c`, white bold text).
- **One table of all floats** (since 2026-11-09; until then one table per float type, ALAMO and ALTO), most recent report first, under `<h2>Floats <small id="mcount">(N of M floats shown)</small></h2>`. 2026-11-09: 232 floats, 112 ALAMO and 120 ALTO.
- **Header rows.** Row 1: `Float` (colspan 6) and the column groups (Mission, Pressures, Timing, Descent, Ascent, Surface, Ice, Other). Row 2: the six float columns (rowspan 2) and the parameter columns (header, then the parameter name and units in small text; the `title` attribute holds the description). Row 3: `P0`, `P1`, `P2` under each phase-dependent column.
- **Float columns.** ID (link to `<plots-url>/<serial>`, WMO in small text if above 1), Type (with CTD type), Firmware (`vM.m.p` from `0xMMmmppbb`, raw word in small text), Last Report (`YYYY-MM-DD HH:MM`, "N days ago"; sorted by `data-order` epoch, searched by a plain-text `data-search`, as is Status: DataTables searches the HTML of a cell with `data-order`, so the icon path `/alamo/img/...` matched a search for `alamo` until 2026-11-09), Status (as on the summary page, 5.3, through `bin/py/FloatStatus.py` since 2026-11-09: icon `<plots-url>/img/s3.png`, `s2.png`, `s1.png` or `s0.png` with PREDEPLOYMENT/ACTIVE/OVERDUE/ARCHIVED, and the last known location, "WHOI" or the sea or ocean, in small gray text under it; sorted by the latest report), Dive No. (the dive the parameters come from, "of N" if not the latest cycle).
- **Parameter columns.** `COLUMNS` in `bin/py/xmtps-generate-mission-page.py`: `MPS`; `DTD`, `PFD`, `PFM` (per phase), `MDS`, `CTF`, `ASD`; `PTO` (per phase), `SGT`, `SET`, `APT`; `DFT` (per phase), `DCM`, `PSM`; `ARR`, `APM`; `SSP`, `SAM`, `SAO`; `ICM`, `ICT`; `PPE`, `WST` (or `WMT`). Values are integers, as listed, hours (`PTO`, from seconds) or off/on. Each value cell carries `data-c` (its column, the `TableCells` index), `data-s` (0: not compared, for an unset value, a phase the mission does not use or a parameter not sent; 1: counted in the statistics but not colored, for a value out of range (`merr`) and for the `UNCOLORED_COLUMNS`, the mission phase `MPS` (since 2026-11-09); 2: compared), `data-v` (the raw value) and `data-t` (its title without the comparison). The script gives each compared cell its `title` (parameter, value and its assessment) and, for an abnormal value, an inline `background-color: hsl(...) !important`.
- **Table footer.** Row "Most common" (the median for `DFT`, marked "median") and row "Typical range" (10th–90th percentile by nearest rank; one value if both are equal). The page writes placeholder cells (`data-fc`, the column, and `data-ft`, `mode` or `range`) that the script fills.
- **Scoring in the browser** (`SCORING_SCRIPT`, since 2026-11-09). At load, and on every DataTables draw whose searched rows differ from the last ones (so sorting does not recompute), the script takes the values with `data-s` 1 or 2 of the rows shown, in their original order. For each column it computes the most common value (the first one with the highest count, as Python's `Counter`), or for `MEDIAN_COLUMNS` the median, and the typical range. With at least `MIN_GROUP_SIZE` (3) rows shown, it scores each `data-s` 2 value: 1 − (floats with the value) / (floats with the most common value), or for `MEDIAN_COLUMNS` the relative deviation from the median between the tolerated (25%) and full-scale (200%) deviation. A score of at least `MIN_SCORE` (0.05) colors the cell: `hsl(55·(1−s), 100%, 100−35·s %)` above the most common value or median, `hsl(195+25·s, 100%, 100−32·s %)` below; hue and lightness are rounded with ties to even, as the former Python formatting. With fewer rows shown, no cell is colored and the heading says "too few to compare". Searching `ALAMO` or `ALTO` gives exactly the colors, titles and footer of the former per-type tables (checked 2026-11-09).
- **Page footer**: section 8.

Status icon URLs are absolute in production (`/alamo/img/s2.png`) because the service passes `--plots-url /alamo`.

## 7. Startup page: `engineering/startup.html`

Written by `bin/py/xmtps-generate-startup-page.py`. It reads each float's `<serial>_summary.json` and `<serial>_engineering_data.json` (the BIT rows, sections `bit.*`; see [json-file-format.md](json-file-format.md) and [xmessage-format.md](xmessage-format.md) B.2). Floats that started up more than `--days` days ago (default 180; the service does not pass it) are left out. The BITs of the startup dives that the summary lists in `field_resets` (resets in the field, since 2026-11-08) are left out, so a long-deployed float that reset at sea is not shown as a new startup.

- **Head.** `<meta charset="utf-8">`, DataTables 1.12.1 from the CDN; `table#sfloats` sorted by column 3 (Startup) descending, with the summary page's paging (`TABLE_PAGING`; until 2026-11-09 no paging and no info line), search field on; the page size, search text and auto refresh follow the URL parameters `n`, `s` and `r` (section 5.4; since 2026-11-10), with an "Auto Refresh" checkbox at the right of the footer. Inline CSS.
- **Heading** `ALAMO/ALTO Float Startup`, the links to the table pages (`WritePageLinks`, as 5.2); introduction with a key: `bstart` (started, light yellow `#fff3c4`), `bpass` (passed, light green `#c8e6c9`), `bfail` (failed, dark red `#b71c1c`, white bold).
- **Table heading** `<h2>Recent Startups <small>(N)</small></h2>`. Production on 2026-10-30: 65 floats.
- **Header rows.** Row 1: `Float` (7), `BIT` (4), then the measurement groups CTD (3), Case (1), Battery (1), Air Pump (8), Oil Pump (6). Row 2: the column headers with units in small text.
- **No status icons** are used on this page.

Columns:

| # | Header | Contents |
|---|---|---|
| 0 | ID | link to `<plots-url>/<serial>`, WMO in small text |
| 1 | Type | type and CTD type |
| 2 | Status | "Started" (class `bstart`; no completed cycle with a profile) or "Deployed", with "startup N" and "N cycles" in small text |
| 3 | Startup [UTC] | start time of the latest BIT (or its first SBD message), "N days ago"; `data-order` epoch |
| 4 | Deployment [UTC] | `deployment.sbd` time; `-` for started floats |
| 5 | Last Report [UTC] | `latest.sbd` time |
| 6 | BITs | number of BITs sent, "N failed" in small text |
| 7 | Duration [min] | BIT duration |
| 8 | Result | "Passed" (`bpass`) or "Failed" (`bfail`) with the failed tests; `title` gives the results mask as `0x%08X` |
| 9 | Errors | subsystem error flags (`berr`), "none", or `-` if the BIT format has none |
| 10 | Firmware | `vM.m.p`, with the raw word and `BIT v<format>` in small text |
| 11–29 | CTD Pressure [dBar], Temp. [degC], Saln. [PSU]; Case Vacuum [inHg]; Battery Voltage [V]; Air Pump Duration [s], Cycles, Max. Pres. [PSI], currAvg [A], currMax [A], 12VCurrMax [A], batMax [V], powerMax [W]; Oil Pump Duration [s], currAvg [A], currMax [A], 12VCurrMax [A], batMax [V], powerMax [W] | `BIT_COLUMNS`; vacuum and voltage as "start / end"; `-` where the BIT format does not report the value or it is unset |

A float without BIT messages gets `-` in every BIT cell (DataTables does not allow cells that span columns in the body).

The page footer is section 8.

### 7.1 Battery and energy page: `engineering/energy.html`

Written by `bin/py/xmtps-generate-energy-page.py` (since 2026-11-09). It reads each deployed float's `<serial>_summary.json` and, next to it, `<serial>_energy.json` (the energy model, [json-file-format.md](json-file-format.md) 4.17.1) and `<serial>_pump.json` (dive and voltage only). Floats without an energy file, and floats whose `pack_type` is not Tadiran (`PACKS`, since 2026-11-09), are left out; `--days` keeps only floats that reported within that many days. The outlook uses `bin/py/EnergyModel.py`, the same code as the float pages' remaining-cycles plot (3.10.1), so the two agree.

- **Head.** `<meta charset="utf-8">`, title "Float Battery and Energy", DataTables 1.12.1 from the CDN; `table.etable` sorted by column 3 (Last Report) descending, with the summary page's paging (`TABLE_PAGING`) and search field; the page size and search text follow the URL parameters `n` and `s` (section 5.4; since 2026-11-10). Inline CSS.
- **Heading** `ALAMO/ALTO Battery and Energy`, the links to the table pages (`WritePageLinks`), and an introduction explaining the model and the color key: `ebad` (dark red `#b71c1c`, white bold), `ewarn` (light red), `elow` (light yellow). The words "float energy model" link to the energy model page, `energy-model.html` in the same directory (`MODEL_PAGE`, 7.2).
- **One table**, one row per float, most recent report first. 2026-11-09: 149 floats (169 others left out: 108 Saft, 40 alkaline, 21 unknown pack).

| # | Header | Contents |
|---|---|---|
| 0–4 | ID, Type, Status, Last Report [UTC], Dive | as on the mission page (ID with WMO, type with CTD, `FloatStatus` icon and location, last report with "N days ago"), and the latest dive with a positive number |
| 5 | Pack | `pack_type` (Tadiran, Saft, Alkaline), nominal energy in MJ in small text |
| 6 | Age [years] | `battery_age_s` of the latest dive |
| 7 | Startup [V] | `battery_start_v` |
| 8 | Pump [V] | highest oil-pump voltage of the latest dive; in small text its median over dives 1–10, or "falling since N" (`PumpVoltageFallDive`; class `ebad`) |
| 9 | Used [MJ] | energy used so far, storage included (`RemainingCycles` `used_frac` × nominal), the share of the nominal energy in small text; `ebad` from 95 %, `ewarn` from 68 % (`USED_CLASSES`, the high and central usable shares) |
| 10 | Cycle [kJ] | energy of a cycle without storage, median of the last 10 dives |
| 11 | Length [days] | cycle length, median of the last 10 dives |
| 12–15 | Pumps, Sensors, Comms, Standby [%] | shares of all the energy used (`ENERGY_SHARES`): oil and air pumps with the internal-resistance loss; CTD, WIMS and MAPLe; Iridium and GPS; controller, self-discharge and storage |
| 16 | By Component | the float page's Estimated Total Energy plot (`energy_by_component`, 3.10.1): its small image (`<plots-url>/<serial>/<serial>_energy_by_component_small.png`, 780 × 115 px) cropped to its rows 31–78, the bar only, without the title, subtitle, tick labels and x-axis label (`PLOT_CROP_ROWS`; all small images have the bar in rows 32–76), and shown 300 px wide (`PLOT_WIDTH_PX`, 18 px high), linking to the Engineering section of the float page (`<plots-url>/<serial>/#eng`); `loading="lazy"`, hidden if missing; not sortable (since 2026-11-09) |
| 17 | Cycles Left | expected (`remaining_mid`), range in small text |
| 18 | Time Left [years] | cycles left × cycle length, range in small text |

Columns 17 and 18 get `ebad` under 0.25 years expected, `ewarn` under 0.5 years and `elow` under 1 year (`TIME_LEFT_CLASSES`; 0.5, 1 and 2 years before 2026-11-09). ARCHIVED floats (no report in 180 days) are not colored. Cells have `data-order` values for sorting, each with a plain-text `data-search` (DataTables searches a cell with `data-order` as its HTML, where the status icon path `/alamo/img/...` matched every float for "alamo"), and `title` texts with the details. The page footer is section 8.

### 7.2 Energy model page: `engineering/energy-model.html`

A static page (since 2026-11-09), not generated: `bin/py/html/energy-model.html`, with its five figures in `bin/py/html/energy-model/` (`fig1`–`fig5` PNGs). It is installed by copying both into `<plots>/engineering/` (production `/data1/alamo/data/plots2/engineering/energy-model.html` and `energy-model/`) and has to be copied again when it changes; the plot service does not touch it. It describes the energy model (each component's estimate and source, the battery packs, the controller drain and Bluetooth fault, and the remaining-cycles outlook and falling-pump-voltage flag of `EnergyModel.py`), how well the cycles left can be predicted (a backtest of the remaining-cycles estimate on the floats that ran out of battery; since 2026-11-09), and summarizes the report *Energy use of ALAMO/ALTO floats that ran out of battery* (November 2026): the depletion criterion, the lifetime energy, profiling distance and controller drain by battery group, the anomalies (September 2020 ALAMO batch, Micro-ALTOs), the accuracy of each term and the expected lifetime of a new ALTO. Its tables and figures are those of the report; its numbers are not updated with the model's constants. The report itself is `engineering/energy-depletion-report.pdf`, linked from the introduction and the sources. Links at the top go to the four table pages (relative: `../`, `startup.html`, `params.html`, `energy.html`); all CSS is inline, with no scripts.

## 8. Page footer and decoder status

The summary, mission and startup pages, and the float plot pages (without the decoder status line; section 4), share a footer written by `WriteStatusFooter` in `bin/py/PageStatus.py`, inside `<div id="foot"><div id="lfoot"><i>`. Production example (summary page, 2026-10-30):

```
Page last updated 2026-10-30T18:25Z <span class="page-updated" epoch="1793384720" runtime="55 seconds">(runtime: 55 seconds)</span>
<br />Python utility version 0.11
<br />Decoder status: active (uptime: 1 day, 20 hours)
<br />XMTPS version 1.0.3336b (built 2026-10-29)
<script type="text/javascript">...</script>
```

| Field | Source |
|---|---|
| Page last updated | the time the page was written (UTC, to the minute) |
| span `page-updated` | `epoch`: the same time as Unix seconds; `runtime`: `--runtime SECONDS` formatted ("55 seconds", "3 minutes", "3 hours, 4 minutes", "2 days, 5 hours"), empty without `--runtime` |
| Python utility version | the JSON format version (`0.11`) |
| Decoder status | `systemctl show xmtps-alamo --timestamp=unix --property=ActiveState,ActiveEnterTimestamp,StateChangeTimestamp` (no privileges needed): `active (uptime: <ActiveEnterTimestamp to now>)`, or `<state> (for <StateChangeTimestamp to now>)`; `unknown` if systemctl fails |
| XMTPS version | `/data1/alamo/bin/xmtps --version`, matched by `\bv(\S+) \(built (\d{4}-\d\d-\d\d)`; `unknown` if it does not match |

The service name `xmtps-alamo` and the binary path are constants (`DECODER_SERVICE`, `DECODER_BINARY`) in `bin/py/PageStatus.py`. `--runtime` is the time from the start of the service loop's run to the start of the page generators (section 11), so it covers the JSON and plot updates but not the pages.

The footer script (`ELAPSED_SCRIPT`) replaces the span's text on page load with "(N minutes ago, runtime: ...)": "just now" (< 2 min), minutes (< 3 h), hours (< 4 days), days. Without JavaScript the span keeps "(runtime: ...)".

The float page (section 4) uses this footer with `decoder_status=False`, so it has no "Decoder status" line, and its runtime is the time spent writing that float's plots.

## 9. Static assets and external scripts

No page uses a local script or stylesheet; all CSS is inline.

| Asset | Location | Used by | Source |
|---|---|---|---|
| `img/s0.png` | `<plots>/img/` | summary (relative `img/`), mission page (`<plots-url>/img/`) | 7 × 7 px PNG, red `rgb(255,120,120)`: ARCHIVED. Installed on production 2022-10-03; copies of all four icons are in `bin/py/img/` since 2026-11-08. |
| `img/s1.png` | `<plots>/img/` | as above | 7 × 7 px, blue `rgb(120,120,255)`: OVERDUE. |
| `img/s2.png` | `<plots>/img/` | as above | 7 × 7 px, green `rgb(120,255,120)`: ACTIVE. |
| `img/s3.png` | `<plots>/img/` | summary page | 7 × 7 px, orange `rgb(255,168,64)` (`s0.png` recolored): PREDEPLOYMENT. Made 2026-11-08. |
| `engineering/energy-model/fig*.png` | `<plots>/engineering/energy-model/` | energy model page (7.2) | the report's figures, copied from `bin/py/html/energy-model/` (2026-11-09) |
| `engineering/energy-depletion-report.pdf` | `<plots>/engineering/` | energy model page (7.2) | the report *Energy use of ALAMO/ALTO floats that ran out of battery* (LaTeX, not in the repository), copied by hand when it changes (2026-11-09) |
| `js/datatables.min.js`, `js/datatables.min.css` | `<plots>/js/` | none (the code that loads them, `/alamo/js/...`, is commented out) | local copy of DataTables, 2025-11-18 |
| DataTables 1.12.1 + jQuery 3.6.0 | `https://cdn.datatables.net/v/dt/jq-3.6.0/dt-1.12.1/` | summary, mission, startup pages | CDN |
| plotly.js | `https://cdn.plot.ly/plotly-<version>.min.js` (production 4.1.1, from plotly 7.1.0) | every plot HTML file and the summary map | CDN, with an `integrity` hash |

The icon colors are the pixel colors at the icon centers; the shape is not specified [inferred: a dot].

## 10. Non-JSON inputs

### 10.1 Bathymetry netCDF

The trajectory plot (section 3.3) and the `position` plot (section 3.1) read seafloor depth from an ETOPO grid. The file is set with `-b`/`--bathymetry` (aliases `--etopo`, `--etopo2`) of `bin/py/xmtps-aggregate-json.py`; the default is `BATHYMETRY_FILE`.

| Item | ETOPO 2022 (default) | ETOPO2v2c (older, still supported) |
|---|---|---|
| Production file | `/data1/etopo2022/ETOPO_2022_v1_60s_N90W180_surface_classic.nc` (933 MB) | `/data1/etopo2/ETOPO2v2c_f4.nc` (233 MB) |
| Source | NOAA NCEI ETOPO 2022 v1, 60 arc-second surface elevation; NOAA's file `ETOPO_2022_v1_60s_N90W180_surface.nc` (netCDF-4 classic model, 478 MB) is kept next to it | NOAA NGDC ETOPO2v2c, 2 arc-minute, float version |
| netCDF kind (`ncdump -k`) | 64-bit offset | classic |
| Dimensions | `lon` = 21600, `lat` = 10800 | `x` = 10800, `y` = 5400 |
| Coordinates | `double lon(lon)` (degrees_east), `double lat(lat)` (degrees_north) | `float x(x)`, `float y(y)`, range −180..180, −90..90 |
| Elevation | `float z(lat, lon)`, meters, positive up, EGM2008 heights, `_FillValue` −99999 | `float z(y, x)`, `_FillValue` NaN, range −10791..8440 |
| Registration | `node_offset = 1` (cell-centered) | `node_offset = 1` |

`scipy.io.netcdf_file`, which the tool uses, reads only classic-format files (classic or 64-bit offset), not netCDF-4. The classic copy is made with:

```
nccopy -k 64-bit-offset ETOPO_2022_v1_60s_N90W180_surface.nc ETOPO_2022_v1_60s_N90W180_surface_classic.nc
```

(`-k 64-bit-offset` is kind 2; `nccopy -k 2` is the same.)

Sampling (`BathymetryPressure`):

1. On first use, the file is opened memory-mapped and kept open. The coordinate names are `lon`/`lat` if the file has `lon`, else `x`/`y`; the elevation is always `z`. The grid name for the trace label is "ETOPO 2022" if the file name contains `2022`, "ETOPO2" if it contains `ETOPO2`, else the file name.
2. The positions are the GPS fixes of dives 1 and later.
3. Grid spacing is computed from the full coordinate span divided by (n − 1), not from neighboring values.
4. The elevation is interpolated bilinearly between the four surrounding grid points. Longitude wraps at the dateline; latitude is clamped to the grid.
5. Depth = −elevation; points at or above sea level become NaN.
6. Depth is converted to pressure with Saunders (1981): `c1 = (5.92 + 5.25 sin²(lat)) × 10⁻³`, `p = ((1 − c1) − sqrt((1 − c1)² − 8.84 × 10⁻⁶ × depth)) / 4.42 × 10⁻⁶` (dBar).

If the file cannot be read, a warning is logged once (`Failed to read bathymetry file ...`) the trajectory plot is drawn without the bathymetry line, and the `position` plot is not written (the float page shows `position_geo` instead).

### 10.2 Decoder status

The page footers (section 8) run two commands on the host at page generation time:

| Command | Information used |
|---|---|
| `systemctl show xmtps-alamo --timestamp=unix --property=ActiveState,ActiveEnterTimestamp,StateChangeTimestamp` | `ActiveState`; `ActiveEnterTimestamp` (uptime); `StateChangeTimestamp` (time in a non-active state). Timestamps as `@<epoch>` or `Day YYYY-MM-DD HH:MM:SS UTC`. |
| `/data1/alamo/bin/xmtps --version` | version and build date; the command prints and exits without contacting the running daemon |

No status file is read. The decoder's own status files are specified in [status-file-format.md](status-file-format.md).

### 10.3 Geomagnetic declination model

Wave directions (computed by `bin/py/xmtps-list2json.py` through `MagneticDeclination` in `bin/py/FloatClasses.py`, and plotted in section 3.13) are corrected from magnetic to true north with the IGRF model of the `ppigrf` package (production: ppigrf 2.1.0). The model coefficients are files inside the package: `ppigrf/IGRF14.shc` (the default) and `IGRF13.shc`. The declination is computed at the dive's GPS fix and time, at altitude 0. Without `ppigrf`, directions stay magnetic and `direction_ref` is `magnetic` in the JSON. No separate data file is installed.

### 10.4 Density lookup table

`bin/py/RhoLUT.py` is a Python module, not a data file. It embeds two tables, `rho_lut` (in-situ density) and `sigma0_lut` (potential density referenced to the surface), as density − 1000 kg/m³ on the grid salinity 0–39 PSU (step 1), temperature −5–39 °C (step 1), pressure 0–2900 dBar (step 100), indexed `[S][T][P]`. `GetRhoFromLut` and `GetSigma0FromLut` interpolate trilinearly (`scipy.interpolate.RegularGridInterpolator`), return `None` outside the grid (pressures below 0 are treated as 0) and round to 3 decimals. It is used by `xmtps-list2json.py` for the `dens` and `sigma0` values that the profile and drift plots show.

### 10.5 Listing files

`xmtps-list2json.py` reads the decoder's listing files; see [listing-file-format.md](listing-file-format.md) and [json-file-format.md](json-file-format.md).

## 11. The plot service

### 11.1 systemd unit

| Item | Value |
|---|---|
| Unit | `/etc/systemd/system/xmtps-alamo-plot.service`, "XMTPS Decoder Plot Generation Service" |
| Command | `ExecStart=/data1/alamo/bin/py/update-plots-service.sh` (symbolic link to `bin/py/update-plots-service.sh` in the source checkout) |
| User, group | `aekholm`, `floatgroup` |
| Restart | `always`, after 30 s |
| `PIDFile` | `/data1/alamo/var/lock/xmtps-plot.pid` (the script does not write it; section 13) |
| Logs | standard output and error to the journal, identifier `update-plots-service.sh` (`journalctl -u xmtps-alamo-plot` or `journalctl -t update-plots-service.sh`) |

### 11.2 Loop (`bin/py/update-plots-service.sh`)

The script activates the venv `/data1/alamo/bin/py/.venv` and loops forever:

1. Log `<ISO time> --- start ---`.
2. Select the floats with recent listings:

   ```
   find /data1/alamo/data/list/ -cmin -14400 -type f | awk -F/ '{print $6}' | sort -nu
   ```

   A float is "recent" if any file in its listing directory `/data1/alamo/data/list/<serial>/` had its status changed (ctime) within the last 14400 minutes (10 days). Field 6 of the path is the serial directory.
3. For each recent float, in the background:

   ```
   python xmtps-list2json.py -v -i"$DLIST/(90|91|92|93|94|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" \
       -x"/\..*" -o"$DJSON" "$DLIST/<serial>"
   python xmtps-aggregate-json.py -v -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" \
       -o"$DJSON" -p"$DPLOT" "$DJSON/<serial>/dives"
   ```

   with `DLIST=/data1/alamo/data/list`, `DJSON=/data1/alamo/data/json`, `DPLOT=/data1/alamo/data/plots2`. A new float is started only while at most `MAX_PROC` (8) child processes run.
4. Wait for the children. If the run has lasted more than `MAX_RUN_T` (3600 s), log "Run time exceeded. Killing children..." and kill the child processes.
5. Compute the run time so far, then write the three pages:

   ```
   python xmtps-generate-summary-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT" --runtime <s> $DJSON
   mkdir -p "$DPLOT/engineering"
   python xmtps-generate-mission-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" \
       --plots-url /alamo --runtime <s> $DJSON
   python xmtps-generate-startup-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" \
       --plots-url /alamo --runtime <s> $DJSON
   python xmtps-generate-energy-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" \
       --plots-url /alamo --runtime <s> $DJSON
   ```

6. Log `<ISO time> --- done, run time = <s> ---`, sleep 300 s.

Production on 2026-10-30: a run takes about 55–60 s, so the pages are rebuilt about every 6 minutes.

### 11.3 State the service relies on

The service itself writes no state file. Repeated runs are cheap because of the tools' own checks:

| State | Kept by | Rule |
|---|---|---|
| Per-dive JSON modification time | `xmtps-list2json.py` | a listing is converted only if its dive JSON is missing or older than the listing |
| `<json>/<serial>/dives/.lastprocessed` | `xmtps-aggregate-json.py` | a float is skipped unless a dive JSON file is newer than the Unix time in this file (e.g. `1793249363.0885403`); the file is written after all output, so a failed run is retried |

Both are specified in [json-file-format.md](json-file-format.md). Because the aggregate run skips unchanged floats, a float's plots and page are rewritten only when a new or changed listing arrives. The three fleet pages are rewritten on every run.

### 11.4 Log lines

The `-v` options make the converters log at INFO level, with the format `<YYYYmmddTHHMMSS> <LEVEL> <message>`. Typical warning and error lines:

```
WARNING No drift data for float 9281
WARNING No profile data for float 11778
ERROR Failed to submerged displacement plotes for float 9326
ERROR Failed to generate ascent plots for float 10270
WARNING Failed to read bathymetry file ...: ...
```

### 11.5 Other scripts

`bin/py/update-plots.sh` (a debugging run for float 9300), `update-plots2.sh` (an older one-pass service loop), `update-plots3.sh` (a one-off full rebuild) and `cp-files.sh` (which copied scripts from the home directory, adding a `#!/usr/bin/python3` line and removing CRs; it is why `FloatClasses.py`, `xmtps-aggregate-json.py`, `xmtps-list2json.py` and `xmtps-generate-summary-page.py` start with two identical `#!` lines) were removed from the repository on 2026-11-02; nothing ran them. Production `/data1/alamo/bin/py/` still holds its own copies of them, which are regular files rather than links to the checkout, and scripts never in the repository (`update-all-plots.sh`, `update-plots4.sh`, `monx-update-plots.sh`, which follows the service journal), plus backups of earlier versions.

## 12. Publishing locations

| Content | Production directory | URL |
|---|---|---|
| Fleet summary | `/data1/alamo/data/plots2/index.html` | `https://argo.whoi.edu/alamo/` |
| Float page | `/data1/alamo/data/plots2/<serial>/index.html` | `https://argo.whoi.edu/alamo/<serial>/` |
| Plot files | `/data1/alamo/data/plots2/<serial>/<serial>_<id>.*` | `https://argo.whoi.edu/alamo/<serial>/<serial>_<id>.*` |
| Engineering pages | `/data1/alamo/data/plots2/engineering/` | `http://argus.whoi.edu/etcetc/alamo/engineering/params.html`, `.../startup.html` |

How the directories are served:

- On argus, Apache's `DocumentRoot` is `/data1/www`. `/data1/www/alamo` → `/data1/alamo/www` → `data/plots2/`, so the plots are also at `http://argus.whoi.edu/alamo/`.
- `/data1/www/etcetc/alamo/engineering` is a symbolic link to `/data1/alamo/data/plots2/engineering`. The same `etcetc/alamo/` directory also links `json`, `list`, `mat`, `netcdf` and `plot` (the old plots directory).
- `argo.whoi.edu` is an alias of `jason.whoi.edu`, a different host (Apache on Debian). On 2026-10-30 18:26 UTC it served the summary page written on argus at 18:13, while argus already had the 18:25 version, so it serves a delayed copy [inferred: a periodic mirror; the mechanism is not in this repository].
- Links between the pages: the summary links floats relatively (`<serial>`); the float page's Back link is the absolute `/alamo/`; the engineering pages link floats and icons under `/alamo` (`--plots-url /alamo`). On argus, `/alamo/` resolves to the same plot directory, so the engineering pages work there too.

## 13. Open questions / discrepancies

Bugs found in the code:

1. **Single-dive floats get almost no plots.** The dive color ramp divides by `len(dives) - 1` (`colorsys.hsv_to_rgb(x*.666/(len(dives)-1), ...)`), which raises `ZeroDivisionError` when the SBD table has one dive. The geo, displacement, profile, contour, ascent, descent, drift, pump, surface and satcom groups fail; only `cycle_timing`, `trajectory` and `phase_vac` are written. Confirmed locally on float 10270 (one dive, 21140) and seen in production for 10008 and 10270. `WriteEngineeringPlots` guards with `max(n-1, 1)`; the others do not.
2. **Wrong error text for surface displacement.** `WriteSurfaceDisplacementPlots` logs "Failed to submerged displacement plotes for float ..." (the submerged text, with "plotes"), so a surface displacement failure looks like a submerged one. Production logs show this for float 9326.
3. **`drift_sigma0_avg` y-axis label** reads "In-situ Density Anomaly, ρ - 1000" although it plots σ₀.
4. **`wave_spectra_disp_Cxy_linear` y-axis label** reads "Co-Spectral Density (X<sub>XY</sub>)"; the acceleration version reads C<sub>XY</sub>.
5. **Titles** "Submerged Dispacement" and "Surface Dispacement" are misspelled.
6. **SWH key labels** in `wave_mwm` are "10m", "5m", "2", "1" (units only on the first two).
7. **Summary page firmware column** takes the patch number from `fwver[7:8]` (the low hex digit of the patch byte) and formats the major version with two digits (`v07.3.5`); the mission and startup pages use the whole byte `fwver[6:8]` and no padding (`v7.3.5`). Patch numbers above 15 are shown wrong on the summary page.
8. **Map vs. table on the summary page.** The map shows every float with a valid latest GPS fix; the table leaves out floats without a completed cycle or profiled distance. The map's unused default color string is misspelled `'rbg(0,0,0)'` (never used, as every branch sets a color).
9. **Duplicate element IDs.** The summary table gives every Deployed cell `id="deployed"` and every Last Report cell `id="last-report"`. Browsers tolerate it; it is not valid HTML.
10. **No character set** is declared on the float page or the summary page, and argo.whoi.edu sends `Content-Type: text/html` without a charset. The pages are almost all ASCII, but figure titles are inserted unescaped and the CSS/JS are inline; any non-ASCII text would depend on the browser's guess.
11. **Unused metadata.** The figure metadata carries `fpath_svg` and `fpath_svg_small` for files that are not written, and `ts_pos` of `WritePlotImageFiles` is unused.

Code vs. production:

12. **Stale plot files** (section 2.6): SVG files and old plot names stay in production because nothing removes files. They are not linked, but are served.
13. **Unused local DataTables copy** in `<plots>/js/`: all pages load DataTables from `cdn.datatables.net`. The pages therefore need internet access to sort and search.
14. **`PIDFile=/data1/alamo/var/lock/xmtps-plot.pid`** in the systemd unit is never written (the directory holds only the decoder's `xmtps.pid`). With `Type=simple` systemd does not need it, but the setting is misleading.
15. **Status icons** `img/s0.png`–`s3.png` are served from `<plots>/img/`, which the tools do not install: a fresh install must copy them from `bin/py/img/`.
16. **Bathymetry comment.** `BathymetryPressure` says the coordinates are float32; that holds for ETOPO2v2c, but ETOPO 2022's `lat`/`lon` are double. The code works for both. ETOPO 2022's `_FillValue` (−99999) is not masked [inferred: the global surface grid has no fill cells].

Code vs. README and docstrings:

17. **Listing include pattern.** The service converts only listings of serials starting with 908, 909, 91, 92, 93, 94, 10 or 11; README's example uses `[0-9]+`. Production has 66 listing directories outside the pattern (9003–9079, and 4790, 6796, 7127, 9511, 9672), none with files changed in the last 10 days; the pattern keeps the service from converting them. A future float with another prefix (for example 95xx or 12xxx) would be silently skipped.
18. **Footer wording.** Fixed 2026-11-04: the `bin/py/PageStatus.py` docstring now shows the four lines the code writes. README says the *summary* page footer shows the decoder status; all three fleet pages show it.
19. **README "Command-line options"** does not list `-b`/`--bathymetry` of `xmtps-aggregate-json.py` (it is described only in the "Versions" table).
20. **Run-time limit.** The service kills only its direct children (the background subshells) after `MAX_RUN_T`; whether the Python processes they started are also stopped is not handled by the script [inferred: they keep running until they finish]. The limit is checked only after all floats have been started.
21. **Process limit.** The count `ps --no-headers -o pid --ppid=$PID | wc -w` also counts the subshell of the command substitution itself [inferred], and the loop allows up to `MAX_PROC` (8) counted processes, so about 7–8 floats run in parallel, not 8 or 9.

Open questions:

22. How `argo.whoi.edu/alamo/` is kept in sync with argus (mirror, proxy cache or shared storage) is not documented here (section 12).
