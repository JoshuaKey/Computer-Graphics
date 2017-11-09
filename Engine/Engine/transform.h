#pragma once

#include <glm\vec3.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(glm::vec3 position = glm::vec3(.0f), 
		glm::quat rotation = glm::quat(1.0f, .0f, .0f, .0f),
		glm::vec3 scale = glm::vec3(1.0f)) :
		position(position), rotation(rotation), scale(scale) {}
	~Transform() {}

	glm::mat4x4 GetMatrix()
	{
		glm::mat4 mxt = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 mxr = glm::mat4_cast(rotation);
		glm::mat4 mxs = glm::scale(glm::mat4(1.0f), scale);

		auto temp = mxt * mxr * mxs;

		return temp;
	}

};