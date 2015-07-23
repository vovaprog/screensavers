#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore{
private:    
    std::mutex mtx;
    std::condition_variable cv;
    int counter;
public:
    Semaphore()
    {
        counter = 0;
    }
    
    void increment()
    {
        std::unique_lock<std::mutex> lock(mtx);
        
        counter++;
        
        cv.notify_one();        
    }
    
    void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        
        while(counter<=0)
        {
            cv.wait(lock);
        }
        
        counter--;
    }    
};

