#pragma once

#include <thread>
#include <atomic>

#include "Semaphore.h"

using namespace std;

void fractalThreadEntry();

class FractalThreadController{
private:
    thread *t=nullptr;
        
    Semaphore semResult, semStartWork;
    
    unsigned int *output=nullptr;

    atomic<bool> threadStopFlag;
    
public:  
    FractalThreadController();
    ~FractalThreadController();
    
    void beginCalculateFractal();
    
    unsigned int* getResult();
    
    void fractalThreadEntry();    
};
