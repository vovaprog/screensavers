#include <string.h>

#ifndef NO_IMAGE_FUNCTIONS
#   include <FreeImage.h>
#   include "filesystem_utils.h"
#endif

#include "ImageUtils.h"

using namespace std;

#ifndef NO_IMAGE_FUNCTIONS

void ImageUtils::saveImage(const char *fileName, const char *fileType, const unsigned int *data, int pictureWidth, int pictureHeight)
{ 
    int pictureSize = pictureWidth * pictureHeight;
    
	if(!tempData || pictureSize != tempPictureSize)
	{
		tempData=unique_ptr<unsigned int[]>(new unsigned int[pictureSize]);
		tempPictureSize =  pictureSize;
	}
	
	for(int i=0;i<pictureSize;i++)
	{
		tempData[i]=(0xff000000 | ((data[i] & 0xff) << 16) | (data[i] & 0xff00) | ((data[i] & 0xff0000) >>16)); 
	}
	
	FIBITMAP* Image = FreeImage_ConvertFromRawBits((BYTE*)tempData.get(), pictureWidth, pictureHeight, pictureWidth * sizeof(unsigned int), 32, 
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
