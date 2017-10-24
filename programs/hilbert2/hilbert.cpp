#include <math.h>
#include <vector>
#include <iostream>

#include <GL/glut.h>

#include <Hilbert2D.h>
#include <Hilbert3D.h>
#include <ConstantFps.h>

namespace
{

const int ConstantFpsValue = 35;
ConstantFps constFps(ConstantFpsValue);
int startupCounter = 0;
int window;

const int Hilbert3DOrder = 4;

Hilbert2D hilbert;
Hilbert3D hilbert3D;

double ang = 0.0;

}


void createHilbert()
{
    hilbert.calc(6, Hilbert2D::CalcMode::Lines);
    hilbert3D.calc(Hilbert3DOrder);
}


void drawHilbert()
{
    glTranslatef(0, 0, -60);
    glColor3f(0.0, 1.0, 0.0);
    glRotatef(ang, 0, 1, 0);
    ang += 0.5;

    float trans = Hilbert3D::calcSize(Hilbert3DOrder) / 2.0;
    glTranslatef(-trans, -trans, trans);

    glBegin(GL_LINE_STRIP);

    for (auto &p : hilbert3D.getLines())
    {
        glVertex3f(std::get<0>(p), std::get<1>(p), std::get<2>(p));
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

