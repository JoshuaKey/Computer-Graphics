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

class Scene11 : public Scene 
{
public:
	Scene11(Engine* e) : Scene(e) {}

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
	Model m_uiTexture = {"uiTexture", this};
	Model m_skyBox = {"skybox", this};
	Model m_suzanne = {"reflection", this};

	glm::vec3 m_worldAmbience;

	const int Rend_Text_WIDTH = 800;
	const int Rend_Text_HEIGHT = 600;

	GLuint m_frameBuffer;
	GLuint m_renderedTexture;

	glm::vec3 m_clearColor;
};

