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
    
    FractalFlameAlgorithm::CalculateFractalResult getResultWithTimeout(unsigned int **ppOutput);    

private:
    
    void fractalThreadEntry();   

private:
    
    thread *t=nullptr;
        
    Semaphore semResult, semStartWork;
    
    unsigned int *output=nullptr;
    FractalFlameAlgorithm::CalculateFractalResult result;

    atomic<bool> threadStopFlag;    
    
    unsigned int numberOfIterations;
    
    FractalFlame *pFractal;    
};
