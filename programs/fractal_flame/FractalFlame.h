#pragma once

#include <memory>

#include "FractalFlameAlgorithm.h"

using namespace std;

class FractalFlame{
public:
    
#ifndef NO_IMAGE_FUNCTIONS

    void render(shared_ptr<RenderParameters> renderParams, const char *fileName);
    void preview(int numberOfPreviews, shared_ptr<RenderParameters> renderParams, shared_ptr<FlameParameters> flameParams);
    
    FractalFlameAlgorithm::CalculateFractalResult screensaverSaveCurrentFractal(const char* fileName);
    
#endif    
    

    void screensaverInit(int width, int height);
    FractalFlameAlgorithm::CalculateFractalResult screensaver(unsigned int **ppOutput);    
    
    void setStopFlag();
    
    static void initFlameLibrary();
    
private:    
    
    FractalFlameAlgorithm algorithm;    

};

