#include "stdafx.h"

#include "engine.h"
#include "renderer.h"
#include "scene01.h"
#include "scene02.h"

#include <glm/vec3.hpp>

float positionData[] =
{
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f,  0.8f, 0.0f
};

float colorData[] =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

glm::vec3 vPositionData[] =
{
	glm::vec3(-0.8f, -0.8f, 0.0f),
	glm::vec3(0.8f, -0.8f, 0.0f),
	glm::vec3(0.0f,  0.8f, 0.0f),
};

glm::vec3 vColorData[] =
{
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
};

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

enum vboID
{
	POSITION,
	COLOR,
	VERTEX
};

int main() {
	std::shared_ptr<Engine> engine(new Engine);
	if (!engine->Initialize())
	{
		engine->Shutdown();
		exit(EXIT_FAILURE);
	}

	//Should we make it a shared pointer?
	Scene* mainScene = new Scene02(engine.get());
	if(!mainScene->Initialize()) {
		mainScene->Shutdown();
		exit(EXIT_FAILURE);
	}

	Renderer* rend = engine->Get<Renderer>();

	//GLuint programID = rend->CreateShaderProgram("../Resources/Shaders/default.vs", "../Resources/Shaders/default.fs");
	//glUseProgram(programID);

	////Gen VAO
	//GLuint vaoHandle;
	//glGenVertexArrays(1, &vaoHandle);
	//glBindVertexArray(vaoHandle);

	//GLuint vboID;
	////GLuint eboID;
	//glGenBuffers(1, &vboID);
	////glGenBuffers(1, &eboID);
	//glBindBuffer(GL_ARRAY_BUFFER, vboID);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof())

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	//GLuint vboHandles[3];
	//glGenBuffers(3, vboHandles);
	//glBindVertexBuffer(0, vboHandles[POSITION], 0, sizeof(glm::vec3));
	//glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0); Sets the Format of the Attribute. Should be done for Color and Pos
	//glVertexAttribBinding(0, 0); -> Bind Position Buffer to Specific Color Buffe

	while (!glfwWindowShouldClose(rend->m_window))
	{
		engine->Update();
		mainScene->Update();

		mainScene->Render();

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// render code
		//glBindVertexArray(vaoHandle);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
		//// render code

		//glfwSwapBuffers(rend->m_window);
	}
	
	mainScene->Shutdown();
	engine->Shutdown();
}