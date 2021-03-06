
NDK_TOOLCHAIN_VERSION := 4.8
NDK_TOOLCHAIN_VERSION = clang

#APP_ABI := all
APP_ABI := armeabi-v7a  x86  x86_64  arm64-v8a   mips  mips64   #armeabi

APP_STL := gnustl_static

APP_CPPFLAGS += -std=c++11 -O3 

APP_CPPFLAGS += -DNO_IMAGE_FUNCTIONS 
APP_CPPFLAGS += -DNO_XML_FUNCTIONS
#APP_CPPFLAGS += -DUSE_LOOKUP

LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include

