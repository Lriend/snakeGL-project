#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
	//Member vars
	GLuint id;

	//Private functions
	std::string loadShaderSource(char*);
	GLuint loadShader(GLenum, char*);
	void linkProgram(GLuint, GLuint, GLuint);

public:
	//Ctors
	Shader(char* vertexFile, char* fragmentFile, char* geometryFile = (char*)"");
	~Shader();

	//Set uniform functions
	void use();
	void unUse();
	void set1i(GLint value, const GLchar* name);
	void set1f(GLfloat value, const GLchar* name);
	void setVec2f(glm::fvec2 value, const GLchar* name);
	void setVec3f(glm::fvec3 value, const GLchar* name);
	void setVec4f(glm::fvec4 value, const GLchar* name);
	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);
	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

};