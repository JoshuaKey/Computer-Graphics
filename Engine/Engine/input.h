#pragma once

#include "system.h"
#include <map>

class Renderer;

class Input : public System
{
public:
	enum class eButtonState
	{
		INVALID,
		UP,
		DOWN,
		HELD,
		RELEASED,
	};

	enum class eButtonType
	{
		INVALID,
		KEYBOARD,
		MOUSE,
		JOYSTICK,
	};

	enum class eAnalogType
	{
		INVALID,
		MOUSE_X,
		MOUSE_Y,
		JOYSTICK_X,
		JOYSTICK_Y,
	};

	typedef struct tButtonInfo
	{
		int id;
		eButtonType type;
		eButtonState state;
	} ButtonInfo;

	typedef struct tAnalogInfo
	{
		int id;
		eAnalogType type;
		float valueAbsolute;
		float valueRelative;
	} AnalogInfo;
	
public:
	Input(Engine* engine);
	~Input();

	bool Initialize();
	void Shutdown();
	void Update();
	
	void AddButton(const std::string name, eButtonType type, int id);
	eButtonState GetButton(const std::string name);

	void AddAnalog(const std::string name, eAnalogType type, int id);
	float GetAnalogAbsolute(const std::string name);
	float GetAnalogRelative(const std::string name);

private:
	std::map<std::string, ButtonInfo> m_buttons;
	std::map<std::string, AnalogInfo> m_analogs;
};

