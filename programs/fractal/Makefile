MYLIB=../../mylib
EXTERNAL=../../external

ifeq ($(OS),Windows_NT)

COMMAND=g++ -O3 -fopenmp -mstackrealign \
		-mfpmath=sse -msse -msse2 -msse3 \
		-Wl,-s -Wall \
		-I. -I$(MYLIB) -I$(EXTERNAL)/include -I$(EXTERNAL)/vectorclass \
		./main.cpp ./fractal.cpp $(MYLIB)/fps.cpp $(MYLIB)/ColorMap.cpp \
		-o ./build_mingw/fractal.exe \
		-L$(EXTERNAL)/lib -lfreeglut -lopengl32 
		
else

COMMAND=g++ -std=c++11 -O3 -fopenmp -Wall -mstackrealign \
		-march=native \
		-I. -I$(MYLIB) -I$(EXTERNAL)/vectorclass \
		./main.cpp ./fractal.cpp $(MYLIB)/fps.cpp $(MYLIB)/ColorMap.cpp \
		-o ./fractal \
		-lglut -lGL
endif

all:
	$(COMMAND)

