#include "InputSystem.h"

InputSystem::InputSystem()
{
	BindAction(Actions::MoveForward, GLFW_KEY_W);
	BindAction(Actions::MoveBack, GLFW_KEY_S);
	BindAction(Actions::MoveLeft, GLFW_KEY_A);
	BindAction(Actions::MoveRight, GLFW_KEY_D);
	BindAction(Actions::ToggleSplitScreen, GLFW_KEY_M);
	BindAction(Actions::SwapScene, GLFW_KEY_T);

}

void InputSystem::BindAction(Actions action, int glfwKey)
{
	if (action < 0 || action >= Actions::Count) return;
	keyBindings[action] = glfwKey;
}

int InputSystem::GetKeyForAction(Actions action)
{
	auto it = keyBindings.find(action);
	if (it != keyBindings.end())
	{
		return it->second;
	}
	return GLFW_KEY_UNKNOWN;
}

void InputSystem::ProccesInput(GLFWwindow* window)
{
	actionsPrevious = actionsCurrent;

	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		firstMouse = true;
		mouseDelta = glm::vec2(0.0f, 0.0f);
		return;
	}

	for (int i = 0; i < Actions::Count; ++i)
	{
		Actions action = static_cast<Actions>(i);
		auto it = keyBindings.find(action);

		if (it != keyBindings.end() && it->second != GLFW_KEY_UNKNOWN)
		{
			actionsCurrent[i] = (glfwGetKey(window, it->second) == GLFW_PRESS);
		}
		else
		{
			actionsCurrent[i] = false;
		}
	}

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glm::vec2 currentMousePosition(static_cast<float>(mouseX), static_cast<float>(mouseY));

	if (firstMouse)
	{
		lastMousePosition = currentMousePosition;
		firstMouse = false;
	}

	mouseDelta = currentMousePosition - lastMousePosition;
	lastMousePosition = currentMousePosition;
}

bool InputSystem::IsActionHeld(Actions action) 
{
	if (action < 0 || action >= Actions::Count) return false;
	return actionsCurrent[action];
}

bool InputSystem::IsActionPressed(Actions action) 
{
	if (action < 0 || action >= Actions::Count) return false;
	return actionsCurrent[action] && !actionsPrevious[action];
}

bool InputSystem::IsActionReleased(Actions action) 
{
	if (action < 0 || action >= Actions::Count) return false;
	return !actionsCurrent[action] && actionsPrevious[action];
}

glm::vec2 InputSystem::GetMouseDelta()
{
	return mouseDelta;
}