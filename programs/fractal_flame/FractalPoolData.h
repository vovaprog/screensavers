#ifndef FRACTAL_POOL_DATA_H
#define FRACTAL_POOL_DATA_H

class FractalPoolData{
public:    
    FractalFlame *fractal;
    FractalFlame::CalculateFractalResult result;
    unsigned int *output=nullptr;
    
    FractalPoolData()
    {
        fractal=new FractalFlame();
    }
    
    ~FractalPoolData()
    {
        if(fractal!=nullptr)
        {
            delete fractal;
        }
    }
};

#endif

