#include <vector>
#include <climits>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <atomic>

#include <FreeImage.h>

using namespace std;

#include "flame_fractal.h"

#include "Function.h"
#include "Point.h"
#include "init_functions.h"
#include "filesystem_utils.h"

enum class CalculateFractalResult { SUCCESS, BAD_PICTURE, STOP_FLAG };

static unsigned int *output=nullptr;
static unsigned int *saveOutput=nullptr;
static Point *points=nullptr;
unsigned int *hist=nullptr;

static vector<Function*> functions;

static int pictureWidth,pictureHeight;
static int outputSize;

static int totalProbabilityWeight;

static unsigned int numberOfIterations=30000000;

static unsigned int histSize=0;

static atomic<bool> stopFlag;


static void destroyFunctions()
{
    for(auto funIter : functions)
    {
        delete funIter;
    }
    
    functions.clear();
}

void fractalDestroy()
{
	if(output!=nullptr)
	{
		delete[] output;
		output=nullptr;
	}
	if(points!=nullptr)
	{
		delete[] points;
		points=nullptr;
	}
	if(hist!=nullptr)
	{
		delete[] hist;
		hist=nullptr;
	}
	if(saveOutput!=nullptr)
	{
		delete[] saveOutput;
		saveOutput=nullptr;
	}
}

void fractalInit(int argPictureWidth, int argPictureHeight)
{	
	fractalDestroy();
	destroyFunctions();
	
    pictureWidth=argPictureWidth;
    pictureHeight=argPictureHeight;    
    outputSize=pictureWidth*pictureHeight;
    
    output=new unsigned int[outputSize];
    points=new Point[outputSize];
    
    srand(time(NULL));    
    
    stopFlag.store(false);
}


static void convertScreenToMath(double &x, double &y)
{
    x = -1.0 + (x / (pictureWidth/2.0));
    y = -1.0 + (y / (pictureHeight/2.0));
}

