#include "game.h"

void Game::setGame()
{
	//set some tingz
	switch (this->gameType)
	{
	case QUICK:
		this->amountOfFruits = 5;
		this->boardHeight = 10;
		this->boardWidth = 15;
		this->bonusFreq = 5;
		this->bonusTime = 5;
		this->gimmeBonus = true;
		this->iWantApple = this->iWantBombs = this->iWantBunny = this->iWantShrooms = this->iWantTurtles = this->surpriseMe = true;
		this->speed = 50.f;
		this->updateNums(FRUITS);
		this->updateNums(HEIGHT);
		this->updateNums(WIDTH);
		this->updateNums(FREQ);
		this->updateNums(TIME);
		this->updateNums(SPEED);
		break;
	case CLASSIC:
		this->amountOfFruits = 1;
		this->boardHeight = 30;
		this->boardWidth = 50;
		this->bonusFreq = 25;
		this->bonusTime = 10;
		this->gimmeBonus = true;
		this->iWantApple = this->iWantBombs = this->iWantBunny = this->iWantShrooms = this->iWantTurtles = this->surpriseMe = false;
		this->speed = 100.f;
		break;
	case CUSTOM:
		this->boardHeight = this->customHeight;
		this->boardWidth = this->customWidth;
		this->amountOfFruits = this->customFruits;
		break;
	}
	
	//reset
	this->score = 0;
	this->initBoard();
	this->camPosition = glm::vec3(0.f, 0.f, boardWidth > boardHeight ? (float)boardWidth / 2 - 9 : (float)boardHeight / 2 - 9); //set Cam position
	this->initMatrices(); //Cam position updated in matrices
	
	this->models.clear();
	this->initModels();
	for (size_t i = 0; i < 6; i++) subMenuElements[i].clear();
	pauseElements.clear();
	this->initGUI();
	this->initUniforms();
	this->gameOver = false;
	this->direction = RIGHT;
	this->initHead();
	this->tail.clear();
	this->tail.shrink_to_fit();
	this->initTail();
	this->shouldGrow = false;
	this->tick = 5.f;
	this->gimmeBonus = true;
	this->fruits.clear();
	this->fruits.shrink_to_fit();
	this->initFruits();
	this->updateFruits();
	if (this->bonusModel) this->bonusModel->~Model();
	this->bonusType = 0;
	this->Bonus->reset(this->goldenApple, this->invertFor, this->fastFor, this->slowFor);
	this->weNeedANewPlague = 0;
}

void Game::updateNums(int customizable)
{
	float temp = boardHeight > boardWidth ? (float)boardHeight / 10 : (float)boardWidth / 10;

	int tempada;
	int x[3];
	tempada = 0;
	switch (customizable)
	{
	case FRUITS:
		tempada = customFruits;
		break;
	case HEIGHT:
		tempada = customHeight;
		break;
	case WIDTH:
		tempada = customWidth;
		break;
	case FREQ:
		tempada = bonusFreq;
		break;
	case TIME:
		tempada = (int)bonusTime;
		break;
	case SPEED:
		tempada = (int)speed;
		break;
	}
	x[0] = tempada / 100;
	x[1] = (tempada - (tempada / 100) * 100) / 10;
	x[2] = tempada - (tempada / 10) * 10;
	for (size_t i = 0; i < 3; i++) {
		if (this->customNumsUI[customizable][i]) delete this->customNumsUI[customizable][i];
		this->customNumsUI[customizable][i] = new Mesh(nums[x[i]], glm::vec3(0.3f + .2f*i, -0.95f + 0.55f*customizable, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f));
	}
}

void Game::updateHighScoreUI()
{
	float temp = boardHeight > boardWidth ? (float)boardHeight / 10 : (float)boardWidth / 10; 
	
	int tempada;
	int x[6];
	tempada = this->highQuick;
	x[0] = tempada / 100000; //setki tysiecy
	x[1] = (tempada - x[0] * 100000) / 10000;
	x[2] = (tempada - x[0]*100000 - x[1]*10000) / 1000;
	x[3] = (tempada - x[0]*100000 - x[1]*10000 - x[2]*1000) / 100;
	x[4] = (tempada - x[0]*100000 - x[1]*10000 - x[2]*1000 - x[3]*100) / 10;
	x[5] = tempada - (tempada / 10) * 10; //jednosci
	for (size_t i = 0; i < 6; i++) {
		if (this->highScoresUI[0][i]) delete this->highScoresUI[0][i];
		this->highScoresUI[0][i] = new Mesh(nums[x[i]], glm::vec3(-2.f + .35f*i, -0.2f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f));
	}
	tempada = this->highClassic;
	x[0] = tempada / 100000; //setki tysiecy
	x[1] = (tempada - x[0] * 100000) / 10000;
	x[2] = (tempada - x[0]*100000 - x[1]*10000) / 1000;
	x[3] = (tempada - x[0]*100000 - x[1]*10000 - x[2]*1000) / 100;
	x[4] = (tempada - x[0]*100000 - x[1]*10000 - x[2]*1000 - x[3]*100) / 10;
	x[5] = tempada - (tempada / 10) * 10; //jednosci
	for (size_t i = 0; i < 6; i++) {
		if (this->highScoresUI[1][i]) delete this->highScoresUI[1][i];
		this->highScoresUI[1][i] = new Mesh(nums[x[i]], glm::vec3(-2.f + .35f*i, -1.3f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f));
	}
}

