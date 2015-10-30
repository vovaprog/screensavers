#!/bin/bash

WIDTH=1680
HEIGHT=1050
ITERATIONS=1000000000
FRACFILE="crystal.xml"

./build/fractal_flame --mode render --width $WIDTH --height $HEIGHT --iterations $ITERATIONS --input-file ./fractals/$FRACFILE

