#include <iostream>
#include <stdio.h>

#include <GL/freeglut.h>

#include <ConstantFps.h>
#include <MovingValue.h>
#include <ColorMap.h>
//#include <fps.h>
#include "julia_opencl.h"

using namespace std;


#define CONSTANT_FPS_VALUE 35
#define ESCAPE_KEY 27

static unsigned int pictureWidth=600, pictureHeight=600;
static int window;

static bool isFullScreen=true;

static MovingValue koefReD(0.000113,0.00000323,-0.80,-0.79,0.000113,-0.8);	
static MovingValue koefImD(0.000099,0.00000245,0.1535,0.158,0.000099,0.156);
static MovingValue movingHighBound(0.005,0.00008,0.4,1.4,-0.003,1.5);

static ColorMapData clrMapData;
static unsigned int *colorMap;


static ConstantFps constFps(CONSTANT_FPS_VALUE);

static unsigned int* output=0;

static double xLowerBound = -1.0,xUpperBound = 1.0,yLowerBound = -1.0,yUpperBound = 1.0;

//static struct fps_counter fpsc;

static void initColorMap()
{
	vector<ColorMapPoint> Points;
	ColorMapPoint p;

	p.Index = 0;
	p.SetColor(0x00000000);	
	Points.push_back(p);	

	p.Index = 40;
	p.SetColor(0x00ff00);	
	Points.push_back(p);

	p.Index = 80;
	p.SetColor(0x00ffff);	
	Points.push_back(p);

	p.Index = ITERATION_LIMIT;
	p.SetColor(0xffff00);
	Points.push_back(p);
	
	clrMapData.InitColorMap(ITERATION_LIMIT, Points, 150);
	colorMap = clrMapData.GetColorMap();  
}

static void keyPressed(unsigned char key, int x, int y) 
{	
	if(key==ESCAPE_KEY) 
	{	    
		//stop_fps_counter(&fpsc);
		//printf("fps: %f\n",fpsc.fps);	    
	    
		glutDestroyWindow(window); 			

		glutLeaveMainLoop();
	}
}

static void prepareOutput(unsigned int *output)
{
    int pictureSize = pictureWidth * pictureHeight;
    int pictureSizeDiv2 =  pictureSize >> 1;
    
    for(int i=0;i < pictureSizeDiv2;i++)
    {
        output[i] = colorMap[output[i]];
        output[pictureSize-i-1] = output[i];
    }
}

static void display()
{    
    calculateJuliaOpenCl(
        output,
        pictureWidth, pictureHeight / 2,
        koefReD.step(), koefImD.step(), 
        movingHighBound.step()
        );
    
    prepareOutput(output);
    
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDrawPixels(pictureWidth, pictureHeight, GL_BGRA, GL_UNSIGNED_BYTE, output);
	
	glutSwapBuffers();	  
	
	clrMapData.ColorMapStep();
	
	constFps.step();		
	
	//fpsc.frame_count++;
}

static void resizeGLScene(int Width,int Height)
{
	glViewport(0,0,Width,Height);
}


static void startScreensaver(int argc, char **argv)
{
    glutInit(&argc,argv);
    

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	

    //====================================================================
    //====================================================================
    //====================================================================

    unsigned int screenResolutionWidth = glutGet(GLUT_SCREEN_WIDTH);
    unsigned int screenResolutionHeight = glutGet(GLUT_SCREEN_HEIGHT);
        
    if(isFullScreen)
    {
        pictureWidth = glutGet(GLUT_SCREEN_WIDTH);
        pictureHeight = glutGet(GLUT_SCREEN_HEIGHT);
    }

    output = new unsigned int[pictureWidth * pictureHeight];

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

    window = glutCreateWindow("opengl");	

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
    
    initColorMap();    
    
    
    initJuliaOpenCl(pictureWidth, pictureHeight / 2);
    
    //start_fps_counter(&fpsc);
    
    glutMainLoop();	    
}

int main(int argc, char **argv)
{
	try{	    
		string mode, fileName;
		int iterations,numberOfPreviews;
		bool windowMode;

		startScreensaver(argc,argv);
            
        return 0;
		
	}catch(string ex){
		cout << "exception thrown: " << ex <<endl;
	}
}

