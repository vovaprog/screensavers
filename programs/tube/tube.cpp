#include <math.h>
#include <vector>
#include <iostream>

#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <ColorMap.h>
#include <ConstantFps.h>

#ifdef __EMSCRIPTEN__
// gluLookAt does not work in emscripten.
void lookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
            GLdouble centerx, GLdouble centery, GLdouble centerz,
            GLdouble upx, GLdouble upy, GLdouble upz);
#else
#    define lookAt gluLookAt
#endif

using glm::vec3;

#define COLOR_MAP_SIZE 120
#define CONSTANT_FPS_VALUE 35
#define PATH_RADIUS 30.0
static const float PI = 3.1415926535;
static const float TUBE_RADIUS = 2.2;

static ColorMapData clrMapData;
static unsigned int *colorMap;

static ColorMapData clrMapDataWall;
static unsigned int *colorMapWall;

static float t = 3.0;

static int window;

static ConstantFps constFps(CONSTANT_FPS_VALUE);

static int startupCounter = 0;

class joint
{
public:
    std::vector<vec3> ps;
    std::vector<int> colors;

    joint(vec3 &p1, vec3 &p2, vec3 &p3, int numberOfPoints)
    {
        vec3 v1 = p2 - p1;
        vec3 v2 = p3 - p2;

        vec3 v = v1 + v2;
        v = normalize(v);

        vec3 vup = vec3(0, 1.0, 0);

        vec3 vhelp = glm::rotate<float>(v, PI / 8.0, vup);

        vec3 vround = normalize(cross(v, vhelp));
        vround = vround * TUBE_RADIUS;

        float angle = 2 * PI / numberOfPoints;

        for (int i = 0; i < numberOfPoints; i++)
        {
            if (i > 0)
            {
                vround = glm::rotate<float>(vround, angle, v);
            }

            ps.push_back(p2 + vround);
            colors.push_back(rand() % COLOR_MAP_SIZE);
        }
    }
};

static std::vector<joint> joints;

inline float xFunction(float t)
{
    return PATH_RADIUS * sin(2 * t);
}

inline float yFunction(float t)
{
    return (PATH_RADIUS / 2.0) * cos(3 * t);
}

inline float zFunction(float t)
{
    return PATH_RADIUS * sin(t);
}

void createTube()
{
    const float tubeStep = 0.015;

    for (float t = 0.0; t <= PI * 2; t += tubeStep)
    {
        vec3 p1(xFunction(t), yFunction(t), zFunction(t));

        vec3 p2(xFunction(t + tubeStep), yFunction(t + tubeStep), zFunction(t + tubeStep));

        vec3 p3(xFunction(t + tubeStep * 2), yFunction(t + tubeStep * 2), zFunction(t + tubeStep * 2));

        joint j = joint(p1, p2, p3, 16);

        joints.push_back(j);
    }
}


#define RED_MASK   0x0000ff
#define GREEN_MASK 0x00ff00
#define BLUE_MASK  0xff0000

#define RED_SHIFT 0
#define GREEN_SHIFT 8
#define BLUE_SHIFT 16

void intColorToFloat(unsigned int Color, float &r, float &g, float &b)
{
    r = (Color & RED_MASK) >> RED_SHIFT;
    g = (Color & GREEN_MASK) >> GREEN_SHIFT;
    b = (Color & BLUE_MASK) >> BLUE_SHIFT;

    r /= 255.0;
    g /= 255.0;
    b /= 255.0;
}

