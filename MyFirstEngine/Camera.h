#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <algorithm>

class Camera
{
public:
	Camera();
	void moveCamera(const glm::vec3& delta);
	void moveLookDirection(const glm::vec2& delta);
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition();
	glm::vec3 getFront();
	void setSensitivity(float sens);
	void setPosition(const glm::vec3& position);

private:
	glm::vec3 cameraPosition;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float yaw = -90.0f;
	float pitch = 0.0f; 
	float sensitivity = 0.15f;
};

