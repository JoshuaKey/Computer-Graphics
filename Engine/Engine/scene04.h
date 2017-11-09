#pragma once

#include "scene.h"
#include <glm\mat4x4.hpp>

#include "material.h"
#include "shader.h"
#include "camera.h"
#include "light.h"

class Renderer;
class Input;
class Timer;

class Scene04 : public Scene 
{
public:
	Scene04(Engine* e) : Scene(e) {}

	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	Renderer* m_rend;
	Input* m_input;
	Timer* m_timer;

	Material m_material;
	Shader m_shader;

	glm::vec3 target;
	glm::vec3 camPos;
	Camera::Data camData;
	Camera m_camera = {"camera", this};

	Light m_light = {"light", this};

	GLuint m_vaoID;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	int vertices;
};
