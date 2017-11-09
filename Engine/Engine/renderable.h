#pragma once

#include "object.h"

#include "shader.h"
#include "material.h"
#include "mesh.h"

class Renderable : public Object
{
public:
	Shader m_shader;
	Material m_material;

public:
	Renderable(const char* name, Scene* scene) : Object(name, scene) {}
	~Renderable() {}

	void virtual Render() = 0;

};
