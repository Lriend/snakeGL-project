#include "libraries.h"

Prefab test;

Vertex vertices[] =
{
	//Positions								//Colors						//texcoords					//Normals
	glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2,	//Triangle 1
	0, 2, 3 	//Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void updateInput(GLFWwindow* window, Mesh &mesh) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mesh.move(glm::vec3(0.f, 0.f, -0.001f));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mesh.move(glm::vec3(0.f, 0.f, 0.001f));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mesh.move(glm::vec3(-0.001f, 0.f, 0.f));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mesh.move(glm::vec3(0.001f, 0.f, 0.f));
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) mesh.rotate(glm::vec3(0.f, -0.1f, 0.f));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) mesh.rotate(glm::vec3(0.f, 0.1f, 0.f));
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) mesh.scaleUp(glm::vec3(0.001f));
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) mesh.scaleUp(glm::vec3(-0.001f));
}

GLFWwindow* createWindow(const char* title, const int width, const int height, int& fbWidth, int& fbHeight, const int glMajorVer, const int glMinorVer, bool resizable) {
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVer);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac (redundand)

	GLFWwindow* win = glfwCreateWindow(width, height, title, NULL, NULL);

	glfwGetFramebufferSize(win, &fbWidth, &fbHeight);			//<----------------+----	//For non-resizable window
	glfwSetFramebufferSizeCallback(win, framebufferResizeCallback);		//<---For resizable window |
	//glViewport(0, 0, framebufferWidth, framebufferHeight); //Canvas	  		  <----------------+

	glfwMakeContextCurrent(win); //Important for GLEW

	return win;
}

int main() {
	//INIT GLFW
	glfwInit();
	
	//CREATE WINDOW
	const int glMajorVersion = 4;
	const int glMinorVersion = 5;
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = WINDOW_WIDTH;
	int framebufferHeight = WINDOW_HEIGHT;
	bool resizable = true;

	GLFWwindow* window = createWindow("Lorem Ipsum Title", WINDOW_WIDTH, WINDOW_HEIGHT, framebufferWidth, framebufferHeight, glMajorVersion, glMinorVersion, resizable);

	//INIT GLEW (REQ WINDOW & OPENGL CONTEXT)
	glewExperimental = GL_TRUE;
	//Error
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR! MAIN.CPP : GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);				//
	glCullFace(GL_BACK);				// Uncomment those to render only front faces of triangles
	glFrontFace(GL_CCW);				//

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL ); //Could be GL_LINE as well as GL_FILL (then will draw only outlines)

	//SHADER INIT
	Shader core_program(glMajorVersion, glMinorVersion, (char*)"VertexShader.glsl", (char*)"FragmentShader.glsl");

	//MODEL MESH
	Mesh test(&Plane());

	//TEXTURE INIT
	//Texture0
	Texture texture0("Textures/cherry.png", GL_TEXTURE_2D, 0);
	
	//Texture1
	Texture texture1("Textures/colorfull.png", GL_TEXTURE_2D, 1);

	//Material0			Ambient color	Diffuse	color	Specular color	texture					   specular texture
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

	//INIT MVP MATRICES
	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;
	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);

	//LIGHTS
	glm::vec3 lightPos0(0.f, 0.f, 1.f);

	//INIT UNIFORMS
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");
	
	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//INTERACTING WITH WINDOW
		glfwPollEvents();
		updateInput(window, test); //Model control
		updateInput(window); //Window control
		//UPDATE GAME INPUT ----------------------------------------------------------------------------------------------------

		//UPDATE GAME ----------------------------------------------------------------------------------------------------------
		
		//DRAW------------------------------------------------------------------------------------------------------------------
		//Clear
		glClearColor(0.f, 0.f, 0.f, 1.f); // (R,G,B,Opacity)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//Update uniforms
		core_program.set1i(texture0.getTextureUnit(), "texture0");
		core_program.set1i(texture1.getTextureUnit(), "texture1");

		material0.sendToShader(core_program);

		//Update framebuffersize & ProjectionMatrix in case of resizing
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		//Use a program
		core_program.use();

		//Activate texture
		texture0.bind();
		texture1.bind();

		//Draw
		test.render(&core_program);

		//End Draw
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	//END OF PROGRAM
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}