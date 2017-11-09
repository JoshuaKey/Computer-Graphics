//• m_shader.Use();
//m_material.SetTextures();
//m_mesh.Render();
#include "stdafx.h"

#include "model.h"

#include "mesh.h"
#include "camera.h"
#include "scene.h"

Model::Model(const char* name, Scene* scene) : Renderable(name, scene) {}
Model::~Model() {}

void Model::Update()
{
	Camera* camera = m_scene->GetObject<Camera>("camera");

	m_shader.SetUniform("model", transform.GetMatrix());
	m_shader.SetUniform("view", camera->GetView());
	m_shader.SetUniform("projection", camera->GetProjection());

	//m_shader.SetUniform("view", glm::lookAt(glm::vec3(.0f, f, -1.0f), glm::vec3(.0f, .0f, .0f), glm::vec3(.0f, 1.0f, .0f)));
	//m_shader.SetUniform("projection", camera->GetProjection());

	m_shader.SetUniform("viewPos", camera->transform.position);
}

void Model::Render()
{
	m_shader.Use();
	m_material.SetTextures();
	m_mesh.Render();
}