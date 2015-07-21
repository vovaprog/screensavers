#pragma once


void fractalInit(int argPictureWidth, int argPictureHeight);


unsigned int* fractalStep();

void fractalPreview();

void fractalRender(const char *fileName);
