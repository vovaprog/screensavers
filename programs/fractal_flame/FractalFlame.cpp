#include <string>
#include <cstdlib>

#include "FractalFlame.h"

#ifndef NO_IMAGE_FUNCTIONS
#   include "image_utils.h"
#   include "filesystem_utils.h"
#endif

using namespace std;


#ifndef NO_IMAGE_FUNCTIONS

void FractalFlame::render(shared_ptr<RenderParameters> renderParams, const char *fileName)
{
    algorithm.setRenderParameters(renderParams);
    
    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->load(fileName);
    
    unsigned int* output;
    
    FractalFlameAlgorithm::CalculateFractalResult result = algorithm.calculate(fp, &output);
    
    if(result==FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
    {        
        string outputFileName(fileName);
        outputFileName += "_render.png";        
                
        saveImage(outputFileName.c_str(), "png", output, renderParams->pictureWidth, renderParams->pictureHeight);
    }
}

void FractalFlame::preview(int numberOfPreviews, shared_ptr<RenderParameters> renderParams, shared_ptr<FlameParameters> flameParams)
{    
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

		unsigned int* output;
		
		FractalFlameAlgorithm::CalculateFractalResult result = algorithm.calculate(flameParams, &output);
	    
		if(result==FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
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

FractalFlameAlgorithm::CalculateFractalResult FractalFlame::screensaverSaveCurrentFractal(const char* fileName)
{
    FractalFlameAlgorithm::CalculateFractalResult result = algorithm.getLastResult();
    
    if(result == FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
    {
        shared_ptr<FlameParameters> fp = algorithm.getFlameParameters();
        shared_ptr<RenderParameters> rp = algorithm.getRenderParameters();
        unsigned int *output=algorithm.getOutput();
        
        if(output!=nullptr && fp && rp)
        {
            saveImage((string(fileName)+".png").c_str(), "png", output, rp->pictureWidth, rp->pictureHeight);
            fp->save((string(fileName)+".xml").c_str());
        }
    }
    
    return result;
}

#endif

void FractalFlame::screensaverInit(int width, int height)
{
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=width;
    rp->pictureHeight=height;    
    rp->numberOfIterations=500000000;
    algorithm.setRenderParameters(rp);
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
    
	
    FractalFlameAlgorithm::CalculateFractalResult result;
    
	for(int i=0;i<30;i++)
	{	    
	    result = algorithm.calculate(flameParams, ppOutput);	    
	    
		if(result==FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
		{		    
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
			    return result;
			}
		}
	}
	
	return result;
}

void FractalFlame::setStopFlag()
{
    algorithm.setStopFlag(true);
}

void FractalFlame::initFlameLibrary()
{
    srand(time(NULL));
    setlocale(LC_NUMERIC,"C");
    initVariations();
}


