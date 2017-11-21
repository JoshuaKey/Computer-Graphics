#version 330

in vec2 UV;

uniform sampler2D texture1;

out vec4 FragColor;

void main(){
	FragColor = texture(texture1, UV);
    //FragColor = vec4(.0f, 1.0f, .5f, 1.0f);
}