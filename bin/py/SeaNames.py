"""Name the sea or ocean of a position, for the float summary table.

SeaName(lat, lon) returns "WHOI" within WHOI_RADIUS_KM of Woods Hole (floats tested or stored
there), else the name of the Natural Earth marine area that contains the position (the smallest
one, so "Labrador Sea" rather than "North Atlantic Ocean"), else that of the nearest area, for a
position on land or in a gap between areas. The areas are Natural Earth's
ne_10m_geography_marine_polys (public domain), converted to data/marine-areas.json by keeping the
named polygons with coordinates rounded to 0.01 degree.
"""

import json
import math
import os

# Woods Hole Oceanographic Institution, Woods Hole, MA
WHOI_LAT, WHOI_LON = 41.524, -70.671
WHOI_RADIUS_KM = 25.0

_DATA = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'data', 'marine-areas.json')
_areas = None


def _load():
    global _areas
    if _areas is None:
        with open(_DATA, encoding='utf-8') as f:
            _areas = json.load(f)['features']
        for a in _areas:
            x0, y0, x1, y1 = a['bbox']
            a['size'] = (x1 - x0) * (y1 - y0)
    return _areas


def _distance_km(lat1, lon1, lat2, lon2):
    """Great-circle distance in km (haversine, spherical Earth)."""
    p1, p2 = math.radians(lat1), math.radians(lat2)
    dp, dl = p2 - p1, math.radians(lon2 - lon1)
    h = math.sin(dp / 2) ** 2 + math.cos(p1) * math.cos(p2) * math.sin(dl / 2) ** 2
    return 2 * 6371.0 * math.asin(min(1.0, math.sqrt(h)))


def _contains(rings, lon, lat):
    """Even-odd ray casting over all rings, so holes are left out."""
    inside = False
    for ring in rings:
        j = len(ring) - 1
        for i in range(len(ring)):
            xi, yi = ring[i]
            xj, yj = ring[j]
            if (yi > lat) != (yj > lat) and lon < (xj - xi) * (lat - yi) / (yj - yi) + xi:
                inside = not inside
            j = i
    return inside


def SeaName(lat, lon):
    """Return "WHOI", the name of the sea or ocean at (lat, lon), or '' for an invalid position.

    Args:
        lat: latitude in degrees north.
        lon: longitude in degrees east (any range; wrapped to -180..180).
    """
    try:
        lat, lon = float(lat), float(lon)
    except (TypeError, ValueError):
        return ''
    if not (math.isfinite(lat) and math.isfinite(lon)) or abs(lat) > 90:
        return ''
    lon = (lon + 180.0) % 360.0 - 180.0
    if _distance_km(lat, lon, WHOI_LAT, WHOI_LON) <= WHOI_RADIUS_KM:
        return 'WHOI'

    areas = _load()
    inside = [ a for a in areas
               if a['bbox'][0] <= lon <= a['bbox'][2] and a['bbox'][1] <= lat <= a['bbox'][3]
               and _contains(a['rings'], lon, lat) ]
    if inside:
        return min(inside, key=lambda a: a['size'])['name']

    # On land or between areas: the area with the nearest vertex, looking first at the areas whose
    # bounding box is within 10 degrees
    best, best_d = None, float('inf')
    for pad in (10.0, 180.0):
        for a in areas:
            x0, y0, x1, y1 = a['bbox']
            if not (x0 - pad <= lon <= x1 + pad and y0 - pad <= lat <= y1 + pad):
                continue
            for ring in a['rings']:
                for x, y in ring:
                    d = _distance_km(lat, lon, y, x)
                    if d < best_d:
                        best, best_d = a, d
        if best is not None:
            break
    return best['name'] if best is not None else ''
