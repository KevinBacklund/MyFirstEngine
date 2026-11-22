#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <array>
#include "Shader.h"
#include "cubemesh.h"
#include "glm.hpp"
#include "ObjectLoader.h"
#include <unordered_map>
#include "Camera.h"
#include "InputSystem.h"

#define PI 3.14159265358979323846f

int width, height;
float aspectRatio;
Shader* myShader;

bool ortho = false;
bool isSplitScreen = false;
int activePlayerCount = 1;

std::unordered_map<std::string, ObjectLoader::Mesh> loadedMeshes;

struct Object
{
	std::string meshName;
	unsigned int textureID;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

std::unordered_map < std::string, Object> sceneObjects;

struct Player
{
	Camera* playerCamera;
	InputSystem* playerInput;
};

std::array<Player*, 2> players;

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

void renderObj(const Object& obj, Shader* shader)
{
	shader->SetUniformVec3("offset", obj.position);
	shader->SetUniformVec3("scale", obj.scale);
	shader->SetUniformVec3("rotation", obj.rotation);
	glBindTexture(GL_TEXTURE_2D, obj.textureID);
	glBindVertexArray(loadedMeshes[obj.meshName].VAO);
	glDrawArrays(GL_TRIANGLES, 0, loadedMeshes[obj.meshName].vertexCount);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderScene(const std::unordered_map<std::string, Object>& sceneObjects, Shader* shader, Player* player)
{
	shader->SetUniformMat4("projection", glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f));
	shader->SetUniformMat4("view", player->playerCamera->getViewMatrix());
	shader->SetUniformVec3("lightDirection", glm::vec3(0.0f, -1.0f, -1.0f));

	for (const auto& pair : sceneObjects)
	{
		const Object& obj = pair.second;
		renderObj(obj, shader);
	}
}

void cameraMovement(Player* player)
{
	float moveSpeed = 0.1f;

	glm::vec3 flatForward = { player->playerCamera->getFront().x, 0.0f, player->playerCamera->getFront().z };

	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 flatRight = glm::normalize(glm::cross(flatForward, worldUp));

	glm::vec3 moveDirection(0.0f);

	if (player->playerInput->IsActionHeld(InputSystem::MoveForward)) moveDirection += flatForward;
	if (player->playerInput->IsActionHeld(InputSystem::MoveBack))    moveDirection -= flatForward;
	if (player->playerInput->IsActionHeld(InputSystem::MoveRight))   moveDirection += flatRight;
	if (player->playerInput->IsActionHeld(InputSystem::MoveLeft))    moveDirection -= flatRight;

	player->playerCamera->moveCamera(moveDirection * moveSpeed);

	//player->playerCamera->moveLookDirection(player->playerInput->GetMouseDelta());
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	width = 800;
	height = 600;
	aspectRatio = width / height;

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

	players[0] = new Player;
	players[0]->playerCamera = new Camera;
	players[0]->playerInput = new InputSystem;

	players[1] = new Player;
	players[1]->playerCamera = new Camera;
	players[1]->playerInput = new InputSystem;

	players[1]->playerInput->BindAction(InputSystem::MoveForward, GLFW_KEY_I);
	players[1]->playerInput->BindAction(InputSystem::MoveBack, GLFW_KEY_K);
	players[1]->playerInput->BindAction(InputSystem::MoveLeft, GLFW_KEY_J);
	players[1]->playerInput->BindAction(InputSystem::MoveRight, GLFW_KEY_L);


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

	loadedMeshes = ObjectLoader().LoadObjects("../Assets/obj/", "MyObjectLoader");

	sceneObjects["tree"] = { "tree", checkTex, glm::vec3(0.0f, -5.0f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
	sceneObjects["pallet"] = { "Wooden_Pallet", solidColorTex, glm::vec3(4.0f, -5.0f, -6.0f), glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
	sceneObjects["scroll"] = { "Scroll", checkTex, glm::vec3(-4.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
	sceneObjects["reed"] = { "reed", solidColorTex, glm::vec3(0.0f, -5.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
	sceneObjects["anvil"] = { "Anvil", checkTex, glm::vec3(1.4f, -2.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader->Use();

		float viewPortWidth = width / activePlayerCount;
		aspectRatio = viewPortWidth / height;

		for (int i = 0; i < activePlayerCount; i++)
		{
			glViewport(i * viewPortWidth, 0, viewPortWidth, height);
			players[i]->playerInput->ProccesInput(window);

			cameraMovement(players[i]);
			if (players[i]->playerInput->IsActionPressed(InputSystem::Actions::ToggleSplitScreen))
			{
				isSplitScreen = !isSplitScreen;
				activePlayerCount = isSplitScreen ? 2 : 1;
			}


			renderScene(sceneObjects, myShader, players[i]);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();

		CheckError();
	}

	glfwTerminate();
	return 0;	
}
