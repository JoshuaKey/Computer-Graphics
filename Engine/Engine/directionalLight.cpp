#include "stdafx.h"
#include "directionalLight.h"

#include "scene.h"
#include "renderable.h"

void DirectionalLight::Initialize()
{
	auto renderables = m_scene->GetObjects<Renderable>();
	auto length = renderables.size();
	for (int i = 0; i < length; i++)
	{
		renderables[i]->m_shader.SetUniform("light.position", glm::vec4(direction, .0f));
	}
}

void DirectionalLight::Update(){}
