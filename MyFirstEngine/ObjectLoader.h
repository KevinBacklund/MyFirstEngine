#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm.hpp>

class ObjectLoader
{
public:
	struct Mesh
	{
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		size_t vertexCount = 0;
	};
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
		bool hasTexCoord = false;
		bool hasNormal = false;
	};
	Mesh LoadObjectTiny(const std::string& filepath, const std::string& mtlBasePath = "../Assets/obj/");
	Mesh MyLoadObject(const std::string& filepath);
private:
	Vertex ParseFaceVertex(const std::string& faceVertex, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals);
};

