#include "stdafx.h"

#include "scene02.h"

#include "renderer.h"
#include "input.h"

#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>

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

bool Scene02::Initialize() {
	m_input = m_engine->Get<Input>();
	m_rend = m_engine->Get<Renderer>();

	glfwSetInputMode(m_rend->m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	m_input->AddButton("W", Input::eButtonType::KEYBOARD, 'W');
	m_input->AddButton("S", Input::eButtonType::KEYBOARD, 'S');
	m_input->AddButton("A", Input::eButtonType::KEYBOARD, 'A');
	m_input->AddButton("D", Input::eButtonType::KEYBOARD, 'D');
	m_input->AddButton("Exit", Input::eButtonType::KEYBOARD, GLFW_KEY_ESCAPE);
	m_input->AddButton("Click", Input::eButtonType::MOUSE, 0);
	m_input->AddAnalog("x-axis", Input::eAnalogType::MOUSE_X, 0);
	m_input->AddAnalog("y-axis", Input::eAnalogType::MOUSE_Y, 0);
	
	m_programID = m_rend->CreateShaderProgram("../Resources/Shaders/cursor.vs", "../Resources/Shaders/default.fs");

	m_cursorModelUniform = glGetUniformLocation(m_programID, "model");
	m_cursorModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.0f));

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

void Scene02::Update() {
	auto exitBut = m_input->GetButton("Exit");
	if ( exitBut != Input::eButtonState::UP) {
		glfwSetWindowShouldClose(m_rend->m_window, true);
	}


	//m_input->GetButton()
	m_mouseX = m_input->GetAnalogAbsolute("x-axis");
	m_mouseY = m_input->GetAnalogAbsolute("y-axis");

	float clipX = (static_cast<float>(m_mouseX) / m_rend->m_width) * 2.0f - 1.0f;
	float clipY = -(static_cast<float>(m_mouseY) / m_rend->m_height) * 2.0f + 1.0f;

	m_cursorModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(clipX, clipY, .0f));
	m_cursorModelMatrix = glm::rotate(m_cursorModelMatrix, 3.14f / 18.0f, glm::vec3(.0f, .0f, 1.0f));
	m_cursorModelMatrix = glm::scale(m_cursorModelMatrix, glm::vec3(m_cursorSize * .03f, m_cursorSize * .06f, .0f));
	// Cursor is affected by size of viewport...
}

void Scene02::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind
	glUseProgram(m_programID);
	glBindVertexArray(m_vaoID);

	// Bind Uniforms
	glUniformMatrix4fv(m_cursorModelUniform, 1, GL_FALSE, &m_cursorModelMatrix[0][0]);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(m_rend->m_window);
}

void Scene02::Shutdown() {
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteProgram(m_programID);
}