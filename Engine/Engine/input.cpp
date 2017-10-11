#include "stdafx.h"

#include "input.h"
#include "renderer.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

Input::Input(Engine* engine) : System(engine)
{
	//
}

Input::~Input()
{
}

bool Input::Initialize()
{
	return true;
}

void Input::Shutdown()
{
	m_buttons.clear();
	m_analogs.clear();
}

void Input::Update()
{
	// update buttons
	for (auto iter = m_buttons.begin(); iter != m_buttons.end(); ++iter)
	{
		ButtonInfo info = iter->second;

		int state = 0;
		if (info.type == eButtonType::KEYBOARD)
		{
			state = glfwGetKey(m_engine->Get<Renderer>()->m_window, info.id);
		}
		else if (info.type == eButtonType::MOUSE)
		{
			state = glfwGetMouseButton(m_engine->Get<Renderer>()->m_window, info.id);
		}
		else if (info.type == eButtonType::JOYSTICK)
		{
			int count;
			const unsigned char* states = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
			if (info.id < count)
			{
				state = states[info.id];
			}
		}

		switch (info.state)
		{
		case eButtonState::UP:
			info.state = (state == GLFW_PRESS) ? eButtonState::DOWN : eButtonState::UP;
			break;

		case eButtonState::DOWN:
			info.state = (state == GLFW_PRESS) ? eButtonState::HELD : eButtonState::RELEASED;
			break;

		case eButtonState::HELD:
			info.state = (state == GLFW_PRESS) ? eButtonState::HELD : eButtonState::RELEASED;
			break;

		case eButtonState::RELEASED:
			info.state = (state == GLFW_PRESS) ? eButtonState::DOWN : eButtonState::UP;
			break;
		}

		iter->second = info;
	}

	// update analog
	for (auto iter = m_analogs.begin(); iter != m_analogs.end(); ++iter)
	{
		AnalogInfo info = iter->second;

		float x = 0;
		float y = 0;

		if (info.type == eAnalogType::JOYSTICK_X || info.type == eAnalogType::JOYSTICK_Y)
		{
			int count;
			const float* states = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
			if (count >= 2)
			{
				x = states[0];
				y = states[1];
			}
		}
		else if (info.type == eAnalogType::MOUSE_X || info.type == eAnalogType::MOUSE_Y)
		{
			double xd;
			double yd;
			glfwGetCursorPos(m_engine->Get<Renderer>()->m_window, &xd, &yd);

			x = (float)xd;
			y = (float)yd;
		}
				
		switch (info.type)
		{
		case eAnalogType::JOYSTICK_X:
			info.valueAbsolute = x;
			break;
		case eAnalogType::JOYSTICK_Y:
			info.valueAbsolute = y;
			break;
		case eAnalogType::MOUSE_X:
			info.valueRelative = x - info.valueAbsolute;
			info.valueAbsolute = x;
			break;
		case eAnalogType::MOUSE_Y:
			info.valueRelative = y - info.valueAbsolute;
			info.valueAbsolute = y;
			break;
		}

		iter->second = info;
	}
}

void Input::AddButton(const std::string name, eButtonType type, int id)
{
	std::map<std::string, ButtonInfo>::iterator iter = m_buttons.find(name);
	if (iter == m_buttons.end())
	{
		ButtonInfo info = { id, type, eButtonState::UP };
		m_buttons[name] = info;
	}
}

Input::eButtonState Input::GetButton(const std::string name)
{
	eButtonState buttonState = eButtonState::UP;

	std::map<std::string, ButtonInfo>::iterator iter = m_buttons.find(name);
	if (iter != m_buttons.end())
	{
		buttonState = iter->second.state;
	}

	return buttonState;
}

void Input::AddAnalog(const std::string name, eAnalogType type, int id)
{
	std::map<std::string, AnalogInfo>::iterator iter = m_analogs.find(name);
	if (iter == m_analogs.end())
	{
		AnalogInfo info = { id, type, 0.0f };
		m_analogs[name] = info;
	}
}

float Input::GetAnalogAbsolute(const std::string name)
{
	float value = 0.0f;

	std::map<std::string, AnalogInfo>::iterator iter = m_analogs.find(name);
	if (iter != m_analogs.end())
	{
		value = iter->second.valueAbsolute;
	}

	return value;
}

float Input::GetAnalogRelative(const std::string name)
{
	float value = 0.0f;

	std::map<std::string, AnalogInfo>::iterator iter = m_analogs.find(name);
	if (iter != m_analogs.end())
	{
		value = iter->second.valueRelative;
	}

	return value;
}