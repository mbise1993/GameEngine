#version 330

in vec4 fragColor;

out vec4 color;

void main()
{
	vec4 fragColorTrans = fragColor;
	fragColorTrans.a = 0.5;

	color = fragColorTrans;
}