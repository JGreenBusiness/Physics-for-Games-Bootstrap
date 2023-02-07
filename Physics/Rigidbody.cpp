#include "Rigidbody.h"
#include <iostream>
Rigidbody::Rigidbody(ShapeType _shapeID, glm::vec2 _position, glm::vec2 _velocity, float _orientation, float _mass) :
	PhysicsObject(_shapeID)
{
	m_position = _position;
	m_velocity = _velocity;
	m_mass = _mass;
	m_orientation = _orientation;
	m_isKinematic = false;

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	CalculateAxes();

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}


	m_lastPosition = m_position;
	m_lastOrientation = m_orientation;

	m_velocity -= m_velocity * m_linearDrag * _timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;

	m_position += m_velocity * _timeStep;
	ApplyForce(_gravity * GetMass() * _timeStep,glm::vec2(0) );

	m_orientation += m_angularVelocity * _timeStep;

	

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}

}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
	m_velocity += _force / GetMass();
	m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

void Rigidbody::ResolveCollision(Rigidbody* _otherActor, glm::vec2 _contact, glm::vec2* _collisionNormal, float _pen)
{
	// find the vector between their centres, or use the provided direction
		// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(_collisionNormal ? *_collisionNormal : _otherActor->GetPosition() - GetPosition());

	// get the vector perpendicular to the collision normal
	glm::vec2 relativeVelocity = _otherActor->GetVelocity() - m_velocity;
	glm::vec2 perp(normal.y, -normal.x);

	// determine the total velocity of the contact points for the two objects, 
// for both linear and rotational 		

		// 'r' is the radius from axis to application of force
	float r1 = glm::dot(_contact - m_position, -perp);
	float r2 = glm::dot(_contact - _otherActor->m_position, perp);
	// velocity of the contact point on this object 
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(_otherActor->m_velocity, normal) +
		r2 * _otherActor->m_angularVelocity;
	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / _otherActor->GetMass() + (r2 * r2) / _otherActor->GetMoment());

		float elasticity = (GetElasticity() + _otherActor->GetElasticity()) / 2.0f;

		float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
			glm::dot(normal, normal * ((1 / GetMass()) + (1 / _otherActor->GetMass())));

		glm::vec2 force = normal * j;

		//apply equal and opposite forces
		//ApplyForce(_otherActor, -force, _contact);
		ApplyForce(-force, _contact - m_position);
		_otherActor->ApplyForce(force, _contact - _otherActor->m_position);

		if (_pen > 0)
		{
			PhysicsScene::ApplyContactForces(this, _otherActor, normal, _pen);
		}
	}



}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (GetMass() * glm::dot(m_velocity, m_velocity) +
		GetMoment() * m_angularVelocity * m_angularVelocity);
}

float Rigidbody::GetEnergy()
{
	return GetPotentialEnergy() + GetKineticEnergy();
}

void Rigidbody::CalculateAxes()
{
	float sn = sinf(m_orientation);
	float cs = cosf(m_orientation);
	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}


void Rigidbody::CalculateSmoothedPosition(float _alpha)
{
	m_smoothedPosition = _alpha * m_position + (1 - _alpha) * m_lastPosition;

	float smoothedOrientation = _alpha * m_orientation 
						+ (1 - _alpha) * m_lastOrientation;

	float sn = sinf(smoothedOrientation);
	float cs = cosf(smoothedOrientation);
	
	m_smoothedLocalX = glm::vec2(cs,sn);
	m_smoothedLocalY = glm::vec2(-sn,cs);
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}
