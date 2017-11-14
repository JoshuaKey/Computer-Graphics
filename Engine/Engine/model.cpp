//• m_shader.Use();
//m_material.SetTextures();
//m_mesh.Render();
#include "stdafx.h"

#include "model.h"

#include "mesh.h"
#include "camera.h"
#include "scene.h"
#include "light.h"

Model::Model(const char* name, Scene* scene) : Renderable(name, scene) {}
Model::~Model() {}

void Model::Update()
{

	Camera* camera = m_scene->GetObject<Camera>("camera");
	Light* light = m_scene->GetObject<Light>("light");

	m_shader.Use();

	auto val = transform.GetMatrix();
	m_shader.SetUniform("model", val);
	val = camera->GetView();
	m_shader.SetUniform("view", val);
	val = camera->GetProjection();
	m_shader.SetUniform("projection", val);

	//m_shader.SetUniform("light.position", light->transform.position);

	m_shader.SetUniform("viewPos", camera->transform.position);
}

void Model::Render()
{
	m_shader.Use();
	m_material.SetTextures();
	m_mesh.Render();
}