#version 330 core

struct Light {
	vec3 ambient;
	vec3 diffuse;
};

out vec4 FragColor;

uniform Light uLight;

void main()
{
   vec3 result = uLight.ambient + uLight.diffuse;
   FragColor = vec4(result, 1.0);
};
