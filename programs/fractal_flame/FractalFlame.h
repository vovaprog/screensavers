#pragma once


enum class CalculateFractalResult { SUCCESS, BAD_PICTURE, TIMEOUT };


void fractalInit(int argPictureWidth, int argPictureHeight);

void fractalPreview(int numberOfPreviews);

void fractalRender(const char *fileName);

CalculateFractalResult fractalScreensaver(unsigned int **ppOutput);

void fractalSetNumberOfIterations(int argNumberOfIterations);

void fractalSetStopFlag();
void fractalSetExitFlag();

void saveCurrentFractal(const char *dirName,int index);

