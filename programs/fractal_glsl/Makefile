MYLIB=../../mylib

all:
	g++ -O3 -march=native -I$(MYLIB) ./fractal_glsl.cpp $(MYLIB)/shader_utils.cpp $(MYLIB)/filesystem_utils.cpp $(MYLIB)/ColorMap.cpp -o ./fractal_glsl -lGL -lglut -lGLU -lboost_filesystem -lboost_system


