#pragma once

#include "renderable.h"

class Model : public Renderable
{
public:
	Model(const char* name, Scene* scene);
	~Model();

	virtual void Render();
	virtual void Update();
	

public:
	Mesh m_mesh; // Should this be a pointer???
	Material m_material;

};