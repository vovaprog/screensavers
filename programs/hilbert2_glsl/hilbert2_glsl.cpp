#include <iostream>

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Hilbert2D.h>
#include <Hilbert3D.h>
#include <ConstantFps.h>

namespace
{

Hilbert2D hilbert2;
Hilbert3D hilbert3;

const int Hilbert2DOrder = 6;
const int Hilbert3DOrder = 4;

const float cubeScale = 0.05f;
const float cubeSize = Hilbert3D::calcSize(Hilbert3DOrder) * cubeScale;

const float wallScale = 0.017f;
const float wallSize = Hilbert2D::calcSize(Hilbert2DOrder) * wallScale;

int wallVertexCount, cubeVertexCount;

int window;

const int ConstantFpsValue = 35;
ConstantFps constFps(ConstantFpsValue);
int startupCounter = 0;

float cubeAngle = 0.0;
const float wallColor[3] = {1.0, 0.0, 1.0};
const float cubeColor[3] = {0.0, 0.0, 1.0};

GLuint shaderProgram;

GLint uniformTransform, uniformColor, uniformProjection;

glm::mat4 transformWall1;
glm::mat4 transformWall2;
glm::mat4 transformCube;

GLuint vertexArray[2]; // Vertex Array Objects for wall and cube


//===========================================================================================
// vertex shader
//===========================================================================================

const char* const vertexShaderSource = R"shader(
#version 150

in vec3 inPosition;
out vec4 pointPosition;

uniform mat4 transformMatrix;
uniform mat4 projMatrix;

void main(void)
{
    pointPosition = transformMatrix * vec4(inPosition, 1.0);
    gl_Position = projMatrix * pointPosition;
}
)shader";


const char* const fragmentShaderSource = R"shader(
#version 150

in vec4 pointPosition;
out vec4 outColor;

uniform vec3 color;

void main(void)
{
    float start = 3.1, end = 4.5;
    float z = -pointPosition.z;
    float k;

    if (z <= start)
    {
        k = 1.0;
    }
    else if(z >= end)
    {
        k = 0.0;
    }
    else
    {
        k = (end - z) / (end - start);
    }

    outColor = vec4(color.x * k, color.y * k, color.z * k, 1.0);
}
)shader";

} // namespace


