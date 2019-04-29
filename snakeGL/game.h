#pragma once

#include "libraries.h"

//ENUMS
enum SHADER{SHADER_CORE_PROGRAM};
enum TEXTURE{COLORFULL, CHERRY, STAR_CUBE, FIELD, TAIL_RGB, TAIL_STRAIGHT, TAIL_TORIGHT, TAIL_TOLEFT, TAIL_END, BROWN, RED};
enum MESH{MESH_PLANE, MESH_CUBE, MESH_FIELD};
enum DIRECTION{UP, DOWN, LEFT, RIGHT};
enum GAME_STATE{MENU, FAST_GAME, CUSTOM_GAME};

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
	float freezeFor;
	float weNeedANewPlague;

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
	std::deque<Model*> tail;
	int direction;
	bool shouldGrow;
	bool gameOver;
	float speed;
	float tick;

	//Collectibles
	int amountOfFruits;
	std::vector<Model*> fruits;
	std::vector<Model*> bonus;
	std::vector<unsigned> bonusType;
	int bonusFreq;

	//Objects
	std::vector<Object*> objects;

	//Manager
	bool pause;
	GAME_STATE state;

//Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions(GLenum fillOrLine = GL_FILL, bool culling = true, bool blend = true);
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();

	//initState
	void init(GAME_STATE state);
	void initObjects();
	void initModels();
	void initBoard(int width, int height);
	void initHead();
	void initFruits();


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
//Snake
//Update update
	void updateFruits();
	void updateBonuses();
	void updateDirection();
	void updateGameOver();

//Update render
	void moveSnake();
	void moveTail();
	void growTail();

//MENU

//Render
	void drawBoard();
	void drawSnake();
	void drawFruits();
	void drawBonuses();
//Update input
	void updateMouseInput();
	void updateKeyboardInput();
	void updateDeltaTime();

//Static functions
	static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void updateInput(GLFWwindow* window);
	static void updateInput(GLFWwindow* window, Mesh& mesh);
};