project (fractal_flame)

set(MYLIB ../../mylib)

include_directories("." ${MYLIB})

set(SOURCE main.cpp flame_fractal.cpp init_functions.cpp Variation.cpp init_functions.cpp 
        ScreensaverAutomat.cpp FractalThreadController.cpp ${MYLIB}/filesystem_utils.cpp ${MYLIB}/small_utils.cpp) 

if(MSVC OR MINGW)
    set(CPPLIBS "C:/cpplibs")
    set(BOOST "C:/Tools/boost")
    include_directories(${INCLUDE_DIRECTORIES} ${CPPLIBS}/include ${BOOST})
    set(TINYXML "${CPPLIBS}/tinyxml")
    set(SOURCE_TINYXML "${TINYXML}/tinystr.cpp" "${TINYXML}/tinyxml.cpp" "${TINYXML}/tinyxmlerror.cpp" "${TINYXML}/tinyxmlparser.cpp")

    set(SOURCE ${SOURCE_TINYXML} ${SOURCE})
    
    link_directories(${CPPLIBS}/lib ${BOOST}/stage/lib)
endif()


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -march=native")
endif()


add_executable(fractal_flame ${SOURCE})


if(MSVC OR MINGW)

    target_link_libraries(fractal_flame opengl32 glu32 freeglut freeimage boost_system boost_filesystem boost_program_options)

else()

    target_link_libraries(fractal_flame GL GLU glut tinyxml freeimage boost_system boost_filesystem boost_program_options pthread)

endif()
