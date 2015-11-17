#pragma once

#include <list>
#include <memory>
#include <vector>

#include <ThreadPool.h>

#include "FractalFlame.h"

class FractalThreadPoolController{    
public:  
    
    FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads=0);
    ~FractalThreadPoolController();

    void setStopFlag();
    FractalFlameAlgorithm::CalculateFractalResult getResult(unsigned int *output);
    
    void startTasks();
    
private:    

    class FractalPoolData{
    public:    
        //FractalFlame *fractal;
        FractalFlame fractal;
        FractalFlameAlgorithm::CalculateFractalResult result;
        unsigned int *output=nullptr;
        
        /*FractalPoolData()
        {
            fractal=new FractalFlame();
        }
        
        ~FractalPoolData()
        {
            if(fractal!=nullptr)
            {
                delete fractal;
            }
        }*/
    };    
    
    //typedef FractalThreadPoolController::FractalPoolData FractalData;
    
    const char *saveDirName="./fractals/screensaver";
    const int saveNumberOfImages=10;
    int imageCounter=0;
    
    //not made auto ptr because must be destroyed before fractals
    ThreadPool *pool;    
    
    int numberOfFractals;
    //FractalPoolData** fractals;
    std::vector<std::unique_ptr<FractalPoolData>> fractals;
    
    int outputSize;
    
    std::list<FractalPoolData*> calcQueue;
    
    friend void* ThreadFractalFunction(void *dataInput);
};
