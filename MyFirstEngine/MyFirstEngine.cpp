#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <array>
#include "Shader.h"

int width, height;
Shader* myShader;
float vertices[] = {
-0.25f, -0.25f, 0.0f,
0.25f, -0.25f, 0.0f,
0.25f, 0.25f, 0.0f
};

void DrawTriangle(std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f }, std::array<float, 3> offset = {0.0f, 0.0f, 0.0f}, std::array<float, 3> scale = {1.0f, 1.0f, 1.0f})
{
	myShader->SetUniformVec3("offset", offset);
	myShader->SetUniformVec3("scale", scale);
	myShader->SetUniformVec4("color", color);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void DrawRectangle(std::array<float, 4> color = { 1.0f, 1.0f, 1.0f, 1.0f }, std::array<float, 3> offset = {0.0f, 0.0f, 0.0f}, std::array<float, 3> scale = {1.0f, 1.0f, 1.0f})
{
	myShader->SetUniformVec3("offset", offset);
	myShader->SetUniformVec3("scale", scale);
	myShader->SetUniformVec4("color", color);
	myShader->SetUniformVec3("rotation", { 1.0f, 1.0f, 1.0f });
	glDrawArrays(GL_TRIANGLES, 0, 3);

	myShader->SetUniformVec3("rotation", { -1.0f, -1.0f, 1.0f });
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

	width = 640;
	height = 480;

    window = glfwCreateWindow(width, height, "My First Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

	myShader = new Shader();
	myShader->Initialize("VertexShader.glsl", "FragmentShader.glsl");

    unsigned int VBO;
    glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

		myShader->Use();


		std::array<float, 4> color = { 1.0f, 0.0f, 0.0f, 1.0f };
        //DrawTriangle();
		DrawRectangle(color);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
