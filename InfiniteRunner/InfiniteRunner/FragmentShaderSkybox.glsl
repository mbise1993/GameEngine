#version 330
in vec4 fragColor;
in vec4 fragGlobalColor;
in vec3 fragNormal;
in vec3 fragPosition;
in vec3 fragCameraPosition;
in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	vec3 unitNormal = normalize(fragNormal);
	vec3 toCameraDir = normalize(fragCameraPosition - fragPosition);

	vec4 texFragColor = texture(tex, fragTexCoord);

	if(texFragColor.r == 0 && texFragColor.g == 0 && (texFragColor.b <= .02 && texFragColor.b >= 0.01)){
		discard;
	}else{
		color = texFragColor;
	}
}