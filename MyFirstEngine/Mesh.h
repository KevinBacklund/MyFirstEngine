#pragma once
#include <vector>
#include <glad/glad.h>
#include "Shader.h"
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>


class Mesh
{
public:
	void SetupMesh();
	void Draw(Shader* shader);
	void SetColor(glm::vec3 color) { this->color = color; }
	void SetRotation(glm::vec3 rotation) { this->rotation = rotation * glm::pi<float>() / 180.0f; };
	void SetScale(glm::vec3 scale) { this->scale = scale; }
	void SetOffset(glm::vec3 offset) { this->offset = offset; }

protected:
	virtual void SetupVertices();
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

private:
	unsigned int VAO, VBO, EBO;
};

