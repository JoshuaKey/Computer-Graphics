#pragma once

#include "renderable.h"
#include "shader.h"

class Light : public Renderable
{
public:
	Light(const char* name, Scene* scene) : Renderable(name, scene) {}

	virtual void Update();
	virtual void Render();
	virtual void SetUniform(int id, Shader &s);

public:
	glm::vec3 diffuse;
	glm::vec3 specular;

	Mesh m_mesh;
};
