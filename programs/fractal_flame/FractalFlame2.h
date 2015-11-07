#pragma once

#include <memory>

#include "FractalFlameAlgorithm.h"

using namespace std;

class FractalFlame2{
public:
    
    void render(shared_ptr<RenderParameters> renderParams, const char *fileName);
    
private:
    
    FractalFlameAlgorithm algorithm;
    
};

