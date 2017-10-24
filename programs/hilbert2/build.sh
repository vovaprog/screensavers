
g++ -O3 -std=c++11 -Wall -I. ./hilbert2d_console.cpp -o ./hilbert2d_console

MYLIB=../../mylib
g++ -O3 -std=c++11 -Wall -I. -I$MYLIB ./hilbert.cpp -o ./hilbert -lglut -lGL -lGLU
