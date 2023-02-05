#include "Rigidbody.h"
#include <iostream>
Rigidbody::Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass) :
	PhysicsObject(_shapeID)
{
	m_position = _position;
	m_velocity = _velocity;
	m_mass = _mass;
	m_orientation = _orientation;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	m_position += m_velocity * _timeStep;
	ApplyForce(_gravity * m_mass * _timeStep);
}

void Rigidbody::ApplyForce(glm::vec2 _force)
{
	if (m_mass > 0)
	{
		m_velocity += _force / m_mass;
	}
}

void Rigidbody::ApplyForce(Rigidbody* _otherActor, glm::vec2 _force)
{
	ApplyForce(_force);
	_otherActor->ApplyForce(-_force);
}

void Rigidbody::ResolveCollision(Rigidbody* _otherActor)
{
	// Collision normal being the normalized difference in position is sufficient for now
	glm::vec2 normal = glm::normalize(_otherActor->GetPosition() - m_position);

	glm::vec2 relativeVelocity = _otherActor->GetVelocity() - m_velocity;

	if (glm::dot(normal,relativeVelocity) >= 0)
	{
		return;
	}

	// using an e coefficient of 1 means no energy will be lost
	float elasticity = 1.0f;

	// j is the impulse magnitude
	float j = glm::dot(-(1.0f + elasticity) * (relativeVelocity),
		normal / ((1 / GetMass()) + (1 / _otherActor->GetMass())));

	glm::vec2 force = normal * j;

	float kePre = GetKineticEnergy() + _otherActor->GetKineticEnergy();

	ApplyForce(_otherActor, -force);

	float kePost = GetKineticEnergy() + _otherActor->GetKineticEnergy();

	float deltaKE = kePost - kePre;

	// Checking if any kenetic energy was lost
	if (deltaKE > kePost * 0.01f)
	{
		std::cout << "Kinetic energy discrepancy greater than 1% detected!" << std::endl;
	}
}

float Rigidbody::GetKineticEnergy()
{
	return .5f * ((glm::dot(m_velocity, m_velocity) * m_mass));
}

float Rigidbody::GetEnergy()
{
	return GetPotentialEnergy() + GetKineticEnergy();
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}
