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
	ApplyForce(_gravity * m_mass * _timeStep,glm::vec2(0) );

	m_orientation = m_angularVelocity * _timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
	m_angularVelocity = (_force.y * _force.x - _force.x * _pos.y) / GetMoment();
	if (m_mass > 0)
	{
		m_velocity += _force / GetMass();
	}
}

void Rigidbody::ApplyForce(Rigidbody* _otherActor, glm::vec2 _force, glm::vec2 _pos)
{
	ApplyForce(_force,_pos);
	_otherActor->ApplyForce(-_force, _pos);
}

void Rigidbody::ResolveCollision(Rigidbody* _otherActor, glm::vec2 _contact, glm::vec2* _collisionNormal)
{
	// find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(_collisionNormal ? *_collisionNormal : _otherActor->m_position - m_position);

	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// determine the total velocity of the contact points for the two objects, 
	// for both linear and rotational 

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(_contact - m_position, -perp);
	float r2 = glm::dot(_contact - _otherActor->m_position, -perp);


	// velocity of the contact point on objects
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(_otherActor->m_velocity, normal) - r2 * _otherActor->m_angularVelocity;

	if (v1 > v2) // moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.

		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / _otherActor->m_mass + (r2 * r2) / _otherActor->m_moment);

		float elasticity = 1;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		//apply equal and opposite forces
		ApplyForce(-force, _contact - m_position);
		_otherActor->ApplyForce(force, _contact - _otherActor->m_position);
	}



	// << Old code for basic collision detection 06/02/23 >>
	//// Collision normal being the normalized difference in position is sufficient for now
	//glm::vec2 normal = glm::normalize(_otherActor->GetPosition() - m_position);

	//glm::vec2 relativeVelocity = _otherActor->GetVelocity() - m_velocity;

	//if (glm::dot(normal,relativeVelocity) >= 0)
	//{
	//	return;
	//}

	//// using an e coefficient of 1 means no energy will be lost
	//float elasticity = 1.0f;

	//// j is the impulse magnitude
	//float j = glm::dot(-(1.0f + elasticity) * (relativeVelocity),
	//	normal / ((1 / GetMass()) + (1 / _otherActor->GetMass())));

	//glm::vec2 force = normal * j;

	//float kePre = GetKineticEnergy() + _otherActor->GetKineticEnergy();

	//ApplyForce(_otherActor, -force);

	//float kePost = GetKineticEnergy() + _otherActor->GetKineticEnergy();

	//float deltaKE = kePost - kePre;

	//// Checking if any kenetic energy was lost
	//if (deltaKE > kePost * 0.01f)
	//{
	//	std::cout << "Kinetic energy discrepancy greater than 1% detected!" << std::endl;
	//}
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
