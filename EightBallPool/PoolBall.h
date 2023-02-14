#pragma once
#include "Circle.h"

enum BallType
{
	Unowned,
	CueBall,
	Stripped,
	Solid,
	BlackBall
};

class PoolBall : public Circle
{
public:
	PoolBall(glm::vec2 _position, float _mass, float _radius,int ballNum);
	~PoolBall();

	int GetBallNum() { return m_ballNum; }
	BallType GetType() { return m_ballType; }

protected:
	int m_ballNum;
	BallType m_ballType;

};

