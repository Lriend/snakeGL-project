#pragma once

#include "libraries.h"

class Menu
{
	//Mamber vars
		//Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int frameBufferWidth;
	int frameBufferHeight;

	//OpenGL context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Matrices
	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;

	//Camera
	float fov;
	float nearPlane;
	float farPlane;

	//Shaders
	std::vector<Shader*> shaders;

	//Textures
	std::vector<Texture*> textures;

	//Materials
	std::vector<Material*> materials;

	//Meshes
	std::vector<Mesh*> meshes;

	//Models
	std::vector<Model*> models;

	//Lights
	std::vector<glm::vec3*> lights;

	//Board
	int boardWidth;
	int boardHeight;

	//Objects
	std::vector<Object*> objects;

	//Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions(GLenum fillOrLine = GL_FILL, bool culling = true, bool blend = true);
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initObjects();
	void initModels();
	void initLights();
	void initUniforms();

	void updateUniforms();
	//Static vars

public:
	//Ctor & dtor
	Menu(const char* title, const int width, const int height, const int glMajorVer, const int glMinorVer, bool resizable);
	virtual ~Menu();

	//Getters
	int getWindowShouldClose();

	//Setters
	void setWindowShouldClose();

	//Functions
	void update();
	void render();

	void updateKeyboardInput();

	//Static functions
	static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void updateInput(GLFWwindow* window);
};