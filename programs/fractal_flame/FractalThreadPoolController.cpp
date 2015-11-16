#include "FractalThreadPoolController.h"

#include <string.h>
#include <algorithm>
#include <iostream>

#include <filesystem_utils.h>

using std::find;


FractalThreadPoolController::FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads /* =0 */)
{
    if(numberOfThreads<=0)
    {
        numberOfThreads=ThreadPool::getNumberOfProcessors();
        
        if(numberOfThreads>1)
        {
            numberOfThreads -= 1;
        }
    }
    
    numberOfFractals=numberOfThreads;
    outputSize = pictureWidth * pictureHeight;
    
    fractals=new FractalPoolData*[numberOfThreads];
    
    for(int i=0;i<numberOfThreads;++i)
    {
        fractals[i]=new FractalData();
        fractals[i]->fractal->screensaverInit(pictureWidth,pictureHeight);
    }
    
    pool=new ThreadPool(numberOfThreads);
    
    
/*#ifndef NO_IMAGE_FUNCTIONS    
    if(directoryExists(saveDirName))
    {	
        deleteDirectory(saveDirName);						
    }   
    createDirectory(saveDirName);
#endif*/    
}

FractalThreadPoolController::~FractalThreadPoolController()
{
    for(int i=0;i<numberOfFractals;++i)
    {
        fractals[i]->fractal->setStopFlag();
    }    
    
    delete pool;
    
    for(int i=0;i<numberOfFractals;++i)
    {
        delete fractals[i];
    }
    
    delete[] fractals;
}

void* ThreadFractalFunction(void *dataInput)
{
    FractalThreadPoolController::FractalPoolData *fData=(FractalThreadPoolController::FractalPoolData*)dataInput;
    
    fData->result = fData->fractal->screensaver(&fData->output);
    
    return fData;
}

void FractalThreadPoolController::setStopFlag()
{
    FractalData *fData = calcQueue.front();
    unsigned long lll=(unsigned long)fData;
    cout <<"data pointer stop: "<<lll<<endl;
    
    fData->fractal->setStopFlag();    
}

FractalFlameAlgorithm::CalculateFractalResult FractalThreadPoolController::getResult(unsigned int *output)
{    
    FractalData *fData = (FractalData*)pool->getResult();
    
    unsigned long lll=(unsigned long)fData;
    cout <<"data pointer result: "<<lll<<endl;
    cout <<"queue size: "<<calcQueue.size()<<endl;
    
    FractalFlameAlgorithm::CalculateFractalResult result = fData->result;
    
    if(fData->result == FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
    {
        memcpy(output,fData->output,sizeof(unsigned int) * outputSize);
        
        fData->fractal->screensaverSaveFractal(screensaverSaveDirName,screensaverSaveCounter);    
        screensaverSaveCounter += 1;
        if(screensaverSaveCounter>=saveNumberOfImages) 
        {
            screensaverSaveCounter=0;
        }        
    }    

    
    
    
//#ifndef NO_IMAGE_FUNCTIONS    
//    fData->fractal->saveCurrentFractal(saveDirName,imageCounter % saveNumberOfImages);
//#endif    
    
//    imageCounter += 1;
    
    
    calcQueue.remove(fData);
    
    startTasks();
    
    return result;
}

void FractalThreadPoolController::startTasks()
{
    for(int i=0;i<numberOfFractals;++i)
    {        
        FractalData *fractal = fractals[i];
        
        if(find(calcQueue.begin(),calcQueue.end(),fractal) == calcQueue.end())
        {
            pool->runTask(ThreadFractalFunction,fractal);
            calcQueue.push_back(fractal);
        }        
    }    
}

