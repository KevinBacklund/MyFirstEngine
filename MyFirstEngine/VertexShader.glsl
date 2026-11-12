#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform vec3 offset;
uniform vec3 scale;
uniform vec3 rotation;
uniform vec3 color;
uniform mat4 projection;

out vec4 vertexColor;
out vec3 vertexNormal;
out vec3 vertexPosition;
out vec2 vertexTexCoord;

void main()
{
	vec3 position = aPos * scale;
    vec3 rotatedPosition;

    float cosZ = cos(rotation.z);
    float sinZ = sin(rotation.z);
    
    float tempX = position.x;
    rotatedPosition.x = tempX * cosZ - position.y * sinZ;
    rotatedPosition.y = tempX * sinZ + position.y * cosZ;
    rotatedPosition.z = position.z;

    float cosY = cos(rotation.y);
    float sinY = sin(rotation.y);
    
    tempX = rotatedPosition.x;
    rotatedPosition.x = tempX * cosY + rotatedPosition.z * sinY;
    rotatedPosition.z = -tempX * sinY + rotatedPosition.z * cosY;

    float cosX = cos(rotation.x);
    float sinX = sin(rotation.x);
    
    float tempY = rotatedPosition.y; 
    rotatedPosition.y = tempY * cosX - rotatedPosition.z * sinX;
    rotatedPosition.z = tempY * sinX + rotatedPosition.z * cosX;

	gl_Position = projection * vec4(rotatedPosition + offset, 1.0);
	vertexColor = vec4(color, 1.0);
    vertexNormal = aNormal;
	vertexPosition = aPos;
	vertexTexCoord = aTexCoord;
}