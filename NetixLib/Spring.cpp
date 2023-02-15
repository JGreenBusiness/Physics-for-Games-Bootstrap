#include "Spring.h"
#include "Gizmos.h"

Spring::Spring(Rigidbody* _body1, Rigidbody* _body2,
	float _springCoefficient, float _damping,float _restLength, glm::vec4 _colour,
	glm::vec2 _contact1, glm::vec2 _contact2) : PhysicsObject(JOINT)
{
	m_body1 = _body1;
	m_body2 = _body2;

	m_springCoefficient = _springCoefficient;
	m_damping = _damping;

	if (_restLength == 0)
	{
		if (m_body1) m_body1->CalculateAxes();
		if (m_body2) m_body2->CalculateAxes();
		m_restLength = glm::distance(GetContact1(), GetContact2());
	}
	else
	{
		m_restLength = _restLength;
	}
	
	m_contact1 = _contact1;
	m_contact2 = _contact2;

	m_colour = _colour;
}

Spring::~Spring()
{
}

void Spring::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
	// Get the world coordinates of the ends of the springs
	glm::vec2 p1 = GetContact1();
	glm::vec2 p2 = GetContact2();

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	// apply damping
	glm::vec2 relativeVelocity = m_body2->GetVelocity() - m_body1->GetVelocity();

	// F = -kX - bv
	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;

	// cap the spring force to 1000 N to prevent numerical instability
	const float threshold = 1000.0f;
	float forceMag = glm::length(force);
	if (forceMag > threshold)
		force *= threshold / forceMag;

	m_body1->ApplyForce(-force * _timeStep, p1 - m_body1->GetPosition());
	m_body2->ApplyForce(force * _timeStep, p2 - m_body2->GetPosition());
}

void Spring::Draw(float _alpha)
{

	aie::Gizmos::add2DLine(GetContact1(), GetContact2(),m_colour);
}

glm::vec2 Spring::GetContact1()
{
	return m_body1 ? m_body1->ToWorld(m_contact1) : m_contact1;
}

glm::vec2 Spring::GetContact2()
{
	return m_body2 ? m_body2->ToWorld(m_contact2) : m_contact2;
}
