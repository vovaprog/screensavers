#include <iostream>

#include <small_utils.h>
#include "FractalThreadController.h"

using namespace std;


FractalThreadController::FractalThreadController(FractalFlame *pFractal):pFractal(pFractal)
{
    threadStopFlag.store(false);    
}

FractalThreadController::~FractalThreadController()
{
    if(t!=nullptr)
    {        
        threadStopFlag.store(true);
        pFractal->setStopFlag();
        semStartWork.increment();
        t->join();
        delete t;
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

        
        result = pFractal->screensaver(&output);
        
        if(threadStopFlag.load())
        {
            semResult.increment();
            return;    
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


FractalFlameAlgorithm::CalculateFractalResult FractalThreadController::getResultWithTimeout(unsigned int **ppOutput)
{    
    semResult.wait(); 

    *ppOutput=output;
    return result;
}


