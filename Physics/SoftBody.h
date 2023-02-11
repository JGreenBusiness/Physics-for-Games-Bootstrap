#pragma once
#include "PhysicsScene.h"
#include <string>
class SoftBody
{
public:
	static void Build(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings);

protected:
	
};