bool setupScene()
{
    GLuint vboWall[1], vboCube[1];

    glGenVertexArrays(2, vertexArray);

    //===========================================================================================
    // create wall vertexes
    //===========================================================================================

    glBindVertexArray(vertexArray[0]);
    glGenBuffers(1, vboWall);
    glBindBuffer(GL_ARRAY_BUFFER, vboWall[0]);

    {
        auto& vertexes = hilbert2.getLines();
        wallVertexCount = vertexes.size();

        GLfloat *pl = new GLfloat[wallVertexCount * 3];

        int index = 0;
        for(auto &p : vertexes)
        {
            pl[index++] = p.first;
            pl[index++] = p.second;
            pl[index++] = 0.0;
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * wallVertexCount * 3, pl, GL_STATIC_DRAW);

        delete[] pl;
    }

    // vertex data is in atrribute 0 and contains 3 floats per vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // enable attribute 0
    glEnableVertexAttribArray(0);


    //===========================================================================================
    // create cube vertexes
    //===========================================================================================

    glBindVertexArray(vertexArray[1]);
    glGenBuffers(1, vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube[0]);

    {
        auto& vertexes = hilbert3.getLines();
        cubeVertexCount = vertexes.size();

        GLfloat *pl = new GLfloat[cubeVertexCount * 3];

        int index = 0;
        for(auto &p : vertexes)
        {
            pl[index++] = std::get<0>(p);
            pl[index++] = std::get<1>(p);
            pl[index++] = std::get<2>(p);
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * cubeVertexCount * 3, pl, GL_STATIC_DRAW);

        delete[] pl;
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);


    //===========================================================================================
    // create vertex shader
    //===========================================================================================

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)(&vertexShaderSource), 0);
    glCompileShader(vertexShader);

    int isCompiled = 0;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == 0)
    {
        int maxLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        char *vertexInfoLog = new char[maxLength];

        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, vertexInfoLog);

        std::cout << vertexInfoLog << std::endl;

        delete[] vertexInfoLog;
        return false;
    }


    //===========================================================================================
    // create fragment shader
    //===========================================================================================

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == 0)
    {
        int maxLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        char *fragmentInfoLog = new char[maxLength];

        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, fragmentInfoLog);

        std::cout << fragmentInfoLog << std::endl;

        delete[] fragmentInfoLog;
        return false;
    }


    //===========================================================================================
    // create program
    //===========================================================================================

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "inPosition");

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isCompiled);
    if (isCompiled == 0)
    {
        int maxLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        char *shaderProgramInfoLog = new char[maxLength];

        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

        std::cout << shaderProgramInfoLog << std::endl;

        delete[] shaderProgramInfoLog;
        return false;
    }

    glUseProgram(shaderProgram);


    //===========================================================================================
    // get uniform locations
    //===========================================================================================

    uniformTransform = glGetUniformLocation(shaderProgram, "transformMatrix");
    uniformColor = glGetUniformLocation(shaderProgram, "color");
    uniformProjection = glGetUniformLocation(shaderProgram, "projMatrix");


    //===========================================================================================
    // prepare wall transform matrixes
    //===========================================================================================

    transformWall1 = glm::mat4(1.0f);
    transformWall1 = glm::translate(transformWall1, glm::vec3(-cubeSize * 1.1f, 0.0f, -4.0f));
    transformWall1 = glm::rotate(transformWall1, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transformWall1 = glm::translate(transformWall1, glm::vec3(-wallSize * 0.5, -wallSize * 0.5, 0.0));
    transformWall1 = glm::scale(transformWall1, glm::vec3(wallScale));

    transformWall2 = glm::mat4(1.0f);
    transformWall2 = glm::translate(transformWall2, glm::vec3(cubeSize * 1.1f, 0.0f, -4.0f));
    transformWall2 = glm::rotate(transformWall2, glm::radians(-80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transformWall2 = glm::translate(transformWall2, glm::vec3(-wallSize * 0.5, -wallSize * 0.5, 0.0));
    transformWall2 = glm::scale(transformWall2, glm::vec3(wallScale));

    return true;
}


void display()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //===========================================================================================
    // draw walls
    //===========================================================================================

    glUniform3fv(uniformColor, 1, wallColor);

    glBindVertexArray(vertexArray[0]);

    glUniformMatrix4fv(uniformTransform, 1, GL_FALSE, glm::value_ptr(transformWall1));
    glDrawArrays(GL_LINE_STRIP, 0, wallVertexCount);

    glUniformMatrix4fv(uniformTransform, 1, GL_FALSE, glm::value_ptr(transformWall2));
    glDrawArrays(GL_LINE_STRIP, 0, wallVertexCount);


    //===========================================================================================
    // draw cube
    //===========================================================================================

    transformCube = glm::mat4(1.0f);
    transformCube = glm::translate(transformCube, glm::vec3(0.0f, 0.0f, -4.0f));
    transformCube = glm::rotate(transformCube, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    transformCube = glm::translate(transformCube, glm::vec3(-cubeSize / 2.0f, -cubeSize / 2.0f, cubeSize / 2.0f));
    transformCube = glm::scale(transformCube, glm::vec3(cubeScale));

    glBindVertexArray(vertexArray[1]);

    glUniform3fv(uniformColor, 1, cubeColor);
    glUniformMatrix4fv(uniformTransform, 1, GL_FALSE, glm::value_ptr(transformCube));
    glDrawArrays(GL_LINE_STRIP, 0, cubeVertexCount);

    cubeAngle += 0.5;


    if (startupCounter < 0)
    {
        glFlush ();
        glutSwapBuffers();
    }
    else if (++startupCounter >= ConstantFpsValue)
    {
        startupCounter = -1;
    }

    constFps.step();

    glutPostRedisplay();
}


void keyPressed(unsigned char key, int x, int y)
{
    const int Escape = 27;

    if (key == Escape)
    {
        glutDestroyWindow(window);
    }
}


void onReshapeWindow(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 1.0f, 30.0f);
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}


int main(int argc, char *argv[])
{
    hilbert2.calc(Hilbert2DOrder, Hilbert2D::CalcMode::Lines);
    hilbert3.calc(Hilbert3DOrder);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion (3, 1);
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

    glutInitWindowSize (800, 600);
    glutInitWindowPosition (100, 100);
    window = glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutKeyboardFunc (keyPressed);
    glutReshapeFunc(onReshapeWindow);

    glEnable(GL_DEPTH_TEST);

    if (!setupScene())
    {
        return -1;
    }

    glutFullScreen();

    glutMainLoop();

    return 0;
}
