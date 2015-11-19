#include "FractalFlameAlgorithm.h"

#include <string>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;


void FractalFlameAlgorithm::setRenderParameters(shared_ptr<RenderParameters> renderParams)
{	
    rp=renderParams;
    
    outputSize=renderParams->pictureWidth * renderParams->pictureHeight;

    output=unique_ptr<unsigned int[]>(new unsigned int[outputSize]);
    points=unique_ptr<Point[]>(new Point[outputSize]);
    
    srand((unsigned int)time(NULL));    
    
    stopFlag.store(false);
}


inline void FractalFlameAlgorithm::convertScreenToMath(double &x, double &y)
{        
    x = fp->xLowerBound + (x / (rp->pictureWidth/(fp->xUpperBound - fp->xLowerBound)));
    y = fp->yLowerBound + (y / (rp->pictureHeight/(fp->yUpperBound - fp->yLowerBound)));
}


inline bool FractalFlameAlgorithm::convertMathToScreen(double x, double y, int &xOut, int &yOut)
{    
    xOut = (int)(((x - fp->xLowerBound) / (fp->xUpperBound - fp->xLowerBound)) * rp->pictureWidth);
    yOut = (int)(((y - fp->yLowerBound) / (fp->yUpperBound - fp->yLowerBound)) * rp->pictureHeight);    
    
    if(xOut<0 || yOut<0 || xOut>=rp->pictureWidth || yOut>=rp->pictureHeight)
    {
        return false;
    }
    else
    {
        return true;
    }    
}


void FractalFlameAlgorithm::getInitialPoint(double &x, double &y)
{
    x = rand() % rp->pictureWidth;
    y = rand() % rp->pictureHeight;
    
    convertScreenToMath(x,y);
}


Function* FractalFlameAlgorithm::getRandomFunction()
{
    int randomValue = rand() % fp->totalProbabilityWeight;
    
    for(auto& funIter : fp->functions)
    {
        if(randomValue<funIter->probabilityUpBorder)
        {
            return funIter.get();    
        }
    }
    
    return nullptr;
}


inline int FractalFlameAlgorithm::outputIndex(int screenX,int screenY)
{
    return screenY * rp->pictureWidth + screenX;    
}


FractalFlameAlgorithm::CalculateFractalResult FractalFlameAlgorithm::plot(double mathX, double mathY, Function *pFun,unsigned int currentIteration)
{
    int screenX,screenY;
    
    if(convertMathToScreen(mathX,mathY,screenX,screenY))
    {    
        int i = outputIndex(screenX,screenY);

        points[i].count += 1;    
        points[i].r = (points[i].r + pFun->r) / 2;
        points[i].g = (points[i].g + pFun->g) / 2;
        points[i].b = (points[i].b + pFun->b) / 2;
        
        goodPointCounter++;
    }
    else
    {
        badPointCounter++;
        
        if(badPointCounter>300000 && badPointCounter/8 > goodPointCounter)
        {
            cout <<"plot BAD PICTURE! good: "<<goodPointCounter<<"   bad: "<<badPointCounter<<endl;
            return FractalFlameAlgorithm::CalculateFractalResult::BAD_PICTURE;
        }
    }
    
    return FractalFlameAlgorithm::CalculateFractalResult::SUCCESS;
}


void FractalFlameAlgorithm::findMinMaxOutput(unsigned int &minOutput,unsigned int &maxOutput)
{    
    minOutput=points[0].count;
    maxOutput=points[0].count;    
    
    for(int i=1;i<outputSize;i++)
    {
        if(points[i].count<minOutput)
        {
            minOutput=points[i].count;    
        }
        else if(points[i].count>maxOutput)
        {
            maxOutput=points[i].count;
        }
    }    
}


unsigned int FractalFlameAlgorithm::histAnalysis(unsigned int minCounter,unsigned int maxCounter)
{        
    unsigned int counterRange=maxCounter-minCounter;
    
    int l = (int)log10(counterRange) - 1;
    
    if(l<1)
    {
        return 0;
    }

    unsigned int HIST_BINS = (unsigned int)pow(10, l);
        
    if(counterRange < HIST_BINS || HIST_BINS<2)
    {
        return 0;
    }
    
    if(histSize < HIST_BINS)
    {
    	hist=unique_ptr<unsigned int[]>(new unsigned int[HIST_BINS]);
    	histSize = HIST_BINS;
    }
    
    memset(hist.get(),0,HIST_BINS*sizeof(unsigned int));
    
    
    unsigned int binSize = counterRange / HIST_BINS;            
    
    for(int i=0;i<outputSize;i++)
    {
        unsigned int binIndex = (points[i].count - minCounter)/binSize;
        if(binIndex>=HIST_BINS)
        {            
            binIndex=HIST_BINS-1;
        }
        hist[binIndex] += 1;        
    }
    
        
    unsigned int pointSum=0;
    
    for(unsigned int i=0;i<HIST_BINS;i++)
    {
        pointSum += hist[i];
        
        if(((double)pointSum / (double)outputSize)>0.999)
        {
            return minCounter + (i+1) * binSize;
        }
    }

           
    return 0;
}


