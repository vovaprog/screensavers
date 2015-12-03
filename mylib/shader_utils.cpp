#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include <filesystem_utils.h>

extern "C" { 

GLhandleARB glCreateShaderObjectARB(GLenum);
void glShaderSourceARB(GLhandleARB, int, const char**, int*);
void glCompileShaderARB(GLhandleARB);
GLhandleARB glCreateProgramObjectARB(void);
void glAttachObjectARB(GLhandleARB, GLhandleARB);
void glLinkProgramARB(GLhandleARB);
void glUseProgramObjectARB(GLhandleARB);
void glGetInfoLogARB(GLhandleARB, GLsizei, GLsizei*, GLcharARB*);
void glGetObjectParameterivARB(GLhandleARB, GLenum, int*);
GLint glGetUniformLocationARB(GLhandleARB, const char*);
void glUniform1f(GLint location, GLfloat v0);
void glUniform1i(GLint location, GLint v0);
void glUniform2f(GLint location, GLfloat v0, GLfloat v1);

}

unsigned int setupShader(const char *fileName) 
{
	unsigned int prog, shaderObject;
	char *sourceCodeBuf;
	int success, linked;

	if(!readTextFile(fileName, &sourceCodeBuf))
	{
	    return 0;
	}	
	
	shaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(shaderObject, 1, (const char**)&sourceCodeBuf, 0);
	delete[] sourceCodeBuf;

	glCompileShaderARB(shaderObject);
	glGetObjectParameterivARB(shaderObject, GL_OBJECT_COMPILE_STATUS_ARB, &success);
	if(!success) 
	{
		int infoLen;
		char *infoLog;
		
		glGetObjectParameterivARB(shaderObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infoLen);
		if(infoLen > 0) 
		{
			if(!(infoLog = (char*)malloc(infoLen + 1))) 
			{
				perror("malloc failed");
				return 0;
			}
			glGetInfoLogARB(shaderObject, infoLen, 0, infoLog);
			fprintf(stderr, "shader compilation failed: %s\r\n", infoLog);
			free(infoLog);
		} 
		else 
		{
			fprintf(stderr, "shader compilation failed\r\n");
		}
		return 0;
	}

	prog = glCreateProgramObjectARB();
	glAttachObjectARB(prog, shaderObject);
	glLinkProgramARB(prog);
	glGetObjectParameterivARB(prog, GL_OBJECT_LINK_STATUS_ARB, &linked);
	if(!linked) 
	{
		fprintf(stderr, "shader linking failed\n");
		return 0;
	}

	glUseProgramObjectARB(prog);
	return prog;
}

bool setUniform1f(unsigned int shaderProgram, const char *name, float value) 
{
	int loc = glGetUniformLocationARB(shaderProgram, name);
	if(loc != -1) 
	{
		glUniform1f(loc, value);
		return true;
	}
	else
	{
	    return false;
	}
}

bool setUniform2f(unsigned int shaderProgram, const char *name, float value0, float value1) 
{
	int loc = glGetUniformLocationARB(shaderProgram, name);
	if(loc != -1) 
	{
		glUniform2f(loc, value0, value1);
		return true;
	}
	else
	{
	    return false;
	}
}

bool setUniform1i(unsigned int shaderProgram, const char *name, int value) 
{
	int loc = glGetUniformLocationARB(shaderProgram, name);
	if(loc != -1) 
	{
		glUniform1i(loc, value);
		return true;
	}
	else
	{
	    return false;
	}
}

