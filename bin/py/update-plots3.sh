#!/bin/bash

DLIST="/data1/alamo/data/list"
DJSON="/data1/alamo/data/json"
DPLOT="/data1/alamo/data/plots2"

MAX_PROC=16
PID=$$

#./xmtps-list2json.py -vr -i"$DLIST/(908|909|91|92|93|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST"
#./xmtps-list2json.py -vr -i"$DLIST/(11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST"
#./xmtps-aggregate-json.py -vr -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" -p"$DPLOT" "$DJSON"
#./xmtps-aggregate-json.py -vr -i"$DJSON/[0-9]+/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" "$DJSON"

echo "`date -uIns` PID=$PID"

#for id in `find /data1/alamo/data/list/ -cmin -14400 -type f | awk -F/ '{print $6}' | sort -nu`; do
#for id in `find /data1/alamo/data/list/ -type f | awk -F/ '{print $6}' | sort -nu`; do
for id in `find /data1/alamo/data/list/ -type f | awk -F/ '{print $6}' | sort -nu | egrep '(92|93|11)[0-9]+'`; do

    
    echo "`date -uIns` Updating plots for float $id..."


    (   #./xmtps-list2json.py -f -i"$DLIST/[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST/$id" > /dev/null
        #./xmtps-aggregate-json.py -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" "$DJSON/$id/dives" > /dev/null
        ./xmtps-aggregate-json.py -f -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" -p "$DPLOT" "$DJSON/$id/dives" > /dev/null
    ) &

    # Dont spawn more processes until below threshold

    while [ 1 ]; do
        
        NPROC=`ps --no-headers -o pid --ppid="$PID" | wc -w`

        echo "`date -uIns` NPROC=$NPROC"
         
        if [ "$NPROC" -le "$MAX_PROC" ]; then

            break

        fi

        sleep 1

    done

done

while [ 1 ]; do
    NPROC=`ps --no-headers -o pid --ppid="$PID" | wc -w`

    echo "`date -uIns` NPROC=$NPROC"
         
    if [ "$NPROC" -le "2" ]; then break; fi

    sleep 1

done

wait

#./xmtps-generate-summary-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT" $DJSON

echo "`date -uIns` --- done ---"
