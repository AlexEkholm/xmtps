#!/bin/bash

DLIST="/data1/alamo/data/list"
DJSON="/data1/alamo/data/json"
DPLOT="/data1/alamo/data/plots2"

DBIN="/data1/alamo/bin/py"

MAX_PROC=8
PID=$$

SLEEP_SEC=300

#./xmtps-list2json.py -vr -i"$DLIST/(908|909|91|92|93|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST"
#./xmtps-list2json.py -vr -i"$DLIST/(11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST"
#./xmtps-aggregate-json.py -vr -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" -p"$DPLOT" "$DJSON"
#./xmtps-aggregate-json.py -vr -i"$DJSON/[0-9]+/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" "$DJSON"

#echo "`date -uIns` PID=$PID"


echo "`date -uIns` --- start ---"

while [ 1 ]; do

for id in `find /data1/alamo/data/list/ -cmin -14400 -type f | awk -F/ '{print $6}' | sort -nu`; do
#for id in `find /data1/alamo/data/list/ -type f | awk -F/ '{print $6}' | sort -nu`; do

    
    echo "`date -uIns` Updating plots for float $id..."


    (   $DBIN/xmtps-list2json.py -i"$DLIST/(908|909|91|92|93|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST/$id" > /dev/null
        $DBIN/xmtps-aggregate-json.py -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" -p"$DPLOT" "$DJSON/$id/dives" > /dev/null
    ) &

    # Dont spawn more processes until below threshold

    while [ 1 ]; do
        
        NPROC=`ps --no-headers -o pid --ppid="$PID" | wc -w`

#        echo "`date -uIns` NPROC=$NPROC"
         
        if [ "$NPROC" -le "$MAX_PROC" ]; then

            break

        fi

        sleep 1

    done

done

while [ 1 ]; do
    NPROC=`ps --no-headers -o pid --ppid="$PID" | wc -w`

#    echo "`date -uIns` NPROC=$NPROC"
         
    if [ "$NPROC" -le "2" ]; then break; fi

    sleep 1

done

wait

$DBIN/xmtps-generate-summary-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT" $DJSON

sleep $SLEEP_SEC

done


echo "`date -uIns` --- done ---"
