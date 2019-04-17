#include "game.h"

//Private functions
void Game::initGLFW()
{
	if (glfwInit() == GLFW_FALSE) { 
		std::cout << "ERROR! GAME.CPP/INITGLFW : GLFW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Game::initWindow(const char* title, bool resizable) {
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac (redundand)

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, glfwGetPrimaryMonitor(), NULL);

	if (this->window == nullptr) {
		std::cout << "ERROR! GAME.CPP/INITWINDOW : WINDOW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);			//<----------------+----	//For non-resizable window
	glfwSetFramebufferSizeCallback(this->window, framebufferResizeCallback);		//<---For resizable window |
	//glViewport(0, 0, framebufferWidth, framebufferHeight); //Canvas	  		  <----------------+

	//Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwMakeContextCurrent(this->window); //Important for GLEW
}

void Game::initGLEW()
{
	//INIT GLEW (REQ WINDOW & OPENGL CONTEXT)
	glewExperimental = GL_TRUE;
	//Error
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR! GAME.CPP/INITGLEW : GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Game::initOpenGLOptions(GLenum fillOrLine, bool culling, bool blend)
{
	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	if (culling) {
		glEnable(GL_CULL_FACE);				//
		glCullFace(GL_BACK);				// Render only front faces of triangles
		glFrontFace(GL_CCW);				//
	}				

	if (blend) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glPolygonMode(GL_FRONT_AND_BACK, fillOrLine); //Could be GL_LINE as well as GL_FILL (then will draw only outlines)
}

void Game::initMatrices()
{	this->ViewMatrix = glm::mat4(1.f);
	ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight, this->nearPlane, this->farPlane);
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		(char*)"VertexShader.glsl", (char*)"FragmentShader.glsl"));
}

