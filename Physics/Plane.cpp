#include "Plane.h"
#include "Gizmos.h"

Plane::Plane() : PhysicsObject(PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
}
Plane::Plane(glm::vec2 _normal, float _distance, glm::vec4 _colour) : 
	PhysicsObject(PLANE)
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_colour = _colour;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
}

void Plane::Draw(float _alpha)
{
	float lineSegmentLength = 300;

	glm::vec2 centrePoint = m_normal * m_distanceToOrigin;
	// easy to rotate through normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centrePoint + (parallel * lineSegmentLength);
	glm::vec2 end = centrePoint - (parallel * lineSegmentLength);
	//aie::Gizmos::add2DLine(start, end, colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f,m_colour, colourFade, colourFade);



}
