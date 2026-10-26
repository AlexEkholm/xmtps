#!/bin/bash
# Apply the 2026-10-26 XFloatMail changes to an existing XMTPS database (see sql/xmtps-schema.sql):
#
#   - drop the columns id_dac and id_wmo (never written), msg_body_text and msg_header_raw_text
#     (never read after parsing; both are part of msg_raw_text);
#   - reorder the primary key to start with id_imei, so each float's messages are stored together
#     (same columns, so the same rows are duplicates);
#   - add inst_dive_idx (id_instrument, id_dive) and active_idx (iridium_position_datetime,
#     msg_is_valid, id_instrument) for the daemon's queries, and drop the indexes they replace:
#     id_dac_idx, id_wmo_idx, id_dive_idx, id_instrument and iridium_position_datetime_idx.
#
# Run as root on argus, after the MySQL data directory has been moved off the root filesystem
# (bin/move-mysql-datadir.sh) and after a build that no longer writes the dropped columns has been
# deployed:
#
#     sudo bash migrate-xfloatmail.sh [DATABASE]      (default: xmtps_alamo)
#
# The script reads the table's current columns and indexes and changes only what is still needed,
# so it can be run again safely. xmtps-alamo is stopped during the change (a single ALTER TABLE that
# rebuilds the table) and started again afterwards. The ALTER TABLE is atomic: if it fails, the
# table is left as it was.

set -euo pipefail

DB=${1:-xmtps_alamo}
TABLE=XFloatMail
SERVICE=xmtps-alamo
BINARY=/data1/alamo/bin/xmtps
APPARMOR_PROFILE=/etc/apparmor.d/usr.sbin.mysqld
APPARMOR_LOCAL=/etc/apparmor.d/local/usr.sbin.mysqld

DROP_COLUMNS="id_dac id_wmo msg_body_text msg_header_raw_text"
DROP_INDEXES="id_dac_idx id_wmo_idx id_dive_idx id_instrument iridium_position_datetime_idx"
NEW_PRIMARY_KEY="id_imei,id_dive,id_sbd_momsn,id_sbd_packet,id_msg"

log()  { echo "$(date -u +%FT%TZ) $*"; }
fail() { log "ERROR: $*"; exit 1; }
sql()  { mysql -N -B -e "$1"; }

# --- Checks ------------------------------------------------------------------------------------

[ "$(id -u)" -eq 0 ] || fail "Run as root (sudo bash $0)."
sql "SELECT 1;" > /dev/null || fail "Cannot connect to MySQL as root."
[ "$(sql "SELECT COUNT(*) FROM information_schema.tables
          WHERE table_schema = '$DB' AND table_name = '$TABLE';")" -eq 1 ] \
  || fail "Table $DB.$TABLE not found."

# The deployed daemon must no longer write the dropped columns, or every save would fail. Only the
# binary's string constants are searched: its debug information names the class members anyway.
if [ -e "$BINARY" ]; then
  binary=$(readlink -f "$BINARY")
  if readelf -p .rodata "$binary" | grep -qE '\] +msg_(header_raw|body)_text$'; then
    fail "$binary still uses the dropped columns: deploy the new build first."
  fi
fi

# The rebuild needs free space for a second copy of the table, plus sort files for the new primary
# key, on the data directory's filesystem; the root filesystem has too little room.
datadir=$(sql "SELECT @@datadir;")
datadir=${datadir%/}
datadir_mount=$(df --output=target "$datadir" | tail -1)
[ "$datadir_mount" != "/" ] || fail "The MySQL data directory ($datadir) is on the root filesystem; move it first (bin/move-mysql-datadir.sh)."

