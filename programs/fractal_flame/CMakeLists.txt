
project (fractal_flame)

set(MYLIB ../../mylib)

include_directories("." ${MYLIB})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -march=native")

add_executable(fractal_flame main.cpp flame_fractal.cpp init_functions.cpp Variation.cpp init_functions.cpp 
	ScreensaverAutomat.cpp FractalThreadController.cpp ${MYLIB}/filesystem_utils.cpp ${MYLIB}/small_utils.cpp)

target_link_libraries(fractal_flame GL GLU glut tinyxml freeimage boost_system boost_filesystem boost_program_options pthread)

