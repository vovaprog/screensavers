#pragma once

#include <memory>
#include <cmath>
#include <iostream>

#ifdef DEBUG_LOOKUP_TABLE
#   include <SimpleProfiler.h>
#endif

inline double absSin(double x)
{
    return -x + M_PI;
}

inline double absCos(double x)
{
    return -x;
}

template <double (*function)(double x), double (*absFunction)(double x),int tableSize>
class LookupTable{
public:    
    LookupTable(double precalcSegment):
        precalcSegment(precalcSegment),
        table(new double[tableSize])
    {
        double x=0.0;
        double dX=precalcSegment/tableSize;
        
        for(int i=0;i<tableSize;++i)
        {
            table[i]=function(x);
            x+=dX;
        }
        
        toTableIndexKoef=(double)tableSize / precalcSegment;        
    }

    inline double f(double xArg)
    {
        double x = toRange(xArg);    
        
        int index = toTableIndex(x);

        if(index<0 || index>=tableSize) 
        {
            //std::cout <<"LookupTable invalid index for argument: "<<x<<std::endl;
            return table[0];
        }
        
        return table[index];    
    }    
    
private:

    inline double toRange(double x)
    {    
        if(x<0.0)
        {            
            x = absFunction(x);
        }
        
        if(x<=precalcSegment) 
        {
            return x;
        }
        
        return fmod(x, 2.0*M_PI);
    }

    inline int toTableIndex(double x)
    {
        return (int)(x * toTableIndexKoef);
    }
        
    double precalcSegment;
    double toTableIndexKoef;
    std::unique_ptr<double[]> table;
    
    
#ifdef DEBUG_LOOKUP_TABLE
public:    
    void correctnessTest()
    {
        using std::cout;
        using std::endl;
        using std::string;
        
        double x=0.0;
        
        for(int i=0;i<10000;i++)
        {
            double funResult=function(x);
            double lookupResult=f(x);
            
            double dif = fabs(funResult-lookupResult);
                        
            cout <<"x: " <<x<<endl;
            cout <<"function: "<<funResult<<endl;
            cout <<"lookup: "<<lookupResult<<endl;
            cout <<"dif: " <<dif<<endl;
            
            if(dif>0.001)
            {            
                throw string("invalid sin!");
            }
            
            x+=0.0011;
        }
    }
    
    void speedTest()
    {          	                        
        double x=0.0;
        double acc=0.0;
    
        //====================================
    
        {
            SimpleProfiler prof("function");
        
            x=0.0;
    
            for(int i=0;i<10000000;i++)
            {        
                acc+=function(x);                
                x+=0.011;
            }
        }
                
        //====================================
        
        {
            SimpleProfiler prof("lookup: ");
    
            x=0.0;
        
            for(int i=0;i<10000000;i++)
            {        
                acc+=f(x);                
                x+=0.011;
            }

        }
        
        std::cout <<acc<<std::endl;
        
        //====================================
    }    
#endif    
};

typedef LookupTable<std::sin,absSin,300000> SinLookupTable;

typedef LookupTable<std::cos,absCos,300000> CosLookupTable;

