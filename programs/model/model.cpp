#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ConstantFps.h>

namespace
{
int window;

const int ConstantFpsValue = 35;
ConstantFps constFps(ConstantFpsValue);
int startupCounter = 0;

GLuint shaderProgram;

GLint uniformTransform, uniformTransformNormal, uniformLightPosition, uniformProjection;

GLuint vertexArray[1];

int modelBufSize;
float* modelVertexes;
float *modelNormals;

float cameraDist = 5.0f, cameraXAngle = -25.0f, cameraYAngle = 40.0f;

float lightDist = 30.0f, lightXAngle = -10.0f, lightYAngle = 30.0f;

const float cameraDistAdd = 0.3f;
const float angleAdd = 2.0f;


//===========================================================================================
// vertex shader
//===========================================================================================

const char* const vertexShaderSource = R"shader(
#version 150

in vec3 inPosition;
in vec3 inNormal;

out vec4 transPos;
out vec4 transNorm;
out vec4 transLight;

uniform mat4 transformMatrix;
uniform mat3 transformNormalMatrix;
uniform mat4 projMatrix;
uniform vec4 lightPosition;

void main(void)
{
    gl_Position = projMatrix * transformMatrix * vec4(inPosition, 1.0);

    transPos = transformMatrix * vec4(inPosition, 1.0);
    transNorm = vec4(transformNormalMatrix * inNormal, 1.0);
    transLight = transformMatrix * lightPosition;
}
)shader";


const char* const fragmentShaderSource = R"shader(
#version 150

in vec4 transPos;
in vec4 transNorm;
in vec4 transLight;

out vec4 outColor;

void main(void)
{
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 emissiveLight = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 materialDiffuse = vec4(0.9, 0.9, 0.9, 1.0);

    vec4 lVec = normalize(transLight - transPos);
    vec4 nVec = normalize(transNorm);

    float cosNL = clamp(dot(nVec, lVec), 0.0, 1.0);

    vec4 diffuseLight =  lightColor * materialDiffuse * cosNL;

    outColor = (emissiveLight + diffuseLight);
}
)shader";

} // namespace


bool loadObj(const char *fileName, bool withNormals, float *&bufVertexes, float *&bufNormals, int &bufSize)
{
    struct Face {
        int vInd0, vInd1, vInd2;
        int nInd0, nInd1, nInd2;
    };

    std::vector<std::tuple<float, float, float>> vertexes;
    std::vector<std::tuple<float, float, float>> normals;
    std::vector<Face> faces;

    // indexes in faces start with 1, so add zero unused vertex.
    vertexes.push_back(std::make_tuple(0.0f, 0.0f, 0.0f));
    normals.push_back(std::make_tuple(0.0f, 0.0f, 0.0f));

    std::fstream fs(fileName);

    std::string line;
    while (std::getline(fs, line))
    {
        std::istringstream iss(line);

        std::string header;
        iss >> header;
        if (header == "v")
        {
            float x, y, z;
            if (!(iss >> x >> y >> z))
            {
                return false;
            }
            vertexes.push_back(std::make_tuple(x, y, z));
        }
        else if (header == "f")
        {
            Face f;

            if (withNormals)
            {
                char temp[100];

                if (sscanf(line.c_str(), "%s %d//%d %d//%d %d//%d", temp,
                           &f.vInd0, &f.nInd0, &f.vInd1, &f.nInd1, &f.vInd2, &f.nInd2) != 7)
                {
                    return false;
                }
            }
            else
            {
                if (!(iss >> f.vInd0 >> f.vInd1 >> f.vInd2))
                {
                    return false;
                }
            }
            faces.push_back(f);
        }
        else if (header == "vn")
        {
            float x, y, z;
            if (!(iss >> x >> y >> z))
            {
                return false;
            }
            normals.push_back(std::make_tuple(x, y, z));
        }
    }

    bufSize = faces.size() * 3 * 3;
    bufVertexes = new float[bufSize];
    bufNormals = new float[bufSize];

    int ind = 0;
    for (const auto &face : faces)
    {
        bufVertexes[ind] = std::get<0>(vertexes[face.vInd0]);
        bufNormals[ind++] = std::get<0>(normals[face.nInd0]);
        bufVertexes[ind] = std::get<1>(vertexes[face.vInd0]);
        bufNormals[ind++] = std::get<1>(normals[face.nInd0]);
        bufVertexes[ind] = std::get<2>(vertexes[face.vInd0]);
        bufNormals[ind++] = std::get<2>(normals[face.nInd0]);

        bufVertexes[ind] = std::get<0>(vertexes[face.vInd1]);
        bufNormals[ind++] = std::get<0>(normals[face.nInd1]);
        bufVertexes[ind] = std::get<1>(vertexes[face.vInd1]);
        bufNormals[ind++] = std::get<1>(normals[face.nInd1]);
        bufVertexes[ind] = std::get<2>(vertexes[face.vInd1]);
        bufNormals[ind++] = std::get<2>(normals[face.nInd1]);

        bufVertexes[ind] = std::get<0>(vertexes[face.vInd2]);
        bufNormals[ind++] = std::get<0>(normals[face.nInd2]);
        bufVertexes[ind] = std::get<1>(vertexes[face.vInd2]);
        bufNormals[ind++] = std::get<1>(normals[face.nInd2]);
        bufVertexes[ind] = std::get<2>(vertexes[face.vInd2]);
        bufNormals[ind++] = std::get<2>(normals[face.nInd2]);
    }

    return true;
}

