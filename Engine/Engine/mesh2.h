#pragma once

#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

class Mesh
{
public:
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> uv;

	int verticeAmo;


};