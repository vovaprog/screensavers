#include <iostream>

#include "FractalThreadController.h"
#include "flame_fractal.h"

using namespace std;

Semaphore *semResult, *semStartWork;
static unsigned int *output;

void fractalThreadEntry()
{
    cout <<"threadEntry"<<endl;
    
    while(/*!stopThreadFlag.load()*/ true)
    {
        cout <<"threadEntry while"<<endl;
        
        semStartWork->wait();
        
        cout <<"te 2"<<endl<<flush;
        
        output = fractalRandom();
        //cout <<"output 2"<<output
        cout <<"te 3"<<endl<<flush;
        
        semResult->increment();
        
        cout <<"te 4"<<endl<<flush;
    }        
}    

void FractalThreadController::beginCalculateFractal()
{
    if(t==nullptr)
    {
        semResult=new Semaphore();
        semStartWork=new Semaphore();
        t=new thread(fractalThreadEntry);
    }
            
    semStartWork->increment();
}

unsigned int* FractalThreadController::getResult()
{
    cout <<"get result 1"<<endl<<flush;
    
    semResult->wait();
    
    cout <<"get result 2"<<endl<<flush;
    
    return output;
}

