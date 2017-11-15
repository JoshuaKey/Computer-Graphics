#pragma once

#include "light.h"
class Scene;

class DirectionalLight : public Light
{
public:
	DirectionalLight(const char* name, Scene* scene) : Light(name, scene) {}

	virtual void Update();
	virtual void Render();
	virtual void SetUniform(int id, Shader &s);

public:
};
