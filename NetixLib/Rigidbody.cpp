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
	m_isTrigger = false;

	m_smoothedPosition = glm::vec2(0);
	m_smoothedLocalX = glm::vec2(0);
	m_smoothedLocalY = glm::vec2(0);

	m_lastOrientation = 0;

	m_angularVelocity = 0;
	m_moment = 0;

	m_localX = glm::vec2(0);
	m_localY = glm::vec2(0);

	m_linearDrag = .3f;
	m_angularDrag = .3f;

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	if (!m_active)
	{
		return;
	}

	CalculateAxes();

	// trigger checks
	if (m_isTrigger)
	{
		// check every object that is inside us and called triggerEnter on
		// if they haven’t registered inside us this frame, they must have exited
		// so remove them from our list and call triggerExit
		for (auto it = m_objectsInside.begin(); it != m_objectsInside.end(); it++)
		{
			if (std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it) == m_objectsInsideThisFrame.end())
			{
				if (triggerExit)
					triggerExit(*it);
				it = m_objectsInside.erase(it);
				if (it == m_objectsInside.end())
					break;
			}
		}
	}

	// clear this list now for next frame
	m_objectsInsideThisFrame.clear();
	
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

	m_orientation += m_angularVelocity * _timeStep;
	
	m_position += m_velocity * _timeStep;
	

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}

	
	ApplyForce((_gravity * GetMass()) * _timeStep, glm::vec2(0));

}

void Rigidbody::ApplyForce(glm::vec2 _force, glm::vec2 _pos)
{
	m_velocity += _force / GetMass();
	m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

void Rigidbody::ResolveCollision(Rigidbody* _otherActor, glm::vec2 _contact, glm::vec2* _collisionNormal, float _pen)
{
	if (!m_active)
	{
		return;
	}

	// register that these two objects have overlapped this frame
	m_objectsInsideThisFrame.push_back(_otherActor);
	_otherActor->m_objectsInsideThisFrame.push_back(this);
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

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		float kePre = GetKineticEnergy() + _otherActor->GetKineticEnergy();

		if(!m_isTrigger && !_otherActor->m_isTrigger)
		{
			//Apply equal and opposite forces
			ApplyForce(-force, _contact - m_position);
			_otherActor->ApplyForce(force, _contact - _otherActor->m_position);

			if (collisionCallback != nullptr) 
				collisionCallback(_otherActor);
			if (_otherActor->collisionCallback)
				_otherActor->collisionCallback(this);
		}
		else
		{
			TriggerEnter(_otherActor);
			_otherActor->TriggerEnter(this);
		}

		float kePost = GetKineticEnergy() + _otherActor->GetKineticEnergy();

		float deltaKE = kePost - kePre;
		if (deltaKE > kePost * 0.01f)
			std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";

		if (_pen > 0)
			PhysicsScene::ApplyContactForces(this, _otherActor, normal, _pen);
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

glm::vec2 Rigidbody::ToWorldSmoothed(glm::vec2 _localPos)
{
	return _localPos + (m_smoothedLocalX * m_smoothedPosition.x) + (m_smoothedLocalY * m_smoothedPosition.y);
}

glm::vec2 Rigidbody::ToWorld(glm::vec2 _localPos)
{
	//return _localPos + (m_localX * _localPos.x) + (m_localY * _localPos.y);
	return m_position;
}

void Rigidbody::TriggerEnter(PhysicsObject* _otherActor)
{
	
	if (m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), _otherActor) == m_objectsInside.end())
	{
		m_objectsInside.push_back(_otherActor);
		if (triggerEnter != nullptr)
			triggerEnter(_otherActor);
	}
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}
