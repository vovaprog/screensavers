#pragma once


void fractalInit(int argPictureWidth, int argPictureHeight);

void fractalPreview(int numberOfPreviews);

void fractalRender(const char *fileName);

unsigned int* fractalScreensaver();

void fractalSetNumberOfIterations(int argNumberOfIterations);

void fractalSetStopFlag();
void fractalSetExitFlag();

void saveCurrentFractal(const char *dirName,int index);

