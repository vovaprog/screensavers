#include <vector>
#include <climits>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <iostream>

#include <FreeImage.h>

using namespace std;

#include "flame_fractal.h"

#include "Function.h"
#include "Point.h"

static unsigned int *output;
static int pictureWidth,pictureHeight;
static int outputSize;

static vector<Function*> functions;

static int totalProbabilityWeight;

static int numberOfIterations=3000000;

static Point *points;

unsigned int *hist=nullptr;
unsigned int histSize=0;

void initFunctions(vector<Function*> &functions, int &totalProbabilityWeight);
void initFunctionsRandom(vector<Function*> &functions, int &totalProbabilityWeight);

static void destroyFunctions()
{
    for(auto funIter : functions)
    {
        delete funIter;
    }
    
    functions.clear();
}

void fractalInit(int argPictureWidth, int argPictureHeight)
{
	Function f;
	f.load("./fractals/fractal_1.xml");
	
    /*pictureWidth=argPictureWidth;
    pictureHeight=argPictureHeight;    
    outputSize=pictureWidth*pictureHeight;
    
    output=new unsigned int[outputSize];
    
    srand(time(NULL));
    
    //initFunctions(functions,totalProbabilityWeight);
    
    points=new Point[outputSize];
    
    //hist =new unsigned int[outputSize];*/
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

static unsigned int histAnalysis(unsigned int minCounter,unsigned int maxCounter)
{        
    unsigned int counterRange=maxCounter-minCounter;
    
    unsigned int l = (unsigned int)log10(counterRange) - 1;
    
    cout <<"counterRange: "<<counterRange<<endl;
    cout <<"log10: "<<l<<endl;
        
    unsigned int HIST_BINS = (unsigned int)pow(10, l);
    
    cout << "HIST_BINS: "<<HIST_BINS<<endl;
    
    if(counterRange < HIST_BINS)
    {
        return 0;
    }
    
    if(histSize < HIST_BINS)
    {
    	if(hist==nullptr)
    	{
    		delete[] hist;	
    	}
    	
    	hist = new unsigned int[HIST_BINS];
    	histSize = HIST_BINS;
    }
    
    memset(hist,0,HIST_BINS*sizeof(unsigned int));
    
    
    unsigned int binSize = counterRange / HIST_BINS;
        
    cout << "binSize: "<<binSize << endl;    
    
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
        cout <<i<<" : "<<hist[i]<<endl;
        pointSum += hist[i];
        
        if(((double)pointSum / (double)outputSize)>0.995)
        {
            return minCounter + (i+1) * binSize;
        }
    }

           
    return 0;
}

static void createOutput()
{
    unsigned int maxCounter, minCounter;
    findMinMaxOutput(minCounter, maxCounter);
    
    cout << "max count: " << maxCounter << "   min count: "<<minCounter << endl;
    
    double maxCounterDivAll = (double)maxCounter / (double)numberOfIterations;
    cout <<"maxCounterToAll: "<<maxCounterDivAll<<endl;
    
    if(maxCounterDivAll>=0.5)
    {
    	cout <<"bad picture!"<<endl;
    	memset(output,0,outputSize * sizeof(unsigned int));
    	return;
    }
    
    
    unsigned int counterUpLimit = histAnalysis(minCounter, maxCounter);
        
    cout << "counterUpLimit: " << counterUpLimit <<endl;

    for(int i=0;i<outputSize;i++)
    {
        double v;
        
        if(points[i].count<counterUpLimit)
        {        
            v = ((double)points[i].count - (double)minCounter) / ((double)counterUpLimit-(double)minCounter);
            
            v = pow(v, 0.45);
        }
        else
        {
            v = 1.0;   
        }
            
        points[i].r = (unsigned int)(points[i].r * v);
        points[i].g = (unsigned int)(points[i].g * v);
        points[i].b = (unsigned int)(points[i].b * v);
        
        output[i] = (0xff000000 | (points[i].b << 16) | (points[i].g <<8) | points[i].r);
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

void cleanBuffers()
{
    for(int i=0;i<outputSize;i++)
    {
        points[i].r=0;
        points[i].g=0;
        points[i].b=0;
        points[i].count=0;
    }
}

void calculateFractal()
{
    double x, y;
        
    cleanBuffers();    
    
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

unsigned int *saveOutput=nullptr;

void saveImage()
{
	if(saveOutput==nullptr)
	{
		saveOutput=new unsigned int[outputSize];	
	}
	
	for(int i=0;i<outputSize;i++)
	{
		saveOutput[i]=(0xff000000 | ((output[i] & 0xff) << 16) | (output[i] & 0xff00) | ((output[i] & 0xff0000) >>16)); 
	}
	
	//BYTE pixels [3*WIDTH*HEIGHT];
	//glReadPixels(0,0,WIDTH,HEIGHT, GL_RGB, GL_BYTE, pixels);
	FIBITMAP* Image = FreeImage_ConvertFromRawBits((BYTE*)saveOutput, pictureWidth, pictureHeight, pictureWidth * sizeof(unsigned int), 32,
		//FI_RGBA_GREEN_MASK,FI_RGBA_GREEN_MASK,FI_RGBA_RED_MASK,false); 
		0x0F0000, 0x00000F, 0x000F00, false); 
	FreeImage_Save(FIF_BMP, Image, "./test.bmp", 0);	
}

unsigned int* fractalStep()
{
    /*destroyFunctions();
    initFunctionsRandom(functions,totalProbabilityWeight);    
    
    calculateFractal();
    saveImage();*/
    return output;
}
