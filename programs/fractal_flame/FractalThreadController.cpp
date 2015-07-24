#include <iostream>

#include <small_utils.h>
#include "FractalThreadController.h"
#include "flame_fractal.h"

using namespace std;

#define FIRST_NUMBER_OF_ITERATIONS 1000000
#define DEFAULT_NUMBER_OF_ITERATIONS 5000000

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
            return;    
        }
        
        
        
        if(firstTime)
        {
            firstTime=false;
            fractalSetNumberOfIterations(FIRST_NUMBER_OF_ITERATIONS);
        }
        else
        {
            fractalSetNumberOfIterations(DEFAULT_NUMBER_OF_ITERATIONS);
        }
        
                        
        unsigned int startMillis=getMilliseconds();
        
        output = fractalRandom();
        
        cout <<"calculate time: "<<(getMilliseconds() - startMillis)<< endl<<flush;
        
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

