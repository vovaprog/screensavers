#include <string>
#include <string.h>

#include "ScreensaverAutomat.h"

using namespace std;

ScreensaverAutomat::ScreensaverAutomat(int pictureWidth,int pictureHeight,int fps):state(AutomatState::FIRST),threadController(&fractal)
{
    this->pictureWidth=pictureWidth;
    this->pictureHeight=pictureHeight;
    this->outputSize = this->pictureWidth * this->pictureHeight;
    this->fps=fps;
    
    threadController.setPeriodMilliseconds(SHOW_MILLIS + TRANSIT_MILLIS);
    
    initSaveDirectory();
}

ScreensaverAutomat::~ScreensaverAutomat()
{
    if(output0!=nullptr)
    {
        delete[] output0;    
    }
    if(output1!=nullptr)
    {
        delete[] output1;    
    }
    if(outputBlend!=nullptr)
    {
        delete[] outputBlend;    
    }
}

void ScreensaverAutomat::initSaveDirectory()
{		
    if(directoryExists(saveDirName))
    {	
        deleteDirectory(saveDirName);						
    }   
    createDirectory(saveDirName);
}    
        
unsigned int* ScreensaverAutomat::nextFrame()
{
    switch(state){
    case AutomatState::FIRST:
        return handleFirst();
    case AutomatState::SECOND:
        return handleSecond();        
    case AutomatState::SHOW_IDLE:
        return handleShowIdle();
    case AutomatState::WAIT_RESULT:
        return handleWaitResult();
    case AutomatState::TRANSIT_START:
        return handleTransitStart();
    case AutomatState::TRANSIT_PROCESS:
        return handleTransitProcess();
    default:
        return nullptr;
    }
}
    
unsigned int* ScreensaverAutomat::handleFirst()
{        
    output0=new unsigned int[outputSize];
    output1=new unsigned int[outputSize];
    outputBlend=new unsigned int[outputSize];
    
    fractal.init(pictureWidth,pictureHeight);

    threadController.beginCalculateFractal();
            
    memset(output0,0,sizeof(unsigned int) * outputSize);
    
    startMillis=getMilliseconds();
    
    
    state=AutomatState::SECOND;        
    return output0;
}
    
unsigned int* ScreensaverAutomat::handleSecond()
{        
    unsigned int millisPassed = getMilliseconds() - startMillis;
    
    if(millisPassed>=SECOND_MILLIS)
    {
        fractal.setStopFlag();
        startMillis=getMilliseconds();
        state=AutomatState::WAIT_RESULT;        
    }
    else
    {
        state=AutomatState::SECOND;
    }
    
    return output0;
}


unsigned int* ScreensaverAutomat::handleShowIdle()
{        
    unsigned int millisPassed = getMilliseconds() - startMillis;
    
    if(millisPassed>=SHOW_MILLIS)
    {
        fractal.setStopFlag();
        startMillis=getMilliseconds();
        state=AutomatState::WAIT_RESULT;
    }
    else
    {
        state=AutomatState::SHOW_IDLE;
    }        
    
    return output0;
}       
    
unsigned int* ScreensaverAutomat::handleWaitResult()
{
    unsigned int millisPassed = getMilliseconds() - startMillis;
    
    if(millisPassed>=WAIT_RESULT_MILLIS)
    {
        unsigned int *p;
        FractalFlame::CalculateFractalResult result = threadController.getResultWithTimeout(&p);
        if(result==FractalFlame::CalculateFractalResult::SUCCESS)
        {
            memcpy(output1,p,sizeof(unsigned int) * outputSize);
            
            fractal.saveCurrentFractal(saveDirName,imageCounter % saveNumberOfImages);
            imageCounter += 1;
            
            threadController.beginCalculateFractal();
            state=AutomatState::TRANSIT_START;
        }
        else if(result==FractalFlame::CalculateFractalResult::TIMEOUT)
        {
            state=AutomatState::WAIT_RESULT;
        }
        else if(result==FractalFlame::CalculateFractalResult::BAD_PICTURE)
        {
            threadController.beginCalculateFractal();
            startMillis=getMilliseconds();
            state=AutomatState::SHOW_IDLE;
        }
    }
    else
    {
        state=AutomatState::WAIT_RESULT;
    }
    return output0;
}

unsigned int* ScreensaverAutomat::handleTransitStart()
{         
    double blendD=1.0 / ((TRANSIT_MILLIS / 1000.0) * fps);
    blendKoef=1.0-blendD;
    
    blend(output0,output1,outputBlend,blendKoef);
 
    startMillis=getMilliseconds();
    
    state=AutomatState::TRANSIT_PROCESS;        
    return outputBlend;        
}
    
unsigned int* ScreensaverAutomat::handleTransitProcess()
{        
    unsigned int millisPassed = getMilliseconds() - startMillis;
    
    if(millisPassed >= TRANSIT_MILLIS)
    {
        unsigned int *p = output0;
        output0=output1;
        output1=p;
        state=AutomatState::SHOW_IDLE;
        startMillis=getMilliseconds();
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
        
        blend(output0,output1,outputBlend,blendKoef);
        
        state=AutomatState::TRANSIT_PROCESS;
        
        return outputBlend;
    }
}
    
void ScreensaverAutomat::blend(unsigned int *p0,unsigned int *p1,unsigned int *output,double k)
{
    for(int i=0;i<outputSize;i++)
    {
        unsigned int r = (unsigned int)(k*GetRed(p0[i]) + (1.0-k)*GetRed(p1[i]));
        unsigned int g = (unsigned int)(k*GetGreen(p0[i]) + (1.0-k)*GetGreen(p1[i]));
        unsigned int b = (unsigned int)(k*GetBlue(p0[i]) + (1.0-k)*GetBlue(p1[i]));
        
        output[i] = CreateColor(r,g,b);            
    }        
}

