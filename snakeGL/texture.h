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
	Texture(const char* fileName, GLenum type, GLint texture_unit);
	~Texture();
	GLuint getID() const;
	void bind();
	void unbind();
	GLint getTextureUnit() const;
	void loadFromFile(const char* fileName);
};

