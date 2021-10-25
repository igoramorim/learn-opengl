#version 330 core

// sampler2D is a opaque wich means we can't instantiate it but only define them as uniforms
// we can't instantiate or pass as function parameter the struct Material
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float     shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uViewPos;
uniform float uTime;

void main()
{
	// ambient light
	vec3 ambient = uLight.ambient * texture(uMaterial.diffuse, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal); // normalize to make sure the it is a unit vector
	vec3 lightDir = normalize(uLight.position - FragPos);
	// Angle between the Normal and the LightDir
	// The lower the angle the more the impact of the light will have on the surface
	// Max used to keep the value >= 0
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * diff * texture(uMaterial.diffuse, TexCoords).rgb;

	// specular
	vec3 viewDir = normalize(uViewPos - FragPos);
	/* The reflect function expects the first vector to point from the light source towards the fragment's position,
	but the lightDir vector is currently pointing the other way around: from the fragment towards the light source
	(this depends on the order of subtraction earlier on when we calculated the lightDir vector) */
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess); // pow(x, y) where y (default 32) is the shininess. Keep it 2 4 8 16 31 64 128 256 ...
	vec3 specular = uLight.specular * spec * texture(uMaterial.specular, TexCoords).rgb;

	// emission
	vec3 emission = texture(uMaterial.emission, TexCoords).rgb;
	emission *= vec3(sin(uTime * 2.0), sin(uTime * 0.7), sin(uTime * 1.3));

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
};
