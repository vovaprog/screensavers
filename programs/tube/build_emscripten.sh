#!/bin/bash

MYLIB=../../mylib
EMCC=~/tools/emscripten/emscripten/1.37.22/em++

# Copy glm from /usr/include. This is needed not to add all /usr/include. 
mkdir ./external
cp --recursive /usr/include/glm ./external

$EMCC -std=c++11 -O3 -Wall -I. -I$MYLIB -I./external ./tube.cpp ./LookAt.cpp $MYLIB/ColorMap.cpp -o ./tube.html -s LEGACY_GL_EMULATION=1 -lglut -lGL -lGLU --shell-file ./template.html 

rm --recursive ./external
