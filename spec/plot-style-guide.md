# Float Plot Style Guide

This guide sets the conventions for the per-float plots that `bin/py/xmtps-aggregate-json.py` writes: hover text, titles, sizes, layouts, axes, colors, line and marker styles, color bars, and the code pattern of a plot writer. It is meant to be complete enough that someone who has never seen the plots can add one that matches them. Follow it for every new plot, and bring an old plot in line whenever you change it. [plot-and-page-files.md](plot-and-page-files.md) describes the plots that exist; this guide describes how they should look and be built.

Status: adopted 2026-11-13. All hover text in `xmtps-aggregate-json.py` was brought in line then (section 14 lists what changed).

## Contents

1. [Symbols and units](#1-symbols-and-units)
2. [Hover text](#2-hover-text)
3. [Figures, titles and sizes](#3-figures-titles-and-sizes)
4. [Axes](#4-axes)
5. [Colors, lines and markers](#5-colors-lines-and-markers)
6. [Hover modes](#6-hover-modes)
7. [Plot families](#7-plot-families)
8. [Missing data and empty plots](#8-missing-data-and-empty-plots)
9. [Writing hover templates in Plotly](#9-writing-hover-templates-in-plotly)
10. [Code structure of a plot group](#10-code-structure-of-a-plot-group)
11. [What `WritePlotImageFiles` does to every figure](#11-what-writeplotimagefiles-does-to-every-figure)
12. [Checklist for a new plot](#12-checklist-for-a-new-plot)
13. [Known inconsistencies in existing plots](#13-known-inconsistencies-in-existing-plots)
14. [Changes made when the guide was adopted](#14-changes-made-when-the-guide-was-adopted)

## 1. Symbols and units

Use these symbols in hover text, and the same quantity names and units on the axes. Write the symbol, a space, `=`, a space, the value, a space and the unit: `p = 1002.4 dBar`.

| Quantity | Symbol | Unit | Format | Example | Notes |
|---|---|---|---|---|---|
| Time (a moment) | none; the timestamp has its own line | UTC | `YYYY-MM-DD hh:mm:ssZ`; `YYYY-MM-DD hh:mmZ` for GPS times | `2026-11-12 08:21:12Z`, GPS `2026-11-12 08:21Z` | Date, a space, the time and `Z`. Times taken from a GPS fix (GPS fixes, profile positions, displacements, waves) have no seconds, so they are shown to the minute; all other times (samples, Iridium sessions, surface start) with seconds. Write `t = …` only when a line needs a label, for example two times in one hover. |
| Time interval since the start of the phase or another reference time | Δt | s, min or hours | as the axis | `Δt = 37.5 min` | Use the time axis's unit: s for the surface phase, min for the ascent, descent and drift, hours for whole cycles. |
| Pump or air-pump run time (duration of one run) | t | s | `.0f` | `t = 46 s` | Lower-case t, like a time, because it's the run's own duration, not an interval from a reference. It takes Δt's place in the line order. A qualifier goes after the unit: `t = 46 s (first pump)`, `t = 120 s (BIT test)`. |
| Cumulative pump time | Σt | s | `.0f` | `Σt = 412 s` | Total pump time so far in the dive. |
| Pressure | p | dBar | `.1f`; `.2f` for surface pressure data; `.0f` for regridded pressures (the regridded contour values S, T, ρ, σ<sub>Θ</sub> show 2 decimals) | `p = 1002.4 dBar`, surface `p = 0.37 dBar`, contour `p = 1002 dBar` | Lower case p. Two decimals only in the surface plots (`WriteSurfacePlots`: surface pressure and its statistics, the pressure correction), where the values are fractions of a dBar. No decimals where the data were interpolated onto a pressure grid (the profile contours), since the grid pressure is not a measured one. Statistics of pressure (min, mean, max, σ) use the same number of decimals. |
| Salinity | S | PSU | `.3f` | `S = 34.712 PSU` | Capital S. |
| Temperature | T | °C | `.3f` | `T = 3.981 °C` | Capital T. Never lower case t, which is time. The Θ in ρ<sub>Θ</sub> and σ<sub>Θ</sub> marks potential density, not a temperature; bearings are lower-case θ. |
| Period (waves) | T, with a subscript for a named period | s | `.1f` (`.0f` for the peak period) | `T = 12.5 s`, `T<sub>avg</sub> = 8.3 s` | The same letter as temperature; the unit (s, not °C) tells them apart, and the two never appear in the same hover. In wave hovers the period follows the frequency it belongs to (`f = …`, then `T = …`), not the p, S, T order. Named periods: T<sub>peak</sub>, T<sub>avg</sub>, T<sub>0</sub>. |
| In-situ density anomaly | ρ − 1000 | kg m<sup>−3</sup> | `.2f` | `ρ − 1000 = 32.41 kg m⁻³` | Only in the hovers of in-situ density plots (`profile_dens`, `profile_contour_dens`, `drift_dens`, `drift_dens_avg`), where it is the plotted quantity. Written `ρ − 1000`, as on the axes, because σ alone is the standard deviation in statistics hovers. |
| Potential density | ρ<sub>Θ</sub> | kg m<sup>−3</sup> | `.2f` | `ρ<sub>Θ</sub> = 1027.80 kg m⁻³` | Prefer the anomaly σ<sub>Θ</sub>; use ρ<sub>Θ</sub> only where the full value is needed. The plots don't use it. |
| Potential density anomaly | σ<sub>Θ</sub> | kg m<sup>−3</sup> | `.2f` | `σ<sub>Θ</sub> = 27.80 kg m⁻³` | ρ<sub>Θ</sub> − 1000. The plots carry this (`sigma0` in the data), so hovers show σ<sub>Θ</sub>. |
| Vertical rate | dp/dt | mBar s<sup>−1</sup> | `.2f` | `dp/dt = −98.40 mBar s⁻¹` | Positive downward (pressure increasing). |
| Voltage | V | V | `.2f` | `V = 14.72 V` | |
| Current | I | A | `.3f` | `I = 0.412 A` | |
| Power | P | W | `.2f` | `P = 6.05 W` | |
| Energy | E, ΣE (sum) | J, kJ | `.1f` J, `.3f` kJ | `E = 1240.2 J` | |
| Case vacuum | vac | inHg | `.2f` | `vac = 7.41 inHg` | |
| Frequency | f | Hz | `.3f` | `f = 0.125 Hz` | |
| Bearing, direction | θ, with a subscript for a named direction | ° | `.0f` | `θ = 245°`, `θ<sub>1</sub> = 80°`, `θ<sub>mean</sub> = 92°` | Degrees clockwise from north, 0–360, no space before the °. Lower-case θ; the capital Θ appears only as the subscript of ρ<sub>Θ</sub> and σ<sub>Θ</sub>. Displacement bearings are θ; wave directions are θ<sub>1</sub> (per frequency) and θ<sub>mean</sub>, in the direction the waves travel toward. |
| Significant wave height | H<sub>m0</sub> | m | `.2f` | `H<sub>m0</sub> = 1.84 m` | |
| Position uncertainty (CEP radius) | CEP | m (GPS), km (Iridium) | `.0f` | `CEP = 12 m` | Iridium positions give it after the position as `± N km` instead. |
| GPS time to fix | TTF | s | `.0f` | `TTF = 34 s` | |
| Counts | none; the count and its noun | | `.0f` | `142 samples`, `12 bins` | |
| Latitude, longitude | none | °N, °E | `.4f`; `.3f` for Iridium positions | `41.5290°N −70.6702°E`; Iridium `41.529°N −70.670°E ± 3 km` | Always decimal degrees, never degrees and minutes. Signed, north and east positive, on one line, latitude first. 4 decimals (about 10 m) for GPS fixes; 3 for Iridium positions, whose uncertainty (CEP radius) follows as `± N km`. |

Typography:

- Use the real characters, not their names: Δ, Σ, ρ, σ, °, ±, and U+2212 (−) for minus signs and negative exponents in units. Plotly fonts render them, and so does the Consolas hover font.
- Write exponents and subscripts with HTML: `kg m<sup>−3</sup>`, `σ<sub>Θ</sub>`.
- Spell pressure units `dBar` and `mBar`, as the rest of the plots and listings do.
- Write a missing value as `-` (preformatted custom data) or leave the line out; never show `nan`.

## 2. Hover text

### 2.1 Layout

Hover text for a sample of a dive has one item per line, in this order, leaving out lines that don't apply:

```
Dive 123: Ascent
2026-11-12 08:21:12Z
Δt = 37.5 min
p = 1002.4 dBar
S = 34.712 PSU
T = 3.981 °C
σ<sub>Θ</sub> = 27.80 kg m⁻³
(other quantities: dp/dt, V, I, P, E, vac, …)
```

1. **Dive and phase.** `Dive <b>N</b>: <Phase>`, with the dive number in bold. The phase is one of `Descent`, `Park/Drift`, `Ascent`, `Surface` and `Profile` (binned ascent profiles and their contours). Leave the phase out when the data have none, for example per-pump records, or when the plot spans the whole cycle (cycle timing, satcom, positions).
2. **Timestamp** of the sample, in UTC.
3. **Δt** since the start of the phase or the plot's reference time; or **t**, a pump run's duration; then **Σt**, cumulative pump time.
4. **p, S, T, σ<sub>Θ</sub>**, in that order, for whichever of them the data have. In-situ density plots add `ρ − 1000` before σ<sub>Θ</sub>; no other plot shows in-situ density. Prefer the anomalies (ρ − 1000, σ<sub>Θ</sub>) over full densities. Show them even when one of them is the plotted axis value: the hover then repeats the axis, which is fine.
5. **Other quantities** after them, in the order that best explains the plot. Put the plotted quantity first among them.

End every template with `<extra></extra>` so that Plotly doesn't add the trace name in a second box (for the component traces of `x unified` stacked bars, see section 7.5). The trace name (the dive number) is already in the first line.

### 2.2 Statistics plots

Plots with one point per dive (`*_avg`, counts, sums) start with the dive, then the statistics, each with its unit:

```
Dive 123: Ascent
min = −110.40 mBar s⁻¹
mean = −98.40 mBar s⁻¹
max = −91.20 mBar s⁻¹
σ = 3.12 mBar s⁻¹
```

Each statistic uses its quantity's format from section 1. Here σ is the standard deviation. It's the one place σ appears without a subscript, so don't confuse it with σ<sub>Θ</sub>.

### 2.3 Hover label style and mode

- Font: `Consolas, monospace`, 14 pt (`hoverlabel=dict(font=dict(family='Consolas, monospace', size=14))`). Crowded unified hovers (stacked bars) may use 12 pt.
- The hover mode depends on the plot kind (section 6).
- Lines that are references or ranges (zero lines, min./max. segments, the coast, the track) get `hoverinfo='none'` or `'skip'`, so they never cover the data.

## 3. Figures, titles and sizes

### 3.1 How a plot gets written

Every float plot is a Plotly figure that a `Write*Plots(float_id, float_aggregate_data, dpath_float, dives, max_dive)` function builds and hands to `WritePlotImageFiles` (section 11). That writes one HTML page and three PNGs: full, small and thumbnail. The float page `index.html` shows the small PNG and links it to the HTML. So every plot must read well as a 780 × 415 (or smaller) PNG with no legend and no hover.

### 3.2 Titles

- The figure title is `'<serial>: <Title>'.format(float_id)`, in title case: `9303: Ascent Rate`. Set it with `title=dict(text=…)` and nothing else: no font size, no `x`. The float page strips the leading `<serial>: ` (only that prefix, so a title may contain further colons).
- A subtitle goes on a second line in `<sup>`: `'{}: Estimated Total Energy<br><sup>1,521.5 kJ in 482 dives (1–483)</sup>'`. Keep it to one short line, because the PNGs leave only a 40 px top margin. A plot with a subtitle uses an HTML top margin of 70 (section 3.4).
- Ranges in titles use an en dash: `(1–483)`, `50–95%`. Thousands get a comma: `1,521.5 kJ`.
- Name the plot after what it shows, as the existing ones do: `Ascent Rate`, `Average Ascent Rate`, `No. Ascent Samples`, `Drift Potential Density`, `Profile Positions (Map)`.

### 3.3 Size classes

Pick a size class (`size=` argument of `WritePlotImageFiles`, from the `opts` dict) by the plot's shape:

| Class | Full PNG | Small PNG (on the float page) | HTML | Use for |
|---|---|---|---|---|
| `fig` | 900 × 900 | 390 × 415 | 900 × 900 fixed | Profiles vs pressure, per-dive time series, maps, polar plots, marker plots |
| `fig2` | 900 × 900 | 390 × 415 | fills the window | Per-dive statistics (`*_avg`), GPS per-dive plots |
| `figWide` | 1600 × 900 | 780 × 415 | fills the window | Wide series: trajectory (shallow), wave height/period, `phase_vac` |
| `figWideS` | 1600 × 900 | 780 × 315 | fills the window | Stacked bars per dive (cycle timing, energy per cycle), remaining cycles |
| `figWideXS` | 1600 × 900 | 780 × 215 | fills the window | Count and sum bars per dive, sample counts |
| `figWideBar` | 1600 × 300 | 780 × 115 | fills the window | A single horizontal share bar |
| `figWideL` | 1600 × 900 | 780 × 515 | fills the window | Deep trajectory (deeper than 500 dBar), contours to 300 dBar, spectra |
| `figWideXL` | 1600 × 900 | 780 × 615 | fills the window | Contours deeper than 300 dBar, waterfalls, satcom time of day |
| `figWideXXL` | 1600 × 900 | 780 × 715 | fills the window | Large polar plots (wave mean direction) |

Thumbnails are the small PNG scaled to 150 px wide. Two square plots sit side by side on the float page; a wide plot takes the full row. Plots of one kind come in the order: per-dive plot, then its `*_avg`, then its count.

### 3.4 Standard layout

Use this layout for every cartesian plot unless its family (section 7) says otherwise:

```python
go.Figure(layout=go.Layout(
    title=dict(text='{}: Ascent Rate'.format(float_id)),
    xaxis=dict(title='Vertical Rate [mBar s<sup>−1</sup>]', autorange=False, range=rng['rate']),
    yaxis=dict(title='Pressure [dBar]', autorange=False, range=rng['pres']),   # [deep, 0]: reversed
    hoverlabel=dict(font=dict(family='Consolas, monospace', size=14)),
    legend=dict(title='Dive'),
    plot_bgcolor='white',
    margin=dict(l=10, r=10, b=20, t=50, pad=4),
    ))
```

- **Background.** `plot_bgcolor='white'`; leave `paper_bgcolor` at its default (white).
- **Grid and axis lines.** Don't set them. The default Plotly template draws grid, zero and axis lines in white, so on the white background the plots show **no grid, no zero line and no axis line**. That's the house style. Where a reference value matters, draw it as a reference line (section 5.4) instead of turning the grid on. Polar and geographic plots are the exceptions (sections 7.10 and 7.11).
- **Margins.** In the HTML, `l=10, r=10, b=20, t=50, pad=4`, or `t=70` with a `<sup>` subtitle. `WritePlotImageFiles` replaces them in the PNGs with `l=15, r=15, b=15, t=40, pad=0`, so you can't tune PNG margins per plot.
- **Fonts.** Don't set a figure font. Plotly's default ("Open Sans", Verdana, Arial) matches the float page's CSS. Set explicit sizes only for annotations (12) and polar ticks (12, radial 10).
- **Legend.** `legend=dict(title='Dive')` for per-dive traces. It appears only in the HTML (section 11).
- **Width and height.** Don't set them in the layout; `WritePlotImageFiles` sets them from the size class.

## 4. Axes

### 4.1 Titles

- `Quantity [unit]`, in title case: `Pressure [dBar]`, `Temperature [°C]`, `Salinity [PSU]`, `Vertical Rate [mBar s<sup>−1</sup>]`, `Duration [hours]`, `Energy [kJ]`, `Vacuum [inHg]`, `Frequency [Hz]`.
- The dive-number axis is titled exactly `Dive`. Section 4.4 relies on this.
- Δt axes say their unit: `Time [minutes]`, `Time [seconds]`. Date axes have an empty title (`''`), since the dates speak for themselves.
- Density axes may plot the anomaly: `In-situ Density Anomaly, ρ - 1000 [kg m<sup>−3</sup>]`, or `Potential Density Anomaly, σ<sub>Θ</sub> [kg m<sup>−3</sup>]`.
- Counts are titled `Samples`, `Bins`, `Messages` and so on, with no unit.

### 4.2 Pressure axes

Pressure axes are reversed, with the surface at the top. Do it with an explicit range `[deep, shallow]` and `autorange=False`, not with `autorange='reversed'`, so that all dives of a float share one fixed scale.

| Plot | Range |
|---|---|
| Profiles, descent | `[round(max + 1), 0]` |
| Ascent, pump | `[round(max + 10), 0]`, pump `[max + 10, −10]` |
| Drift | percentiles `[100, 0]`, padded `+1/−1` |
| Trajectory | `[round(max + 5), −10]`, room above the surface for the event markers |
| Contours | `[p × 1.02, 0]`, p = the 90th percentile of all profile pressures `+ 5`, rounded to 10 dBar, at most 500 dBar (`PCUTOFF`) |
| Surface | percentiles `[99, 1]`, padded `±1`, rounded to 0.1 |

### 4.3 Ranges

Fix the ranges of all plots of one kind from the whole float's data, not per dive. Clip them to percentiles so a few bad samples don't flatten the plot:

1. Take percentiles of the finite values: `RangePercentile(values, [1, 99])`, which returns NaN for no data.
2. Pad: `±1` for temperature, `±0.95` for salinity, `±1` for in-situ density anomaly, `±0.5` for σ<sub>Θ</sub>, `±9` for vertical rate; pressure as in section 4.2. Surface temperature is padded `±2`, since it ranges more than water temperature.
3. Round to a sensible step: 1 decimal for T and density, 2 for salinity, 0 for rates and pressure.
4. Set `autorange=False` and `range=rng[...]`.

Further rules:

- **Density-like axes that are not pressure:** for drift density and σ<sub>Θ</sub>, the denser value goes at the bottom (percentiles `[99, 1]`), like pressure.
- **Quantities that start at zero:** energy, counts and durations use `[0, …]` or `rangemode='tozero'`.
- **Pre-mission outliers:** where they would flatten the bars (BIT tests, first pump), cap the range at `1.1 × max` of the dives ≥ 0.
- **Rates:** a rate axis always includes zero and a little of the other sign. Ascent forces the minimum to −2 or below; descent forces the maximum to +2 or above.
- **Log axes:** Plotly takes `type='log'` ranges in log<sub>10</sub> units (`range=np.log10([lo, hi])`).

### 4.4 Dive-number axes

- The range is `[first − 0.5, last + 0.5]`, so the first and last bars are whole. `IntegerDiveTicks` (below) sets it on any dive axis without a fixed range, so a writer only sets one to override it. Startup dives are negative and included, except where they would mislead: the air pump and first-pump plots start at −0.5, and `cycle_timing` leaves out the startup dives, which have no cycle.
- **Whole-number ticks.** Plotly puts ticks on halves (0, 0.5, 1, …) when a float has only a few dives. `WritePlotImageFiles` therefore calls `IntegerDiveTicks(figure)` on every figure. For every axis titled `Dive` it:
  - skips date, category and log axes, and ignores date or text values;
  - sets the range `[first − 0.5, last + 0.5]` when the axis has none;
  - formats the tick labels as integers (`tickformat='d'`);
  - puts a tick on every dive when the axis spans 10 dives or fewer, and on every second dive up to 20 (`tickmode='linear'`, `tick0=0`, `dtick=1` or `2`);
  - leaves longer spans to Plotly, whose ticks are then whole numbers.

  A new plot gets this for free as long as its dive axis is titled exactly `Dive`. Don't set `dtick` on dive axes yourself.

### 4.5 Time axes

- Δt axes are linear, in minutes (ascent, descent, drift) or seconds (surface), starting at 0.
- Date axes (`type='date'`, or datetime x values) show UTC. Pad the trajectory's time range by 3% of its span plus 6 hours on each side, and the contours' by 1% plus 1 hour.
- A time-of-day axis (satcom) plots times on a fixed dummy date with `type='date'` and `tickformat='%H:%M'`.

## 5. Colors, lines and markers

### 5.1 Dive colors

Per-dive traces are colored along an HSV ramp, from blue for the first dive in `dives` to red for the last. The latest dive is black:

```python
colors = [colorsys.hsv_to_rgb(x*.666/((len(dives)-1) or 1), .8, 1) for x in range(len(dives))][::-1]
colors = ['rgb'+str(tuple([round(255*y/5)*5 for y in x])) for x in colors]
colors[len(dives)-1] = 'rgb(0,0,0)'
```

- `dives` includes the negative startup dives and dive 0. Index colors by **position** in `dives`, keeping a counter `i` that you increment even when you skip a dive. That keeps a dive's color the same in every plot.
- **The latest dive is black and wider:** line width 2, against 1.2 for the others (`width=2 if dive == dives[-1] else 1.2`). On marker plots it is black at the normal size; on maps it is a star (section 7.11).
- **Exception, waves:** the ramp runs over dive numbers 0 to `max_dive` and is indexed by dive number (`colors[dive]`).

### 5.2 Named colors

| Use | Color |
|---|---|
| Statistics: min./max. segments | `rgb(194,204,204)` |
| Statistics: ±1σ segments | `rgb(123,123,123)` |
| Statistics: mean | `rgb(0,0,0)` |
| Neutral bars (counts, sums, durations) | fill `rgb(220,220,220)`, outline `rgb(188,188,188)` width 0.5 |
| Reference lines (zero rate, constant-value guides) | `rgb(222,222,222)` |
| Configured value (e.g. DFT, the pressure correction) | `rgb(0,0,255)` blue |
| Pair of limits (e.g. air bladder SAX and SAN) | maximum `rgb(255,0,0)` red, minimum `rgb(0,0,255)` blue |
| Background targets (trajectory drift target) | `rgb(190,190,190)`, width 1, dashed |
| Warning (pump voltage falling, field reset) | `rgb(214,39,40)` |
| Estimate with a range (remaining cycles) | line `rgb(31,119,180)`, band `rgba(31,119,180,0.18)` |
| BIT test points | `rgb(255,140,0)` orange |
| GPS fix before descent / after ascent | `rgba(255,69,69,0.7)` / `rgba(69,69,255,0.7)` |
| Land (maps) | `rgb(220,220,220)` on geo maps, `rgb(190,190,190)` on the bathymetry map |
| Track (maps) | `rgb(0,0,0)` |

**Phase colors** come in two sets: a muted one for bars and a bright one for lines.

| Phase | Bars (`cycle_timing`) | Lines (`trajectory`) |
|---|---|---|
| Descent | `rgb(99,222,99)` | `rgb(69,255,69)` |
| Park/Drift | `rgb(99,99,222)` | `rgb(69,69,255)` |
| Ascent | `rgb(222,99,99)` | `rgb(255,69,69)` |
| Surface | `rgb(122,122,122)` | `rgb(122,122,122)` |

**Named palettes in the code:**

- `DEPTH_BLUES`, from dark blue at depth (0) to near white at the surface (1): bathymetry and pump pressure.
- `ENERGY_COLORS`, per energy component:
  - pump: blues (`158,202,225`, `33,113,181`, air pump `8,48,107`);
  - CTD: greens;
  - Iridium and GPS: oranges;
  - WIMS and MAPLe: purples;
  - controller and losses: grays.
- `SBD_TYPE_COLORS`, per message type. Retransmissions get a `/` hatch (`pattern=dict(shape='/')`).

**Continuous scales** are cmocean, by quantity:

| Quantity | Scale |
|---|---|
| Temperature | `thermal` |
| Salinity | `haline` |
| Density, σ<sub>Θ</sub> | `dense` |
| Spectral amplitude (microstructure) | `amp` |
| Depth, pump pressure | `DEPTH_BLUES` |

Wave waterfalls use their own white → blue → purple → red scale; mean direction a cyclic scale.

### 5.3 Line widths and dashes

| Line | Width | Dash |
|---|---|---|
| Per-dive data, latest dive | 2 | solid |
| Per-dive data, other dives | 1.2 | solid |
| Statistics segments (min./max., ±1σ) | 1.5 | solid |
| Statistics mean | 1.2 | solid |
| Marker plots' connecting lines (pump records) | 1 | solid |
| Reference line | 1.5 (1 for background guides) | `dash` |
| Configured value or limit | 1.5 | `dash`; steps with `shape='hv'` |
| Map track | 1 (1.3 on the bathymetry map) | solid |
| Whiskers (min/avg/max) | 0.5 | `dot` |

### 5.4 Reference lines

Draw reference values that aren't data (zero rate, limits, constant-acceleration guides) so that they never hide or take the hover from the data:

- **Preferred:** a layout shape below the traces, as `AddCaseVacLimits` does:

  ```python
  fig.add_shape(type='line', xref='paper', x0=0, x1=1, y0=y, y1=y, layer='below',
                line=dict(color='rgb(150,150,150)', width=1, dash='dash'))
  ```

  Limits that mark a fault band (the case-vacuum limits) use the darker gray `rgb(150,150,150)`, width 1. Plain references (zero lines, guides) use `rgb(222,222,222)`, width 1.5, or width 1 for many background guides.

  Use `fig.add_vline(...)` for a marked event, such as the pump-voltage fall, with an annotation (section 5.7).
- **Also in use:** a trace added first, with `hoverinfo='none'` and `showlegend=False`, in `rgb(222,222,222)`, width 1.5, `dash='dash'`. The zero lines of the rate plots are drawn this way.

### 5.5 Markers

| Use | Symbol | Size | Color | Outline | Opacity |
|---|---|---|---|---|---|
| Pump records (lines+markers) | circle | `clip((t − t_p1) / t_p99 × 20 + 5, 5, 25)`, rounded: a longer pump run gives a bigger marker | dive color | `rgb(0,0,0)` 0.3 | 0.7 |
| One value per dive (e.g. pump energy vs max. p) | circle | 18 | dive color | black 0.5 | 0.6 |
| Phase points (`phase_vac`) | circle | 8 | dive color | black 0.3 | 0.7 |
| Map: profile positions | circle | 10 (9 on the bathymetry map) | dive color | 0.2 (0.4) | 0.5 (0.85) |
| Map: latest profile | star | 10 (16 on the bathymetry map) | dive color (the maps don't blacken the latest dive) | 0.5 (0.8) | 1 |
| Map: GPS before descent / after ascent / beacon | `x` / `cross` / `star` | 9 | as section 5.2 | 0.5 | 0.7 |
| Map: Iridium composite / single SBD position | `diamond` 9 / circle 5 | | | | 0.8 |
| GPS per dive (x = dive ∓ 0.25) | `x` before descent, `cross` after ascent | 8 | as section 5.2 | black 0.5 | 0.7 |
| Min/avg/max whiskers | `x-thin`, `cross-thin` | 6 | series color | line 1 | |
| BIT tests | `diamond` | 7 | `rgb(255,140,0)` | | |
| Satcom time of day | circle | 16 (GPS 8) | per event type | black 0.5 | 0.7–0.8 |
| Polar displacement | circle | 10 | dive color | black 0.3 | |
| Trajectory surface events (at p = −0.5) | `x-thin` Iridium, `cross-thin` GPS | 8, 7 | `rgb(69,255,255)`, `rgb(255,69,255)` | line 2, 1.5 | 0.8 |

Markers on data plots get a thin black outline (`marker=dict(line=dict(color='rgb(0,0,0)', width=0.3))`), so light colors stay visible on white.

### 5.6 Color bars

| Plot kind | Color bar |
|---|---|
| Contours, waterfalls, heatmaps under a wide plot | horizontal: `colorbar=dict(title=dict(text='Quantity [unit]', side='top'), thickness=20, orientation='h')` |
| Marker or bar colors (pump pressure) | vertical, thickness 12. Draw it with an invisible marker trace (`marker=dict(size=0, opacity=0, colorscale=…, cmin=…, cmax=…, showscale=True)`) that carries only the scale. |
| Maps | vertical, `len=0.6`, `y=0.3`, title `Depth [m]` |
| Subplots | vertical, thickness 15, `len=0.9`, title on the right, on the last panel only |

Tick a color bar at round steps (1, 2, 2.5 or 5 × 10<sup>n</sup>), with at most 6 intervals. Label depths and pressures as positive numbers, with the surface at the top: plot −p or −depth and set `ticktext`.

### 5.7 Annotations

- Font size 12, `showarrow=False`, in the color of what they label.
- Place them in paper coordinates: `y=1, yref='paper', yanchor='top'`. A label next to a vertical line sits on its right with `xshift=4`, or on its left when the line is past 60% of the axis.
- Use annotations instead of a legend when the PNG must identify series. The PNGs have no legend; for example, the microstructure channel labels sit in the plot corner in the channel colors.

## 6. Hover modes

| Plot kind | `hovermode` |
|---|---|
| Per-dive statistics, counts, sums, per-dive time series vs dive or date | `'x'` |
| Stacked bars | `'x unified'`, with an invisible "Total" marker trace at the top of each bar (`go.Scatter(mode='markers', marker=dict(size=0, opacity=0), showlegend=False)`) that carries the dive total |
| Profiles vs pressure, Δt series, maps, polar plots | default (`'closest'`) |

The hover label font is `Consolas, monospace`, 14 pt everywhere. Use 12 pt only for crowded unified hovers (energy and SBD stacks). Reference lines, bands, tracks and coastlines get `hoverinfo='none'` or `'skip'`.

## 7. Plot families

Each family gives the trace recipe, the layout differences from section 3.4, and the size class. Copy the representative function named in each heading as the starting point for a new plot.

### 7.1 Profiles vs pressure (`WriteProfilePlots`, `WriteAscentPlots`, `WriteDescentPlots`)

- **Trace:** one `go.Scatter(mode='lines')` per dive, `name='<dive>'`, `line=dict(color=colors[i], width=2 if dive == dives[-1] else 1.2)`. No markers.
- **Axes:** x is the quantity, with a fixed percentile range (section 4.3). y is pressure, reversed (section 4.2).
- A rate profile gets a dashed zero line, `x=[0, 0]`, `y=[0, pmax]` (section 5.4).
- `legend=dict(title='Dive')`; default hovermode.
- **Size:** `fig`. **Categories:** `profile`, `ascent`, `descent`.

### 7.2 Time series vs Δt (`WriteDriftPlots`, `WriteSurfacePlots`, ascent/descent vs time)

- **Trace:** same as 7.1, with x = Δt since the start of the phase (`dt_ref_delta_sec / 60` for minutes).
- **x axis:** `Time [minutes]` with range `[0, round(max + pad)]` (pad 1 for ascent and descent, 2 for drift), or `Time [seconds]` `[0, max]` at the surface.
- **y axis:** the quantity with a fixed percentile range; pressure and density reversed (sections 4.2, 4.3).
- **Size:** `fig`.

### 7.3 Per-dive statistics, `*_avg` (e.g. `ascent_rate_avg` in `WriteAscentPlots`)

Draw three traces, in this order. Each is one Scatter of vertical segments, one per dive, separated by NaN:

```python
seg = lambda lo, hi: dict(x=np.vstack((dive, dive, [np.nan]*len(dive))).flatten('F'),
                          y=np.vstack((lo, hi, [np.nan]*len(dive))).flatten('F'))
fig.add_trace(go.Scatter(**seg(vmin, vmax), mode='lines', name='min./max.', hoverinfo='none',
                         line=dict(color='rgb(194,204,204)', width=1.5)))
fig.add_trace(go.Scatter(**seg(np.maximum(mean-std, vmin), np.minimum(mean+std, vmax)), mode='lines',
                         name='±1σ', hoverinfo='none', line=dict(color='rgb(123,123,123)', width=1.5)))
fig.add_trace(go.Scatter(x=dive, y=mean, mode='lines', name='mean', customdata=np.stack((vmin, vmax, std), -1),
                         hovertemplate=..., line=dict(color='rgb(0,0,0)', width=1.2)))
```

- **Layout:** `xaxis=dict(title='Dive', range=[−0.5, last + 0.5])`, `hovermode='x'`, and the y range of the same quantity's per-dive plot. Leave out the legend title.
- **Extras:** a configured value or correction is a dashed blue line, width 1.5, with markers size 4. A composite value is drawn as `cross-thin` markers.
- **Size:** `fig2`.

### 7.4 Counts and sums per dive (e.g. `ascent_sample_count`)

- **Trace:** `go.Bar(x=dives, y=values, marker=dict(color='rgb(220,220,220)', line=dict(color='rgb(188,188,188)', width=0.5)), showlegend=False)`.
- **Layout:** `bargap=0`, `hovermode='x'`, `xaxis=dict(title='Dive', range=[−0.5, last + 0.5])`; y from zero (section 4.3).
- **Overlays:**
  - BIT tests: orange diamond markers.
  - Configured values: dashed blue steps over `dive ± 0.5`, with NaN breaks.
  - A second quantity: `yaxis2=dict(overlaying='y', side='right', zeroline=True, zerolinecolor='rgb(188,188,188)')`, as lines+markers, width 1.2, size 3.
- **Size:** `figWideXS` (`figWideS` with a second axis).

### 7.5 Stacked bars (`cycle_timing`, `energy_per_cycle`, `satcom_sbd_data_size`, `pump_energy_sum`)

- **Traces:** one `go.Bar` per component, bottom up, with `marker=dict(color=…, line=dict(color='rgb(255,255,255)', width=0.3))`. The thin white outline separates the segments.
- **Layout:** `barmode='stack'`, `bargap=0`, `hovermode='x unified'` with the invisible Total trace (section 6), `legend=dict(title=<what the components are>, traceorder='reversed')` (`Component`, `Cycle Phase`, `Message type`), so that the legend reads top-down like the stack. y titled with its unit, `rangemode='tozero'`.
- **Hover of a component:** in an `x unified` hover each line is one trace. Either leave `<extra></extra>` out so Plotly prefixes the trace name, or keep it and name the component in the template (`'Descent: Δt = …'`). The Total trace's template is `'Total: <b>…</b><extra></extra>'`.
- **Segments colored by a continuous value** (pump pressure): color each segment with `plotly.colors.sample_colorscale(DEPTH_BLUES, 1 − p/pmax)`, gray `rgb(140,140,140)` where the value is invalid. Outline `rgba(90,90,90,0.35)`, width 0.4, or none above 150 dives. Show the scale with an invisible-marker color bar (section 5.6) and write the plot without a legend (`showlegend=False`).
- **Size:** `figWideS` for a breakdown, `figWideXS` for a count or sum (including `satcom_sbd_data_size`, bytes per dive by message type).

### 7.6 Share of a total (`ShareBarFigure(title, labels, values, colors, text, hover_value)`)

Call the helper, don't rebuild it. It draws one horizontal bar stacked in percent:

- parts outlined white 0.5;
- labels inside the parts of 5% or more (`uniformtext=dict(minsize=11, mode='hide')`);
- `xaxis=dict(title='Share [%]', range=[0, 100], ticksuffix='%')`, hidden y;
- a horizontal legend below the plot;
- `t=70` for the subtitle.

**Size:** `figWideBar`.

### 7.7 Marker plots of records (`WritePumpPlots`, `phase_vac`)

- **Trace:** `go.Scatter(mode='lines+markers', opacity=0.7, line=dict(color=colors[i], width=1), marker=dict(color=colors[i], size=…, line=dict(color='rgb(0,0,0)', width=0.3)))`. Size the markers by a third quantity where one exists (section 5.5), otherwise use 8.
- **Records without a time:** plot them against their order, titled `Pump #`, with range `[−1, n + 1]`.
- **Categorical x** (phases): use `tickvals`/`ticktext`, with None gaps between categories, and `connectgaps=False`.
- **Size:** `fig` (`figWide` for `phase_vac`).

### 7.8 Contours and heatmaps (`WriteContourPlots`)

- **Regridding:** interpolate the profiles onto a pressure grid (1 dBar to 100, 2 dBar to 300, 5 dBar deeper) and onto the profile times.
- **Trace:**

  ```python
  go.Contour(x=grid_time, y=grid_pres, z=regrid, zauto=False, zmin=rng[q][0], zmax=rng[q][1],
             connectgaps=False, contours=dict(coloring='heatmap', showlabels=False),
             line=dict(color='rgba(0,0,0,0.5)', smoothing=0.85, width=0.3),
             colorscale='thermal', colorbar=dict(title=dict(text='Temperature [°C]', side='top'),
                                                 thickness=20, orientation='h'))
  ```

- **Axes:** an empty x title on the date axis; pressure reversed and capped at 500 dBar (section 4.2).
- **Color ranges:** fixed per quantity from the whole float, padded less than the line plots because the colors must resolve small differences: density and σ<sub>Θ</sub> percentiles [1, 99] ±0.05; temperature [1, 99] ±1.25; salinity [5, 98] padded −0.55/+0.15 (fresh surface outliers); all rounded to 0.1.
- **Hover values** are the regridded ones, preformatted with 2 decimals (`-` where missing), and the grid pressure has none (section 1).
- Write the plot with `showlegend=False`.
- **Size:** `figWideL`, or `figWideXL` when the pressure axis (before its 2% margin) goes deeper than 300 dBar.

### 7.9 Spectra and waterfalls (`WriteWavePlots`, `WriteMicrostructurePlots`)

- **Line spectra:** per-dive lines as in 7.1. x `Frequency [Hz]` with a fixed range. Log y variants use `type='log'` with ranges in log<sub>10</sub>, floored so that noise doesn't stretch the axis.
- **Backgrounds:**
  - Guide lines: `rgb(222,222,222)`, width 1.
  - Shaded noise floor: `fill='tozeroy'`, `fillcolor='rgba(244,244,244,0.7)'`.
  - Text labels: size 12.
- **Waterfalls:** pass the data as a grid, not one row per point: 1-D x (times) and y (frequencies), 2-D z (32-bit floats) and 2-D `customdata` of short preformatted strings (`WaveGrid`). One row per point repeats the time string and frequency in every cell and made these pages 15–20 MB for long missions. `go.Heatmap(zsmooth='best', connectgaps=True)` with the colorbar of section 5.6. Log variants plot log<sub>10</sub>(z), with `10<sup>n</sup>` tick text.
- **Size:** `figWideL` for spectra, `figWideXL` for waterfalls.

### 7.10 Polar plots (`WriteSubmergedDisplacementPlots`, `WriteSurfaceDisplacementPlots`)

- **Trace:** `go.Scatterpolar(mode='markers', marker=dict(color=[colors[i]], size=10, line=dict(color=['black'], width=0.3)))`, one per dive.
- **`polar`:**
  - `bgcolor='white'`.
  - `angularaxis`: `direction='clockwise'`, `rotation=90` (north up), `gridcolor='rgb(169,169,169)'`, `griddash='dash'`, `gridwidth=0.5`, `linecolor='rgb(169,169,169)'`, `linewidth=0.5`. Ticks every 45° with only N labeled; `tickfont` 12.
  - `radialaxis`: `angle=22.5`, `tickangle=22.5`, `nticks=8`, `showline=False`, the same dashed grid; title and ticks at size 10.
- **Range:** `[0, max + pad]`, rounded to hundreds for the surface distance and to tens otherwise.
- **Margins:** `l=10, r=10, b=10, t=70`.
- **Size:** `fig`.

### 7.11 Maps (`WriteGeoPlots`, `PositionBathymetryFigure`)

**Geographic (`go.Scattergeo`):**
- **`geo`:**
  - `showland=True`, `landcolor='rgb(220,220,220)'`, `showocean=False`, `showcountries=False`.
  - `coastlinewidth=0.5`, `coastlinecolor='rgb(188,188,188)'`, `resolution=50`, `fitbounds=False`.
  - Lat/lon grid `rgb(123,123,123)`, width 0.5.
- **Projection:** `orthographic` for the globe, centered on the circular mean position; `winkel tripel` for regional maps.
- **Regional range:** the profile positions, widened toward a 3:4 aspect, padded by 0.5° and rounded to 0.1°. When the profiles span 1° of longitude or less, the map is ±3° lat and ±4° lon around them.
- **Traces:**
  - Track: black, width 1, no hover.
  - Tropics and polar circles: `rgb(123,123,123)`, width 0.5, dashed. Equator: `rgb(88,88,88)`, width 0.6, solid.
  - Markers: as section 5.5.
- **Legends** of maps with several marker kinds: font size 10, grouped per dive (`legendgroup`, `legendgrouptitle`, `groupclick='toggleitem'`).

**Bathymetry (cartesian lon/lat):**
- **Axes:**
  - `yaxis.scaleanchor='x'`, `scaleratio=1/cos(mid-latitude)`.
  - `constrain='domain'` on both axes.
  - `ticks='outside'`, `showgrid=False`.
- **Region:** a square in km around the profiles plus 0.6°.
- **Layers, bottom up:**
  1. Depth heatmap (`DEPTH_BLUES`, land gray `rgb(190,190,190)`).
  2. Hillshade heatmap: black to white, opacity 0.35, no scale, no hover.
  3. Isobaths: `rgba(40,40,40,0.55)`, width 0.7, every 500 m (1000 m deeper than 6000 m), from `ContourLines`.
  4. Coast: `rgb(60,60,60)`, width 1.
  5. Track: black, 1.3.
  6. Profile markers.
- **Hover:** `hovermode='closest'`, no legend.
- **PNG:** written with `static_bare=True`, which drops the color bar and axis labels from the PNGs.

### 7.12 Trajectory (`WriteTrajectoryPlots`)

- **Phase lines:** pressure vs date, in the bright phase colors (section 5.2), width 1, opacity 0.8. Surface is lines+markers with `circle-open` size 5.
- **Background traces:**
  - Bathymetry: black, width 1.5.
  - Drift target: `rgb(190,190,190)`, width 1, dashed, `shape='hv'`.
- **Surface events:** at p = −0.5, the top of the reversed axis (section 5.5).
- **Size:** `figWideL` if deeper than 500 dBar, else `figWide`.

### 7.13 Estimate with an uncertainty band (`WriteRemainingCyclesPlot`)

- **Band:** first the upper edge, invisible (`line=dict(width=0)`, `showlegend=False`, `hoverinfo='skip'`). Then the lower edge with `fill='tonexty'`, `fillcolor='rgba(31,119,180,0.18)'`, named `Range`.
- **Estimate:** `rgb(31,119,180)`, width 2.
- **Event:** a red dashed `add_vline` with an annotation (section 5.7).
- **Layout:** `rangemode='tozero'`, `hovermode='x'`, `t=70` for the subtitle.
- **Size:** `figWideS`.

### 7.14 Subplots (`WriteMicrostructurePlots`)

- **Construction:** `make_subplots(rows=1, cols=1+n, shared_yaxes=True, horizontal_spacing=0.05, column_widths=[0.3]+[0.7/n]*n)`.
- **Axes:** pressure on the shared y axis, reversed. Label each panel's x axis.
- Series labels are annotations in the panel, because the PNGs have no legend. Only the last panel shows a color bar (section 5.6).
- **Size:** `figWideL`.
- **Older styling:** this family alone sets an explicit grid (`rgb(230,232,235)`) and axis line (`rgb(89,99,110)`). Don't copy that to other plots.

## 8. Missing data and empty plots

- Listing fill values are NaN before plotting, so they are not drawn. Hover templates show `-` for values preformatted in Python.
- **Dives with too little data:** a dive with no samples, or only one sample for a line, is skipped with a log message, but its color index still advances.
- **Plots with too little data:** a plot that would be empty is not written. Its writer returns no metadata for it, so the float page leaves it out. Use `DropFigures(figs, keys, reason, float_id)` to drop a group of figures for one reason (for example "No salinity").
- **Background traces** (references, ranges, guides) go in before the data traces, so the data draw on top.

## 9. Writing hover templates in Plotly

- Build the template with `'<br>'.join([...])`, one line per item, so items can be reordered and reviewed:

  ```python
  customdata=np.stack((df_dive['dt_str'], df_dive['pres'], df_dive['temp'], df_dive['saln'], df_dive['sigma0']), -1),
  hovertemplate='<br>'.join([
      'Dive <b>{}</b>: Park/Drift'.format(dive),
      '%{customdata[0]|%Y-%m-%d %H:%M:%S}Z',
      'Δt = %{x:.1f} min',
      'p = %{customdata[1]:.1f} dBar',
      'S = %{customdata[3]:.3f} PSU',
      'T = %{customdata[2]:.3f} °C',
      'σ<sub>Θ</sub> = %{customdata[4]:.2f} kg m<sup>−3</sup>',
      '<extra></extra>',
      ]),
  ```

- Values the hover shows but the axes don't go in `customdata`. Keep the order of `np.stack` and the indices in the template in step, and check both when you edit either.
- A template can't do arithmetic. Put derived values into `customdata`.
- Format timestamps in the template, not in Python, so all plots match: `%{customdata[0]|%Y-%m-%d %H:%M:%S}Z`, or `%{…|%Y-%m-%d %H:%M}Z` for GPS times, and `%{x|…}` on date axes. Where a time must be a Python string (one time per trace), use `HoverTime(dt_str)`, or `HoverTime(dt_str, seconds = False)` for a GPS time.
- Preformat values in Python only where the template can't do it, for example `-` for a missing value in contour grids. Then use `%{customdata[i]}` with no format.

## 10. Code structure of a plot group

A plot-group writer follows one pattern. Copy an existing writer of the same family:

1. **Check for data.** Return `[]` if the float's aggregate has no data for the group (log at debug or warning level).
2. **Clean and prepare.** Copy the DataFrame, set invalid values to NaN, and compute derived columns, such as `dens + 1000` for ρ.
3. **Compute ranges.** Build `rng`, one fixed range per quantity, from the whole float (section 4.3).
4. **Set up colors.** Build `colors` with the dive ramp (section 5.1).
5. **Create the figures.** Build `figs = dict(<id_str> = go.Figure(layout=…), …)`, one entry per plot, with the standard layout (section 3.4). The `id_str` becomes the file name `<serial>_<id_str>` and the key in the float page's figure table.
6. **Add traces.** Add background traces first, then loop over `dives` with the color counter `i` and add one trace per dive to each figure. Wrap each dive in `try/except`, logging the error, or raising it with `--halt-on-error` (`args.halt`).
7. **Write.** Call `WritePlotImageFiles(id_str=fig, figure=figs[fig], float_id=float_id, dpath_float=dpath_float, category='<category>', size=fig_size.get(fig, 'fig'), showlegend=…)` for each figure, and collect the returned metadata.
8. **Register.** Add the writer to `PLOT_WRITERS` and the plot ids to the float page's layout lists (`WriteHtmlFile`), and document the plots in [plot-and-page-files.md](plot-and-page-files.md) section 3.

## 11. What `WritePlotImageFiles` does to every figure

You don't do these yourself; know them so the plot survives them:

1. **Dive ticks.** It calls `IntegerDiveTicks` (section 4.4).
2. **HTML.** It sets `showlegend` from its argument and the HTML size from the size class, then writes the HTML with plotly.js from the CDN and `config=dict(displaylogo=False, modeBarButtonsToRemove=['select2d', 'lasso2d'])`.
3. **`static_bare=True`.** Only for `position`: it hides heatmap and contour color bars and blanks the axis ticks and titles in the PNGs.
4. **PNGs.** It turns the legend off, sets the small PNG size and the PNG margins `l=15, r=15, b=15, t=40, pad=0`, and queues the figure. The small and full PNGs are rendered from the same layout and differ only in pixel size. The thumbnail is the small PNG scaled to 150 px wide.
5. **Things that do nothing.** The `ts_pos` argument is unused (there is no timestamp annotation), and layout `width`, `height` and `showlegend` set in a writer are overwritten.

## 12. Checklist for a new plot

- [ ] It belongs to a family in section 7 and copies that family's representative code.
- [ ] The title is `<serial>: <Title>` in title case. Any subtitle is one line in `<sup>`, with `t=70`.
- [ ] The layout is the standard one (section 3.4): white background, no grid, Consolas hover labels, `legend=dict(title='Dive')`.
- [ ] Axis titles are `Quantity [unit]`; the dive axis is titled exactly `Dive`, with range `[−0.5, last + 0.5]`.
- [ ] Ranges are fixed from the whole float with percentile clipping and padding; pressure is reversed.
- [ ] Per-dive traces use the dive ramp, indexed by position, with the latest dive black and width 2.
- [ ] Reference lines, bands and tracks are gray, dashed where they are references, and have no hover.
- [ ] Markers have a thin black outline and the size, symbol and opacity of section 5.5.
- [ ] The hover follows section 2: dive and phase, time (to the minute for GPS times), Δt or t, Σt, p, S, T, σ<sub>Θ</sub>, then the rest, with the section 1 symbols, units and formats, ending in `<extra></extra>`. The `customdata` indices match the template.
- [ ] The size class suits the plot's shape (section 3.3).
- [ ] The small PNG reads well with no legend and no hover.
- [ ] The plot is registered in `PLOT_WRITERS` and the float page, and listed in [plot-and-page-files.md](plot-and-page-files.md) section 3.

## 13. Known inconsistencies in existing plots

New plots follow the rules above. These older plots differ and should be brought in line when they are next changed:

| Plot | Differs in | Rule |
|---|---|---|
| `trajectory` vs `cycle_timing` | two phase-color sets, not shared constants | 5.2 (keep both sets, define them once) |
| Microstructure | explicit grid and axis lines (`rgb(230,232,235)`, `rgb(89,99,110)`); pressure with `autorange='reversed'` | 3.4, 4.2 |
| Pump and `phase_vac` | the latest dive is black but not wider | 5.1 (markers keep their size) |
| `energy_per_cycle`, `energy_remaining_cycles`, `satcom_sbd_data_size` | hover font 12 pt | 6 (allowed for crowded unified hovers) |
| Reference lines | drawn as traces in some plots, as shapes in others | 5.4 |
| Ascent and descent rate statistics | min and max with 1 decimal, mean and σ with 2 | 2.2 |
| A few hover labels | `no. pumps =`, `No. =` (Iridium composite), `drift seek adj.`, `Dive N: surface start` | 1 |

## 14. Changes made when the guide was adopted

On 2026-11-13 the hover text of `xmtps-aggregate-json.py` was brought in line with this guide:

- Temperature is `T` (was `t`) and salinity `S` (was `s`).
- Pressure lines without a label got one (`p = …`), and vertical rates are `dp/dt = …`.
- Pressures have one decimal (were two), except surface pressure data, which keep two, and the regridded pressures of the profile contours, which keep none.
- Timestamps are `YYYY-MM-DD hh:mm:ssZ`, or `YYYY-MM-DD hh:mmZ` for times from GPS fixes, which have no seconds. Some plots showed only minutes for every time.
- The first line names the phase in the ascent, descent, drift, surface, profile and contour plots.
- Ascent, descent, drift and surface hovers give Δt, from the plot's time axis.
- Pump and air-pump run durations are `t = … s`; cumulative pump time is `Σt`.
- Lines are in the standard order: dive, time, Δt/Σt, p, S, T, (ρ − 1000), σ<sub>Θ</sub>, then the rest.
- Only the in-situ density plots show in-situ density, as the anomaly `ρ − 1000`. Profile and drift hovers show σ<sub>Θ</sub>.
- Potential density anomaly is σ<sub>Θ</sub> (was σ<sub>0</sub>), in hovers and axis titles; the profile contour's color bar, titled "Potential Density", is now "Potential Density Anomaly".
- Fixed: the drift temperature hover showed temperature as S and salinity as σ<sub>Θ</sub>, from wrong custom-data indices.
- Dive-number axes get whole-number ticks (`IntegerDiveTicks`, section 4.4). A float with a few dives showed ticks at 0.5 steps.
- The `drift_sigma0_avg` y axis is titled "Potential Density Anomaly, σ<sub>Θ</sub>" (was "In-situ Density Anomaly, ρ - 1000").
- The profile and wave hovers show their GPS times as `YYYY-MM-DD hh:mmZ`. The profile hover gives S, T and σ<sub>Θ</sub> with 3 decimals (were unformatted).
- Displacement bearings are `θ = …°` (were `bearing = …°`).
- After an audit against this guide (2026-11-13), the plots were brought in line except for the items in section 13. Among the changes:
  - `cycle_timing` became a unified stacked bar (white segment outlines, reversed legend, Total trace, `Park/Drift`, `Δt = … hours` per phase);
  - dive axes without a fixed range get `[first − 0.5, last + 0.5]` from `IntegerDiveTicks`;
  - U+2212 in all exponents;
  - no `NaN` in pump, surface and air-pump hovers (`HoverStr`);
  - fixed ranges for the pump vacuum, pump energy and GPS plots (`PaddedRange`);
  - full times with seconds in the displacement hovers and wave heatmaps;
  - densities shown as anomalies, with in-situ density only in the in-situ density plots;
  - `drift_dens_avg` labels its statistics as the in-situ density anomaly;
  - count bars say `N samples` or `N bins`;
  - microstructure uses `DEPTH_BLUES` with the surface at the top and standard hovers;
  - `displacement_surface` blackens the latest dive and the "Displacement" typo is fixed;
  - `air_pump_pres` is not written without bladder data;
  - expanded abbreviations in titles.
- Dive 0, the diagnostic dive, is included in the trajectory, map, contour, wave and SBD-type plots (it was left out of them). The energy plots still leave it out.
- Densities (ρ, ρ<sub>Θ</sub>, σ<sub>Θ</sub>, and their statistics) have two decimals everywhere (were 1 or 3).
