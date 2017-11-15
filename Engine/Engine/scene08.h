#pragma once


#include "scene.h"
#include <glm\mat4x4.hpp>

#include "material.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "directionalLight.h"
#include "spotlight.h"
#include "model.h"

class Renderer;
class Input;
class Timer;

class Scene08 : public Scene 
{
public:
	Scene08(Engine* e) : Scene(e) {}

	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

private:
	Renderer* m_rend;
	Input* m_input;
	Timer* m_timer;

	glm::vec3 camPos;
	glm::vec3 target;
	Camera::Data camData;
	Camera m_camera = {"camera", this};

	Light m_light = {"light", this};
	DirectionalLight m_directionalLight = {"directionalLight", this};
	SpotLight m_spotLight = {"spotLight", this};

	Model m_model = {"defaultModel", this};
	Model m_exoticModel = {"exoticModel", this};
	Model m_plane = {"ground", this};

	glm::vec3 worldAmbience;
};

