#pragma once

#define ITERATION_LIMIT 160

bool initJuliaOpenCl(int width, int height);

bool calculateJuliaOpenCl(unsigned int *output, int width, int height,float koefRe,float koefIm, float bound);

