#include <iostream>

#include <small_utils.h>
#include "FractalThreadController.h"
#include "flame_fractal.h"

using namespace std;

#define FIRST_NUMBER_OF_ITERATIONS 1000000

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
    bool firstTime=true;
    
    while(!threadStopFlag.load())
    {       
        semStartWork.wait();
    
        if(threadStopFlag.load())
        {
            semResult.increment();
            return;    
        }
        
        
        
        if(firstTime)
        {
            firstTime=false;
            numberOfIterations = FIRST_NUMBER_OF_ITERATIONS;
            fractalSetNumberOfIterations(FIRST_NUMBER_OF_ITERATIONS);
        }
        
                        
        unsigned int startMillis=getMilliseconds();
        
        output = fractalRandom();
        
        
        
        if(threadStopFlag.load())
        {
            semResult.increment();
            return;    
        }
                       
        
        
        unsigned int millisPassed = getMilliseconds() - startMillis; 
        
        cout <<"!!!calculate time: "<<millisPassed<< endl<<flush;
        cout <<"!!!number of iterations: "<<numberOfIterations<< endl<<flush;
        
        unsigned int period = periodMilliseconds.load();
                
        double d = (double)millisPassed / ((double)period * 0.4);
                
        if(d>1.0+0.3 || d<1.0-0.3)
        {
            numberOfIterations /= d;
            fractalSetNumberOfIterations(numberOfIterations);
            cout << "!!!set number of iterations: "<< numberOfIterations <<endl<<flush;
        }
        
        
        
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

