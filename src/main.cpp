#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/Color.hpp"
#include "graphics/Shader.hpp"

using namespace std;

constexpr int GLFW_MAJOR_VERSION = 3;
constexpr int GLFW_MINOR_VERSION = 3;

const char *SHADERS_BASE_PATH = "./res/shaders/";
const Color BG = Color(0.2f, 0.3f, 0.3f);

vector<unsigned int> VAOs, VBOs;
vector<Shader> shaderPrograms;
vector<int> pressedKeys;

// Forward declare functions
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processWindowInput(GLFWwindow *window);
void cleanVObjects();
int exit_clean(int const &code, string const &reason);
void clearColor(Color c);
void setupShaders();
void copyVertexObjectsToVector(const unsigned int vaos[], const unsigned int vbos[], const int numObjects);
void setupTriangles();
void drawTrangles(Shader &shader);

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

	setupShaders();

	setupTriangles();

	Shader triangleShader = shaderPrograms[0]; // TODO store/retrieve shaders in a different way

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// handle input events
		processWindowInput(window);

		// render commands
		clearColor(BG);
		drawTrangles(triangleShader);

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

	int wKeyState = glfwGetKey(window, GLFW_KEY_W);
	if (wKeyState == GLFW_PRESS && find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_W) == pressedKeys.end())
	{
		pressedKeys.emplace_back(GLFW_KEY_W);
	}
	if (wKeyState == GLFW_RELEASE && find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_W) != pressedKeys.end())
	{
		cout << "wireframe mode on." << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		pressedKeys.erase(find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_W));
	}

	int fKeyState = glfwGetKey(window, GLFW_KEY_F);
	if (fKeyState == GLFW_PRESS && find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_F) == pressedKeys.end())
	{
		pressedKeys.emplace_back(GLFW_KEY_F);
	}
	if (fKeyState == GLFW_RELEASE && find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_F) != pressedKeys.end())
	{
		cout << "fill mode on." << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		pressedKeys.erase(find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_F));
	}

	int pKeyState = glfwGetKey(window, GLFW_KEY_P);
	if (pKeyState == GLFW_PRESS && find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_P) == pressedKeys.end())
	{
		pressedKeys.emplace_back(GLFW_KEY_P);
	}
	if (pKeyState == GLFW_RELEASE && find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_P) != pressedKeys.end())
	{
		cout << "point mode on." << endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		pressedKeys.erase(find(pressedKeys.begin(), pressedKeys.end(), GLFW_KEY_P));
	}
}

void cleanVObjects()
{
	for (unsigned int vao : VAOs)
	{
		glDeleteVertexArrays(1, &vao);
	}
	for (unsigned int vbo : VBOs)
	{
		glDeleteBuffers(1, &vbo);
	}

	VAOs.clear();
	VBOs.clear();
}

int exit_clean(int const &code, string const &reason)
{
	if (reason != "")
	{
		cout << reason << endl;
	}

	cleanVObjects();

	for (Shader shaderProgram : shaderPrograms)
	{
		glDeleteProgram(shaderProgram.programId);
	}

	glfwTerminate();
	return code;
}

void clearColor(Color c)
{
	glClearColor(c.get(Color::R), c.get(Color::G), c.get(Color::B), c.get(Color::A));
	glClear(GL_COLOR_BUFFER_BIT);
}

void setupShaders()
{
	char vertexPath[1000], fragmentPath[1000];
	strcpy(vertexPath, SHADERS_BASE_PATH);
	strcpy(fragmentPath, SHADERS_BASE_PATH);
	strcat(vertexPath, "vertex.vs");
	strcat(fragmentPath, "fragment.fs");

	Shader triangleShader(vertexPath, fragmentPath);
	shaderPrograms.emplace_back(triangleShader); // TODO store/retrieve Shaders smarter...
}

void copyVertexObjectsToVector(const unsigned int vaos[], const unsigned int vbos[], const int numObjects)
{
	// copy the VOAs and VBOs
	cleanVObjects();

	for (int i = 0; i < numObjects; i++)
	{
		VAOs.emplace_back(vaos[i]);
		VBOs.emplace_back(vbos[i]);
	}
}

void setupTriangles()
{
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

	glGenVertexArrays(3, vaos);
	glGenBuffers(3, vbos);

	copyVertexObjectsToVector(vaos, vbos, 3);

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
}

void drawTrangles(Shader &shader)
{
	for (int i = 0; i < sizeof(VAOs); i++)
	{
		shader.use();
		glBindVertexArray(VAOs[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}