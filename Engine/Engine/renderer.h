#pragma once

#include "system.h"

class Renderer : public System
{
public:
	Renderer(Engine* engine);
	~Renderer();

	bool Initialize();
	void Shutdown();
	void Update();

	GLuint CreateShaderProgram(const std::string& vertextShader, const std::string& fragmentShader);
	GLuint CreateShader(const std::string& shader, GLenum shaderType);

private:
	std::string ReadFile(const std::string& filename);

public:
	GLFWwindow* m_window;
	static int m_width;
	static int m_height;
};

