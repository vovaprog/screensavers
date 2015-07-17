#pragma once

#include <vector>

#include <cstdlib>

#include <cmath>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

using namespace std;

typedef void (*VariationPointer)(double x, double y, double &xOut, double &yOut);

class Function{
public:        
    int probabilityWeight;
    int probabilityUpBorder;
    
    unsigned int r,g,b;
    
    double postTransformKoef[2][3];

    vector<VariationPointer> variations;
    
    Function():probabilityWeight(1.0),r(255),g(255),b(255)
    {
        postTransformKoef[0][0]=1.0;
        postTransformKoef[0][1]=0.0;
        postTransformKoef[0][2]=0.0;
        
        postTransformKoef[1][0]=0.0;
        postTransformKoef[1][1]=1.0;
        postTransformKoef[1][2]=0.0;        
    }
};

//=================================================================

inline double radius(double x, double y)
{
    return sqrt(x*x + y*y);
}

inline double theta(double x, double y)
{
    return atan(x/y);    
}

inline double fi(double x, double y)
{
    return atan(y/x);    
}

inline double omega()
{
    if(rand() % 2==0)
    {
        return M_PI;    
    }
    else
    {
        return 0.0;    
    }
}

//=================================================================

inline void variationSin(double x, double y, double &xOut, double &yOut) //+
{
    xOut=sin(x);
    yOut=sin(y);
}    

inline void variationFisheye(double x, double y, double &xOut, double &yOut)
{        
    xOut = (2.0 / (radius(x,y) +1)) * y; 
    yOut = (2.0 / (radius(x,y) +1)) * x;
}

inline void variationMirror(double x, double y, double &xOut, double &yOut)
{        
    xOut = -x;
    yOut = -y;
}

inline void variationSpherical(double x, double y, double &xOut, double &yOut) //+
{       
    double r = radius(x,y);
    
    xOut=(1.0/(r*r)) * x;
    yOut=(1.0/(r*r)) * y;        
}

inline void variationSwirl(double x, double y, double &xOut, double &yOut) //+
{       
    double r = radius(x,y);
    double r2 = r*r;
    
    xOut=x*sin(r2)-y*cos(r2);
    yOut=x*cos(r2)+y*sin(r2);        
}

inline void variationPolar(double x, double y, double &xOut, double &yOut)
{       
    xOut = theta(x,y) / M_PI;
    yOut = radius(x,y)-1.0;
}

inline void variationHandkerchief(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = r*sin(th+r);
    yOut = r*cos(th-r);
}

