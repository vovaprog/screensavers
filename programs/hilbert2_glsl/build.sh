#!/bin/bash

g++ -O3 -Wall -march=native -std=c++11 -I. -I../../mylib -I../hilbert2 ./hilbert2_glsl.cpp -o hilbert2_glsl -lglut -lGL -lGLU
