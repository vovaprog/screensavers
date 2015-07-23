#pragma once

#include <condition_variable>
#include <mutex>

class Semaphore{
private:    
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<int> counter;
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
        
        while(counter.load()<=0)
        {
            cv.wait(lock);
        }
        
        counter--;
    }    
};