FractalFlameAlgorithm::CalculateFractalResult FractalFlameAlgorithm::createOutput()
{
    unsigned int maxCounter, minCounter;
    findMinMaxOutput(minCounter, maxCounter);
        
    double maxCounterDivAll = (double)maxCounter / (double)rp->numberOfIterations;
    
    if(maxCounter==0 || maxCounter<=minCounter+10 || maxCounterDivAll>=0.5)
    {
    	cout <<"bad picture!"<<endl;
    	memset(output.get(),0,outputSize * sizeof(unsigned int));
    	return FractalFlameAlgorithm::CalculateFractalResult::BAD_PICTURE;
    }
        
    unsigned int counterUpLimit = histAnalysis(minCounter, maxCounter);
    
    for(int i=0;i<outputSize;i++)
    {
        double v;
        
        if(points[i].count<counterUpLimit)
        {        
            v = ((double)points[i].count - (double)minCounter) / ((double)counterUpLimit-(double)minCounter);
            
            v = pow(v, fp->colorPower);
        }
        else
        {
            v = 1.0;   
        }
            
        points[i].r = (unsigned int)(points[i].r * v);
        points[i].g = (unsigned int)(points[i].g * v);
        points[i].b = (unsigned int)(points[i].b * v);
        
        output[i] = (0xff000000 | (points[i].r << 16) | (points[i].g <<8) | points[i].b);
    }
    
    return FractalFlameAlgorithm::CalculateFractalResult::SUCCESS;
}


void FractalFlameAlgorithm::applyFunction(Function *pFun, double &x, double &y)
{
    double xAccum=0, yAccum=0;
    
    
    x=pFun->preTransformX_CoefX * x + pFun->preTransformX_CoefY * y + pFun->preTransformX_CoefC;
    y=pFun->preTransformY_CoefX * x + pFun->preTransformY_CoefY * y + pFun->preTransformY_CoefC;
    
    
    for(auto& fun : pFun->variations)
    {            
        double xOut,yOut;
        
        fun.f(x , y, xOut, yOut);
        
        xAccum += xOut;
        yAccum += yOut;
    }
    
    
    x=pFun->postTransformX_CoefX * xAccum + pFun->postTransformX_CoefY * yAccum + pFun->postTransformX_CoefC;
    y=pFun->postTransformY_CoefX * xAccum + pFun->postTransformY_CoefY * yAccum + pFun->postTransformY_CoefC;    
}


void FractalFlameAlgorithm::cleanBuffers()
{
    for(int i=0;i<outputSize;i++)
    {
        points[i].r=0;
        points[i].g=0;
        points[i].b=0;
        points[i].count=0;
    }
}


FractalFlameAlgorithm::CalculateFractalResult FractalFlameAlgorithm::calculateFractal()
{    
    double x, y;
        
    cleanBuffers();    
    
    getInitialPoint(x,y);
    
    for(unsigned int i=0;i<rp->numberOfIterations;i++)
    {        
        Function* pFun=getRandomFunction();
                
        applyFunction(pFun, x, y);        
        
        if(i>20)
        {
            if(plot(x,y,pFun,i)==FractalFlameAlgorithm::CalculateFractalResult::BAD_PICTURE)
            {
                memset(output.get(),0,outputSize * sizeof(unsigned int));
                return FractalFlameAlgorithm::CalculateFractalResult::BAD_PICTURE;   
            }
        }        
        
        if((i & 0xff)==0)
        {
            if(stopFlag.load())
            {
                cout <<"stop flag. this: "<<(unsigned long)this<< " iterations: "<<i<<endl;
                break;
            }
        }
    }
    
    return createOutput();
}


void FractalFlameAlgorithm::resetVariables()
{
    goodPointCounter=0;
    badPointCounter=0;
}


FractalFlameAlgorithm::CalculateFractalResult FractalFlameAlgorithm::calculate(std::shared_ptr<FlameParameters> params, unsigned int **ppOutput)
{
    resetVariables();
    
    fp = params;
    fp->setViewBoundsForPictureSize(rp->pictureWidth,rp->pictureHeight);
    
    FractalFlameAlgorithm::CalculateFractalResult result = calculateFractal();
    lastCalculateResult = result;
    
    if(result==FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
    {
        *ppOutput = output.get();        
    }
    else
    {
        ppOutput=nullptr;
    }
    
    return result;
}


void FractalFlameAlgorithm::setStopFlag(bool value)
{
    stopFlag.store(value);    
}

bool FractalFlameAlgorithm::getStopFlag()
{
    return stopFlag.load();
}

std::shared_ptr<FlameParameters> FractalFlameAlgorithm::getFlameParameters()
{
    return fp;    
}

std::shared_ptr<RenderParameters> FractalFlameAlgorithm::getRenderParameters()
{
    return rp;    
}

unsigned int* FractalFlameAlgorithm::getOutput()
{
    if(lastCalculateResult==CalculateFractalResult::SUCCESS)
    {
        return output.get();
    }
    else
    {
        return nullptr;
    }
}

FractalFlameAlgorithm::CalculateFractalResult FractalFlameAlgorithm::getLastResult()
{
    return lastCalculateResult;
}

