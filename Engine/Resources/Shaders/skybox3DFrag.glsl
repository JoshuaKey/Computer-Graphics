#version 430

in vec3 reflectionDir;

uniform samplerCube cubeMapTexture;

uniform vec3 worldAmbience;

out vec4 outColorFragment;

void main()
{
	vec3 cubeMapColor = vec3(texture(cubeMapTexture, reflectionDir));
	//outColorFragment = vec4(cubeMapColor * worldAmbience, 1.0f);
	outColorFragment = vec4(cubeMapColor, 1.0f);
}