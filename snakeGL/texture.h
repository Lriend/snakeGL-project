#pragma once

#include <iostream>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "SOIL2.h"

class Texture
{
	GLuint id;
	int width;
	int height;
	unsigned int type;
	GLint textureUnit;
public:
	//Ctor & dtor
	Texture(const char* fileName, GLenum type, GLint texture_unit);
	~Texture();

	//Getters
	GLuint getID() const;
	GLint getTextureUnit() const;

	//Functions
	void bind();
	void unbind();
	void loadFromFile(const char* fileName);
};

