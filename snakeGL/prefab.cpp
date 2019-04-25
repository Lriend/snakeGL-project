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
		//Positions													//texcoords					//Normals
		glm::vec3(0.f, 0.5f, 0.f),					glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-0.5f, -0.5f, 0.f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, -0.5f, 0.f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f)
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
		//Positions									//texcoords					//Normals
		glm::vec3(-0.5f, 0.5f, 0.f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(-0.5f, -0.5f, 0.f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, -0.5f, 0.f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, 0.5f, 0.f),					glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)
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
		//Positions									//texcoords					//Normals
		glm::vec3(-0.5f, 0.5f, 0.f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),	//0
		glm::vec3(-0.5f, -0.5f, 0.f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	//1
		glm::vec3(0.5f, -0.5f, 0.f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	//2
		glm::vec3(0.5f, 0.5f, 0.f),					glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),	//3

		glm::vec3(-0.5f, 0.5f, 0.f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),	//4
		glm::vec3(-0.5f, -0.5f, 0.f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),	//5
		glm::vec3(0.5f, -0.5f, 0.f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),	//6
		glm::vec3(0.5f, 0.5f, 0.f),					glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, -1.f)	//7
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		0, 1, 2, 0, 2, 3, 	//Quad Front
		6, 5, 4, 7, 6, 4	//Quad Back
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

//CUBE ------------------------------------------------------------------------------------------------------------------
Cube::Cube() : Prefab() {

	Vertex vertices[] =
	{
		//Positions									//texcoords					//Normals
		glm::vec3(-0.5f, 0.5f, 0.5f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),	//0
		glm::vec3(-0.5f, -0.5f, 0.5f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	//1		FRONT
		glm::vec3(0.5f, -0.5f, 0.5f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),	//2
		glm::vec3(0.5f, 0.5f, 0.5f),				glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),	//3

		glm::vec3(-0.5f, 0.5f, -0.5f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),	//4
		glm::vec3(-0.5f, -0.5f, -0.5f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),	//5		BACK
		glm::vec3(0.5f, -0.5f, -0.5f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),	//6
		glm::vec3(0.5f, 0.5f, -0.5f),				glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),	//7

		glm::vec3(-0.5f, 0.5f, -0.5f),				glm::vec2(0.f, 1.f),		glm::vec3(-1.f, 0.f, 0.f),	//8
		glm::vec3(-0.5f, -0.5f, -0.5f),				glm::vec2(0.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),	//9		LEFT
		glm::vec3(-0.5f, -0.5f, 0.5f),				glm::vec2(1.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),	//10
		glm::vec3(-0.5f, 0.5f, 0.5f),				glm::vec2(1.f, 1.f),		glm::vec3(-1.f, 0.f, 0.f),	//11
		
		glm::vec3(0.5f, 0.5f, -0.5f),				glm::vec2(0.f, 1.f),		glm::vec3(1.f, 0.f, 0.f),	//12
		glm::vec3(0.5f, -0.5f, -0.5f),				glm::vec2(0.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),	//13	RIGHT
		glm::vec3(0.5f, -0.5f, 0.5f),				glm::vec2(1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),	//14
		glm::vec3(0.5f, 0.5f, 0.5f),				glm::vec2(1.f, 1.f),		glm::vec3(1.f, 0.f, 0.f),	//15

		glm::vec3(-0.5f, 0.5f, -0.5f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, 1.f, 0.f),	//16
		glm::vec3(-0.5f, 0.5f, 0.5f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, 1.f, 0.f),	//17	UP
		glm::vec3(0.5f, 0.5f, 0.5f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, 1.f, 0.f),	//18
		glm::vec3(0.5f, 0.5f, -0.5f),				glm::vec2(1.f, 1.f),		glm::vec3(0.f, 1.f, 0.f),	//19

		glm::vec3(-0.5f, -0.5f, -0.5f),				glm::vec2(0.f, 1.f),		glm::vec3(0.f, -1.f, 0.f),	//20
		glm::vec3(-0.5f, -0.5f, 0.5f),				glm::vec2(0.f, 0.f),		glm::vec3(0.f, -1.f, 0.f),	//21	DOWN
		glm::vec3(0.5f, -0.5f, 0.5f),				glm::vec2(1.f, 0.f),		glm::vec3(0.f, -1.f, 0.f),	//22
		glm::vec3(0.5f, -0.5f, -0.5f),				glm::vec2(1.f, 1.f),		glm::vec3(0.f, -1.f, 0.f)	//23
		//Usunac powtarzajace sie vertices!
	};
	unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);
	
	GLuint indices[] =
	{
		0, 1, 2, 0, 2, 3, 	//Quad Front
		6, 5, 4, 7, 6, 4,	//Quad Back
		9, 10, 8, 10, 11, 8,	//Quad Left
		12, 14, 13, 12, 15, 14,	//Quad Right
		16, 17, 18, 16, 18, 19,	//Quad Up
		22, 21, 20, 23, 22, 20 //Quad down
	};
	unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

	this->set(vertices, nrOfVertices, indices, nrOfIndices);
}

Object::Object(const char* fileName)
{
	std::ifstream in(fileName, std::ios::in);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	if (!in)
	{
		std::cerr << "Cannot open " << fileName << std::endl;
		exit(1);
	}
	std::string line;
	while (std::getline(in, line))
	{
		//check v for vertices
		if (line.substr(0, 2) == "v ") {
			std::istringstream v(line.substr(2));
			glm::vec3 vert; double x, y, z;
			v >> x; v >> y; v >> z;
			vert = glm::vec3(x, y, z);
			positions.push_back(vert);
		}
		else if (line.substr(0, 3) == "vn ") {
			std::istringstream v(line.substr(3));
			glm::vec3 vert;	double x, y, z;
			v >> x; v >> y; v >> z;
			vert = glm::vec3(x, y, z);
			normals.push_back(vert);
		}
		////check for faces
		//else if (line.substr(0, 2) == "f ") {
		//	int a, b, c; //to store mesh index
		//	std::istringstream v(line.substr(2));
		//	v >> a; v >> b; v >> c;
		//	indices.push_back(a);
		//	indices.push_back(b);
		//	indices.push_back(c);
		//}
	}
	for (size_t i = 0; i < positions.size(); i++) {
		Vertex vert;
		vert.position = positions[i];
		vert.texcoord = glm::vec2(rand()%1, rand()%1);
		vert.normal = normals[i/3];
		vertices.push_back(vert);
	}
}
