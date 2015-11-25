#include "FractalThreadPoolController.h"

#include <string.h>
#include <algorithm>
#include <iostream>

#include <filesystem_utils.h>

using namespace std;


FractalThreadPoolController::FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads /* =0 */)
{
#ifdef SCREENSAVER_ONE_THREAD    
    
    numberOfThreads = 1;

#else

    if(numberOfThreads<=0)
    {
        numberOfThreads=ThreadPool::getNumberOfProcessors();
        
        if(numberOfThreads>1)
        {
            numberOfThreads -= 1;
        }
        else if(numberOfThreads <= 0)
        {
            numberOfThreads = 1;
        }
    }

#endif    
    
    outputSize = pictureWidth * pictureHeight;
    
    for(int i=0;i<numberOfThreads;++i)
    {
        fractals.push_back(unique_ptr<FractalPoolData>(new FractalPoolData()));        
        fractals[i]->fractal.screensaverInit(pictureWidth,pictureHeight);
    }
    
    pool=new ThreadPool(numberOfThreads);
    
#ifndef NO_IMAGE_FUNCTIONS        
    if(directoryExists(saveDirName))
    {	
        deleteDirectory(saveDirName);						
    }   
    createDirectory(saveDirName);
#endif    
}

FractalThreadPoolController::~FractalThreadPoolController()
{
    for(auto& fractal_ptr : fractals)
    {
        fractal_ptr->fractal.setStopFlag();
    }
        
    delete pool;
}

void* ThreadFractalFunction(void *dataInput)
{
    FractalThreadPoolController::FractalPoolData *fData=(FractalThreadPoolController::FractalPoolData*)dataInput;
    
    fData->result = fData->fractal.screensaver(&fData->output);
    
    return fData;
}

void FractalThreadPoolController::setStopFlag()
{
    FractalPoolData *fData = calcQueue.front();
    fData->fractal.setStopFlag();    
}

FractalFlameAlgorithm::CalculateFractalResult FractalThreadPoolController::getResult(unsigned int *output)
{    
    FractalPoolData *fData = (FractalPoolData*)pool->getResult();
    
    FractalFlameAlgorithm::CalculateFractalResult result = fData->result;
        
    if(fData->result == FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
    {
        memcpy(output,fData->output,sizeof(unsigned int) * outputSize);
    }    


#ifndef NO_IMAGE_FUNCTIONS    
    fData->fractal.screensaverSaveCurrentFractal((string(saveDirName)+"/"+to_string(imageCounter % saveNumberOfImages)).c_str());
        
    imageCounter += 1;
#endif    
    

    calcQueue.remove(fData);
    
    startTasks();
    
    return result;
}

void FractalThreadPoolController::startTasks()
{
    for(auto& fractal_ptr : fractals)
    {
        FractalPoolData *fractal = fractal_ptr.get();
        
        if(find(calcQueue.begin(),calcQueue.end(),fractal) == calcQueue.end())
        {
            pool->runTask(ThreadFractalFunction,fractal);
            calcQueue.push_back(fractal);
        }        
    }    
}

