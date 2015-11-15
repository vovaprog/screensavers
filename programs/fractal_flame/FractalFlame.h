#pragma once

#include <memory>

#include "FractalFlameAlgorithm.h"

using namespace std;

class FractalFlame2{
public:
    
    void render(shared_ptr<RenderParameters> renderParams, const char *fileName);
    void preview(int numberOfPreviews, shared_ptr<RenderParameters> renderParams, shared_ptr<FlameParameters> flameParams);
    
    void screensaverInit(int width, int height);
    FractalFlameAlgorithm::CalculateFractalResult screensaver(unsigned int **ppOutput);

    void setStopFlag();
    
private:    
    
    FractalFlameAlgorithm algorithm;
    
};

