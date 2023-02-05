#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>

#include "PhysicsScene.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType _shapeID, glm::vec2 _position,
		glm::vec2 _velocity, float _orientation, float _mass);
	 ~Rigidbody();

	void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;
	void ApplyForce(glm::vec2 force);
	void ApplyForce(Rigidbody* _otherActor, glm::vec2 _force);

	void ResolveCollision(Rigidbody* _otherActor);

	void ResetVelocity() { m_velocity = glm::vec2(0); }

	float GetPotentialEnergy();
	
	float GetKineticEnergy() override;
	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	void SetMass(float _mass) { m_mass = _mass; }
	
	float GetEnergy() override;

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;

	

	// Getters
	

	// Setters
private:



};

