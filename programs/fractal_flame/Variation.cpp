#include <cstdlib>

#include <cmath>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#include <Variation.h>

using namespace std;


static inline double radius(double x, double y)
{
    return sqrt(x*x + y*y);
}

static inline double theta(double x, double y)
{
    return atan(x/y);    
}

static inline double fi(double x, double y)
{
    return atan(y/x);    
}

static inline double omega()
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

static inline double psi()
{
    return ((double)(rand() % 10000))/ 10000.0;
}

//=================================================================

void variationSin(double x, double y, double &xOut, double &yOut) //+
{
    xOut=sin(x);
    yOut=sin(y);
}    

void variationFisheye(double x, double y, double &xOut, double &yOut)
{         
    double r = radius(x,y);
    
    xOut = (2.0 / (r + 1.0)) * y; 
    yOut = (2.0 / (r + 1.0)) * x;
}

void variationMirror(double x, double y, double &xOut, double &yOut)
{        
    xOut = -x;
    yOut = -y;
}

void variationSpherical(double x, double y, double &xOut, double &yOut) //+
{       
    double r = radius(x,y);
    double r2 = r*r;
    
    xOut=(1.0/(r2)) * x;
    yOut=(1.0/(r2)) * y;        
}

void variationSwirl(double x, double y, double &xOut, double &yOut) //+
{       
    double r = radius(x,y);
    double r2 = r*r;
    
    xOut=x*sin(r2)-y*cos(r2);
    yOut=x*cos(r2)+y*sin(r2);        
}

void variationHorseshoe(double x, double y, double &xOut, double &yOut)
{       
    double r = radius(x,y);
    
    xOut=(1.0/r)*(x-y)*(x+y);
    yOut=(1.0/r)*2.0*x*y;        
}

void variationPolar(double x, double y, double &xOut, double &yOut)
{       
    xOut = theta(x,y) / M_PI;
    yOut = radius(x,y)-1.0;
}

void variationHandkerchief(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = r*sin(th+r);
    yOut = r*cos(th-r);
}

void variationHeart(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = r*sin(th*r);
    yOut = r*(-cos(th*r));
}

void variationDisk(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = (th/M_PI) * sin(M_PI*r);
    yOut = (th/M_PI) * cos(M_PI*r);
}

void variationSpiral(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = (1.0/r) * (cos(th)+sin(r));
    yOut = (1.0/r) * (sin(th)-cos(r));
}

void variationHyperbolic(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = sin(th)/r;
    yOut = r*cos(th);
}

void variationDiamond(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = sin(th)*cos(r);
    yOut = cos(th)*sin(r);
}

void variationJulia(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    double om=omega();
    double sqrt_r = sqrt(r); 
    
    xOut = sqrt_r*cos(th/2+om);
    yOut = sqrt_r*sin(th/2+om);
}

void variationEx(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    double p0=sin(th+r);
    double p1=cos(th-r);
    
    double p0_3=p0*p0*p0;
    double p1_3=p1*p1*p1;
    
    xOut=r*(p0_3+p1_3);
    yOut=r*(p0_3-p1_3);
}

void variationBent(double x, double y, double &xOut, double &yOut)
{       
    if(x>=0 && y>=0)
    {
        xOut=x;
        yOut=y;            
    }
    else if(x<0 && y>=0)
    {
        xOut=2*x;
        yOut=y;
    }
    else if(x>=0 && y<0)
    {
        xOut=x;
        yOut=y/2;
    }
    else
    {
        xOut=2*x;
        yOut=y/2;
    }
}

//=======================================================================

void variationExponential(double x, double y, double &xOut, double &yOut) //-
{       
    xOut=exp(x-1)*cos(M_PI * y);
    yOut=exp(x-1)*sin(M_PI * y);
}

void variationPower(double x, double y, double &xOut, double &yOut)
{
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut=pow(r,sin(th)) * cos(th);
    yOut=pow(r,sin(th)) * sin(th);
}

void variationCosine(double x, double y, double &xOut, double &yOut) //-
{
    xOut=cos(M_PI*x) * cosh(y);
    yOut=-sin(M_PI*x) * sinh(y);
}

void variationBubble(double x, double y, double &xOut, double &yOut)
{
    double r=radius(x,y);
    double r2=r*r;
    
    xOut = (4.0 / (r2+4.0)) * x;
    yOut = (4.0 / (r2+4.0)) * y;
}

void variationCylinder(double x, double y, double &xOut, double &yOut)
{
    xOut = sin(x);
    yOut = y;
}

void variationNoise(double x, double y, double &xOut, double &yOut)
{
    double psi1=psi();
    double psi2=psi();
    
    xOut = psi1 * x * cos(2.0 * M_PI * psi2);
    yOut = psi1 * y * sin(2.0 * M_PI * psi2);
}

void variationBlur(double x, double y, double &xOut, double &yOut)
{
    double psi1=psi();
    double psi2=psi();

    xOut = psi1 * cos(2.0 * M_PI * psi2);
    yOut = psi1 * sin(2.0 * M_PI * psi2);
}

void variationGaussian(double x, double y, double &xOut, double &yOut)
{
    double psi_sum = psi() + psi() + psi() + psi() - 2.0;
    double psi5=psi();
    
    xOut = psi_sum * cos(2.0 * M_PI * psi5);
    yOut = psi_sum * sin(2.0 * M_PI * psi5);
}

void variationTangent(double x, double y, double &xOut, double &yOut)
{
    xOut = sin(x) / cos(y);
    yOut = tan(y);    
}

