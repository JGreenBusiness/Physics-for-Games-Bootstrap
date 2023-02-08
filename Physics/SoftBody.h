#pragma once
#include "PhysicsScene.h"
#include <string>
class SoftBody
{
public:
	static void Build(PhysicsScene* scene, glm::vec2 position, float damping, float springForce, float spacing, std::vector<std::string>& strings);

protected:
	
};

