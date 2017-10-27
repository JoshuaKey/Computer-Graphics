#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

class Shader
{
public:
	Shader();
	~Shader();

public:


	void CompileShader(const std::string& shader, GLenum shaderType);

	void Link();
	void Use();

	void SetUniform(const std::string& uniformName, float x, float y, float z);
	void SetUniform(const std::string& uniformName, const glm::vec2& v2);
	void SetUniform(const std::string& uniformName, const glm::vec3& v3);
	void SetUniform(const std::string& uniformName, const glm::vec4& v4);
	void SetUniform(const std::string& uniformName, const glm::mat4& mx4);
	void SetUniform(const std::string& uniformName, const glm::mat3& mx3);
	void SetUniform(const std::string& uniformName, float vfloat);
	void SetUniform(const std::string& uniformName, int vint);
	void SetUniform(const std::string& uniformName, bool vbool);
	void SetUniform(const std::string& uniformName, GLuint vuint);
	
	void PrintActiveUniforms();
	void PrintActiveAttribs();

	GLuint GetHandle() { return m_program; }
	bool IsLinked() { return m_isLinked; }

private:
	GLuint CreateShader(const std::string& shaderFilename, GLenum shaderType);
	std::string ReadFile(const std::string& filename);

	GLint GetUniformLocation(const std::string& uniformName);

private:
	GLuint m_program = 0;
	bool m_isLinked = false;
	std::map<std::string, GLuint> m_uniformLocations;
};
