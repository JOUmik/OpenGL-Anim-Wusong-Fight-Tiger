#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* tcsPath, const GLchar* tesPath, const GLchar* geoPath, const GLchar* fragmentPath);
	// Uses the current shader
	void Use();

private:
	void checkCompileErrors(GLuint ID, std::string type);
};

#endif