#pragma once

#include <iostream>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "SOIL2.h"

class Texture
{
	//Member vars
	GLuint id;
	int width;
	int height;
	unsigned int type;
public:
	//Ctor & dtor
	Texture(const char* fileName, GLenum type);
	~Texture();

	//Getters
	GLuint getID() const;

	//Functions
	void bind(const GLint textureUnit);
	void unbind();
	void loadFromFile(const char* fileName);
};

