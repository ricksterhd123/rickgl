#version 300 es
precision mediump float;

in vec4 Color;
in vec2 TexCoord;
out vec4 FragColor;

uniform float gTime;
uniform sampler2D texture1;

void main()
{
   FragColor = Color * texture(texture1, TexCoord);
}
