#pragma once
#include "Rigidbody.h"
class Circle : public Rigidbody
{
public:
	Circle(glm::vec2 _position, glm::vec2 _velocity,
		float _mass, float _radius, glm::vec4 _colour);

	~Circle();

	//Getter
	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }
	//Setter

protected:
	float m_radius;
	glm::vec4 m_colour;

	


	// Inherited via Rigidbody
	void Draw(float _alpha) override;

};

