#include "graphics/Shader.hpp"

using namespace std;

void Shader::compileShader(const unsigned int &shader, const char *source, const char *name)
{
	int success;
	char infoLog[512];

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 512, NULL, infoLog);

	if (!success)
	{
		std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED" << endl
				  << infoLog << std::endl;
	}
}

void Shader::linkShaders(const unsigned int &program, unsigned int &vertex, unsigned int &fragment)
{
	int success;
	char infoLog[512];

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl
			 << infoLog << endl;
	}
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	string vertexCode, fragmentCode;
	ifstream vShaderFile, fShaderFile;
	stringstream vShaderStream, fShaderStream;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_READ " << e.what() << endl;
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertexShader, vShaderCode, "VERTEX");
	compileShader(fragmentShader, fShaderCode, "FRAGMENT");

	programId = glCreateProgram();
	linkShaders(programId, vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(programId);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}