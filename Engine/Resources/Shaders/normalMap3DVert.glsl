#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out vec3 WorldPos;
out vec2 UV;
//out vec3 Normal;
out mat3 TBN;
//out vec3 Cross;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	WorldPos = vec3(model *  vec4(aPos, 1.0f));
	UV = aUV;
	//Normal = vec3(vec4(aNormal, 0.0f));
	//Cross = cross(Normal, vec3(.0f, .0f, 1.0f));
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	T = cross(N, B);
	TBN = mat3(T, B, N); // We could transpose and modify light normal info in Vertex for faster compute.

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}