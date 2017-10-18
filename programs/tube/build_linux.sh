#!/bin/bash

MYLIB=../../mylib

g++ -std=c++11 -O3 -march=native -Wall -I. -I$MYLIB ./main.cpp $MYLIB/ColorMap.cpp -o ./tube -lglut -lGL -lGLU
