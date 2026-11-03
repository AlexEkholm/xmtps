# `bin/` — service units and tools

This directory holds the systemd units that run XMTPS in production and, in [`py/`](py/README.md),
the Python tools that turn the decoder's listing files into JSON, plots and web pages. The decoder
itself is built from `src/` (see [Building](../README.md#building)); its binary is not kept here.

| File | What it is |
|---|---|
| [`xmtps-alamo.service`](#xmtps-alamoservice) | systemd unit for the decoder daemon |
| [`xmtps-alamo-plot.service`](#xmtps-alamo-plotservice) | systemd unit for the plot-generation loop |
| [`py/`](py/README.md) | Python tools: listing → JSON conversion, aggregation and plots, summary and engineering pages, and the plot service loop |

## Production layout

On `argus.whoi.edu`, production runs from `/data1/alamo/`:

- `/data1/alamo/bin/xmtps` is a link to the Release build, `/data1/alamo/src/xmtps/Release/xmtps`.
- `/data1/alamo/bin/py/` holds one symbolic link per script into `/data1/alamo/src/xmtps/bin/py/`,
  not one link for the whole directory, so a new script needs its own link. It also holds the
  Python virtual environment, `.venv`.
- The installed unit files are copies in `/etc/systemd/system/`. After installing or changing a
  unit, run `sudo systemctl daemon-reload`, then restart the service.

## `xmtps-alamo.service`

Runs the decoder daemon:

```
/data1/alamo/bin/xmtps -v --config /data1/alamo/etc/xmtps.cfg --daemon --pid-file /data1/alamo/var/lock/xmtps.pid
```

| Setting | Value | Why |
|---|---|---|
| `User` / `Group` | `argo` / `floatgroup` | owner of the data directories |
| `Restart` / `RestartSec` | `always` / `30` | a startup failure (for example MySQL not yet running) keeps retrying without hitting systemd's start limit |
| `TimeoutStopSec` | `300` | gives the daemon 5 minutes to finish the messages still queued before systemd kills it |
| `StandardOutput` / `StandardError` | `null` | the daemon writes its own logs to `/data1/alamo/var/log/xmtps/` (see [spec/log-and-lock-files.md](../spec/log-and-lock-files.md)) |
| `PIDFile` | `/data1/alamo/var/lock/xmtps.pid` | the run-time lock the daemon takes at startup |

Start, stop or restart it with `sudo systemctl {start|stop|restart} xmtps-alamo`. A newly built
decoder runs only after a restart. The daemon's options are described in
[Configuration](../README.md#configuration) and [Running](../README.md#running).

## `xmtps-alamo-plot.service`

Runs [`py/update-plots-service.sh`](py/README.md#update-plots-servicesh), the loop that keeps the
JSON files, plots and pages up to date. It runs as `aekholm:floatgroup` with `Restart=always` and
`RestartSec=30`, and records its PID in `/data1/alamo/var/lock/xmtps-plot.pid`.

The script loops forever, so systemd treats it as a long-running service. Restart the service
(`sudo systemctl restart xmtps-alamo-plot`) after changing the script itself; changes to the Python
tools are picked up on the next pass without a restart.
