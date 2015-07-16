//#include <math.h>
//#include <vector>
#include <iostream>

#include <GL/freeglut.h>

using namespace std;

#include "flame_fractal.h"

#define PICTURE_WIDTH_BIG 1024
#define PICTURE_HEIGHT_BIG 768

#define PICTURE_WIDTH_SMALL 800
#define PICTURE_HEIGHT_SMALL 500


unsigned int pictureWidth=PICTURE_WIDTH_BIG, pictureHeight=PICTURE_HEIGHT_BIG;
static int window;

static bool isFullScreen=false;
static bool useAllScreen=false;


#define ESCAPE 27
static void keyPressed(unsigned char key, int x, int y) 
{	
	if(key==ESCAPE) 
	{ 			
		glutDestroyWindow(window); 			

		glutLeaveMainLoop();
	}
}

unsigned int* output=0;

void display()
{
    if(output==0)
    {
        output=fractalStep();
        //cout <<"aaa: "<<endl;
        for(int i=0;i<100;i++)
        {
            //cout <<output[i]<<" ";    
        }
    }

	glClear(GL_COLOR_BUFFER_BIT);
	
	glDrawPixels(pictureWidth, pictureHeight, GL_RGBA, GL_UNSIGNED_BYTE, output);
	
	glutSwapBuffers();	    
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

int main( int argc, char **argv )
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

	glutDisplayFunc(&display);  

	glutIdleFunc(&display);

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

	fractalInit(pictureWidth,pictureHeight);

	glutMainLoop();	

	return 0;
}