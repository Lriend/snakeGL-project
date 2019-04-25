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
	Vertex* vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;

	GLuint VAO, VBO, EBO;
	
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	glm::mat4 ModelMatrix;

	//Private functions
	void initVAO();
	void updateModelMatrix();
	void updateUniforms(Shader* shader);
public:
	//Ctors & dtor
	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, 
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	Mesh(Prefab* prefab, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));
	Mesh(const Mesh& obj);
	~Mesh();

	//Getters
	glm::vec3 getPosition();
	glm::vec3 getRotation();

	//Setters
	void setPosition(const glm::vec3 position);
	void setRotation(const glm::vec3 rotation);
	void setScale(const glm::vec3 scale);

	//Functions
	void move(const glm::vec3 position);
	void rotate(const glm::vec3 rotation);
	void scaleUp(const glm::vec3 scale);
	void render(Shader* shader);
};

