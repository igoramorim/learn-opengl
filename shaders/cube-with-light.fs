#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform float uAmbientStrength;
uniform float uSpecularStrength;
uniform float uSpecularShininess;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
	// ambiente light
	float ambientStrength = uAmbientStrength; // 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	// diffuse
	vec3 norm = normalize(Normal); // normalize to make sure the it is a unit vector
	vec3 lightDir = normalize(uLightPos - FragPos);
	// Angle between the Normal and the LightDir
	// The lower the angle the more the impact of the light will have on the surface
	// Max used to keep the value >= 0
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;

	// specular
	float specularStrength = uSpecularStrength; // 0.5;
	vec3 viewDir = normalize(uViewPos - FragPos);
	/* The reflect function expects the first vector to point from the light source towards the fragment's position,
	but the lightDir vector is currently pointing the other way around: from the fragment towards the light source
	(this depends on the order of subtraction earlier on when we calculated the lightDir vector) */
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uSpecularShininess); // pow(x, y) where y (default 32) is the shininess. Keep it 2 4 8 16 31 64 128 256 ...
	vec3 specular = specularStrength * spec * uLightColor;

	vec3 result = (ambient + diffuse + specular) * uObjectColor;
	FragColor = vec4(result, 1.0);
};
