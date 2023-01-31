#pragma once
#include "PhysicsObject.h"
#include "glm/vec4.hpp"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 _normal, float _distance, glm::vec4 _colour);
	~Plane();

	// Inherited via PhysicsObject
	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) ;
	virtual void Draw(float _alpha) ;

	glm::vec2 GetNormal() { return m_normal; };
	float GetDistance() {return m_distanceToOrigin;}

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_colour;

};

