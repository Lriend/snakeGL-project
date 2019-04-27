#include "menu.h"

//Private functions
void Menu::initGLFW()
{
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "ERROR! GAME.CPP/INITGLFW : GLFW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Menu::initWindow(const char* title, bool resizable) {
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

void Menu::initGLEW()
{
	//INIT GLEW (REQ WINDOW & OPENGL CONTEXT)
	glewExperimental = GL_TRUE;
	//Error
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR! GAME.CPP/INITGLEW : GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
}

void Menu::initOpenGLOptions(GLenum fillOrLine, bool culling, bool blend)
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

void Menu::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight, this->nearPlane, this->farPlane);
}

void Menu::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		(char*)"VertexShader.glsl", (char*)"FragmentShader.glsl"));
}

void Menu::initTextures()
{
	//this->textures.push_back(new Texture("Textures/star.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/colorfull.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/cherry.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/starCube.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/field.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/tailtemptex.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/straight.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/toRight.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/toLeft.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/end.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Textures/brown.png", GL_TEXTURE_2D));
	//this->textures.push_back(new Texture("Textures/tailtrantemptex.png", GL_TEXTURE_2D));
}

void Menu::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), DIFFUSE_TEX, SPECULAR_TEX));
}

void Menu::initObjects()
{
	this->objects.push_back(new Object("Objects/straightFR.obj"));
	this->objects.push_back(new Object("Objects/cornerToLeftFR.obj"));
	this->objects.push_back(new Object("Objects/cornerToRightFR.obj"));
	this->objects.push_back(new Object("Objects/endFR.obj"));
	this->objects.push_back(new Object("Objects/headBG.obj"));
}

void Menu::initModels()
{
	this->meshes.push_back(new Mesh(&Object("Objects/nMonkey.obj"), glm::vec3(0.f,0.f,-0.5f), glm::vec3(0.f), glm::vec3(0.3f, 0.3f, 0.3f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[9], this->textures[9], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
	this->meshes.push_back(new Mesh(&Object("Objects/snakeGL.obj"), glm::vec3(-1.75f, 1.f, -1.f), glm::vec3(90.f, 0.f, 0.f), glm::vec3(1.f, 0.5f, 1.f)));
	this->models.push_back(new Model(glm::vec3(0.f), this->materials[0], this->textures[2], this->textures[2], this->meshes));
	for (auto*&i : this->meshes) delete i;
	this->meshes.clear();
}

void Menu::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 0.f, 1.f));
}


void Menu::initUniforms()
{
	this->shaders[0]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[0]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

	this->shaders[0]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[0]->setVec3f(this->camPosition, "cameraPos");
}

void Menu::updateUniforms()
{

	//Update framebuffersize & ProjectionMatrix in case of resizing
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->fov), static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight, this->nearPlane, this->farPlane);
	this->shaders[0]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Ctor & dtor
Menu::Menu(const char* title, const int width, const int height, const int glMajorVer, const int glMinorVer, bool resizable)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height), GL_VERSION_MAJOR(glMajorVer), GL_VERSION_MINOR(glMinorVer)
{
	//Init vars
	this->boardWidth = 3;
	this->boardHeight = 3;

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
	this->initOpenGLOptions(GL_FILL, false);
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initObjects();
	this->initModels();
	this->initLights();
	this->initUniforms();

}

Menu::~Menu()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	for (size_t i = 0; i < this->shaders.size(); i++) delete this->shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++) delete this->textures[i];
	for (size_t i = 0; i < this->materials.size(); i++) delete this->materials[i];
	for (auto*&i : this->models) delete i;
	for (size_t i = 0; i < this->lights.size(); i++) delete this->lights[i];
}

//Getters
int Menu::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Setters
void Menu::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Menu::update()
{
	//INTERACTING WITH WINDOW
	updateInput(this->window);

	updateKeyboardInput();

	//UPDATE MENU
}

void Menu::render()
{
	//Clear
	glClearColor(0.2f, 0.2f, 0.2f, 1.f); // (R,G,B,Opacity) BACKGROUND
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update uniforms
	this->updateUniforms();
	//this->materials[MATERIAL1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

	//Use a program
	this->shaders[0]->use();
	//Activate textures and draw stuff
	//Object from file attempt Monkey
	this->models[0]->render(this->shaders[0]);
	this->models[1]->render(this->shaders[0]);

	//End Draw
	glfwSwapBuffers(this->window); //Swap frames
	glFlush(); //glFlush -> GL. Flush.

	//Reset
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}


void Menu::updateKeyboardInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->boardHeight++;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->boardHeight--;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

//Static functions
void Menu::framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void Menu::updateInput(GLFWwindow* window) {
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}