void quad(vec3 a, vec3 b, vec3 c, vec3 d, int aClr, int bClr, int cClr, int dClr)
{
    float rc, gc, bc;

    intColorToFloat(colorMap[aClr], rc, gc, bc);

    const float SUB1 = 0.05;
    const float SUB2 = 0.4;
    const float SUB3 = 0.1;

    float rc1 = rc - SUB1 < 0.0 ? 0.0 : rc - SUB1;
    float gc1 = gc - SUB1 < 0.0 ? 0.0 : gc - SUB1;
    float bc1 = bc - SUB1 < 0.0 ? 0.0 : bc - SUB1;

    float rc2 = rc - SUB2 < 0.0 ? 0.0 : rc - SUB2;
    float gc2 = gc - SUB2 < 0.0 ? 0.0 : gc - SUB2;
    float bc2 = bc - SUB2 < 0.0 ? 0.0 : bc - SUB2;

    float rc3 = rc - SUB3 < 0.0 ? 0.0 : rc - SUB3;
    float gc3 = gc - SUB3 < 0.0 ? 0.0 : gc - SUB3;
    float bc3 = bc - SUB3 < 0.0 ? 0.0 : bc - SUB3;

    glBegin(GL_QUADS);

    glColor3f(rc, gc, bc);
    glVertex3f(a.x, a.y, a.z);

    glColor3f(rc1, gc1, bc1);
    glVertex3f(b.x, b.y, b.z);

    glColor3f(rc2, gc2, bc2);
    glVertex3f(c.x, c.y, c.z);

    glColor3f(rc3, gc3, bc3);
    glVertex3f(d.x, d.y, d.z);

    glEnd();
}

void drawSegment(joint &jt1, joint &jt2)
{
    for (unsigned int i = 0; i < jt1.ps.size(); i++)
    {
        if (i < jt1.ps.size() - 1)
        {
            quad(jt1.ps[i], jt2.ps[i], jt2.ps[i + 1], jt1.ps[i + 1],
                 jt1.colors[i], jt2.colors[i], jt2.colors[i + 1], jt1.colors[i + 1]);
        }
        else
        {
            quad(jt1.ps[i], jt2.ps[i], jt2.ps[0], jt1.ps[0],
                 jt1.colors[i], jt2.colors[i], jt2.colors[0], jt1.colors[0]);
        }
    }
}

void drawCubes()
{
    for (unsigned int i = 0; i < joints.size() - 1; i++)
    {
        if (i % 15 < 8)
        {
            drawSegment(joints[i], joints[i + 1]);
        }
    }
}

void getWallColor(int wallIndex, float clr[4][3])
{
    int clrIndex = 0;

    switch (wallIndex)
    {
    case 0:
        clrIndex = 0;
        break;
    case 1:
        clrIndex = COLOR_MAP_SIZE / 3;
        break;
    case 2:
        clrIndex = COLOR_MAP_SIZE * 2 / 3;
        break;
    case 3:
        clrIndex = COLOR_MAP_SIZE - 1;
        break;
    }

    float r, g, b;

    intColorToFloat(colorMapWall[clrIndex], r, g, b);

    const float SUB0 = 0.05;
    const float SUB1 = 0.4;
    const float SUB2 = 0.1;
    const float SUB3 = 0.8;

    clr[0][0] = r - SUB0 < 0.0 ? 0.0 : r - SUB0;
    clr[0][1] = g - SUB0 < 0.0 ? 0.0 : g - SUB0;
    clr[0][2] = b - SUB0 < 0.0 ? 0.0 : b - SUB0;

    clr[1][0] = r - SUB1 < 0.0 ? 0.0 : r - SUB1;
    clr[1][1] = g - SUB1 < 0.0 ? 0.0 : g - SUB1;
    clr[1][2] = b - SUB1 < 0.0 ? 0.0 : b - SUB1;

    clr[2][0] = r - SUB2 < 0.0 ? 0.0 : r - SUB2;
    clr[2][1] = g - SUB2 < 0.0 ? 0.0 : g - SUB2;
    clr[2][2] = b - SUB2 < 0.0 ? 0.0 : b - SUB2;

    clr[3][0] = r - SUB3 < 0.0 ? 0.0 : r - SUB3;
    clr[3][1] = g - SUB3 < 0.0 ? 0.0 : g - SUB3;
    clr[3][2] = b - SUB3 < 0.0 ? 0.0 : b - SUB3;
}

