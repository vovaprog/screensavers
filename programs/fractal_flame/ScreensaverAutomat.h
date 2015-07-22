#pragma once

#include <small_utils.h>

enum class AutomatState{ FIRST,CALC_NEXT_FRACTAL,SHOW_IDLE,TRANSIT_START,TRANSIT_PROCESS };

class ScreensaverAutomat{
private:
    AutomatState state;
    const unsigned int TRANSIT_MILLIS = 3 * 1000;
    const unsigned int SHOW_MILLIS = 5 * 1000;
    
    unsigned int startMillis;
    unsigned int *output0,*output1,*outputBlend;
    
    int pictureWidth,pictureHeight,outputSize;
    
    double blendKoef = 0.0;
    
    int fps;

public:
    ScreensaverAutomat(int pictureWidth,int pictureHeight,int fps):state(AutomatState::FIRST)
    {
        this->pictureWidth=pictureWidth;
        this->pictureHeight=pictureHeight;
        this->outputSize = this->pictureWidth * this->pictureHeight;
        this->fps=fps;
    }
    
    unsigned int* nextFrame()
    {
        switch(state){
        case AutomatState::FIRST:
            return handleFirst();
        case AutomatState::CALC_NEXT_FRACTAL:
            return handleCalcNextFractal();
        case AutomatState::SHOW_IDLE:
            return handleShowIdle();
        case AutomatState::TRANSIT_START:
            return handleTransitStart();
        case AutomatState::TRANSIT_PROCESS:
            return handleTransitProcess();
        default:
            return nullptr;
        }
    }
    
private:    
    unsigned int* handleFirst()
    {
        output0=new unsigned int[outputSize];
        output1=new unsigned int[outputSize];
        outputBlend=new unsigned int[outputSize];
        
        fractalInit(pictureWidth,pictureHeight);
                
        startMillis=getMilliseconds();
                
        unsigned int *p = fractalRandom();
        memcpy(output0,p,sizeof(unsigned int) * outputSize);
        
        state=AutomatState::CALC_NEXT_FRACTAL;        
        return output0;
    }
    
    unsigned int* handleCalcNextFractal()
    {
        unsigned int *outputOriginal = fractalRandom();
        
        memcpy(output1,outputOriginal,sizeof(unsigned int) * outputSize);
        
        state=AutomatState::SHOW_IDLE;
        return output0;
    }
    
    unsigned int* handleShowIdle()
    {
        unsigned int millisPassed = getMilliseconds() - startMillis;
        
        if(millisPassed>=SHOW_MILLIS)
        {
            state=AutomatState::TRANSIT_START;        
        }
        else
        {
            state=AutomatState::SHOW_IDLE;
        }        
        
        return output0;
    }       
    
    unsigned int* handleTransitStart()
    {
        startMillis=getMilliseconds();
        
        //double 
        
        double blendD=2.0 / ((TRANSIT_MILLIS / 1000.0) * fps);
        blendKoef=2.0-blendD;
        
        
        //cout << "blendKoef: " <<blendKoef<<"   blendD: "<<blendD<<endl<<flush;
        
        blend(output0,output1,outputBlend,blendKoef);
     
        state=AutomatState::TRANSIT_PROCESS;
        
        return outputBlend;        
    }
    
    unsigned int* handleTransitProcess()
    {
        unsigned int millisPassed = getMilliseconds() - startMillis;
        
        cout << "passed: "<<millisPassed<<endl;
        
        if(millisPassed >= TRANSIT_MILLIS)
        {
            unsigned int *p = output0;
            output0=output1;
            output1=p;
            state=AutomatState::CALC_NEXT_FRACTAL;
            return output0;
        }
        else
        {
            unsigned int millisLeft = TRANSIT_MILLIS - millisPassed;
            
            double blendD=blendKoef / ((millisLeft / 1000.0) * fps);
            blendKoef-=blendD;
            
            if(blendKoef < 0.0)
            {
                blendKoef=0.0;
            }
            
            //cout << "blendKoef: " <<blendKoef<<"   blendD: "<<blendD<<endl<<flush;
            
            blend(output0,output1,outputBlend,blendKoef);
            
            state=AutomatState::TRANSIT_PROCESS;
            
            return outputBlend;
        }
    }
    
    void blend(unsigned int *p0,unsigned int *p1,unsigned int *output,double k)
    {
        for(int i=0;i<outputSize;i++)
        {
            unsigned int r = (unsigned int)((k*GetRed(p0[i]) + (2.0-k)*GetRed(p1[i])) / 2.0);
            unsigned int g = (unsigned int)((k*GetGreen(p0[i]) + (2.0-k)*GetGreen(p1[i])) / 2.0);
            unsigned int b = (unsigned int)((k*GetBlue(p0[i]) + (2.0-k)*GetBlue(p1[i])) / 2.0);
            
            output[i] = CreateColor(r,g,b);            
        }        
    }
};
