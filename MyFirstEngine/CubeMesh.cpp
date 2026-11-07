#include "CubeMesh.h"

CubeMesh::CubeMesh(glm::vec3 color)
{
	this->color = color;
	SetupMesh();
}

void CubeMesh::SetupVertices()
{
	vertices =
	{
        // Front Face (+Z)
         -0.5f, -0.5f,  0.5f, // 0: Bottom-Left
          0.5f, -0.5f,  0.5f, // 1: Bottom-Right
          0.5f,  0.5f,  0.5f, // 2: Top-Right
         -0.5f,  0.5f,  0.5f, // 3: Top-Left

         // Back Face (-Z)
         -0.5f, -0.5f, -0.5f, // 4: Bottom-Right (from local view)
         -0.5f,  0.5f, -0.5f, // 5: Top-Right
          0.5f,  0.5f, -0.5f, // 6: Top-Left
          0.5f, -0.5f, -0.5f, // 7: Bottom-Left

          // Top Face (+Y)
          -0.5f,  0.5f,  0.5f, // 8: Bottom-Left
           0.5f,  0.5f,  0.5f, // 9: Bottom-Right
           0.5f,  0.5f, -0.5f, // 10: Top-Right
          -0.5f,  0.5f, -0.5f, // 11: Top-Left

          // Bottom Face (-Y)
          -0.5f, -0.5f, -0.5f, // 12: Bottom-Left
           0.5f, -0.5f, -0.5f, // 13: Bottom-Right
           0.5f, -0.5f,  0.5f, // 14: Top-Right
          -0.5f, -0.5f,  0.5f, // 15: Top-Left

          // Right Face (+X)
           0.5f, -0.5f,  0.5f, // 16: Bottom-Left
           0.5f, -0.5f, -0.5f, // 17: Bottom-Right
           0.5f,  0.5f, -0.5f, // 18: Top-Right
           0.5f,  0.5f,  0.5f, // 19: Top-Left

           // Left Face (-X)
           -0.5f, -0.5f, -0.5f, // 20: Bottom-Left
           -0.5f, -0.5f,  0.5f, // 21: Bottom-Right
           -0.5f,  0.5f,  0.5f, // 22: Top-Right
           -0.5f,  0.5f, -0.5f, // 23: Top-Left
	};
	indices =
	{
    0,  1,  2,    2,  3,  0,  // Front
    4,  5,  6,    6,  7,  4,  // Back
    8,  9,  10,   10, 11, 8,  // Top
    12, 13, 14,   14, 15, 12, // Bottom
    16, 17, 18,   18, 19, 16, // Right
    20, 21, 22,   22, 23, 20  // Left
	};
}