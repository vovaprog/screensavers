#include <stdio.h>

#include "CudaFractal.h"


static unsigned int *devOutput;

#define CHECK_DISTANCE 50.0f
#define CHECK_DISTANCE_SQUARE (CHECK_DISTANCE * CHECK_DISTANCE)


inline __device__ unsigned int iterFunJulia(float re,float im,float koefRe,float koefIm)
{
	int iter = 0, tempIter = 0;
	float tempRe, tempIm;

	while(
		(re*re+im*im < CHECK_DISTANCE_SQUARE)
		&& 
		(tempIter+=1)<ITERATION_LIMIT)
	{
		iter = tempIter;		

		tempRe = re*re - im*im + koefRe;
		tempIm = 2.0f*re*im + koefIm;
		
		re = tempRe;
		im = tempIm;
	}

	return iter;
}

inline __device__ void getStartEndIndexes(int threadIndex, int outputSize, int totalNumberOfThreads,int &startIndex, int &endIndex)
{
    int slice = (outputSize / totalNumberOfThreads) + 1;
    
    startIndex = threadIndex * slice;
    endIndex = startIndex + slice;
    if(endIndex > outputSize)
    {
        endIndex = outputSize;
    }    
}

inline __device__ void screenIndexToMathCoord(
    int index, 
    int width, int height, 
    float reLowBound, float reHighBound, float imLowBound, float imHighBound, 
    float &re, float &im, float &dRe, float &dIm, int &rowIndex)
{    
    dIm = (imHighBound - imLowBound) / height;
    dRe = (reHighBound - reLowBound) / width;
    
    rowIndex = index % width;
    
    im = imLowBound + (index / width) * dIm;
    re = reLowBound + (rowIndex) * dRe;    
}

__global__ void deviceFractalFunction(unsigned int *output, int width, int height,float koefRe,float koefIm, float bound, int totalNumberOfThreads)
{
	int threadIndex = blockIdx.x * blockDim.x + threadIdx.x;

	int startIndex, endIndex;
	
	int outputSize = width * height;
	
	getStartEndIndexes(threadIndex,outputSize,totalNumberOfThreads,startIndex,endIndex);

	float re, im, dRe, dIm;
	int rowIndex;
	screenIndexToMathCoord(startIndex, width, height, -bound, bound, -bound, 0.0, re, im, dRe, dIm, rowIndex);
	
	for(int i=startIndex;i<endIndex;i++)
	{
		unsigned int iter = iterFunJulia(re,im,koefRe,koefIm);

		output[i]=iter;
		
		if(rowIndex>=width - 1)
		{		    
		    re = -bound;
		    im += dIm;
		    rowIndex=0;
		}
		else
		{
		    re += dRe;
		    rowIndex++;
		}		
	}
}

bool cudaCalculateFractal(
    unsigned int *output,
    int width, int height,
    float koefRe,float koefIm, 
    float bound,
    int numberBlocks,int threadsPerBlock)
{
	cudaError_t cudaStatus=cudaSuccess;

    int totalNumberOfThreads = numberBlocks * threadsPerBlock;
        
	deviceFractalFunction<<<numberBlocks, threadsPerBlock>>>(
	    devOutput,
	    width,height,
	    koefRe, koefIm,
	    bound,
	    totalNumberOfThreads
	    );

    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) 
    {
        fprintf(stderr, "cuda error: %s\r\n", cudaGetErrorString(cudaStatus));
        return false;
    }
    
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaDeviceSynchronize error: %d\r\n", (int)cudaStatus);
        return false;
    }	
	
        
    cudaStatus = cudaMemcpy(output, devOutput, width * height * sizeof(unsigned int), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) 
	{
		fprintf(stderr, "cudaMemcpy error: %d\r\n", (int)cudaStatus);
		return false;
	}
    
	return true;
}

bool cudaInitFractal(unsigned int size)
{
	cudaError_t cudaStatus;
    
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice error: %d\r\n", (int)cudaStatus);
        return false;
    }

    cudaStatus = cudaMalloc((void**)&devOutput, size * sizeof(unsigned int));
	if (cudaStatus != cudaSuccess) 
	{
		fprintf(stderr, "cudaMalloc error: %d\r\n", (int)cudaStatus);
		return false;
	}
	
	return true;
}

