#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include <shader_utils.h>
#include <ConstantFps.h>
#include <MovingValue.h>
#include <ColorMap.h>


static const int iterLimit = 160;
static const float checkDistanceSquare = 50.0 * 50.0;

static MovingValue koefReD(0.000113,0.00000323,-0.80,-0.79,0.000113,-0.8);	
static MovingValue koefImD(0.000099,0.00000245,0.1535,0.158,0.000099,0.156);
static MovingValue movingHighBound(0.005,0.00008,0.4,1.4,-0.003,1.5);

static unsigned int shaderProgram, shaderSky;

static ColorMapData clrMapData;
static unsigned int *colorMap;


static ColorMapData clrMapDataSky;
static unsigned int *colorMapSky;




static ConstantFps constFps(35);


extern "C" {
void glUseProgramObjectARB(GLhandleARB);
}

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

	p.Index = iterLimit;
	p.SetColor(0xffff00);
	Points.push_back(p);
	
	clrMapData.InitColorMap(iterLimit, Points, 150);
	colorMap = clrMapData.GetColorMap();  
	
	
	
	Points = vector<ColorMapPoint>();
	
	p.Index = 0;
	p.SetColor(0x000000ff);	
	Points.push_back(p);	

	p.Index = 40;
	p.SetColor(0x3333ff);	
	Points.push_back(p);

	p.Index = 80;
	p.SetColor(0x9999ff);	
	Points.push_back(p);

	p.Index = iterLimit;
	p.SetColor(0xffffff);
	Points.push_back(p);
	
	clrMapDataSky.InitColorMap(256, Points, 150);
	colorMapSky = clrMapDataSky.GetColorMap();  
}

static MovingValue xAngle(0.055,0.001,-5,5,0.055,0.0);
static MovingValue yAngle(0.045,0.001,-5,5,0.045,0.0);
static MovingValue zAngle(0.05,0.001,-5,5,0.05,0.0);

static void drawGLScene() 
{	
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    float w = glutGet( GLUT_WINDOW_WIDTH );
    float h = glutGet( GLUT_WINDOW_HEIGHT );
    gluPerspective(60, w / h, 0.1, 300);

    glMatrixMode( GL_MODELVIEW );    
    
    //=============================================
    
    glLoadIdentity();
	
	static float time = 0.0;
	time += 0.003;
	
	glUseProgramObjectARB(shaderSky);
	setUniform1f(shaderSky, "time", time);				
	
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, colorMapSky);

	
	float skySize=10.0;
	
	glTranslatef(0,0,-5);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-skySize, -skySize);
	glTexCoord2f(1, 0);
	glVertex2f(skySize, -skySize);
	glTexCoord2f(1, 1);
	glVertex2f(skySize, skySize);
	glTexCoord2f(0, 1);
	glVertex2f(-skySize, skySize);
	glEnd();
    
	//=============================================
	    
    glLoadIdentity();
    
    glTranslatef(0,0,-2.4);
    glRotatef(xAngle.step(),1,0,0);
    glRotatef(yAngle.step(),0,1,0);
	glRotatef(zAngle.step(),0,0,1);

    
    glUseProgramObjectARB(shaderProgram);
	setUniform2f(shaderProgram, "c", koefReD.step(), koefImD.step());
	setUniform1f(shaderProgram, "bound", movingHighBound.step());

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, iterLimit, 0, GL_BGRA, GL_UNSIGNED_BYTE, colorMap);
	
		
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-1, -1);
	glTexCoord2f(1, 0);
	glVertex2f(1, -1);
	glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(-1, 1);
	glEnd();
	
	//=============================================
	
	glutSwapBuffers();
	
	clrMapData.ColorMapStep();
	constFps.step();
}

static void keyPressed(unsigned char key, int x, int y) 
{
    const unsigned char ESCAPE = 27;
    
	if(key == ESCAPE)
	{
		exit(0);
	}
}

int main(int argc, char **argv) 
{		
	glutInitWindowSize(800, 600);
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("opengl");

	glutDisplayFunc(drawGLScene);
	glutIdleFunc(drawGLScene);
	glutKeyboardFunc(keyPressed);	

	
	initColorMap();	
		
	glBindTexture(GL_TEXTURE_1D, 1);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	
	glTexImage1D(GL_TEXTURE_1D, 0, 4, iterLimit, 0, GL_BGRA, GL_UNSIGNED_BYTE, colorMap);

	glEnable(GL_TEXTURE_1D);

		
	if(!(shaderProgram = setupShader("julia.glsl"))) 
	{	
		return -1;
	}

	setUniform1i(shaderProgram, "iterLimit", iterLimit);
	setUniform1f(shaderProgram, "checkDistanceSquare", checkDistanceSquare);
	
	
	if(!(shaderSky = setupShader("sky.glsl"))) 
	{	
		return -1;
	}

    glutFullScreen();
	
	glutMainLoop();
	return 0;
}



