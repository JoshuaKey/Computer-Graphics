#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include <map>

class Shader
{
public:
	Shader();
	~Shader();

	void CompileShader(const char* shader, GLenum shaderType);

	void Link();
	void Use();

	void SetUniform(const char* uniformName, float x, float y, float z);
	void SetUniform(const char* uniformName, const glm::vec2& v2);
	void SetUniform(const char* uniformName, const glm::vec3& v3);
	void SetUniform(const char* uniformName, const glm::vec4& v4);
	void SetUniform(const char* uniformName, const glm::mat4& mx4);
	void SetUniform(const char* uniformName, const glm::mat3& mx3);
	void SetUniform(const char* uniformName, float vfloat);
	void SetUniform(const char* uniformName, int vint);
	void SetUniform(const char* uniformName, bool vbool);
	void SetUniform(const char* uniformName, GLuint vuint);
	
	void PrintActiveUniforms();
	void PrintActiveAttribs();

	GLuint GetHandle() { return m_program; }
	bool IsLinked() { return m_isLinked; }

private:
	GLuint CreateShader(const char* shaderFilename, GLenum shaderType);
	std::string ReadFile(const char* filename);

	GLint GetUniformLocation(const char* uniformName);

private:
	GLuint m_program = 0;
	bool m_isLinked = false;
	std::map<std::string, GLuint> m_uniformLocations;
};
