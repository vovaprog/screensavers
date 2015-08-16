#pragma once

#include <list>

#include <ThreadPool.h>

#include "FractalFlame.h"

class FractalThreadPoolController{    
public:  
    
    FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads);
    ~FractalThreadPoolController();

    void setStopFlag();
    FractalFlame::CalculateFractalResult getResult(unsigned int *output);
    
    void startTasks();
    
private:    

    class FractalPoolData{
    public:    
        FractalFlame *fractal;
        FractalFlame::CalculateFractalResult result;
        unsigned int *output=nullptr;
        
        FractalPoolData()
        {
            fractal=new FractalFlame();
        }
        
        ~FractalPoolData()
        {
            if(fractal!=nullptr)
            {
                delete fractal;
            }
        }
    };    
    
    typedef FractalThreadPoolController::FractalPoolData FractalData;
    
    const char *saveDirName="./fractals/screensaver";
    const int saveNumberOfImages=10;
    int imageCounter=0;
    
    ThreadPool *pool;
    
    int numberOfFractals;
    FractalPoolData** fractals;
    
    int outputSize;
    
    std::list<FractalPoolData*> calcQueue;
    
    friend void* ThreadFractalFunction(void *dataInput);
};
