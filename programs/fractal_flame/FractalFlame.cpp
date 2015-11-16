#include <string>

#include <iostream>

#include "FractalFlame.h"
#include "image_utils.h"
#include "filesystem_utils.h"

using namespace std;

void FractalFlame::render(shared_ptr<RenderParameters> renderParams, const char *fileName)
{
    algorithm.setRenderParameters(renderParams);
    
    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->load(fileName);
    
    unsigned int* output = algorithm.calculate(fp);
    
    if(output!=nullptr)
    {        
        string outputFileName(fileName);
        outputFileName += "_render.png";        
                
        saveImage(outputFileName.c_str(), "png", output, renderParams->pictureWidth, renderParams->pictureHeight);
    }
}

void FractalFlame::preview(int numberOfPreviews, shared_ptr<RenderParameters> renderParams, shared_ptr<FlameParameters> flameParams)
{
    srand((unsigned int)time(NULL));
    
    algorithm.setRenderParameters(renderParams);
    
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
	    flameParams->initRandom();

		unsigned int* output = algorithm.calculate(flameParams);
	    
		if(output!=nullptr)
		{
            string fileName=dirName+"/fractal_"+to_string(i)+".xml";    
            if(fileExists(fileName.c_str()))
            {
                deleteFile(fileName.c_str());
            }            
            flameParams->save(fileName.c_str());
            
            fileName=dirName + "/fractal_"+to_string(i)+".png";
            if(fileExists(fileName.c_str()))
            {
                deleteFile(fileName.c_str());
            }    
            
            saveImage(fileName.c_str(), "png", output, renderParams->pictureWidth, renderParams->pictureHeight);		  		    
		}
		else
		{
		    i--;
		}
	}        
}

void FractalFlame::screensaverInit(int width, int height)
{
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=width;
    rp->pictureHeight=height;    
    rp->numberOfIterations=500000000;
    algorithm.setRenderParameters(rp);
}

void FractalFlame::screensaverSaveFractal(const char* screensaverSaveDirName,int screensaverSaveCounter)
{
    shared_ptr<RenderParameters> rp = algorithm.getRenderParameters();
    shared_ptr<FlameParameters> fp = algorithm.getFlameParameters();
    unsigned int* output = algorithm.getOutput();
    
    if(fp && rp && output)
    {    
        string dirName(screensaverSaveDirName);
        
		if(!directoryExists(dirName.c_str()))
		{			
			createDirectory(dirName.c_str());
		}        
        
        string fileName=dirName+"/fractal_"+to_string(screensaverSaveCounter)+".xml";    
        if(fileExists(fileName.c_str()))
        {
            deleteFile(fileName.c_str());
        }        
        fp->save(fileName.c_str());
        
        fileName=dirName + "/fractal_"+to_string(screensaverSaveCounter)+".png";
        if(fileExists(fileName.c_str()))
        {
            deleteFile(fileName.c_str());
        }    
        
        saveImage(fileName.c_str(), "png", output, rp->pictureWidth, rp->pictureHeight);
                
        unsigned long lll=(unsigned long)this;
        cout <<"fractal save this pointer: "<<lll<<endl;

    }
}

FractalFlameAlgorithm::CalculateFractalResult FractalFlame::screensaver(unsigned int **ppOutput)
{
    shared_ptr<FlameParameters> flameParams = algorithm.getFlameParameters();
    
    if(!flameParams)
    {
        flameParams=shared_ptr<FlameParameters>(new FlameParameters());        
    }
    
    flameParams->initRandom();
    algorithm.setStopFlag(false);
    
	
	for(int i=0;i<30;i++)
	{
	    unsigned int* output = algorithm.calculate(flameParams);
	    
		if(output!=nullptr)
		{
		    *ppOutput=output;
		    
		    return FractalFlameAlgorithm::CalculateFractalResult::SUCCESS;
		}
		else
		{
		    if(!algorithm.getStopFlag())
		    {		 
                flameParams->initRandom();
			}
			else
			{
			    *ppOutput=nullptr;
			    return FractalFlameAlgorithm::CalculateFractalResult::BAD_PICTURE;
			}
		}
	}
	
	*ppOutput=nullptr;
	return FractalFlameAlgorithm::CalculateFractalResult::BAD_PICTURE;
}


void FractalFlame::setStopFlag()
{
    algorithm.setStopFlag(true);
}


