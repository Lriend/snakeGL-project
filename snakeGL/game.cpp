#include "game.h"

void Game::reset()
{

	this->boardWidth > this->boardHeight ? this->boardHeight++ : this->boardWidth++;
	this->amountOfFruits = 3;

	//reset
	this->board.clear();
	this->initBoard();
	this->camPosition = glm::vec3(0.f, 0.f, boardWidth > boardHeight ? (float)boardWidth / 2 - 9 : (float)boardHeight / 2 - 9); //set Cam position
	this->initMatrices(); //Cam position updated in matrices
	this->models.clear();
	this->initModels();
	this->initUniforms();
	this->gameOver = false;
	this->direction = RIGHT;
	this->initHead();
	this->tail.clear();
	this->initTail();
	this->shouldGrow = false;
	this->tick = 0.5f;
	this->speed = 1.f;
	this->gimmeBonus = true;
	this->invertFor = 0.f;
	this->fastFor = 0.f;
	this->slowFor = 0.f;
	this->fruits.clear();
	this->initFruits();
	if (this->bonus) this->bonus->~Model();
	this->bonusType = 0;
	this->goldenApple = 0;
	this->weNeedANewPlague = 0;

}

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
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac (redundand)

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, resizable ? NULL : glfwGetPrimaryMonitor(), NULL);

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

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
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
	//this->textures.push_back(new Texture("Textures/star.png", GL_TEXTURE_2D));
	//this->textures.push_back(new Texture("Textures/colorfull.png", GL_TEXTURE_2D));
	//this->textures.push_back(new Texture("Textures/cherry.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/starCube.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/field.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/tailtemptex.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/straight.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/toRight.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/toLeft.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/end.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/red.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/gray.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/brown.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/green.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/black.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/golden.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/blue.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/yellow.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/dark.png", GL_TEXTURE_2D));
	//this->textures.push_back(new Texture("Textures/tailtrantemptex.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), DIFFUSE_TEX, SPECULAR_TEX));
}

void Game::initObjects()
{
	this->objects.push_back(new Object("Objects/straightFR.obj"));
	this->objects.push_back(new Object("Objects/cornerToLeftFR.obj"));
	this->objects.push_back(new Object("Objects/cornerToRightFR.obj"));
	this->objects.push_back(new Object("Objects/endFR.obj"));
	this->objects.push_back(new Object("Objects/headBG.obj"));
	this->objects.push_back(new Object("Objects/cherry.obj"));
	this->objects.push_back(new Object("Objects/bunny.obj"));
	this->objects.push_back(new Object("Objects/shroom.obj"));
	this->objects.push_back(new Object("Objects/turtle2.obj"));
	this->objects.push_back(new Object("Objects/bomb.obj"));
	this->objects.push_back(new Object("Objects/apple.obj"));
	this->objects.push_back(new Object("Objects/question.obj"));
	this->objects.push_back(new Object("Objects/nMonkey.obj"));
	this->objects.push_back(new Object("Objects/snakeGL.obj"));
	this->objects.push_back(new Object("Objects/gameover.obj"));
	this->objects.push_back(new Object("Objects/restart.obj"));
	this->objects.push_back(new Object("Objects/pause.obj"));
}