void drawWall()
{
    const float WALL_DISTANCE = 50.0;
    const float WALL_DEPTH = 3.0;
    const float WALL_BOUND = 45.0;
    const int NUMBER_OF_SQUARES = 9;

    float x = -WALL_BOUND, y = WALL_BOUND, z;
    float d = 2.0 * WALL_BOUND / NUMBER_OF_SQUARES;
    int colorIndex;

    glBegin(GL_QUADS);

    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        for (int j = 0; j < NUMBER_OF_SQUARES; j++)
        {
            if (i % 2 == 0 && j % 2 == 0)
            {
                z = -WALL_DISTANCE;
                colorIndex = 0;
            }
            else if (i % 2 != 0 && j % 2 != 0)
            {
                z = -WALL_DISTANCE - WALL_DEPTH;
                colorIndex = 1;
            }
            else if (i % 2 != 0 && j % 2 == 0)
            {
                z = -WALL_DISTANCE - WALL_DEPTH * 2;
                colorIndex = 2;
            }
            else if (i % 2 == 0 && j % 2 != 0)
            {
                z = -WALL_DISTANCE - WALL_DEPTH * 3;
                colorIndex = 3;
            }

            float clr[4][3];

            getWallColor(colorIndex, clr);

            glColor3f(clr[0][0], clr[0][1], clr[0][2]);
            glVertex3f(x, y, z);

            glColor3f(clr[1][0], clr[1][1], clr[1][2]);
            glVertex3f(x + d, y, z);

            glColor3f(clr[2][0], clr[2][1], clr[2][2]);
            glVertex3f(x + d, y - d, z);

            glColor3f(clr[3][0], clr[3][1], clr[3][2]);
            glVertex3f(x, y - d, z);

            x += d;
        }

        x = -WALL_BOUND;
        y -= d;
    }

    glEnd();
}

void drawSky()
{
    glPushMatrix();

    drawWall();

    glRotatef(90, 0, 1, 0);

    drawWall();

    glRotatef(90, 0, 1, 0);

    drawWall();

    glRotatef(90, 0, 1, 0);

    drawWall();

    glRotatef(90, 1, 0, 0);

    drawWall();

    glRotatef(180, 1, 0, 0);

    drawWall();

    glPopMatrix();
}



void display()
{
    const float LOOK_FORWARD_DISTANCE = 0.18;
    const float MOVE_DISTANCE = 0.001;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float w = glutGet(GLUT_WINDOW_WIDTH);
    float h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(60, w / h, 0.1, 300);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lookAt
    (
        //eye
        xFunction(t), yFunction(t), zFunction(t),

        //lookat
        xFunction(t + LOOK_FORWARD_DISTANCE), yFunction(t + LOOK_FORWARD_DISTANCE), zFunction(t + LOOK_FORWARD_DISTANCE),

        //up
        0, 1, 0
    );

    drawSky();

    drawCubes();


    t += MOVE_DISTANCE;

    if (t > PI * 2)
    {
        t -= PI * 2;
    }

    clrMapData.ColorMapStep();
    clrMapDataWall.ColorMapStep();

    if (startupCounter < 0)
    {
        glutSwapBuffers();
    }
    else if (++startupCounter >= CONSTANT_FPS_VALUE)
    {
        startupCounter = -1;
    }

    constFps.step();

    glutPostRedisplay();
}

void createColorMap()
{
    clrMapData.InitColorMap(COLOR_MAP_SIZE, 16, 200);
    colorMap = clrMapData.GetColorMap();

    clrMapDataWall.SetCheckMixedColorVariants(true);
    clrMapDataWall.SetUseBlackColor(false);
    clrMapDataWall.InitColorMap(COLOR_MAP_SIZE, 4, 150);
    colorMapWall = clrMapDataWall.GetColorMap();
}

#define ESCAPE 27
static void keyPressed(unsigned char key, int x, int y)
{
    if (key == ESCAPE)
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

    createColorMap();

    createTube();

    glutMainLoop();

    return 0;
}

