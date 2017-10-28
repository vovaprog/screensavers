#!/bin/bash

WIDTH=2000
HEIGHT=1500
ITERATIONS=150000000
FRACFILE="bird.xml"
FRACFOLDER="./fractals"
FRACPROGRAM="./build/fractal_flame"

$FRACPROGRAM --mode render --width $WIDTH --height $HEIGHT --iterations $ITERATIONS --input-file $FRACFOLDER/$FRACFILE

