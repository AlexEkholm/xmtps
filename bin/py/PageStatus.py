# -*- coding: utf-8 -*-
"""Status line for the footer of the float summary page (xmtps-generate-summary-page.py).

The footer reads, for example:

    Page last updated 2026-10-26T17:40Z (105 minutes ago, runtime: 3 minutes) - python version
    0.11 - decoder status: active (uptime: 3 hours, 4 minutes) - xmtps version 1.0.3318b
    (built 2026-10-26)

The time since the update ("105 minutes ago") is filled in by the browser when the page is viewed
(see ELAPSED_SCRIPT); everything else is fixed when the page is written. The decoder status and
uptime come from systemd (`systemctl show`, which needs no privileges), the decoder version from
`xmtps --version`, which prints its version and exits without touching the running daemon.
Anything that cannot be determined is shown as "unknown".

Author: aekholm
"""

import datetime
import functools
import re
import subprocess

DECODER_SERVICE = 'xmtps-alamo'
DECODER_BINARY = '/data1/alamo/bin/xmtps'

# Fills each <span class="page-updated" epoch="..." runtime="..."> with "(N minutes ago" plus
# ", runtime: ..." when the page has a runtime, and ")". Without JavaScript the span keeps the
# "(runtime: ...)" text it was written with.
ELAPSED_SCRIPT = '''<script type="text/javascript">
function pageElapsedString(dt) {
    if (dt < 120) return "just now";
    if (dt < 10800) return Math.floor(dt / 60) + " minutes ago";
    if (dt < 345600) return Math.floor(dt / 3600) + " hours ago";
    return Math.floor(dt / 86400) + " days ago";
}
document.addEventListener("DOMContentLoaded", function () {
    var now = Math.floor(Date.now() / 1000);
    document.querySelectorAll("span.page-updated").forEach(function (elem) {
        var dt = now - elem.getAttribute("epoch");
        var runtime = elem.getAttribute("runtime");
        if (dt < 0) return;
        elem.textContent = "(" + pageElapsedString(dt) + (runtime ? ", runtime: " + runtime : "") + ")";
    });
});
</script>'''


def FormatDuration(seconds):
    """Format a duration in words: '45 seconds', '3 minutes', '3 hours, 4 minutes',
    '2 days, 5 hours'."""
    def unit(n, name):
        return '{} {}{}'.format(n, name, '' if n == 1 else 's')

    s = int(max(0, round(seconds)))
    if s < 60:
        return unit(s, 'second')
    if s < 3600:
        return unit(s // 60, 'minute')
    if s < 86400:
        h, m = divmod(s // 60, 60)
        return unit(h, 'hour') + (', ' + unit(m, 'minute') if m else '')
    d, h = divmod(s // 3600, 24)
    return unit(d, 'day') + (', ' + unit(h, 'hour') if h else '')


def _SystemdTime(value):
    """Parse a systemd timestamp ('@1793044815' or 'Mon 2026-10-26 20:00:15 UTC') to epoch
    seconds, or None if it is empty or cannot be parsed."""
    value = value.strip()
    if not value or value == 'n/a':
        return None
    if value.startswith('@'):
        try:
            return float(value[1:])
        except ValueError:
            return None
    m = re.search(r'(\d{4}-\d\d-\d\d \d\d:\d\d:\d\d) UTC', value)
    if m:
        return datetime.datetime.strptime(m.group(1), '%Y-%m-%d %H:%M:%S').replace(
            tzinfo=datetime.timezone.utc).timestamp()
    return None


@functools.lru_cache(maxsize=None)
def DecoderStatus(service=DECODER_SERVICE):
    """Return the decoder service's status text, e.g. 'active (uptime: 3 hours, 4 minutes)' or
    'failed (for 2 hours, 1 minute)', or 'unknown'."""
    try:
        out = subprocess.run(
            ['systemctl', 'show', service, '--timestamp=unix',
             '--property=ActiveState,ActiveEnterTimestamp,StateChangeTimestamp'],
            capture_output=True, text=True, timeout=10).stdout
    except (OSError, subprocess.SubprocessError):
        return 'unknown'
    props = dict(line.split('=', 1) for line in out.splitlines() if '=' in line)
    state = props.get('ActiveState', '')
    if not state:
        return 'unknown'
    now = datetime.datetime.now(datetime.timezone.utc).timestamp()
    if state == 'active':
        since = _SystemdTime(props.get('ActiveEnterTimestamp', ''))
        return 'active (uptime: {})'.format(FormatDuration(now - since)) if since else 'active'
    since = _SystemdTime(props.get('StateChangeTimestamp', ''))
    return '{} (for {})'.format(state, FormatDuration(now - since)) if since else state


@functools.lru_cache(maxsize=None)
def DecoderVersion(binary=DECODER_BINARY):
    """Return the decoder's version text, e.g. '1.0.3318b (built 2026-10-26)', or 'unknown'."""
    try:
        out = subprocess.run([binary, '--version'], capture_output=True, text=True,
                             timeout=10).stdout
    except (OSError, subprocess.SubprocessError):
        return 'unknown'
    m = re.search(r'\bv(\S+) \(built (\d{4}-\d\d-\d\d)', out)
    return '{} (built {})'.format(m.group(1), m.group(2)) if m else 'unknown'


def WriteStatusFooter(doc, tag, text, json_version, runtime_s=None):
    """Write the footer status line into a yattag document.

    Args:
        doc, tag, text: the yattag Doc and its tag and text functions.
        json_version: the Python tools' JSON format version, shown as the python version.
        runtime_s: the duration of the JSON and plot update run in seconds, or None to leave the
            runtime out.
    """
    now = datetime.datetime.now(datetime.timezone.utc)
    runtime = FormatDuration(runtime_s) if runtime_s is not None else ''
    text('Page last updated ' + now.isoformat()[:16] + 'Z ')
    with tag('span', ('class', 'page-updated'), ('epoch', str(round(now.timestamp()))),
             ('runtime', runtime)):
        if runtime:
            text('(runtime: ' + runtime + ')')
    text(' - python version ' + json_version
         + ' - decoder status: ' + DecoderStatus()
         + ' - xmtps version ' + DecoderVersion())
    doc.asis(ELAPSED_SCRIPT)
