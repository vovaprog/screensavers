#pragma once

#include <thread>

#include "Semaphore.h"

using namespace std;

void fractalThreadEntry();

class FractalThreadController{
private:
    thread *t=nullptr;
        
    Semaphore semResult, semStartWork;
    
    unsigned int *output=nullptr;

    
public:    
    void beginCalculateFractal();
    
    unsigned int* getResult();
    
    void fractalThreadEntry();    
};
