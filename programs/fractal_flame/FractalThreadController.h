#pragma once

#include <thread>
#include <atomic>

#include "Semaphore.h"
#include "flame_fractal.h"

using namespace std;

void fractalThreadEntry();

class FractalThreadController{
private:
    thread *t=nullptr;
        
    Semaphore semResult, semStartWork;
    
    unsigned int *output=nullptr;
    CalculateFractalResult result;

    atomic<bool> threadStopFlag;
    atomic<unsigned int> periodMilliseconds;
    
    unsigned int numberOfIterations;
    
public:  
    FractalThreadController();
    ~FractalThreadController();
    
    void beginCalculateFractal();
    
    //unsigned int* getResult();
    CalculateFractalResult getResultWithTimeout(unsigned int **ppOutput);    
    
    void fractalThreadEntry();   
    
    void setPeriodMilliseconds(unsigned int milliseconds);
};
