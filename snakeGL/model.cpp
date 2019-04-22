#include "model.h"


//Private functions
void Model::updateUniforms() {

}

//Ctor & dtor
Model::Model(glm::vec3 origin, Material * material, Texture * overrideTextureDiffuse, Texture * overrideTextureSpecular, std::vector<Mesh*> meshes)
{
	this->origin = origin;
	this->material = material;
	this->overrideTextureDiffuse = overrideTextureDiffuse;
	this->overrideTextureSpecular = overrideTextureSpecular;

	for (auto* i : meshes) this->meshes.push_back(new Mesh(*i));
}

Model::~Model()
{
	for (auto*& i : this->meshes) delete i;
}

void Model::rotate(const glm::vec3 rotation)
{
	for (auto& i : this->meshes) i->rotate(rotation);
}

//Functions
void Model::update()
{

}

void Model::render(Shader* shader)
{
	this->updateUniforms();

	this->material->sendToShader(*shader);

	shader->use();

	this->overrideTextureDiffuse->bind(DIFFUSE_TEX);
	this->overrideTextureSpecular->bind(SPECULAR_TEX);

	for (auto& i : this->meshes) i->render(shader);
}
