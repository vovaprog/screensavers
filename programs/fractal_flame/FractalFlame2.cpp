#include <string>

#include "FractalFlame2.h"
#include "ImageUtils.h"

using namespace std;

void FractalFlame2::render(shared_ptr<RenderParameters> renderParams, const char *fileName)
{
    algorithm.setRenderParameters(renderParams);
    
    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->load(fileName);
    
    unsigned int* output = algorithm.calculate(fp);
    
    if(output!=nullptr)
    {    
        ImageUtils imgUtils;
    
        string outputFileName(fileName);
        outputFileName += "_render.png";        
        
        imgUtils.saveImage(outputFileName.c_str(), "png", output, renderParams->pictureWidth, renderParams->pictureHeight);
    }
}

