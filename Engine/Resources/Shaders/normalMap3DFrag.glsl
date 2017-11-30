#version 430

in vec3 WorldPos;
in vec2 UV;
//in vec3 Normal;
//in vec3 Cross;
in mat3 TBN;

out vec4 FragColor;

uniform mat4 model;

layout(binding=0) uniform sampler2D texture1;
layout(binding=1) uniform sampler2D normalTexture;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

// World Am
uniform vec3 worldAmbience;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec4 position;
	vec3 direction;

	float exponent;
	float cutoff;

	int ID;
};
uniform Light lights[5];

struct Fog{
	float distMin;
	float distMax;
	vec3 color;
};
uniform Fog fog;
  
uniform vec3 viewPos; 

void defaultLight(int id, vec3 normal, out vec3 diffuse, out vec3 specular){
	Light light = lights[id];
	// For Point and Directional Lights

	// Diffuse
	vec3 lightDir = vec3(.0f, .0f, .0f);
	if(light.position.w == 0){ // Direction
		lightDir = vec3(light.position);
	} else { // Position
		lightDir = normalize(vec3(light.position) - WorldPos);
	}
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = diff * light.diffuse * material.diffuse;

	// Specular
	vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = spec * light.specular * material.specular; 
}

void spotLight(int id, vec3 normal, out vec3 diffuse, out vec3 specular){
	Light light = lights[id];

	// For Spot Lights
	vec3 directionToLight = normalize(vec3(light.position) - WorldPos);
	vec3 spotDirection = normalize(light.direction);

	float angle = acos(dot(-directionToLight, spotDirection));

	if(angle < light.cutoff){
		float spotFactor = pow(dot(-directionToLight, spotDirection), light.exponent);

		// Diffuse
		vec3 lightDir = vec3(.0f, .0f, .0f);
		if(light.position.w == 0){ // Direction
			lightDir = vec3(light.position);
		} else { // Position
			lightDir = normalize(vec3(light.position) - WorldPos);
		}
		float diff = max(dot(normal, lightDir), 0.0);
		diffuse = diff * light.diffuse * material.diffuse * spotFactor;

		//Specular
		vec3 viewDir = normalize(viewPos - WorldPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = spec * light.specular * material.specular * spotFactor; 
	} else {
		diffuse = vec3(.0f, .0f, .0f);
		specular = vec3(.0f, .0f, .0f);
	}
}

void main(){
	// Grab texture color
	vec4 defaultColor = texture(texture1, UV);

	vec3 normal = texture(normalTexture, UV).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	//normalMap.w = .0f;

	//vec3 normal = normalize(vec3(model * normalMap));

	//vec3 normal = texture(normalTexture, UV).rgb;
	//normal = normal * 2.0 - 1.0;
	//normal = cross(normal, Cross);

	//normal = normalize(normal);

    // Normalize
    //vec3 normal = normalize(Normal);

	vec3 ambient = material.ambient * worldAmbience;
	vec3 diffuse;
	vec3 specular;
	for(int i = 0; i < lights.length(); i++){
		vec3 tempDiffuse;
		vec3 tempSpecular;

		if(lights[i].ID == 1){
			defaultLight(i, normal, tempDiffuse, tempSpecular);
		} else if(lights[i].ID == 0) {
			spotLight(i, normal, tempDiffuse, tempSpecular);
		}
		
		diffuse += tempDiffuse;
		specular += tempSpecular;
	}

	// fog
	float fogDist = length(viewPos - WorldPos);
	float fogIntensity = clamp((fogDist - fog.distMin) / (fog.distMax-fog.distMin), .0f, 1.0f);
	// Use the dot Product to be more efficient with distance
	
	vec4 color = defaultColor * vec4(ambient + diffuse, 1.0);

	color = mix(color, vec4(fog.color, 1.0f), fogIntensity);

	FragColor = color;//vec4((normal + 1) / 2, 1.0);

	//FragColor = vec4((Cross - 1) / 2 + 1, 1.0);
} 