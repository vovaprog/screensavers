MYLIB=../../mylib
EXTERNAL=../../external

ifeq ($(OS),Windows_NT)

COMMAND=g++ -std=c++11 -O3 -fopenmp -mstackrealign \
		-mfpmath=sse -msse -msse2 -msse3 \
		-Wl,-s -Wall \
		-I. -I$(MYLIB) -I$(EXTERNAL)/include -I$(EXTERNAL)/vectorclass -I$(EXTERNAL)/inc \
		./main.cpp ./flame_fractal.cpp  \
		-o C:\files\programs\flame_fractal.exe \
		-L$(EXTERNAL)/lib -lfreeglut -lopengl32 -lglu32 -lgdi32

#		-o ./build_mingw/flame_fractal.exe 
		
else

COMMAND=g++ -std=c++11 -O3 -fopenmp -Wall -mstackrealign \
		-march=native \
		-I. -I$(MYLIB) -I$(EXTERNAL)/include \
		./main.cpp ./flame_fractal.cpp ./init_functions.cpp \
		-o ./fractal_flame \
		-lglut -lGL -lGLU
endif

all:
	$(COMMAND)

