#pragma once

#include <small_utils.h>

enum class AutomatState{ FIRST,IDLE };

class ScreensaverAutomat{
private:
    AutomatState state;
    const int TRANSIT_MILLIS = 3 * 1000;
    const int SHOW_MILLIS = 10 * 1000;
    
    unsigned int startMillis;
    unsigned int *output0,*output1,*outputBlend;
    
    int pictureWidth,pictureHeight,pictureSize;

public:
    ScreensaverAutomat(int pictureWidth,int pictureHeight):state(AutomatState::FIRST)
    {
        this->pictureWidth=pictureWidth;
        this->pictureHeight=pictureHeight;
        this->pictureSize=this->pictureWidth * this->pictureHeight; 
    }
    
    unsigned int* nextFrame()
    {
        switch(state){
        case AutomatState::FIRST:
            return handleFirst();
        case AutomatState::IDLE:
            return handleIdle();
        default:
            return nullptr;
        }
    }
    
    unsigned int* handleFirst()
    {
        output0=new unsigned int[pictureSize];
        output1=new unsigned int[pictureSize];
        output_blend=new unsigned int[pictureSize];
        
        fractalInit(pictureWidth,pictureHeight);
        
                
        startMillis=getMilliseconds();
        
        output0 = fractalRandom();
        
        state=AutomatState::CALC_NEXT_FRACTAL;        
        return output0;
    }
    
    unsigned int* handleCalcNextFractal()
    {
        unsigned int *outputOriginal = fractalRandom();
        
        memcpy(output1,outputOriginal,sizeof(unsigned int) * pictureSize);
        
        state=AutomatState::SHOW_IDLE;
        return output0;
    }
    
    unsigned int* handleShowIdle()
    {
        unsigned int millisPassed = getMilliseconds() - startMillis;
        
        if(secondsPassed>=SHOW_MILLIS)
        {
            state=AutomatState::TRANSIT_START;        
        }
        else
        {
            state=AutomatState::SHOW_IDLE;
        }        
        
        return output0;
    }
    
    void blend()
    {
        for(int i=0;i<outputSize;i++)
        {
            
            
        }        
    }
    
    unsigned int* handleTransitStart()
    {
        startMillis=getMilliseconds();
        
        blendD=1.0 / (fps * TRANSIT_SECONDS);
        blendKoef=blendD;
        
        blend(output0,output1,blendKoef);
     
        state=AutomatState::TRANSIT_PROCESS;
        
        return output_blend;
    }
    
    unsigned int* handleTransitProcess()
    {
        unsigned int millisPassed = getMilliseconds() - startMillis;
        
        if(millisPassed >= TRANSIT_MILLIS)
        {
            unsigned int *p = output0;
            output0=output1;
            output1=p;
            state=AutomatState::handleCalcNextFractal;
            return output0;
        }
        else
        {
            unsigned int millisLeft = TRANSIT_MILLIS - millisPassed;
            
            blendD=1.0 / (fps * ((double)millisLeft / 1000.0));
            blendKoef+=blendD;
            
            blend(output0,output1,blendKoef);
            
            state=AutomatState::TRANSIT_PROCESS;
            
            return output_blend;
        }
    }
};
