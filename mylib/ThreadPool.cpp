#include <ThreadPool.h>

using std::thread;
using std::unique_lock;
using std::mutex;

ThreadPool::ThreadPool(int numberOfThreads):numberOfThreads(numberOfThreads)
{
    stopFlag.store(false);
    
    
    threads=new thread*[numberOfThreads];
    
    for(int i=0;i<numberOfThreads;++i)
    {
        threads[i]=new thread(&ThreadPool::threadEntry,this);
    }    
}


ThreadPool::~ThreadPool()
{
    if(threads!=nullptr)
    {    
        {
            unique_lock<mutex> lock(mutexTasks);
        
            stopFlag.store(true);
        }
        
        cvTasks.notify_all();
        
        
        for(int i=0;i<numberOfThreads;i++)
        {
            threads[i]->join();
            delete threads[i];
        }
            
        delete[] threads;
    }
}

void ThreadPool::runTask(ThreadFunction fun, void *userData)
{
    unique_lock<mutex> lock(mutexTasks);
    
    tasks.push(ThreadPoolTask(fun,userData));

    cvTasks.notify_one();
}

void* ThreadPool::getResult()
{
    unique_lock<mutex> lock(mutexResults);
    
    while(results.empty())
    {
        cvResults.wait(lock);
    }
    
    void *result = results.front();
    results.pop();
    
    return result;
}

void ThreadPool::threadEntry()
{
    ThreadPoolTask task;
    
    while(!stopFlag.load())
    {    
        {
            unique_lock<mutex> lock(mutexTasks);
            
            while(tasks.empty() && !stopFlag.load())
            {
                cvTasks.wait(lock);                
            }
            
            if(!tasks.empty() && !stopFlag.load())
            {
                task = tasks.front();
                tasks.pop();
            }                    
        }

        if(stopFlag.load())
        {
            return;                
        }                    
                    
        void *result = task.function(task.inputData);

        {
            std::unique_lock<std::mutex> lock(mutexResults);
        
            results.push(result);
 
            cvResults.notify_one();
        }            
    }
}
