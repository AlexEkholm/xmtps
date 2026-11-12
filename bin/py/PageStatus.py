# -*- coding: utf-8 -*-
"""Status lines for the footer of the summary, mission and startup pages and the float plot pages.

The footer reads, for example (one item per line):

    Page last updated 2026-10-26T17:40Z (105 minutes ago, runtime: 3 minutes)
    Python utility version 0.11
    Decoder status: active (uptime: 3 hours, 4 minutes)
    XMTPS version 1.0.3318b (built 2026-10-26)

The float plot pages leave out the decoder status line, and their runtime is the time spent
writing that float's plots.

The time since the update ("105 minutes ago") is filled in by the browser when the page is viewed
(see ELAPSED_SCRIPT); everything else is fixed when the page is written. The decoder status and
uptime come from systemd (`systemctl show`, which needs no privileges), the decoder version from
`xmtps --version`, which prints its version and exits without touching the running daemon.
Anything that cannot be determined is shown as "unknown".

Author: aekholm
"""

import datetime
import functools
import json
import logging
import os
import re
import subprocess

logger = logging.getLogger(__name__)

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


def WriteStatusFooter(doc, tag, text, json_version, runtime_s=None, decoder_status=True):
    """Write the footer status lines into a yattag document.

    Args:
        doc, tag, text: the yattag Doc and its tag and text functions.
        json_version: the Python tools' JSON format version, shown as the Python utility version.
        runtime_s: the duration of the JSON and plot update run in seconds, or None to leave the
            runtime out.
        decoder_status: whether to include the "Decoder status" line. The float plot pages
            (xmtps-aggregate-json.py) leave it out.
    """
    now = datetime.datetime.now(datetime.timezone.utc)
    runtime = FormatDuration(runtime_s) if runtime_s is not None else ''
    text('Page last updated ' + now.isoformat()[:16] + 'Z ')
    with tag('span', ('class', 'page-updated'), ('epoch', str(round(now.timestamp()))),
             ('runtime', runtime)):
        if runtime:
            text('(runtime: ' + runtime + ')')
    # One item per line. The breaks are tags, not text (text() escapes them), and the first one
    # is outside the span, whose text the script replaces.
    lines = ['Python utility version ' + json_version]
    if decoder_status:
        lines.append('Decoder status: ' + DecoderStatus())
    lines.append('XMTPS version ' + DecoderVersion())
    for line in lines:
        doc.stag('br')
        text(line)
    doc.asis(ELAPSED_SCRIPT)


def HasWimsData(summary_fpath, serial):
    """Return whether a float has valid WIMS wave data, from the aggregate <serial>_wave.json file
    next to its <serial>_summary.json file (summary_fpath).

    Valid means at least one wave measurement with more than one spectral bin (slen > 1), as the
    energy estimate counts a WIMS run. A missing or unreadable file means no.
    """
    fpath = os.path.join(os.path.dirname(summary_fpath), '{}_wave.json'.format(serial))
    if not os.path.isfile(fpath):
        return False
    try:
        with open(fpath, 'r', encoding='utf-8') as f:
            wave = json.load(f)
        i = wave['columns'].index('slen')
        return any(isinstance(r[i], (int, float)) and r[i] > 1 for r in wave['data'])
    except Exception as e:
        logger.warning('Failed to read WIMS data from {}: {}'.format(fpath, e))
        return False


def FloatSubtype(ctdtype, wims):
    """Return the subtype shown under the float type in the fleet tables: the CTD type (e.g. RBR),
    with /WIMS for a float with valid WIMS wave data, e.g. 'RBR/WIMS', 'WIMS', or ''."""
    return '/'.join([t for t in (ctdtype or '', 'WIMS' if wims else '') if t])


# The fleet table pages, linked from the top of each: (key, title, path under the plots directory)
TABLE_PAGES = (
    ('summary', 'Summary', ''),
    ('startup', 'Startups', 'engineering/startup.html'),
    ('params', 'Parameters', 'engineering/params.html'),
    ('energy', 'Energy', 'engineering/energy.html'),
    )

# DataTables paging and search of the fleet table pages: the page size and the search text start
# from the URL parameters 'n' and 's' (TABLE_URL_SCRIPT, which each page must include, defines
# pageSizeFromUrl and searchFromUrl); wrap the DataTable in BindTableUrl to write changes back.
TABLE_PAGING = ('"pageLength": pageSizeFromUrl(), "search": { "search": searchFromUrl() }, '
                '"lengthMenu": [[ 10, 20, 50, 100, -1 ], [ 10, 20, 50, 100, "ALL" ]]')

