# KML File Format Specification

XMTPS writes Google Earth files that show where each float has surfaced: one KMZ file per float and one master KMZ file with all floats. The post-processing thread (stage 4) writes them from the GPS fixes stored in the MySQL database. They are meant to be opened in Google Earth, directly or through a network link to `http://argo.whoi.edu/kml/master.kmz`; no program in this repository reads them.

Status: compiled on 2026-10-30 from the decoder source and the production files on argus (`/data1/alamo/data/kml`).

## Contents

- [1. Configuration](#1-configuration)
- [2. Naming and location](#2-naming-and-location)
- [3. When the files are written](#3-when-the-files-are-written)
- [4. Container: KMZ](#4-container-kmz)
- [5. Per-float file](#5-per-float-file)
- [6. Master file](#6-master-file)
- [7. Styles and icons](#7-styles-and-icons)
- [8. Which positions are used](#8-which-positions-are-used)
- [9. Other files in the KML directory](#9-other-files-in-the-kml-directory)
- [10. Readers](#10-readers)
- [11. Open questions / discrepancies](#11-open-questions--discrepancies)

## 1. Configuration

The options are parsed in `src/Main.cpp`; see also [configuration-file-format.md](configuration-file-format.md).

| Option | Default | Effect |
|---|---|---|
| `dir.kml` | `kml` | Directory of all KML files (`XFloat::kml_directory_path_`). Created if missing. |
| `gen.kml` | `1` | Write the per-float and master files (`XFloat::kml_file_generation_is_enabled_`). With `0`, neither is written. |

There is no per-float switch. Every float in the decoder's float cache gets a file.

## 2. Naming and location

| File | Path | Written by |
|---|---|---|
| Per-float | `<dir.kml>/<serial>.kmz` (`/%04d.kmz`) | `XFloat::WriteKmlFile()` (`src/data/XFloat.cpp`) |
| Master | `<dir.kml>/master.kmz` | `XFloat::WriteKmlMasterFile()` (`src/data/XFloat.cpp`) |

`<serial>` is the WHOI instrument serial number, zero-padded to four digits (`2987.kmz`, `9245.kmz`, `11014.kmz`). The directory is flat; there are no per-float subdirectories.

## 3. When the files are written

The files are written by the post-processing thread, `XFloatPostProcessingService::operator()()` (`src/stage4b_PostProcessingThread.h`). The build uses the polling loop, not the queue (`XMTPS_POST_PROC_USES_QUEUE` is `false` in `src/ThreadDefinitions.h`). Each pass:

1. Sleeps 60 seconds (`idle_sleep_seconds_`, in 100 ms steps so that shutdown is seen quickly).
2. For every cached float (`XFloat::GetAllCachedFloats()`): calls `WriteKmlFile()`, then `WriteStatusFile()` ([status-file-format.md](status-file-format.md)). A failure for one float is logged and the loop goes on.
3. If at least one per-float KML file was written in this pass, writes the master file.
4. Closes the SQL connection.

`WriteKmlFile()` writes only if the float's `kml_file_updated_` flag is false, then sets it. The flag is false:

- for every float when the decoder starts (the constructors set it false), so all per-float files and the master file are rewritten in the first pass after startup;
- after a dive of the float becomes complete. `ProcessDiveData()` (`src/stage3_TelemetryDecoderThread.h`) then clears `kml_document_updated_`, `kml_file_updated_` and `status_file_updated_`.

A message that does not complete a dive does not mark the KML file stale; it only marks the status file stale. A late message for an already complete dive does mark it stale again.

The per-float KML document is cached in the float object (`kml_document_`) and regenerated only when `kml_document_updated_` is false. The master document is regenerated from the database every time it is written.

So the master file is rewritten at most once a minute, and only in minutes when some float completed a dive. In production on 2026-10-30, up to 14:36 UTC, the log shows 14 per-float KML writes and 14 master writes.

Files are written in place (not to a temporary file and renamed). [inferred: a reader can see a partly written file]

## 4. Container: KMZ

Both files are KMZ archives written by libkml's `kmlengine::KmzFile::WriteKmz()`. Each archive holds one entry, `doc.kml`, with a zero (1980-00-00) timestamp:

```
Archive:  11014.kmz
  Length      Date    Time    Name
---------  ---------- -----   ----
   134957  1980-00-00 00:00   doc.kml
```

`doc.kml` is serialized by `kmldom::SerializePretty()`: two-space indentation, no XML declaration, and a root `<kml>` element without the `xmlns="http://www.opengis.net/kml/2.2"` namespace. Descriptions are written as CDATA sections.

Coordinates are `longitude,latitude,altitude` in decimal degrees and meters, one tuple per line, with the full double precision the database holds (for example `-159.81635,73.1951666666667,2000`).

## 5. Per-float file

Built by `XFloat::GenerateKmlDocument(true, true)` (`src/data/XFloat.cpp`). Element tree:

```
kml
└─ Document id="<serial>"
   ├─ name          <serial>
   ├─ description   WHOI ALAMO/Hurricane Float #<serial>
   ├─ Style id="path<k>"      line style for the trajectory
   ├─ Style id="dot1_<k>"     large dot for the latest position
   ├─ Style id="dot1s_<k>"    small dot for each surface position
   ├─ Style id="tri1_<k>"     triangle for the deployment position
   ├─ Placemark  "<serial> deployment position"   (only with a valid deployment position)
   ├─ Placemark  "trajectory"                      (always)
   ├─ Placemark  "<serial>"                        (latest position; only if there is a GPS fix)
   └─ Folder     "surface positions"
      └─ Placemark "<dive>:<yyyymmdd>T<hhmm>Z"     (one per profile fix)
```

`<k>` is the float's style index (section 7).

### 5.1 Deployment position placemark

| Element | Value |
|---|---|
| `name` | `<serial> deployment position` |
| `visibility` | `0` (hidden by default) |
| `styleUrl` | `#tri1_<k>` |
| `Point/extrude` | `1` |
| `Point/altitudeMode` | `relativeToGround` |
| `Point/coordinates` | deployment longitude, latitude, `2000` |

It is written only if the float has metadata with a valid `launch position` ([float-metadata-format.md](float-metadata-format.md)). The 2000 m altitude with `extrude` draws a vertical line from the ground to the triangle.

### 5.2 Trajectory placemark

| Element | Value |
|---|---|
| `name` | `trajectory` |
| `visibility` | `1` |
| `styleUrl` | `#path<k>` |
| `LineString/altitudeMode` | `relativeToGround` |
| `LineString/coordinates` | every trajectory fix (section 8), oldest first, altitude `0` |

It is written even when there are no fixes; the coordinates element is then empty (`<coordinates/>`).

### 5.3 Latest position placemark

| Element | Value |
|---|---|
| `name` | `<serial>` |
| `visibility` | `1` |
| `description` | HTML in CDATA, see below |
| `styleUrl` | `#dot1_<k>` |
| `Point/altitudeMode` | `relativeToGround` |
| `Point/coordinates` | the last trajectory fix, altitude `0` |

The description has the form:

```
<strong>Most Recent Profile Position for WHOI ALAMO/Hurricane Float #<serial></strong>
<br /><p>Profile #<dive> reported on <yyyy-mm-ddThh:mm:ss>Z at <ISO 6709 position></p>
```

The ISO 6709 position is `%+.5f%+.5f` latitude then longitude, for example `+76.14653-163.40539`.

### 5.4 Surface positions folder

A `Folder` named `surface positions`, with one placemark per fix of type 0x00 (diagnostic dive) or 0x02 (end of dive), oldest first:

| Element | Value |
|---|---|
| `name` | `<dive>:<yyyymmdd>T<hhmm>Z`, from `to_iso_string()` cut to 13 characters, for example `333:20181004T1040Z` |
| `visibility` | `0` |
| `styleUrl` | `#dot1s_<k>` |
| `Point/altitudeMode` | `relativeToGround` |
| `Point/coordinates` | the fix, altitude `0` |

The folder is written even when it is empty.

### 5.5 Example

Production, float 11014 (trimmed):

```
<kml>
  <Document id="11014">
    <name>11014</name>
    <description>WHOI ALAMO/Hurricane Float #11014</description>
    <Style id="path4">
      <LineStyle>
        <color>b080ff00</color>
        <width>5</width>
      </LineStyle>
    </Style>
    <Style id="dot1_4">
    ...
    </Style>
    <Placemark>
      <name>11014 deployment position</name>
      <visibility>0</visibility>
      <styleUrl>#tri1_4</styleUrl>
      <Point>
        <extrude>1</extrude>
        <altitudeMode>relativeToGround</altitudeMode>
        <coordinates>
          -159.81635,73.1951666666667,2000
        </coordinates>
      </Point>
    </Placemark>
    <Placemark>
      <name>trajectory</name>
      <visibility>1</visibility>
      <styleUrl>#path4</styleUrl>
      <LineString>
        <altitudeMode>relativeToGround</altitudeMode>
        <coordinates>
          -159.8396084,73.2070408,0
          -159.8622931,73.2145035,0
          ...
        </coordinates>
      </LineString>
    </Placemark>
    <Placemark>
      <name>11014</name>
      <visibility>1</visibility>
      <description><![CDATA[<strong>Most Recent Profile Position for WHOI ALAMO/Hurricane Float #11014</strong>
<br /><p>Profile #333 reported on 2018-10-04T10:40:00Z at +76.14653-163.40539</p>]]></description>
      <styleUrl>#dot1_4</styleUrl>
      <Point>
        <altitudeMode>relativeToGround</altitudeMode>
        <coordinates>
          -163.4053858,76.1465257,0
        </coordinates>
      </Point>
    </Placemark>
    <Folder>
      <name>surface positions</name>
      <Placemark>
        <name>0:20180906T0307Z</name>
        <visibility>0</visibility>
        <styleUrl>#dot1s_4</styleUrl>
        <Point>
          <altitudeMode>relativeToGround</altitudeMode>
          <coordinates>
            -159.8396084,73.2070408,0
          </coordinates>
        </Point>
      </Placemark>
      ...
    </Folder>
  </Document>
</kml>
```

This file has 337 placemarks: deployment, trajectory, latest position and 334 surface positions.

## 6. Master file

Built by `XFloat::GenerateKmlMasterDocument()` (`src/data/XFloat.cpp`):

```
kml
└─ Document
   ├─ name    WHOI ALAMO/Hurricane Floats
   ├─ Style × 52        all 13 style sets (path1..pathD, dot1_1..dot1_D, dot1s_1..dot1s_D, tri1_1..tri1_D)
   ├─ Folder "active"   visibility 1
   │  └─ Document id="<serial>" × N    (visibility 1)
   └─ Folder "archive"  visibility 0
      └─ Document id="<serial>" × M    (visibility 0)
```

Each float's `Document` is built by `GenerateKmlDocument(false, visibility)`: the same content as the per-float file (section 5) but without its own `Style` elements, since the styles are defined once at the top of the master document. The float `Document` also gets a `visibility` element, and its trajectory and latest-position placemarks take the folder's visibility. The deployment and surface-position placemarks stay hidden in both folders.

A float goes into `active` if its latest Iridium position time (`latest_iridium_position_datetime`, any SBD message with a valid Iridium position) is less than 30 days before the time the master file is written. Otherwise, including floats with no Iridium time, it goes into `archive`. Floats are listed in the order of the float cache, a `flat_map` keyed by serial number, so in ascending serial number order.

Production example (trimmed). Float 2987 has no GPS fixes, so its trajectory is empty and it has no latest-position placemark:

```
<kml>
  <Document>
    <name>WHOI ALAMO/Hurricane Floats</name>
    <Style id="path1">
    ...
    <Folder>
      <name>active</name>
      <visibility>1</visibility>
      <Document id="2987">
        <name>2987</name>
        <visibility>1</visibility>
        <description>WHOI ALAMO/Hurricane Float #2987</description>
        <Placemark>
          <name>trajectory</name>
          <visibility>1</visibility>
          <styleUrl>#pathB</styleUrl>
          <LineString>
            <altitudeMode>relativeToGround</altitudeMode>
            <coordinates/>
          </LineString>
        </Placemark>
        <Folder>
          <name>surface positions</name>
        </Folder>
      </Document>
      <Document id="9119">
      ...
```

On 2026-10-30 the production master file held 424 float documents (68 active, 356 archived) and 58,350 placemarks; `doc.kml` was 26 MB uncompressed and the KMZ 2.4 MB.

## 7. Styles and icons

Styles come from `util::kml::KmlUtil::SetDefaultKmlStyle()` (`src/utility/KmlUtil.h`, tables in `src/utility/KmlUtil.cpp`). A float's style index is `<serial> mod 13`; index `i` gives the style ID suffix `1`–`9`, `A`–`D` for `i` = 0–12. For example 11014 mod 13 = 3 gives `path4`, `dot1_4`, `dot1s_4`, `tri1_4`.

Each index defines four styles, all in the same color:

| Style ID | Used by | Contents |
|---|---|---|
| `path<k>` | trajectory | `LineStyle`: color, width 5. |
| `dot1_<k>` | latest position | `IconStyle`: color, scale 1.5, icon `http://argo.whoi.edu/kml/img/dot2.png`, hot spot (0.5, 0.5) fraction. `LabelStyle`: scale 1.5. |
| `dot1s_<k>` | surface positions | `IconStyle`: color, scale 0.7, same dot icon and hot spot. `LabelStyle`: scale 0.6. |
| `tri1_<k>` | deployment position | `IconStyle`: color, no scale (1), icon `http://argo.whoi.edu/kml/img/tri2.png`, hot spot (0.5, 0) fraction (bottom center). `LabelStyle`: scale 0.6. |

KML colors are `aabbggrr` hex. All colors have alpha `b0` (69 % opaque):

| Index | Suffix | KML color | Appearance [inferred from the bytes] |
|---|---|---|---|
| 0 | `1` | `b0ff0000` | blue |
| 1 | `2` | `b0ff8000` | azure |
| 2 | `3` | `b0ffff00` | cyan |
| 3 | `4` | `b080ff00` | spring green |
| 4 | `5` | `b000ff00` | green |
| 5 | `6` | `b000ff80` | chartreuse |
| 6 | `7` | `b000ffff` | yellow |
| 7 | `8` | `b00080ff` | orange |
| 8 | `9` | `b00000ff` | red |
| 9 | `A` | `b08000ff` | rose |
| 10 | `B` | `b0ff00ff` | magenta |
| 11 | `C` | `b0ff0080` | violet |
| 12 | `D` | `b0808080` | gray |

The icons are white images tinted by the style color. They are not written by XMTPS; they live in `<dir.kml>/img/` on argus (`dot1.png`, `dot2.png`, `tri1.png`, `tri2.png`, dated 2015). [inferred: `http://argo.whoi.edu/kml/` serves the production `dir.kml`]

## 8. Which positions are used

Only GPS fixes are drawn. Iridium positions are not drawn; they only decide the active/archive folder in the master file (section 6).

The fixes come from `XFloat::GetGpsPositions()` (`src/data/XFloat.cpp`), a database query on the GPS table ([database-schema.md](database-schema.md)):

- `id_instrument` is the float;
- `valid = 1`;
- `id_type <= 3`: diagnostic dive (0x00), start of dive (0x01), end of dive (0x02), mission abort (0x03); built-in-test fixes (0x05) are left out;
- the fix time is after the float's start of mission, if known (the latest built-in-test GPS fix time);
- the fix time is after the deployment time in the `.meta` file, if known;
- ordered by fix time, oldest first.

| Feature | Fixes |
|---|---|
| Trajectory line | all of them |
| Latest position | the last of them (any of types 0x00–0x03) |
| Surface positions folder | types 0x00 and 0x02 only |

GPS fixes with a future date are rejected when they are decoded, so they do not appear. See [xmessage-format.md](xmessage-format.md) A.2 for the GPS message.

## 9. Other files in the KML directory

The production `kml` directory also holds files that XMTPS does not write:

| File | Content |
|---|---|
| `master-auto-update.kmz` | A hand-made `NetworkLink` named `WHOI ALAMO/Hurricane Floats` that loads `http://argo.whoi.edu/kml/master.kmz` and refreshes it every 300 seconds (dated 2017-03-13). |
| `img/dot1.png`, `img/dot2.png`, `img/tri1.png`, `img/tri2.png` | Icons referenced by the styles (dated 2015-08-14). |

On 2026-10-30 the directory had 467 per-float KMZ files: 424 rewritten since the decoder restart on 2026-10-28 22:08 UTC, matching the 424 float documents in the master file, and 43 last written in 2020 or 2021. The 43 old ones belong to floats that are no longer in the decoder's float cache (the instrument table); XMTPS never deletes KML files. [inferred: the cache was not queried directly]

## 10. Readers

- Google Earth users, through `master-auto-update.kmz` or by opening the KMZ files from `http://argo.whoi.edu/kml/`. [inferred]
- No program in this repository reads the KML files. The Python tools draw their own trajectory maps from the JSON files ([plot-and-page-files.md](plot-and-page-files.md)).

## 11. Open questions / discrepancies

1. **Color comments.** The comments in `src/utility/KmlUtil.cpp` label the 13 colors with hues 0°, 30°, ... 330° as if the bytes were red-green-blue. `kmlbase::Color32(a, b, g, r)` takes alpha-blue-green-red, so index 0 is blue, not red, and the sequence runs the other way around the color wheel. The colors are still distinct.
2. **"Most Recent Profile Position" may not be a profile position.** The latest-position placemark uses the last fix of types 0x00–0x03, which can be a start-of-dive fix (0x01) or an abort fix (0x03), and the description gives that fix's dive number as "Profile #". The PHY file and the surface-positions folder use stricter rules.
3. **No KML namespace.** The root element is `<kml>` without the KML 2.2 namespace. Google Earth accepts it; strict KML validators do not. [inferred]
4. **Master file not refreshed on time alone.** The active/archive split is evaluated only when the master file is written, which needs some float to complete a dive. In a period without completed dives a float that passes the 30-day limit stays in `active`. [inferred]
5. **Iridium-only activity does not update the KML.** Messages that do not complete a dive (for example a float sending only engineering data or a failed profile) do not rewrite the float's KML file, though they update its status file and its Iridium time.
6. **Stale files are kept.** Files of floats that leave the cache are never removed (43 in production). The master file does not include them.
7. **Brand text.** All names and descriptions say "WHOI ALAMO/Hurricane Float(s)", also for ALTO floats.
