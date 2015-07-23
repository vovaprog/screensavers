#pragma once

#include <thread>

using namespace std;

class FractalThreadController{
private:
    thread *t=nullptr;

public:
    void beginCalculateFractal()
    {
        if(t==nullptr)
        {
            t=new thread(&FractalThreadController::threadEntry,*this);
        }
        
    }
    
    unsigned int* getResult();
    
private:
    
    void threadEntry()
    {
        
        
    }
    
};
