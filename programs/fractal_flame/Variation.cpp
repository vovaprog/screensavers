#include <cstdlib>
#include <algorithm>
#include <string.h>

#include <cmath>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#include "Variation.h"

using namespace std;


#ifdef USE_LOOKUP

#include <LookupTable.h>

SinLookupTable sinLookup(70.0);
CosLookupTable cosLookup(70.0);

#define SIN(x) sinLookup.f(x)
#define COS(x) cosLookup.f(x)

#else

#define SIN(x) std::sin(x)
#define COS(x) std::cos(x)

#endif



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
    xOut=SIN(x);
    yOut=SIN(y);
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
        
    xOut=x*SIN(r2)-y*COS(r2);
    yOut=x*COS(r2)+y*SIN(r2);            
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
        
    xOut = r*SIN(th+r);    
    yOut = r*COS(th-r);    
}

void variationHeart(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = r*SIN(th*r);
    yOut = r*(-COS(th*r));
}

void variationDisk(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = (th/M_PI) * SIN(M_PI*r);
    yOut = (th/M_PI) * COS(M_PI*r);
}

void variationSpiral(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = (1.0/r) * (COS(th)+SIN(r));
    yOut = (1.0/r) * (SIN(th)-COS(r));
}

void variationHyperbolic(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = SIN(th)/r;
    yOut = r*COS(th);
}

void variationDiamond(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = SIN(th)*COS(r);
    yOut = COS(th)*SIN(r);    
}

void variationJulia(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    double om=omega();
    double sqrt_r = sqrt(r); 
    
    xOut = sqrt_r*COS(th/2+om);
    yOut = sqrt_r*SIN(th/2+om);
}

void variationEx(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    double p0=SIN(th+r);
    double p1=COS(th-r);
    
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
    xOut=exp(x-1)*COS(M_PI * y);
    yOut=exp(x-1)*SIN(M_PI * y);
}

void variationPower(double x, double y, double &xOut, double &yOut)
{
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut=pow(r,SIN(th)) * COS(th);
    yOut=pow(r,SIN(th)) * SIN(th);    
}

void variationCosine(double x, double y, double &xOut, double &yOut) //-
{    
    xOut=COS(M_PI*x) * cosh(y);
    yOut=-SIN(M_PI*x) * sinh(y);    
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
    xOut = SIN(x);
    yOut = y;
}

void variationNoise(double x, double y, double &xOut, double &yOut)
{
    double psi1=psi();
    double psi2=psi();
        
    xOut = psi1 * x * COS(2.0 * M_PI * psi2);
    yOut = psi1 * y * SIN(2.0 * M_PI * psi2);
}

void variationBlur(double x, double y, double &xOut, double &yOut)
{
    double psi1=psi();
    double psi2=psi();
    
    xOut = psi1 * COS(2.0 * M_PI * psi2);
    yOut = psi1 * SIN(2.0 * M_PI * psi2);
}

void variationGaussian(double x, double y, double &xOut, double &yOut)
{
    double psi_sum = psi() + psi() + psi() + psi() - 2.0;
    double psi5=psi();
        
    xOut = psi_sum * COS(2.0 * M_PI * psi5);
    yOut = psi_sum * SIN(2.0 * M_PI * psi5);
}

void variationTangent(double x, double y, double &xOut, double &yOut)
{
    xOut = SIN(x) / COS(y);
    yOut = tan(y);    
}


static vector<Variation> variations; 


void initVariations()
{
    Variation v;
    
    v.name="sinusoidal";
    v.f=variationSin;
    variations.push_back(v);    
    
    v.name="eyefish";
    v.f=variationFisheye;
    variations.push_back(v);    
    
    v.name="spherical";
    v.f=variationSpherical;
    variations.push_back(v);    

    v.name="swirl";
    v.f=variationSwirl;
    variations.push_back(v);    

    v.name="horseshoe";
    v.f=variationHorseshoe;
    variations.push_back(v);    

    v.name="polar";
    v.f=variationPolar;
    variations.push_back(v);    

    v.name="handkerchief";
    v.f=variationHandkerchief;
    variations.push_back(v);    

    v.name="heart";
    v.f=variationHeart;
    variations.push_back(v);    

    v.name="disk";
    v.f=variationDisk;
    variations.push_back(v);    

    v.name="spiral";
    v.f=variationSpiral;
    variations.push_back(v);    

    v.name="hyperbolic";
    v.f=variationHyperbolic;
    variations.push_back(v);    
    
    v.name="diamond";
    v.f=variationDiamond;
    variations.push_back(v);    

    v.name="julia";
    v.f=variationJulia;
    variations.push_back(v);    

    v.name="ex";
    v.f=variationEx;
    variations.push_back(v);    
    
    v.name="bent";
    v.f=variationBent;
    variations.push_back(v);           

    v.name="mirror";
    v.f=variationMirror;
    variations.push_back(v);    

    v.name="power";
    v.f=variationPower;
    variations.push_back(v);    

    v.name="bubble";
    v.f=variationBubble;
    variations.push_back(v);    

    v.name="cylinder";
    v.f=variationCylinder;
    variations.push_back(v);    

    v.name="tangent";
    v.f=variationTangent;
    variations.push_back(v);    

    v.name="noise";
    v.f=variationNoise;
    variations.push_back(v);    

    v.name="blur";
    v.f=variationBlur;
    variations.push_back(v);    

    v.name="gaussian";
    v.f=variationGaussian;
    variations.push_back(v);    

    v.name="exponential";
    v.f=variationExponential;
    variations.push_back(v);    

    v.name="cosine";
    v.f=variationCosine;
    variations.push_back(v);    
}

vector<Variation>& getVariations()
{
    return variations;
}

Variation getVariationByName(const char *name)
{       
    auto iter = find_if(variations.begin(),variations.end(),[name](const Variation& v)
        {
            return (strcmp(name,v.name)==0);                
        });
    
    if(iter!=variations.end())
    {
        return *iter;
    }
    
    Variation nullVar;
    nullVar.name=nullptr;
    nullVar.f=nullptr;
    
    return nullVar;
}
