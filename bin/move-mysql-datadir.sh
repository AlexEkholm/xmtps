#!/bin/bash
# Move the MySQL data directory from /var/lib/mysql to /data1/mysql, to free space on the root
# filesystem. Run as root on argus:
#
#     sudo bash move-mysql-datadir.sh
#
# Steps: stop xmtps-alamo and MySQL, copy the data directory with rsync, keep the old one as
# /var/lib/mysql.old, set datadir in mysqld.cnf, allow the new path in AppArmor, make MySQL wait
# for /data1 at boot, start MySQL, check it, and start xmtps-alamo again. If a step fails after
# MySQL is stopped, the old data directory and configuration are restored and both services are
# started again.
#
# The old copy is not deleted. Once everything has run normally for a day or so, free the space
# on the root filesystem with:
#
#     sudo rm -rf /var/lib/mysql.old

set -euo pipefail

OLD_DIR=/var/lib/mysql
NEW_DIR=/data1/mysql
BACKUP_DIR=/var/lib/mysql.old
CNF=/etc/mysql/mysql.conf.d/mysqld.cnf
APPARMOR_PROFILE=/etc/apparmor.d/usr.sbin.mysqld
APPARMOR_LOCAL=/etc/apparmor.d/local/usr.sbin.mysqld
DROPIN=/etc/systemd/system/mysql.service.d/data1.conf
SERVICES="xmtps-alamo"     # Services that use MySQL; stopped first and started last.

log()  { echo "$(date -u +%FT%TZ) $*"; }
fail() { log "ERROR: $*"; exit 1; }

# --- Checks before anything is changed ---------------------------------------------------------

[ "$(id -u)" -eq 0 ]           || fail "Run as root (sudo bash $0)."
[ -d "$OLD_DIR/mysql" ]        || fail "$OLD_DIR does not look like a MySQL data directory."
[ ! -e "$NEW_DIR" ]            || fail "$NEW_DIR already exists; remove it or move it aside first."
[ ! -e "$BACKUP_DIR" ]         || fail "$BACKUP_DIR already exists; remove it or move it aside first."
[ -f "$CNF" ]                  || fail "$CNF not found."
mountpoint -q /data1           || fail "/data1 is not mounted."

need_kb=$(du -sk "$OLD_DIR" | cut -f1)
free_kb=$(df -k --output=avail /data1 | tail -1)
[ "$free_kb" -gt $((need_kb * 2)) ] || fail "Not enough free space on /data1."

log "MySQL data directory: $OLD_DIR ($((need_kb / 1024 / 1024)) GB), to be moved to $NEW_DIR."
log "This stops $SERVICES and MySQL (and so MediaWiki) for about 10-15 minutes."
read -r -p "Continue? [y/N] " answer
[ "$answer" = "y" ] || [ "$answer" = "Y" ] || fail "Cancelled."

# --- Rollback, used if a step fails once MySQL has been stopped --------------------------------

cp -p "$CNF" "$CNF.before-move"

rollback() {
  trap - ERR
  set +e
  log "A step failed: restoring the old data directory and configuration..."
  systemctl stop mysql || true
  if [ -d "$BACKUP_DIR" ] && [ ! -e "$OLD_DIR" ]; then
    mv "$BACKUP_DIR" "$OLD_DIR"
  fi
  cp -p "$CNF.before-move" "$CNF"
  systemctl start mysql && log "MySQL started from $OLD_DIR." || log "MySQL did not start: check 'journalctl -u mysql'."
  systemctl start $SERVICES || true
  log "Rolled back. The partial copy in $NEW_DIR was left in place; remove it before trying again."
  exit 1
}

# --- Stop the services -------------------------------------------------------------------------

log "Stopping $SERVICES..."
systemctl stop $SERVICES

log "Stopping MySQL..."
systemctl stop mysql
trap rollback ERR
for _ in $(seq 60); do
  pgrep -x mysqld -u mysql > /dev/null || break
  sleep 1
done
! pgrep -x mysqld -u mysql > /dev/null || { log "MySQL is still running."; false; }

# --- Copy the data directory -------------------------------------------------------------------

log "Copying $OLD_DIR to $NEW_DIR..."
rsync -aHAX "$OLD_DIR/" "$NEW_DIR/"

old_kb=$(du -sk --apparent-size "$OLD_DIR" | cut -f1)
new_kb=$(du -sk --apparent-size "$NEW_DIR" | cut -f1)
log "Copied: $old_kb KB in $OLD_DIR, $new_kb KB in $NEW_DIR."
[ "$old_kb" -eq "$new_kb" ] || { log "The sizes differ."; false; }

mv "$OLD_DIR" "$BACKUP_DIR"
log "Old data directory kept as $BACKUP_DIR."

# --- MySQL configuration -----------------------------------------------------------------------

if grep -qE '^[[:space:]]*#?[[:space:]]*datadir[[:space:]]*=' "$CNF"; then
  sed -i -E "s|^[[:space:]]*#?[[:space:]]*datadir[[:space:]]*=.*|datadir         = $NEW_DIR|" "$CNF"
else
  sed -i -E "/^\[mysqld\]/a datadir         = $NEW_DIR" "$CNF"
fi
grep -q "^datadir *= *$NEW_DIR\$" "$CNF" || { log "Failed to set datadir in $CNF."; false; }
log "Set datadir = $NEW_DIR in $CNF."

# --- AppArmor ----------------------------------------------------------------------------------

if [ -f "$APPARMOR_PROFILE" ]; then
  touch "$APPARMOR_LOCAL"
  grep -qF "$NEW_DIR/ r," "$APPARMOR_LOCAL"    || echo "$NEW_DIR/ r,"    >> "$APPARMOR_LOCAL"
  grep -qF "$NEW_DIR/** rwk," "$APPARMOR_LOCAL" || echo "$NEW_DIR/** rwk," >> "$APPARMOR_LOCAL"
  apparmor_parser -r "$APPARMOR_PROFILE"
  log "Allowed $NEW_DIR in AppArmor."
else
  log "No AppArmor profile for mysqld; skipped."
fi

# --- Make MySQL wait for /data1 at boot --------------------------------------------------------

mkdir -p "$(dirname "$DROPIN")"
printf '[Unit]\nRequiresMountsFor=/data1\n' > "$DROPIN"
systemctl daemon-reload
log "MySQL now waits for /data1 to be mounted at boot ($DROPIN)."

# --- Start and check MySQL ---------------------------------------------------------------------

log "Starting MySQL..."
systemctl start mysql

datadir=$(mysql -N -e "SELECT @@datadir;")
log "MySQL datadir: $datadir"
[ "${datadir%/}" = "$NEW_DIR" ] || { log "MySQL is not using $NEW_DIR."; false; }

tables=$(mysql -N -e "SHOW TABLES FROM xmtps_alamo;" | wc -l)
floats=$(mysql -N -e "SELECT COUNT(*) FROM xmtps_alamo.XFloat;")
log "xmtps_alamo: $tables tables, $floats floats."
[ "$tables" -ge 5 ] || { log "Expected at least 5 tables in xmtps_alamo."; false; }

trap - ERR

# --- Start the services again ------------------------------------------------------------------

log "Starting $SERVICES..."
systemctl start $SERVICES

log "Done. MySQL now runs from $NEW_DIR."
log "Root filesystem: $(df -h --output=used,avail,pcent / | tail -1) (used, free, use%)."
log "The old copy is still in $BACKUP_DIR. Once everything has run normally, free the space with:"
log "    sudo rm -rf $BACKUP_DIR"
