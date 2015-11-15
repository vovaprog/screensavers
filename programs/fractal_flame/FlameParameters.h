#pragma once

#include <vector>
#include <memory>

#include "Function.h"

class FlameParameters{
public:          
    void load(const char *fileName);
    void load_old(const char *fileName);
    
    void save(const char *fileName);
    
    void initRandom();
    
    void setViewBoundsForPictureSize(int pictureWidth, int pictureHeight);
    
    void prepare();
    
private:
    void initFunctionProbabilities();
    double getRandom01();
    double getRandomValue(double start, double end);
    void resetVariables();
    void prepareLocale();
    
public:    
    double xLowerBound=-1.0, xUpperBound=1.0;
    double yLowerBound=-1.0, yUpperBound=1.0;
    
    double colorPower = 0.5;
    std::vector<std::unique_ptr<Function>> functions;
            
    enum Axis { x=1, y=2, none=0};
    Axis setBoundsAxis = Axis::x;
    double setBoundsRatio=1.0, setBoundsCenter=0.0;
    
    int totalProbabilityWeight=0;
    
    static const int MIN_NUMBER_OF_FUNCTIONS = 2;
    static const int MAX_NUMBER_OF_FUNCTIONS = 3;
    static const int MIN_NUMBER_OF_VARIATIONS = 1;
    static const int MAX_NUMBER_OF_VARIATIONS = 3;
    
    static bool initRandomGenerator;
};

