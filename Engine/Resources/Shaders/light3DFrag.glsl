#version 330

//in vec3 WorldPos;
//in vec2 UV;
//in vec3 Normal;

out vec4 FragColor;

/*
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

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform Light light;

struct Fog{
	float distMin;
	float distMax;
	vec3 color;
};

uniform Fog fog;
  
uniform vec3 viewPos; 
*/

//in vec4 pos;

uniform vec3 lightColor;

void main(){
/*
	vec4 defaultColor = texture(texture1, UV);
	//vec4 specColor = texture(texture2, UV);


    // Normalize
    vec3 normal = normalize(Normal);

    // ambience
    vec3 ambient = material.ambience * light.ambient * material.ambient;
    
    // diffuse
    vec3 lightDir = normalize(light.position - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular; 

	// fog
	float fogDist = length(viewPos - WorldPos);
	float fogIntensity = clamp((fogDist - fog.distMin) / (fog.distMax-fog.distMin), .0f, 1.0f);

	// Use the dot Product to be more efficient with distance

	//vec4 color = defaultColor * vec4(ambient + diffuse, 1.0) + specColor * vec4(specular, 1.0);
	vec4 color = defaultColor * vec4(ambient + diffuse + specular, 1.0);
	//vec4 color = defaultColor * vec4(diffuse, 1.0);

	color = mix(color, vec4(fog.color, 1.0f), fogIntensity);

	//FragColor = color;
	//FragColor = vec4(fog.color * fogDist, 1.0f);
	
	//FragColor = vec4(spec, spec, spec, 1.0f);
	*/


	//FragColor = vec4(.1f, .7f, .3f, 1.0f);






	FragColor = vec4(lightColor, 1.0);
} 