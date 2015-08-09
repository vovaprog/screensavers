#include <iostream>
#include <string>

#include <GL/freeglut.h>

#include <boost/program_options.hpp>

#include <ConstantFps.h>
#include "FractalFlame.h"
#include "ScreensaverAutomat.h"

using namespace std;
using namespace boost::program_options;

#define CONSTANT_FPS_VALUE 30
#define ESCAPE_KEY 27

static unsigned int pictureWidth=600, pictureHeight=600;
static int window;

static bool isFullScreen=true;

static ScreensaverAutomat *screensaver;

static ConstantFps constFps(CONSTANT_FPS_VALUE);

static unsigned int* output=0;


static void keyPressed(unsigned char key, int x, int y) 
{	
	if(key==ESCAPE_KEY) 
	{
	    delete screensaver;
	    
		glutDestroyWindow(window); 			

		glutLeaveMainLoop();
	}
}

static void display()
{    
    output=screensaver->nextFrame();    

	glClear(GL_COLOR_BUFFER_BIT);
	
	glDrawPixels(pictureWidth, pictureHeight, GL_RGBA, GL_UNSIGNED_BYTE, output);
	
	glutSwapBuffers();	  
	
	constFps.step();		
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

    screensaver=new ScreensaverAutomat(pictureWidth,pictureHeight,CONSTANT_FPS_VALUE);
    
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
    
    glutMainLoop();	    
}

static void startRender(const string &fileName, int numberOfIterations)
{
    FractalFlame fractal;
    
    fractal.init(pictureWidth,pictureHeight);
    
    fractal.setNumberOfIterations(numberOfIterations);
    
    fractal.render(fileName.c_str());
}

static void startPreview(int numberOfPreviews)
{
    FractalFlame fractal;
    
    fractal.init(pictureWidth,pictureHeight);
    
    fractal.setNumberOfIterations(300000);
    
    fractal.preview(numberOfPreviews);    
}

int main(int argc, char **argv)
{
	try{	    
		string mode, fileName;
		int iterations,numberOfPreviews;
		bool windowMode;
		
		options_description desc("program options");
        desc.add_options()
            ("help,h", "print help message")
            ("mode", value(&mode)->default_value("screensaver"), "program mode (screensaver, render, preview)")
            ("width",value(&pictureWidth)->default_value(600),"width")
            ("height",value(&pictureHeight)->default_value(600),"height")
            ("iterations",value(&iterations)->default_value(30000000),"number of iterations for render mode")
            ("input-file",value(&fileName),"file with fractal parameters (for render mode)")
            ("number-of-previews", value(&numberOfPreviews)->default_value(30),"number of previews (for preview mode)")
            ("window",bool_switch(&windowMode), "show in window mode")
            ;
        
        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);    
        
        if (vm.count("help")) 
        {
            cout << desc << endl;
            return 0;
        }        
                
        isFullScreen = !windowMode;
        
        if(mode == "screensaver")
        {
            startScreensaver(argc,argv);
        }
        else if(mode=="render")
        {
            if(fileName.size()==0)
            {
                cout <<"input-file parameter not specified!"<<endl;
                return -1;
            }

            startRender(fileName, iterations);
        }
        else if(mode=="preview")
        {
            startPreview(numberOfPreviews);
        }
        else
        {
            cout <<"unknown mode!"<<endl;
            return -1;
        }
            
        return 0;
		
	}catch(string ex){
		cout << "exception thrown: " << ex <<endl;
	}
}