void Game::initTextures()
{	
	this->textures.push_back(new Texture("Textures/star.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/colorfull.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/cherry.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/starCube.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/field.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/tailtemptex.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/tailtrantemptex.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), DIFFUSE_TEX, SPECULAR_TEX));

}

void Game::initMeshes()
{
	//this->meshes.push_back(new Mesh(&Plane(), glm::vec3(-3.f, 0.f, -9.75f))); //Fruit prefab
	//this->meshes.push_back(new Mesh(&Cube(), glm::vec3(-9.f, 4.f, -9.75f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f))); //Snake's head prefab
	//this->meshes.push_back(new Mesh(&Quad(), glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f), glm::vec3(20.f, 10.f, 1.f))); //Board prefab
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}

void Game::initBoard(int width, int height)
{
	this->boardPos = glm::vec2(-this->boardWidth/2, -this->boardHeight/2);
	for (int i = 0; i < boardWidth; i++)
		for (int j = 0; j < boardHeight; j++)
			this->board.push_back(new Mesh(&Quad(), glm::vec3(i - this->boardWidth / 2, j - this->boardHeight / 2, -10.f)));
}

void Game::initHead() {
	this->head = new Mesh(&Cube(), glm::vec3(this->boardPos.x, this->boardPos.y, -9.75f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f));
}

void Game::initFruits()
{
	std::srand((unsigned int)time(NULL));
	for (int i = 0; i < this->amountOfFruits; i++)
		this->fruits.push_back(new Mesh(&Plane(), glm::vec3(boardPos.x + std::rand() % boardWidth, boardPos.y + std::rand() % boardHeight, -9.75f)));
}

void Game::initUniforms()
{
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");
}

void Game::updateUniforms()
{
	//Update uniforms
	this->materials[MATERIAL1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	//Update framebuffersize & ProjectionMatrix in case of resizing
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight, this->nearPlane, this->farPlane);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Ctor & dtor
Game::Game(const char* title, const int width, const int height, const int glMajorVer, const int glMinorVer, bool resizable, int boardWidth, int boardHeight, int amountOfFruits)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VERSION_MAJOR(glMajorVer), GL_VERSION_MINOR(glMinorVer)
{
	//Init vars
	this->boardWidth = boardWidth;
	this->boardHeight = boardHeight;

	this->amountOfFruits = amountOfFruits;

	this->direction = RIGHT;
	this->shouldGrow = true;

	this->window = nullptr;
	this->frameBufferHeight = this->WINDOW_HEIGHT;
	this->frameBufferWidth = this->WINDOW_WIDTH;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions(GL_FILL);
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	//this->initMeshes();
	this->initLights();
	this->initUniforms();
	this->initBoard(this->boardWidth, this->boardHeight);
	this->initHead();
	this->growTail();
	this->initFruits();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	for (size_t i = 0; i < this->shaders.size(); i++) delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++) delete this->textures[i];
	for (size_t i = 0; i < this->materials.size(); i++) delete this->materials[i];
	for (size_t i = 0; i < this->meshes.size(); i++) delete this->meshes[i];
	for (size_t i = 0; i < this->lights.size(); i++) delete this->lights[i];
	for (size_t i = 0; i < this->board.size(); i++) delete this->board[i];
	for (size_t i = 0; i < this->tail.size(); i++) delete this->tail[i];
	for (size_t i = 0; i < this->fruits.size(); i++) delete this->fruits[i];
}

//Getters
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Setters
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Game::update()
{
	//INTERACTING WITH WINDOW
	glfwPollEvents();
	updateInput(this->window);
	
	//UPDATE GAME INPUT
	//updateInput(this->window, *this->head);
	updateDirection();

	//UPDATE GAME
	if (shouldGrow) growTail();
	else moveTail();
	moveSnake();
	updateFruits();

	//Rotate fruits
	//for (size_t i = 0; i < this->fruits.size(); i++)	this->fruits[i]->rotate(glm::vec3(0.f, 5.f, 0.f));
	//this->meshes[CUBE]->rotate(glm::vec3(0.2f, 0.f, 0.f));

	Sleep(100);
}

void Game::render()
{
	//Clear
	glClearColor(0.2f, 0.2f, 0.2f, 1.f); // (R,G,B,Opacity) BACKGROUND
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update uniforms
	this->updateUniforms();

	//Use a program
	this->shaders[SHADER_CORE_PROGRAM]->use();

	//Activate textures and draw stuff
	drawBoard();
	drawSnake();
	drawFruits();

	//End Draw
	glfwSwapBuffers(this->window); //Swap frames
	glFlush(); //glFlush -> GL. Flush.

	//Reset
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::drawBoard()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	this->textures[FIELD]->bind(DIFFUSE_TEX);
	for (size_t i = 0; i < this->board.size(); i++)
		this->board[i]->render(this->shaders[SHADER_CORE_PROGRAM]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::drawSnake()
{
	//STAR CUBE same ting
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	this->textures[STAR_CUBE]->bind(DIFFUSE_TEX);
	this->textures[CHERRY]->bind(SPECULAR_TEX);
	this->head->render(this->shaders[SHADER_CORE_PROGRAM]);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	this->textures[TAIL_RGB]->bind(DIFFUSE_TEX);
	this->textures[TAIL_RGBA]->bind(SPECULAR_TEX);
	for (size_t i = 0; i < this->tail.size(); i++)
		this->tail[i]->render(this->shaders[SHADER_CORE_PROGRAM]);
}

void Game::drawFruits()
{
	//CHERRY PLANE yes.
	this->textures[CHERRY]->bind(DIFFUSE_TEX);
	this->textures[COLORFULL]->bind(SPECULAR_TEX);
	for (size_t i = 0; i < this->fruits.size(); i++)
		this->fruits[i]->render(this->shaders[SHADER_CORE_PROGRAM]);

}

void Game::moveSnake()
{
	if  (
		this->head->getPosition().y < this->boardPos.y + this->boardHeight
		&& this->head->getPosition().y >= this->boardPos.y
		&& this->head->getPosition().x >= this->boardPos.x
		&& this->head->getPosition().x < this->boardPos.x + this->boardWidth
		)
	switch (this->direction)
	{
	case UP: this->head->move(glm::vec3(0.f, 1.f, 0.f)); break;
	case DOWN: this->head->move(glm::vec3(0.f, -1.f, 0.f)); break;
	case LEFT: this->head->move(glm::vec3(-1.f, 0.f, 0.f)); break;
	case RIGHT: this->head->move(glm::vec3(1.f, 0.f, 0.f)); break;
	}
}

void Game::moveTail()
{
	this->tail.push_back(new Mesh(&Cube(), glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -9.75f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f)));
	tail.pop_front();
}

void Game::growTail()
{
	this->tail.push_back(new Mesh(&Cube(), glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -9.75f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f)));
	this->shouldGrow = false;
}

void Game::updateFruits()
{
	for (size_t i = 0; i < this->fruits.size(); i++)
		if (this->fruits[i]->getPosition() == this->head->getPosition())
		{
			this->shouldGrow = true;
			while (this->fruits[i]->getPosition() == this->head->getPosition()) //DOPISAC OGON!!!
				this->fruits[i]->setPosition(glm::vec3(boardPos.x + std::rand() % boardWidth, boardPos.y + std::rand() % boardHeight, -9.75f));
		}
}

void Game::updateDirection()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && tail.back()->getPosition().y <= head->getPosition().y) this->direction = UP;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && tail.back()->getPosition().y >= head->getPosition().y) this->direction = DOWN;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && tail.back()->getPosition().x >= head->getPosition().x) this->direction = LEFT;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && tail.back()->getPosition().x <= head->getPosition().x) this->direction = RIGHT;
}

//Static functions
void Game::framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void Game::updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void Game::updateInput(GLFWwindow* window, Mesh& mesh) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mesh.move(glm::vec3(0.f, 1.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mesh.move(glm::vec3(0.f, -1.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mesh.move(glm::vec3(-1.f, 0.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mesh.move(glm::vec3(1.f, 0.f, 0.f));
	Sleep(100);
	//PRZEROBIC NA ASYNC BO WSTYD XD

	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) mesh.rotate(glm::vec3(0.1f, 0.f, 0.f));
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) mesh.rotate(glm::vec3(0.f, -0.1f, 0.f));
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) mesh.rotate(glm::vec3(0.f, 0.1f, 0.f));
	//if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) mesh.scaleUp(glm::vec3(0.001f));
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) mesh.scaleUp(glm::vec3(-0.001f));
}