void Game::updateCurrentGameScoreUI()
{
	float temp = boardHeight > boardWidth ? (float)boardHeight / 10 : (float)boardWidth / 10;

	int tempada;
	int x[6];
	gameType == QUICK ? tempada = this->highQuick : tempada = this->highClassic;
	x[0] = tempada / 100000; //setki tysiecy
	x[1] = (tempada - x[0] * 100000) / 10000;
	x[2] = (tempada - x[0] * 100000 - x[1] * 10000) / 1000;
	x[3] = (tempada - x[0] * 100000 - x[1] * 10000 - x[2] * 1000) / 100;
	x[4] = (tempada - x[0] * 100000 - x[1] * 10000 - x[2] * 1000 - x[3] * 100) / 10;
	x[5] = tempada - (tempada / 10) * 10; //jednosci
	for (size_t i = 0; i < 6; i++) {
		if (this->currentGameScoreUI[0][i]) delete this->currentGameScoreUI[0][i];
		this->currentGameScoreUI[0][i] = new Mesh(nums[x[i]], gameType == QUICK ? glm::vec3(2.6f + .25f*i, 1.f, -4.2f) : glm::vec3(20.f + 1.75f*i, 4.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), gameType==QUICK?glm::vec3(temp*1.f, temp*0.5f, temp*1.f): glm::vec3(temp*2.f, temp*1.f, temp*2.f));
	}
	tempada = this->score;
	x[0] = tempada / 100000; //setki tysiecy
	x[1] = (tempada - x[0] * 100000) / 10000;
	x[2] = (tempada - x[0] * 100000 - x[1] * 10000) / 1000;
	x[3] = (tempada - x[0] * 100000 - x[1] * 10000 - x[2] * 1000) / 100;
	x[4] = (tempada - x[0] * 100000 - x[1] * 10000 - x[2] * 1000 - x[3] * 100) / 10;
	x[5] = tempada - (tempada / 10) * 10; //jednosci
	for (size_t i = 0; i < 6; i++) {
		if (this->currentGameScoreUI[1][i]) delete this->currentGameScoreUI[1][i];
		this->currentGameScoreUI[1][i] = new Mesh(nums[x[i]], gameType == QUICK ? glm::vec3(2.6f + .25f*i, .5f, -4.2f) : glm::vec3(20.f + 1.75f*i, 1.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), gameType == QUICK ? glm::vec3(temp*1.f, temp*0.5f, temp*1.f) : glm::vec3(temp*2.f, temp*1.f, temp*2.f));
	}
}


