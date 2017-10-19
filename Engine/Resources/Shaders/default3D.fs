#version 330

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;

out vec4 FragColor;

uniform float ambience;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 viewPos; 

void main(){
    vec3 color = vec3(UV, .5);

    // Normalize
    vec3 normal = normalize(Normal);

    // ambience
    //vec3 ambient = lightColor * ambience;
    
    // diffuse
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    //vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    //vec3 specular = spec * lightColor; 

    color = max(.0f, min(1.0f, (ambience + diff + spec))) * color;
	FragColor = vec4(color, 1.0);
}