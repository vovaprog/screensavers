#pragma once

#include <thread>
#include <atomic>

#include "Semaphore.h"
#include "FractalFlame.h"

using namespace std;

class FractalThreadController{    
public:  
    
    FractalThreadController(FractalFlame *pFractal);
    ~FractalThreadController();
    
    void beginCalculateFractal();
    
    FractalFlame::CalculateFractalResult getResultWithTimeout(unsigned int **ppOutput);    

private:
    
    void fractalThreadEntry();   

private:
    
    thread *t=nullptr;
        
    Semaphore semResult, semStartWork;
    
    unsigned int *output=nullptr;
    FractalFlame::CalculateFractalResult result;

    atomic<bool> threadStopFlag;    
    
    unsigned int numberOfIterations;
    
    FractalFlame *pFractal;    
};
