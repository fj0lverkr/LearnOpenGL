#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/Color.hpp"
#include <vector>
#include <cmath>

using namespace std;

constexpr int GLFW_MAJOR_VERSION = 3;
constexpr int GLFW_MINOR_VERSION = 3;

const Color BG = Color(0.2f, 0.3f, 0.3f);

const char *vss = "#version 330 core\n"
				  "layout (location = 0) in vec3 aPos;\n"
				  "layout (location = 1) in vec3 aColor;\n"
				  "out vec3 ourColor;\n"
				  "void main()\n"
				  "{\n"
				  "   gl_Position = vec4(aPos, 1.0);\n"
				  "   ourColor = aColor;\n"
				  "}\0";

const char *fss = "#version 330 core\n"
				  "out vec4 FragColor;\n"
				  "in vec3 ourColor;\n"
				  "void main()\n"
				  "{\n"
				  "   FragColor = vec4(ourColor, 1.0);\n"
				  "}\n\0";

vector<unsigned int> VAOs, VBOs, shaderPrograms;

// Forward declare functions
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processWindowInput(GLFWwindow *window);
int exit_clean(int const &code, string const &reason);
void clearColor(Color c);
void compileShader(const unsigned int &shader, const char *source, const string &name);
void linkShaders(const unsigned int &vertexShader, const unsigned int &fragmentShader, const string &name);
void copyVertexObjectsToVector(const unsigned int vaos[], const unsigned int vbos[]);
void setupTriangles(const bool &wireFrame);
void drawTrangles();

// main function
int main()
{
	if (!glfwInit())
	{
		exit_clean(-1, "Failed to initialize GLFW, exiting...");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		exit_clean(-1, "Failed to create GLFW window, exiting...");
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		exit_clean(-1, "Failed to initialize GLAD, exiting...");
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	setupTriangles(false);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// handle input events
		processWindowInput(window);

		// render commands
		clearColor(BG);
		drawTrangles();

		// poll for events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	exit_clean(0, "");
}

// Functions
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processWindowInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int exit_clean(int const &code, string const &reason)
{
	if (reason != "")
	{
		cout << reason << endl;
	}
	for (unsigned int vao : VAOs)
	{
		glDeleteVertexArrays(1, &vao);
	}
	for (unsigned int vbo : VBOs)
	{
		glDeleteBuffers(1, &vbo);
	}

	for (unsigned int shaderProgram : shaderPrograms)
	{
		glDeleteProgram(shaderProgram);
	}

	glfwTerminate();
	return code;
}

void clearColor(Color c)
{
	glClearColor(c.get(Color::R), c.get(Color::G), c.get(Color::B), c.get(Color::A));
	glClear(GL_COLOR_BUFFER_BIT);
}

void compileShader(const unsigned int &shader, const char *source, const string &name)
{
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 512, NULL, infoLog);

	if (!success)
	{
		std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED" << endl
				  << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::SHADER::" << name << "::COMPILATION" << endl
				  << infoLog << std::endl;
	}
}

void linkShaders(const unsigned int &vertexShader, const unsigned int &fragmentShader, const string &name)
{
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	shaderPrograms.emplace_back(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

	if (!success)
	{
		std::cout << "ERROR::SHADER::PROGRAM::" << name << "::LINKING_FAILED" << endl
				  << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::SHADER::PROGRAM::" << name << "::LINKING" << endl
				  << infoLog << std::endl;
	}
}

void copyVertexObjectsToVector(const unsigned int vaos[], const unsigned int vbos[])
{
	// copy the VOAs and VBOs
	VAOs.clear();
	VBOs.clear();
	for (int i = 0; i < sizeof(vaos); i++)
	{
		VAOs.emplace_back(vaos[i]);
		VBOs.emplace_back(vbos[i]);
	}
}

void setupTriangles(const bool &wireFrame)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int vbos[3], vaos[3]; // for now we hardcode the number

	float leftTriangle[] = {
		// points		// colors
		-0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f,   // bottom right
		-0.75f, 0.5f, 0.f, 0.f, 0.f, 1.f,  // bottom left
		-0.625f, 0.75f, 0.f, 1.f, 0.f, 0.f // top
	};

	float rightTriangle[] = {
		// points		// colors
		0.75f, 0.5f, 0.f, 0.f, 0.f, 1.f,  // bottom right
		0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f,	  // bottom left
		0.625f, 0.75f, 0.f, 0.f, 1.f, 0.f // top
	};

	float bigTriangle[] = {
		// points		// colors
		0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f,	 // top right
		-0.5f, 0.5f, 0.f, 0.f, 1.f, 0.f, // top left
		0.f, -0.5f, 0.f, 0.f, 0.f, 1.f	 // bottom
	};

	compileShader(vertexShader, vss, "VERTEX");
	compileShader(fragmentShader, fss, "FRAGMENT");

	// create a program for each triangle (can probably be done differently later)
	linkShaders(vertexShader, fragmentShader, "1");
	linkShaders(vertexShader, fragmentShader, "1");
	linkShaders(vertexShader, fragmentShader, "1");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenVertexArrays(3, vaos);
	glGenBuffers(3, vbos);

	copyVertexObjectsToVector(vaos, vbos);

	// first triangle setup
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangle), leftTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second triangle setup
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangle), rightTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// third triangle setup
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bigTriangle), bigTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (wireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void drawTrangles()
{

	for (int i = 0; i < sizeof(shaderPrograms); i++)
	{
		glUseProgram(shaderPrograms[i]);
		glBindVertexArray(VAOs[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}