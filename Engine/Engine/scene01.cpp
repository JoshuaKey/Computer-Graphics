#include "stdafx.h"

#include "scene01.h"

#include "renderer.h"
#include <glm\vec3.hpp>

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

bool Scene01::Initialize() {
	m_rend = m_engine->Get<Renderer>();
	m_programID = m_rend->CreateShaderProgram("../Resources/Shaders/default.vs", "../Resources/Shaders/default.fs");

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

	glBindVertexArray(0);
	glDeleteBuffers(1, &vboID);

	return true;
}

void Scene01::Update() {

}
void Scene01::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind
	glUseProgram(m_programID);
	glBindVertexArray(m_vaoID);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(m_rend->m_window);
}

void Scene01::Shutdown() {

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vaoID);
}