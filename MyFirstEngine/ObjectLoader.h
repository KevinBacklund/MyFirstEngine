#pragma once
#include <iostream>
#include <vector>
#include <string>

class ObjectLoader
{
public:
	struct Mesh
	{
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		size_t vertexCount = 0;
	};
	Mesh LoadObject(const std::string& filepath, const std::string& mtlBasePath = "../Assets/obj/");

private:

};

