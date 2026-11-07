#pragma once
#include "Mesh.h"

class CubeMesh : public Mesh
{
public:
	CubeMesh(glm::vec3 color);

protected:
	void SetupVertices() override;
};

