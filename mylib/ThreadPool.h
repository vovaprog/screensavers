#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

typedef void* (*ThreadFunction)(void *dataInput);

class ThreadPoolTask{
public:    
    ThreadFunction function;
    void *inputData;
    void *outputData;
    
    ThreadPoolTask():inputData(nullptr),outputData(nullptr),function(nullptr){}
    
    ThreadPoolTask(ThreadFunction function,void *inputData):function(function),inputData(inputData),outputData(nullptr){}
};

class ThreadPool{
public:
    ThreadPool(int numberOfThreads);
    
    ~ThreadPool();
    
    void runTask(ThreadFunction fun, void *userData);
        
    void* getResult();
    
private:    
    void threadEntry();

    std::queue<ThreadPoolTask> tasks;
    std::queue<void*> results;
    
    std::atomic_bool stopFlag;

    std::mutex mutexTasks;
    std::condition_variable cvTasks;

    std::mutex mutexResults;
    std::condition_variable cvResults;    
    
    int numberOfThreads;
    std::thread **threads;
    
    ThreadFunction function;
};

#endif
