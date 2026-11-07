#pragma once
#include "Mesh.h"
#include <glm.hpp>
#include <gtc/constants.hpp>

class SphereMesh : public Mesh
{
public:
	SphereMesh(glm::vec3 color, float radius);

protected:
	void SetupVertices() override;
	float radius;
};

