#version 330 core

layout (location = 0) in vec3 aPos;		// the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
   // Fragment position used to calculate the light ray
   // All the lighting calculations done in world space so we multiply the vertex position with the model matrix
   FragPos = vec3(uModel * vec4(aPos, 1.0));

   // TODO: Calculate Normal matrix on the CPU
   // http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
   // Transpose and inverse operations needed to avoid lighting errors when scaling non-uniform (e.g 0.6, 1.4) the object
   // Such operations are expensive and should be done in the CPU and send to shaders like uniform
   Normal = mat3(transpose(inverse(uModel))) * aNormal;

   TexCoords = aTexCoords;

   // Note that we read the multiplication from right to left
   gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
