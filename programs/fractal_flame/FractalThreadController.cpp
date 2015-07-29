#include <iostream>

#include <small_utils.h>
#include "FractalThreadController.h"
#include "flame_fractal.h"

using namespace std;


FractalThreadController::FractalThreadController()
{
    threadStopFlag.store(false);    
}

FractalThreadController::~FractalThreadController()
{
    if(t!=nullptr)
    {        
        threadStopFlag.store(true);
        fractalSetStopFlag();
        semStartWork.increment();
        t->join();
        t=nullptr;
    }
}

void FractalThreadController::fractalThreadEntry()
{    
    while(!threadStopFlag.load())
    {       
        semStartWork.wait();
    
        if(threadStopFlag.load())
        {
            semResult.increment();
            return;    
        }

        
                        
        unsigned int startMillis=getMilliseconds();
        
        output = fractalScreensaver();
        
        
        
        if(threadStopFlag.load())
        {
            semResult.increment();
            return;    
        }
                       
        
        
        unsigned int millisPassed = getMilliseconds() - startMillis; 
        
        cout <<"calculate time: "<<millisPassed<<endl;
                       
        
        semResult.increment();       
    }        
}    

void FractalThreadController::beginCalculateFractal()
{
    if(t==nullptr)
    {
        t=new thread(&FractalThreadController::fractalThreadEntry,this);
    }
            
    semStartWork.increment();
}

unsigned int* FractalThreadController::getResult()
{    
    semResult.wait();
    return output;
}

unsigned int* FractalThreadController::getResultWithTimeout()
{    
    if(semResult.waitMilliseconds(100))
    {    
        return output;
    }
    else
    {
        return 0;
    }
}


void FractalThreadController::setPeriodMilliseconds(unsigned int milliseconds)
{
    periodMilliseconds.store(milliseconds);    
}

