#include "stdafx.h"

#include "scene03.h"

#include "renderer.h"
#include "input.h"
#include "timer.h"

#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>

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
		-.5f, -.5f, -.5f, 1.0f,  .0f,  .0f, .0f, -1.0f, // Back Bot Left
		-.5f,  .5f, -.5f, 1.0f, 1.0f,  .0f, .0f, -1.0f, // Back Top Left
		.5f,  .5f, -.5f,   .5f,  .5f,  .0f, .0f, -1.0f, // Back Top Right

		-.5f, -.5f, -.5f, 1.0f, .0f,   .0f, .0f, -1.0f, // Back Bot Left
		.5f, -.5f, -.5f,  .5f, .0f,  .0f, .0f, -1.0f, // Back Bot Right
		.5f,  .5f, -.5f,  .5f, .5f, .0f, .0f, -1.0f, // Back Top Right

		// Left
		-.5f, -.5f, .5f, .25f, .0f,	 -1.0f, .0f, .0f, // Front Bot Left 
		-.5f,  .5f, .5f, .0f, .25f,	 -1.0f, .0f, .0f,// Front Top Left
		-.5f,  .5f, -.5f, .3f, .7f,  -1.0f, .0f, .0f, // Back Top Left

		-.5f, -.5f, .5f, .25f, .0f,	 -1.0f, .0f, .0f, // Front Bot Left 
		-.5f, -.5f, -.5f, .25f, .25f,   -1.0f, .0f, .0f, // Back Bot Left
		-.5f,  .5f, -.5f, .3f, .7f,  -1.0f, .0f, .0f, // Back Top Left

		// Right
		.5f, -.5f, .5f,  .1f, .9f,	 1.0f, .0f, .0f, // Front Bot Right
		.5f,  .5f, .5f,  .9f, .1f,  1.0f, .0f, .0f,// Front Top Right
		.5f,  .5f, -.5f, .3f, .7f, 1.0f, .0f, .0f, // Back Top Right

		.5f, -.5f, .5f,  .1f, .9f,	 1.0f, .0f, .0f, // Front Bot Right
		.5f, -.5f, -.5f, .7f, .3f,  1.0f, .0f, .0f, // Back Bot Right
		.5f,  .5f, -.5f, .3f, .7f, 1.0f, .0f, .0f, // Back Top Right

		// Top
		-.5f,  .5f, .5f,  .64f, .21f,  .0f, 1.0f, .0f,// Front Top Left
		-.5f,  .5f, -.5f, .19f, .23f, .0f, 1.0f, .0f, // Back Top Left
		.5f,  .5f, -.5f,  .69f, .55f,  .0f, 1.0f, .0f, // Back Top Right

		-.5f,  .5f, .5f,  .64f, .21f,  .0f, 1.0f, .0f,// Front Top Left
		.5f,  .5f, .5f,   .45f, .78f,  .0f, 1.0f, .0f,// Front Top Right
		.5f,  .5f, -.5f,  .69f, .55f,  .0f, 1.0f, .0f, // Back Top Right
		
		// Bot
		-.5f, -.5f, .5f,  .3f, .2f,	 .0f, -1.0f, .0f, // Front Bot Left 
		-.5f, -.5f, -.5f, .1f, .0f,  .0f, -1.0f, .0f, // Back Bot Left
		.5f, -.5f, -.5f,  .7f, .6f,  .0f, -1.0f, .0f,  // Back Bot Right

		-.5f, -.5f, .5f,  .3f, .2f,	 .0f, -1.0f, .0f, // Front Bot Left 
		.5f, -.5f, .5f,   .9f, .8f,	 .0f, -1.0f, .0f, // Front Bot Right
		.5f, -.5f, -.5f,  .7f, .6f,  .0f, -1.0f, .0f,  // Back Bot Right
	};

	const unsigned int POINTS_FOR_INDEXING = 36;
}

