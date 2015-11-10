#include <string>

#include "FractalFlame2.h"
#include "ImageUtils.h"
#include "filesystem_utils.h"

using namespace std;

void FractalFlame2::render(shared_ptr<RenderParameters> renderParams, const char *fileName)
{
    algorithm.setRenderParameters(renderParams);
    
    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->load(fileName);
    
    unsigned int* output = algorithm.calculate(fp);
    
    if(output!=nullptr)
    {    
        //ImageUtils imgUtils;
    
        string outputFileName(fileName);
        outputFileName += "_render.png";        
        
        //imgUtils.
        saveImage(outputFileName.c_str(), "png", output, renderParams->pictureWidth, renderParams->pictureHeight);
    }
}

void FractalFlame2::preview(int numberOfPreviews, shared_ptr<RenderParameters> renderParams, shared_ptr<FlameParameters> flameParams)
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

	//ImageUtils imgUtils;
	
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
            //imgUtils.
            saveImage(fileName.c_str(), "png", output, renderParams->pictureWidth, renderParams->pictureHeight);		  		    
		}
		else
		{
		    i--;
		}
	}        
}

void FractalFlame2::screensaverInit(int width, int height)
{
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=width;
    rp->pictureHeight=height;    
    rp->numberOfIterations=500000000;
    algorithm.setRenderParameters(rp);
}

FractalFlameAlgorithm::CalculateFractalResult FractalFlame2::screensaver(unsigned int **ppOutput)
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


void FractalFlame2::setStopFlag()
{
    algorithm.setStopFlag(true);
}


