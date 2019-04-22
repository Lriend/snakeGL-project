#pragma once

#include "libraries.h"

//ENUMS
enum SHADER{SHADER_CORE_PROGRAM};
enum TEXTURE{CHERRY, STAR_CUBE, FIELD, TAIL_RGB};
enum MATERIAL{MATERIAL1};
enum MESH{MESH_PLANE, MESH_CUBE, MESH_FIELD};
//enum TEXTURE_UNITS{DIFFUSE_TEX, SPECULAR_TEX};
enum DIRECTION{UP, DOWN, LEFT, RIGHT};

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

	//Delta time
	float deltaTime;
	float curTime;
	float lastTime;

	//Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool mouseInit;

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
	glm::vec2 boardPos;
	std::vector<Mesh*> board;
	
	//Snake
	Mesh* head;
	std::deque<Mesh*> tail;
	int direction;
	bool shouldGrow;
	bool gameOver;

	//Fruits
	int amountOfFruits;
	std::vector<Mesh*> fruits;

//Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions(GLenum fillOrLine = GL_FILL, bool culling = true, bool blend = true);
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initModels();
	void initLights();
	void initBoard(int width, int height);
	void initUniforms();
	void initHead();
	void initFruits();

	void updateUniforms();
//Static vars

public:
//Ctor & dtor
	Game(const char* title, const int width, const int height, const int glMajorVer, const int glMinorVer, bool resizable, int boardWidth, int boardHeight, int amountOfFruits);
	virtual ~Game();

//Getters
	int getWindowShouldClose();

//Setters
	void setWindowShouldClose();

//Functions
	void update();
	void render();

	void drawBoard();
	void drawSnake();
	void drawFruits();

	void moveSnake();
	void moveTail();
	void growTail();

	void updateFruits();
	void updateDirection();
	void updateGameOver();

	void updateMouseInput();
	void updateDeltaTime();

//Static functions
	static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void updateInput(GLFWwindow* window);
	static void updateInput(GLFWwindow* window, Mesh& mesh);
};