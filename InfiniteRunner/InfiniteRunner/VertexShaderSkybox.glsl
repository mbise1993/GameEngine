#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 texCoord;

out vec4 fragColor;
out vec4 fragGlobalColor;
out vec3 fragNormal;
out vec3 fragPosition;
out vec3 fragCameraPosition;
out vec2 fragTexCoord;

uniform vec3 worldCameraPos;
uniform mat4 cameraToScreenMatrix;
uniform mat4 worldToCameraMatrix;
uniform mat4 localToWorldMatrix;

void main()
{
	fragTexCoord = texCoord;
	vec4 positionInWorld = localToWorldMatrix * position;
	gl_Position = cameraToScreenMatrix * worldToCameraMatrix * positionInWorld;

	vec3 worldVertexNormal = mat3(localToWorldMatrix) * vertexNormal;
	fragNormal = vec3(worldVertexNormal);
	vec3 unitNormal = normalize(fragNormal);

	fragGlobalColor = vertexColor;
	fragPosition = vec3(positionInWorld);
	fragCameraPosition = worldCameraPos;
	fragColor = vertexColor;
}