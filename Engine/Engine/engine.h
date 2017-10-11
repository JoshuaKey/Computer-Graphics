#pragma once

class System;

class Engine
{
public:
	Engine();
	~Engine();

	bool Initialize();
	void Shutdown();
	void Update();

	template <typename T> 
	T* Get()
	{
		for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter)
		{
			if (dynamic_cast<T*>(iter->get()) != nullptr)
			{
				return static_cast<T*>(iter->get());
			}
		}

		return nullptr;
	}

private:
	std::vector<std::shared_ptr<System>> m_systems;
};

