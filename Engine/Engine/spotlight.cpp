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

void SpotLight::Render()
{
	m_shader.Use();
	m_mesh.Render();
}

