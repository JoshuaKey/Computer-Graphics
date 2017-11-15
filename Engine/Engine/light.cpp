#include "stdafx.h"

#include "light.h"
#include "camera.h"
#include "scene.h"

void Light::Update()
{
	Camera* camera = m_scene->GetObject<Camera>("camera");

	m_shader.Use();

	auto val = transform.GetMatrix();
	m_shader.SetUniform("model", val);
	val = camera->GetView();
	m_shader.SetUniform("view", val);
	val = camera->GetProjection();
	m_shader.SetUniform("projection", val);
}

void Light::SetUniform(int id, Shader &s)
{
	char uniformName[32];
	s.Use();

	sprintf_s(uniformName, 32, "lights[%d].diffuse", id);
	s.SetUniform(uniformName, diffuse);

	sprintf_s(uniformName, 32, "lights[%d].specular", id);
	s.SetUniform(uniformName, specular);

	sprintf_s(uniformName, 32, "lights[%d].position", id);
	s.SetUniform(uniformName, glm::vec4(transform.position, 1.0f));

	sprintf_s(uniformName, 32, "lights[%d].ID", id);
	s.SetUniform(uniformName, 1);
}

void Light::Render()
{
	//if(!m_shader.IsLinked || m_mesh.)
	
	m_shader.Use();
	m_mesh.Render();
}