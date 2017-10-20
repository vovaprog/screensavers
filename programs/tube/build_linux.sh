#!/bin/bash

MYLIB=../../mylib

OUTDIR=./build
rm --recursive $OUTDIR
mkdir --parents $OUTDIR

g++ -std=c++11 -O3 -march=native -Wall -I. -I$MYLIB ./tube.cpp $MYLIB/ColorMap.cpp -o $OUTDIR/tube -lglut -lGL -lGLU
