#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <array>
#include "Shader.h"
#include "cubemesh.h"
#include "pyramidmesh.h"
#include "spheremesh.h"
#include "glm.hpp"
#include "ObjectLoader.h"

#define PI 3.14159265358979323846f

int width, height;
Shader* myShader;
float vertices[] = {
-0.25f, -0.25f, 0.0f,
0.25f, -0.25f, 0.0f,
0.25f, 0.25f, 0.0f
};

bool ortho = false;
bool isSpacePressed = false;

GLenum CheckError()
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cerr << error << std::endl;
	}
	return errorCode;
}

float DegreesToRadians(float degrees)
{
	return degrees * (PI / 180.0f);
}

float SinDegrees(float degrees)
{
	return sinf(DegreesToRadians(degrees));
}

float CosDegrees(float degrees)
{
	return cosf(DegreesToRadians(degrees));
}

void DrawTriangle(std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f }, std::array<float, 3> offset = {0.0f, 0.0f, 0.0f}, std::array<float, 3> scale = {1.0f, 1.0f, 1.0f})
{
	myShader->SetUniformVec3("offset", offset);
	myShader->SetUniformVec3("scale", scale);
	myShader->SetUniformVec4("color", color);
    myShader->SetUniformVec3("rotation", { SinDegrees(0.0f), CosDegrees(0.0f), 1.0f });

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void DrawRectangle(std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f }, std::array<float, 3> offset = {0.0f, 0.0f, 0.0f}, std::array<float, 3> scale = {1.0f, 1.0f, 1.0f})
{
	myShader->SetUniformVec3("offset", offset);
	myShader->SetUniformVec3("scale", scale);
	myShader->SetUniformVec4("color", color);
	myShader->SetUniformVec3("rotation", { SinDegrees(0.0f), CosDegrees(0.0f), 1.0f });
	glDrawArrays(GL_TRIANGLES, 0, 3);

	myShader->SetUniformVec3("rotation", { SinDegrees(180.0f), CosDegrees(180.0f), 1.0f });
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void DrawHouse(std::array<float, 4> roofColor = { 0.0f, 1.0f, 1.0f, 1.0f })
{
	std::array<float, 4> houseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	std::array<float, 4> doorColor = { 0.5f, 0.25f, 0.0f, 1.0f };
	std::array<float, 4> windowColor = { 0.0f, 0.0f, 1.0f, 1.0f };
    
	std::array<float, 3> houseOffset = { 0.0f, -0.25f, 0.0f };

	DrawRectangle(houseColor, houseOffset, { 1.0f, 1.0f, 1.0f });
	DrawTriangle(roofColor, { 0.0f, 0.125f, 0.0f }, { 1.2f, 0.5f, 1.0f });

	DrawRectangle(doorColor, { 0.0f, -0.375f, 0.0f }, { 0.25f, 0.5f, 1.0f });
	DrawRectangle(windowColor, { -0.15f, -0.25f, 0.0f }, { 0.25f, 0.25f, 1.0f });
	DrawRectangle(windowColor, { 0.15f, -0.25f, 0.0f }, { 0.25f, 0.25f, 1.0f });
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	width = 800;
	height = 600;

	std::array<float, 4> roofColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	window = glfwCreateWindow(width, height, "My First Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	myShader = new Shader();
	myShader->Initialize("VertexShader.glsl", "FragmentShader.glsl");

	//CubeMesh* myCubeMesh = new CubeMesh({ 0.0f, 1.0f, 0.0f });
	//glm::vec3 cubeRotation = { 0.0f, 0.0f, 0.0f };

	//PyramidMesh* myPyramidMesh = new PyramidMesh({ 1.0f, 0.0f, 0.0f });
	//myPyramidMesh->SetRotation({ 35.0f, 0.0f, 0.0f });
	//glm::vec3 pyramidScale = { 1.0f, 1.0f, 1.0f };

	//SphereMesh* mySphereMesh = new SphereMesh({ 0.0f, 0.0f, 1.0f }, 0.5f);

 //   unsigned int VBO;
 //   glGenBuffers(1, &VBO);

	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//
 //   glBindVertexArray(VAO);
 //   glBindBuffer(GL_ARRAY_BUFFER, VBO);

 //   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

 //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 //   glEnableVertexAttribArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	CheckError();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader->Use();

		ObjectLoader::Mesh mesh = ObjectLoader().LoadObject("../Assets/obj/tree.obj");
		myShader->SetUniformVec3("offset", { 0.0f, -5.0f, -10.0f });
		myShader->SetUniformVec3("scale", { 1.0f, 1.0f, 1.0f });
		myShader->SetUniformVec3("rotation", { 0.0f, 0.0f, 0.0f });
		myShader->SetUniformVec3("color", { 0.0f, 1.0f, 0.0f });
		myShader->SetUniformMat4("projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)* glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		myShader->SetUniformVec3("lightDirection", { 0.5f, -0.8f, 0.3f });
		glBindVertexArray(mesh.VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();

		CheckError();
	}

	glfwTerminate();
	return 0;	
}
