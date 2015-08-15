#include "FractalThreadPoolController.h"

#include <string.h>
#include <algorithm>

#include <iostream>

using std::find;

FractalThreadPoolController::FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads)
{
    numberOfFractals=numberOfThreads;
    outputSize = pictureWidth * pictureHeight;
    
    fractals=new FractalPoolData*[numberOfThreads];
    
    for(int i=0;i<numberOfThreads;++i)
    {
        fractals[i]=new FractalPoolData();
        fractals[i]->fractal->init(pictureWidth,pictureHeight);
    }
    
    pool=new ThreadPool(numberOfThreads);
}

FractalThreadPoolController::~FractalThreadPoolController()
{
    delete pool;
    
    for(int i=0;i<numberOfFractals;++i)
    {
        delete fractals[i];
    }
    
    delete[] fractals;
}

void* ThreadFractalFunction(void *dataInput)
{
    FractalPoolData *fData=(FractalPoolData*)dataInput;
    
    fData->result = fData->fractal->screensaver(&fData->output);
    
    return fData;
}

FractalFlame::CalculateFractalResult FractalThreadPoolController::getResult(unsigned int *output)
{
    FractalPoolData *fData = calcQueue.front();
    fData->fractal->setStopFlag();
    
    fData = (FractalPoolData*)pool->getResult();
    
    std::cout <<"getting result from: "<<(unsigned long long int)fData->fractal<<std::endl;
    
    FractalFlame::CalculateFractalResult result = fData->result;
    
    if(fData->result == FractalFlame::CalculateFractalResult::SUCCESS)
    {
        memcpy(output,fData->output,sizeof(unsigned int) * outputSize);
    }    
    
    calcQueue.remove(fData);
    
    startTasks();
    
    return result;
}

void FractalThreadPoolController::startTasks()
{
    for(int i=0;i<numberOfFractals;++i)
    {        
        FractalPoolData *fractal = fractals[i];
        
        if(find(calcQueue.begin(),calcQueue.end(),fractal) == calcQueue.end())
        {
            pool->runTask(ThreadFractalFunction,fractal);
            calcQueue.push_back(fractal);
        }        
    }    
}

