#pragma once
#include "Mesh.h"
class PyramidMesh : public Mesh
{
public:
	PyramidMesh(glm::vec3 color);

protected:
	void SetupVertices() override;
};

