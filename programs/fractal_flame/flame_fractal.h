#pragma once


void fractalInit(int argPictureWidth, int argPictureHeight);

void fractalPreview(int numberOfPreviews);

void fractalRender(const char *fileName);

unsigned int* fractalRandom();

void fractalSetNumberOfIterations(int argNumberOfIterations);

void fractalSetStopFlag();

void saveCurrentFractal(const string &dirName,int index);

