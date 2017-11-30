#version 430
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;

out vec3 reflectionDir;

subroutine vec3 reflectionModelType();
subroutine uniform reflectionModelType reflectionModel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos; 

subroutine(reflectionModelType)
vec3 skybox()
{
	return aPos;
}

subroutine(reflectionModelType)
vec3 reflection()
{    
	vec3 worldPosition = vec3(model * vec4(aPos, 1.0));
	vec3 worldNormal = vec3(model * vec4(aNorm, 0.0));
	vec3 worldView = normalize(viewPos - worldPosition);

	return reflect(worldView, worldNormal);
}

void main()
{    
	//reflectionDir = aPos;
	reflectionDir = reflectionModel();

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

// 