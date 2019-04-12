#include "prefab.h"

//PREFAB ----------------------------------------------------------------------------------------------------------------
//Ctor & dtor
Prefab::Prefab(){}

Prefab::~Prefab(){}

//Setters
void Prefab::set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices) {
	for (size_t i = 0; i < nrOfVertices; i++) this->vertices.push_back(vertices[i]);
	for (size_t i = 0; i < nrOfIndices; i++) this->indices.push_back(indices[i]);
}

//Getters
Vertex* Prefab::getVertices() { return this->vertices.data(); }

GLuint* Prefab::getIndices() { return this->indices.data(); }

const unsigned Prefab::getNrOfVertices() { return this->vertices.size(); }

const unsigned Prefab::getNrOfIndices() { return this->indices.size(); }

//TRIANGLE
//BASIC ONE-SIDED REGULAR TRIANGLE
Triangle::Triangle() : Prefab() {
	Vertex vertices[] =
	{
		//Positions								//Colors						//texcoords					//Normals
		glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f)
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		0, 1, 2,	//Triangle 1
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

//QUAD ------------------------------------------------------------------------------------------------------------------
//TWO TRIANGLES IN QUAD SHAPE
Quad::Quad() : Prefab() {
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
	
	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

//PLANE -----------------------------------------------------------------------------------------------------------------
//TWO-SIDED QUAD
Plane::Plane() : Prefab() {
	Vertex vertices[] =
	{
		//Positions								//Colors						//texcoords					//Normals
		glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),	//0
		glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	//1
		glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	//2
		glm::vec3(0.5f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),	//3

		glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),	//4
		glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),	//5
		glm::vec3(0.5f, -0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),	//6
		glm::vec3(0.5f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, -1.f)	//7
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		0, 1, 2,	//Triangle 1  FRONT
		0, 2, 3, 	//Triangle 2  FRONT
		6, 5, 4,	//Triangle 3  BACK
		7, 6, 4		//Triangle 4  BACK
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);
	
	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}