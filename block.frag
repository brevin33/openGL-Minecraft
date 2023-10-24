#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float OcclusionValue;

uniform sampler2D texture1;

void main()
{
	vec4 TexColor = texture(texture1,TexCoord);
	FragColor = TexColor * (1 - OcclusionValue * .8);
}