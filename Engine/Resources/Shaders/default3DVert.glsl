#version 330

layout (location=0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec3 WorldPos;
out vec2 UV;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	WorldPos = vec3(model *  vec4(aPos, 1.0f));
	UV = aUV;
	Normal = vec3(model *  vec4(aNormal, 1.0f));


	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = model * vec4(aPos, 1.0);
}