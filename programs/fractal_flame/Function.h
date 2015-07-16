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
    double radius(double x, double y)
    {
        return sqrt(x*x + y*y);
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

    /*virtual void calculate(double x, double y, double &xOut, double &yOut)//horseshoe
    {
        double r = sqrt(x*x + y*y);
        
        if(r>0.01)
        {
            xOut = (1.0 / r) * ((x-y)*(x+y));
        }
        else
        {
            xOut = 0.0;
        }
        
        yOut = 2.0*x*y;
    }*/
        /*double r = sqrt(x*x + y*y);
        
        xOut = x * sin(r*r) - y * cos(r*r);
        yOut = x * cos(r*r) + y * sin(r*r);*/
