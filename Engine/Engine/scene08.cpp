#include "stdafx.h"

#include "scene08.h"

#include "renderer.h"
#include "input.h"
#include "timer.h"
#include "image.h"
#include "meshLoader.h"

#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace
{
	bool isDir = true;
}

bool Scene08::Initialize()
{
	m_input = m_engine->Get<Input>();
	m_rend = m_engine->Get<Renderer>();
	m_timer = m_engine->Get<Timer>();

	// Keyboard Input
	{
		m_input->AddButton("W", Input::eButtonType::KEYBOARD, 'W');
		m_input->AddButton("S", Input::eButtonType::KEYBOARD, 'S');
		m_input->AddButton("A", Input::eButtonType::KEYBOARD, 'A');
		m_input->AddButton("D", Input::eButtonType::KEYBOARD, 'D');
		m_input->AddButton(" ", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);
		m_input->AddButton("LShift", Input::eButtonType::KEYBOARD, GLFW_KEY_LEFT_SHIFT);
		m_input->AddButton("Exit", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);
	}

	// Mouse Input
	{
		//glfwSetInputMode(m_rend->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_input->AddButton("mRight", Input::eButtonType::MOUSE, GLFW_MOUSE_BUTTON_RIGHT);
		m_input->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X, 0);
		m_input->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y, 0);
	}


	// Set Camera
	{
		float camYaw = 3.14f / 2;
		float camPitch = 0;
		auto x = glm::cos(camYaw);
		auto y = glm::cos(camPitch);
		glm::vec3 camDirection = {y * x, glm::sin(camPitch), y * glm::sin(-camYaw)};

		camPos = glm::vec3(.0f, .0f, .0f);
		target = glm::vec3(.0f, 0.0f, -3.0f);

		camData.distance = 1.0f;
		camData.fov = 3.14f / 3.0f;
		camData.nearClip = .01f;
		camData.farClip = 100.0f;
		camData.pitchMin = -3.14f / 2.0f;
		camData.pitchMax = 3.14f / 2.0f;
		camData.type = Camera::eType::EDITOR;
		m_camera.Initialize(camPos, target, camData);

		//m_camera.transform.rotation *= glm::quat(glm::vec3(.0f, 3.14f, .0f));

		AddObject(&m_camera);
	}

	worldAmbience = {.3f, .3f, .3f};

	// Directional Light
	{
		m_directionalLight.diffuse = {1.0f, 1.0f, 1.0f};
		m_directionalLight.specular = {1.0f, 1.0f, 1.0f};
		m_directionalLight.transform.rotation = glm::quat(glm::vec3(.3f, .7f, .1f));

		m_directionalLight.isStatic = true;

		AddObject(&m_directionalLight);
	}

	// Light 
	{
		m_light.m_shader.CompileShader("../Resources/Shaders/light3DVert.glsl", GL_VERTEX_SHADER);
		m_light.m_shader.CompileShader("../Resources/Shaders/light3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_light.m_shader.Link();
		m_light.m_shader.Use();

		m_light.diffuse = {1.0f, 1.0f, 1.0f};
		m_light.specular = {1.0f, 1.0f, 1.0f};

		m_light.m_shader.SetUniform("lightColor", glm::vec3(.0f, 1.0f, 1.0f));

		m_light.m_mesh.Load("../Resources/Meshes/box.obj");
		m_light.m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);

		m_light.transform.position = {.0f, .0f, -3.0f};
		m_light.transform.scale = {.2f, .2f, .2f};

		AddObject(&m_light);
	}

	// SpotLight 
	{
		m_spotLight.m_shader.CompileShader("../Resources/Shaders/light3DVert.glsl", GL_VERTEX_SHADER);
		m_spotLight.m_shader.CompileShader("../Resources/Shaders/light3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_spotLight.m_shader.Link();
		m_spotLight.m_shader.Use();

		m_spotLight.diffuse = {1.0f, 1.0f, 1.0f};
		m_spotLight.specular = {1.0f, 1.0f, 1.0f};
		m_spotLight.cutoff = 3.14f /3.0f;
		m_spotLight.exponent = .2f;

		m_spotLight.isStatic = true;

		m_spotLight.m_shader.SetUniform("lightColor", glm::vec3(.1f, .2f, .2f));

		m_spotLight.m_mesh.Load("../Resources/Meshes/solarPanel.obj");
		m_spotLight.m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);

		m_spotLight.transform.position = {.0f, -.5f, 0.0f};
		//m_spotLight.transform.rotation *= glm::quat(glm::vec3(.0f, 3.14f/2.0f, .0f));
		m_spotLight.transform.scale = {.05f, .05f, .05f};

		AddObject(&m_spotLight);
	}

	auto lights = GetObjects<Light>();
	int lightLength = lights.size();

	// Model
	{
		m_model.m_shader.CompileShader("../Resources/Shaders/multiLight3DVert.glsl", GL_VERTEX_SHADER);
		m_model.m_shader.CompileShader("../Resources/Shaders/multiLight3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_model.m_shader.Link();
		m_model.m_shader.Use();

		m_model.m_material.m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		m_model.m_material.m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		m_model.m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		m_model.m_material.m_shininess = 32.0f;
		m_model.m_material.LoadTexture2D("../Resources/Textures/crate.bmp", GL_TEXTURE0);

		m_model.m_shader.SetUniform("material.ambient", m_model.m_material.m_ambient);
		m_model.m_shader.SetUniform("material.diffuse", m_model.m_material.m_diffuse);
		m_model.m_shader.SetUniform("material.specular", m_model.m_material.m_specular);
		m_model.m_shader.SetUniform("material.shininess", m_model.m_material.m_shininess);

		m_model.m_shader.SetUniform("worldAmbience", worldAmbience);

		for (int i = 0; i < lightLength; i++)
		{
			lights[i]->SetUniform(i, m_model.m_shader);
		}

		m_model.m_shader.SetUniform("fog.distMin", 1.0f);
		m_model.m_shader.SetUniform("fog.distMax", 10.0f);
		m_model.m_shader.SetUniform("fog.color", glm::vec3(1.0f, .2f, .2f));
		glClearColor(1.0f, .2f, .2f, 1.0f);

		m_model.m_mesh.Load("../Resources/Meshes/cube_texture.obj");

		m_model.m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
		m_model.m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
		m_model.m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

		m_model.transform.position = {1.0f, .0f, -1.0f};

		AddObject(&m_model);
	}

	// Exotic Model 
	{
		m_exoticModel.m_shader.CompileShader("../Resources/Shaders/multiLight3DVert.glsl", GL_VERTEX_SHADER);
		m_exoticModel.m_shader.CompileShader("../Resources/Shaders/multiLight3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_exoticModel.m_shader.Link();
		m_exoticModel.m_shader.Use();

		m_exoticModel.m_material.m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		m_exoticModel.m_material.m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		m_exoticModel.m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		m_exoticModel.m_material.m_shininess = 32.0f;
		m_exoticModel.m_material.LoadTexture2D("../Resources/Textures/crate.bmp", GL_TEXTURE0);

		m_exoticModel.m_shader.SetUniform("material.ambient", m_model.m_material.m_ambient);
		m_exoticModel.m_shader.SetUniform("material.diffuse", m_model.m_material.m_diffuse);
		m_exoticModel.m_shader.SetUniform("material.specular", m_model.m_material.m_specular);
		m_exoticModel.m_shader.SetUniform("material.shininess", m_model.m_material.m_shininess);
		m_model.m_shader.SetUniform("worldAmbience", worldAmbience);

		for (int i = 0; i < lightLength; i++)
		{
			lights[i]->SetUniform(i, m_exoticModel.m_shader);
		}

		m_exoticModel.m_shader.SetUniform("fog.distMin", 1.0f);
		m_exoticModel.m_shader.SetUniform("fog.distMax", 10.0f);
		m_exoticModel.m_shader.SetUniform("fog.color", glm::vec3(1.0f, .2f, .2f));

		m_exoticModel.m_mesh.Load("../Resources/Meshes/humanoid_tri.obj");

		m_exoticModel.m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
		m_exoticModel.m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
		m_exoticModel.m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

		m_exoticModel.transform.position = {-1.0f, .0f, 0.0f};
		m_exoticModel.transform.rotation = glm::quat(glm::vec3(-3.14f / 2.0f, .0f, .0f));
		m_exoticModel.transform.scale = {.05f, .05f, .05f};


		AddObject(&m_exoticModel);
	}

	// Plane
	{
		m_plane.m_shader.CompileShader("../Resources/Shaders/multiLight3DVert.glsl", GL_VERTEX_SHADER);
		m_plane.m_shader.CompileShader("../Resources/Shaders/multiLight3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_plane.m_shader.Link();
		m_plane.m_shader.Use();

		m_plane.m_material.m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		m_plane.m_material.m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		m_plane.m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		m_plane.m_material.m_shininess = 32.0f;
		m_plane.m_material.LoadTexture2D("../Resources/Textures/crate.bmp", GL_TEXTURE0);

		m_plane.m_shader.SetUniform("material.ambient", m_model.m_material.m_ambient);
		m_plane.m_shader.SetUniform("material.diffuse", m_model.m_material.m_diffuse);
		m_plane.m_shader.SetUniform("material.specular", m_model.m_material.m_specular);
		m_plane.m_shader.SetUniform("material.shininess", m_model.m_material.m_shininess);

		m_model.m_shader.SetUniform("worldAmbience", worldAmbience);

		for (int i = 0; i < lightLength; i++)
		{
			lights[i]->SetUniform(i, m_plane.m_shader);
		}

		m_plane.m_shader.SetUniform("fog.distMin", 1.0f);
		m_plane.m_shader.SetUniform("fog.distMax", 10.0f);
		m_plane.m_shader.SetUniform("fog.color", glm::vec3(1.0f, .2f, .2f));

		m_plane.m_mesh.Load("../Resources/Meshes/plane.obj");

		m_plane.m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
		m_plane.m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
		m_plane.m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

		m_plane.transform.position = {0.0f, -1.0f, 0.0f};

		m_plane.transform.scale = {20.0f, 1.0f, 20.0f};


		AddObject(&m_plane);
	}

	////m_directionalLight.Initialize();
	//if (isDir)
	//{
	//	m_model.m_shader.Use();
	//	m_model.m_shader.SetUniform("light.position", glm::vec4(m_directionalLight.direction, .0f));
	//	m_exoticModel.m_shader.Use();
	//	m_exoticModel.m_shader.SetUniform("light.position", glm::vec4(m_directionalLight.direction, .0f));
	//	m_plane.m_shader.Use();
	//	m_plane.m_shader.SetUniform("light.position", glm::vec4(m_directionalLight.direction, .0f));
	//} else 
	//{
	//	m_model.m_shader.Use();
	//	m_model.m_shader.SetUniform("light.position", glm::vec4(m_light.transform.position, 1.0f));
	//	m_exoticModel.m_shader.Use();
	//	m_exoticModel.m_shader.SetUniform("light.position", glm::vec4(m_light.transform.position, 1.0f));
	//	m_plane.m_shader.Use();
	//	m_plane.m_shader.SetUniform("light.position", glm::vec4(m_light.transform.position, 1.0f));
	//}

	return true;
}
void Scene08::Update()
{
	float dt = m_timer->GetDeltaTime();

	int length = m_objects.size();
	for (int i = 0; i < length; i++)
	{
		auto obj = m_objects[i];
		obj->Update();
	}

	auto relPos = m_light.transform.position - m_model.transform.position;
	relPos = glm::vec3(glm::rotate(glm::mat4(1.0f), 1.0f * dt, glm::vec3(.0f, 1.0f, .0f)) * glm::vec4(relPos, 1.0f));
	m_light.transform.position = m_model.transform.position + relPos;

	auto lights = GetObjects<Light>();
	int lightLength = lights.size();
	for (int i = 0; i < lightLength; i++)
	{
		if (!lights[i]->isStatic)
		{
			lights[i]->SetUniform(i, m_model.m_shader);
			lights[i]->SetUniform(i, m_exoticModel.m_shader);
			lights[i]->SetUniform(i, m_plane.m_shader);
		}
		
	}

	auto exitBut = m_input->GetButton("Exit");
	if ( exitBut != Input::eButtonState::UP) 
	{
		glfwSetWindowShouldClose(m_rend->m_window, true);
	}

	// Key Input
	glm::vec3 mvmt(.0f);
	{
		auto direction = m_camera.GetDirection();
		auto wBut = m_input->GetButton("W");
		auto aBut = m_input->GetButton("A");
		auto sBut = m_input->GetButton("S");
		auto dBut = m_input->GetButton("D");
		auto spaceBut = m_input->GetButton (" ");
		auto lShiftBut = m_input->GetButton ("LShift");

		if ( wBut == Input::eButtonState::DOWN || wBut == Input::eButtonState::HELD) 
		{
			mvmt += direction * dt;
		}
		if ( sBut == Input::eButtonState::DOWN || sBut == Input::eButtonState::HELD) 
		{
			mvmt -= direction * dt;
		}
		if ( dBut == Input::eButtonState::DOWN || dBut == Input::eButtonState::HELD) 
		{
			mvmt += glm::cross(direction, glm::vec3(.0f, 1.0f, .0f)) * dt;
		}
		if ( aBut == Input::eButtonState::DOWN || aBut == Input::eButtonState::HELD) 
		{
			mvmt -= glm::cross(direction, glm::vec3(.0f, 1.0f, .0f)) * dt;
		}
		if ( spaceBut == Input::eButtonState::DOWN || spaceBut == Input::eButtonState::HELD) 
		{
			mvmt += glm::vec3 (.0f, 1.0f, .0f) * dt;
		}
		if ( lShiftBut == Input::eButtonState::DOWN || lShiftBut == Input::eButtonState::HELD) 
		{
			mvmt -= glm::vec3 (.0f, 1.0f, .0f) * dt;
		}
	}


	// Mouse Input
	glm::vec3 rotate(.0f);
	{
		static float currMouseX = m_input->GetAnalogAbsolute("x-axis");
		static float currMouseY = m_input->GetAnalogAbsolute("y-axis");
		float tempMouseX = m_input->GetAnalogAbsolute("x-axis");
		float tempMouseY = m_input->GetAnalogAbsolute("y-axis");

		if (m_input->GetButton("mRight") != Input::eButtonState::UP)
		{
			rotate.x = (tempMouseY - currMouseY) / 200.0f;
			rotate.y = (tempMouseX - currMouseX) / 200.0f;
		}

		currMouseX = tempMouseX;
		currMouseY = tempMouseY;
	}

	m_camera.UpdateTransformEditor(mvmt, rotate);

	camPos += mvmt;
	target += mvmt;
}
void Scene08::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderables = GetObjects<Renderable>();
	int length = renderables.size();
	for (int i = 0; i < length; i++)
	{
		renderables[i]->Render();
	}

	//m_plane.Render();

	glfwSwapBuffers(m_rend->m_window);
}
void Scene08::Shutdown() {}