#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""First-descent seek and ascent-rate control of ALAMO/ALTO floats: simulations.

Compares the present seek rule (reconstructed from the documentation, fleet settings) with
second-order rules that use the vertical rate, with windowed and Kalman-filter state
estimation. Module (used by AscentSim.py and ControlFigures.py) and script:

    python SeekSim.py --n 80 --jobs 8 --out seek_sim.json

Model:

Plant (quasi-steady, drag-dominated; inertia neglected, acceleration time ~ tens of s):
    dV(p, O) = O - O_T + int_{p_T}^{p} k(p') dp'          excess volume (mL, + buoyant)
    k(p)     = k_c + k_s exp(-p / h_s)                    stiffness (mL/dBar): hull vs water
                                                          compressibility + stratification
    y        = w|w| = 2 g dV 1e-6 / C_dA                  (m/s)^2, w positive up
    dp/dt    = -w + d(eta)/dt                              dBar ~ m; eta: isopycnal heave (down +)
The stratification part of k acts at the heaved coordinate p - eta (the float sees the displaced
isopycnals); compressibility acts at p. eta = A [semidiurnal tide + 2 h wave] + (A / 3) x three
waves of 8-40 min period, random phases: A = 5 m gives vertical water speeds of ~1-2 cm/s,
A = 15 m of ~3-6 cm/s.
Pressure is measured with noise; the controller sees only pressure (and its own run times).

Key identity used by the second-order rule: between operations y(p) is linear in p with slope
s = 2 g k / C_dA, so the measured pressure series gives the float's present excess volume up to
the scale C_dA / (2 g); a pump pulse of known duration gives that scale in pump-seconds per unit
y (G = dy / dt_pump). The pump time that puts the neutral point at the target is then
    t = -(y_now + s (p_T - p_now)) / G
whatever the float's speed: no need to wait for w ~ 0, and q and C_dA need not be known.
"""

import argparse
import json
import numpy as np

G0 = 9.81
PUMP_POWER = {'ALTO': (2.93, 0.00491), 'ALAMO': (3.07, 0.00396)}   # W, W/dBar (fleet fit)

# ---------------------------------------------------------------- plant


class Float:
    def __init__(self, rng, ftype, p_T, iw_sigma, strat, prior_err, kappa_spread):
        self.rng = rng
        self.p_T = p_T
        if ftype == 'ALTO':
            V = 18.3e3                                   # mL
            cda = 0.07 * np.exp(rng.normal(0, 0.3))      # fleet spread ~ +-30 %
            k_rel = 2.42e-6 * (1 + rng.normal(0, kappa_spread))
            self.q_p = 0.30 * (1 + rng.normal(0, 0.12))  # mL/s (fit: 15-18 mL/min)
            self.q_v_deep = 0.09 * np.exp(rng.normal(0, 0.5))   # mL/s above the fuse (5.5 mL/min)
            self.q_v_shallow = self.q_v_deep * 3
            self.p_fuse = 25.0
            D_nom = 40.0                                 # mL the ballast plan expects to pump
        else:
            V = 9.7e3
            cda = 0.033 * np.exp(rng.normal(0, 0.3))
            k_rel = 2.42e-6 * (1 + rng.normal(0, kappa_spread))   # ALAMO not measured
            self.q_p = 0.30 * (1 + rng.normal(0, 0.12))
            self.q_v_deep = 0.09 * np.exp(rng.normal(0, 0.5))
            self.q_v_shallow = self.q_v_deep * 3
            self.p_fuse = 25.0
            D_nom = 20.0
        gamma_w = 4.2e-6                                 # seawater compressibility, 1/dBar (mid-depth)
        self.k_c = V * (gamma_w - k_rel)                 # ~0.033 mL/dBar ALTO
        # stratification term: strong thermocline (k_s0) decaying with depth
        self.k_s0, self.h_s = {'strong': (V * 1.2e-5, 250.0), 'weak': (V * 1.5e-6, 400.0),
                               'none': (0.0, 1.0)}[strat]
        self.cda = cda
        self.k_w = V * 1e-6 / G0 if strat != 'none' else 0.0   # deep N^2 = 1e-6 s^-2
        self.D_nom = D_nom
        # True oil the float must pump (relative to the surface-bled state) to be neutral at p_T:
        # the a priori ballast plan is off by prior_err (fraction, random sign/size)
        self.D_true = D_nom * (1 + prior_err * rng.uniform(-1, 1))
        self.O = -self.D_true                            # oil relative to neutral at p_T (mL)
        self.p = 50.0                                    # start below the surface valve bleed
        A = iw_sigma                                     # heave amplitude scale (m)
        self.iw_sigma = A
        if A:
            per = [12.42 * 3600, 7200.0] + list(rng.uniform(480, 2400, 3))
            amp = [A * 0.7, A * 0.7] + [A / 3] * 3
            self.iw = [(a, 2 * np.pi / T, rng.uniform(0, 2 * np.pi)) for a, T in zip(amp, per)]
        else:
            self.iw = []
        self.w_iw = 0.0
        self.t = 0.0
        self.pump_s = 0.0
        self.valve_s = 0.0
        self.n_pump = 0
        self.n_valve = 0
        self.pump_on = self.valve_on = 0.0               # remaining run time (s)
        # Pump power against pressure, fitted to the fleet's pump records (W); the valve's
        # energy is negligible. Energy per mL = power / flow: ALTO ~10 J near the surface,
        # 26 J at 1000 dBar, 42 J at 2000 dBar.
        self.pw0, self.pw1 = PUMP_POWER[ftype]
        self.energy = 0.0                                # pump energy (J)

    def eta(self, t=None):
        t = self.t if t is None else t
        return sum(a * np.sin(om * t + ph) for a, om, ph in self.iw)

    def eta_dot(self, t=None):
        t = self.t if t is None else t
        return sum(a * om * np.cos(om * t + ph) for a, om, ph in self.iw)

    def K(self, p, eta=0.0):
        """int_{p_T}^{p} k dp; stratification at the heaved coordinate p - eta"""
        kc = self.k_c * (p - self.p_T)
        q = p - eta
        ks = self.k_s0 * self.h_s * (np.exp(-self.p_T / self.h_s) - np.exp(-q / self.h_s))             + self.k_w * (q - self.p_T)
        return kc + ks

    def dV(self):
        return self.O + self.K(self.p, self.eta())

    def w(self):
        y = 2 * G0 * self.dV() * 1e-6 / self.cda
        return np.sign(y) * np.sqrt(abs(y))

    def pump(self, t):
        if t > 0:
            self.pump_on += t
            self.pump_s += t
            self.n_pump += 1

    def valve(self, t):
        if t > 0:
            self.valve_on += t
            self.valve_s += t
            self.n_valve += 1

    def step(self, dt):
        if self.pump_on > 0:
            d = min(dt, self.pump_on)
            self.O += self.q_p * d
            self.energy += (self.pw0 + self.pw1 * max(self.p, 0.0)) * d
            self.pump_on -= d
        if self.valve_on > 0:
            d = min(dt, self.valve_on)
            q = self.q_v_shallow if self.p < self.p_fuse else self.q_v_deep
            self.O -= q * d
            self.valve_on -= d
        self.w_iw = -self.eta_dot()                      # water velocity, positive up
        self.p = max(self.p - (self.w() + self.w_iw) * dt, 0.0)
        self.t += dt

    def measure(self):
        return self.p + self.rng.normal(0, 0.05)

    def busy(self):
        return self.pump_on > 0 or self.valve_on > 0


# ---------------------------------------------------------------- helpers


class Track:
    """Pressure history and line fits of y = w|w| against p."""

    def __init__(self):
        self.t, self.p = [], []

    def add(self, t, p):
        self.t.append(t)
        self.p.append(p)

    def yline(self, t0, t1, lag=60.0):
        """Fit y = a + s p over samples in [t0, t1]; w by differences over `lag` s."""
        t = np.array(self.t)
        p = np.array(self.p)
        m = (t >= t0) & (t <= t1)
        t, p = t[m], p[m]
        if len(t) < 4:
            return None
        n = max(1, int(round(lag / np.median(np.diff(t)))))
        if len(t) <= n + 3:
            return None
        w = -(p[n:] - p[:-n]) / (t[n:] - t[:-n])
        pm = 0.5 * (p[n:] + p[:-n])
        y = w * np.abs(w)
        A = np.vstack([np.ones_like(pm), pm]).T
        coef, *_ = np.linalg.lstsq(A, y, rcond=None)
        return coef[0], coef[1], pm[-1], float(np.mean(pm))     # a, s, p at end, mean p


def settle_time(ts, ps, p_T, tol):
    ts, ps = np.array(ts), np.array(ps)
    bad = np.where(np.abs(ps - p_T) > tol)[0]
    if len(bad) == 0:
        return 0.0
    if bad[-1] == len(ps) - 1:
        return np.nan
    return ts[bad[-1] + 1]


# ---------------------------------------------------------------- descent seek controllers


def run_firmware(fl, horizon, dt, DPT, DUT, DUC, DCT=10800, DSW=3600, DCA=3, PST=3600, PSD=100,
                 DCP=5, DCF=5, DFT=None, DSD=None):
    """Reconstructed MFA seek: pump DFT at the target, wait DCT, DCA seeks DSW apart, then park
    seeks every PST when off by more than PSD. Run times: DUC * DPT (DUT) per 100 dBar of error."""

    if DFT is None:
        DFT = fl.D_nom / 0.317                         # firmware's plan from PFR = 19 mL/min
    ts, ps = [], []
    state, t_next, n_seek = 'fall', None, 0
    while fl.t < horizon:
        p = fl.measure()
        if state == 'fall' and p >= (fl.p_T if DSD is None else DSD):
            fl.pump(DFT)
            state, t_next = 'wait', fl.t + DFT + DCT
        elif state in ('wait', 'park') and fl.t >= t_next and not fl.busy():
            e = p - fl.p_T
            act = state == 'wait' or abs(e) > PSD
            if act:
                if e > 0 and DUC * DPT * e / 100 >= DCP:
                    fl.pump(min(DUC * DPT * e / 100, 1020))
                elif e < 0 and DUC * DUT * -e / 100 >= DCF:
                    fl.valve(DUC * DUT * -e / 100)
            if state == 'wait':
                n_seek += 1
                if n_seek >= DCA:
                    state, t_next = 'park', fl.t + PST
                else:
                    t_next = fl.t + DSW
            else:
                t_next = fl.t + PST
        fl.step(dt)
        ts.append(fl.t)
        ps.append(fl.p)
    return ts, ps


def run_drag(fl, horizon, dt, t_cal=60.0, win=600.0, trim_every=1800.0, tol=10.0,
             max_trim=4, calibrate=True, G_prior=None, aim=1.0, q_v_prior=0.09, q_p_nom=0.317,
             log=None, margin=0.0, t_min_pump=2.0, t_min_valve=2.0, trim_mode='second'):
    """Second-order (drag-law) seek: calibration pulse on the way down, closed-loop brake at the
    target, then trims every trim_every s when the predicted neutral point is off by > tol."""

    tr = Track()
    ts, ps = [], []
    state = 'fall'
    t_cal_start = None
    G = G_prior                      # dy per pump second
    Gv = None                        # dy per valve second (negative)
    s_est = None
    last_valve = None
    n_trim = 0
    t_next = None
    pre = None
    while fl.t < horizon:
        p = fl.measure()
        tr.add(fl.t, p)
        if state == 'fall' and calibrate and fl.t > win + 60 and p > 0.25 * fl.p_T:
            pre = tr.yline(fl.t - win, fl.t)
            if pre is not None:
                fl.pump(t_cal)
                t_cal_start = fl.t
                state = 'cal'
        elif state == 'fall' and not calibrate:
            state = 'approach'
        elif state == 'cal' and fl.t >= t_cal_start + t_cal + 60 + win:
            post = tr.yline(t_cal_start + t_cal + 60, fl.t)
            pc = 0.5 * (pre[3] + post[3])
            dy = (post[0] + post[1] * pc) - (pre[0] + pre[1] * pc)
            G = dy / t_cal if dy > 0 else G_prior
            if log is not None: log.append(('cal', fl.t, p, dy, G, post[1]))
            s_est = post[1]
            state = 'approach'
        elif state == 'approach':
            fit = tr.yline(fl.t - win, fl.t)
            if fit is not None and G:
                a, s, p_end, _ = fit
                s_use = s if s > 0 else (s_est or 0.0)
                s_est = s_use
                y_now = a + s_use * p
                y_T = y_now + s_use * (fl.p_T - p)
                need = (-y_T + s_use * margin) / G            # pump s: neutral margin dBar above p_T
                w_now = np.sign(y_now) * np.sqrt(abs(y_now))
                lead = max(-w_now, 0.0) * need * 0.5          # dBar travelled while pumping
                if need > 0 and p >= fl.p_T - lead:
                    lf = tr.yline(fl.t - 3000, fl.t)
                    if lf is not None and lf[1] > 0:
                        s_est = lf[1]
                    fl.pump(need)
                    if log is not None: log.append(('brake', fl.t, p, y_now, y_T, need, s_use))
                    state, t_next = 'trim', fl.t + need + trim_every
                elif need <= 0 and p >= fl.p_T - 5:           # already light enough: stop here
                    state, t_next = 'trim', fl.t + trim_every
        elif state == 'trim' and fl.t >= t_next and not fl.busy() and n_trim < max_trim:
            # Mean rate over the window (straight line p(t)); neutral point p_n = p - y / s with the
            # stiffness slope s measured on the approach. Robust near rest, unlike a y(p) fit.
            t_next = fl.t + trim_every
            tt = np.array(tr.t)
            m = tt >= fl.t - trim_every + 60
            if m.sum() > 10:
                pp = np.array(tr.p)[m]
                c = np.polyfit(tt[m] - fl.t, pp, 1)
                w_m = -c[0]
                y_m = w_m * abs(w_m)
                p_m = np.polyval(c, -0.5 * (fl.t - tt[m][0]))
                p_n = p_m - y_m / s_est
                if trim_mode == 'first':        # rate ignored: mean pressure over the window
                    p_n = float(np.mean(pp))
                e_n = p_n - fl.p_T
                if last_valve is not None:            # learn the valve gain from its effect
                    t_v, p_n0 = last_valve
                    dy = s_est * (p_n0 - p_n)
                    if dy > 0:
                        Gv = -dy / t_v
                    last_valve = None
                if log is not None: log.append(('trim', fl.t, fl.p, w_m, p_n, e_n, fl.dV()))
                if abs(e_n) > tol:
                    if e_n > 0:                         # settles too deep: pump
                        t = s_est * e_n / G
                        if t >= t_min_pump:
                            fl.pump(min(t, 1020))
                            n_trim += 1
                    else:                               # settles too shallow: valve
                        gv = Gv if Gv else -G * q_v_prior / q_p_nom
                        t = s_est * -e_n / -gv
                        if t >= t_min_valve:
                            fl.valve(min(t, 1800))
                            last_valve = (min(t, 1800), p_n)
                            n_trim += 1
        fl.step(dt)
        ts.append(fl.t)
        ps.append(fl.p)
    return ts, ps


def run_so(fl, horizon, dt, win=1800.0, t_cal=90.0, tol=15.0, damp=0.8, max_ops=8, win_trim=None, trim_first=False,
           q_v_prior=0.09, q_p_nom=0.317, cda_nom=0.07, k_prior=0.035, t_min=2.0, log=None):
    """Second-order seek on window averages (robust to internal-wave heave).

    Every `win` s: straight-line fit of p(t) over the window -> mean rate w_m and pressure p_m;
    y_m = w_m |w_m|. On the way down: one calibration pump (t_cal) gives the pump gain G from the
    shift of the y(p) line; the slope s of y(p) comes from the window means. When the float will
    reach the target before the next decision, the brake pump is scheduled for the predicted
    arrival, sized so y(p_T) = 0. Afterwards every window: neutral pressure p_n = p_m - y_m / s and
    a pump (deep) or valve (shallow) run of damp * s |p_n - p_T| / G when |p_n - p_T| > tol."""

    tr = Track()
    ts, ps = [], []
    W = []                         # window stats: (t_end, p_m, y_m, clean)
    G = 2 * G0 * q_p_nom * 1e-6 / cda_nom
    s_prior = 2 * G0 * k_prior * 1e-6 / cda_nom
    s_est = s_prior
    Gv = None
    state = 'fall'
    t_next = win
    t_last_op = -1e9
    cal = None
    brake_at = None
    brake_t = 0.0
    last_valve = None
    ops = 0
    while fl.t < horizon:
        p = fl.measure()
        tr.add(fl.t, p)
        if brake_at is not None and fl.t >= brake_at:
            fl.pump(brake_t)
            ops += 1
            t_last_op = fl.t + brake_t
            brake_at = None
            state = 'trim'
            t_next = fl.t + brake_t + (win_trim or win)
        if fl.t >= t_next and not fl.busy():
            wn = (win_trim or win) if state == 'trim' else win
            t_next = fl.t + wn
            tt = np.array(tr.t)
            m = tt >= max(fl.t - wn, t_last_op + 60)
            if m.sum() > 20 and (tt[m][-1] - tt[m][0]) > 0.6 * wn:
                pp = np.array(tr.p)[m]
                c = np.polyfit(tt[m] - fl.t, pp, 1)
                w_m = -c[0]
                p_m = float(np.mean(pp))
                y_m = w_m * abs(w_m)
                W.append((fl.t, p_m, y_m))
                if state == 'fall' and p_m > 0.2 * fl.p_T and w_m < -0.02:
                    fl.pump(t_cal)
                    ops += 1
                    cal = (p_m, y_m)
                    t_last_op = fl.t + t_cal
                    state = 'postcal'
                elif state == 'postcal':
                    dy = y_m - (cal[1] + s_est * (p_m - cal[0]))
                    if dy > 0:
                        G = dy / t_cal
                    state = 'approach'
                    W_post = [(p_m, y_m)]
                elif state == 'approach':
                    W_post.append((p_m, y_m))
                    if len(W_post) >= 2:
                        P = np.array(W_post)
                        sl = np.polyfit(P[:, 0], P[:, 1], 1)[0]
                        if sl > 0:
                            s_est = sl
                    y_T = y_m + s_est * (fl.p_T - p_m)
                    w_now = -np.sqrt(max(-(y_m + s_est * (pp[-1] - p_m)), 0))
                    dist = fl.p_T - pp[-1]
                    eta_arr = dist / max(-w_now, 1e-3)
                    need = -y_T / G
                    if need <= 0:
                        state = 'trim'
                    elif eta_arr < win * 1.2:
                        brake_t = need
                        brake_at = fl.t + max(eta_arr - need / 2, 0)
                        state = 'braking'
                    if log is not None: log.append(('appr', fl.t, p_m, w_m, y_T, need, eta_arr, s_est, G))
                elif state == 'trim' and ops < max_ops:
                    p_n = p_m if trim_first else p_m - y_m / s_est
                    e = p_n - fl.p_T
                    if last_valve is not None and not trim_first:
                        t_v, pn0 = last_valve
                        dyv = s_est * (pn0 - p_n)
                        if dyv > 0:
                            Gv = -dyv / t_v
                        last_valve = None
                    if log is not None: log.append(('trim', fl.t, p_m, w_m, p_n, e))
                    if abs(e) > tol:
                        if e > 0:
                            t = damp * s_est * e / G
                            if t >= t_min:
                                fl.pump(min(t, 1020)); ops += 1; t_last_op = fl.t + t
                        else:
                            gv = Gv if Gv else -G * q_v_prior / q_p_nom
                            t = damp * s_est * -e / -gv
                            if t >= t_min:
                                t = min(t, 1800)
                                fl.valve(t); ops += 1; t_last_op = fl.t + t
                                last_valve = (t, p_n)
        fl.step(dt)
        ts.append(fl.t)
        ps.append(fl.p)
    return ts, ps


Y0 = 1e-5          # (1 cm/s)^2: below this the filter's drag law turns linear (keeps it regular)


def f_reg(y):
    """Speed for y = w|w|, regularised near 0: sign(y) sqrt|y| for |y| >> Y0, y / sqrt(Y0) near 0."""
    return y / np.sqrt(abs(y) + Y0)


def df_reg(y):
    a = abs(y) + Y0
    return (a - 0.5 * abs(y)) / a ** 1.5


class FloatEKF:
    """Extended Kalman filter a float could run on its pressure record (5 states, scalar update).

    State x = [p, y, w_h, s, g]:
      p    pressure (dBar)
      y    w|w| the float's buoyancy gives at its present pressure ((m/s)^2) = 2 g dV / C_dA
      w_h  water vertical velocity (internal waves), first-order Gauss-Markov, positive down
      s    slope dy/dp = 2 g k / C_dA (stiffness over drag)
      g    pump gain, dy per pump second = 2 g q / C_dA
    Model: dp/dt = -f(y) + w_h; dy/dt = -s f(y); pump of t s: y += g t; valve: y -= gv t.
    Neutral pressure p_n = p - y / s. Measurement: pressure only.
    Cost per sample: a 5 x 5 covariance propagation and a scalar update, ~300 flops.
    """

    def __init__(self, p0, y0, sy, s0, ss, g0, sg, tau=600.0, sig_w=0.015, sigma_p=0.1,
                 q_y=1e-9, q_s=1e-15):
        self.x = np.array([p0, y0, 0.0, s0, g0], float)
        self.P = np.diag([0.1, sy, sig_w, ss, sg]) ** 2
        self.tau, self.sig_w, self.R = tau, sig_w, sigma_p ** 2
        self.q_y, self.q_s = q_y, q_s
        self.u_pump = 0.0             # pump seconds still to apply (the oil moves over the run)
        self.u_valve = 0.0
        self.gv = 0.0
        self.gv_sd = 0.0

    def predict(self, dt):
        p, y, wh, s, g = self.x
        a = np.exp(-dt / self.tau)
        fy, d = f_reg(y), df_reg(y)
        F = np.eye(5)
        F[0, 1] = -dt * d
        F[0, 2] = dt
        F[1, 1] = 1 - dt * s * d
        F[1, 3] = -dt * fy
        F[2, 2] = a
        y_new = y - dt * s * fy
        Q = np.diag([1e-4 * dt, self.q_y * dt, self.sig_w ** 2 * (1 - a * a), self.q_s * dt, 0.0])
        if self.u_pump > 0:
            dp_ = min(dt, self.u_pump)
            self.u_pump -= dp_
            y_new += g * dp_
            F[1, 4] += dp_
        if self.u_valve > 0:
            dv_ = min(dt, self.u_valve)
            self.u_valve -= dv_
            y_new -= self.gv * dv_
            Q[1, 1] += (self.gv_sd * dv_) ** 2
        self.x = np.array([p + dt * (-fy + wh), y_new, a * wh, s, g])
        self.P = F @ self.P @ F.T + Q

    def update(self, z):
        PH = self.P[:, 0]
        S = PH[0] + self.R
        K = PH / S
        self.x = self.x + K * (z - self.x[0])
        self.P = self.P - np.outer(K, PH)
        self.P = 0.5 * (self.P + self.P.T)
        self.x[3] = max(self.x[3], 1e-7)
        self.x[4] = max(self.x[4], 1e-6)

    def pump(self, t):
        self.u_pump += t

    def valve(self, t, gv, rel_sd=0.5):
        self.u_valve += t
        self.gv, self.gv_sd = gv, rel_sd * gv

    def neutral(self):
        p, y, wh, s, g = self.x
        J = np.array([1, -1 / s, 0, y / s ** 2, 0])
        return p - y / s, float(np.sqrt(max(J @ self.P @ J, 0)))

    def c_at(self, p_T):
        p, y, wh, s, g = self.x
        return y + s * (p_T - p)


def run_ekf(fl, horizon, dt, cda_nom=0.07, k_prior=0.035, q_p_nom=0.317, q_v_prior=0.09,
            t_cal=90.0, decide=1800.0, tol=15.0, damp=0.7, t_min=2.0, max_ops=10, log=None,
            sig_w=0.015, tau=600.0):
    """Seek with the EKF: calibration pump on the way down, brake at the predicted arrival, then
    a decision every `decide` s on the estimated neutral pressure (acting only when the error is
    larger than tol and twice its standard deviation)."""

    g0 = 2 * G0 * q_p_nom * 1e-6 / cda_nom
    s0 = 2 * G0 * k_prior * 1e-6 / cda_nom
    ekf = FloatEKF(fl.measure(), -0.02, 0.02, s0, s0 * 0.5, g0, g0 * 0.35, sig_w=sig_w, tau=tau)
    ts, ps = [], []
    state = 'fall'
    t_next = 0.0
    ops = 0
    while fl.t < horizon:
        ekf.predict(dt)
        ekf.update(fl.measure())
        if not fl.busy() and fl.t >= t_next:
            p, y_now, wh, s, g = ekf.x
            c = ekf.c_at(fl.p_T)
            w_now = f_reg(y_now)
            if state == 'fall' and p > 0.25 * fl.p_T and fl.t > 1200:
                fl.pump(t_cal); ekf.pump(t_cal); ops += 1
                state = 'approach'
                t_next = fl.t + t_cal + 1800
            elif state == 'approach':
                need = -c / g
                dist = fl.p_T - p
                eta = dist / max(-w_now, 1e-3)
                if dist < 0 or (need <= 0 and abs(w_now) < 0.01):
                    state, t_next = 'trim', fl.t + decide
                elif need > 0 and eta < need / 2 + 2 * dt:
                    fl.pump(need); ekf.pump(need); ops += 1
                    state, t_next = 'trim', fl.t + need + decide
                else:
                    t_next = fl.t + dt
            elif state == 'trim' and ops < max_ops:
                pn, spn = ekf.neutral()
                e = pn - fl.p_T
                if log is not None: log.append(('trim', fl.t, p, pn, spn, e, s, g))
                if abs(e) > max(tol, 2 * spn):
                    if c < 0:
                        t = min(damp * -c / g, 1020)
                        if t >= t_min:
                            fl.pump(t); ekf.pump(t); ops += 1
                    else:
                        gv = g * q_v_prior / q_p_nom
                        t = min(damp * c / gv, 1800)
                        if t >= t_min:
                            fl.valve(t); ekf.valve(t, gv); ops += 1
                t_next = fl.t + decide
        fl.step(dt)
        ts.append(fl.t)
        ps.append(fl.p)
    return ts, ps


def run_eco(fl, horizon, dt, win_d=600.0, win_t=1800.0, t_cal=90.0, f_commit=0.9, tol=20.0,
            tol_v=40.0, damp=0.8, max_ops=8, t_min=2.0, cda_nom=0.07, k_prior=0.035,
            q_p_nom=0.317, q_v_prior=0.09, p_cal_frac=0.15, p_commit_frac=0.4, log=None,
            fall_frac=None, p_fall=None, n_post=2):
    """Energy-aware second-order seek: pump the oil shallow, finish with small trims.

    Pump energy per mL rises with pressure (about 10 J near the surface, 26 J at 1000 dBar on the
    ALTO), so oil is cheapest pumped shallow, and oil let out through the valve is energy
    wasted. The rule:
      1. calibration pump at p_cal_frac * p_T (shallow, cheap): its response gives the gain G;
      2. at p_commit_frac * p_T, extrapolate the y(p) line of the descent windows to the target
         and pump f_commit of the oil still needed, so the float ends slightly heavy (deep);
      3. trims every win_t s on window averages: pump when the neutral pressure is more than tol
         deep; open the valve only when it is more than tol_v shallow.
    """

    tr = Track()
    ts, ps = [], []
    W = []
    G = 2 * G0 * q_p_nom * 1e-6 / cda_nom
    s_est = 2 * G0 * k_prior * 1e-6 / cda_nom
    Gv = None
    state = 'fall'
    t_next = win_d
    t_last_op = -1e9
    pre = None
    post = []
    ops = 0
    last_valve = None
    while fl.t < horizon:
        p = fl.measure()
        tr.add(fl.t, p)
        if fl.t >= t_next and not fl.busy():
            wn = win_t if state == 'trim' else win_d
            t_next = fl.t + wn
            tt = np.array(tr.t)
            m = tt >= max(fl.t - wn, t_last_op + 60)
            if m.sum() > 20 and (tt[m][-1] - tt[m][0]) > 0.6 * wn:
                pp = np.array(tr.p)[m]
                c = np.polyfit(tt[m] - fl.t, pp, 1)
                w_m = -c[0]
                p_m = float(np.mean(pp))
                y_m = w_m * abs(w_m)
                if state == 'fall':
                    W.append((p_m, y_m))
                    p_cal = p_fall if p_fall is not None else p_cal_frac * fl.p_T
                    if fall_frac is not None:          # the calibration pump is the fall pump
                        t_cal = fall_frac * fl.D_nom / q_p_nom
                    if p_m > p_cal and len(W) >= 2:
                        P = np.array(W[-3:])
                        sl = np.polyfit(P[:, 0], P[:, 1], 1)[0] if len(P) > 1 else s_est
                        pre = (p_m, y_m, sl if sl > 0 else s_est)
                        fl.pump(t_cal); ops += 1
                        t_last_op = fl.t + t_cal
                        state = 'postcal'
                elif state == 'postcal':
                    post.append((p_m, y_m))
                    if len(post) >= n_post:
                        P = np.array(post)
                        if len(P) > 1:
                            sl = np.polyfit(P[:, 0], P[:, 1], 1)[0]
                            if sl > 0:
                                s_est = sl
                        # gain: jump of the line at the calibration pressure
                        y_post_at = P[0, 1] + s_est * (pre[0] - P[0, 0])
                        dy = y_post_at - pre[1]
                        if dy > 0:
                            G = dy / t_cal
                        state = 'descend'
                elif state == 'descend':
                    post.append((p_m, y_m))
                    P = np.array(post[-4:])
                    sl = np.polyfit(P[:, 0], P[:, 1], 1)[0]
                    if sl > 0:
                        s_est = sl
                    if p_m > p_commit_frac * fl.p_T or w_m > -0.02:
                        y_T = y_m + s_est * (fl.p_T - p_m)
                        need = -y_T / G
                        if log is not None: log.append(('commit', fl.t, p_m, y_T, need, s_est, G))
                        if need * f_commit >= t_min:
                            fl.pump(need * f_commit); ops += 1
                            t_last_op = fl.t + need * f_commit
                        state = 'trim'
                        t_next = t_last_op + win_t
                elif state == 'trim' and ops < max_ops:
                    p_n = p_m - y_m / s_est
                    e = p_n - fl.p_T
                    if last_valve is not None:
                        t_v, pn0 = last_valve
                        dyv = s_est * (pn0 - p_n)
                        if dyv > 0:
                            Gv = -dyv / t_v
                        last_valve = None
                    if log is not None: log.append(('trim', fl.t, p_m, p_n, e))
                    if e > tol:
                        t = damp * s_est * e / G
                        if t >= t_min:
                            fl.pump(min(t, 1020)); ops += 1; t_last_op = fl.t + t
                    elif e < -tol_v:
                        gv = Gv if Gv else -G * q_v_prior / q_p_nom
                        t = min(damp * s_est * -e / -gv, 1800)
                        if t >= t_min:
                            fl.valve(t); ops += 1; t_last_op = fl.t + t
                            last_valve = (t, p_n)
        fl.step(dt)
        ts.append(fl.t)
        ps.append(fl.p)
    return ts, ps


def run_prog(fl, horizon, dt, win=900.0, win_t=1800.0, p_fall=150.0, fall_frac=0.25,
             sig_w=0.005, sig_s=0.25, sig_g=0.15, n_sig=2.0, tol=20.0, tol_v=40.0, damp=0.8,
             max_ops=10, t_min=2.0, cda_nom=0.07, k_prior=0.035, q_p_nom=0.317, q_v_prior=0.09,
             log=None):
    """Progressive second-order seek: pump only the oil the estimate is sure of.

    The rate predicts the neutral pressure p_n = p - y / s with an error that shrinks as the
    float approaches it (2 |w| dw / s from the rate, (p - p_n) ds / s from the slope). At each
    window on the way down the float pumps the predicted remaining need minus n_sig standard
    deviations, so the bulk of the oil goes in early, shallow and cheap, nothing overshoots
    (no oil let out and pumped again), and only small trims are left for depth.
      1. fall pump at p_fall, sized at fall_frac of the nominal need (below any plausible
         need); its response gives the gain G;
      2. every `win` s while descending: progressive pump;
      3. near rest: trims every win_t s (pump if > tol deep, valve only if > tol_v shallow).
    """

    tr = Track()
    ts, ps = [], []
    G = 2 * G0 * q_p_nom * 1e-6 / cda_nom
    s_est = 2 * G0 * k_prior * 1e-6 / cda_nom
    g_sd = 0.35                                   # relative uncertainty of G before calibration
    Gv = None
    state = 'fall'
    t_next = win
    t_last_op = -1e9
    W = []
    pre = None
    t_fall = fall_frac * fl.D_nom / q_p_nom
    ops = 0
    last_valve = None
    while fl.t < horizon:
        p = fl.measure()
        tr.add(fl.t, p)
        if state == 'fall' and p > p_fall and fl.t > 600:
            # fall pump on the pressure sample (as the firmware's DSD), rate from the last window
            tt = np.array(tr.t)
            m = tt >= fl.t - min(win, fl.t)
            pp = np.array(tr.p)[m]
            c = np.polyfit(tt[m] - fl.t, pp, 1)
            pre = (float(np.mean(pp)), -c[0] * abs(c[0]))
            fl.pump(t_fall); ops += 1
            t_last_op = fl.t + t_fall
            state = 'cal'
            t_next = t_last_op + win
        if fl.t >= t_next and not fl.busy():
            wn = win_t if state == 'trim' else win
            t_next = fl.t + wn
            tt = np.array(tr.t)
            m = tt >= max(fl.t - wn, t_last_op + 60)
            if m.sum() > 20 and (tt[m][-1] - tt[m][0]) > 0.6 * wn:
                pp = np.array(tr.p)[m]
                c = np.polyfit(tt[m] - fl.t, pp, 1)
                w_m = -c[0]
                p_m = float(np.mean(pp))
                y_m = w_m * abs(w_m)
                if state in ('fall', 'descend'):
                    W.append((p_m, y_m, fl.t))
                    seg = [x for x in W if x[2] > t_last_op]
                    if len(seg) >= 3:
                        P = np.array(seg[-4:])
                        sl = np.polyfit(P[:, 0], P[:, 1], 1)[0]
                        if sl > 0:
                            s_est = 0.5 * s_est + 0.5 * sl
                if state == 'cal':
                    y_pre_here = pre[1] + s_est * (p_m - pre[0])
                    dy = y_m - y_pre_here
                    if dy > 0:
                        G = dy / t_fall
                        g_sd = sig_g
                    state = 'descend'
                elif state == 'descend':
                    y_T = y_m + s_est * (fl.p_T - p_m)
                    need = -y_T / G                          # pump s still needed
                    dist = abs(y_m / s_est)                  # dBar to the predicted neutral point
                    sd_pn = np.hypot(2 * abs(w_m) * sig_w / s_est, sig_s * dist)
                    sd_need = np.hypot(s_est * sd_pn / G, g_sd * max(need, 0))
                    t = need - n_sig * sd_need
                    if log is not None: log.append(('desc', fl.t, p_m, w_m, need, sd_need, t))
                    if t >= t_min:
                        fl.pump(min(t, 1020)); ops += 1; t_last_op = fl.t + t
                    if abs(w_m) < 0.01 or p_m > fl.p_T:
                        state = 'trim'
                        t_next = fl.t + win_t
                elif state == 'trim' and ops < max_ops:
                    p_n = p_m - y_m / s_est
                    e = p_n - fl.p_T
                    if last_valve is not None:
                        t_v, pn0 = last_valve
                        dyv = s_est * (pn0 - p_n)
                        if dyv > 0:
                            Gv = -dyv / t_v
                        last_valve = None
                    if log is not None: log.append(('trim', fl.t, p_m, p_n, e))
                    if e > tol:
                        t = damp * s_est * e / G
                        if t >= t_min:
                            fl.pump(min(t, 1020)); ops += 1; t_last_op = fl.t + t
                    elif e < -tol_v:
                        gv = Gv if Gv else -G * q_v_prior / q_p_nom
                        t = min(damp * s_est * -e / -gv, 1800)
                        if t >= t_min:
                            fl.valve(t); ops += 1; t_last_op = fl.t + t
                            last_valve = (t, p_n)
        fl.step(dt)
        ts.append(fl.t)
        ps.append(fl.p)
    return ts, ps


# ---------------------------------------------------------------- ascent


def ascent_bangbang(fl, dt, ARR, APS, APN, APF, ASI=10.0):
    """MFA ascent: pump APS, then every APF s after a pump: if rise rate < ARR pump APN."""
    tr = Track()
    fl.pump(APS)
    t_check = APS + APF
    rates = []
    while fl.p > 2 and fl.t < 6 * 3600:
        p = fl.measure()
        tr.add(fl.t, p)
        if fl.t >= t_check and not fl.busy():
            fit = len(tr.t) > 13
            w_meas = -(tr.p[-1] - tr.p[-13]) / (tr.t[-1] - tr.t[-13]) if fit else 0
            if w_meas < ARR:
                fl.pump(APN)
                t_check = fl.t + APN + APF
            else:
                t_check = fl.t + APF
        fl.step(dt)
        rates.append(fl.w() + fl.w_iw)
    return np.array(rates)


def ascent_rate(fl, dt, ARR, APS, interval=60.0, win=300.0, min_run=2.0, q_p_nom=0.317, cda_nom=None):
    """Rate-matched ascent: first pump APS; G calibrated from its response; then every interval
    pump the time that brings y to ARR^2 at the pressure expected at the next check."""
    tr = Track()
    fl.pump(APS)
    t0 = fl.t
    G = None
    rates = []
    t_check = APS + win
    y_target = ARR * ARR
    while fl.p > 2 and fl.t < 6 * 3600:
        p = fl.measure()
        tr.add(fl.t, p)
        if fl.t >= t_check and not fl.busy():
            fit = tr.yline(fl.t - win, fl.t, lag=60.0)
            if fit is not None:
                a, s, _, _ = fit
                y_now = a + s * p
                if G is None:
                    G = max(y_now, 1e-4) / APS                # from start (y ~ 0 at park)
                w_now = np.sqrt(max(y_now, 0))
                y_next = y_now + s * (-w_now * interval)      # loss while rising
                t = (y_target - y_next) / G
                if t >= min_run:
                    fl.pump(t)
            t_check = fl.t + interval
        fl.step(dt)
        rates.append(fl.w() + fl.w_iw)
    return np.array(rates)


# ---------------------------------------------------------------- main


CDA_NOM = {'ALTO': 0.07, 'ALAMO': 0.033}


def seek_case(c):
    ftype, p_T, strat, iw, prior_err, ctrl, n = c
    dt, horizon = 10.0, 12 * 3600
    out = []
    for i in range(n):
        rng = np.random.default_rng([i, int(p_T), int(iw * 10), int(prior_err * 100),
                                     len(strat), len(ftype)])
        fl = Float(rng, ftype, p_T, iw, strat, prior_err, 0.05 if ftype == 'ALTO' else 0.5)
        # Fleet's most common settings (mission.json, Nov 2026): ALTO DPT = DUT = 10, DUC 50 %,
        # DSW = DCT = 3 h, PSD 200, PST 6 h; ALAMO DPT = DUT = 6, DUC 100 %, DSW 1 h, DCT 3 h
        if ftype == 'ALTO':
            fw = dict(DPT=10, DUT=10, DUC=0.5, DCT=10800, DSW=10800, DCA=3, PST=21600, PSD=200,
                      DCP=2, DCF=2, DSD=200)
        else:
            fw = dict(DPT=6, DUT=6, DUC=1.0, DCT=10800, DSW=3600, DCA=3, PST=21600, PSD=200,
                      DCP=2, DCF=2, DSD=50)
        if ctrl == 'fw':
            ts, ps = run_firmware(fl, horizon, dt, **fw)
        elif ctrl == 'fw_dca8':           # fleet rule with more attempts: DCA 8, DSW 1 h
            ts, ps = run_firmware(fl, horizon, dt, **dict(fw, DCA=8, DSW=3600))
        elif ctrl == 'fw_gainK':          # first order, gain matched to the true stiffness
            kT = fl.k_c + fl.k_s0 * np.exp(-p_T / fl.h_s)
            ts, ps = run_firmware(fl, horizon, dt, DPT=100 * kT / 0.317,
                                  DUT=100 * kT / fl.q_v_deep, DUC=1.0, DCT=3600, DSW=3600, DCA=6,
                                  PST=3600, PSD=10, DCP=2, DCF=2)
        elif ctrl == 'drag':
            ts, ps = run_drag(fl, horizon, dt, t_min_pump=2.0, t_min_valve=2.0)
        elif ctrl == 'so':               # second order on 30 min window averages
            ts, ps = run_so(fl, horizon, dt, cda_nom=CDA_NOM[ftype],
                            k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'so_t60':           # trims on 1 h windows
            ts, ps = run_so(fl, horizon, dt, win_trim=3600.0, cda_nom=CDA_NOM[ftype],
                            k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'so_fo':            # second-order approach and brake, then first-order trims
                                         # on 1 h mean pressure with the gain measured in descent
            ts, ps = run_so(fl, horizon, dt, win_trim=3600.0, trim_first=True, damp=0.7,
                            cda_nom=CDA_NOM[ftype], k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'so_t60d5':         # trims on 1 h windows, damping 0.5, tolerance 20
            ts, ps = run_so(fl, horizon, dt, win_trim=3600.0, damp=0.5, tol=20.0,
                            cda_nom=CDA_NOM[ftype], k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'prog':             # progressive second order: energy-efficient
            ts, ps = run_prog(fl, horizon, dt, win=1200.0, fall_frac=0.9, n_sig=2.0,
                              p_fall=200.0 if ftype == 'ALTO' else 50.0, cda_nom=CDA_NOM[ftype],
                              k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'eco':              # energy-aware second order: oil pumped shallow
            ts, ps = run_eco(fl, horizon, dt, cda_nom=CDA_NOM[ftype],
                             k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'ekf':              # Kalman filter state estimate, same decisions
            ts, ps = run_ekf(fl, horizon, dt, cda_nom=CDA_NOM[ftype],
                             k_prior=0.035 if ftype == 'ALTO' else 0.017)
        elif ctrl == 'drag_fine':        # 1 s minimum runs
            ts, ps = run_drag(fl, horizon, dt, t_min_pump=1.0, t_min_valve=1.0)
        elif ctrl == 'hybrid':           # second-order brake, then hourly first-order trims with
                                         # the gain measured on the way down
            ts, ps = run_drag(fl, horizon, dt, t_min_pump=2.0, t_min_valve=2.0, trim_mode='first',
                              trim_every=3600.0, tol=15.0, max_trim=6)
        else:                             # no calibration pulse: fleet-median C_dA, nominal q
            ts, ps = run_drag(fl, horizon, dt, calibrate=False,
                              G_prior=2 * G0 * 0.317e-6 / CDA_NOM[ftype])
        ts, ps = np.array(ts), np.array(ps)
        # Heave moves the float with the isopycnals; judge the controller on hour means
        k = int(round(3600 / dt))
        pm = np.convolve(ps, np.ones(k) / k, mode='valid')
        tm = ts[k - 1:]
        st_m = settle_time(tm, pm, p_T, 25.0)
        arrive = ts[np.argmax(ps >= p_T - 10)] if (ps >= p_T - 10).any() else np.nan
        out.append(dict(settle_mean25_h=st_m / 3600,
                        err_last4h=float(np.mean(ps[ts >= horizon - 4 * 3600]) - p_T),
                        settle_h=settle_time(ts, ps, p_T, 10.0) / 3600,
                        settle25_h=settle_time(ts, ps, p_T, 25.0) / 3600,
                        arrive_h=arrive / 3600,
                        err_6h=float(ps[ts.searchsorted(6 * 3600)] - p_T),
                        err_end=float(ps[-1] - p_T), ops=fl.n_pump + fl.n_valve,
                        n_pump=fl.n_pump, n_valve=fl.n_valve, pump_s=fl.pump_s,
                        valve_s=fl.valve_s, max_over=float(ps.max() - p_T),
                        energy=fl.energy))
    med = lambda k: float(np.nanmedian([o[k] for o in out])) if np.isfinite([o[k] for o in out]).any() else float('nan')
    return dict(type=ftype, p_T=p_T, strat=strat, iw=iw, prior_err=prior_err, ctrl=ctrl, n=n,
                settled_mean25=float(np.mean([np.isfinite(o['settle_mean25_h']) for o in out])),
                settle_mean25_h=med('settle_mean25_h'),
                abs_err_last4h=float(np.median([abs(o['err_last4h']) for o in out])),
                frac_last4h_25=float(np.mean([abs(o['err_last4h']) <= 25 for o in out])),
                settled10=float(np.mean([np.isfinite(o['settle_h']) for o in out])),
                settled25=float(np.mean([np.isfinite(o['settle25_h']) for o in out])),
                settle10_h=med('settle_h'), settle25_h=med('settle25_h'), arrive_h=med('arrive_h'),
                abs_err_6h=float(np.median([abs(o['err_6h']) for o in out])),
                abs_err_end=float(np.median([abs(o['err_end']) for o in out])),
                ops=med('ops'), n_pump=med('n_pump'), n_valve=med('n_valve'),
                pump_s=med('pump_s'), valve_s=med('valve_s'), max_over=med('max_over'),
                energy=med('energy'), energy_mean=float(np.mean([o['energy'] for o in out])))


def ascent_case(c):
    ftype, strat, iw, ctrl, n = c
    out = []
    for i in range(n):
        rng = np.random.default_rng([i, 7, int(iw * 10), len(strat), len(ftype)])
        p0 = 2000.0 if ftype == 'ALTO' else 1000.0
        fl = Float(rng, ftype, p0, iw, strat, 0.0, 0.05 if ftype == 'ALTO' else 0.5)
        fl.O, fl.p = 0.0, p0
        # Fleet's most common settings: ALTO APS 120, APN 20, APF 100; ALAMO APS 60, APN 10, APF 20
        APS, APN, APF = (120, 20, 100) if ftype == 'ALTO' else (60, 10, 20)
        if ctrl == 'bangbang':
            r = ascent_bangbang(fl, 5.0, ARR=0.10, APS=APS, APN=APN, APF=APF)
        elif ctrl == 'bangbang_long':
            r = ascent_bangbang(fl, 5.0, ARR=0.10, APS=APS, APN=2 * APN, APF=2 * APF)
        else:
            r = ascent_rate(fl, 5.0, ARR=0.10, APS=APS, interval=float(ctrl.split('_')[1]))
        out.append(dict(rms_cm_s=float(np.sqrt(np.mean((r - 0.10) ** 2))) * 100,
                        mean_cm_s=float(np.mean(r)) * 100,
                        p10_cm_s=float(np.percentile(r, 10)) * 100,
                        p90_cm_s=float(np.percentile(r, 90)) * 100,
                        t_h=fl.t / 3600, n_pump=fl.n_pump, pump_s=fl.pump_s))
    return dict(type=ftype, strat=strat, iw=iw, ctrl=ctrl, n=n,
                **{k: float(np.median([o[k] for o in out])) for k in out[0]})


CTRLS = ('fw', 'fw_dca8', 'fw_gainK', 'so', 'prog', 'ekf')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--n', type=int, default=60)
    ap.add_argument('--jobs', type=int, default=16)
    ap.add_argument('--out', default='seek2sim.json')
    args = ap.parse_args()
    from multiprocessing import Pool
    seek = [(ft, pT, st, iw, pe, ctrl, args.n)
            for ft, pT in (('ALTO', 1000.0), ('ALTO', 500.0), ('ALAMO', 500.0))
            for st in ('strong', 'weak') for iw in (0.0, 5.0, 15.0) for pe in (0.1, 0.75)
            for ctrl in CTRLS]
    asc = [(ft, st, iw, ctrl, args.n)
           for ft in ('ALTO', 'ALAMO') for st in ('strong', 'weak') for iw in (0.0, 5.0)
           for ctrl in ('bangbang', 'bangbang_long', 'rate_60', 'rate_300')]
    with Pool(args.jobs) as pool:
        rs = pool.map(seek_case, seek, chunksize=1)
        ra = pool.map(ascent_case, asc, chunksize=1)
    json.dump(dict(seek=rs, ascent=ra), open(args.out, 'w'), indent=1)


if __name__ == '__main__':
    main()
