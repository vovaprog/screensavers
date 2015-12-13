#include <iostream>
#include <string>
#include <unistd.h>

#include <GL/freeglut.h>

#include <boost/program_options.hpp>

#include <ConstantFps.h>


#include "ScreensaverAutomatPool.h" 

#include "FractalFlame.h"

using namespace std;
using namespace boost::program_options;

#define CONSTANT_FPS_VALUE 30
#define ESCAPE_KEY 27

static unsigned int pictureWidth=600, pictureHeight=600;
static int window;

static bool isFullScreen=true;


static ScreensaverAutomatPool *screensaver;


static ConstantFps constFps(CONSTANT_FPS_VALUE);

static unsigned int* output=0;

static double xLowerBound = -1.0,xUpperBound = 1.0,yLowerBound = -1.0,yUpperBound = 1.0;

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
	
	glDrawPixels(pictureWidth, pictureHeight, GL_BGRA, GL_UNSIGNED_BYTE, output);
	
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

    
    screensaver=new ScreensaverAutomatPool(pictureWidth,pictureHeight,CONSTANT_FPS_VALUE);
    

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

#ifndef NO_IMAGE_FUNCTIONS

static void startRender(const string &fileName, int numberOfIterations)
{        
    FractalFlame fractal;
    
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=pictureWidth;
    rp->pictureHeight=pictureHeight;
    rp->numberOfIterations=numberOfIterations;    
    
    fractal.render(rp,fileName.c_str());
}

static void startPreview(int numberOfPreviews, int numberOfIterations)
{    
    FractalFlame fractal;

    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=pictureWidth;
    rp->pictureHeight=pictureHeight;
    rp->numberOfIterations=numberOfIterations;    

    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->xLowerBound=xLowerBound;
    fp->xUpperBound=xUpperBound;
    fp->yLowerBound=yLowerBound;
    fp->yUpperBound=yUpperBound;
    
    fractal.preview(numberOfPreviews,rp,fp);
}

static void* fortuneThreadFunction(void *dataInput)
{
    FractalFlame *fractal = (FractalFlame *)dataInput;
    
    unsigned int* output;
    fractal->screensaver(&output);
    
    return nullptr;    
}

static int startFortune(string &outputFileName)
{   
    FractalFlame fractal;

    fractal.screensaverInit(pictureWidth,pictureHeight);
    
    ThreadPool pool(1);

    pool.runTask(fortuneThreadFunction,&fractal);

    sleep(3);
    
    fractal.setStopFlag();
    
    pool.getResult();
    
    if(outputFileName.size() < 1)
    {
        outputFileName = "./fortune";
    }
    
    if(fractal.screensaverSaveCurrentFractal(outputFileName.c_str())==FractalFlameAlgorithm::CalculateFractalResult::SUCCESS)
    {
        return 0;
    }
    else
    {
        return -1;
    }    
}

#endif

int main(int argc, char **argv)
{
	try{	    
		string mode, fileName, outputFileName;
		int iterations,numberOfPreviews;
		bool windowMode;

		{
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
                ("x-lower-bound",value(&xLowerBound)->default_value(-1.0),"x lower bound")
                ("x-upper-bound",value(&xUpperBound)->default_value(1.0),"x upper bound")
                ("y-lower-bound",value(&yLowerBound)->default_value(-1.0),"y lower bound")
                ("y-upper-bound",value(&yUpperBound)->default_value(1.0),"y upper bound")
                ("output-file",value(&outputFileName),"output file name")
                ;
            
            variables_map vm;
            store(parse_command_line(argc, argv, desc), vm);
            notify(vm);    
            
            if (vm.count("help")) 
            {
                cout << desc << endl;
                return 0;
            }
        }
                
        isFullScreen = !windowMode;
        
        
        FractalFlame::initFlameLibrary();
        
        if(mode == "screensaver")
        {
            startScreensaver(argc,argv);
        }
#ifndef NO_IMAGE_FUNCTIONS        
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
            startPreview(numberOfPreviews,1000000);
        }
#endif      
        else if(mode=="convert_old")
        {
            FlameParameters fp;
            fp.load_old(fileName.c_str());
            fp.save((fileName+"_converted.xml").c_str());
        }
        else if(mode=="fortune")
        {
            return startFortune(outputFileName);
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
