#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D objTexture1;
uniform sampler2D objTexture2;

void main()
{
	//color = vertexColor;
    color = mix(texture(objTexture1, texCoord), texture(objTexture2, texCoord), 0.2);
} 