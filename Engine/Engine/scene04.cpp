#include "stdafx.h"

#include "scene04.h"

#include "renderer.h"
#include "input.h"
#include "timer.h"
#include "image.h"
#include "meshLoader.h"

#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>

float vertexData2[] =
{
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f,  0.8f, 0.0f,

	1.0f,  0.0f, 0.0f,
	0.0f,  1.0f, 0.0f,
	0.0f,  0.0f, 1.0f,

	.0f, 1.0f,
	.0f, .0f,
	1.0f, .0f,
};

namespace {
	const unsigned int CUBE_DATA_SIZE = 6 * 6 * 8;
	GLfloat cubeData[CUBE_DATA_SIZE] = {
		// Position		UVs			// Normals
		// Front
		-.5f, -.5f, .5f, .0f, .0f,	 .0f, .0f, 1.0f, // Front Bot Left 
		-.5f,  .5f, .5f, .0f, 1.0f,	 .0f, .0f, 1.0f,// Front Top Left
		.5f,  .5f, .5f, 1.0f, 1.0f, .0f, .0f, 1.0f,// Front Top Right

		-.5f, -.5f, .5f, .0f, .0f,	 .0f, .0f, 1.0f, // Front Bot Left 
		.5f, -.5f, .5f, 1.0f, .0f,	 .0f, .0f, 1.0f, // Front Bot Right
		.5f,  .5f, .5f, 1.0f, 1.0f,  .0f, .0f, 1.0f,  // Front Top Right

		// Back
		-.5f, -.5f, -.5f,   .0f,  .0f,  .0f, .0f, -1.0f, // Back Bot Left
		-.5f,  .5f, -.5f,   .0f, 1.0f,  .0f, .0f, -1.0f, // Back Top Left
		.5f,  .5f, -.5f,   1.0f, 1.0f,  .0f, .0f, -1.0f, // Back Top Right

		-.5f, -.5f, -.5f,  .0f,  .0f,   .0f, .0f, -1.0f, // Back Bot Left
		.5f, -.5f, -.5f,  1.0f,  .0f,  .0f, .0f, -1.0f, // Back Bot Right
		.5f,  .5f, -.5f,  1.0f, 1.0f, .0f, .0f, -1.0f, // Back Top Right

		// Left
		-.5f, -.5f, .5f,   .0f,  .0f,	 -1.0f, .0f, .0f, // Front Bot Left 
		-.5f,  .5f, .5f,   .0f, 1.0f,	 -1.0f, .0f, .0f,// Front Top Left
		-.5f,  .5f, -.5f, 1.0f, 1.0f,  -1.0f, .0f, .0f, // Back Top Left

		-.5f, -.5f, .5f,   .0f,  .0f,	 -1.0f, .0f, .0f, // Front Bot Left 
		-.5f, -.5f, -.5f, 1.0f,  .0f,   -1.0f, .0f, .0f, // Back Bot Left
		-.5f,  .5f, -.5f, 1.0f, 1.0f,  -1.0f, .0f, .0f, // Back Top Left

		// Right
		.5f, -.5f, .5f,   .0f,  .0f,	 1.0f, .0f, .0f, // Front Bot Right
		.5f,  .5f, .5f,   .0f, 1.0f,  1.0f, .0f, .0f,// Front Top Right
		.5f,  .5f, -.5f, 1.0f, 1.0f, 1.0f, .0f, .0f, // Back Top Right

		.5f, -.5f, .5f,   .0f,  .0f,	 1.0f, .0f, .0f, // Front Bot Right
		.5f, -.5f, -.5f, 1.0f,  .0f,  1.0f, .0f, .0f, // Back Bot Right
		.5f,  .5f, -.5f, 1.0f, 1.0f, 1.0f, .0f, .0f, // Back Top Right

		// Top
		-.5f,  .5f, .5f,  .0f, .0f,  .0f, 1.0f, .0f,// Front Top Left
		-.5f,  .5f, -.5f, .0f, 1.0f, .0f, 1.0f, .0f, // Back Top Left
		.5f,  .5f, -.5f,  1.0f, 1.0f,  .0f, 1.0f, .0f, // Back Top Right

		-.5f,  .5f, .5f,   .0f,  .0f,  .0f, 1.0f, .0f,// Front Top Left
		.5f,  .5f, .5f,   1.0f,  .0f,  .0f, 1.0f, .0f,// Front Top Right
		.5f,  .5f, -.5f,  1.0f, 1.0f,  .0f, 1.0f, .0f, // Back Top Right
		
		// Bot
		-.5f, -.5f, .5f,   .0f,  .0f,	 .0f, -1.0f, .0f, // Front Bot Left 
		-.5f, -.5f, -.5f,  .0f, 1.0f,  .0f, -1.0f, .0f, // Back Bot Left
		.5f, -.5f, -.5f,  1.0f, 1.0f,  .0f, -1.0f, .0f,  // Back Bot Right

		-.5f, -.5f, .5f,   .0f,  .0f,	 .0f, -1.0f, .0f, // Front Bot Left 
		.5f, -.5f, .5f,   1.0f,  .0f,	 .0f, -1.0f, .0f, // Front Bot Right
		.5f, -.5f, -.5f,  1.0f, 1.0f,  .0f, -1.0f, .0f,  // Back Bot Right
	};

