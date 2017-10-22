
g++ -O3 -std=c++11 -I. ./hilbert2d_console.cpp -o ./hilbert2d_console

MYLIB=../../mylib
g++ -O3 -std=c++11 -I. -I$MYLIB ./hilbert.cpp -o ./hilbert -lglut -lGL -lGLU
