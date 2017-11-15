#include "stdafx.h"

#include "directionalLight.h"

#include "scene.h"
#include "camera.h"


void DirectionalLight::Update()
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

void DirectionalLight::SetUniform(int id, Shader &s)
{
	char uniformName[32];
	s.Use();

	sprintf_s(uniformName, 32, "lights[%d].diffuse", id);
	s.SetUniform(uniformName, diffuse);

	sprintf_s(uniformName, 32, "lights[%d].specular", id);
	s.SetUniform(uniformName, specular);

	sprintf_s(uniformName, 32, "lights[%d].position", id);
	auto temp = glm::vec4(transform.rotation * glm::vec3(.0f, .0f, 3.14f / 2.0f), 0.0f);
	temp = glm::normalize(temp);
	s.SetUniform(uniformName, temp);

	sprintf_s(uniformName, 32, "lights[%d].ID", id);
	s.SetUniform(uniformName, 1);
}

void DirectionalLight::Render()
{
	//if(!m_shader.IsLinked || m_mesh.)

	//m_shader.Use();
	//m_mesh.Render();
}
