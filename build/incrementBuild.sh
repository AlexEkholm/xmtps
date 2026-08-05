#!/bin/bash

BFILE=../build/buildNo.txt

if [ ! -f $BFILE ]; then
    echo "0" > $BFILE;
fi

BNO=$(cat $BFILE)
BNO=$((BNO+1))
echo "$BNO" | tee $BFILE
