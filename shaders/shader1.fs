#version 330 core

in vec3 ourColor;
in vec3 ourPos;

out vec4 FragColor;

uniform float time;

void main()
{
   vec3 c = vec3(sin(time) * 0.5 + 0.5);
   c = vec3(ourPos.x);
   FragColor = vec4(c, 1.0);
   // FragColor = vec4(ourColor, 1.0);
};
