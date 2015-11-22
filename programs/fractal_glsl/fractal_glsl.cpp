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

static unsigned int shaderProgram;

static ColorMapData clrMapData;
static unsigned int *colorMap;

static ConstantFps constFps(35);


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
}

static void drawGLScene() 
{	
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
	
	glutMainLoop();
	return 0;
}



