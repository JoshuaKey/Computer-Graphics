#pragma once

#include "object.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera : public Object
{
public:
	enum class eType
	{
		EDITOR,
		ORBIT
	};

	struct Data
	{
		eType type;
		float fov;
		float distance;
		float nearClip;
		float farClip;
		float pitchMin;
		float pitchMax;
		
		Data()
		{
			type = eType::ORBIT;
			fov = 45.0f;
			distance = 4.0f;
			nearClip = 0.01f;
			farClip = 1000.0f;

			pitchMin = -89.0f;
			pitchMax = 89.0f;
		}
	};

public:
	Camera(const char* id, Scene* scene);
	~Camera();

	void Initialize(const glm::vec3& position, const glm::vec3& target, const Camera::Data& data = {});

	void Update();
	void UpdateTransformEditor(const glm::vec3& translate, const glm::vec3& rotate);
	void UpdateTransformOrbit(const glm::vec3& translate, const glm::vec3& rotate);

	void SetPosition(const glm::vec3& position) { transform.position = position; }
	void SetTarget(const glm::vec3& target);

	glm::mat4 GetView() { return m_mxView; }
	glm::mat4 GetProjection() { return m_mxProjection; }
	glm::vec3 GetDirection();

	Data& GetData() { return m_data; }
	
protected:
	void UpdateInput(glm::vec3& translate, glm::vec3& rotate);

private:
	Data m_data;

	glm::vec3 m_target;
	glm::mat4 m_mxView;
	glm::mat4 m_mxProjection;
};
