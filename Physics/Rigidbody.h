#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType _shapeID, glm::vec2 _position,
		glm::vec2 _velocity, float _orientation, float _mass, glm::vec4 _colour);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep);
	void ApplyForce(glm::vec2 force);
	void ApplyForce(Rigidbody* _otherActor, glm::vec2 _force);

	glm::vec2 GetPosition() { return m_position; }
	float GetOritentation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;
	glm::vec4 m_colour;

	

	// Getters
	

	// Setters
private:



};