# URL parameters of the fleet table pages, kept up to date with replaceState (no history entries)
# unless noted:
#   n  the table's page size: 10, 20, 50, 100 or 'all'; anything else is dropped; left out for 10.
#   s  the table's search text, as typed into its search field; left out when empty.
#   r  auto-refresh, on pages with the Auto Refresh checkbox (WriteAutoRefresh): reload every r
#      seconds; 1 means 60, others are at least 10 and at most 86400; anything that is not a
#      number is dropped. The checkbox sets r=1 or removes it
#      (pushState, as before on the summary page).
TABLE_URL_SCRIPT = '''<script type="text/javascript">
var PAGE_SIZES = [10, 20, 50, 100, -1];
function pageSizeUrl(len) {
    const url = new URL(window.location.href);
    if (len === 10) url.searchParams.delete("n");
    else url.searchParams.set("n", len === -1 ? "all" : String(len));
    window.history.replaceState(window.history.state, '', url.toString());
}
function pageSizeFromUrl() {
    const n = new URL(window.location.href).searchParams.get("n");
    if (n === null) return 10;
    const len = n.toLowerCase() === "all" ? -1 : parseInt(n, 10);
    if (PAGE_SIZES.indexOf(len) < 0) { pageSizeUrl(10); return 10; }
    return len;
}
function searchUrl(s) {
    const url = new URL(window.location.href);
    if (url.searchParams.get("s") === (s || null)) return;
    if (s) url.searchParams.set("s", s);
    else url.searchParams.delete("s");
    window.history.replaceState(window.history.state, '', url.toString());
}
function searchFromUrl() {
    return new URL(window.location.href).searchParams.get("s") || "";
}
function BindTableUrl(dt) {
    dt.on('length.dt', function (e, settings, len) { pageSizeUrl(len); });
    dt.on('search.dt', function () { searchUrl(dt.search()); });
    return dt;
}
var reloading;
var REFRESH_MAX_S = 86400;
function checkAutoRefresh() {
    const cb = document.getElementById("autorefresh");
    if (!cb) return;
    const url = new URL(window.location.href);
    const urlparams = url.searchParams;
    let r = +urlparams.get("r");
    // Not a number (?r=abc): NaN would make setTimeout reload at once, again and again
    if (!Number.isFinite(r) || r < 1) {
        if (urlparams.has("r")) {
            urlparams.delete("r");
            window.history.pushState({}, '', url.toString());
        }
        return;
    } else if (r == 1) {
        r = 60;
    } else if (r < 10) {
        r = 10;
        urlparams.set("r", "10");
        window.history.pushState({}, '', url.toString());
    } else if (r > REFRESH_MAX_S) {
        // A delay over 2^31-1 ms would overflow and also reload at once
        r = REFRESH_MAX_S;
        urlparams.set("r", String(REFRESH_MAX_S));
        window.history.pushState({}, '', url.toString());
    }
    reloading = setTimeout(function () { window.location.reload(); }, r * 1000);
    cb.checked = true;
}
function toggleAutoRefresh(cb) {
    const url = new URL(window.location.href);
    const urlparams = url.searchParams;
    if (cb.checked) {
        urlparams.set("r", "1");
        reloading = setTimeout(function () { window.location.reload(); }, 60000);
    } else {
        urlparams.delete("r");
        clearTimeout(reloading);
    }
    window.history.pushState({}, '', url.toString());
}
$(document).ready(checkAutoRefresh);
</script>'''


def WriteAutoRefresh(doc, tag, text):
    """Write the "Auto Refresh" checkbox (URL parameter 'r', TABLE_URL_SCRIPT) into a yattag
    document, as the right-hand part of the footer: a div with id 'rfoot', next to 'lfoot'."""
    with tag('div', id='rfoot', style='float: right; margin: 0 0 30px 0;'):
        with tag('label', ('for', 'autorefresh')):
            text('Auto Refresh ')
            doc.input(name='autorefresh', type='checkbox', id='autorefresh', value='no',
                      onclick='toggleAutoRefresh(this);', style='margin: 3px; vertical-align: middle;')


def WritePageLinks(doc, tag, text, current, plots_url):
    """Write the links to the fleet table pages (TABLE_PAGES) into a yattag document, as a div with
    id 'tlinks' under the page title; the current page is shown as plain text.

    Args:
        doc, tag, text: the yattag Doc and its tag and text functions.
        current: the TABLE_PAGES key of this page.
        plots_url: the URL of the plots directory relative to this page ('.' for the summary page,
            '..' or '/alamo' for the pages in engineering/).
    """
    with tag('div', id='tlinks', style='margin-top: 6px; font-size: 14px; color: #aaa;'):
        for i, (key, title, path) in enumerate(TABLE_PAGES):
            if i:
                text(' | ')
            if key == current:
                with tag('span', style='color: #555;'):
                    text(title)
            else:
                with tag('a', href='{}/{}'.format(plots_url, path)):
                    text(title)
