#!/bin/bash

g++ -O3 -march=native -std=c++11 -Wall -I. -I../../mylib ./model.cpp -o model -lglut -lGL -lGLU
