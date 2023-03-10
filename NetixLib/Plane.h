#pragma once
#include "PhysicsObject.h"
#include "glm/vec4.hpp"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 _normal, float _distance, glm::vec4 _colour);
	~Plane();

	// Inherited via PhysicsObject
	void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;
	void Draw(float _alpha) override;

	glm::vec2 GetNormal() { return m_normal; };
	float GetDistance() {return m_distanceToOrigin;}

	void ResolveCollision(Rigidbody* _otherActor, glm::vec2 _contact);


	float GetKineticEnergy() override;
	float GetEnergy() override;


protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_colour;

};

