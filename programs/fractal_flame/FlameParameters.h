#pragma once

#include <vector>
#include <memory>

#include "Function.h"

class FlameParameters{
public:    
    void load(const char *fileName, int &totalProbabilityWeight);    
    void save(const char *fileName);
    
    void initRandom(int &totalProbabilityWeight);
    
private:
    void initFunctionProbabilities(int &totalProbabilityWeight);
    double getRandom01();
    double getRandomValue(double start, double end);
    
    void setXViewBoundsByRatio(double ratio, double center);
    void setYViewBoundsByRatio(double ratio, double center);    
    
    double xLowerBound=-1.0, xUpperBound=1.0;
    double yLowerBound=-1.0, yUpperBound=1.0;
    double colorPower = 0.5;
    std::vector<std::unique_ptr<Function>> functions;
    
    const int MIN_NUMBER_OF_FUNCTIONS = 2;
    const int MAX_NUMBER_OF_FUNCTIONS = 3;
    const int MIN_NUMBER_OF_VARIATIONS = 1;
    const int MAX_NUMBER_OF_VARIATIONS = 3;        
};

