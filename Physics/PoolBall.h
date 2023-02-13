#pragma once
#include "Circle.h"
class PoolBall : public Circle
{
public:
	PoolBall(glm::vec2 _position, float _mass, float _radius,int ballNum);
	~PoolBall();

protected:
	int m_ballNum;
	bool m_striped;

};

