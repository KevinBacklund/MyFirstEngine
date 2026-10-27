#pragma once
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <iostream>

class Shader
{
private:
	std::string LoadShader(const char* aPath);
	unsigned int LoadVertexShader(const char* aPath);
	unsigned int LoadFragmentShader(const char* aPath);

public:
	unsigned int myShaderProgram;
	void Initialize(const char* aVertexPath, const char* aFragmentPath);
	void Use();
	void SetUniformVec3(const char* aName, std::array<float, 3> vec);
	void SetUniformVec4(const char* aName, std::array<float, 4> vec);
};

