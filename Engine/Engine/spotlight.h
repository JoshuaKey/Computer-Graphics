#pragma once


#include "renderable.h"

class SpotLight : public Renderable
{
public:
	SpotLight(const char* name, Scene* scene) : Renderable(name, scene) {}

	virtual void Update();
	virtual void Render();

public:
	// Position and Rotation are in Transform
	glm::vec3 diffuse;
	glm::vec3 specular;
	float exponent;
	float cutoff;
	Mesh m_mesh;
};