void Game::initModels()
{
	this->meshes.push_back(new Mesh(objects[12], glm::vec3((float)boardWidth / 2 + 2.5f, -(float)boardHeight / 2 + 4, -10.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	this->meshes.push_back(new Mesh(objects[13], glm::vec3((float)boardWidth / 2 + 1.f, -(float)boardHeight / 2 + 1.f, -10.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	float temp = boardHeight > boardWidth ? (float)boardHeight / 10 : (float)boardWidth / 10;
	this->meshes.push_back(new Mesh(objects[14], glm::vec3((float)boardPos.x + this->boardWidth / 2, (float)boardPos.y + this->boardHeight / 2, -9.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	this->meshes.push_back(new Mesh(objects[15], glm::vec3((float)boardPos.x + this->boardWidth / 2, (float)boardPos.y + this->boardHeight / 2, -9.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[RED], this->textures[RED], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	this->meshes.push_back(new Mesh(objects[16], glm::vec3((float)boardPos.x + this->boardWidth / 2, (float)boardPos.y + this->boardHeight / 2, -9.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[BLUE], this->textures[BLUE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
}


void Game::initBoard()
{
	this->boardPos = glm::vec2(-this->boardWidth / 2, -this->boardHeight / 2);
	for (int i = 0; i < boardWidth; i++)
		for (int j = 0; j < boardHeight; j++)
			this->board.push_back(new Mesh(&Quad(), glm::vec3(i - this->boardWidth / 2, j - this->boardHeight / 2, -10.f)));
}

void Game::initHead() {
	this->head = new Mesh(objects[4], glm::vec3(this->boardPos.x, this->boardPos.y, -10.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f));
}

void Game::initTail()
{
	this->meshes.push_back(new Mesh(objects[3], glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, -10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 0.5f)));
	this->tail.push_back(new Model(glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, -10.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
}

void Game::initFruits()
{
	std::srand((unsigned int)time(NULL));
	for (int i = 0; i < this->amountOfFruits; i++) {
		this->meshes.push_back(new Mesh(objects[5], glm::vec3(boardPos.x + std::rand() % boardWidth, boardPos.y + std::rand() % boardHeight, -10.f)));
		this->fruits.push_back(new Model(meshes.empty() ? glm::vec3(0.f) : meshes[0]->getPosition(), this->materials[0], this->textures[RED], this->textures[RED], this->meshes));
		for (auto*&i : this->meshes) delete i;
		this->meshes.clear();
	}
}


void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
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

	//Update framebuffersize & ProjectionMatrix in case of resizing
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight, this->nearPlane, this->farPlane);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Ctor & dtor
Game::Game(const char* title, const int width, const int height, const int glMajorVer, const int glMinorVer, bool resizable)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VERSION_MAJOR(glMajorVer), GL_VERSION_MINOR(glMinorVer)
{
	//Init vars
	this->gameState = GAME;
	this->gameType = FAST;

	this->boardWidth = 3;
	this->boardHeight = 4;

	this->amountOfFruits = 3;
	this->bonusFreq = 25;
	this->bonusTime = 0.f;

	this->tick = 0.5f;
	this->speed = 1.f;
	this->direction = RIGHT;
	this->shouldGrow = false;
	this->gimmeBonus = true;

	this->gameOver = false;
	this->pause = false;

	this->window = nullptr;
	this->frameBufferHeight = this->WINDOW_HEIGHT;
	this->frameBufferWidth = this->WINDOW_WIDTH;

	this->wireframed = false;

	this->camPosition = glm::vec3(0.f, 0.f, boardWidth > boardHeight ? (float)boardWidth / 2 - 9 : (float)boardHeight / 2 - 9);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->deltaTime = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;
	this->weNeedANewPlague = 0.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions(GL_FILL, false);
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();

	//Snook inits
	this->initObjects();
	this->initBoard();
	this->initModels();
	this->initHead();
	this->initTail();
	this->initFruits();

	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	for (size_t i = 0; i < this->shaders.size(); i++) delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++) delete this->textures[i];
	for (size_t i = 0; i < this->materials.size(); i++) delete this->materials[i];
	for (auto*&i : this->objects) delete i;
	for (auto*&i : this->models) delete i;
	for (auto*&i : this->board) delete i;
	for (auto*&i : this->tail) delete i;
	for (auto*&i : this->fruits) delete i;
	for (size_t i = 0; i < this->lights.size(); i++) delete this->lights[i];
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
	//UPDATE DELTA TIME
	this->updateDeltaTime();

	//INTERACTING WITH WINDOW
	updateInput(this->window);

	//UPDATE MOUSE INPUT
	updateMouseInput();

	handleGameEvents();
	if (!pause) {
		if (this->fastFor > 0) { this->fastFor -= deltaTime; this->speed = 2.f; }
		else
			if (this->slowFor > 0) { this->slowFor -= deltaTime; this->speed = .5f; }
			else
				this->speed = 1.f;
		if (this->invertFor > 0) { this->invertFor -= deltaTime; this->wireframed = true; }
		else
			this->wireframed = false;
		if (bonusTime) this->bonusTime -= deltaTime;
		//UPDATE GAME INPUT
		updateDirection();
		updateGameOver();
		this->freezeFor = this->tick / speed; //Speed

		//UPDATE GAME
		if (this->weNeedANewPlague >= this->freezeFor && !gameOver) {
			updateBonus();
			updateFruits();
			if (this->goldenApple) { this->goldenApple--; this->shouldGrow = true; }
			if (shouldGrow) growTail();
			else moveTail();
			moveSnake();
			this->weNeedANewPlague = 0;
		}
		else this->weNeedANewPlague += this->deltaTime;
		if (this->deltaTime) this->models[0]->rotate(glm::vec3(0.f, 25.f, 0.f)*this->deltaTime);
	}
}

void Game::render()
{
	//Clear
	glClearColor(0.2f,0.2f,0.2f, 1.f); // (R,G,B,Opacity) BACKGROUND
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update uniforms
	this->updateUniforms();
	//this->materials[MATERIAL1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	//Use a program
	this->shaders[SHADER_CORE_PROGRAM]->use();
	//Activate textures and draw stuff
	drawBoard();
	drawSnake();
	drawFruits();
	if (this->bonus)this->bonus->render(this->shaders[SHADER_CORE_PROGRAM]);

	//Object from file attempt Monkey
	this->models[0]->render(this->shaders[SHADER_CORE_PROGRAM]);
	this->models[1]->render(this->shaders[SHADER_CORE_PROGRAM]);
	if (this->gameOver) {
		this->models[2]->render(this->shaders[SHADER_CORE_PROGRAM]);
		this->models[3]->render(this->shaders[SHADER_CORE_PROGRAM]);
	}
	if (this->pause) this->models[4]->render(this->shaders[SHADER_CORE_PROGRAM]);

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
	this->textures[FIELD]->bind(SPECULAR_TEX); //Do podmiany tekstura specular
	for (size_t i = 0; i < this->board.size(); i++)
		this->board[i]->render(this->shaders[SHADER_CORE_PROGRAM]);
	if (!wireframed)glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Czy wireframe czy fill to tutaj chyba zrobimy, nie? Prostego ifa jakiegos ;)
}

void Game::drawSnake()
{
	//STAR CUBE same ting
	this->textures[WHITE]->bind(DIFFUSE_TEX);
	this->textures[WHITE]->bind(SPECULAR_TEX); //Do podmiany tekstura specular
	this->head->render(this->shaders[SHADER_CORE_PROGRAM]);

	if (!this->tail.empty())
		for (size_t i = 0; i < this->tail.size(); i++) {
			this->tail[i]->render(this->shaders[SHADER_CORE_PROGRAM]);
		}
}

void Game::drawFruits()
{
	//CHERRY PLANE yes.
	for (size_t i = 0; i < this->fruits.size(); i++)
		this->fruits[i]->render(this->shaders[SHADER_CORE_PROGRAM]);

}


void Game::moveSnake()
{
	if (!gameOver)
		switch (this->direction)
		{
		case UP: this->head->move(glm::vec3(0.f, 1.f, 0.f)); this->head->setRotation(glm::vec3(0.f, 90.f, 90.f)); break;
		case DOWN: this->head->move(glm::vec3(0.f, -1.f, 0.f)); this->head->setRotation(glm::vec3(0.f, 270.f, 270.f)); break;
		case LEFT: this->head->move(glm::vec3(-1.f, 0.f, 0.f)); this->head->setRotation(glm::vec3(90.f, 180.f, 0.f)); break;
		case RIGHT: this->head->move(glm::vec3(1.f, 0.f, 0.f)); this->head->setRotation(glm::vec3(90.f, 0.f, 0.f)); break;
		}
}

void Game::moveTail()
{
	this->growTail();
	this->tail.pop_front();

	//END
	glm::vec3 backPosTemp = this->tail.front()->getOrigin();
	float textureRotation = this->tail.front()->getRotationAtZ(0);
	this->tail.pop_front();
	this->meshes.push_back(new Mesh(objects[3], backPosTemp, glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
	this->tail.push_front(new Model(backPosTemp, this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
}

void Game::growTail()
{
	int texture;
	float textureRotation;
	switch (this->direction) {
	case UP:
		textureRotation = 90.f;
		break;
	case DOWN:
		textureRotation = 270.f;
		break;
	case LEFT:
		textureRotation = 180.f;
		break;
	case RIGHT:
		textureRotation = 0.f;
		break;
	}
	glm::vec3 headPosTemp = this->head->getPosition();
	if (!this->tail.empty()) {
		if (
			(this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x - 1.f, headPosTemp.y, -10.f) && this->direction == RIGHT)
			|| (this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x + 1.f, headPosTemp.y, -10.f) && this->direction == LEFT)
			|| (this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x, headPosTemp.y - 1.f, -10.f) && this->direction == UP)
			|| (this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x, headPosTemp.y + 1.f, -10.f) && this->direction == DOWN)
			) texture = TAIL_STRAIGHT;
		else if (
			(this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x - 1.f, headPosTemp.y, -10.f) && this->direction == UP)
			|| (this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x + 1.f, headPosTemp.y, -10.f) && this->direction == DOWN)
			|| (this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x, headPosTemp.y - 1.f, -10.f) && this->direction == LEFT)
			|| (this->tail.back()->getOrigin() == glm::vec3(headPosTemp.x, headPosTemp.y + 1.f, -10.f) && this->direction == RIGHT)
			) texture = TAIL_TOLEFT;
		else texture = TAIL_TORIGHT;
	}
	switch (this->tail.empty() ? TAIL_END : texture)
	{
	case TAIL_STRAIGHT:
		this->meshes.push_back(new Mesh(objects[0], glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -10.f), glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
		break;
	case TAIL_TOLEFT:
		this->meshes.push_back(new Mesh(objects[1], glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -10.f), glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
		break;
	case TAIL_TORIGHT:
		this->meshes.push_back(new Mesh(objects[2], glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -10.f), glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
		break;
	case TAIL_END:
		this->meshes.push_back(new Mesh(objects[3], glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -10.f), glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
		break;
	}
	//this->meshes.push_back(new Mesh(&Cube(), glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -9.75f), glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
	//this->meshes.push_back(new Mesh(&Object("Objects/straight.obj"), glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -9.75f), glm::vec3(0.f, 0.f, textureRotation), glm::vec3(1.f, 1.f, 0.5f)));
	this->tail.push_back(new Model(glm::vec3(this->head->getPosition().x, this->head->getPosition().y, -10.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	this->shouldGrow = false;

}

void Game::updateFruits()
{
	glm::vec3 fruitPotPos(0.f);
	switch (this->direction)
	{
	case UP: fruitPotPos = glm::vec3(this->head->getPosition().x, this->head->getPosition().y + 1.f, this->head->getPosition().z); break;
	case DOWN: fruitPotPos = glm::vec3(this->head->getPosition().x, this->head->getPosition().y - 1.f, this->head->getPosition().z); break;
	case LEFT: fruitPotPos = glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, this->head->getPosition().z);  break;
	case RIGHT: fruitPotPos = glm::vec3(this->head->getPosition().x + 1.f, this->head->getPosition().y, this->head->getPosition().z); break;
	}
	for (size_t i = 0; i < this->fruits.size(); i++) {
		bool getOutOfHere = false;
		/*for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		if (this->head->getPosition() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		for (size_t j = 0; j < this->fruits.size(); j++) if (this->fruits[j]->getOrigin() == this->fruits[i]->getOrigin() && i != j) getOutOfHere = true;
		if (tail.size() < board.size() - amountOfFruits - 5) if (this->bonus) if (this->bonus->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		*/while (this->fruits[i]->getOrigin() == fruitPotPos || getOutOfHere) {
			getOutOfHere = false;
			if (this->bonus && this->tail.size() >= this->board.size() - fruits.size() - 5) { this->bonus->~Model(); this->gimmeBonus = false; this->bonusType = 0; }
			if (tail.size() >= board.size() - amountOfFruits-1) { amountOfFruits--; fruits.at(i)->~Model(); this->shouldGrow = true; break; }
			this->shouldGrow = true;
			this->fruits[i]->setOrigin(glm::vec3(boardPos.x + std::rand() % boardWidth, boardPos.y + std::rand() % boardHeight, -10.f));
			for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
			if (this->head->getPosition() == this->fruits[i]->getOrigin()) getOutOfHere = true;
			for (size_t j = 0; j < this->fruits.size(); j++) if (this->fruits[j]->getOrigin() == this->fruits[i]->getOrigin() && i != j) getOutOfHere = true;
			if (tail.size() < board.size() - amountOfFruits - 5) if (this->bonus) if (this->bonus->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		}
	}
}

void Game::updateBonus()
{
	glm::vec3 fruitPotPos(0.f);
	switch (this->direction)
	{
	case UP: fruitPotPos = glm::vec3(this->head->getPosition().x, this->head->getPosition().y + 1.f, this->head->getPosition().z); break;
	case DOWN: fruitPotPos = glm::vec3(this->head->getPosition().x, this->head->getPosition().y - 1.f, this->head->getPosition().z); break;
	case LEFT: fruitPotPos = glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, this->head->getPosition().z);  break;
	case RIGHT: fruitPotPos = glm::vec3(this->head->getPosition().x + 1.f, this->head->getPosition().y, this->head->getPosition().z); break;
	}
	if(bonus) for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == this->bonus->getOrigin()) this->bonusType = 0;
	if (this->bonusType == 0 && this->bonus) this->bonus->~Model();
	if (tail.size() >= board.size() - amountOfFruits - 5) gimmeBonus = false;
	std::srand((unsigned int)time(NULL));
	if (tail.size() < board.size() - amountOfFruits - 5)
		if (rand() % this->bonusFreq == 0 && this->gimmeBonus) {
			this->bonusTime = 10.f;
			unsigned type = 6 + rand() % 6;
			bool dont = true;
			float potX, potY;
			while (dont) {
				dont = false;
				potX = boardPos.x + std::rand() % boardWidth;
				potY = boardPos.y + std::rand() % boardHeight;
				for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == glm::vec3(potX, potY, -10.f)) dont = true;
				for (size_t j = 0; j < this->fruits.size(); j++) if (this->fruits[j]->getOrigin() == glm::vec3(potX, potY, -10.f)) dont = true;
				if (this->head->getPosition() == glm::vec3(potX, potY, -10.f)) dont = true;
				if (fruitPotPos == glm::vec3(potX, potY, -10.f)) dont = true;
			}
			this->meshes.push_back(new Mesh(objects[type], glm::vec3(potX, potY, -10.f)));
			this->bonus = new Model(meshes.empty() ? glm::vec3(0.f) : meshes[0]->getPosition(), this->materials[0], this->textures[2 + type], this->textures[2 + type], this->meshes);
			for (auto*&i : this->meshes) delete i;
			this->meshes.clear();
			if (type == 11) type = 6 + rand() % 5;
			this->bonusType = type;
			this->gimmeBonus = false;
		}
	if (tail.size() < board.size() - amountOfFruits - 10) {
		if (this->bonus) {
			if (this->bonus->getOrigin() == fruitPotPos) {
				switch (bonusType) {
				case 6:
					this->fastFor = 10.f;
					break;
				case 7:
					this->invertFor = 5.f;
					break;
				case 8:
					this->slowFor = 10.f;
					break;
				case 9:
					if (tail.size() > 1) {
						int tailToCut = tail.size() / 2;
						for (int i = 0; i < tailToCut; i++) this->tail.pop_front();
					}
					break;
				case 10:
					this->goldenApple = 5;
					break;
				}
				this->bonus->~Model();
				this->bonusType = 0;
				this->gimmeBonus = true;
			}
			if (this->bonusTime <= 0)
			{
				this->bonus->~Model();
				this->bonusType = 0;
				this->gimmeBonus = true;
			}
		}
	}
}

void Game::updateDirection()
{
	if (this->invertFor > 0) {
		if ((glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) && tail.back()->getOrigin().y <= head->getPosition().y) this->direction = UP;
		else if ((glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) && tail.back()->getOrigin().y >= head->getPosition().y) this->direction = DOWN;
		else if ((glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) && tail.back()->getOrigin().x >= head->getPosition().x) this->direction = LEFT;
		else if ((glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) && tail.back()->getOrigin().x <= head->getPosition().x) this->direction = RIGHT;
	}
	else {
		if ((glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) && tail.back()->getOrigin().y <= head->getPosition().y) this->direction = UP;
		else if ((glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) && tail.back()->getOrigin().y >= head->getPosition().y) this->direction = DOWN;
		else if ((glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) && tail.back()->getOrigin().x >= head->getPosition().x) this->direction = LEFT;
		else if ((glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) && tail.back()->getOrigin().x <= head->getPosition().x) this->direction = RIGHT;
	}
}

void Game::updateGameOver()
{
	if (
		this->head->getPosition().y >= this->boardPos.y + this->boardHeight
		|| this->head->getPosition().y < this->boardPos.y
		|| this->head->getPosition().x < this->boardPos.x
		|| this->head->getPosition().x >= this->boardPos.x + this->boardWidth
		) gameOver = true;
	for (size_t i = 0; i < tail.size(); i++) if (tail[i]->getOrigin() == head->getPosition()) gameOver = true;
	if (!amountOfFruits) gameOver = true;
}

void Game::handleGameEvents()
{
	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) this->pause = true;
	if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS)
	{
		this->pause = !this->pause;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS)
	{
		reset();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}/*
	if (glfwGetKey(this->window, GLFW_KEY_Z) == GLFW_PRESS) {
		this->wireframed = !this->wireframed;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}*/
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);
	if (this->mouseInit) {
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->mouseInit = false;
	}

	//Calculate offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}


void Game::updateDeltaTime()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->deltaTime = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

//Static functions
void Game::framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void Game::updateInput(GLFWwindow* window) {
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void Game::updateInput(GLFWwindow* window, Mesh& mesh) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mesh.move(glm::vec3(0.f, 1.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mesh.move(glm::vec3(0.f, -1.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mesh.move(glm::vec3(-1.f, 0.f, 0.f));
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mesh.move(glm::vec3(1.f, 0.f, 0.f));
	//PRZEROBIC NA ASYNC BO WSTYD XD

	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) mesh.rotate(glm::vec3(0.1f, 0.f, 0.f));
	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) mesh.rotate(glm::vec3(0.f, -0.1f, 0.f));
	//if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) mesh.rotate(glm::vec3(0.f, 0.1f, 0.f));
	//if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) mesh.scaleUp(glm::vec3(0.001f));
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) mesh.scaleUp(glm::vec3(-0.001f));
}