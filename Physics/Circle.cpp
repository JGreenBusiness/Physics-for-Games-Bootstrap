#include "Circle.h"
#include "Gizmos.h"
Circle::Circle(glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius, glm::vec4 _colour) :
	Rigidbody(CIRCLE, _position, _velocity, 0, _mass)
{
	m_radius = _radius;
	m_colour = _colour;
}

Circle::~Circle()
{
}

void Circle::Draw(float _alpha)
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 15, m_colour);

	// << Temporary code which shows the previous and next frame >>
	//----------------------------------------------------------------
	
	// CalculateSmoothedPosition(alpha);
	//
	// // draw the physics frames at half alpha
	// glm::vec4 halfColor = m_color;
	// halfColor.a = 0.5f;
	// aie::Gizmos::add2DCircle(m_position, m_radius, 12, halfColor);
	// aie::Gizmos::add2DCircle(m_lastPosition, m_radius, 12, halfColor);
	//
	// aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);
	
	//----------------------------------------------------------------

}

