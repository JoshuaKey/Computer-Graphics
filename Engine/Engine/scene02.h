#pragma once

#include "scene.h"
#include <glm\mat4x4.hpp>

class Renderer;
class Input;

class Scene02 : public Scene 
{
public:
	Scene02(Engine* e) : Scene(e) {}

	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	Renderer* m_rend;
	Input* m_input;

	GLuint m_programID;
	GLuint m_vaoID;

	glm::mat4 m_cursorModelMatrix;
	GLuint m_cursorModelUniform;

	int m_mouseX;
	int m_mouseY;
	float m_cursorSize = 1.0f;
};
