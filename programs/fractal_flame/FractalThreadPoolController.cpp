#include "FractalThreadPoolController.h"

#include <string.h>
#include <algorithm>

#include <filesystem_utils.h>

using namespace std;


FractalThreadPoolController::FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads /* =0 */)
{
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
    
    numberOfFractals=numberOfThreads;
    outputSize = pictureWidth * pictureHeight;
    
    //fractals=new FractalPoolData*[numberOfThreads];
    
    for(int i=0;i<numberOfThreads;++i)
    {
        //fractals[i]=new FractalData();
        fractals.push_back(unique_ptr<FractalPoolData>(new FractalPoolData()));        
        fractals[i]->fractal.screensaverInit(pictureWidth,pictureHeight);
    }
    
    //pool=unique_ptr<ThreadPool>(new ThreadPool(numberOfThreads));
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
    for(int i=0;i<numberOfFractals;++i)
    {
        fractals[i]->fractal.setStopFlag();
    }    
    
    delete pool;
    
    /*for(int i=0;i<numberOfFractals;++i)
    {
        delete fractals[i];
    }
    
    delete[] fractals;*/
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

    
//#ifndef NO_IMAGE_FUNCTIONS    
//    fData->fractal->saveCurrentFractal(saveDirName,imageCounter % saveNumberOfImages);
//#endif    
    
    imageCounter += 1;
    
    
    calcQueue.remove(fData);
    
    startTasks();
    
    return result;
}

void FractalThreadPoolController::startTasks()
{
    //for(int i=0;i<fractals.size();++i)
    //{        
        //FractalPoolData *fractal = fractals[i].get();
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

