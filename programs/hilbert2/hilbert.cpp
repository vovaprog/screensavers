#include <math.h>
#include <vector>
#include <iostream>

#include <GL/glut.h>

#include <Hilbert2D.h>
#include <ConstantFps.h>


Hilbert2D hilbert;
static const int ConstantFpsValue = 35;
static ConstantFps constFps(ConstantFpsValue);
static int startupCounter = 0;
static int window;


void createHilbert()
{
    hilbert.calc(6, Hilbert2D::CalcMode::Lines);
}


void drawHilbert()
{
    int hilbertSize = Hilbert2D::calcSize(6);

    glTranslatef(-hilbertSize / 2, -hilbertSize / 3, -hilbertSize);
    glRotatef(-45, 1, 0, 0);
    glColor3f(0.0, 1.0, 0.0);
    
    glBegin(GL_LINE_STRIP);

    for (auto &p : hilbert.getLines())
    {
        glVertex3f(p.first, p.second, 0.0);
    }

    glEnd();
}


void display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float w = glutGet(GLUT_WINDOW_WIDTH);
    float h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(60, w / h, 0.1, 300);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawHilbert();

    if (startupCounter < 0)
    {
        glutSwapBuffers();
    }
    else if (++startupCounter >= ConstantFpsValue)
    {
        startupCounter = -1;
    }

    constFps.step();

    glutPostRedisplay();
}


static void keyPressed(unsigned char key, int x, int y)
{
    const int Escape = 27;

    if (key == Escape)
    {
        glutDestroyWindow(window);
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    window = glutCreateWindow("opengl");
    glutDisplayFunc(display);
    glutKeyboardFunc(&keyPressed);
    glEnable(GL_DEPTH_TEST);

    glutFullScreen();

    createHilbert();

    glutMainLoop();

    return 0;
}

