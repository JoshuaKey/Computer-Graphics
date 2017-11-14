#pragma once

#include "object.h"
class Scene;

class DirectionalLight : public Object
{
public:
	DirectionalLight(const char* name, Scene* scene) : Object(name, scene) {}

	void Initialize();
	virtual void Update();


public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 direction;
};
