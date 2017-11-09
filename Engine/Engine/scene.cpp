#include "stdafx.h"

#include "scene.h"

//#include "object.h"

Scene::~Scene()
{
	int length = m_objects.size();
	for (int i = 0; i < length; i++)
	{
		auto obj = m_objects[i];
		delete obj;
	}
}

void Scene::AddObject(Object* obj)
{
	m_objects.push_back(obj);
}