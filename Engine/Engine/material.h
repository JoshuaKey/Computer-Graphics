#pragma once

#include <glm\vec3.hpp>
#include <glad\glad.h>

#include <vector>

struct TextureInfo
{
	GLuint activeTexture; // Texture Unit
	GLuint texture; // Texture ID
};

class Material
{
public:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;

	std::vector<TextureInfo> m_textures;

	Material(glm::vec3 ambient = {1.0f, 1.0f, 1.0f},
		glm::vec3 diffuse = {1.0f, 1.0f, 1.0f},
		glm::vec3 specular = {1.0f, 1.0f, 1.0f},
		float brightness = 32.0f) :
		m_ambient(ambient), m_diffuse(diffuse), 
		m_specular(specular), m_shininess(brightness) {}
	~Material() {}

	void SetMaterial(glm::vec3 ambient = {1.0f, 1.0f, 1.0f},
		glm::vec3 diffuse = {1.0f, 1.0f, 1.0f},
		glm::vec3 specular = {1.0f, 1.0f, 1.0f},
		float brightness = 2.0f)
	{
		m_ambient = ambient;
		m_diffuse - diffuse;
		m_specular = specular;
		m_shininess = brightness;
	}

	bool LoadTexture2D(const char* filename, GLuint activeTexture);
	void SetTextures();

};
