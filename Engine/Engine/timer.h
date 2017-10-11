#pragma once

#include "System.h"

class Timer : public System
{
public:
	Timer(Engine* engine);
	~Timer();

	bool Initialize() final;
	void Shutdown() final;
	void Update() final;

	float FrameTime() { return m_dt; }
	float FramesPerSecond() { return m_fps; }
	float MillisecondsPerSecond() { return m_mps; }

private:
	float m_dt;
	float m_fps;
	float m_mps;
	int m_frameCount;

	std::chrono::high_resolution_clock::time_point m_timePrev;
	float m_timeStamp;
};

