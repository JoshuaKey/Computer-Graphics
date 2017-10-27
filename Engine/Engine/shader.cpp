#include "stdafx.h"

#include "shader.h"
#include "renderer.h"

Shader::Shader()
{
	//
}

Shader::~Shader()
{
	if (m_program == 0)
	{
		return;
	}

	GLint numShaders = 0;
	glGetProgramiv(m_program, GL_ATTACHED_SHADERS, &numShaders);

	GLuint* shaders = new GLuint[numShaders];
	glGetAttachedShaders(m_program, numShaders, NULL, shaders);
	for (int i = 0; i < numShaders; i++)
	{
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(m_program);
	delete[] shaders;
}

void Shader::CompileShader(const std::string& shader, GLenum shaderType)
{
	// create program if not created
	if (m_program == 0)
	{
		m_program = glCreateProgram();
		if (m_program == 0)
		{
			std::cerr << "Error creating program.\n";
		}
	}

	GLuint shaderID = CreateShader(shader, shaderType);

	glAttachShader(m_program, shaderID);
}

void Shader::Link()
{
	assert(!m_isLinked && m_program != 0);

	glLinkProgram(m_program);

	GLint status;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Failed to link shader program.\n";

		GLint logLength;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength)
		{
			char* log = new char[logLength];
			GLsizei written;
			glGetProgramInfoLog(m_program, logLength, &written, log);

			std::cerr << "Program log: " << log << std::endl;

			delete[] log;
		}
	}
	else
	{
		m_isLinked = true;
	}
}

void Shader::Use()
{
	assert(m_program >= 0 && m_isLinked);
	glUseProgram(m_program);
}

void Shader::SetUniform(const std::string& uniformName, float x, float y, float z)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform3f(uniformLocation, x, y, z);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec2& v2)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform2f(uniformLocation, v2.x, v2.y);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec3& v3)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform3f(uniformLocation, v3.x, v3.y, v3.z);
}

void Shader::SetUniform(const std::string& uniformName, const glm::vec4& v4)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform4f(uniformLocation, v4.x, v4.y, v4.z, v4.w);
}

void Shader::SetUniform(const std::string& uniformName, const glm::mat4& mx4)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mx4[0][0]);
}

void Shader::SetUniform(const std::string& uniformName, const glm::mat3& mx3)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, &mx3[0][0]);
}

void Shader::SetUniform(const std::string& uniformName, float vfloat)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1f(uniformLocation, vfloat);
}

void Shader::SetUniform(const std::string& uniformName, int vint)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1i(uniformLocation, vint);
}

void Shader::SetUniform(const std::string& uniformName, bool vbool)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1i(uniformLocation, vbool);
}

void Shader::SetUniform(const std::string& uniformName, GLuint vuint)
{
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1ui(uniformLocation, vuint);
}

GLint Shader::GetUniformLocation(const std::string& uniformName)
{
	auto uniformLocation = m_uniformLocations.find(uniformName);
	if (uniformLocation == m_uniformLocations.end())
	{
		m_uniformLocations[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
	}
	assert(m_uniformLocations[uniformName] != -1);

	return m_uniformLocations[uniformName];
}

void Shader::PrintActiveUniforms()
{
	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };
	
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
	
	for (int i = 0; i < numUniforms; ++i)
	{
		GLint results[4];
		glGetProgramResourceiv(m_program, GL_UNIFORM, i, 4, properties, 4, NULL, results);
		
		if (results[3] != -1) continue; // skip uniform blocks

		GLint bufferSize = results[0] + 1;
		char* name = new char[bufferSize];
		glGetProgramResourceName(m_program, GL_UNIFORM, i, bufferSize, NULL, name);

		std::cout << "uniform " << results[2] << " " << Renderer::GetTypeString(results[1]) << " " << name << " " << std::endl;

		delete[] name;
	}
}

void Shader::PrintActiveAttribs()
{
	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	// IN
	GLint numAttribs;
	glGetProgramInterfaceiv(m_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	for (int i = 0; i < numAttribs; ++i)
	{
		GLint results[3];
		glGetProgramResourceiv(m_program, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

		GLint bufferSize = results[0] + 1;
		char* name = new char[bufferSize];
		glGetProgramResourceName(m_program, GL_PROGRAM_INPUT, i, bufferSize, NULL, name);

		std::cout << "in  " << results[2] << " " << Renderer::GetTypeString(results[1]) << " " << name  << std::endl;

		delete[] name;
	}

	// OUT
	glGetProgramInterfaceiv(m_program, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	for (int i = 0; i < numAttribs; ++i)
	{
		GLint results[3];
		glGetProgramResourceiv(m_program, GL_PROGRAM_OUTPUT, i, 3, properties, 3, NULL, results);

		GLint bufferSize = results[0] + 1;
		char* name = new char[bufferSize];
		glGetProgramResourceName(m_program, GL_PROGRAM_OUTPUT, i, bufferSize, NULL, name);

		std::cout << "out " << results[2] << " " << Renderer::GetTypeString(results[1]) << " " << name << std::endl;

		delete[] name;
	}

	std::cout << std::endl;
}

GLuint Shader::CreateShader(const std::string& shaderFilename, GLenum shaderType)
{
	// read shader file
	std::string shaderText = ReadFile(shaderFilename);
	const char* shaderString = shaderText.c_str();

	// vertex shader
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		std::cerr << "Error creating shader: " << shaderFilename << std::endl;
		return 0;
	}

	glShaderSource(shader, 1, &shaderString, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Shader compilation failed: " << shaderFilename << std::endl;
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength)
		{
			char* log = new char[logLength];
			GLsizei written;
			glGetShaderInfoLog(shader, logLength, &written, log);

			std::cerr << "Shader log: " << log << std::endl;

			delete[] log;
		}
	}

	return shader;
}

std::string Shader::ReadFile(const std::string& filename)
{
	std::string content;
	std::ifstream fileStream(filename, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cerr << "Could not read file " << filename << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}