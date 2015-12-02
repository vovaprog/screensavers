#ifndef CUDA_FRACTAL_INCLUDED
#define CUDA_FRACTAL_INCLUDED


bool cudaInitFractal(unsigned int size);

bool cudaCalculateFractal(
    unsigned int *output,
    int width, int height,
    float koefRe,float koefIm, 
    float bound,
    int numberBlocks,int threadsPerBlock);

#define ITERATION_LIMIT 160


#endif

