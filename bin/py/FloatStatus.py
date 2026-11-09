"""A float's status and last known location, shared by the summary and mission parameters pages.

LastLocation(summary) names the sea or ocean of the float's last known position ("WHOI" near
Woods Hole; SeaNames.SeaName), and FloatStatus(summary, location) gives its status: PREDEPLOYMENT
when last seen at WHOI, otherwise ACTIVE, OVERDUE or ARCHIVED by the days since its latest SBD
message. Each status has a name, a table icon (relative to the plots directory) and a map marker
color and size.
"""

import datetime
import logging

from SeaNames import SeaName

logger = logging.getLogger(__name__)

STATUS_PREDEPLOYMENT = dict(name='PREDEPLOYMENT', img='img/s3.png', color='rgb(255,168,64)', size=6)
STATUS_BY_AGE = ((30, dict(name='ACTIVE', img='img/s2.png', color='rgb(123,255,123)', size=6)),
                 (180, dict(name='OVERDUE', img='img/s1.png', color='rgb(123,123,255)', size=5)),
                 (float('inf'), dict(name='ARCHIVED', img='img/s0.png', color='rgb(255,123,123)', size=4)))


def FloatStatus(summary, location):
    """Return the status dict (STATUS_PREDEPLOYMENT or one of STATUS_BY_AGE) of a float.

    Args:
        summary: the float's summary (the latest SBD time gives the days since its last report).
        location: its LastLocation(); "WHOI" makes it PREDEPLOYMENT.
    """
    if location == 'WHOI':
        return STATUS_PREDEPLOYMENT
    days = (datetime.datetime.now(datetime.UTC).timestamp() - summary['latest']['sbd'].dt_epoch) / 86400
    for limit, status in STATUS_BY_AGE:
        if days < limit:
            return status
    return STATUS_BY_AGE[-1][1]


def LastLocation(summary):
    """Return the sea or ocean of a float's last known position, "WHOI" near Woods Hole, or ''.

    The position is the latest valid one of the summary's latest GPS fix and latest SBD message
    (Iridium position); SeaNames.SeaName names it.
    """
    best = None
    for key in ('gps', 'sbd'):
        p = summary.get('latest', {}).get(key)
        try:
            if p is not None and p.is_valid() and (best is None or p.dt_epoch > best.dt_epoch):
                best = p
        except Exception:
            continue
    if best is None:
        return ''
    try:
        return SeaName(best.lat, best.lon)
    except Exception as e:
        logger.warning('Failed to name the sea of {}, {}: {}'.format(best.lat, best.lon, e))
        return ''
