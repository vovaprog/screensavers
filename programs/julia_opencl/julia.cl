
#define ITERATION_LIMIT 160
#define CHECK_DISTANCE 50.0f
#define CHECK_DISTANCE_SQUARE (CHECK_DISTANCE * CHECK_DISTANCE)


unsigned int iterFunJulia(float re,float im,float koefRe,float koefIm)
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

void getStartEndIndexes(int threadIndex, int outputSize, int totalNumberOfThreads,int *startIndex, int *endIndex)
{
    int slice = (outputSize / totalNumberOfThreads) + 1;
    
    *startIndex = threadIndex * slice;
    *endIndex = *startIndex + slice;
    if(*endIndex > outputSize)
    {
        *endIndex = outputSize;
    }    
}

void screenIndexToMathCoord(
    int index, 
    int width, int height, 
    float reLowBound, float reHighBound, float imLowBound, float imHighBound, 
    float *re, float *im, float *dRe, float *dIm, int *rowIndex)
{    
    *dIm = (imHighBound - imLowBound) / height;
    *dRe = (reHighBound - reLowBound) / width;
    
    *rowIndex = index % width;
    
    *im = imLowBound + (index / width) * (*dIm);
    *re = reLowBound + (*rowIndex) * (*dRe);    
}

__kernel void juliaKernel(__global unsigned int *output, int width, int height,float koefRe,float koefIm, float bound)
{
    int totalNumberOfThreads = get_global_size(0);
    
	int threadIndex = get_global_id(0);

	int startIndex, endIndex;
	
	int outputSize = width * height;
	
	getStartEndIndexes(threadIndex,outputSize,totalNumberOfThreads,&startIndex,&endIndex);

	float re, im, dRe, dIm;
	int rowIndex;
	screenIndexToMathCoord(startIndex, width, height, -bound, bound, -bound, 0.0, &re, &im, &dRe, &dIm, &rowIndex);
	
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


