#include "stdafx.h"

#include "engine.h"
#include "renderer.h"
#include "scene01.h"
#include "scene02.h"
#include "scene03.h"
#include "scene04.h"
#include "scene06.h"
#include "scene07.h"
#include "scenePractice.h"

#include <glm/vec3.hpp>
#include "image.h"

int main() 
{


	std::shared_ptr<Engine> engine(new Engine);
	if (!engine->Initialize())
	{
		engine->Shutdown();
		exit(EXIT_FAILURE);
	}

	//Should we make it a shared pointer?
	Scene* mainScene = new Scene07(engine.get());
	if(!mainScene->Initialize()) 
	{
		mainScene->Shutdown();
		exit(EXIT_FAILURE);
	}

	Renderer* rend = engine->Get<Renderer>();

	while (!glfwWindowShouldClose(rend->m_window))
	{
		engine->Update();
		mainScene->Update();

		mainScene->Render();
	}
	
	mainScene->Shutdown();
	engine->Shutdown();
}
