#version 330

in vec3 Color;

out vec4 FragColor;

void main(){
	FragColor = vec4(Color, 1.0);
    //FragColor = vec4(1.0, .2, .57, 1.0);
}