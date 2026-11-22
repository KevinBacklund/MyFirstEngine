#pragma once
#include <glfw3.h>
#include <array>
#include <glm.hpp>
#include <map>

class InputSystem
{

public:
	enum Actions
	{
		MoveForward,
		MoveBack,
		MoveRight,
		MoveLeft,
		Count
	};
	
	InputSystem();

	void ProccesInput(GLFWwindow* window);
	bool IsActionPressed(Actions action);
	bool IsActionReleased(Actions action);
	bool IsActionHeld(Actions action);     
	glm::vec2 GetMouseDelta();

	void BindAction(Actions action, int glfwKey);
	int GetKeyForAction(Actions action);

private:
	bool firstMouse = true;
	glm::vec2 mouseDelta = {0.0f, 0.0f};
	glm::vec2 lastMousePosition;

	std::map<Actions, int> keyBindings;
	std::array<bool, Actions::Count> actionsCurrent;
	std::array<bool, Actions::Count> actionsPrevious;

};


