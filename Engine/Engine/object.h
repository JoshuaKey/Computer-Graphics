#pragma once

#include "transform.h"

class Scene;

class Object
{
public:
	const char* name;
	Transform transform;
	bool isStatic = false;

protected:
	Scene* m_scene;

public:
	Object(const char* name, Scene* scene) : name(name), m_scene(scene) {}
	~Object() {}

	virtual void Update() = 0;



};