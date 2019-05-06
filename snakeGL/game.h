#pragma once
#include "libraries.h"

//ENUMS
enum DIRECTION { UP, DOWN, LEFT, RIGHT };
enum GAME_STATE { MENU, GAME };
enum GAME_TYPE { QUICK, CLASSIC, CUSTOM };
enum BONUS { BUNNY, MUSHROOM, TURTLE, BOMB, APPLE, QUESTION };
enum MENU_ELEMENT { QUICK_GAME, CLASSIC_GAME, CUSTOM_GAME, SCOREBOARD, SETTINGS, EXIT };
enum CUSTOMIZABLE{SPEED, TIME, FREQ, WIDTH, HEIGHT, FRUITS};

class Game
{
	//Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int frameBufferWidth;
	int frameBufferHeight;

	//OpenGL context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	void updateNums(int customizable);
	Mesh* customNumsUI[6][3];
	std::vector<Mesh*> GUIelements;
	std::vector<Mesh*> subMenuElements[6];
	std::vector<Mesh*> pauseElements;
	void initGUI();
	GAME_STATE gameState;
	GAME_TYPE gameType;
	unsigned menuElement;
	unsigned subMenuElement;
	bool subMenu;
	Object* nums[10];

	bool iWantBunny;
	bool iWantShrooms;
	bool iWantTurtles;
	bool iWantBombs;
	bool iWantApple;
	bool surpriseMe;

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

	int customHeight;
	int customWidth;

	bool customizing;

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
	int customFruits;
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
	void setGame();

	//initState
	void initObjects();
	void initModels();
	void initBoard();
	void initHead();
	void initTail();
	void initFruits();

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

	void updateDeltaTime();
	void handleGameEvents();
	void handleMenuEvents();
	void handleSubMenuEvents();
	void handlePauseEvents();

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