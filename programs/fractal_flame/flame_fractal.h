#pragma once


void fractalInit(int argPictureWidth, int argPictureHeight);


unsigned int* fractalStep();

void fractalPreview(int numberOfPreviews);

void fractalRender(const char *fileName);
