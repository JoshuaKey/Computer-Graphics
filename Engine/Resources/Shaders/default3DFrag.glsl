#version 330

in vec3 WorldPos;
in vec2 UV;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float ambience;
};

uniform Material material;

// For Directional and Point Lights
/*
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};
*/

// For SpotLights
struct Light {
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;
	float exponent;
	float cutoff;
};

uniform Light light;

struct Fog{
	float distMin;
	float distMax;
	vec3 color;
};

uniform Fog fog;
  
uniform vec3 viewPos; 

void phong(vec4 position, vec3 normal, out vec3 ambientDiffuse, out vec3 specular){

	// Ambience
	vec3 ambient = material.ambience * material.ambient;

	// For Spot Lights
	vec3 directionToLight = normalize(vec3(position) - WorldPos);
	vec3 spotDirection = normalize(light.direction);

	float angle = acos(dot(-directionToLight, spotDirection));

	if(angle < light.cutoff){
		float spotFactor = pow(dot(-directionToLight, spotDirection), light.exponent);

		// Diffuse
		vec3 lightDir = vec3(.0f, .0f, .0f);
		if(position.w == 0){ // Direction
			lightDir = vec3(position);
		} else { // Position
			lightDir = normalize(vec3(position) - WorldPos);
		}
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * light.diffuse * material.diffuse * spotFactor;

		ambientDiffuse = ambient + diffuse;

		//Specular
		vec3 viewDir = normalize(viewPos - WorldPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = spec * light.specular * material.specular * spotFactor; 
	} else {
		ambientDiffuse = ambient;
		specular = vec3(.0f, .0f, .0f);
	}


	// For Poitn and Directional Lights
	/*

	// Diffuse
	vec3 lightDir = vec3(.0f, .0f, .0f);
	if(position.w == 0){ // Direction
		lightDir = vec3(position);
	} else { // Position
		lightDir = normalize(vec3(position) - WorldPos);
	}
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

	ambientDiffuse = ambient + diffuse;

	// Specular
	vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = spec * light.specular * material.specular; 
	*/
}

void main(){
	vec4 defaultColor = texture(texture1, UV);
	//vec4 specColor = texture(texture2, UV);


    // Normalize
    vec3 normal = normalize(Normal);

	vec3 ambientDiffuse;
	vec3 specular;
	phong(vec4(light.position, 1.0f), normal, ambientDiffuse, specular);


    // ambience
    //vec3 ambient = material.ambience * light.ambient * material.ambient;
    
    // diffuse
    //vec3 lightDir = normalize(light.position - WorldPos);
	//vec3 lightDir = lightDirection;
    //float diff = max(dot(normal, lightDir), 0.0);
    //vec3 diffuse = diff * light.diffuse * material.diffuse;

    // specular
    //vec3 viewDir = normalize(viewPos - WorldPos);
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = spec * light.specular * material.specular; 

	// fog
	float fogDist = length(viewPos - WorldPos);
	float fogIntensity = clamp((fogDist - fog.distMin) / (fog.distMax-fog.distMin), .0f, 1.0f);
	// Use the dot Product to be more efficient with distance


	//vec4 color = defaultColor * vec4(ambient + diffuse, 1.0) + specColor * vec4(specular, 1.0);
	
	//vec4 color = defaultColor * vec4(ambient + diffuse + specular, 1.0);
	vec4 color = defaultColor * vec4(ambientDiffuse + specular, 1.0);

	color = mix(color, vec4(fog.color, 1.0f), fogIntensity);

	FragColor = color;
} 