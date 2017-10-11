#pragma once

#include "Engine.h"

class System
{
public:
	System(Engine* engine) : m_engine(engine) {}
	virtual ~System() {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;

protected:
	Engine* m_engine;
};

