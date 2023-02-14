#pragma once
#include "Circle.h"

enum BallType
{
	Stripped,
	Solid
};

class PoolBall : public Circle
{
public:
	PoolBall(glm::vec2 _position, float _mass, float _radius,int ballNum);
	~PoolBall();

protected:
	int m_ballNum;
	BallType m_ballType;

};

