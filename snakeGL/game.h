#pragma once
#include "libraries.h"

//ENUMS
enum SHADER{SHADER_CORE_PROGRAM};

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

	GAME_STATE state;

	//Mouse Input
	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool mouseInit;

	//Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions(GLenum fillOrLine = GL_FILL, bool culling = true, bool blend = true);

	int boardWidth;
	int boardHeight;

	//Meshes
	std::vector<Mesh*> meshes;

	//Models
	std::vector<Model*> models;

	//Objects
	std::vector<Object*> objects;

	//Delta time
	float deltaTime;
	float curTime;
	float lastTime;
	float freezeFor;
	float weNeedANewPlague;

	//Board
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
	Model* bonus;
	unsigned bonusType;
	int bonusFreq;
	bool gimmeBonus;
	float invertFor;
	float fastFor;
	float slowFor;
	int goldenApple;
	float bonusTime;

	//Manager
	bool pause;
	bool wireframed;
	void reset();

	int fbW, fbH;

	//initState
	void initObjects();
	void initModels();
	void initBoard(int width, int height);
	void initHead();
	void initTail();
	void initFruits();

	int GLmaj, GLmin;
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

	//Materials
	std::vector<Material*> materials;

	//Lights
	std::vector<glm::vec3*> lights;

	//Textures
	std::vector<Texture*> textures;

	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();

	void initLights();
	void initUniforms();

	void updateUniforms();


	void updateFruits();
	void updateBonus();
	void updateDirection();
	void updateGameOver();

	void moveSnake();
	void moveTail();
	void growTail();

	void drawBoard();
	void drawSnake();
	void drawFruits();
public:
	void init(GAME_STATE state);
	void handleGameEvents();
	void updateDeltaTime();



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
	//MENU
	//Update input
	void updateMouseInput();

	//Static functions
	static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
	static void updateInput(GLFWwindow* window);
	static void updateInput(GLFWwindow* window, Mesh& mesh);
};