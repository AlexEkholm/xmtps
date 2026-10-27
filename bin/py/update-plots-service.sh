#!/bin/bash

source /data1/alamo/bin/py/.venv/bin/activate

# static options 

DLIST="/data1/alamo/data/list"
DJSON="/data1/alamo/data/json"
DPLOT="/data1/alamo/data/plots2"

DBIN="/data1/alamo/bin/py"

MAX_PROC=8
MAX_RUN_T=3600
PID=$$

SLEEP_SEC=300

# run forever
while [ 1 ]; do

echo "`date -uIseconds` --- start ---"
epoch_start=`date +%s`

for id in `find /data1/alamo/data/list/ -cmin -14400 -type f | awk -F/ '{print $6}' | sort -nu`; do
#for id in `find /data1/alamo/data/list/ -type f | awk -F/ '{print $6}' | sort -nu`; do

    # run json and plot updates in background
    (   source /data1/alamo/bin/py/.venv/bin/activate
        /data1/alamo/bin/py/.venv/bin/python $DBIN/xmtps-list2json.py -v -i"$DLIST/(908|909|91|92|93|94|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST/$id"
        /data1/alamo/bin/py/.venv/bin/python $DBIN/xmtps-aggregate-json.py -v -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" -p"$DPLOT" "$DJSON/$id/dives" 
    ) &

    # Dont spawn more processes until below threshold
    while [ 1 ]; do
        NPROC=`ps --no-headers -o pid --ppid="$PID" | wc -w`
        if [ "$NPROC" -le "$MAX_PROC" ]; then
            break
        fi
        sleep 1
    done
done

# wait for processes to complete
while [ 1 ]; do

    # check how many processes are still running
    NPROC=`ps --no-headers -o pid --ppid="$PID" | wc -w`
    if [ "$NPROC" -le "1" ]; then break; fi

    # kill processes if they hang (runtime exceeded))
    epoch_now=`date +%s`
    run_time="$(($epoch_now - $epoch_start))" 
    if [ "$run_time" -gt "$MAX_RUN_T" ]; then
        echo "Run time exceeded. Killing children..."
        kill `ps --no-headers -o pid --ppid=$PID`
        break
    fi

    sleep 1
done


# make summary, mission parameters and startup pages, with the run time of the JSON and plot updates in their footers
epoch_now=`date +%s`
python $DBIN/xmtps-generate-summary-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT" --runtime "$(($epoch_now - $epoch_start))" $DJSON
# engineering pages, published at /etcetc/alamo/engineering/ (a link to $DPLOT/engineering); their
# float links go to the float pages at /alamo/
mkdir -p "$DPLOT/engineering"
python $DBIN/xmtps-generate-mission-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" --plots-url /alamo --runtime "$(($epoch_now - $epoch_start))" $DJSON
python $DBIN/xmtps-generate-startup-page.py -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT/engineering" --plots-url /alamo --runtime "$(($epoch_now - $epoch_start))" $DJSON


epoch_now=`date +%s`
run_time="$(($epoch_now - $epoch_start))" 
#echo "`date -uIns` --- done, run time = $run_time seconds, start = $epoch_start, end = $epoch_now ---"
echo "`date -uIseconds` --- done, run time = $run_time ---"

# sleep for duration
sleep $SLEEP_SEC

done


