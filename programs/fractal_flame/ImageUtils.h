#pragma once

#include <memory>

/*class ImageUtils{
public:    
    void saveImage(const char *fileName, const char *fileType, const unsigned int *data, int pictureWidth, int pictureHeight);    
//private:    
    //std::unique_ptr<unsigned int[]> tempData;
    //int tempPictureSize = 0;
};*/

void saveImage(const char *fileName, const char *fileType, const unsigned int *data, int pictureWidth, int pictureHeight);

