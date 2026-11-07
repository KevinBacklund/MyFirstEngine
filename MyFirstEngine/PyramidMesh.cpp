#include "PyramidMesh.h"

PyramidMesh::PyramidMesh(glm::vec3 color)
{
	this->color = color;
	SetupMesh();
}

void PyramidMesh::SetupVertices()
{
	vertices =
	{
		 //Front Face (+Z)
		-0.5f, -0.5f,  0.5f, // 0: Bottom-Left
		 0.5f, -0.5f,  0.5f, // 1: Bottom-Right
		 0.0f,  0.5f,  0.0f, // 2: Top

		 //Back Face (-Z)
		-0.5f, -0.5f, -0.5f, // 3: Bottom-Left
		 0.5f, -0.5f, -0.5f, // 4: Bottom-Right
		 0.0f,  0.5f, 0.0f,  // 5: Top

		 //Right Face (+X)
		 0.5f, -0.5f,  0.5f, // 6: Bottom-Left
		 0.5f, -0.5f, -0.5f, // 7: Bottom-Right
		 0.0f,  0.5f, 0.0f,  // 8: Top

		 //Left Face (-X)
	     -0.5f, -0.5f, -0.5f, // 9: Bottom-Left
	     -0.5f, -0.5f,  0.5f, // 10: Bottom-Right
		  0.0f,  0.5f, 0.0f,  // 11: Top

		  //Bottom Face (-Y)
		  -0.5f, -0.5f, -0.5f, // 12: Back-Left
		   0.5f, -0.5f, -0.5f, // 13: Back-Right
		   0.5f, -0.5f,  0.5f, // 14: Front-Right
		  -0.5f, -0.5f,  0.5f, // 15: Front-Left
	};
	indices =
	{
		0,2,1, // Front Face
		3,5,4, // Back Face
		6,8,7, // Right Face
		9,11,10, // Left Face
		12,15,13, 15,13,14 // Bottom Face
	};
}