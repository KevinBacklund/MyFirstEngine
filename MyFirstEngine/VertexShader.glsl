#version 330 core
layout(location = 0) in vec3 aPos;

uniform vec3 offset;
uniform vec3 scale;
uniform vec3 rotation;
uniform vec4 color;

out vec4 vertexColor;

void main()
{
	vec3 position = aPos;
	vec3 rotatedPosition;
	rotatedPosition.x = position.x * rotation.y + position.y * rotation.x;
	rotatedPosition.y = position.y * rotation.y - position.x * rotation.x;
	rotatedPosition.z = position.z;
	gl_Position = vec4(rotatedPosition * scale + offset, 1.0);
	vertexColor = color;
}