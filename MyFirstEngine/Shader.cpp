#include "Shader.h"

std::string Shader::LoadShader(const char* aPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.open(aPath);

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	
	shaderFile.close();
	shaderCode = shaderStream.str();

	return shaderCode;
}

unsigned int Shader::LoadVertexShader(const char* aPath)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << Log << std::endl;
	}

	return shaderObject;
}

unsigned int Shader::LoadFragmentShader(const char* aPath)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);


	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << Log << std::endl;
	}

	return shaderObject;
}

void Shader::Initialize(const char* aVertexPath, const char* aFragmentPath)
{
	int result;
	char Log[512];

	unsigned int vertexShader = LoadVertexShader(aVertexPath);
	unsigned int fragmentShader = LoadFragmentShader(aFragmentPath);

	myShaderProgram = glCreateProgram();
	glAttachShader(myShaderProgram, vertexShader);
	glAttachShader(myShaderProgram, fragmentShader);
	glLinkProgram(myShaderProgram);

	glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(myShaderProgram, 512, NULL, Log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << Log << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(myShaderProgram);
}

void Shader::SetUniformVec3(const char* aName, std::array<float, 3> vec)
{
	int location = glGetUniformLocation(myShaderProgram, aName);
	glUniform3f(location, vec[0], vec[1], vec[2]);
}

void Shader::SetUniformVec4(const char* aName, std::array<float, 4> vec)
{
	int location = glGetUniformLocation(myShaderProgram, aName);
	glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
}