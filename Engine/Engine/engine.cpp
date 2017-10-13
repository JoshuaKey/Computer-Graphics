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
	// Create all systems, pass in Engine
	std::shared_ptr<Timer> timer(new Timer(this));
	m_systems.push_back(timer);

	std::shared_ptr<Renderer> renderer(new Renderer(this));
	m_systems.push_back(renderer);

	std::shared_ptr<Input> input(new Input(this));
	m_systems.push_back(input);

	// Loop and initialiaze them
	for ( auto iter = m_systems.begin(); iter != m_systems.end(); ++iter ) {
		if ( iter->get()->Initialize() == false ) {
			std::cerr << "Error initializing engine.\n";
			return false;
		}
	}
	return true;
}

void Engine::Shutdown()
{
	// Loop and shut them down
	for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter)
	{
		iter->get()->Shutdown();
	}
}

void Engine::Update()
{
	// Ask GLFW to process events (keys, mouse clicks, etc.)
	glfwPollEvents();

	// Loop and update them all
	for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter)
	{
		iter->get()->Update();
	}
}