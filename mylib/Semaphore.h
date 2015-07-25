#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>

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
    
    bool waitMilliseconds(int millis)
    {
        std::unique_lock<std::mutex> lock(mtx);
        
        while(counter<=0)
        {
            if(cv.wait_for(lock, std::chrono::milliseconds(millis))== std::cv_status::timeout)
            {
                return false;
            }
        }
        
        counter--;
        return true;
    }     
};

