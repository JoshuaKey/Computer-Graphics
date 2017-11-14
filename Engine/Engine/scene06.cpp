#include "stdafx.h"

#include "scene06.h"

#include "renderer.h"
#include "input.h"
#include "timer.h"
#include "image.h"
#include "meshLoader.h"

#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>

bool Scene06::Initialize()
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

		camPos = glm::vec3(.0f, .0f, 1.5f);
		target = glm::vec3(.0f, .0f, .0f);

		camData.distance = 1.0f;
		camData.fov = 3.14f / 3.0f;
		camData.nearClip = .01f;
		camData.farClip = 100.0f;
		camData.pitchMin = -3.14f / 2.0f;
		camData.pitchMax = 3.14f / 2.0f;
		camData.type = Camera::eType::EDITOR;
		m_camera.Initialize(camPos, target, camData);

		m_camera.transform.rotation *= glm::quat(glm::vec3(.0f, 3.14f, .0f));

		AddObject(&m_camera);
	}

	// Light 
	{
		m_light.ambient = {1.0f, 1.0f, 1.0f};
		m_light.diffuse = {1.0f, 1.0f, 1.0f};
		m_light.specular = {1.0f, 1.0f, 1.0f};
		m_light.transform.position = {.0f, 5.0f, .0f};

		AddObject(&m_light);
	}

	// Model
	{
		m_model.m_shader.CompileShader("../Resources/Shaders/default3DVert.glsl", GL_VERTEX_SHADER);
		m_model.m_shader.CompileShader("../Resources/Shaders/default3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_model.m_shader.Link();
		m_model.m_shader.Use();

		m_model.m_material.m_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		m_model.m_material.m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		m_model.m_material.m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
		m_model.m_material.m_shininess = 32.0f;
		m_model.m_material.m_ambience = .25f;
		m_model.m_material.LoadTexture2D("../Resources/Textures/crate.bmp", GL_TEXTURE0);

		m_model.m_shader.SetUniform("material.ambient", m_model.m_material.m_ambient);
		m_model.m_shader.SetUniform("material.diffuse", m_model.m_material.m_diffuse);
		m_model.m_shader.SetUniform("material.specular", m_model.m_material.m_specular);
		m_model.m_shader.SetUniform("material.shininess", m_model.m_material.m_shininess);
		m_model.m_shader.SetUniform("material.ambience", m_model.m_material.m_ambience);

		m_model.m_shader.SetUniform("light.ambient", m_light.ambient);
		m_model.m_shader.SetUniform("light.diffuse", m_light.diffuse);
		m_model.m_shader.SetUniform("light.specular", m_light.specular);

		m_model.m_shader.SetUniform("fog.distMin", 1.0f);
		m_model.m_shader.SetUniform("fog.distMax", 10.0f);
		m_model.m_shader.SetUniform("fog.color", glm::vec3(1.0f, .2f, .2f));
		glClearColor(1.0f, .2f, .2f, 1.0f);

		m_model.m_shader.SetUniform("light.position", m_light.transform.position);

		m_model.m_mesh.Load("../Resources/Meshes/bunny.obj");

		m_model.m_mesh.BindVertexAttrib(0, Mesh::eVertexType::POSITION);
		m_model.m_mesh.BindVertexAttrib(1, Mesh::eVertexType::NORMAL);
		m_model.m_mesh.BindVertexAttrib(2, Mesh::eVertexType::TEXCOORD);

		m_model.transform.position = {.0f, .0f, 1.0f};
		

		AddObject(&m_model);
	}


	return true;
}
void Scene06::Update()
{
	float dt = m_timer->GetDeltaTime();

	int length = m_objects.size();
	for (int i = 0; i < length; i++)
	{
		auto obj = m_objects[i];
		obj->Update();
	}

	//glm::quat val(glm::vec3(3.14f/3 * dt, .0f, .0f));
	//m_model.transform.rotation *= val;

	auto exitBut = m_input->GetButton("Exit");
	if ( exitBut != Input::eButtonState::UP) 
	{
		glfwSetWindowShouldClose(m_rend->m_window, true);
	}

	// Key Input
	glm::vec3 mvmt(.0f);
	{
		auto direction = m_camera.GetDirection();
		//auto direction = glm::vec3(1.0f, .0f, .0f);
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
	// .04 -.8 2.1
	m_camera.UpdateTransformEditor(mvmt, rotate);
	//camPos += mvmt;
	//m_viewMatrix = glm::lookAt(camPos, camPos + direction, glm::vec3(.0f, 1.0f, .0f));
}
void Scene06::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_model.Render();
	/*
	// Bind
	glBindVertexArray(m_vaoID);
	m_shader.Use();

	// Bind Uniforms
	{
		m_material.SetTextures();
		m_shader.SetUniform("texture1", 0);
		m_shader.SetUniform("texture2", 1);

		m_shader.SetUniform("material.ambience", m_material.m_ambience);
		m_shader.SetUniform("material.brightness", m_material.m_brightness);
		m_shader.SetUniform("material.ambient", m_material.m_ambient);
		m_shader.SetUniform("material.diffuse", m_material.m_diffuse);
		m_shader.SetUniform("material.specular",m_material.m_specular);

		m_shader.SetUniform("light.ambient", m_light.ambient);
		m_shader.SetUniform("light.diffuse", m_light.diffuse);
		m_shader.SetUniform("light.specular", m_light.specular);
		m_shader.SetUniform("light.position", m_light.transform.position);

		m_shader.SetUniform("viewPos", m_camera.transform.position);

		m_shader.SetUniform("model", m_modelMatrix);
		m_shader.SetUniform("view", m_viewMatrix);
		m_shader.SetUniform("projection", m_projectionMatrix);
	}

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, vertices);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);
	*/

	glfwSwapBuffers(m_rend->m_window);
}
void Scene06::Shutdown()
{
	/*glDeleteVertexArrays(1, &m_vaoID);*/
}