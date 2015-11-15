project (fractal_flame)

set(CMAKE_VERBOSE_MAKEFILE on)

set(MYLIB ../../mylib)

include_directories("." ${MYLIB})

set(SOURCE main.cpp FractalFlame2.cpp FractalFlameAlgorithm.cpp FlameParameters.cpp Variation.cpp  
           ScreensaverAutomat.cpp FractalThreadController.cpp FractalThreadPoolController.cpp ScreensaverAutomatPool.cpp ImageUtils.cpp ${MYLIB}/filesystem_utils.cpp ${MYLIB}/small_utils.cpp ${MYLIB}/ThreadPool.cpp) 

        
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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Ob2 /Og /Oi /Ot /Oy /GL /EHsc")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")

    if(NOT (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL DEBUG))
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
    endif()
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_THREAD_POOL")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_LOOKUP")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_IMAGE_FUNCTIONS")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_XML_FUNCTIONS")


#add_library(fractal_flame_library ${SOURCE})
add_executable(fractal_flame ${SOURCE})


if(MSVC OR MINGW)

    target_link_libraries(fractal_flame opengl32 glu32 freeglut freeimage boost_system boost_filesystem boost_program_options)

else()

    target_link_libraries(fractal_flame GL GLU glut tinyxml freeimage boost_system boost_filesystem boost_program_options pthread)

endif()

