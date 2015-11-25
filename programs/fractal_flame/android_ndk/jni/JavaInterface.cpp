#include <jni.h>
#include <stdlib.h>


#include <ScreensaverAutomatPool.h>
static ScreensaverAutomatPool *screensaver;
static const int NUMBER_OF_THREADS = 2;


static int pictureWidth, pictureHeight;

static const int CONSTANT_FPS_VALUE = 10;

extern "C"
{

JNIEXPORT void JNICALL
Java_screensavers_fractalflame_FractalFlameView_fractaFlameInit(JNIEnv *env, jobject obj, int PictureWidthParameter,int PictureHeightParameter)
{
    pictureWidth = PictureWidthParameter;
    pictureHeight = PictureHeightParameter;
    
    screensaver=new ScreensaverAutomatPool(pictureWidth,pictureHeight,CONSTANT_FPS_VALUE,NUMBER_OF_THREADS);
}
    
    
JNIEXPORT void JNICALL
Java_screensavers_fractalflame_FractalFlameView_fractalFlameNextFrame(JNIEnv *env, jobject obj, jintArray arr)
{
	unsigned int *output = screensaver->nextFrame();
	
	int *javaOutput = env->GetIntArrayElements(arr,0);

    memcpy(javaOutput,output,pictureWidth*pictureHeight*sizeof(int));

	env->ReleaseIntArrayElements(arr,javaOutput,0);	
}

JNIEXPORT void JNICALL
Java_screensavers_fractalflame_FractalFlameView_fractaFlameDestroy(JNIEnv *env, jobject obj)
{
    if(screensaver!=nullptr)
    {
        delete screensaver;
    }
}

}


