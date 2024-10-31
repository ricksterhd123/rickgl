#version 300 es
precision mediump float;

in vec2 TexCoord;
out vec4 FragColor;

uniform float gTime;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), abs(cos(gTime/1000.0f)));
   // FragColor = texture(texture1, TexCoord);
}
