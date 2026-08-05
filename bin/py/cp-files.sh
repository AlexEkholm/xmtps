#!/bin/bash

for x in FloatClasses.py xmtps-aggregate-json.py xmtps-list2json.py xmtps-generate-summary-page.py; do

    echo $x
    (echo '#!/usr/bin/python3'; tr -d '\r' < ~/$x) > $x

done
