#!/bin/bash

MYLIB=../../mylib
EMCC=~/tools/emscripten/emscripten/1.37.22/em++

# Generate html with js file.
OUTDIR=./build_emscripten/html
rm --recursive $OUTDIR
mkdir --parents $OUTDIR
$EMCC -std=c++11 -O3 -Wall -I. ./hilbert2d_console.cpp -o $OUTDIR/hilbert2dconsole.html --shell-file ./template.html

# Generate html and compile code to web assembly.
OUTDIR=./build_emscripten/webassembly
rm --recursive $OUTDIR
mkdir --parents $OUTDIR
$EMCC -std=c++11 -O3 -Wall -I. ./hilbert2d_console.cpp -o $OUTDIR/hilbert2dconsole.html --shell-file ./template.html -s WASM=1

# Generate html with js file.
OUTDIR=./build_emscripten/html3d
rm --recursive $OUTDIR
mkdir --parents $OUTDIR
$EMCC -std=c++11 -O3 -Wall -I. -I$MYLIB ./hilbert.cpp -o $OUTDIR/hilbert3d.html -s LEGACY_GL_EMULATION=1 -lglut -lGL -lGLU --shell-file ./template3d.html

