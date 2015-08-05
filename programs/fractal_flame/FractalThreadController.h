#pragma once

#include <thread>
#include <atomic>

#include "Semaphore.h"
#include "FractalFlame.h"

using namespace std;

void fractalThreadEntry();

class FractalThreadController{
private:
    thread *t=nullptr;
        
    Semaphore semResult, semStartWork;
    
    unsigned int *output=nullptr;
    FractalFlame::CalculateFractalResult result;

    atomic<bool> threadStopFlag;
    atomic<unsigned int> periodMilliseconds;
    
    unsigned int numberOfIterations;
    
    FractalFlame *pFractal;
    
public:  
    FractalThreadController(FractalFlame *pFractal);
    ~FractalThreadController();
    
    void beginCalculateFractal();
    
    FractalFlame::CalculateFractalResult getResultWithTimeout(unsigned int **ppOutput);    
    
    void fractalThreadEntry();   
    
    void setPeriodMilliseconds(unsigned int milliseconds);
};
