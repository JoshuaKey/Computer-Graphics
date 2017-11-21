
#include "stdafx.h"

#include "material.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool Material::LoadTexture2D(const char* filename, GLuint activeTexture)
{
	int width;
	int height;
	int n;
	unsigned char* image = stbi_load(filename, &width, &height, &n, 0);
	if (!image)
	{
		return false;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLenum storageFormat = (n == 4) ? GL_RGBA : GL_RGB;
	GLenum imageFormat = (n == 4) ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, storageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	TextureInfo texInfo = {activeTexture, textureID, GL_TEXTURE_2D};
	m_textures.push_back(texInfo);

	delete[] image;
	return true;
}

bool Material::LoadTextureCube(const char* basename, const std::vector<std::string>& suffixes, const char* type,
	GLuint activeTexture)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);


	TextureInfo texInfo = {activeTexture, texture, GL_TEXTURE_CUBE_MAP};
	m_textures.push_back(texInfo);

	GLuint targets[] = 
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++){
		int width;
		int height;
		int n;
		std::string filename = basename + suffixes[i] + "." + type;
		unsigned char* image = stbi_load(filename.c_str(), &width, &height, &n, 0);
		assert(image);
		GLenum imageFormat = (n == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(targets[i], 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
		delete image;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return true;
}

void Material::AddTexture(GLuint textureID, GLuint activeTexture)
{
	TextureInfo texInfo = {activeTexture, textureID, GL_TEXTURE_2D};
	m_textures.push_back(texInfo);
}

void Material::SetTextures()
{
	for (TextureInfo textureInfo : m_textures)
	{
		glActiveTexture(textureInfo.activeTexture);
		glBindTexture(textureInfo.type, textureInfo.texture);
	}
}

GLuint Material::CreateTexture(int width, int height)
{
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}