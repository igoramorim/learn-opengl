#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec3 ourPos;
in vec2 TexCoord;

uniform float time;
uniform sampler2D ourTex;

void main()
{
   // FragColor = vec4(ourColor, 1.0);
   FragColor = texture(ourTex, TexCoord) * vec4(ourColor, 1.0);
};
