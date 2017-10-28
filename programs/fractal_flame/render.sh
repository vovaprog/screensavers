#!/bin/bash

WIDTH=2000
HEIGHT=1500
ITERATIONS=2000000000
FRACFILE="crystal.xml"
FRACFOLDER="./fractals"
FRACPROGRAM="./build/fractal_flame"

$FRACPROGRAM --mode render --width $WIDTH --height $HEIGHT --iterations $ITERATIONS --input-file $FRACFOLDER/$FRACFILE

