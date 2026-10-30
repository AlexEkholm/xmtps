#!/usr/bin/env python3
"""Generate the derived formats of the X-message specification.

spec/xmessage-format.md is the source of truth. This script reads it and writes, next to it:

    xmessage-format.json  document tree (sections, blocks, tables) as JSON
    xmessage-format.yaml  the same tree as YAML
    xmessage-format.txt   plain text, 100 columns, tables as text grids
    xmessage-format.tex   LaTeX (XeLaTeX), and xmessage-format.pdf built from it

Run it after every change to the markdown:

    python spec/build-formats.py            # all formats, PDF if xelatex is on the PATH
    python spec/build-formats.py --no-pdf   # skip the LaTeX run

Only the Python standard library is needed. The PDF needs XeLaTeX (MiKTeX or TeX Live) with
fontspec, longtable, booktabs, array, enumitem, fvextra, xcolor, hyperref and the DejaVu fonts.

The parser handles the markdown subset the specification uses: ATX headings, paragraphs,
bullet and ordered lists (nested, with indented code blocks), block quotes, pipe tables,
fenced code blocks and horizontal rules. Inline markup (code spans, bold, italic, links) is
kept as markdown in the JSON/YAML text fields, and rendered in the text and PDF outputs.
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import textwrap

SPEC_DIR = os.path.dirname(os.path.abspath(__file__))
BASENAME = 'xmessage-format'
TEXT_WIDTH = 100

# --------------------------------------------------------------------------------------------------
# Block parser
# --------------------------------------------------------------------------------------------------

HEADING_RE = re.compile(r'^(#{1,6})\s+(.*?)\s*#*\s*$')
FENCE_RE = re.compile(r'^(\s*)(```+|~~~+)(.*)$')
LIST_RE = re.compile(r'^(\s*)([-*+]|\d+[.)])(\s+)(.*)$')
RULE_RE = re.compile(r'^\s{0,3}([-*_])(\s*\1){2,}\s*$')
ALIGN_CELL_RE = re.compile(r'^\s*:?-+:?\s*$')
NUMBER_RE = re.compile(r'^(?:(?:Part|Appendix)\s+([A-Z](?:\.[0-9A-Z]+)*)|([A-Z](?:\.[0-9A-Z]+)+))'
                       r'(?=[\s:])[:\s—–-]*')


def indent_of(line):
    return len(line) - len(line.lstrip(' '))


def split_row(line):
    """Split a pipe-table row into cells; `\\|` is a literal pipe."""
    s = line.strip()
    if s.startswith('|'):
        s = s[1:]
    if s.endswith('|') and not s.endswith('\\|'):
        s = s[:-1]
    cells, cur, i = [], '', 0
    while i < len(s):
        if s[i] == '\\' and i + 1 < len(s) and s[i + 1] == '|':
            cur += '|'
            i += 2
            continue
        if s[i] == '|':
            cells.append(cur.strip())
            cur = ''
        else:
            cur += s[i]
        i += 1
    cells.append(cur.strip())
    return cells


def is_table_start(lines, i):
    if i + 1 >= len(lines) or not lines[i].lstrip().startswith('|'):
        return False
    cells = split_row(lines[i + 1])
    return lines[i + 1].lstrip().startswith('|') and all(ALIGN_CELL_RE.match(c) for c in cells)


def starts_block(lines, i):
    line = lines[i]
    return (FENCE_RE.match(line) or LIST_RE.match(line) or line.lstrip().startswith('>')
            or HEADING_RE.match(line) or RULE_RE.match(line) or is_table_start(lines, i))


def parse_blocks(lines):
    """Parse a list of lines (already dedented to the container) into blocks."""
    blocks, i, n = [], 0, len(lines)
    while i < n:
        line = lines[i]
        if not line.strip():
            i += 1
            continue
        m = FENCE_RE.match(line)
        if m:
            ind, fence, info = len(m.group(1)), m.group(2), m.group(3).strip()
            body, i = [], i + 1
            while i < n and not (lines[i].strip().startswith(fence[0] * len(fence))
                                 and not lines[i].strip().strip(fence[0])):
                l = lines[i]
                body.append(l[min(ind, indent_of(l)):])
                i += 1
            i += 1
            blk = {'type': 'code', 'text': '\n'.join(body)}
            if info:
                blk['language'] = info
            blocks.append(blk)
            continue
        if RULE_RE.match(line):
            blocks.append({'type': 'rule'})
            i += 1
            continue
        if line.lstrip().startswith('>'):
            body = []
            while i < n and lines[i].lstrip().startswith('>'):
                l = lines[i].lstrip()[1:]
                body.append(l[1:] if l.startswith(' ') else l)
                i += 1
            blocks.append({'type': 'blockquote', 'content': parse_blocks(body)})
            continue
        if is_table_start(lines, i):
            header = split_row(lines[i])
            aligns = []
            for c in split_row(lines[i + 1]):
                c = c.strip()
                aligns.append('center' if c.startswith(':') and c.endswith(':') else
                              'right' if c.endswith(':') else 'left')
            rows, i = [], i + 2
            while i < n and lines[i].lstrip().startswith('|'):
                row = split_row(lines[i])
                row = (row + [''] * len(header))[:len(header)]
                rows.append(row)
                i += 1
            blocks.append({'type': 'table', 'align': aligns, 'header': header, 'rows': rows})
            continue
        m = LIST_RE.match(line)
        if m:
            blk, i = parse_list(lines, i)
            blocks.append(blk)
            continue
        m = HEADING_RE.match(line)
        if m:  # heading inside a container: keep as a bold paragraph
            blocks.append({'type': 'paragraph', 'text': '**' + m.group(2) + '**'})
            i += 1
            continue
        para = [line.strip()]
        i += 1
        while i < n and lines[i].strip() and not starts_block(lines, i):
            para.append(lines[i].strip())
            i += 1
        blocks.append({'type': 'paragraph', 'text': ' '.join(para)})
    return blocks


def parse_list(lines, i):
    m = LIST_RE.match(lines[i])
    base = len(m.group(1))
    ordered = m.group(2)[0].isdigit()
    blk = {'type': 'list', 'ordered': ordered}
    if ordered:
        start = int(m.group(2)[:-1])
        if start != 1:
            blk['start'] = start
    items, n = [], len(lines)
    while i < n:
        m = LIST_RE.match(lines[i])
        if not m or len(m.group(1)) != base or m.group(2)[0].isdigit() != ordered:
            break
        content_col = base + len(m.group(2)) + len(m.group(3))
        body = [m.group(4)]
        i += 1
        in_fence = None
        while i < n:
            l = lines[i]
            if in_fence:
                body.append(l[min(content_col, indent_of(l)):])
                if l.strip().startswith(in_fence):
                    in_fence = None
                i += 1
                continue
            if not l.strip():
                # A blank line continues the item only if the next text is indented past the marker.
                j = i
                while j < n and not lines[j].strip():
                    j += 1
                if j < n and indent_of(lines[j]) > base:
                    body.extend([''] * (j - i))
                    i = j
                    continue
                break
            ind = indent_of(l)
            if ind <= base and (LIST_RE.match(l) or starts_block(lines, i)):
                break
            fm = FENCE_RE.match(l)
            if fm:
                in_fence = fm.group(2)
            body.append(l[min(content_col, ind):])
            i += 1
        items.append(parse_blocks(body))
    blk['items'] = items
    return blk, i


def github_slug(text, seen):
    s = plain(text).strip().lower()
    s = ''.join(ch for ch in s if ch.isalnum() or ch in ' -_')
    s = s.replace(' ', '-')
    slug, k = s, 1
    while slug in seen:
        slug = '%s-%d' % (s, k)
        k += 1
    seen.add(slug)
    return slug


def parse_document(text):
    """Split into a section tree on headings outside code fences."""
    lines = text.splitlines()
    root = {'level': 0, 'content_lines': [], 'sections': []}
    stack, seen, in_fence = [root], set(), None
    for line in lines:
        fm = FENCE_RE.match(line)
        if fm and (in_fence is None or fm.group(2).startswith(in_fence)):
            in_fence = fm.group(2)[:3] if in_fence is None else None
        hm = None if in_fence or fm else HEADING_RE.match(line)
        if hm:
            level, heading = len(hm.group(1)), hm.group(2)
            nm = NUMBER_RE.match(heading)
            sec = {'id': github_slug(heading, seen), 'level': level, 'heading': heading}
            if nm:
                sec['number'] = nm.group(1) or nm.group(2)
                sec['title'] = heading[nm.end():]
            else:
                sec['title'] = heading
            sec['content_lines'], sec['sections'] = [], []
            while stack[-1]['level'] >= level:
                stack.pop()
            stack[-1]['sections'].append(sec)
            stack.append(sec)
        else:
            stack[-1]['content_lines'].append(line)

    def finish(sec):
        sec['content'] = [b for b in parse_blocks(sec.pop('content_lines')) if b['type'] != 'rule']
        for s in sec['sections']:
            finish(s)
        sections = sec.pop('sections')
        if sections:
            sec['sections'] = sections
        return sec

    finish(root)
    # The single level-1 heading is the document title.
    top = root['sections'][0]
    doc = {
        'title': top['title'],
        'source': 'spec/%s.md' % BASENAME,
        'note': 'Generated by spec/build-formats.py from spec/%s.md, which is the source of truth. '
                'Text fields hold inline markdown (`code`, **bold**, *italic*, [links](#section-id)); '
                'link targets are section ids.' % BASENAME,
        'content': top['content'],
        'sections': top.get('sections', []),
    }
    return doc


# --------------------------------------------------------------------------------------------------
# Inline markdown
# --------------------------------------------------------------------------------------------------

PUNCT = set('!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~')


def parse_inline(s):
    """Return a list of nodes: ('text', str), ('code', str), ('strong', nodes), ('em', nodes),
    ('link', nodes, url)."""
    out, buf, i, n = [], '', 0, len(s)

    def flush():
        nonlocal buf
        if buf:
            out.append(('text', buf))
            buf = ''

    while i < n:
        c = s[i]
        if c == '\\' and i + 1 < n and s[i + 1] in PUNCT:
            buf += s[i + 1]
            i += 2
            continue
        if c == '`':
            k = i
            while k < n and s[k] == '`':
                k += 1
            run = s[i:k]
            j = s.find(run, k)
            while j != -1 and (j + len(run) < n and s[j + len(run)] == '`'):
                j = s.find(run, j + len(run) + 1)
            if j == -1:
                buf += run
                i = k
                continue
            code = s[k:j]
            if len(code) > 1 and code.startswith(' ') and code.endswith(' ') and code.strip():
                code = code[1:-1]
            flush()
            out.append(('code', code))
            i = j + len(run)
            continue
        if s.startswith('**', i):
            j = find_closing(s, '**', i + 2)
            if j != -1:
                flush()
                out.append(('strong', parse_inline(s[i + 2:j])))
                i = j + 2
                continue
        if c in '*_' and i + 1 < n and s[i + 1] not in (' ', c):
            left_ok = c == '*' or i == 0 or not s[i - 1].isalnum()
            j = find_closing(s, c, i + 1)
            if left_ok and j != -1 and (c == '*' or j + 1 >= n or not s[j + 1].isalnum()):
                flush()
                out.append(('em', parse_inline(s[i + 1:j])))
                i = j + 1
                continue
        if c == '[':
            j = find_closing(s, ']', i + 1)
            if j != -1 and j + 1 < n and s[j + 1] == '(':
                k = s.find(')', j + 2)
                if k != -1:
                    flush()
                    out.append(('link', parse_inline(s[i + 1:j]), s[j + 2:k]))
                    i = k + 1
                    continue
        buf += c
        i += 1
    flush()
    return out


def find_closing(s, delim, start):
    """Find delim after start, skipping code spans and escapes; a single * must not be part of **."""
    i, n = start, len(s)
    while i < n:
        if s[i] == '\\':
            i += 2
            continue
        if s[i] == '`':
            k = i
            while k < n and s[k] == '`':
                k += 1
            j = s.find(s[i:k], k)
            i = (j + (k - i)) if j != -1 else k
            continue
        if s.startswith(delim, i) and i > start and s[i - 1] != ' ':
            if delim == '*' and s.startswith('**', i):
                i += 2
                continue
            return i
        if delim == '*' and s.startswith('**', i):
            j = find_closing(s, '**', i + 2)
            if j != -1:
                i = j + 2
                continue
        i += 1
    return -1


def plain_nodes(nodes):
    out = ''
    for node in nodes:
        if node[0] in ('text', 'code'):
            out += node[1]
        else:
            out += plain_nodes(node[1])
    return out


def plain(s):
    return plain_nodes(parse_inline(s))


# --------------------------------------------------------------------------------------------------
# YAML
# --------------------------------------------------------------------------------------------------

YAML_PLAIN_RE = re.compile(r'^[A-Za-z][A-Za-z0-9 ._/()-]*$')
YAML_RESERVED = {'y', 'n', 'yes', 'no', 'true', 'false', 'on', 'off', 'null', '~'}


def yaml_scalar(v, indent):
    if v is None:
        return 'null'
    if isinstance(v, bool):
        return 'true' if v else 'false'
    if isinstance(v, (int, float)):
        return str(v)
    if '\n' in v and '\t' not in v and not any(l != l.rstrip() for l in v.split('\n')):
        ind = ' ' * indent
        first = next((l for l in v.split('\n') if l), '')
        chomp = '-' if not v.endswith('\n') else '+' if v.endswith('\n\n') else ''
        head = '|' + ('2' if first.startswith(' ') else '') + chomp
        if v.endswith('\n'):
            v = v[:-1]
        return head + '\n' + '\n'.join((ind + l) if l else '' for l in v.split('\n'))
    if YAML_PLAIN_RE.match(v) and v.lower() not in YAML_RESERVED and not v.endswith(' '):
        return v
    return json.dumps(v, ensure_ascii=False)


def to_yaml(v, indent=0):
    pad = ' ' * indent
    if isinstance(v, dict):
        if not v:
            return '{}'
        lines = []
        for k, x in v.items():
            if isinstance(x, (dict, list)) and x:
                lines.append('%s%s:\n%s' % (pad, k, to_yaml(x, indent + 2)))
            else:
                lines.append('%s%s: %s' % (pad, k, yaml_inline(x, indent + 2)))
        return '\n'.join(lines)
    if isinstance(v, list):
        if not v:
            return '[]'
        lines = []
        for x in v:
            if isinstance(x, (dict, list)) and x:
                sub = to_yaml(x, indent + 2)
                lines.append('%s- %s' % (pad, sub[indent + 2:]))
            else:
                lines.append('%s- %s' % (pad, yaml_inline(x, indent + 2)))
        return '\n'.join(lines)
    return pad + yaml_scalar(v, indent)


def yaml_inline(x, indent):
    if isinstance(x, (dict, list)):
        return '{}' if isinstance(x, dict) else '[]'
    return yaml_scalar(x, indent)


# --------------------------------------------------------------------------------------------------
# Plain text
# --------------------------------------------------------------------------------------------------

NBSP = ' '


def plain_nobreak(s):
    """Plain text in which short code spans keep their spaces on one line (as NBSP)."""
    def nodes(ns):
        out = ''
        for node in ns:
            if node[0] == 'code':
                out += node[1].replace(' ', NBSP) if len(node[1]) <= 40 else node[1]
            elif node[0] == 'text':
                out += node[1]
            else:
                out += nodes(node[1])
        return out
    return nodes(parse_inline(s))


def wrap(text, width, first='', rest=''):
    lines = textwrap.wrap(text, width, initial_indent=first, subsequent_indent=rest,
                          break_long_words=False, break_on_hyphens=False) or [first.rstrip()]
    return [l.replace(NBSP, ' ') for l in lines]


def text_table(blk, width):
    header = [plain(c) for c in blk['header']]
    rows = [[plain(c) for c in r] for r in blk['rows']]
    ncol = len(header)
    allrows = [header] + rows
    natural = [max(len(r[k]) for r in allrows) for k in range(ncol)]
    minw = [max([6] + [max((len(w) for w in r[k].split()), default=0) for r in allrows])
            for k in range(ncol)]
    minw = [min(m, 12) for m in minw]
    avail = width - (3 * ncol + 1)
    widths = natural[:]
    while sum(widths) > avail:
        k = max(range(ncol), key=lambda c: widths[c] - minw[c] if widths[c] > minw[c] else -1)
        if widths[k] <= minw[k]:
            break
        widths[k] -= 1
    wrapped = any(len(r[k]) > widths[k] for r in allrows for k in range(ncol))
    sep = '+' + '+'.join('-' * (w + 2) for w in widths) + '+'
    hsep = '+' + '+'.join('=' * (w + 2) for w in widths) + '+'

    def fmt(row, aligns):
        cols = [textwrap.wrap(row[k], widths[k], break_long_words=True, break_on_hyphens=False) or ['']
                for k in range(ncol)]
        h = max(len(c) for c in cols)
        out = []
        for j in range(h):
            parts = []
            for k in range(ncol):
                t = cols[k][j] if j < len(cols[k]) else ''
                parts.append(t.rjust(widths[k]) if aligns[k] == 'right' else
                             t.center(widths[k]) if aligns[k] == 'center' else t.ljust(widths[k]))
            out.append('| ' + ' | '.join(parts) + ' |')
        return out

    out = [sep] + fmt(header, ['left'] * ncol) + [hsep]
    for idx, r in enumerate(rows):
        out += fmt(r, blk['align'])
        if wrapped or idx == len(rows) - 1:
            out.append(sep)
    if not rows:
        out[-1] = sep
    return out


def text_blocks(blocks, width):
    out = []
    for b in blocks:
        t = b['type']
        if t == 'paragraph':
            out += wrap(plain_nobreak(b['text']), width)
        elif t == 'code':
            out += ['    ' + l if l else '' for l in b['text'].split('\n')]
        elif t == 'blockquote':
            out += ['  | ' + l if l else '  |' for l in text_blocks(b['content'], width - 4)]
        elif t == 'table':
            out += text_table(b, width)
        elif t == 'list':
            num = b.get('start', 1)
            for item in b['items']:
                marker = ('%d. ' % num) if b['ordered'] else '- '
                num += 1
                sub = text_blocks(item, width - len(marker))
                for j, l in enumerate(sub):
                    out.append((marker if j == 0 else ' ' * len(marker)) + l if l else '')
            out.append('')
            continue
        out.append('')
    while out and out[-1] == '':
        out.pop()
    # Collapse runs of blank lines (lists add their own).
    res = []
    for l in out:
        if l == '' and res and res[-1] == '':
            continue
        res.append(l)
    return res


def text_document(doc):
    w = TEXT_WIDTH
    title = doc['title']
    out = ['=' * w, title.center(w).rstrip(), '=' * w, '',
           'Plain-text rendering of %s, generated by spec/build-formats.py.' % doc['source'],
           'The markdown file is the source of truth.', '']
    out += text_blocks(doc['content'], w) + ['']
    under = {2: '=', 3: '-', 4: '~', 5: '.', 6: '.'}

    def sec(s):
        h = wrap(plain_nobreak(s['heading']), w)
        out.extend([''] + h + [under[s['level']] * max(len(l) for l in h), ''])
        body = text_blocks(s['content'], w)
        if body:
            out.extend(body + [''])
        for c in s.get('sections', []):
            sec(c)

    for s in doc['sections']:
        sec(s)
    return '\n'.join(l.rstrip() for l in out).rstrip() + '\n'


# --------------------------------------------------------------------------------------------------
# LaTeX
# --------------------------------------------------------------------------------------------------

TEX_SPECIAL = {
    '\\': r'\textbackslash{}', '{': r'\{', '}': r'\}', '$': r'\$', '&': r'\&', '#': r'\#',
    '%': r'\%', '_': r'\_', '~': r'\textasciitilde{}', '^': r'\textasciicircum{}',
}


def tex_escape(s):
    # Curly double quotes: opening at the start or after a space or bracket, closing elsewhere.
    s = re.sub(r'(^|(?<=[\s(\[{—–]))"', '“', s).replace('"', '”')
    out = ''.join(TEX_SPECIAL.get(c, c) for c in s).replace(r'\_', r'\_\allowbreak{}')
    # Stop the TeX ligatures -- --- `` '' from forming.
    out = re.sub(r'-(?=-)', '-{}', out)
    out = re.sub(r"([`'])(?=\1)", r'\1{}', out)
    return out


def tex_code(s):
    out = ''
    for k, c in enumerate(s):
        # Break points: inside camelCase identifiers, and after punctuation.
        if k and c.isupper() and s[k - 1].islower():
            out += r'\allowbreak{}'
        out += TEX_SPECIAL.get(c, c)
        if c in '_/,;|.:=(' or c == '\\':
            out += r'\allowbreak{}'
    out = re.sub(r'-(?=-)', '-{}', out)
    return r'\code{' + out + '}'


def tex_inline(s, labels):
    out = tex_nodes(parse_inline(s), labels)
    # A leading [ would be read as an optional argument of a preceding \\ or \item.
    return '{}' + out if out.startswith('[') else out


def tex_nodes(nodes, labels):
    out = ''
    for node in nodes:
        k = node[0]
        if k == 'text':
            out += tex_escape(node[1])
        elif k == 'code':
            out += tex_code(node[1])
        elif k == 'strong':
            out += r'\textbf{' + tex_nodes(node[1], labels) + '}'
        elif k == 'em':
            out += r'\emph{' + tex_nodes(node[1], labels) + '}'
        elif k == 'link':
            url, inner = node[2], tex_nodes(node[1], labels)
            if url.startswith('#') and url[1:] in labels:
                out += r'\hyperref[sec:%s]{%s}' % (url[1:], inner)
            elif url.startswith('#'):
                sys.stderr.write('warning: link to unknown section %s\n' % url)
                out += inner
            else:
                out += r'\href{%s}{%s}' % (url.replace('%', r'\%').replace('#', r'\#'), inner)
    return out


TEX_LINE_CHARS = 112  # about what fits on one line of a \footnotesize table


def tex_table(b, labels):
    ncol = len(b['header'])
    allrows = [b['header']] + b['rows']
    natural = [max(len(plain(r[k])) for r in allrows) + 1 for k in range(ncol)]
    # Header words are bold, about 15% wider.
    words = [max([4] + [len(w) for r in b['rows'] for w in plain(r[k]).split()]
                 + [int(len(w) * 1.15 + 1) for w in plain(b['header'][k]).split()]) for k in range(ncol)]
    minw = [min(max(w, 4), 16) for w in words]
    # Each column also takes 2\tabcolsep (6pt, about 1.5 characters) of padding.
    avail = TEX_LINE_CHARS - 1.5 * ncol
    widths = natural[:]
    while sum(widths) > avail:
        k = max(range(ncol), key=lambda c: widths[c] - minw[c] if widths[c] > minw[c] else -1)
        if widths[k] <= minw[k]:
            break
        widths[k] -= 1
    total = max(sum(widths), 1)
    scale = min(1.0, avail / total)
    spec = []
    for k in range(ncol):
        frac = widths[k] * scale / TEX_LINE_CHARS
        al = {'right': r'\raggedleft', 'center': r'\centering'}.get(b['align'][k], r'\raggedright')
        spec.append(r'>{%s\arraybackslash}p{%.4f\linewidth}' % (al, frac))
    out = [r'\begingroup\footnotesize\setlength{\tabcolsep}{3pt}',
           r'\begin{longtable}{@{}%s@{}}' % ''.join(spec), r'\toprule']
    head = ' & '.join(r'\textbf{%s}' % tex_inline(c, labels) for c in b['header']) + r' \\'
    out += [head, r'\midrule', r'\endfirsthead', r'\toprule', head, r'\midrule', r'\endhead',
            r'\bottomrule', r'\endlastfoot']
    for r in b['rows']:
        out.append(' & '.join(tex_inline(c, labels) for c in r) + r' \\')
    out += [r'\end{longtable}', r'\endgroup', '']
    return out


def tex_blocks(blocks, labels):
    out = []
    for b in blocks:
        t = b['type']
        if t == 'paragraph':
            out += [tex_inline(b['text'], labels), '']
        elif t == 'code':
            out += [r'\begin{Verbatim}', b['text'], r'\end{Verbatim}', '']
        elif t == 'blockquote':
            out += [r'\begin{notebox}'] + tex_blocks(b['content'], labels) + [r'\end{notebox}', '']
        elif t == 'table':
            out += tex_table(b, labels)
        elif t == 'list':
            env = 'enumerate' if b['ordered'] else 'itemize'
            opt = '[start=%d]' % b['start'] if 'start' in b else ''
            out.append(r'\begin{%s}%s' % (env, opt))
            for item in b['items']:
                body = tex_blocks(item, labels)
                while body and body[-1] == '':
                    body.pop()
                out.append(r'\item ' + (body[0] if body else ''))
                out += body[1:]
            out += [r'\end{%s}' % env, '']
    return out


TEX_PREAMBLE = r"""%% Generated by spec/build-formats.py from spec/%(base)s.md. Do not edit; edit the markdown.
\documentclass[10pt,letterpaper]{article}
\usepackage[margin=0.8in]{geometry}
\usepackage{fontspec}
\setmainfont{DejaVu Serif}[Scale=0.92]
\setsansfont{DejaVu Sans}[Scale=0.92]
\setmonofont{DejaVu Sans Mono}[Scale=0.84]
\usepackage{array,longtable,booktabs}
\usepackage[shortlabels]{enumitem}
\setlist{itemsep=2pt,topsep=3pt,parsep=2pt}
\usepackage{fvextra}
\fvset{breaklines=true,breakanywhere=true,fontsize=\footnotesize,frame=leftline,framesep=6pt,
  xleftmargin=8pt,rulecolor=\color{black!35}}
