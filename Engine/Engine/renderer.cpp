#include "stdafx.h"

#include "renderer.h"

#include <glm/vec3.hpp>

//#define FULLSCREEN
int Renderer::m_width = 0;
int Renderer::m_height = 0;

void resize_callback(GLFWwindow* window, int width, int height)
{
	Renderer::m_width = width;
	Renderer::m_width = width;
}

Renderer::Renderer(Engine* engine) : System(engine)
{
	m_window = nullptr;
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	if (!glfwInit())
	{
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef FULLSCREEN
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	m_window = glfwCreateWindow(videoMode->width, videoMode->height, "computer graphics", monitor, nullptr);
#else
	m_window = glfwCreateWindow(640, 480, "computer graphics", nullptr, nullptr);
#endif // FULLSCREEN
	if (!m_window)
	{
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	Renderer::m_width = 640;
	Renderer::m_height = 480;

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return false;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL Version: " << version << std::endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	
	return true;
}

void Renderer::Shutdown()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Renderer::Update()
{
	//
}

GLuint Renderer::CreateShaderProgram(const std::string& vertextShaderFilename, const std::string& fragmentShaderFilename)
{
	GLuint vertexShader = CreateShader(vertextShaderFilename, GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	if (program == 0)
	{
		std::cerr << "Error creating program.\n";
		return 0;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glLinkProgram(program);
	
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Failed to link shader program.\n";

		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength)
		{
			char* log = new char[logLength];
			GLsizei written;
			glGetProgramInfoLog(program, logLength, &written, log);

			std::cerr << "Program log: " << log << std::endl;

			delete[] log;
		}
	}
	else
	{
		glUseProgram(program);
	}

	return program;
}

GLuint Renderer::CreateShader(const std::string& shaderFilename, GLenum shaderType)
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

std::string Renderer::ReadFile(const std::string& filename)
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