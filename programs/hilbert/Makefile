
ifeq ($(OS),Windows_NT)

COMMAND = g++ -O3 -fopenmp -D OS_WINDOWS -Wall -Wl,-s \
	-I../../mylib -I.  -I../../external/include \
	./entry2.cpp ../../mylib/fps.cpp \
	-o ./build_mingw/hilbert.exe \
	-L. -L../../external/lib \
	-lfreeglut -lopengl32 -lglu32 

else

COMMAND=g++ -O3 -Wl,-s -Wall -I../../mylib -I. ./entry2.cpp ../../mylib/fps.cpp -o ./hilbert -lglut -lGL -lGLU

endif

all:
	$(COMMAND)

