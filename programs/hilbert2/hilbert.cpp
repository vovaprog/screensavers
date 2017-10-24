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

const int Hilbert2DOrder = 6;
const int Hilbert3DOrder = 4;
const double Hilbert2DSizeDiv2 = Hilbert2D::calcSize(Hilbert2DOrder) / 2.0;
const double Hilbert3DSizeDiv2 = Hilbert3D::calcSize(Hilbert3DOrder) / 2.0;
const double hilbert2DScale = 1.2 * static_cast<double>(Hilbert3D::calcSize(Hilbert3DOrder)) / static_cast<double>(Hilbert2D::calcSize(Hilbert2DOrder));

Hilbert2D hilbert2D;
Hilbert3D hilbert3D;

double cubeAngle = 0.0;
}


void createHilbert()
{
    hilbert2D.calc(Hilbert2DOrder, Hilbert2D::CalcMode::Lines);
    hilbert3D.calc(Hilbert3DOrder);
}


void drawWall(int movX, int movY)
{
    glPushMatrix();

    glTranslatef(-Hilbert3DSizeDiv2 * 2.2 * movX, -Hilbert3DSizeDiv2 * 2.2 * movY, -60);
    if (movX != 0)
    {
        glRotatef(90, 0, 1, 0);
    }
    else
    {
        glRotatef(90, 1, 0, 0);
    }

    glColor3f(0.0, 0.0, 0.8);

    glBegin(GL_LINE_STRIP);

    for (auto &p : hilbert2D.getLines())
    {
        glVertex3f((p.first - Hilbert2DSizeDiv2) * hilbert2DScale, (p.second - Hilbert2DSizeDiv2) * hilbert2DScale, 0.0);
    }

    glEnd();

    glPopMatrix();
}


void drawHilbert()
{
    glPushMatrix();

    glTranslatef(0, 0, -60);
    glRotatef(cubeAngle, 0, 1, 0);
    cubeAngle += 0.5;

    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_LINE_STRIP);

    for (auto &p : hilbert3D.getLines())
    {
        glVertex3f(std::get<0>(p) - Hilbert3DSizeDiv2, std::get<1>(p) - Hilbert3DSizeDiv2, std::get<2>(p) + Hilbert3DSizeDiv2);
    }

    glEnd();

    glPopMatrix();
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
    drawWall(1, 0);
    drawWall(-1, 0);

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

