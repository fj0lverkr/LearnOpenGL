#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <map>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "graphics/Color.hpp"
#include "graphics/Shader.hpp"
#include "util/Text.hpp"

using namespace std;

constexpr int GLFW_MAJOR_VERSION = 3;
constexpr int GLFW_MINOR_VERSION = 3;

const char *SHADERS_BASE_PATH = "./res/shaders/";
const char *TEXTURES_BASE_PATH = "./res/textures/";
const string TEX_CONTAINER = "TextureContainer";

enum SHADERS
{
	SHA_TRI_RBW,
	SHA_TRI_CON
};

const Color BG = Color(0.2f, 0.3f, 0.3f);

vector<unsigned int> VAOs, VBOs, EBOs;
vector<int> pressedKeys;
map<SHADERS, Shader> shaderPrograms;
map<string, unsigned int> textures;

// Forward declare functions
void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processWindowInput(GLFWwindow *window);
void cleanVObjects();
int exit_clean(int const &code, string const &reason);
void clearColor(Color c);
void setupShader(const char *vertexFileName, const char *fragmentFileName, const SHADERS &ShaderId);
void setupTexture(const char *fileName, const string &textureName);
void setupTriangles();
void drawTrangles(Shader &shader, const unsigned int &texture);

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

	setupTexture("container.jpg", TEX_CONTAINER);

	setupShader("vertex.vs", "fragment.fs", SHADERS::SHA_TRI_RBW);
	setupShader("vertex_with_texture.vs", "fragment_with_texture.fs", SHADERS::SHA_TRI_CON);

	setupTriangles();

	Shader triangleShader = shaderPrograms.at(SHA_TRI_CON);
	unsigned int texture = textures[TEX_CONTAINER];

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// handle input events
		processWindowInput(window);

		// render commands
		clearColor(BG);
		drawTrangles(triangleShader, texture);

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
	for (unsigned int ebo : EBOs)
	{
		glDeleteBuffers(1, &ebo);
	}

	VAOs.clear();
	VBOs.clear();
	EBOs.clear();
}

int exit_clean(int const &code, string const &reason)
{
	if (reason != "")
	{
		cout << reason << endl;
	}

	cleanVObjects();

	for (auto const &[_, shaderProgram] : shaderPrograms)
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

void setupShader(const char *vertexFileName, const char *fragmentFileName, const SHADERS &ShaderId)
{
	char *vertexPath = CharUtil::concat(SHADERS_BASE_PATH, vertexFileName);
	char *fragmentPath = CharUtil::concat(SHADERS_BASE_PATH, fragmentFileName);

	Shader shader(vertexPath, fragmentPath);
	shaderPrograms.insert_or_assign(ShaderId, shader);
}

void setupTexture(const char *fileName, const string &textureName)
{
	char *containerPath = CharUtil::concat(TEXTURES_BASE_PATH, fileName);
	int width, height, nrChannels;
	unsigned int texture;
	unsigned char *data = stbi_load(containerPath, &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		textures[textureName] = texture;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void setupTriangles()
{
	unsigned int VAO, VBO, EBO;
	float vertices[] = {
		// positions      // colors         // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f	  // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// first triangle setup
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	VAOs.emplace_back(VAO);
	VBOs.emplace_back(VBO);
	EBOs.emplace_back(EBO);
}

void drawTrangles(Shader &shader, const unsigned int &texture)
{
	for (int i = 0; i < sizeof(VAOs); i++)
	{
		shader.use();
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAOs[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}