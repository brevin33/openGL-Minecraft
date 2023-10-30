#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float OcclusionValue;

uniform sampler2D texture1;

void main()
{
	vec4 TexColor = texture(texture1,TexCoord);
	float toPos = TexCoord.x * 1024 / 16;
	float toPosy = TexCoord.y * 512 / 16;
	bool isGrass = toPos >= 31.0 && toPos <= 32.0 && toPosy >= 31.0 && toPosy <= 32.0;
	bool isLeaf = toPos >= 33.0 && toPos <= 34.0 && toPosy >= 6.0 && toPosy <= 7.0;
	if(isGrass){
		TexColor = TexColor * vec4(0.4,0.65,0.235,0.5) * 2.0;
	}
	else if(isLeaf){
		TexColor = TexColor * vec4(0.4,0.65,0.235,0.5) * 1.5;
	}
	if(TexColor.a < 0.3){
		discard;
	}
	FragColor = vec4(vec3(TexColor) * min((1 - OcclusionValue),.8) * 0.8,TexColor.a);
}