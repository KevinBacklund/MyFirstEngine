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

unsigned int CreateSolidColorTexture(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned char pixelData[4] = { r, g, b, a };

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0); 
	return textureID;
}

void renderObj(const ObjectLoader::Mesh& mesh, unsigned int textureID, Shader* shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	shader->SetUniformVec3("offset", position);
	shader->SetUniformVec3("scale", scale);
	shader->SetUniformVec3("rotation", rotation);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(mesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	CheckError();




	unsigned char checkPixels[] = {
		255, 255, 255,   100,   100,   100,  
		  100,   100,   100, 255, 255, 255   
	};

	unsigned int checkTex;
	glGenTextures(1, &checkTex);
	glBindTexture(GL_TEXTURE_2D, checkTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, checkPixels);

	unsigned int solidColorTex = CreateSolidColorTexture(255, 0, 0);

	//ObjectLoader::Mesh treeMesh = ObjectLoader().LoadObjectTiny("../Assets/obj/tree.obj");
	//ObjectLoader::Mesh palletMesh = ObjectLoader().LoadObjectTiny("../Assets/obj/Wooden_Pallet.obj");
	//ObjectLoader::Mesh scrollMesh = ObjectLoader().LoadObjectTiny("../Assets/obj/Scroll.obj");
	//ObjectLoader::Mesh reedMesh = ObjectLoader().LoadObjectTiny("../Assets/obj/reed.obj");
	//ObjectLoader::Mesh anvilMesh = ObjectLoader().LoadObjectTiny("../Assets/obj/Anvil.obj");

	ObjectLoader::Mesh treeMesh = ObjectLoader().MyLoadObject("../Assets/obj/tree.obj");
	ObjectLoader::Mesh palletMesh = ObjectLoader().MyLoadObject("../Assets/obj/Wooden_Pallet.obj");
	ObjectLoader::Mesh scrollMesh = ObjectLoader().MyLoadObject("../Assets/obj/Scroll.obj");
	ObjectLoader::Mesh reedMesh = ObjectLoader().MyLoadObject("../Assets/obj/reed.obj");
	ObjectLoader::Mesh anvilMesh = ObjectLoader().MyLoadObject("../Assets/obj/Anvil.obj");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader->Use();

		myShader->SetUniformMat4("projection", glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)* glm::lookAt(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		myShader->SetUniformVec3("lightDirection", glm::vec3(0.0f, -1.0f, -1.0f));

		glActiveTexture(GL_TEXTURE0);

		renderObj(treeMesh, checkTex, myShader, glm::vec3(0.0f, -5.0f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		renderObj(palletMesh, solidColorTex, myShader, glm::vec3(4.0f, -5.0f, -6.0f), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		renderObj(scrollMesh, checkTex, myShader,glm::vec3(-4.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		renderObj(reedMesh, solidColorTex, myShader, glm::vec3(0.0f, -5.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		renderObj(anvilMesh, checkTex, myShader, glm::vec3(1.4f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		glfwSwapBuffers(window);

		glfwPollEvents();

		CheckError();
	}

	glfwTerminate();
	return 0;	
}
