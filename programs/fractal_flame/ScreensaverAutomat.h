#pragma once

#include <small_utils.h>
#include <filesystem_utils.h>
#include "flame_fractal.h"
#include "FractalThreadController.h"

enum class AutomatState{ FIRST,SECOND,SHOW_IDLE,WAIT_RESULT,TRANSIT_START,TRANSIT_PROCESS };

class ScreensaverAutomat{
private:
    AutomatState state;
    const unsigned int TRANSIT_MILLIS = 3 * 1000;
    const unsigned int SHOW_MILLIS = 5 * 1000;
    const unsigned int WAIT_RESULT_MILLIS = 1 * 1000;
    const unsigned int SECOND_MILLIS = 1 * 1000;
    
    unsigned int startMillis;
    unsigned int *output0=nullptr,*output1=nullptr,*outputBlend=nullptr;
    
    int pictureWidth,pictureHeight,outputSize;
    
    double blendKoef = 0.0;
    
    int fps;

    FractalThreadController threadController;
    
    const char *saveDirName="./fractals/screensaver";
    int imageCounter=0;
    int saveNumberOfImages=10;
    
public:
    ScreensaverAutomat(int pictureWidth,int pictureHeight,int fps);
        
    ~ScreensaverAutomat();
    
    unsigned int* nextFrame();
    
private:    
    unsigned int* handleFirst();
    
    unsigned int* handleSecond();
    
    unsigned int* handleShowIdle();
    
    unsigned int* handleWaitResult();
    
    unsigned int* handleTransitStart();
    
    unsigned int* handleTransitProcess();
    
    void blend(unsigned int *p0,unsigned int *p1,unsigned int *output,double k);
    
    void initSaveDirectory();
};
