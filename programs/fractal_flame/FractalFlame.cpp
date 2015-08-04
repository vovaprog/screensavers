#include <string>
#include <string.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>

#include <FreeImage.h>

#include "FractalFlame.h"
#include "filesystem_utils.h"

using namespace std;


void FractalFlame::resetVariables()
{
    functions.clear();
    
    goodPointCounter=0;
    badPointCounter=0;
}

void FractalFlame::fractalInit(int argPictureWidth, int argPictureHeight)
{	
	functions.clear();
	
    pictureWidth=argPictureWidth;
    pictureHeight=argPictureHeight;    
    outputSize=pictureWidth*pictureHeight;

    output=unique_ptr<unsigned int[]>(new unsigned int[outputSize]);
    points=unique_ptr<Point[]>(new Point[outputSize]);
    
    srand((unsigned int)time(NULL));    
    
    stopFlag.store(false);
}


void FractalFlame::convertScreenToMath(double &x, double &y)
{
    x = -1.0 + (x / (pictureWidth/2.0));
    y = -1.0 + (y / (pictureHeight/2.0));
}

bool FractalFlame::convertMathToScreen(double x, double y,int &xOut,int &yOut)
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
}

void FractalFlame::getInitialPoint(double &x, double &y)
{
    x = rand() % pictureWidth;
    y = rand() % pictureHeight;
    
    convertScreenToMath(x,y);
}

Function* FractalFlame::getRandomFunction()
{
    int randomValue = rand() % totalProbabilityWeight;
    
    for(auto& funIter : functions)
    {
        if(randomValue<funIter->probabilityUpBorder)
        {
            return funIter.get();    
        }
    }
    
    return nullptr;
}

inline int FractalFlame::outputIndex(int screenX,int screenY)
{
    return screenY * pictureWidth + screenX;    
}

CalculateFractalResult FractalFlame::plot(double mathX, double mathY, Function *pFun,unsigned int currentIteration)
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
            return CalculateFractalResult::BAD_PICTURE;
        }
    }
    
    return CalculateFractalResult::SUCCESS;
}

void FractalFlame::findMinMaxOutput(unsigned int &minOutput,unsigned int &maxOutput)
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

unsigned int FractalFlame::histAnalysis(unsigned int minCounter,unsigned int maxCounter)
{        
    unsigned int counterRange=maxCounter-minCounter;
    
    int l = (int)log10(counterRange) - 1;
    
    if(l<1)
    {
        return 0;
    }

    unsigned int HIST_BINS = (unsigned int)pow(10, l);
    
    cout << "histogram bins: "<<HIST_BINS<<endl;
    
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
        
    cout << "bin size: "<<binSize << endl;    
    
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
        
        if(((double)pointSum / (double)outputSize)>0.999)//0.995)
        {
            return minCounter + (i+1) * binSize;
        }
    }

           
    return 0;
}

CalculateFractalResult FractalFlame::createOutput()
{
    unsigned int maxCounter, minCounter;
    findMinMaxOutput(minCounter, maxCounter);
    
    cout << "max count: " << maxCounter << "   min count: "<<minCounter << endl;
    
    double maxCounterDivAll = (double)maxCounter / (double)numberOfIterations;
    
    if(maxCounter==0 || maxCounter<=minCounter+10 || maxCounterDivAll>=0.5)
    {
    	cout <<"bad picture!"<<endl;
    	memset(output.get(),0,outputSize * sizeof(unsigned int));
    	return CalculateFractalResult::BAD_PICTURE;
    }
        
    unsigned int counterUpLimit = histAnalysis(minCounter, maxCounter);

    for(int i=0;i<outputSize;i++)
    {
        double v;
        
        if(points[i].count<counterUpLimit)
        {        
            v = ((double)points[i].count - (double)minCounter) / ((double)counterUpLimit-(double)minCounter);
            
            v = pow(v, colorPowerArgument);
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

void FractalFlame::applyFunction(Function *pFun, double &x, double &y)
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

void FractalFlame::cleanBuffers()
{
    for(int i=0;i<outputSize;i++)
    {
        points[i].r=0;
        points[i].g=0;
        points[i].b=0;
        points[i].count=0;
    }
}



CalculateFractalResult FractalFlame::calculateFractal()
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
                memset(output.get(),0,outputSize * sizeof(unsigned int));
                return CalculateFractalResult::BAD_PICTURE;   
            }
        }        
        
        if((i & 0xff)==0)
        {
            if(stopFlag.load())
            {
                cout <<"iterations: "<<i<<endl;
                break;
            }
        }
    }
    
    return createOutput();
}



void FractalFlame::saveImage(const char *fileName,const char *fileType)
{ 
	if(!saveOutput)
	{
		saveOutput=unique_ptr<unsigned int[]>(new unsigned int[outputSize]);	
	}
	
	for(int i=0;i<outputSize;i++)
	{
		saveOutput[i]=(0xff000000 | ((output[i] & 0xff) << 16) | (output[i] & 0xff00) | ((output[i] & 0xff0000) >>16)); 
	}
	
	FIBITMAP* Image = FreeImage_ConvertFromRawBits((BYTE*)saveOutput.get(), pictureWidth, pictureHeight, pictureWidth * sizeof(unsigned int), 32, 
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

void FractalFlame::saveCurrentFractal(const char *argDirName,int index)
{
    string dirName(argDirName);
    
    string fileName=dirName+"/fractal_"+to_string(index)+".xml";    
    if(fileExists(fileName.c_str()))
    {
        deleteFile(fileName.c_str());
    }        
    saveFunctions(fileName.c_str(),functions);

    
    fileName=dirName + "/fractal_"+to_string(index)+".png";
    if(fileExists(fileName.c_str()))
    {
        deleteFile(fileName.c_str());
    }    
    saveImage(fileName.c_str(),"png");		        
}

void FractalFlame::fractalPreview(int numberOfPreviews)
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
		initFunctionsRandom(functions,totalProbabilityWeight);
		
		if(calculateFractal()==CalculateFractalResult::SUCCESS)
		{		    
            saveCurrentFractal(dirName.c_str(),i);
		}
		else
		{
		    i-=1;    
		}		
	}		
}

void FractalFlame::fractalRender(const char *fileName)
{
	loadFunctions(fileName,functions,totalProbabilityWeight);
	
	calculateFractal();
	
	string outputFileName(fileName);
	outputFileName += "_render.bmp";
	
	saveImage(outputFileName.c_str(),"bmp");
}

CalculateFractalResult FractalFlame::fractalScreensaver(unsigned int **ppOutput)
{
	resetVariables();
	initFunctionsRandom(functions,totalProbabilityWeight);
	stopFlag.store(false);
	numberOfIterations=300000000;
	
	CalculateFractalResult result=CalculateFractalResult::BAD_PICTURE;
	
	for(int i=0;i<30;i++)
	{
	    result=calculateFractal();
	    
		if(result==CalculateFractalResult::SUCCESS)
		{
			break;
		}
		else if(result==CalculateFractalResult::BAD_PICTURE)
		{
		    if(!stopFlag.load())
		    {		 
                resetVariables();
                initFunctionsRandom(functions,totalProbabilityWeight);
			}
			else
			{
			    break;
			}
		}
	}
	
	*ppOutput=output.get();
	return result;
}

void FractalFlame::fractalSetNumberOfIterations(int argNumberOfIterations)
{
    numberOfIterations = argNumberOfIterations;   
}

void FractalFlame::fractalSetStopFlag()
{
    stopFlag.store(true);    
}

