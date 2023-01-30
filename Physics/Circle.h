#pragma once
#include "Rigidbody.h"
class Circle : public Rigidbody
{
public:
	Circle();
	Circle(glm::vec2 _position, glm::vec2 _velocity,
		float _mass, float _radius, glm::vec4 _colour);

	~Circle();

	virtual void Draw();

	//Getter

	//Setter

protected:
	float m_radius;

};

