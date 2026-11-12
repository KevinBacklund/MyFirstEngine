#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec3 vertexNormal;
in vec3 vertexPosition;
in vec2 vertexTexCoord;

uniform vec3 lightDirection;

void main()
{
	vec3 normalizedNormal = normalize(vertexNormal);
	vec3 lightDir = normalize(-lightDirection);
	float diff = max(dot(normalizedNormal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
	vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0); 
	vec3 resultColor = (ambient + diffuse) * vertexColor.rgb;
	FragColor = vec4(resultColor, vertexColor.a);
}