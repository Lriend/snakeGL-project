#include "libraries.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

Vertex vertices[] =
{
	//Positions								//Colors						//texcoords					//Normals
	glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, -1.f)
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

void updateInput(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position.z -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position.z += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position.x -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position.x += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotation.y -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotation.y += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) scale += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) scale -= 0.001f;
}

int main() {
	//INIT GLFW
	glfwInit();
	
	//CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac (redundand)

	GLFWwindow* win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lorem Ipsum Title", NULL, NULL);

	glfwGetFramebufferSize(win, &framebufferWidth, &framebufferHeight);			//<----------------+----	//For non-resizable window
	glfwSetFramebufferSizeCallback(win, framebufferResizeCallback);		//<---For resizable window |
	//glViewport(0, 0, framebufferWidth, framebufferHeight); //Canvas	  		  <----------------+

	glfwMakeContextCurrent(win); //Important for GLEW


	//INIT GLEW (REQ WINDOW & OPENGL CONTEXT)
	glewExperimental = GL_TRUE;
	//Error
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR! MAIN.CPP : GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	//OPENGL OPTIONS
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);			//
	glCullFace(GL_BACK);				// Uncomment those to render only front faces of triangles
	glFrontFace(GL_CCW);				//

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL ); //Could be GL_LINE as well as GL_FILL (then will draw only outlines)

	//SHADER INIT
	Shader core_program((char*)"VertexShader.glsl", (char*)"FragmentShader.glsl");

	//MODEL
	//VAO, VBO, EBO
	//GEN VAO AND BIND
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//GEN VBO AND BIND AND SEND DATA
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	//SET AND ENABLE VERTEXATTRIBPOINTERS
	//Position
	//GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position");									attribLoc could be use instead of direct zeroes-+
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));//		<----------------------------there--------------+
	glEnableVertexAttribArray(0);//	<--------------------------------------------------------------------------------------------------------there--------------+
	//																																							|
	//Color																																	and ones			|
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));//		<----------------------------there--------------+
	glEnableVertexAttribArray(1);//	<--------------------------------------------------------------------------------------------------------there--------------+
	//																																							|
	//Texcoord																																and twos			|
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));//		<----------------------------there--------------+
	glEnableVertexAttribArray(2);//	<--------------------------------------------------------------------------------------------------------there--------------+
	//																																							|
	//Normal																																and twos			|
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));//		<----------------------------there--------------+
	glEnableVertexAttribArray(3);//	<--------------------------------------------------------------------------------------------------------there--------------+

	//BIND VAO 0
	glBindVertexArray(0);

	//TEXTURE INIT
	//Texture0
	Texture texture0("Textures/colorfull.png", GL_TEXTURE_2D, 0);
	
	//Texture1
	Texture texture1("Textures/cherry.png", GL_TEXTURE_2D, 1);

	//Material0			Ambient color	Diffuse	color	Specular color	texture					   specular texture
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

	//INIT MVP MATRICES
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ModelMatrix(1.f);
	ModelMatrix = glm::translate(ModelMatrix, position);					//moveXYZ
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));	//rotateX
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));	//rotateY
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));	//rotateZ
	ModelMatrix = glm::scale(ModelMatrix, scale);									//scaleXYZ

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
	core_program.setMat4fv(ModelMatrix, "ModelMatrix");
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");
	
	//MAIN LOOP
	while (!glfwWindowShouldClose(win)) {
		//INTERACTING WITH WINDOW
		glfwPollEvents();
		updateInput(win, position, rotation, scale); //Model control
		updateInput(win); //Window control
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

		//Move, rotate and scale
		//rotation.y += 0.05f; //this way
		//rotation.z += 0.1f;

		glm::mat4 ModelMatrix(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position);										
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));	
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));	
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));	
		ModelMatrix = glm::scale(ModelMatrix, scale);								
		core_program.setMat4fv(ModelMatrix, "ModelMatrix");

		//Update camera in case of resizing
		glfwGetFramebufferSize(win, &framebufferWidth, &framebufferHeight);
		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		//Use a program
		core_program.use();

		//Activate texture
		texture0.bind();
		texture1.bind();

		//Bind vertex array object
		glBindVertexArray(VAO);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		//End Draw
		glfwSwapBuffers(win);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	//END OF PROGRAM
	glfwDestroyWindow(win);
	glfwTerminate();

	return 0;
}