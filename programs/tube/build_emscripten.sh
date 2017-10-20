#!/bin/bash

MYLIB=../../mylib
EMCC=~/tools/emscripten/emscripten/1.37.22/em++

# Copy glm from /usr/include. This is needed not to add all /usr/include. 
mkdir ./external
cp --recursive /usr/include/glm ./external

# Generate html with js file.
OUTDIR=./build_emscripten/html
rm --recursive $OUTDIR
mkdir --parents $OUTDIR
$EMCC -std=c++11 -O3 -Wall -I. -I$MYLIB -I./external ./tube.cpp ./LookAt.cpp $MYLIB/ColorMap.cpp -o $OUTDIR/tube.html -s LEGACY_GL_EMULATION=1 -lglut -lGL -lGLU --shell-file ./template.html

# Generate html and compile code to web assembly.
OUTDIR=./build_emscripten/webassembly
rm --recursive $OUTDIR
mkdir --parents $OUTDIR
$EMCC -std=c++11 -O3 -Wall -I. -I$MYLIB -I./external ./tube.cpp ./LookAt.cpp $MYLIB/ColorMap.cpp -o $OUTDIR/tube.html -s WASM=1 -s LEGACY_GL_EMULATION=1 -lglut -lGL -lGLU --shell-file ./template.html

rm --recursive ./external
