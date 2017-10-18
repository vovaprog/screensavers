// Implementation of gluLookAt.
// Is needed when compiling with emscripten,
// because in emscripten gluLookAt does not work. 

#include <math.h>
#include <GL/glut.h>


static void normalize(float v[3])
{
    float r;

    r = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    
    if (r == 0.0)
    {
        return;
    }

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}


static void cross(float v1[3], float v2[3], float result[3])
{
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}


static void gluMakeIdentityf(GLfloat (&m)[4][4])
{
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;

    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;

    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}


void lookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
            GLdouble centerx, GLdouble centery, GLdouble centerz,
            GLdouble upx, GLdouble upy, GLdouble upz)
{
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    cross(forward, up, side);
    normalize(side);

    cross(side, forward, up);

    gluMakeIdentityf(m);

    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}
