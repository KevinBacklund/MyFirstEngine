#include "Camera.h"
#include <iostream>


Camera::Camera()
{
    cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::moveCamera(const glm::vec3& delta)
{
	cameraPosition += delta;
}

void Camera::moveLookDirection(const glm::vec2& delta)
{
    if (std::isnan(delta.x) || std::isnan(delta.y)) return;

    float xOffset = delta.x * sensitivity;
    float yOffset = -delta.y * sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    pitch = std::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

glm::mat4 Camera::getViewMatrix() 
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

glm::vec3 Camera::getPosition() 
{
	return cameraPosition;
}

glm::vec3 Camera::getFront() 
{
	return cameraFront;
}

void Camera::setSensitivity(float sens)
{
    sensitivity = sens;
}

void Camera::setPosition(const glm::vec3& position)
{
    cameraPosition = position;
}