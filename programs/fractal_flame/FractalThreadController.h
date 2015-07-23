#pragma once

#include <thread>

#include "Semaphore.h"

using namespace std;

class FractalThreadController{
private:
    thread *t=nullptr;
    Semaphore semResult, semStartWork;
    atomic<bool> stopThreadFlag=false;
    unsigned int *output;
public:
    void beginCalculateFractal()
    {
        if(t==nullptr)
        {
            t=new thread(&FractalThreadController::threadEntry,*this);
        }
        
        semStartWork.increment();
    }
    
    unsigned int* getResult()
    {
        semResult.wait();
        
        return output;
    }
    
private:
    
    void threadEntry()
    {
        while(!stopThreadFlag.load())
        {
            semStartWork.wait();
            
            output = fractalRandom();
            
            semResult.increment();
        }        
    }    
};
