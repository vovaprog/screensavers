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

static int numberOfIterations=3000000;

Point *points;

void initFunctions(vector<Function*> &functions, int totalProbabilityWeight);

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


/*static void calcFunction(FunctionData &funData, double x, double y, double &xOut, double &yOut)
{    
    funData.function(x,y,xOut,yOut);
}

static void calcFinalFunction(double x, double y, double &xOut, double &yOut)
{
    if(finalFunctionPointer!=0)
    {
        finalFunctionPointer(x,y,xOut,yOut);    
    }
}*/

int outputIndex(int screenX,int screenY)
{
    return screenY * pictureWidth + screenX;    
}

void plot(double mathX, double mathY, Function *pFun)
{
    int screenX,screenY;
    
    convertMathToScreen(mathX,mathY,screenX,screenY);
    
    //intermediateOutput[outputIndex(screenX,screenY)]+=1;
    
    int i = outputIndex(screenX,screenY);
    
    points[i].count += 1;    
    points[i].r = (points[i].r + pFun->r) / 2;
    points[i].g = (points[i].g + pFun->g) / 2;
    points[i].b = (points[i].b + pFun->b) / 2;
}

void findMinMaxOutput(unsigned int &minOutput,unsigned int &maxOutput)
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

void createOutput()
{
    unsigned int maxCounter, minCounter;
    findMinMaxOutput(minCounter, maxCounter);
    
    cout << "max count: " << maxCounter <<endl;
    
    double counterRange=maxCounter-minCounter;
    
    for(int i=0;i<outputSize;i++)
    {
        double v = (points[i].count - minCounter) / counterRange;
        
        //v = sqrt (v);
        v = pow(v, 0.2);
        
        //unsigned int color = (unsigned int)(v * 255.0);
 
        //output[i] = (0xff000000 | (color << 16) | (color <<8) | color);
        
        points[i].r = (unsigned int)(points[i].r * v);
        points[i].g = (unsigned int)(points[i].g * v);
        points[i].b = (unsigned int)(points[i].b * v);
        
        output[i] = (0xff000000 | (points[i].r << 16) | (points[i].g <<8) | points[i].b);
    }
}

void calculateFractal()
{
    double x, y, xOut, yOut;
    
    memset(intermediateOutput,0,sizeof(unsigned int) * outputSize);
    
    getInitialPoint(x,y);
    
    int noPlotCounter=0;
    
    for(int i=0;i<numberOfIterations;i++)
    {
        Function* pFun=getRandomFunction();
        
        pFun->calculate(x,y,xOut,yOut);
        
        x=pFun->postTransformKoef[0][0] * xOut + pFun->postTransformKoef[0][1] * yOut + pFun->postTransformKoef[0][2];
        y=pFun->postTransformKoef[1][0] * xOut + pFun->postTransformKoef[1][1] * yOut + pFun->postTransformKoef[1][2];        

        /*if(x<=-1.0 || y<=-1.0 || x>=1.0 || y>=1.0)
        {
            cout << "!!!"<<x<<"   "<<y<<endl;
            getInitialPoint(x,y);
            
            //noPlotCounter=0;
        }*/
        
        if(++noPlotCounter>20)
        {
            plot(x,y,pFun);    
        }        
    }
    
    createOutput();
}

unsigned int* fractalStep()
{
    calculateFractal();
    return output;
}