static bool convertMathToScreen(double x, double y,int &xOut,int &yOut)
{
    xOut = (int)(((x+1.0)/2.0) * pictureWidth);
    yOut = (int)(((y+1.0)/2.0) * pictureHeight);
    
    if(xOut<0 || yOut<0 || xOut>=pictureWidth || yOut>=pictureHeight)
    {
        return false;
    }
    else
    {
        return true;
    }
/*    
    if(xOut>=pictureWidth) xOut=pictureWidth-1;
    else if(xOut<0) xOut=0;
    
    if(yOut>=pictureHeight) yOut=pictureHeight-1;
    else if(yOut<0) yOut=0;*/
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

static CalculateFractalResult plot(double mathX, double mathY, Function *pFun,unsigned int currentIteration)
{
    int screenX,screenY;
    
    if(convertMathToScreen(mathX,mathY,screenX,screenY))
    {    
        int i = outputIndex(screenX,screenY);
        
        if(currentIteration > numberOfIterations / 8 &&
            points[i].count > currentIteration / 2)
        {
            cout <<"early rec BAD_PICTURE!"<<endl;
            return CalculateFractalResult::BAD_PICTURE;    
        }
        
        points[i].count += 1;    
        points[i].r = (points[i].r + pFun->r) / 2;
        points[i].g = (points[i].g + pFun->g) / 2;
        points[i].b = (points[i].b + pFun->b) / 2;
    }
    
    return CalculateFractalResult::SUCCESS;
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
    
    int l = (int)log10(counterRange) - 1;
    
    if(l<1)
    {
        return 0;
    }
    
    cout <<"counterRange: "<<counterRange<<endl;
    cout <<"log10: "<<l<<endl;
        
    unsigned int HIST_BINS = (unsigned int)pow(10, l);
    
    cout << "HIST_BINS: "<<HIST_BINS<<endl;
    
    if(counterRange < HIST_BINS || HIST_BINS<2)
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

static CalculateFractalResult createOutput()
{
    unsigned int maxCounter, minCounter;
    findMinMaxOutput(minCounter, maxCounter);
    
    cout << "max count: " << maxCounter << "   min count: "<<minCounter << endl;
    
    double maxCounterDivAll = (double)maxCounter / (double)numberOfIterations;
    cout <<"maxCounterToAll: "<<maxCounterDivAll<<endl;
    
    if(maxCounter==0 || maxCounter<=minCounter+10 || maxCounterDivAll>=0.5)
    {
    	cout <<"bad picture!"<<endl;
    	memset(output,0,outputSize * sizeof(unsigned int));
    	return CalculateFractalResult::BAD_PICTURE;
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
    
    return CalculateFractalResult::SUCCESS;
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

static void cleanBuffers()
{
    for(int i=0;i<outputSize;i++)
    {
        points[i].r=0;
        points[i].g=0;
        points[i].b=0;
        points[i].count=0;
    }
}



static CalculateFractalResult calculateFractal()
{
    double x, y;
        
    cleanBuffers();    
    
    getInitialPoint(x,y);
    
    for(unsigned int i=0;i<numberOfIterations;i++)
    {
        Function* pFun=getRandomFunction();
                
        applyFunction(pFun, x, y);        
        
        if(i>20)
        {
            if(plot(x,y,pFun,i)==CalculateFractalResult::BAD_PICTURE)
            {
                return CalculateFractalResult::BAD_PICTURE;   
            }
        }        
        
        if((i & 0xff)==0)
        {
            if(stopFlag.load())
            {
                return CalculateFractalResult::STOP_FLAG;   
            }
        }
    }
    
    return createOutput();
}



static void saveImage(const char *fileName,const char *fileType)
{ 
	if(saveOutput==nullptr)
	{
		saveOutput=new unsigned int[outputSize];	
	}
	
	for(int i=0;i<outputSize;i++)
	{
		saveOutput[i]=(0xff000000 | ((output[i] & 0xff) << 16) | (output[i] & 0xff00) | ((output[i] & 0xff0000) >>16)); 
	}
	
	FIBITMAP* Image = FreeImage_ConvertFromRawBits((BYTE*)saveOutput, pictureWidth, pictureHeight, pictureWidth * sizeof(unsigned int), 32, 
		0xFF0000, 0x00FF00, 0x0000FF, false); 
	
	if(strcmp(fileType,"png")==0)
	{
		FreeImage_Save(FIF_PNG, Image, fileName, 0);
	}
	else
	{
		FreeImage_Save(FIF_BMP, Image, fileName, 0);
	}
	
	FreeImage_Unload(Image);
}

void fractalPreview(int numberOfPreviews)
{    
	string dirName;
	
	for(int i=0;i<500;i++)
	{
		dirName="./fractals/"+to_string(i);
		
		if(!directoryExists(dirName.c_str()))
		{			
			createDirectory(dirName.c_str());
			break;
		}
	}

	for(int i=0;i<numberOfPreviews;i++)
	{
		destroyFunctions();
		initFunctionsRandom(functions,totalProbabilityWeight);
		
		if(calculateFractal()==CalculateFractalResult::SUCCESS)
		{
            string fileName=dirName+"/fractal_"+to_string(i)+".xml";            
            saveFunctions(fileName.c_str(),functions);
            fileName=dirName + "/fractal_"+to_string(i)+".png";
            saveImage(fileName.c_str(),"png");		    
		}
		else
		{
		    i-=1;    
		}		
	}		
}

void fractalRender(const char *fileName)
{
	destroyFunctions();
	loadFunctions(fileName,functions,totalProbabilityWeight);
	
	numberOfIterations = 30000000;
	
	calculateFractal();
	
	string outputFileName(fileName);
	outputFileName += "_render.bmp";
	
	saveImage(outputFileName.c_str(),"bmp");
}

unsigned int* fractalRandom()
{
	destroyFunctions();
	initFunctionsRandom(functions,totalProbabilityWeight);
	
	for(int i=0;i<50;i++)
	{
	    CalculateFractalResult result=calculateFractal();
	    
		if(result==CalculateFractalResult::SUCCESS)
		{
			break;
		}
		else if(result==CalculateFractalResult::BAD_PICTURE)
		{
			initFunctionsRandom(functions,totalProbabilityWeight);
		}
		else if(result==CalculateFractalResult::STOP_FLAG)
		{
		    return nullptr;    
		}
	}
	
	return output;
}

void fractalSetNumberOfIterations(int argNumberOfIterations)
{
    numberOfIterations = argNumberOfIterations;   
}

void fractalSetStopFlag()
{
    stopFlag.store(true);    
}

