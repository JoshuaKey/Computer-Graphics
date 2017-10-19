#pragma once

#include "scene.h"
#include <glm\mat4x4.hpp>

class Renderer;
class Input;
class Timer;

class Scene03 : public Scene 
{
public:
	Scene03(Engine* e) : Scene(e) {}

	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	Renderer* m_rend;
	Input* m_input;
	Timer* m_timer;

	GLuint m_programID;
	GLuint m_vaoID;

	float m_camYaw, m_camPitch;
	glm::vec3 m_camPos;
	glm::vec3 m_camDirection;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	GLint m_modelUniform;
	GLint m_viewUniform;
	GLint m_projectionUniform;
	
	GLint m_ambienceUniform;
	GLint m_lightColorUniform;
	GLint m_lightPosUniform;
	GLint m_viewPosUniform;
};