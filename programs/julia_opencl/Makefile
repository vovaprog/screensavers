MYLIB=../../mylib

all:
	g++ -std=c++11 -O3 -march=native -I$(MYLIB) ./main.cpp ./julia_opencl.cpp $(MYLIB)/filesystem_utils.cpp $(MYLIB)/ColorMap.cpp -o ./julia_opencl -lGL -lglut -lboost_filesystem -lboost_system -lOpenCL

