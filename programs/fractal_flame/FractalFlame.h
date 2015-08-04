#pragma once

#include <vector>
#include <atomic>
#include <memory>

#include "Function.h"
#include "Point.h"

enum class CalculateFractalResult { SUCCESS, BAD_PICTURE, TIMEOUT };

class FractalFlame{
private:    

    //=====calculate parameters=====
    int pictureWidth,pictureHeight;
    std::vector<std::unique_ptr<Function>> functions;
    unsigned int numberOfIterations=30000000;
    double colorPowerArgument = 0.5; //0.45;
    //=====calculate parameters=====


    //=====buffers=====
    std::unique_ptr<unsigned int[]> output;
    std::unique_ptr<unsigned int[]> saveOutput;
    std::unique_ptr<Point[]> points;
    std::unique_ptr<unsigned int[]> hist;    
    //=====buffers=====


    //=====variables=====
    std::atomic<bool> stopFlag;
    int outputSize;
    unsigned int histSize=0;
    int totalProbabilityWeight;
    
    unsigned int goodPointCounter=0;
    unsigned int badPointCounter=0;
    //=====variables=====

public:    
    void fractalInit(int argPictureWidth, int argPictureHeight);
    
    void fractalPreview(int numberOfPreviews);
    
    void fractalRender(const char *fileName);
    
    CalculateFractalResult fractalScreensaver(unsigned int **ppOutput);
    
    void fractalSetNumberOfIterations(int argNumberOfIterations);
    
    void fractalSetStopFlag();
    
    void saveCurrentFractal(const char *dirName,int index);
    
private:    
    //void destroyFunctions();
    void resetVariables();
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
    void saveImage(const char *fileName,const char *fileType);
};
