#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box(glm::vec2 _position, glm::vec2 _velocity,
		float _mass, glm::vec2 _extents, glm::vec4 _colour);
	~Box();


	// Getters
	
	glm::vec2 GetExtents() { return m_extents; };
	glm::vec2 GetColour() { return m_colour; }
	float GetWidth() { return m_extents.x * 2; }
	float GetHeight() { return m_extents.y * 2; }

	// Setters


protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;

private:


	// Inherited via Rigidbody
	virtual void Draw(float _alpha) override;

};

