#ifndef SHADER_H
#define SHADER_H

#include <GLAD/glad.h>
#include <string>

GLuint compileShader(GLenum shaderType, const std::string& source);
GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
GLuint createComputeShaderProgram(const std::string& computeShaderSource);
std::string loadShaderSource(const std::string& filePath);

#endif
