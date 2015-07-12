MYLIB=../../mylib
EXTERNAL=../../external

ifeq ($(OS),Windows_NT)

COMMAND=g++ -O3 -fopenmp -mstackrealign \
		-mfpmath=sse -msse -msse2 -msse3 \
		-Wl,-s -Wall \
		-I. -I$(MYLIB) -I$(EXTERNAL)/include -I$(EXTERNAL)/vectorclass -I$(EXTERNAL)/inc \
		./main.cpp $(MYLIB)/ColorMap.cpp \
		-o ./build_mingw/tube.exe \
		-L$(EXTERNAL)/lib -lfreeglut -lopengl32 -lglu32 -lgdi32
		
else

COMMAND=g++ -std=c++11 -O3 -fopenmp -Wall -mstackrealign \
		-march=native \
		-I. -I$(MYLIB) -I$(EXTERNAL)/vectorclass -I$(EXTERNAL)/include \
		./main.cpp $(MYLIB)/ColorMap.cpp \
		-o ./tube \
		-lglut -lGL -lGLU
endif

all:
	$(COMMAND)