bool setupScene()
{
    GLuint vbo[2];

    glGenVertexArrays(1, vertexArray);

    //===========================================================================================
    // create buffers with vertexes and normals
    //===========================================================================================

    glBindVertexArray(vertexArray[0]);

    glGenBuffers(2, vbo);

    // load vertixes
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * modelBufSize, modelVertexes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // load normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * modelBufSize, modelNormals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);


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
    glBindAttribLocation(shaderProgram, 1, "inNormal");

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
    uniformTransformNormal = glGetUniformLocation(shaderProgram, "transformNormalMatrix");

    uniformLightPosition = glGetUniformLocation(shaderProgram, "lightPosition");
    uniformProjection = glGetUniformLocation(shaderProgram, "projMatrix");

    return true;
}


void calcCameraMatrix()
{
    glm::vec4 cameraVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::mat4 transformCamera = glm::mat4(1.0f);
    transformCamera = glm::rotate(transformCamera, glm::radians(cameraYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    transformCamera = glm::rotate(transformCamera, glm::radians(cameraXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec4 cameraPos = transformCamera * cameraVec;
    cameraPos = cameraPos * cameraDist;

    glm::mat4 cameraMatrix = glm::lookAt(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(uniformTransform, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    glm::mat3 transformNormal = glm::transpose(glm::inverse(glm::mat3(cameraMatrix)));

    glUniformMatrix3fv(uniformTransformNormal, 1, GL_FALSE, glm::value_ptr(transformNormal));
}


void calcLight()
{
    glm::vec4 lightVec(0.0f, 0.0f, 1.0f, 1.0f);

    glm::mat4 transformLight = glm::mat4(1.0f);
    transformLight = glm::rotate(transformLight, glm::radians(lightYAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    transformLight = glm::rotate(transformLight, glm::radians(lightXAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec4 lightPos = transformLight * lightVec;
    lightPos = lightPos * lightDist;
    lightPos.w = 1.0f;

    glUniform4fv(uniformLightPosition, 1, glm::value_ptr(lightPos));
}


void display()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glDrawArrays(GL_TRIANGLES, 0, modelBufSize);


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


float clamp(float value, float minValue, float maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    else if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}


void keyPressed(unsigned char key, int x, int y)
{
    const int Escape = 27;

    bool updateCamera = false, updateLight = false;

    switch (key) {
    case Escape:
        glutDestroyWindow(window);
        break;
    case '-':
        cameraDist += cameraDistAdd;
        updateCamera = true;
        break;
    case '=':
        cameraDist -= cameraDistAdd;
        updateCamera = true;
        break;
    case 'a':
        lightYAngle -= angleAdd;
        updateLight = true;
        break;
    case 'd':
        lightYAngle += angleAdd;
        updateLight = true;
        break;
    case 'w':
        lightXAngle -= angleAdd;
        updateLight = true;
        break;
    case 's':
        lightXAngle += angleAdd;
        updateLight = true;
        break;
    case 'l':
    {
        GLint result[2];
        glGetIntegerv(GL_POLYGON_MODE, result);
        if (result[1] == GL_LINE)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    break;
    }

    if (updateCamera)
    {
        calcCameraMatrix();
    }
    else if (updateLight)
    {
        calcLight();
    }
}


void specialKeyPressed(int key, int x, int y)
{
    const int UpKey = 101, DownKey = 103, LeftKey = 100, RightKey = 102, PgUpKey = 104, PgDownKey = 105;

    bool updateCamera = false;

    switch (key) {
    case UpKey:
        cameraXAngle = clamp(cameraXAngle - angleAdd, -89.0f, 89.0f);
        updateCamera = true;
        break;
    case DownKey:
        cameraXAngle = clamp(cameraXAngle + angleAdd, -89.0f, 89.0f);
        updateCamera = true;
        break;
    case LeftKey:
        cameraYAngle += angleAdd;
        updateCamera = true;
        break;
    case RightKey:
        cameraYAngle -= angleAdd;
        updateCamera = true;
        break;
    case PgUpKey:
        cameraDist += cameraDistAdd;
        updateCamera = true;
        break;
    case PgDownKey:
        cameraDist -= cameraDistAdd;
        updateCamera = true;
        break;
    }

    if (updateCamera)
    {
        calcCameraMatrix();
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
    if (!loadObj("./plane.obj", true, modelVertexes, modelNormals, modelBufSize))
    {
        std::cout << "can't load model" <<std::endl;
        return -1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitContextVersion (3, 1);
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

    glutInitWindowSize (1000, 600);
    glutInitWindowPosition (100, 100);
    window = glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(specialKeyPressed);
    glutReshapeFunc(onReshapeWindow);

    glEnable(GL_DEPTH_TEST);

    if (!setupScene())
    {
        return -1;
    }

    calcCameraMatrix();
    calcLight();

    glutFullScreen();

    const char * const KeyHelp =
R"keys(up, down left, right: rotate model.
w, s, a, d:           rotate light.
PageDown, + :         move to model.
PageUp, - :           move from model.
l :                   toggle line mode.)keys";

    std::cout << KeyHelp << std::endl;

    glutMainLoop();

    return 0;
}
