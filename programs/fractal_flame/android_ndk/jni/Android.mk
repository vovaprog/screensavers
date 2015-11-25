LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := FractalFlame

LOCAL_SRC_FILES := ../../FractalFlame.cpp ../../Variation.cpp ../../FractalFlameAlgorithm.cpp ../../FlameParameters.cpp 
LOCAL_SRC_FILES += ../../ScreensaverAutomatPool.cpp ../../FractalThreadPoolController.cpp    
LOCAL_SRC_FILES += ./JavaInterface.cpp  ../../../../mylib/small_utils.cpp ../../../../mylib/ThreadPool.cpp    

LOCAL_C_INCLUDES += ../.. ../../../../mylib

include $(BUILD_SHARED_LIBRARY)

