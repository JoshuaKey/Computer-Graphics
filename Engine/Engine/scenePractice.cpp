#include "stdafx.h"

#include "scenePractice.h"

#include "renderer.h"
#include "input.h"

#include <glm\vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {
	glm::vec3 vertexData[] =
	{
		glm::vec3(-0.8f, -0.8f, 0.0f),
		glm::vec3(1.0f,  0.0f, 0.0f),
		glm::vec3(0.8f, -0.8f, 0.0f),
		glm::vec3(0.0f,  1.0f, 0.0f),
		glm::vec3(0.0f,  0.8f, 0.0f),
		glm::vec3(0.0f,  0.0f, 1.0f),
		glm::vec3(-0.8f, -0.8f, 0.0f),
		glm::vec3(1.0f,  0.0f, 0.0f),
	};
}

bool ScenePractice::Initialize() 
{
	m_input = m_engine->Get<Input>();
	m_rend = m_engine->Get<Renderer>();

	// Keyboard Input
	m_input->AddButton("W", Input::eButtonType::KEYBOARD, 'W');
	m_input->AddButton("S", Input::eButtonType::KEYBOARD, 'S');
	m_input->AddButton("A", Input::eButtonType::KEYBOARD, 'A');
	m_input->AddButton("D", Input::eButtonType::KEYBOARD, 'D');
	m_input->AddButton("Exit", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);

	// Mouse Input
	m_input->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X, 0);
	m_input->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y, 0);

	// Create Program
	m_programID = m_rend->CreateShaderProgram("../Resources/Shaders/default3D.vs", "../Resources/Shaders/default3D.fs");

	// Get uniforms
	m_modelUniform = glGetUniformLocation(m_programID, "model");
	m_viewUniform = glGetUniformLocation(m_programID, "view");
	m_projectionUniform = glGetUniformLocation(m_programID, "projection");

	// Set inital MAtrix
	m_projectionMatrix = glm::perspective (3.14f / 6.0f, 4.0f / 3.0f, .01f, 100.0f);
	m_viewMatrix = glm::lookAt (glm::vec3(.0f, 2.0f, -2.0f), {.0f, .0f, .0f}, {.0f, 1.0f, .0f});
	m_modelMatrix = glm::translate (glm::mat4 (1.0f), glm::vec3 (.0f, .0f, 1.0f));
	
	// Create VAOs
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	GLuint vboID;
	//GLuint eboID;
	glGenBuffers(1, &vboID);
	//glGenBuffers(1, &eboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof())

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	return true;
}
void ScenePractice::Update ()
{
	auto exitBut = m_input->GetButton("Exit");
	if ( exitBut != Input::eButtonState::UP) 
	{
		glfwSetWindowShouldClose(m_rend->m_window, true);
	}
}
void ScenePractice::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind
	glUseProgram(m_programID);
	glBindVertexArray(m_vaoID);

	// Bind Uniforms
	glUniformMatrix4fv(m_modelUniform, 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(m_viewUniform, 1, GL_FALSE, &m_viewMatrix[0][0]);
	glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, &m_projectionMatrix[0][0]);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(m_rend->m_window);
}
void ScenePractice::Shutdown()
{

}