#include <CL/opencl.h>
#include <stdio.h>

#include <filesystem_utils.h>

#include "julia_opencl.h"

static cl_mem buffer;
static cl_kernel kernel;
static cl_command_queue queue;


static bool openClBuild(cl_device_id device, cl_program program)
{
    cl_int err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    
	if(err != CL_SUCCESS)
	{
		char *buildLog;
		size_t buildLogSize;
		err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &buildLogSize);

		buildLog = new char[buildLogSize+1];
		err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, buildLogSize, buildLog, NULL);
		buildLog[buildLogSize] = 0;
		printf("opencl build error: %s", buildLog);
		delete[] buildLog;
		
		return false;
	}    
	
	return true;
}


bool initJuliaOpenCl(int width, int height)
{
    cl_int err;
    
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext( NULL,
            1,
            &device,
            NULL, NULL, NULL);
    
    queue = clCreateCommandQueue( context,
            device,
            0, NULL );
    
    char *source = nullptr;
    readTextFile("./julia.cl", &source);
    
    cl_program program = clCreateProgramWithSource( context,
            1,
            (const char**) &source,
            NULL, NULL );
    
    delete[] source;
    

    openClBuild(device,program);
    
    kernel = clCreateKernel( program, "juliaKernel", &err );
    
    if(kernel==NULL)
    {
        printf("clCreateKernel error %d\n",err);
        return false;
    }
    
    cl_int status;
    
    buffer = clCreateBuffer( context, CL_MEM_WRITE_ONLY, width * height * sizeof(unsigned int),NULL, &status);
    
    return true;
}


bool calculateJuliaOpenCl(unsigned int *output, int width, int height,float koefRe,float koefIm, float bound)
{
    clSetKernelArg(kernel, 0, sizeof(buffer), (void*) &buffer);
    clSetKernelArg(kernel, 1, sizeof(int), (void*) &width);
    clSetKernelArg(kernel, 2, sizeof(int), (void*) &height);
    clSetKernelArg(kernel, 3, sizeof(float), (void*) &koefRe);
    clSetKernelArg(kernel, 4, sizeof(float), (void*) &koefIm);
    clSetKernelArg(kernel, 5, sizeof(float), (void*) &bound);
    
    int outputSize = width * height * sizeof(unsigned int);
        
    size_t global_work_size = outputSize;
    
    cl_int err = clEnqueueNDRangeKernel( queue,
                            kernel,
                            1,
                            NULL,
                            &global_work_size,
                            NULL, 0, NULL, NULL);
    
    if(err != CL_SUCCESS)
    {
        printf("clEnqueueNDRangeKernel error: %d\n",err);
    }
    
    clFinish(queue);        
    
    clEnqueueReadBuffer(
        queue, buffer, CL_TRUE, 0,
        outputSize, output,
        0, NULL, NULL);
    
    return true;
}


