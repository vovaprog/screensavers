#!/bin/bash

WIDTH=1680
HEIGHT=1050
ITERATIONS=2000000000
FRACFILE="crystal.xml"
FRACFOLDER="./fractals"
FRACPROGRAM="./build/fractal_flame"

$FRACPROGRAM --mode render --width $WIDTH --height $HEIGHT --iterations $ITERATIONS --input-file $FRACFOLDER/$FRACFILE

