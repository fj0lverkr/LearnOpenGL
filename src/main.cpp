#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/Color.hpp"
#include <vector>

using namespace std;

constexpr int GLFW_MAJOR_VERSION = 3;
constexpr int GLFW_MINOR_VERSION = 3;

const Color BG = Color(0.2f, 0.3f, 0.3f);

const char *vss = "#version 330 core\n"
				  "layout (location = 0) in vec3 aPos;\n"
				  "void main()\n"
				  "{\n"
				  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
				  "}\0";

const char *fssOrange = "#version 330 core\n"
						"out vec4 FragColor;\n"
						"void main()\n"
						"{\n"
						"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
						"}\n\0";

const char *fssYellow = "#version 330 core\n"
						"out vec4 FragColor;\n"
						"void main()\n"
						"{\n"
						"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED" << endl
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
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::" << name << "::LINKING_FAILED" << endl
				  << infoLog << std::endl;
	}
}

void copyVertexObjectsToVector(const unsigned int vaos[], const unsigned int vbos[])
{
	// copy the VOAs and VBOs
	VAOs.clear();
	VBOs.clear();
	for (int i = 0; i < 2; i++)
	{
		VAOs.emplace_back(vaos[i]);
		VBOs.emplace_back(vbos[i]);
	}
}

void setupTriangles(const bool &wireFrame)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int vbos[2], vaos[2]; // for now we hardcode the number

	float firstTriangle[] = {
		0.5f, 0.5f, 0.0f,	// top right
		0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f	// top left
	};

	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f, // left
		0.9f, -0.5f, 0.0f, // right
		0.45f, 0.5f, 0.0f  // top
	};

	compileShader(vertexShader, vss, "VERTEX");
	compileShader(fragmentShaderOrange, fssOrange, "FRAGMENT_ORANGE");
	compileShader(fragmentShaderYellow, fssYellow, "FRAGMENT_YELLOW");

	linkShaders(vertexShader, fragmentShaderOrange, "ORANGE");
	linkShaders(vertexShader, fragmentShaderYellow, "YELLOW");

	glGenVertexArrays(2, vaos);
	glGenBuffers(2, vbos);

	copyVertexObjectsToVector(vaos, vbos);

	// first triangle setup
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); // Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines

	// second triangle setup
	glBindVertexArray(VAOs[1]);				// note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // and a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

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