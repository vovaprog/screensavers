#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <fps.h>
#include <const_fps.h>
#include <hilbert2d_gl.h>
#include <hilbert3d_gl.h>

static struct fps_counter fpsc;

static const_fps cnst_fps(30.0);

static hilbert2d_gl *ph2d;
static hilbert3d_gl *ph3d;

static double rot=0.0;

static int window;

static void ReSizeGLScene(int width, int height)
{
	if(height==0) height=1;
	
	glViewport(0, 0, width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

static void init_gl(int width, int height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glClearDepth(1.0f);	
	
	ReSizeGLScene(width,height);

	start_fps_counter(&fpsc);
	
	ph2d=new hilbert2d_gl(7,60);
	ph3d=new hilbert3d_gl(2,16);
	
}

static void draw_gl_scene(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	glTranslated(-30,-30,-27.0);	
	
	glColor3d(0,0.5,0);
	
	ph2d->draw();
	
	glTranslated(30,30,10);	
	
	glRotated(40,1.0,0.0,0);
	glRotated(rot,0,1.0,0);rot+=1.0;	

	glTranslated(-8,-8,8);
	
	glColor3d(1,1,1);
	
	ph3d->draw();
	
	glutSwapBuffers();	

	cnst_fps.sleep();

	fpsc.frame_count++;
}

static void on_exit_program()
{
	stop_fps_counter(&fpsc);
	printf("fps: %f\n",fpsc.fps);
}


#define ESCAPE 27
static void keyPressed(unsigned char key, int x, int y) 
{	
	if(key==ESCAPE) 
	{ 			
		on_exit_program();
		
		glutDestroyWindow(window); 			
		
		exit(0);                   
	}
}

int main(int argc, char **argv) 
{  
	unsigned int screen_resolution_x=1024,screen_resolution_y;

	screen_resolution_y=(screen_resolution_x/4)*3;

	glutInit(&argc,argv);  

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(screen_resolution_x,screen_resolution_y);  

	glutInitWindowPosition(0, 0);  

	window=glutCreateWindow("opengl");	

	glutDisplayFunc(&draw_gl_scene);  

	glutIdleFunc(&draw_gl_scene);

	glutReshapeFunc(&ReSizeGLScene);

	glutKeyboardFunc(&keyPressed);
  	
	glutFullScreen();	
		
	init_gl(screen_resolution_x,screen_resolution_y);

	glutMainLoop();

	return 0;
}


