#pragma once

#include "object.h"

class Light : public Object
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(const char* name, Scene* scene) : Object(name, scene) {}

	virtual void Update();
};
