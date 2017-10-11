#include "stdafx.h"
#include "timer.h"

Timer::Timer(Engine* engine) : System(engine)
{
	m_dt = 0.0f;
	m_fps = 0.0f;
	m_timeStamp = 0.0f;
	m_frameCount = 0;
}

Timer::~Timer()
{
	//
}

bool Timer::Initialize()
{
	m_timePrev = std::chrono::high_resolution_clock::now();

	return true;
}

void Timer::Shutdown()
{
	//
}

void Timer::Update()
{
	auto time(std::chrono::high_resolution_clock::now());
	auto elapsedTime(time - m_timePrev);
	float milliseconds = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count();

	m_dt = milliseconds / 1000.0f;

	m_frameCount++;
	m_timeStamp = m_timeStamp + m_dt;
	if (m_timeStamp >= 1.0f)
	{
		m_fps = (float)m_frameCount;
		m_mps = 1000.0f / m_frameCount;
		m_frameCount = 0;
		m_timeStamp = 0;
	}

 	m_timePrev = time;
}