#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
	void compileShader(const unsigned int &shader, const char *source, const char *name);
	void linkShaders(const unsigned int &program, unsigned int &vertex, unsigned int &fragment);

public:
	unsigned int programId;

	Shader(const char *vertexPath, const char *fragmentPath);
	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif // GRAHICS_SHADER_HPP