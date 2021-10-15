#version 330 core

out vec4 FragColor;

uniform vec3 u_objectColor;
uniform vec3 u_lampColor;

void main()
{
   FragColor = vec4(u_lampColor * u_objectColor, 1.0);
};