void Game::initGUI(){
	float temp = boardHeight > boardWidth ? (float)boardHeight / 10 : (float)boardWidth / 10;
	if (GUIelements.empty()) {
		for(size_t i=0;i<6;i++)
		this->GUIelements.push_back(new Mesh(objects[17+i], glm::vec3(0.f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(3 * temp*1.f, 3 * temp*0.5f, 3 * temp*1.f)));
	}
	this->pauseElements.push_back(new Mesh(objects[23], glm::vec3((float)boardPos.x + (float)this->boardWidth / 2-1.f, (float)boardPos.y + (float)this->boardHeight / 2, -9.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(3*temp*1.f, temp*0.5f, 3*temp*1.f)));
	this->pauseElements.push_back(new Mesh(objects[25], glm::vec3((float)boardPos.x + (float)this->boardWidth / 2-1.f, (float)boardPos.y + (float)this->boardHeight / 2, -9.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(3*temp*1.f, temp*0.5f, 3*temp*1.f)));

	this->subMenuElements[QUICK_GAME].push_back(new Mesh(objects[23], glm::vec3(-0.8f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[QUICK_GAME].push_back(new Mesh(objects[24], glm::vec3(-0.2f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[QUICK_GAME].push_back(new Mesh(objects[36], glm::vec3(0.f, -0.5f, -4.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));

	this->subMenuElements[CLASSIC_GAME].push_back(new Mesh(objects[23], glm::vec3(-0.8f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[CLASSIC_GAME].push_back(new Mesh(objects[24], glm::vec3(-0.2f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[CLASSIC_GAME].push_back(new Mesh(objects[37], glm::vec3(0.f, -0.5f, -3.5f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));

	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[23], glm::vec3(-0.8f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[24], glm::vec3(-0.2f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	for (size_t i = 0; i < 6;i++) this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[32-i], glm::vec3(1.f, -0.95f+0.55f*i, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[surpriseMe?34:33], glm::vec3(-2.f, -0.8f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[iWantApple?34:33], glm::vec3(-2.f, -0.8f+0.55f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[iWantBombs?34:33], glm::vec3(-2.f, -0.8f+0.55f*2, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[iWantTurtles?34:33], glm::vec3(-2.f, -0.8f+0.55f*3, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[iWantShrooms?34:33], glm::vec3(-2.f, -0.8f+0.55f*4, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[iWantBunny?34:33], glm::vec3(-2.f, -0.8f+0.55f*5, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	
	for (size_t i = 0; i < 6;i++) this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(objects[6+i], glm::vec3(-2.5f, -0.8f+0.55f*i, -4.2f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
	//for (size_t i = 0; i <= 9;i++) this->subMenuElements[CUSTOM_GAME].push_back(new Mesh(nums[i], glm::vec3(-0.2f*i, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));

	this->subMenuElements[SCOREBOARD].push_back(new Mesh(objects[24], glm::vec3(-2.1f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[SCOREBOARD].push_back(new Mesh(objects[18], glm::vec3(4.5f, -.55f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[SCOREBOARD].push_back(new Mesh(objects[17], glm::vec3(6.1f, .55f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));

	this->subMenuElements[SETTINGS].push_back(new Mesh(objects[26], glm::vec3(0.1f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[SETTINGS].push_back(new Mesh(objects[24], glm::vec3(0.1f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));

	this->subMenuElements[EXIT].push_back(new Mesh(objects[22], glm::vec3(-12.7f, -0.75f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(2*temp*1.f, 2*temp*0.5f, 2*temp*1.f)));
	this->subMenuElements[EXIT].push_back(new Mesh(objects[24], glm::vec3(-0.4f, 0.f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->subMenuElements[EXIT].push_back(new Mesh(objects[35], glm::vec3(0.f, -0.5f, -4.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
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
		abort();
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
	this->objects.push_back(new Object("Objects/bunny.obj")); //objects[6]
	this->objects.push_back(new Object("Objects/shroom.obj"));
	this->objects.push_back(new Object("Objects/turtle2.obj"));
	this->objects.push_back(new Object("Objects/bomb.obj"));
	this->objects.push_back(new Object("Objects/apple.obj"));
	this->objects.push_back(new Object("Objects/question.obj")); //objects[11]
	this->objects.push_back(new Object("Objects/nMonkey.obj"));
	this->objects.push_back(new Object("Objects/snakeGL.obj"));
	this->objects.push_back(new Object("Objects/gameover.obj"));
	this->objects.push_back(new Object("Objects/restart.obj"));
	this->objects.push_back(new Object("Objects/pause.obj"));
	this->objects.push_back(new Object("Objects/quickGame.obj"));
	this->objects.push_back(new Object("Objects/classicGame.obj"));
	this->objects.push_back(new Object("Objects/customGame.obj"));
	this->objects.push_back(new Object("Objects/scoreboard.obj")); //objects[20]
	this->objects.push_back(new Object("Objects/settings.obj"));
	this->objects.push_back(new Object("Objects/exit.obj")); //objects[22]
	this->objects.push_back(new Object("Objects/play.obj")); //objects[23]
	this->objects.push_back(new Object("Objects/return.obj")); //objects[24]
	this->objects.push_back(new Object("Objects/menu.obj")); //objects[25]
	this->objects.push_back(new Object("Objects/confirm.obj")); //objects[26]
	this->objects.push_back(new Object("Objects/fruitsUI.obj")); //objects[27]
	this->objects.push_back(new Object("Objects/heightUI.obj")); //objects[28]
	this->objects.push_back(new Object("Objects/widthUI.obj")); //objects[29]
	this->objects.push_back(new Object("Objects/bonusFreqUI.obj")); //objects[30]
	this->objects.push_back(new Object("Objects/bonusTimeUI.obj")); //objects[31]
	this->objects.push_back(new Object("Objects/speedUI.obj")); //objects[32]
	this->objects.push_back(new Object("Objects/false.obj")); //objects[33]
	this->objects.push_back(new Object("Objects/true.obj")); //objects[34]
	this->objects.push_back(new Object("Objects/exitScreen.obj")); //objects[35]
	this->objects.push_back(new Object("Objects/quickScreen.obj")); //objects[36]
	this->objects.push_back(new Object("Objects/classicScreen.obj")); //objects[37]

	this->nums[0] = new Object("Objects/zero.obj");
	this->nums[1] = new Object("Objects/one.obj");
	this->nums[2] = new Object("Objects/two.obj");
	this->nums[3] = new Object("Objects/three.obj");
	this->nums[4] = new Object("Objects/four.obj");
	this->nums[5] = new Object("Objects/five.obj");
	this->nums[6] = new Object("Objects/six.obj");
	this->nums[7] = new Object("Objects/seven.obj");
	this->nums[8] = new Object("Objects/eight.obj");
	this->nums[9] = new Object("Objects/nine.obj");
}

void Game::initModels()
{
	if (!models.empty()) for (auto i : models) delete i;
	this->meshes.push_back(new Mesh(objects[12], glm::vec3((float)boardWidth / 2 + 2.5f, -(float)boardHeight / 2 + 4, -10.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	this->meshes.push_back(new Mesh(objects[13], glm::vec3((float)boardWidth / 2 + 3.f, -(float)boardHeight / 2 + 1.f, -10.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f)));
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

	this->meshes.push_back(new Mesh(objects[13], glm::vec3(0.f,0.f,	-4.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(temp*1.f, temp*0.5f, temp*1.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	
	}

void Game::initBoard()
{
	if (!this->board.empty()) { for (auto i : board) delete i; board.clear(); board.shrink_to_fit(); }
	this->boardPos = glm::vec2(-this->boardWidth / 2, -this->boardHeight / 2);
	for (int i = 0; i < boardWidth; i++)
		for (int j = 0; j < boardHeight; j++)
			this->board.push_back(new Mesh(&Quad(), glm::vec3(i - this->boardWidth / 2, j - this->boardHeight / 2, -10.f)));
}

void Game::initHead() {
	if (this->head) delete head;
	this->head = new Mesh(objects[4], glm::vec3(this->boardPos.x, this->boardPos.y, -10.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f));
}

void Game::initTail()
{
	if (!tail.empty()) for (auto i : tail) delete i;
	this->meshes.push_back(new Mesh(objects[3], glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, -10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 0.5f)));
	this->tail.push_back(new Model(glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, -10.f), this->materials[0], this->textures[WHITE], this->textures[WHITE], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
}

void Game::initFruits()
{
	if (!this->fruits.empty()) for (auto i : fruits) delete i;
	std::srand((unsigned int)time(NULL));
	for (int i = 0; i < this->amountOfFruits; i++) {
		glm::vec3 potFrPos = glm::vec3(boardPos.x + std::rand() % boardWidth, boardPos.y + std::rand() % boardHeight, -10.f);
		this->meshes.push_back(new Mesh(objects[5], potFrPos));
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
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VERSION_MAJOR(glMajorVer), GL_VERSION_MINOR(glMinorVer), highClassic(0), highQuick(0), score(0)
{
	//Init vars
	this->gameState = MENU;
	this->gameType = QUICK;
	this->menuElement = QUICK_GAME;
	this->subMenu = false;

	this->customizing = false;

	this->boardWidth = 3;
	this->boardHeight = 4;

	this->customHeight = 20;
	this->customWidth = 30;

	this->amountOfFruits = 3;
	this->customFruits = 5;
	this->bonusFreq = 25;
	this->bonusTime = 10.f;

	this->iWantApple = this->iWantBombs = this->iWantBunny = this->iWantShrooms = this->iWantTurtles = this->surpriseMe = true;

	this->tick = 5.f;
	this->speed = 50.f;
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
	this->initGUI();
	this->updateNums(FRUITS);
	this->updateNums(HEIGHT);
	this->updateNums(WIDTH);
	this->updateNums(FREQ);
	this->updateNums(TIME);
	this->updateNums(SPEED);
	this->initHead();
	this->initTail();
	this->initFruits();
	this->updateFruits();

	this->initLights();
	this->initUniforms();

	this->Bonus = &Bunny;
	this->setGame();
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
	for (auto*&i : this->GUIelements) delete i;
	//DODAC RESZTE MENU/SUBMENU
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
	this->updateDeltaTime();

	//UPDATE MOUSE INPUT
	//updateMouseInput();

	//PollEv
	glfwPollEvents();

	if (gameState) {//--------------------------------------------------------------------GAME----------------------------------------------------------------------------
		handleGameEvents();
		if (!pause) {
			if (this->fastFor > 0) { this->fastFor -= deltaTime; this->tick = 2.5f; }
			else
				if (this->slowFor > 0) { this->slowFor -= deltaTime; this->tick = 10.f; }
				else
					this->tick = 5.f;
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
				moveSnake(); handleGameEvents(); this->updateCurrentGameScoreUI();
				this->weNeedANewPlague = 0;
			}
			else this->weNeedANewPlague += this->deltaTime;
			if (this->deltaTime) this->models[0]->rotate(glm::vec3(0.f, 25.f, 0.f)*this->deltaTime);
		}
		else {//----------------------------------------------------------------------------PAUSE-------------------------------------------------------------------------
			handlePauseEvents();
		}
	}
	else
	{
		if (!subMenu)//-----------------------------------------------------------------------------------MENU-------------------------------------------------------------------------- 
		{
			handleMenuEvents();
		}
		else {//----------------------------------------------------------------SUBMENU--------------------------------------------------------------------------------------
			handleSubMenuEvents();
		}
	}
}

void Game::render()
{
	//Clear
	//glClearColor(0.8f, 0.2f, 0.5f, 1.f); // (R,G,B,Opacity) BACKGROUND
	glClearColor(0.2f, 0.2f, 0.2f, 1.f); // (R,G,B,Opacity) BACKGROUND
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update uniforms
	this->updateUniforms();
	//this->materials[MATERIAL1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	//Use a program
	this->shaders[SHADER_CORE_PROGRAM]->use();
	//Activate textures and draw stuff
	if (gameState) {//--------------------------------------------------------------------GAME----------------------------------------------------------------------------
		drawBoard();
		drawSnake();
		drawFruits();
		if (this->bonusModel)this->bonusModel->render(this->shaders[SHADER_CORE_PROGRAM]);
		for (size_t i = 0; i < 2; i++)
			for (size_t j = 0; j < 6; j++) {
				this->textures[i==0?GRAY:WHITE]->bind(DIFFUSE_TEX);
				if (currentGameScoreUI[i][j])	currentGameScoreUI[i][j]->render(this->shaders[SHADER_CORE_PROGRAM]);
			}
		//Object from file attempt Monkey
		this->models[0]->render(this->shaders[SHADER_CORE_PROGRAM]);
		this->models[1]->render(this->shaders[SHADER_CORE_PROGRAM]);
		if (this->gameOver) {
			this->models[2]->render(this->shaders[SHADER_CORE_PROGRAM]);
			this->models[3]->render(this->shaders[SHADER_CORE_PROGRAM]);
		}
		if (this->pause) { //------------------------------------------------------------PAUSE----------------------------------------------------------------------------------
			this->models[4]->render(this->shaders[SHADER_CORE_PROGRAM]);
			for (size_t i = 0; i < 2; i++) {
				this->textures[i == subMenuElement ? RED : WHITE]->bind(DIFFUSE_TEX);
				pauseElements[i]->render(this->shaders[SHADER_CORE_PROGRAM]);
			}
		}
	}
	else
	{
		if (!(this->menuElement == CUSTOM_GAME && this->subMenu))this->models[5]->render(this->shaders[SHADER_CORE_PROGRAM]);

		if (!subMenu) {//-----------------------------------------------------------------------------------MENU--------------------------------------------------------------------------
			for (size_t i = 0; i < 6; i++) {
				this->textures[i == menuElement ? RED : WHITE]->bind(DIFFUSE_TEX);
				GUIelements[i]->render(this->shaders[SHADER_CORE_PROGRAM]);
			}
		}
		else {//----------------------------------------------------------------SUBMENU--------------------------------------------------------------------------------------
			for (size_t i = 0; i < subMenuElements[menuElement].size(); i++) {
				this->textures[i == subMenuElement ? customizing ? BLUE : RED : WHITE]->bind(DIFFUSE_TEX);
				if (subMenuElements[menuElement][i])subMenuElements[menuElement][i]->render(this->shaders[SHADER_CORE_PROGRAM]);
			}
			if (menuElement == CUSTOM_GAME) {/*
				for (size_t i = 2; i < 8; i++) {
					for (size_t j = 0; j < 3; j++) {
						this->textures[i == subMenuElement ? customizing ? BLUE : RED : WHITE]->bind(DIFFUSE_TEX);
						customNumsUI[i - 2][j]->render(this->shaders[SHADER_CORE_PROGRAM]);
					}
				}*/
				for (size_t i = 0; i < 6; i++)
					for (size_t j = 0; j < 3; j++) {
						this->textures[WHITE]->bind(DIFFUSE_TEX);
						if (customNumsUI[i][j])	customNumsUI[i][j]->render(this->shaders[SHADER_CORE_PROGRAM]);
					}
			}
			else if (menuElement == SCOREBOARD) {
				for (size_t i = 0; i < 2; i++)
					for (size_t j = 0; j < 6; j++) {
						this->textures[WHITE]->bind(DIFFUSE_TEX);
						if (highScoresUI[i][j])	highScoresUI[i][j]->render(this->shaders[SHADER_CORE_PROGRAM]);
					}
			}
		}
	}
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
	this->meshes.shrink_to_fit();
	this->growTail();
	delete this->tail[0];
	this->tail.pop_front();

	//END
	glm::vec3 backPosTemp = this->tail.front()->getOrigin();
	float textureRotation = this->tail.front()->getRotationAtZ(0);
	delete this->tail[0];
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
		bool getOutOfHere = false; if (this->fruits[i]->getOrigin() == fruitPotPos)gameType==QUICK?score++:score+=10;
		/*for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		if (this->head->getPosition() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		for (size_t j = 0; j < this->fruits.size(); j++) if (this->fruits[j]->getOrigin() == this->fruits[i]->getOrigin() && i != j) getOutOfHere = true;
		if (tail.size() < board.size() - amountOfFruits - 5) if (this->bonus) if (this->bonus->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		*/while (this->fruits[i]->getOrigin() == fruitPotPos || getOutOfHere) {
			getOutOfHere = false;
			if (this->bonusModel && this->tail.size() >= this->board.size() - fruits.size() - 5) { this->bonusModel->~Model(); this->gimmeBonus = false; this->bonusType = 0; }
			if (tail.size() >= board.size() - amountOfFruits-1) { amountOfFruits--; fruits.at(i)->~Model(); this->shouldGrow = true; break; }
			this->shouldGrow = true;
			this->fruits[i]->setOrigin(glm::vec3(boardPos.x + std::rand() % boardWidth, boardPos.y + std::rand() % boardHeight, -10.f));
			for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
			if (this->head->getPosition() == this->fruits[i]->getOrigin()) getOutOfHere = true;
			for (size_t j = 0; j < this->fruits.size(); j++) if (this->fruits[j]->getOrigin() == this->fruits[i]->getOrigin() && i != j) getOutOfHere = true;
			if (tail.size() < board.size() - amountOfFruits - 5) if (this->bonusModel) if (this->bonusModel->getOrigin() == this->fruits[i]->getOrigin()) getOutOfHere = true;
		}
	}
}

void Game::updateBonus()
{
	if (iWantApple || iWantBombs || iWantBunny || iWantTurtles || iWantShrooms || surpriseMe) {
		glm::vec3 fruitPotPos(0.f);
		switch (this->direction)
		{
		case UP: fruitPotPos = glm::vec3(this->head->getPosition().x, this->head->getPosition().y + 1.f, this->head->getPosition().z); break;
		case DOWN: fruitPotPos = glm::vec3(this->head->getPosition().x, this->head->getPosition().y - 1.f, this->head->getPosition().z); break;
		case LEFT: fruitPotPos = glm::vec3(this->head->getPosition().x - 1.f, this->head->getPosition().y, this->head->getPosition().z);  break;
		case RIGHT: fruitPotPos = glm::vec3(this->head->getPosition().x + 1.f, this->head->getPosition().y, this->head->getPosition().z); break;
		}
		if (bonusModel) for (size_t j = 0; j < this->tail.size(); j++) if (this->tail[j]->getOrigin() == this->bonusModel->getOrigin()) this->bonusType = 0;
		if (this->bonusType == 0 && this->bonusModel) this->bonusModel->~Model();
		if (tail.size() >= board.size() - amountOfFruits - 5) gimmeBonus = false;
		std::srand((unsigned int)time(NULL));
		if (tail.size() < board.size() - amountOfFruits - 5)
			if (rand() % this->bonusFreq == 0 && this->gimmeBonus) {
				this->bonusTime = 10.f;
				bool rerand = true;
				unsigned type;
				while (rerand) {
					rerand = false;
					type = 6 + rand() % 6;
					if (!iWantBunny && type == 6 + BUNNY) rerand = true;
					if (!iWantApple && type == 6 + APPLE) rerand = true;
					if (!iWantTurtles && type == 6 + TURTLE) rerand = true;
					if (!iWantShrooms && type == 6 + MUSHROOM) rerand = true;
					if (!iWantBombs && type == 6 + BOMB) rerand = true;
					if (!surpriseMe && type == 6 + QUESTION) rerand = true;
				}
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
				if(this->bonusModel != nullptr)delete this->bonusModel;
				this->bonusModel = new Model(meshes.empty() ? glm::vec3(0.f) : meshes[0]->getPosition(), this->materials[0], this->textures[2 + type], this->textures[2 + type], this->meshes);
				for (auto*&i : this->meshes) delete i;
				this->meshes.clear();
				if (type == 11) type = 6 + rand() % 5;
				this->bonusType = type;
				this->gimmeBonus = false;
			}
		if (tail.size() < board.size() - amountOfFruits - 10) {
			if (this->bonusModel) {
				if (this->bonusModel->getOrigin() == fruitPotPos&&!this->gimmeBonus) {
					switch (bonusType) {
					case 6:
						this->Bonus = &Bunny;
						break;
					case 7:
						this->Bonus = &Shroom;
						break;
					case 8:
						this->Bonus = &Turtle;
						break;
					case 9:
						this->Bonus = &Bomb;
						break;
					case 10:
						this->Bonus = &Apple;
						break;
					}
					this->Bonus->affect(this->goldenApple, this->invertFor, this->fastFor, this->slowFor, this->tail, this->score);
					this->bonusModel->~Model();
					this->bonusType = 0;
					this->gimmeBonus = true;
				}
				if (this->bonusTime <= 0)
				{
					this->bonusModel->~Model();
					this->bonusType = 0;
					this->gimmeBonus = true;
				}
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
	if (gameType == QUICK && score > highQuick) highQuick = score;
	if (gameType == CLASSIC && score > highClassic) highClassic = score;
	
	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED))
		this->pause = true;
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		this->pause = !this->pause;
		subMenuElement = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::cout << "HighScore Classic - " << this->highClassic << std::endl;
		std::cout << "HighScore Quick - " << this->highQuick << std::endl;
		std::cout << "Current Score - " << this->score << std::endl;
	}
	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS)
	{
		setGame();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	/*
	if (glfwGetKey(this->window, GLFW_KEY_Z) == GLFW_PRESS) {
		this->wireframed = !this->wireframed;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}*/
}

void Game::handleMenuEvents()
{
	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) if (this->menuElement) { this->menuElement--; std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) if (this->menuElement < 5) { this->menuElement++; std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
	if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		this->subMenu = true;
		subMenuElement = 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		this->updateHighScoreUI();
	}
}

void Game::handleSubMenuEvents()
{
	if (
		menuElement == QUICK_GAME ||
		menuElement == CLASSIC_GAME ||
		menuElement == SETTINGS || ///////////////////////////////////////////-------------------DO ODDZIELNEGO OBSLUZENIA
		menuElement == EXIT
		) {
		if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) if (this->subMenuElement) { this->subMenuElement--; }
		if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) if (!this->subMenuElement) { this->subMenuElement++; }
		if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS)
			if (!subMenuElement)
				switch ((MENU_ELEMENT)menuElement) {
				case QUICK_GAME:
					this->gameState = GAME;
					this->gameType = QUICK;
					setGame(); this->updateCurrentGameScoreUI();
					break;
				case CLASSIC_GAME:
					this->gameState = GAME;
					this->gameType = CLASSIC;
					setGame(); this->updateCurrentGameScoreUI();
					break;
				case EXIT:
					glfwPollEvents();
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
				default:
					break;
				}
			else { subMenu = false; std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
	}
	else if (menuElement == SCOREBOARD) {
		subMenuElement = 0;
		if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS) { subMenu = false; std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
	}
	else if (menuElement == CUSTOM_GAME) {
		float temp = boardHeight > boardWidth ? (float)boardHeight / 10 : (float)boardWidth / 10;
		if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS)
			switch (subMenuElement) {
			case 0:
				this->gameState = GAME;
				this->gameType = CUSTOM;
				setGame();
				break;
			case 1:
				subMenu = false;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			case BUNNY+8:
				iWantBunny = !iWantBunny;
				delete this->subMenuElements[CUSTOM_GAME][subMenuElement];
				this->subMenuElements[CUSTOM_GAME][subMenuElement] = (new Mesh(objects[iWantBunny ? 34 : 33], glm::vec3(-2.f, -0.8f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			case MUSHROOM+8:
				iWantShrooms = !iWantShrooms;
				delete this->subMenuElements[CUSTOM_GAME][subMenuElement];
				this->subMenuElements[CUSTOM_GAME][subMenuElement] = new Mesh(objects[iWantShrooms ? 34 : 33], glm::vec3(-2.f, -0.8f + 0.55f, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f));
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			case TURTLE+8:
				iWantTurtles = !iWantTurtles;
				delete this->subMenuElements[CUSTOM_GAME][subMenuElement];
				this->subMenuElements[CUSTOM_GAME][subMenuElement] = (new Mesh(objects[iWantTurtles ? 34 : 33], glm::vec3(-2.f, -0.8f + 0.55f * 2, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			case BOMB+8:
				iWantBombs = !iWantBombs;
				delete this->subMenuElements[CUSTOM_GAME][subMenuElement];
				this->subMenuElements[CUSTOM_GAME][subMenuElement] =(new Mesh(objects[iWantBombs ? 34 : 33], glm::vec3(-2.f, -0.8f + 0.55f * 3, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			case APPLE+8:
				iWantApple = !iWantApple;
				delete this->subMenuElements[CUSTOM_GAME][subMenuElement];
				this->subMenuElements[CUSTOM_GAME][subMenuElement] = (new Mesh(objects[iWantApple ? 34 : 33], glm::vec3(-2.f, -0.8f + 0.55f*4, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			case QUESTION+8:
				surpriseMe = !surpriseMe;
				delete this->subMenuElements[CUSTOM_GAME][subMenuElement];
				this->subMenuElements[CUSTOM_GAME][subMenuElement] = (new Mesh(objects[surpriseMe ? 34 : 33], glm::vec3(-2.f, -0.8f+0.55f*5, -4.2f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(0.3f*temp*1.f, 0.3f*temp, 0.3f*temp*1.f)));
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			default:
				customizing = !customizing;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				break;
			}
		if (customizing) {
			if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {
				switch (subMenuElement)
				{
				case SPEED + 2:
					if (speed <= 990)
						speed += 5.f;
					break;
				case TIME + 2:
					if (bonusTime <= 998)
						bonusTime += 1.f;
					break;
				case FREQ + 2:
					if (bonusFreq <= 998)
						bonusFreq++;
					break;
				case WIDTH + 2:
					if (customWidth <= 98)
						customWidth++;
					break;
				case HEIGHT + 2:
					if (customHeight <= 98)
						customHeight++;
					break;
				case FRUITS + 2:
					if (customFruits < customHeight*customWidth - 1)
						customFruits++;
					break;
				} std::this_thread::sleep_for(std::chrono::milliseconds(150));
				updateNums(subMenuElement - 2);
			}
			if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				switch (subMenuElement)
				{
				case SPEED + 2:
					if (speed > 5)
						speed -= 5;
					break;
				case TIME + 2:
					if (bonusTime >= 1)
						bonusTime -= 1;
					break;
				case FREQ + 2:
					if (bonusFreq > 1)
						bonusFreq--;
					break;
				case WIDTH + 2:
					if (customWidth > 5)
						customWidth--;
					break;
				case HEIGHT + 2:
					if (customHeight > 5)
						customHeight--;
					break;
				case FRUITS + 2:
					if (customFruits > 1)
						customFruits--;
					break;
				} std::this_thread::sleep_for(std::chrono::milliseconds(150));
				updateNums(subMenuElement - 2);
			}
		}
		else {
			if (customHeight*customWidth <= customFruits + 1) { customFruits = customHeight * customWidth - 1; updateNums(FRUITS); }
			if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) { if (this->subMenuElement > 1 && this->subMenuElement < 8) this->subMenuElement += 6; else if (this->subMenuElement == 1) this->subMenuElement = 0;
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}
			if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) { if (this->subMenuElement < 14 && this->subMenuElement>7) this->subMenuElement -= 6; else if (!this->subMenuElement) this->subMenuElement = 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}
			if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {
				if (this->subMenuElement < 13 && this->subMenuElement>7) this->subMenuElement++; else if (!this->subMenuElement) this->subMenuElement = 8;
				else if (this->subMenuElement < 7 && this->subMenuElement>0) this->subMenuElement++;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}
			if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
				if (this->subMenuElement > 8) this->subMenuElement--; else if (this->subMenuElement < 8 && this->subMenuElement>1) this->subMenuElement--; else if (this->subMenuElement == 8) this->subMenuElement = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
			}
		}
	}
}

void Game::handlePauseEvents()
{
	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) if (this->subMenuElement) { this->subMenuElement--; }
	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) if (!this->subMenuElement) { this->subMenuElement++; }
	if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS)
		if (subMenuElement) {
			this->gameState = MENU;
			this->gameType = QUICK;
			setGame();
			this->subMenu = false;
			this->pause = false;
			this->gameOver = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		else { pause = false; std::this_thread::sleep_for(std::chrono::milliseconds(150)); }
}
//
//void Game::updateMouseInput()
//{
//	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);
//	if (this->mouseInit) {
//		this->lastMouseX = this->mouseX;
//		this->lastMouseY = this->mouseY;
//		this->mouseInit = false;
//	}
//
//	//Calculate offset
//	this->mouseOffsetX = this->mouseX - this->lastMouseX;
//	this->mouseOffsetY = this->lastMouseY - this->mouseY;
//
//	this->lastMouseX = this->mouseX;
//	this->lastMouseY = this->mouseY;
//}

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


/*	for (auto i : GUIelements) delete i;
	for (auto i : pauseElements) delete i;
	for (auto i : subMenuElements[QUICK_GAME]) delete i;
	for (auto i : subMenuElements[CLASSIC_GAME]) delete i;
	for (auto i : subMenuElements[CUSTOM_GAME]) delete i;
	for (auto i : subMenuElements[SCOREBOARD]) delete i;
	for (auto i : subMenuElements[SETTINGS]) delete i;
	for (auto i : subMenuElements[EXIT]) delete i;*/
}