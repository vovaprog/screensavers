#pragma once

#include <vector>

#include "Variation.h"


class Function{
public:        
    int probabilityWeight;
    int probabilityUpBorder;
    
    unsigned int r,g,b;
    
    //double preTransformKoef[2][3];
    //double postTransformKoef[2][3];
    
    double preTransformX_CoefX=1.0, preTransformX_CoefY=0.0, preTransformX_CoefC=0.0;
    double preTransformY_CoefX=0.0, preTransformY_CoefY=1.0, preTransformY_CoefC=0.0;
    
    double postTransformX_CoefX=1.0, postTransformX_CoefY=0.0, postTransformX_CoefC=0.0;
    double postTransformY_CoefX=0.0, postTransformY_CoefY=1.0, postTransformY_CoefC=0.0;
    
    std::vector<Variation> variations;
    
    Function():probabilityWeight(1),r(255),g(255),b(255)
    {
    }    
};


