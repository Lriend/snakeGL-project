#pragma once

#include "libraries.h"

//ENUMS
enum SHADER{SHADER_CORE_PROGRAM};
enum TEXTURE{STAR, COLORFULL, CHERRY};
enum MATERIAL{MATERIAL1};
enum MESH{MESH_PLANE};

class Game
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

	//Lights
	std::vector<glm::vec3*> lights;

//Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions(GLenum fillOrLine = GL_FILL, bool culling = true, bool blend = true);
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();

	void updateUniforms();

//Static vars

public:
//Ctor & dtor
	Game(const char* title, const int width, const int height, const int glMajorVer, const int glMinorVer, bool resizable);
	virtual ~Game();

//Getters
	int getWindowShouldClose();

//Setters
	void setWindowShouldClose();

//Functions
	void update();
	void render();

//Static functions
	static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void updateInput(GLFWwindow* window);
	static void updateInput(GLFWwindow* window, Mesh& mesh);
};