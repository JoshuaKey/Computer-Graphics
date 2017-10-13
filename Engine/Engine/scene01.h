#pragma once
#include "scene.h"

class Renderer;

class Scene01 : public Scene 
{
public:
	Scene01(Engine* e) : Scene(e) {}

	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	Renderer* m_rend;

	GLuint m_programID;
	GLuint m_vaoID;
};
