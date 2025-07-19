#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

constexpr int GLFW_MAJOR_VERSION = 3;
constexpr int GLFW_MINOR_VERSION = 3;

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

int exit_clean(int const &code, string const &reason = "")
{
	if (reason != "")
	{
		cout << reason << endl;
	}

	glfwTerminate();
	return code;
}

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

	while (!glfwWindowShouldClose(window))
	{
		processWindowInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	exit_clean(0);
}