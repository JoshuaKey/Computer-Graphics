#pragma once

#include "scene.h"
#include <glm\mat4x4.hpp>

class Renderer;
class Input;

class ScenePractice : public Scene 
{
public:
	ScenePractice(Engine* e) : Scene(e) {}

	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	Renderer* m_rend;
	Input* m_input;

	GLuint m_programID;
	GLuint m_vaoID;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	GLuint m_modelUniform;
	GLuint m_viewUniform;
	GLuint m_projectionUniform;
};
