#version 330


layout (location=0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec3 WorldPos;
out vec2 UV;
//out vec3 Normal;

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main(){
	//WorldPos = vec3(model *  vec4(aPos, 1.0f));
	UV = aUV;
	//Normal = vec3(model *  vec4(aNormal, 0.0f));

	gl_Position =  model * vec4(aPos, 1.0);
}

/*
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aUV;

out vec2 UV;

uniform mat4 model;

void main(){
	UV = aUV;

	projection *
	gl_Position = vec4(aPos, 1.0);
}
*/