\usepackage{xcolor}
\usepackage[framemethod=default]{mdframed}
\newmdenv[topline=false,bottomline=false,rightline=false,linewidth=2pt,linecolor=black!30,
  backgroundcolor=black!4,innerleftmargin=8pt,innertopmargin=4pt,innerbottommargin=4pt,
  skipabove=6pt,skipbelow=6pt]{notebox}
\usepackage[hidelinks,bookmarksnumbered=false,bookmarksopen=true,bookmarksopenlevel=1,
  pdftitle={%(title)s},pdfsubject={Generated from spec/%(base)s.md}]{hyperref}
\newcommand{\code}[1]{\texttt{#1}}
\setlength{\parindent}{0pt}
\setlength{\parskip}{4pt plus 2pt}
\setcounter{secnumdepth}{0}
\setcounter{tocdepth}{3}
\setlength{\LTpre}{4pt}
\setlength{\LTpost}{6pt}
\renewcommand{\arraystretch}{1.15}
\makeatletter
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}{2.5ex plus 1ex minus .2ex}{0.8ex}%%
  {\normalfont\normalsize\bfseries\itshape}}
\makeatother
\emergencystretch=3em
\tolerance=1000
\title{%(title_tex)s}
\author{XMTPS}
\date{Generated from \texttt{spec/%(base)s.md} by \texttt{spec/build-formats.py}}
\begin{document}
\maketitle
"""

TEX_COMMANDS = {2: 'section', 3: 'subsection', 4: 'subsubsection', 5: 'paragraph', 6: 'paragraph'}


def tex_document(doc):
    labels = set()

    def collect(s):
        labels.add(s['id'])
        for c in s.get('sections', []):
            collect(c)

    for s in doc['sections']:
        collect(s)
    out = [TEX_PREAMBLE % {'base': BASENAME, 'title': plain(doc['title']),
                           'title_tex': tex_inline(doc['title'], labels)}]
    out += tex_blocks(doc['content'], labels)
    out += [r'\tableofcontents', r'\clearpage', '']

    def sec(s):
        cmd = TEX_COMMANDS[s['level']]
        title = tex_inline(s['heading'], labels)
        bm = plain(s['heading'])
        if cmd == 'section' and out[-2:] != [r'\clearpage', '']:
            out.append(r'\clearpage' if s.get('number') else '')
        out.append(r'\%s{\texorpdfstring{%s}{%s}}\label{sec:%s}' % (cmd, title, tex_escape(bm), s['id']))
        if cmd == 'paragraph':
            out.append(r'\mbox{}\par')
        out.append('')
        out.extend(tex_blocks(s['content'], labels))
        for c in s.get('sections', []):
            sec(c)

    # The markdown's hand-written Contents list is replaced by \tableofcontents.
    for s in doc['sections']:
        if s['id'] != 'contents':
            sec(s)
    out.append(r'\end{document}')
    return '\n'.join(out) + '\n'


def build_pdf(tex_path):
    exe = shutil.which('xelatex')
    if not exe:
        sys.stderr.write('xelatex not found; skipping the PDF\n')
        return False
    outdir = os.path.dirname(tex_path)
    name = os.path.basename(tex_path)
    # Three runs: the TOC, then longtable column widths and page references settle.
    for _ in range(3):
        r = subprocess.run([exe, '-interaction=nonstopmode', '-halt-on-error', name],
                           cwd=outdir, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if r.returncode:
            sys.stderr.write(r.stdout.decode('utf-8', 'replace')[-4000:])
            sys.stderr.write('\nxelatex failed; see %s\n' % tex_path.replace('.tex', '.log'))
            return False
    for ext in ('.aux', '.toc', '.out', '.log'):
        p = tex_path[:-4] + ext
        if os.path.exists(p):
            os.remove(p)
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__.split('\n\n')[0])
    parser.add_argument('--no-pdf', action='store_true', help='write the .tex but do not run xelatex')
    args = parser.parse_args()

    src = os.path.join(SPEC_DIR, BASENAME + '.md')
    with open(src, encoding='utf-8') as f:
        doc = parse_document(f.read())

    def write(ext, text):
        path = os.path.join(SPEC_DIR, BASENAME + ext)
        with open(path, 'w', encoding='utf-8', newline='\n') as f:
            f.write(text)
        print('wrote', os.path.relpath(path))
        return path

    write('.json', json.dumps(doc, indent=2, ensure_ascii=False) + '\n')
    write('.yaml', '# Generated by spec/build-formats.py from spec/%s.md. Do not edit.\n' % BASENAME
          + to_yaml(doc) + '\n')
    write('.txt', text_document(doc))
    tex = write('.tex', tex_document(doc))
    if not args.no_pdf and build_pdf(tex):
        print('wrote', os.path.relpath(tex[:-4] + '.pdf'))


if __name__ == '__main__':
    main()
