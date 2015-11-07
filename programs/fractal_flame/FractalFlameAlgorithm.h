#pragma once

#include <vector>
#include <atomic>
#include <memory>

#include "Function.h"
#include "Point.h"
#include "RenderParameters.h"
#include "FlameParameters.h"

class FractalFlameAlgorithm{
public:
    enum class CalculateFractalResult { SUCCESS, BAD_PICTURE };    
    
    void setRenderParameters(std::shared_ptr<RenderParameters> renderParams);
    unsigned int* calculate(std::shared_ptr<FlameParameters> params);

    void setStopFlag(bool value);    
    bool getStopFlag();
    
    std::shared_ptr<FlameParameters> getFlameParameters();
    
private:        
    void convertScreenToMath(double &x, double &y);
    bool convertMathToScreen(double x, double y,int &xOut,int &yOut);
    void getInitialPoint(double &x, double &y);
    Function* getRandomFunction();
    int outputIndex(int screenX,int screenY);
    CalculateFractalResult plot(double mathX, double mathY, Function *pFun,unsigned int currentIteration);
    void findMinMaxOutput(unsigned int &minOutput,unsigned int &maxOutput);
    unsigned int histAnalysis(unsigned int minCounter,unsigned int maxCounter);
    CalculateFractalResult createOutput();
    void applyFunction(Function *pFun, double &x, double &y);
    void cleanBuffers();
    CalculateFractalResult calculateFractal();
    void resetVariables();
    
private:        
    //=====calculate parameters=====    
    std::shared_ptr<FlameParameters> fp;
    std::shared_ptr<RenderParameters> rp;    
    //=====calculate parameters=====


    //=====buffers=====
    std::unique_ptr<unsigned int[]> output;
    std::unique_ptr<Point[]> points;
    std::unique_ptr<unsigned int[]> hist;    
    //=====buffers=====


    //=====variables=====
    std::atomic<bool> stopFlag;
    int outputSize;
    unsigned int histSize=0;    
    
    unsigned int goodPointCounter=0;
    unsigned int badPointCounter=0;
    
    bool firstTimeInitFunctionsRandom=true;
    //=====variables=====    
};
