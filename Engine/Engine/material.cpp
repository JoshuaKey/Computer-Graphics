
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

	TextureInfo texInfo = {activeTexture, textureID};
	m_textures.push_back(texInfo);

	delete[] image;
	return true;
}

void Material::SetTextures()
{
	for (TextureInfo textureInfo : m_textures)
	{
		glActiveTexture(textureInfo.activeTexture);
		glBindTexture(GL_TEXTURE_2D, textureInfo.texture);
	}
}