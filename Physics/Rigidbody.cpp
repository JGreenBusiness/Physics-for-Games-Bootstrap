#include "Rigidbody.h"

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
	m_velocity += _force / m_mass;
}

void Rigidbody::ApplyForce(Rigidbody* _otherActor, glm::vec2 _force)
{
	ApplyForce(_force);
	_otherActor->ApplyForce(-_force);
}
