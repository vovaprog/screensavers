#pragma once


unsigned int setupShader(const char *fileName);

bool setUniform1f(unsigned int shaderProgram, const char *name, float value);
bool setUniform2f(unsigned int shaderProgram, const char *name, float value0, float value1);
bool setUniform1i(unsigned int shaderProgram, const char *name, int value);