bool Scene03::Initialize() 
{
	m_input = m_engine->Get<Input>();
	m_rend = m_engine->Get<Renderer>();
	m_timer = m_engine->Get<Timer>();

	// Keyboard Input
	m_input->AddButton("W", Input::eButtonType::KEYBOARD, 'W');
	m_input->AddButton("S", Input::eButtonType::KEYBOARD, 'S');
	m_input->AddButton("A", Input::eButtonType::KEYBOARD, 'A');
	m_input->AddButton("D", Input::eButtonType::KEYBOARD, 'D');
	m_input->AddButton(" ", Input::eButtonType::KEYBOARD, GLFW_KEY_SPACE);
	m_input->AddButton("LShift", Input::eButtonType::KEYBOARD, GLFW_KEY_LEFT_SHIFT);
	m_input->AddButton("Exit", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);

	// Mouse Input
	glfwSetInputMode(m_rend->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_input->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X, 0);
	m_input->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y, 0);

	// Create Program
	m_programID = m_rend->CreateShaderProgram("../Resources/Shaders/default3DVert.glsl", "../Resources/Shaders/default3DFrag.glsl");

	// Get uniforms
	m_modelUniform = glGetUniformLocation(m_programID, "model");
	m_viewUniform = glGetUniformLocation(m_programID, "view");
	m_projectionUniform = glGetUniformLocation(m_programID, "projection");

	m_ambienceUniform =  glGetUniformLocation(m_programID, "ambience");
	m_lightColorUniform =  glGetUniformLocation(m_programID, "lightColor");
	m_lightPosUniform =  glGetUniformLocation(m_programID, "lightPos");
	m_viewPosUniform =  glGetUniformLocation(m_programID, "viewPos");

	// Set inital MAtrix
	m_camYaw = 3.14f / 2;
	m_camPitch = 0;
	auto x = glm::cos(m_camYaw);
	auto y = glm::cos(m_camPitch);
	m_camDirection = { y * x, glm::sin(m_camPitch), y * glm::sin(-m_camYaw) };
	m_camPos = glm::vec3 (.0f, .0f, 1.5f);

	m_projectionMatrix = glm::perspective (3.14f / 3.0f, (float)m_rend->m_width / m_rend->m_height, .01f, 100.0f);
	m_viewMatrix = glm::lookAt (m_camPos, m_camPos + m_camDirection, {.0f, 1.0f, .0f});
	//m_viewMatrix = glm::lookAt (glm::vec3(.0f, 1.0f, 1.5f), {.0f, .0f, .0f}, {.0f, 1.0f, .0f});
	//m_projectionMatrix = glm::mat4 (1.0f);
	//m_viewMatrix = glm::mat4 (1.0f);
	m_modelMatrix = glm::mat4(1.0f);

	// Create VAOs
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	glBindVertexArray (0);
	glDeleteBuffers (1, &vboID);
	//glDeleteBuffers (1, &eboID);

	return true;
}
void Scene03::Update ()
{
	float dt = m_timer->GetDeltaTime();

	auto exitBut = m_input->GetButton("Exit");
	if ( exitBut != Input::eButtonState::UP) 
	{
		glfwSetWindowShouldClose(m_rend->m_window, true);
	}

	static float rotateAmo = .0f;
	rotateAmo += 3.14f * dt;
	m_modelMatrix = glm::rotate (glm::mat4 (1.0f), rotateAmo, glm::vec3 (.0f, 1.0f, .0f));

	// Key Input
	auto wBut = m_input->GetButton("W");
	auto aBut = m_input->GetButton("A");
	auto sBut = m_input->GetButton("S");
	auto dBut = m_input->GetButton("D");
	auto spaceBut = m_input->GetButton (" ");
	auto lShiftBut = m_input->GetButton ("LShift");
	if ( wBut == Input::eButtonState::DOWN || wBut == Input::eButtonState::HELD) 
	{
		m_camPos += m_camDirection * dt;
	}
	if ( sBut == Input::eButtonState::DOWN || sBut == Input::eButtonState::HELD) 
	{
		m_camPos -= m_camDirection * dt;
	}
	if ( dBut == Input::eButtonState::DOWN || dBut == Input::eButtonState::HELD) 
	{
		m_camPos += glm::cross(m_camDirection, glm::vec3(.0f, 1.0f, .0f)) * dt;
	}
	if ( aBut == Input::eButtonState::DOWN || aBut == Input::eButtonState::HELD) 
	{
		m_camPos -= glm::cross(m_camDirection, glm::vec3(.0f, 1.0f, .0f)) * dt;
	}
	if ( spaceBut == Input::eButtonState::DOWN || spaceBut == Input::eButtonState::HELD) 
	{
		m_camPos += glm::vec3 (.0f, 1.0f, .0f) * dt;
	}
	if ( lShiftBut == Input::eButtonState::DOWN || lShiftBut == Input::eButtonState::HELD) 
	{
		m_camPos -= glm::vec3 (.0f, 1.0f, .0f) * dt;
	}

	// Mouse Input
	static int currMouseX = m_input->GetAnalogAbsolute("x-axis");
	static int currMouseY = m_input->GetAnalogAbsolute("y-axis");
	int tempMouseX = m_input->GetAnalogAbsolute("x-axis");
	int tempMouseY = m_input->GetAnalogAbsolute("y-axis");
	m_camPitch += (currMouseY - tempMouseY) / 200.0f;
	if(m_camPitch > 3.14f / 2.0f) { m_camPitch =  3.14f / 2.0f; }
	if(m_camPitch < -3.14f / 2.0f) { m_camPitch =  -3.14f / 2.0f; }
	m_camYaw += (currMouseX - tempMouseX) / 400.0f;
	currMouseX = tempMouseX;
	currMouseY = tempMouseY;

	auto x = glm::cos(m_camYaw);
	auto y = glm::cos(m_camPitch);
	m_camDirection = { y * x, glm::sin(m_camPitch), y * glm::sin(-m_camYaw) };


	m_viewMatrix = glm::lookAt (m_camPos, m_camPos + m_camDirection, {.0f, 1.0f, .0f});
}
void Scene03::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind
	glUseProgram(m_programID);
	glBindVertexArray(m_vaoID);

	// Bind Uniforms
	
	static float rot = .0f;
	rot += 3.14f / 4000;

	glm::vec3 lightPos = glm::vec3(.0f, 5.0f, 5.0f);
	glm::mat4 lightModel = glm::rotate (glm::mat4(1.0f), rot, glm::vec3 (1.0f, .0f, .0f));
	lightPos = glm::vec3(lightModel * glm::vec4(lightPos, 1.0f));

	glUniform1f (m_ambienceUniform, .25f);
	glUniform3f (m_lightColorUniform, 1.0f, 1.0f, 1.0f);
	glUniform3f (m_lightPosUniform, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f (m_viewPosUniform, .0f, 2.0f, -2.0f);

	glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, &m_viewMatrix[0][0]);
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, &m_projectionMatrix[0][0]);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(m_rend->m_window);
}
void Scene03::Shutdown()
{
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteProgram(m_programID);
}