#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <vector>
#include "vertex.h"

class Prefab
{
	//Member vars
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:
	//Ctor & dtor
	Prefab();
	virtual ~Prefab();

	//Setters
	void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices);
	
	//Getters
	Vertex* getVertices();
	GLuint* getIndices();
	const unsigned getNrOfVertices();
	const unsigned getNrOfIndices();
};

class Triangle : public Prefab {
public:
	Triangle();
};

class Quad : public Prefab {
public:
	Quad();
};

class Plane : public Prefab {
public:
	Plane();
};

class Cube : public Prefab {
public:
	Cube();
};