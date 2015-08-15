#pragma once

#include <list>

#include <ThreadPool.h>

#include "FractalFlame.h"
#include "FractalPoolData.h"

class FractalThreadPoolController{    
public:  
    
    FractalThreadPoolController(int pictureWidth,int pictureHeight,int numberOfThreads);
    ~FractalThreadPoolController();

    FractalFlame::CalculateFractalResult getResult(unsigned int *output);
    void startTasks();
    
private:    
    
    ThreadPool *pool;
    
    int numberOfFractals;
    FractalPoolData** fractals;
    
    int outputSize;
    
    std::list<FractalPoolData*> calcQueue;
};