	const unsigned int POINTS_FOR_INDEXING = 36;
}

bool Scene04::Initialize()
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
		glfwSetInputMode(m_rend->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_input->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X, 0);
		m_input->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y, 0);
	}

	// Create Program
	{
		m_shader.CompileShader("../Resources/Shaders/default3DVert.glsl", GL_VERTEX_SHADER);
		m_shader.CompileShader("../Resources/Shaders/default3DFrag.glsl", GL_FRAGMENT_SHADER);
		m_shader.Link();
		m_shader.Use();

		//m_programID = m_rend->CreateShaderProgram("../Resources/Shaders/default3DVert.glsl", "../Resources/Shaders/default3DFrag.glsl");
	}

	// Set Camera
	{
		float camYaw = 3.14f / 2;
		float camPitch = 0;
		auto x = glm::cos(camYaw);
		auto y = glm::cos(camPitch);
		glm::vec3 camDirection = {y * x, glm::sin(camPitch), y * glm::sin(-camYaw)};

		camPos = glm::vec3(.0f, .0f, 1.5f);
		target = camPos + camDirection;

		camData.distance = 1.0f;
		camData.fov = 3.14f / 3.0f;
		camData.nearClip = .01f;
		camData.farClip = 100.0f;
		camData.pitchMin = -3.14f / 2.0f;
		camData.pitchMax = 3.14f / 2.0f;
		camData.type = Camera::eType::ORBIT;
		m_camera.Initialize(camPos, target, camData);

		AddObject(&m_camera);
	}

	// Set Matrix
	{
		//m_projectionMatrix = glm::perspective (3.14f / 3.0f, (float)m_rend->m_width / m_rend->m_height, .01f, 100.0f);
		//m_viewMatrix = glm::lookAt (m_camPos, m_camPos + m_camDirection, {.0f, 1.0f, .0f});

		m_projectionMatrix = m_camera.GetProjection();
		//m_viewMatrix = m_camera.GetView();
		m_viewMatrix = glm::lookAt(glm::vec3(.0f, 1.5f, -5.0f), glm::vec3(.0f, .0f, .0f), glm::vec3(.0f, 1.0f, .0f));
		m_modelMatrix = glm::mat4(1.0f);
	}

	// VAO
	{
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		GLuint vboID;
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> norm;
		std::vector<glm::vec2> uv;
		
		MeshLoader::ParseModel(L"../Resources/Meshes/Dargon.obj", pos, uv, norm);
		
		GLintptr posSize = pos.size() * sizeof(glm::vec3);
		GLintptr uvSize = uv.size() * sizeof(glm::vec2);
		GLintptr normSize = norm.size() * sizeof(glm::vec3);
		GLsizeiptr size = posSize + uvSize + normSize;
		vertices = pos.size();

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, posSize, &pos[0]);
		if (normSize > 0)
		{
			glBufferSubData(GL_ARRAY_BUFFER, posSize, normSize, &norm[0]);
		}
		if (uvSize > 0)
		{
			glBufferSubData(GL_ARRAY_BUFFER, posSize + normSize, uvSize, &uv[0]);
		}

		// Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)posSize);

		// UVs
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(posSize + normSize));

		glBindVertexArray(0);
		glDeleteBuffers(1, &vboID);
	}

	// Materials / Textures
	{
		m_material.SetMaterial( {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f},
			32.0f);
		m_material.LoadTexture2D("../Resources/Textures/DargonSkin.bmp", GL_TEXTURE0);
		m_material.LoadTexture2D("../Resources/Textures/crate_specular2.bmp", GL_TEXTURE1);
	}

	// Light 
	{
		//m_light.ambient = {1.0f, 1.0f, 1.0f};
		m_light.diffuse = {1.0f, 1.0f, 1.0f};
		m_light.specular = {1.0f, 1.0f, 1.0f};
		m_light.transform.position = {.0f, 5.0f, .0f};

		AddObject(&m_light);
	}
	return true;
}
void Scene04::Update()
{
	float dt = m_timer->GetDeltaTime();

	int length = m_objects.size();
	for (int i = 0; i < length; i++)
	{
		auto obj = m_objects[i];
		obj->Update();
	}

	auto exitBut = m_input->GetButton("Exit");
	if ( exitBut != Input::eButtonState::UP) 
	{
		glfwSetWindowShouldClose(m_rend->m_window, true);
	}

	//static float rotateAmo = .0f;
	//rotateAmo += 3.14f * dt;
	//m_modelMatrix = glm::rotate (glm::mat4 (1.0f), rotateAmo, glm::vec3 (.0f, 1.0f, .0f));

	// Key Input
	auto direction = m_camera.GetDirection();
	auto wBut = m_input->GetButton("W");
	auto aBut = m_input->GetButton("A");
	auto sBut = m_input->GetButton("S");
	auto dBut = m_input->GetButton("D");
	auto spaceBut = m_input->GetButton (" ");
	auto lShiftBut = m_input->GetButton ("LShift");

	glm::vec3 mvmt(.0f);
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
	

	// Mouse Input
	glm::vec3 rotate(.0f);

	static float currMouseX = m_input->GetAnalogAbsolute("x-axis");
	static float currMouseY = m_input->GetAnalogAbsolute("y-axis");
	float tempMouseX = m_input->GetAnalogAbsolute("x-axis");
	float tempMouseY = m_input->GetAnalogAbsolute("y-axis");
	rotate.x = (currMouseY - tempMouseY) / 200.0f;
	rotate.y = (currMouseX - tempMouseX) / 400.0f;

	currMouseX = tempMouseX;
	currMouseY = tempMouseY;

	//m_camera.UpdateTransformEditor(mvmt, rotate);
	camPos += mvmt;
	m_viewMatrix = glm::lookAt(camPos, camPos + direction, glm::vec3(.0f, 1.0f, .0f));
}
void Scene04::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind
	glBindVertexArray(m_vaoID);
	m_shader.Use();

	// Bind Uniforms
	{
		m_material.SetTextures();
		m_shader.SetUniform("texture1", 0);
		m_shader.SetUniform("texture2", 1);

		//m_shader.SetUniform("material.ambience", m_material.m_ambience);
		m_shader.SetUniform("material.brightness", m_material.m_shininess);
		m_shader.SetUniform("material.ambient", m_material.m_ambient);
		m_shader.SetUniform("material.diffuse", m_material.m_diffuse);
		m_shader.SetUniform("material.specular",m_material.m_specular);

		//m_shader.SetUniform("light.ambient", m_light.ambient);
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

	glfwSwapBuffers(m_rend->m_window);
}
void Scene04::Shutdown()
{
	glDeleteVertexArrays(1, &m_vaoID);
}