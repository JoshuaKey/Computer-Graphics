#include "stdafx.h"
#include "engine.h"

#include "Renderer.h"
#include "Input.h"
#include "Timer.h"

Engine::Engine()
{
	//
}

Engine::~Engine()
{
	//
}

bool Engine::Initialize()
{
	std::shared_ptr<Timer> timer(new Timer(this));
	m_systems.push_back(timer);

	std::shared_ptr<Renderer> renderer(new Renderer(this));
	m_systems.push_back(renderer);

	std::shared_ptr<Input> input(new Input(this));
	m_systems.push_back(input);

	for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter)
	{
		if (iter->get()->Initialize() == false)
		{
			std::cerr << "Error initializing engine.\n";
			return false;
		}
	}

	return true;
}

void Engine::Shutdown()
{
	for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter)
	{
		iter->get()->Shutdown();
	}
}

void Engine::Update()
{
	for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter)
	{
		iter->get()->Update();
	}

	glfwPollEvents();
}