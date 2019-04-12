#include "mesh.h"

//Private functions
void Mesh::initVAO(Prefab* prefab) {
	this->nrOfVertices = prefab->getNrOfVertices();
	this->nrOfIndices = prefab->getNrOfIndices();

	//VAO, VBO, EBO
	//GEN VAO AND BIND
	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), prefab->getVertices(), GL_DYNAMIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), prefab->getIndices(), GL_DYNAMIC_DRAW);

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
}

void Mesh::initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices) {
	this->nrOfVertices = nrOfVertices;
	this->nrOfIndices = nrOfIndices;

	//VAO, VBO, EBO
	//GEN VAO AND BIND
	glCreateVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//GEN VBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), vertexArray, GL_DYNAMIC_DRAW);

	//GEN EBO AND BIND AND SEND DATA
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices*sizeof(GLuint), indexArray, GL_DYNAMIC_DRAW);

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
}

void Mesh::updateModelMatrix() {
	this->ModelMatrix = glm::mat4(1.f);
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);					//moveXYZ
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));	//rotateX
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));	//rotateY
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));	//rotateZ
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);									//scaleXYZ
}

void Mesh::updateUniforms(Shader* shader) {
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

//Ctors & dtor
Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
	this->updateModelMatrix();
}

Mesh::Mesh(Prefab* prefab, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->initVAO(prefab);
	this->updateModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

//Getters

//Setters
void Mesh::setPosition(const glm::vec3 position) {
	this->position = position;
}

void Mesh::setRotation(const glm::vec3 rotation) {
	this->rotation = rotation;
}

void Mesh::setScale(const glm::vec3 scale) {
	this->scale = scale;
}

//Functions
void Mesh::move(const glm::vec3 position) {
	this->position += position;
}

void Mesh::rotate(const glm::vec3 rotation) {
	this->rotation += rotation;
}

void Mesh::scaleUp(const glm::vec3 scale) {
	this->scale += scale;
}

void Mesh::render(Shader* shader){
	//Update uniforms
	this->updateModelMatrix();
	this->updateUniforms(shader);

	shader->use();

	//Bind vertex array object
	glBindVertexArray(this->VAO);

	//Render
	if (!this->nrOfIndices) glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
	else glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
}