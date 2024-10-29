#version 330 core

uniform float gTime;

out vec4 FragColor;
in vec3 ourColor;

void main()
{
   float intensity = sin(gTime/1000);
   FragColor = vec4(ourColor.x * intensity, ourColor.y * intensity, ourColor.z * intensity, 1.0f);
}
