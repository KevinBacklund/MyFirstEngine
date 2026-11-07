#include "Mesh.h"


void Mesh::SetupVertices()
{
}

void Mesh::SetupMesh()
{
	SetupVertices();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Mesh::Draw(Shader* shader)
{
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	shader->Use();
	shader->SetUniformVec3("color", { color.x, color.y, color.z });
	shader->SetUniformVec3("offset", { offset.x, offset.y, offset.z });
	shader->SetUniformVec3("scale", { scale.x, scale.y, scale.z });
	shader->SetUniformVec3("rotation", { rotation.x, rotation.y, rotation.z });
	shader->SetUniformMat4("projection", projectionMatrix);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::SetOrtho(bool on)
{
	if (on)
	{
		projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}