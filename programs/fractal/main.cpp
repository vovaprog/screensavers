#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <stdio.h>

#include <vectorf128.h>

#include <fps.h>

#include "fractal.h"
#include <ConstantFps.h>

using namespace std;

#define PICTURE_WIDTH_BIG 1024
#define PICTURE_HEIGHT_BIG 768

#define PICTURE_WIDTH_SMALL 800
#define PICTURE_HEIGHT_SMALL 600


static bool isFullScreen=true;
static bool useAllScreen=false;

static int pictureWidth=PICTURE_WIDTH_SMALL;
static int pictureHeight=PICTURE_HEIGHT_SMALL;

static int window;
static struct fps_counter fpsc;

#define CONSTANT_FPS_VALUE 35

static ConstantFps constFps(CONSTANT_FPS_VALUE);

static int startupCounter=0;

static void drawGLScene()
{
	unsigned int* output=fractalStep();	

	glClear(GL_COLOR_BUFFER_BIT);
	
	if(startupCounter<0)
	{
		glDrawPixels(pictureWidth, pictureHeight, GL_RGBA, GL_UNSIGNED_BYTE, output);		
	}
	else if(++startupCounter>=CONSTANT_FPS_VALUE)
	{
		startupCounter=-1;
	}		

	glutSwapBuffers();
	
	constFps.step();
	
	fpsc.frame_count++;
}


static void resizeGLScene(int Width,int Height)
{
	glViewport(0,0,Width,Height);

	if(isFullScreen && !useAllScreen)
	{
		float w = (float)pictureWidth * 2.0/(float)glutGet(GLUT_SCREEN_WIDTH);
		float h = (float)pictureHeight * 2.0/(float)glutGet(GLUT_SCREEN_HEIGHT);

		glRasterPos2f(-1+(2-w)/2,-1+(2-h)/2);
	}
}


#define ESCAPE 27
static void keyPressed(unsigned char key, int x, int y) 
{	
	if(key==ESCAPE) 
	{ 			
		stop_fps_counter(&fpsc);
		printf("fps: %f\n",fpsc.fps);

		glutDestroyWindow(window); 			

		glutLeaveMainLoop();
	}
}


int main(int argc, char* argv[])
{	
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	

	//====================================================================
	//====================================================================
	//====================================================================

	int screenResolutionWidth= glutGet(GLUT_SCREEN_WIDTH);
	int screenResolutionHeight= glutGet(GLUT_SCREEN_HEIGHT);

	//====================================================================
	//====================================================================
	//====================================================================

	if(isFullScreen)
	{
		if(useAllScreen)
		{
			pictureWidth=screenResolutionWidth;
			pictureHeight=screenResolutionHeight;
		}
		else
		{
			if(screenResolutionWidth>=PICTURE_WIDTH_BIG && screenResolutionHeight>=PICTURE_HEIGHT_BIG)
			{
				pictureWidth=PICTURE_WIDTH_BIG;
				pictureHeight=PICTURE_HEIGHT_BIG;
			}
			else if(screenResolutionWidth>=PICTURE_WIDTH_SMALL && screenResolutionHeight>=PICTURE_HEIGHT_SMALL)
			{
				pictureWidth=PICTURE_WIDTH_SMALL;
				pictureHeight=PICTURE_HEIGHT_SMALL;
			}
			else
			{
				pictureWidth=screenResolutionWidth;
				pictureHeight=screenResolutionHeight;
			}
		}
	}
	else
	{
		pictureWidth=PICTURE_WIDTH_SMALL;
		pictureHeight=PICTURE_HEIGHT_SMALL;
	}

	fractalInit(pictureWidth,pictureHeight);
	
	//====================================================================
	//====================================================================
	//====================================================================

	glutInitWindowSize(pictureWidth,pictureHeight);  

	if(isFullScreen)
	{
		glutInitWindowPosition(0, 0); 
	}
	else
	{	
		glutInitWindowPosition((screenResolutionWidth - pictureWidth)/2, (screenResolutionHeight-pictureHeight)/2); 
	}

	//====================================================================
	//====================================================================
	//====================================================================

	window=glutCreateWindow("opengl");	

	glutDisplayFunc(&drawGLScene);  

	glutIdleFunc(&drawGLScene);

	glutReshapeFunc(&resizeGLScene);

	glutKeyboardFunc(&keyPressed);

	//====================================================================
	//====================================================================
	//====================================================================

	if(isFullScreen)
	{
		glutFullScreen();
	}

	//====================================================================
	//====================================================================
	//====================================================================

	start_fps_counter(&fpsc);

	glutMainLoop();	

	return 0;
}

