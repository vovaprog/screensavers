#include <string>
#include <string.h>

#include "ScreensaverAutomat.h"

using namespace std;

ScreensaverAutomat::ScreensaverAutomat(int pictureWidth,int pictureHeight,int fps):state(AutomatState::FIRST)
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
    
unsigned int* ScreensaverAutomat::handleFirst()
{        
    output0=new unsigned int[outputSize];
    output1=new unsigned int[outputSize];
    outputBlend=new unsigned int[outputSize];
    
    fractalInit(pictureWidth,pictureHeight);
            
    startMillis=getMilliseconds();
            
    threadController.beginCalculateFractal();
    unsigned int *p = threadController.getResult();
    if(p!=nullptr)
    {
        memcpy(output0,p,sizeof(unsigned int) * outputSize);
        
        saveCurrentFractal(saveDirName,imageCounter % saveNumberOfImages);
        imageCounter += 1;
    }
    else
    {
        memset(output0,0,sizeof(unsigned int) * outputSize);    
    }
    
    threadController.beginCalculateFractal();
            
    state=AutomatState::SHOW_IDLE;        
    return output0;
}
    
unsigned int* ScreensaverAutomat::handleShowIdle()
{        
    unsigned int millisPassed = getMilliseconds() - startMillis;
    
    if(millisPassed>=SHOW_MILLIS)
    {
        unsigned int *p = threadController.getResultWithTimeout();
        if(p!=nullptr)
        {
            memcpy(output1,p,sizeof(unsigned int) * outputSize);
            
            saveCurrentFractal(saveDirName,imageCounter % saveNumberOfImages);
            imageCounter += 1;
            
            threadController.beginCalculateFractal();
            state=AutomatState::TRANSIT_START;
        }
        else
        {
            state=AutomatState::SHOW_IDLE;
        }                        
    }
    else
    {
        state=AutomatState::SHOW_IDLE;
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

