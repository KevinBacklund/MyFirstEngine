#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm.hpp>
#include <unordered_map>
#include <filesystem>

class ObjectLoader
{
public:
	struct Mesh
	{
		std::string name;
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

	std::unordered_map<std::string, Mesh> LoadObjects(const std::string& folderPath, const std::string& objectLoader, const std::string& mtlBasePath = "../Assets/obj/");

private:
	Mesh LoadObjectTiny(const std::string& filepath, const std::string& mtlBasePath = "../Assets/obj/");
	Mesh MyLoadObject(const std::string& filepath);
	Vertex ParseFaceVertex(const std::string& faceVertex, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals);
};

