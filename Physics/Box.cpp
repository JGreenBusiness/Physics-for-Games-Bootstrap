#include "Box.h"
#include "Gizmos.h"


Box::Box(glm::vec2 _position, glm::vec2 _velocity, float _mass,
		 glm::vec2 _extents, glm::vec4 _colour) : 
	Rigidbody(ShapeType::BOX, _position, _velocity, 0, _mass)
{
	m_extents = _extents;
	m_colour = m_colour;
}

Box::~Box()
{
}

void Box::Draw(float _alpha)
{
	CalculateSmoothedPosition(_alpha);

	glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
		- m_smoothedLocalY * m_extents.y;
	glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
		- m_smoothedLocalY * m_extents.y;

	glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
		+ m_smoothedLocalY * m_extents.y;
	glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
		+ m_smoothedLocalY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}