table_kb=$(( $(sql "SELECT data_length + index_length FROM information_schema.tables
                     WHERE table_schema = '$DB' AND table_name = '$TABLE';") / 1024 ))
free_kb=$(df -k --output=avail "$datadir" | tail -1)
[ "$free_kb" -gt $((table_kb * 3)) ] || fail "Not enough free space on $datadir_mount for the rebuild."

# --- Work out the changes still needed ---------------------------------------------------------

columns=$(sql "SELECT column_name FROM information_schema.columns
               WHERE table_schema = '$DB' AND table_name = '$TABLE';")
indexes=$(sql "SELECT DISTINCT index_name FROM information_schema.statistics
               WHERE table_schema = '$DB' AND table_name = '$TABLE';")
primary_key=$(sql "SELECT GROUP_CONCAT(column_name ORDER BY seq_in_index) FROM information_schema.statistics
                   WHERE table_schema = '$DB' AND table_name = '$TABLE' AND index_name = 'PRIMARY';")

changes=()
for c in $DROP_COLUMNS; do
  grep -qx "$c" <<< "$columns" && changes+=("DROP COLUMN \`$c\`")
done
for i in $DROP_INDEXES; do
  grep -qx "$i" <<< "$indexes" && changes+=("DROP INDEX \`$i\`")
done
if [ "$primary_key" != "$NEW_PRIMARY_KEY" ]; then
  changes+=("DROP PRIMARY KEY" "ADD PRIMARY KEY ($NEW_PRIMARY_KEY)")
fi
grep -qx "inst_dive_idx" <<< "$indexes" || changes+=("ADD INDEX inst_dive_idx (id_instrument, id_dive)")
grep -qx "active_idx" <<< "$indexes" \
  || changes+=("ADD INDEX active_idx (iridium_position_datetime, msg_is_valid, id_instrument)")

if [ ${#changes[@]} -eq 0 ]; then
  log "$DB.$TABLE already has the new layout; nothing to do."
  exit 0
fi

alter="ALTER TABLE \`$DB\`.\`$TABLE\`"
for ((k = 0; k < ${#changes[@]}; k++)); do
  alter+=$'\n  '"${changes[k]}"
  [ $k -lt $((${#changes[@]} - 1)) ] && alter+=","
done
alter+=";"

log "$DB.$TABLE: $((table_kb / 1024 / 1024)) GB, data directory $datadir ($((free_kb / 1024 / 1024)) GB free)."
log "Current primary key: ($primary_key)"
echo
echo "$alter"
echo
log "This stops $SERVICE while the table is rebuilt (expect 10-30 minutes)."
read -r -p "Continue? [y/N] " answer
[ "$answer" = "y" ] || [ "$answer" = "Y" ] || fail "Cancelled."

# --- Sort files on the data directory's filesystem, not in /tmp on the root filesystem ---------

tmpdir=$datadir_mount/mysql-tmp
mkdir -p "$tmpdir"
chown mysql:mysql "$tmpdir"
chmod 750 "$tmpdir"
if [ -f "$APPARMOR_PROFILE" ] && ! grep -qF "$tmpdir/** rwk," "$APPARMOR_LOCAL" 2>/dev/null; then
  echo "$tmpdir/ r,"    >> "$APPARMOR_LOCAL"
  echo "$tmpdir/** rwk," >> "$APPARMOR_LOCAL"
  apparmor_parser -r "$APPARMOR_PROFILE"
  log "Allowed $tmpdir in AppArmor."
fi

# --- Stop the daemon, change the table, start the daemon ---------------------------------------

was_active=0
if systemctl is-active --quiet "$SERVICE"; then
  was_active=1
  log "Stopping $SERVICE..."
  systemctl stop "$SERVICE"
fi
trap '[ $was_active -eq 1 ] && systemctl start "$SERVICE" && log "Started $SERVICE again."' EXIT

log "Rebuilding $DB.$TABLE..."
start=$(date +%s)
mysql -e "SET SESSION innodb_tmpdir = '$tmpdir'; $alter"
log "Done in $(( ($(date +%s) - start) / 60 )) min."

mysql -N -B -e "ANALYZE TABLE \`$DB\`.\`$TABLE\`;" > /dev/null
new_kb=$(( $(sql "SELECT data_length + index_length FROM information_schema.tables
                   WHERE table_schema = '$DB' AND table_name = '$TABLE';") / 1024 ))
log "$DB.$TABLE: $((table_kb / 1024)) MB before, $((new_kb / 1024)) MB after."
log "Indexes:"
mysql -e "SELECT index_name, GROUP_CONCAT(column_name ORDER BY seq_in_index) AS columns
          FROM information_schema.statistics WHERE table_schema = '$DB' AND table_name = '$TABLE'
          GROUP BY index_name;"
rmdir "$tmpdir" 2>/dev/null || true
