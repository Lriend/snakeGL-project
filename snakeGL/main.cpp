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

bool loadShaders(GLuint &program) {
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex
	in_file.open("VertexShader.glsl");

	if (in_file.is_open())
		while (std::getline(in_file, temp))
			src += temp + "\n";
	else { std::cout << "ERROR! MAIN.CPP/LOADSHADERS : CANNOT_OPEN_VERTEXSHADER.GLSL_FILE" << std::endl; loadSuccess = false; }

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertexSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR! MAIN.CPP/LOADSHADERS : CANNOT_COMPILE_VERTEXSHADER.GLSL" << std::endl << infoLog << std::endl; loadSuccess = false;
	}
	temp = "";
	src = "";

	//Fragment
	in_file.open("FragmentShader.glsl");

	if (in_file.is_open())
		while (std::getline(in_file, temp))
			src += temp + "\n";
	else {
		std::cout << "ERROR! MAIN.CPP/LOADSHADERS : CANNOT_OPEN_FRAGMENTSHADER.GLSL_FILE" << std::endl; loadSuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR! MAIN.CPP/LOADSHADERS : CANNOT_COMPILE_FRAGMENTSHADER.GLSL" << std::endl << infoLog << std::endl; loadSuccess = false;
	}
	temp = "";
	src = "";

	//Program
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR! MAIN.CPP/LOADSHADERS : CANNOT_LINK_PROGRAM" << std::endl << infoLog << std::endl; loadSuccess = false;
	}

	//End
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
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

	//glEnable(GL_CULL_FACE);			//
	//glCullFace(GL_BACK);				// Uncomment those to render only front faces of triangles
	//glFrontFace(GL_CCW);				//

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL ); //Could be GL_LINE as well as GL_FILL (then will draw only outlines)

	//SHADER INIT
	GLuint core_program;
	if(!loadShaders(core_program)) glfwTerminate();

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
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));//	<----------------------------there--------------+
	glEnableVertexAttribArray(2);//	<--------------------------------------------------------------------------------------------------------there--------------+
	//																																							|
	//Normal																																and twos			|
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));//		<----------------------------there--------------+
	glEnableVertexAttribArray(3);//	<--------------------------------------------------------------------------------------------------------there--------------+

	//BIND VAO 0
	glBindVertexArray(0);

	//TEXTURE INIT
	//Texture0
	int imageWidth = 0;
	int imageHeight = 0;
	unsigned char* image = SOIL_load_image("Textures/colorfull.png", &imageWidth, &imageHeight, NULL, SOIL_LOAD_RGBA);

	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "ERROR! MAIN.CPP : TEXTURE_LOADING_FAILED" << std::endl;

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	
	//Texture1
	int imageWidth1 = 0;
	int imageHeight1 = 0;
	unsigned char* image1 = SOIL_load_image("Textures/cherry.png", &imageWidth1, &imageHeight1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth1, imageHeight1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "ERROR! MAIN.CPP : TEXTURE_LOADING_FAILED" << std::endl;

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image1);

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
	glUseProgram(core_program);
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	
	glUniform3fv(glGetUniformLocation(core_program, "lightPos0"), 1, glm::value_ptr(lightPos0));
	glUniform3fv(glGetUniformLocation(core_program, "cameraPos"), 1, glm::value_ptr(camPosition));
	
	glUseProgram(0);

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

		//Use a program
		glUseProgram(core_program);

		//Update uniforms
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

		//Move, rotate and scale
		//rotation.y += 0.05f; //this way
		//rotation.z += 0.1f;

		glm::mat4 ModelMatrix(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position);										
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));	
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));	
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));	
		ModelMatrix = glm::scale(ModelMatrix, scale);												
		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		//Update camera in case of resizing
		glfwGetFramebufferSize(win, &framebufferWidth, &framebufferHeight);
		ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth) / framebufferHeight, nearPlane, farPlane);
		glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		
		//Activate texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

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

	//DELETE PROGRAM
	glDeleteProgram(core_program);

	return 0;
}