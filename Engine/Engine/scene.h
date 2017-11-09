#pragma once

#include "engine.h"

#include "object.h"

class Scene
{
public:
	Engine* m_engine;
	std::vector<Object*> m_objects;

	Scene (Engine* e) : m_engine (e) {}
	virtual ~Scene();

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Shutdown()= 0;

	void AddObject(Object* obj);

	template<typename T>
	T* GetObject(const char* name);

	template<typename T>
	std::vector<T*> GetObjects();
};

template<typename T>
T* Scene::GetObject(const char* name)
{
	int length = m_objects.size();
	for (int i = 0; i < length; i++)
	{
		auto obj = m_objects[i];
		if (!strcmp(obj->name, name))
		{
			return static_cast<T*>(obj);
		}
	}
	return nullptr;
}

template<typename T>
std::vector<T*> Scene::GetObjects()
{
	int length = m_objects.size();
	std::vector<T*> objects;
	for (int i = 0; i < length; i++)
	{
		auto obj = m_objects[i];
		if (!strcmp(obj->name, name))
		{
			objects.push_back(static_cast<T*>(obj));
		}
	}
	return objects;
}
