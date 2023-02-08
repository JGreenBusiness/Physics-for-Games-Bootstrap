#include "Box.h"
#include "Gizmos.h"


Box::Box(glm::vec2 _position, glm::vec2 _velocity,float _orientation, float _mass,
		 glm::vec2 _extents, glm::vec4 _colour) : 
	Rigidbody(ShapeType::BOX, _position, _velocity, _orientation, _mass)
{
	m_extents = _extents;
	m_colour = _colour;

	m_moment = 1.0f / 12.0f * GetMass() * GetWidth() * GetHeight();

	SetElastcity(.5f);
}

Box::~Box()
{
}

// check if any of the other box's corners are inside this box
bool Box::CheckBoxCorners(const Box& _box, glm::vec2& _contact, int& _numContacts, float& _pen, glm::vec2& _edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = _box.GetExtents().x * 2;
	float boxH = _box.GetExtents().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	// loop over all corners of the other box
	for (float x = -_box.GetExtents().x; x < boxW; x += boxW)
	{
		for (float y = -_box.GetExtents().y; y < boxH; y += boxH)
		{
			// Get the position in worldspace
			glm::vec2 p = _box.GetPosition() + x * _box.m_localX + y * _box.m_localY;
			// Get the position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, m_localX),
				glm::dot(p - m_position, m_localY));

			// update the extents in each cardinal direction in our box's space
			// (ie extents along the separating axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// if this corner is inside the box, add it to the list of contact points
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}

	// if we lie entirely to one side of the box along one axis, we've found a separating
	// axis, and we can exit
	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.y || minY >= m_extents.y)
		return false;
	if (numLocalContacts == 0)
		return false;

	bool res = false;
	_contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) /
		(float)numLocalContacts;
	_numContacts++;

	// find the minimum penetration vector as a penetration amount and normal
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < _pen || _pen == 0)) {
		_edgeNormal = m_localX;
		_pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < _pen || _pen == 0)) {
		_edgeNormal = -m_localX;
		_pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < _pen || _pen == 0)) {
		_edgeNormal = m_localY;
		_pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < _pen || _pen == 0)) {
		_edgeNormal = -m_localY;
		_pen = pen0;
		res = true;
	}
	return res;
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
