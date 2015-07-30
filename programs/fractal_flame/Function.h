#pragma once

#include <vector>

#include "Variation.h"

using namespace std;

class Function{
public:        
    int probabilityWeight;
    int probabilityUpBorder;
    
    unsigned int r,g,b;
    
    double preTransformKoef[2][3];
    double postTransformKoef[2][3];

    vector<VariationPointer> variations;
    
    Function():probabilityWeight(1),r(255),g(255),b(255)
    {
        preTransformKoef[0][0]=1.0;
        preTransformKoef[0][1]=0.0;
        preTransformKoef[0][2]=0.0;
        
        preTransformKoef[1][0]=0.0;
        preTransformKoef[1][1]=1.0;
        preTransformKoef[1][2]=0.0;        
        
        
        postTransformKoef[0][0]=1.0;
        postTransformKoef[0][1]=0.0;
        postTransformKoef[0][2]=0.0;
        
        postTransformKoef[1][0]=0.0;
        postTransformKoef[1][1]=1.0;
        postTransformKoef[1][2]=0.0;        
    }
};


