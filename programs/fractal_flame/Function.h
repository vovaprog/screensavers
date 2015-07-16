#pragma once

#include <cmath>

using namespace std;

class Function{
public:        
    int probabilityWeight;
    int probabilityUpBorder;
    
    unsigned int r,g,b;
    
    Function():probabilityWeight(1.0),r(255),g(255),b(255){}
        
    virtual void calculate(double x, double y, double &xOut, double &yOut)=0;
    
protected:
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
};

class FunctionSin: public Function{
public:
    FunctionSin()
    {
        r=200;
    }
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {
        xOut=sin(x);
        yOut=sin(y);
    }    
};

class Function2:public Function{
public:
    Function2()
    {
        g=200;
    }
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {        
        xOut = cos(x+y);
        yOut = cos(x-y);
    }    

};

class Function3:public Function{
public:
    Function3()
    {
        b=200;
    }
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {        
        xOut = cos(x+y) * cos(x-y);
        yOut = sin(x+y)/* * cos(x*x)*/;
    }    
};

class Function4:public Function{
public:
    Function4()
    {
        r=200;
        b=200;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {        
        xOut = (x + y) /3;
        //yOut = y * 0.8;
        
        //y = y* y;
        
        if(y>0)
        {
            yOut = sqrt(y);
        }
        else
        {
            //y = atan(y);
            y = y* y;
            //cout <<"h";
            //yOut = sqrt(-y); //y*0.8; //
        }
        //yOut = (2*x + 2*y) /6;
    }    
};


class FunctionFisheye:public Function{
public:
    FunctionFisheye()
    {
        g=200;
        b=200;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {        
        xOut = (2.0 / (radius(x,y) +1)) * y; 
        yOut = (2.0 / (radius(x,y) +1)) * x;
    }
};


class FunctionMirror:public Function{
public:
    FunctionMirror()
    {
        r=220;
        b=180;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {        
        xOut = -x;
        yOut = -y;
    }
};

class FunctionSpherical:public Function{
public:
    FunctionSpherical()
    {     
        g=220;
        b=180;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r = radius(x,y);
        
        xOut=(1.0/(r*r)) * x;
        yOut=(1.0/(r*r)) * y;        
    }
};

class FunctionSwirl:public Function{
public:
    FunctionSwirl()
    {     
        r=220;
        g=180;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r = radius(x,y);
        double r2 = r*r;
        
        xOut=x*sin(r2)-y*cos(r2);
        yOut=x*cos(r2)+y*sin(r2);        
    }
};

class FunctionHorseshoe:public Function{
public:
    FunctionHorseshoe()
    {     
        r=150;
        g=200;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r = radius(x,y);
        
        xOut=(1.0/r)*(x-y)*(x+y);
        yOut=(1.0/r)*2.0*x*y;        
    }
};

class FunctionPolar:public Function{
public:
    FunctionPolar()
    {     
        r=150;
        b=220;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        xOut = theta(x,y) / M_PI;
        yOut = radius(x,y)-1.0;
    }
};

class FunctionHandkerchief:public Function{
public:
    FunctionHandkerchief()
    {     
        r=200;
        g=200;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        
        xOut = r*sin(th+r);
        yOut = r*cos(th-r);
    }
};

class FunctionHeart:public Function{
public:
    FunctionHeart()
    {     
        g=230;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        
        xOut = r*sin(th*r);
        yOut = r*(-cos(th*r));
    }
};

class FunctionDisk:public Function{
public:
    FunctionDisk()
    {     
        r=230;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        
        xOut = (th/M_PI) * sin(M_PI*r);
        yOut = (th/M_PI) * cos(M_PI*r);
    }
};

class FunctionSpiral:public Function{
public:
    FunctionSpiral()
    {     
        b=230;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        
        xOut = (1.0/r) * (cos(th)+sin(r));
        yOut = (1.0/r) * (sin(th)-cos(r));
    }
};

class FunctionHyperbolic:public Function{
public:
    FunctionHyperbolic()
    {     
        r=190;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        
        xOut = sin(th)/r;
        yOut = r*cos(th);
    }
};

class FunctionDiamond:public Function{
public:
    FunctionDiamond()
    {     
        g=190;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        
        xOut = sin(th)*cos(r);
        yOut = cos(th)*sin(r);
    }
};

class FunctionJulia:public Function{
public:
    FunctionJulia()
    {     
        b=190;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
    {       
        double r=radius(x,y);
        double th=theta(x,y);
        double om=omega();
        
        xOut = sqrt(r)*cos(th/2+om);
        yOut = sqrt(r)*sin(th/2+om);
    }
};

class FunctionEx:public Function{
public:
    FunctionEx()
    {
        r=210;
        b=190;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
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
};

class FunctionBent:public Function{
public:
    FunctionBent()
    {
        r=190;
        b=210;
    }    
    
    virtual void calculate(double x, double y, double &xOut, double &yOut)
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
};
