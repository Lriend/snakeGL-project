#pragma once

#include <iostream>
#include <vector>

#include "vertex.h"
#include "prefab.h"
#include "shader.h"
#include "texture.h"
#include "material.h"

class Mesh
{
	//Member vars
	unsigned nrOfVertices;
	unsigned nrOfIndices;
	GLuint VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;

	//Private functions
	void initVAO(Prefab* prefab);
	void initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices);
	void updateModelMatrix();
	void updateUniforms(Shader* shader);
public:
	//Ctors & dtor
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, 
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	Mesh(Prefab* prefab, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	~Mesh();

	//Getters
	//Setters
	void setPosition(const glm::vec3 position);
	void setRotation(const glm::vec3 rotation);
	void setScale(const glm::vec3 scale);

	//Functions
	void move(const glm::vec3 position);
	void rotate(const glm::vec3 rotation);
	void scaleUp(const glm::vec3 scale);
	void update();
	void render(Shader* shader);
};

