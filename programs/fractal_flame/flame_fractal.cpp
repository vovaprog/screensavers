#include <vector>
#include <climits>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "flame_fractal.h"

#include "Function.h"
#include "Point.h"

static unsigned int *output;
static unsigned int *intermediateOutput;
static int pictureWidth,pictureHeight;
static int outputSize;

static vector<Function*> functions;

static int totalProbabilityWeight;

static int numberOfIterations=10000000;

static Point *points;

void initFunctions(vector<Function*> &functions, int &totalProbabilityWeight);

/*static void destroyFunctions()
{
    for(auto funIter : functions)
    {
        delete (*funIter);
    }
    
    functions.clear();
}*/

void fractalInit(int argPictureWidth, int argPictureHeight)
{
    pictureWidth=argPictureWidth;
    pictureHeight=argPictureHeight;    
    outputSize=pictureWidth*pictureHeight;
    
    output=new unsigned int[outputSize];
    intermediateOutput=new unsigned int[outputSize];
    
    srand(time(NULL));
    
    initFunctions(functions,totalProbabilityWeight);
    
    points=new Point[outputSize];
}

static void convertScreenToMath(double &x, double &y)
{
    x = -1.0 + (x / (pictureWidth/2.0));
    y = -1.0 + (y / (pictureHeight/2.0));
}

static void convertMathToScreen(double x, double y,int &xOut,int &yOut)
{
    xOut = (int)(((x+1.0)/2.0) * pictureWidth);
    yOut = (int)(((y+1.0)/2.0) * pictureHeight);
    
    if(xOut>=pictureWidth) xOut=pictureWidth-1;
    else if(xOut<0) xOut=0;
    
    if(yOut>=pictureHeight) yOut=pictureHeight-1;
    else if(yOut<0) yOut=0;
}

static void getInitialPoint(double &x, double &y)
{
    x = rand() % pictureWidth;
    y = rand() % pictureHeight;
    
    convertScreenToMath(x,y);
}

static Function* getRandomFunction()
{
    int randomValue = rand() % totalProbabilityWeight;
    
    for(auto funIter : functions)
    {
        if(randomValue<funIter->probabilityUpBorder)
        {
            return funIter;    
        }
    }
    
    return nullptr;
}

static inline int outputIndex(int screenX,int screenY)
{
    return screenY * pictureWidth + screenX;    
}

static void plot(double mathX, double mathY, Function *pFun)
{
    int screenX,screenY;
    
    convertMathToScreen(mathX,mathY,screenX,screenY);
    
    int i = outputIndex(screenX,screenY);
    
    points[i].count += 1;    
    points[i].r = (points[i].r + pFun->r) / 2;
    points[i].g = (points[i].g + pFun->g) / 2;
    points[i].b = (points[i].b + pFun->b) / 2;
}

static void findMinMaxOutput(unsigned int &minOutput,unsigned int &maxOutput)
{    
    minOutput=points[0].count;
    maxOutput=points[0].count;
    
    int maxIndex=-1;
    
    for(int i=1;i<outputSize;i++)
    {
        if(points[i].count<minOutput)
        {
            minOutput=points[i].count;    
        }
        else if(points[i].count>maxOutput)
        {
            maxOutput=points[i].count;
            maxIndex=i;
        }
    }    
    
    cout <<"max index: "<<maxIndex % pictureWidth<<","<<maxIndex/pictureWidth<<endl;
}

static void createOutput()
{
    unsigned int maxCounter, minCounter;
    findMinMaxOutput(minCounter, maxCounter);
    
    cout << "max count: " << maxCounter <<endl;
    
    double counterRange=maxCounter-minCounter;
    double counterRangeLimited=counterRange * 0.0005;
    
    for(int i=0;i<outputSize;i++)
    {
        double v;
        
        if(points[i].count<minCounter+counterRangeLimited)
        {        
            v = (points[i].count - minCounter) / counterRangeLimited;
            
            v = pow(v, 0.45);
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
}

static void applyFunction(Function *pFun, double &x, double &y)
{
    double xAccum=0, yAccum=0;

    x=pFun->preTransformKoef[0][0] * x + pFun->preTransformKoef[0][1] * y + pFun->preTransformKoef[0][2];
    y=pFun->preTransformKoef[1][0] * x + pFun->preTransformKoef[1][1] * y + pFun->preTransformKoef[1][2];            
        
    for(auto fun : pFun->variations)
    {            
        double xOut,yOut;
        
        fun(x , y, xOut, yOut);
        
        xAccum += xOut;
        yAccum += yOut;
    }
    
    x=pFun->postTransformKoef[0][0] * xAccum + pFun->postTransformKoef[0][1] * yAccum + pFun->postTransformKoef[0][2];
    y=pFun->postTransformKoef[1][0] * xAccum + pFun->postTransformKoef[1][1] * yAccum + pFun->postTransformKoef[1][2];            
}

void calculateFractal()
{
    double x, y;
    
    memset(intermediateOutput,0,sizeof(unsigned int) * outputSize);
    
    getInitialPoint(x,y);
    
    int noPlotCounter=0;
    
    for(int i=0;i<numberOfIterations;i++)
    {
        Function* pFun=getRandomFunction();
                
        applyFunction(pFun, x, y);        
        
        if(++noPlotCounter>20)
        {
            plot(x,y,pFun);    
        }        
    }
    
    createOutput();
}

unsigned int* fractalStep()
{
    //functions[0]->preTransformKoef[0][0]+=0.01;
    //functions[1]->preTransformKoef[1][1]+=0.01;
    calculateFractal();
    return output;
}
