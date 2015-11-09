#pragma once

#include <vector>
#include <memory>

#include "Function.h"

class FlameParameters{
public:    
    void load(const char *fileName);    
    void save(const char *fileName);
    
    void initRandom();
    
    void setViewBoundsForPictureSize(int pictureWidth, int pictureHeight);
    
    void prepare();
    
private:
    void initFunctionProbabilities();
    double getRandom01();
    double getRandomValue(double start, double end);
    
public:    
    double xLowerBound=-1.0, xUpperBound=1.0;
    double yLowerBound=-1.0, yUpperBound=1.0;
    double viewBoundsRatio=1.0, viewBoundsCenter=0.0;
    double colorPower = 0.5;
    std::vector<std::unique_ptr<Function>> functions;
    
    bool setViewBoundsByX=false, setViewBoundsByY=false;    
    int totalProbabilityWeight;
    
    static const int MIN_NUMBER_OF_FUNCTIONS = 2;
    static const int MAX_NUMBER_OF_FUNCTIONS = 3;
    static const int MIN_NUMBER_OF_VARIATIONS = 1;
    static const int MAX_NUMBER_OF_VARIATIONS = 3;        
};

