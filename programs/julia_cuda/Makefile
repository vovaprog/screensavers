MYLIB=../../mylib

all:
	nvcc -c ./CudaFractal.cu
	g++ -O3 -march=native -I$(MYLIB) ./main.cpp $(MYLIB)/ColorMap.cpp -o ./julia_cuda ./CudaFractal.o -lGL -lglut -lGLU -lcuda -lcudart 


