#ifndef NO_IMAGE_FUNCTIONS

#include <string.h>

#include <FreeImage.h>

#include "image_utils.h"

using namespace std;

void saveImage(const char *fileName, const char *fileType, const unsigned int *data, int pictureWidth, int pictureHeight)
{ 
	FIBITMAP* Image = FreeImage_ConvertFromRawBits((BYTE*) data, pictureWidth, pictureHeight, pictureWidth * sizeof(unsigned int), 32, 
		0xFF0000, 0x00FF00, 0x0000FF, false); 
	
	if(strcmp(fileType,"png")==0)
	{
		FreeImage_Save(FIF_PNG, Image, fileName, 0);
	}
	else
	{
		FreeImage_Save(FIF_BMP, Image, fileName, 0);
	}
	
	FreeImage_Unload(Image);
}

#endif
