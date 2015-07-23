#pragma once

#include <thread>
#include <atomic>

#include "Semaphore.h"

using namespace std;

void fractalThreadEntry();

class FractalThreadController{
private:
    thread *t=nullptr;
    
    //atomic<bool> stopThreadFlag;
    
public:
    FractalThreadController()
    {
        //stopThreadFlag.store(false);   
        //output=0;
    }
    
    void beginCalculateFractal();
    
    unsigned int* getResult();
    
/*private:
    
    void threadEntry()
    {
        cout <<"threadEntry"<<endl;
        
        while(!stopThreadFlag.load() true)
        {
            cout <<"threadEntry while"<<endl;
            
            semStartWork->wait();
            
            cout <<"te 2"<<endl<<flush;
            
            output = fractalRandom();
            cout <<"output 2"
            cout <<"te 3"<<endl<<flush;
            
            semResult->increment();
            
            cout <<"te 4"<<endl<<flush;
        }        
    }*/    
};
