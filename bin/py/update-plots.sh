#!/bin/bash

DLIST="/data1/alamo/data/list"
DJSON="/data1/alamo/data/json"
DPLOT="/data1/alamo/data/plots2"

#./xmtps-list2json.py -drf -i"$DLIST/(908|909|91|92|93|10|11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST/9300"
#./xmtps-list2json.py -vr -i"$DLIST/(11)[0-9]+/lis[0-9]+-(-?)[0-9]+.txt" -x"/\\..*" -o"$DJSON" "$DLIST"
./xmtps-aggregate-json.py -drHf -i"$DJSON/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" -p"$DPLOT" "$DJSON/9300"
#./xmtps-aggregate-json.py -vr -i"$DJSON/[0-9]+/[0-9]+/dives/[0-9]+_-?[0-9]+.json" -o"$DJSON" "$DJSON"

#./xmtps-generate-summary-page.py -v -i"$DJSON/[0-9]+/[0-9]+_summary.json" -o"$DPLOT" $DJSON

echo "done"
