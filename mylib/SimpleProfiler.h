#pragma once

#include <string>

#include <small_utils.h>

class SimpleProfiler{
public:
    SimpleProfiler(const std::string &name):name(name)
    {
        startMillis=getMilliseconds();
    }
    
    ~SimpleProfiler()
    {
        unsigned int millisPassed = getMilliseconds() - startMillis;
        
        std::cout << name <<": "<<millisPassed<<std::endl;
    }
private:
    unsigned int startMillis;
    std::string name;
};

