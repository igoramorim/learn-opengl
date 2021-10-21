#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uViewPos;

void main()
{
	// ambient light
	vec3 ambient = uLight.ambient * uMaterial.ambient;

	// diffuse
	vec3 norm = normalize(Normal); // normalize to make sure the it is a unit vector
	vec3 lightDir = normalize(uLight.position - FragPos);
	// Angle between the Normal and the LightDir
	// The lower the angle the more the impact of the light will have on the surface
	// Max used to keep the value >= 0
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);

	// specular
	vec3 viewDir = normalize(uViewPos - FragPos);
	/* The reflect function expects the first vector to point from the light source towards the fragment's position,
	but the lightDir vector is currently pointing the other way around: from the fragment towards the light source
	(this depends on the order of subtraction earlier on when we calculated the lightDir vector) */
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess); // pow(x, y) where y (default 32) is the shininess. Keep it 2 4 8 16 31 64 128 256 ...
	vec3 specular = uLight.specular * (spec * uMaterial.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
};
