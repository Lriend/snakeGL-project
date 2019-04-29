#pragma once

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "material.h"

enum TEXTURE_UNITS{DIFFUSE_TEX, SPECULAR_TEX};

class Model
{
	//Member vars
	Material * material;
	Texture * overrideTextureDiffuse;
	Texture * overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 origin;
	
	//Private functions
	void updateUniforms();
public:
	//Ctor & dtor
	Model(glm::vec3 origin, Material * material, Texture * overrideTextureDiffuse, Texture * overrideTextureSpecular, std::vector<Mesh*> meshes);
	~Model();

	//Functions
	void rotate(const glm::vec3 rotation);
	void move(const glm::vec3 move);
	glm::vec3 getOrigin();
	void setOrigin(glm::vec3 origin);

	float getRotationAtZ(int meshesID);

	void update();
	void render(Shader* shader);
};
