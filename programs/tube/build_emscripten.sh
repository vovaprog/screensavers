#!/bin/bash

MYLIB=../../mylib

EMCC=~/tools/emscripten/emscripten/1.37.22/em++

$EMCC -std=c++11 -O3 -Wall -I. -I$MYLIB -I./external ./tube.cpp ./LookAt.cpp $MYLIB/ColorMap.cpp -o ./tube.html -s LEGACY_GL_EMULATION=1 -lglut -lGL -lGLU --shell-file ./template.html 
