#pragma once


#include "light.h"

class SpotLight : public Light
{
public:
	SpotLight(const char* name, Scene* scene) : Light(name, scene) {}

	virtual void Update();
	virtual void Render();
	virtual void SetUniform(int id, Shader &s);

public:
	float exponent;
	float cutoff;
};