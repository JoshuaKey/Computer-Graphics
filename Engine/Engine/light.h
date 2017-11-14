#pragma once

#include "renderable.h"

class Light : public Renderable
{
public:
	Light(const char* name, Scene* scene) : Renderable(name, scene) {}

	virtual void Update();
	virtual void Render();

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Mesh m_mesh;
};
