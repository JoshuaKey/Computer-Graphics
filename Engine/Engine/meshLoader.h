#pragma once

#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

class Mesh;

namespace MeshLoader
{

	bool ParseModel(const wchar_t *filePath, Mesh& mesh);
	bool ParseModel(const wchar_t *filePath,
		std::vector<glm::vec3>& pos,
		std::vector<glm::vec2>& uv,
		std::vector<glm::vec3>& norm);
}
