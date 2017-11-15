#include "stdafx.h"

#include "spotlight.h"

#include "camera.h"
#include "scene.h"

void SpotLight::Update()
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

void SpotLight::SetUniform(int id, Shader &s)
{
	char uniformName[32];
	s.Use();

	sprintf_s(uniformName, 32, "lights[%d].diffuse", id);
	s.SetUniform(uniformName, diffuse);

	sprintf_s(uniformName, 32, "lights[%d].specular", id);
	s.SetUniform(uniformName, specular);

	sprintf_s(uniformName, 32, "lights[%d].position", id);
	s.SetUniform(uniformName, glm::vec4(transform.position, 1.0f));

	sprintf_s(uniformName, 32, "lights[%d].direction", id);
	s.SetUniform(uniformName, transform.rotation * glm::vec3(.0f, .0f, -3.14f/2.0f));

	sprintf_s(uniformName, 32, "lights[%d].exponent", id);
	s.SetUniform(uniformName, exponent);

	sprintf_s(uniformName, 32, "lights[%d].cutoff", id);
	s.SetUniform(uniformName, cutoff);

	sprintf_s(uniformName, 32, "lights[%d].ID", id);
	s.SetUniform(uniformName, 2);
}

void SpotLight::Render()
{
	m_shader.Use();
	m_mesh.Render();
}

