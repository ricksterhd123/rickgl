#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Color;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
   Color = aColor;
   Normal = aNormal;
   TexCoord = aTexCoord;

   FragPos = vec3(model * vec4(aPos, 1.0));
   gl_Position = projection * view * model * vec4(aPos, 1.0f);
}       
