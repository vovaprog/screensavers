#include <iostream>

#include <small_utils.h>
#include "FractalThreadController.h"
#include "FractalFlame.h"

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

        
                        
        unsigned int startMillis=getMilliseconds();
        
        result = pFractal->screensaver(&output);
        
        
        
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


FractalFlame::CalculateFractalResult FractalThreadController::getResultWithTimeout(unsigned int **ppOutput)
{    
    if(semResult.waitMilliseconds(100))
    {   
        *ppOutput=output;
        return result;
    }
    else
    {
        return FractalFlame::CalculateFractalResult::TIMEOUT;
    }
}


