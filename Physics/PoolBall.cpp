#include "PoolBall.h"


PoolBall::PoolBall(glm::vec2 _position, float _mass, float _radius, int _ballNum) : Circle(_position,glm::vec2(0), _mass, _radius, glm::vec4(1))
{
	m_ballNum = _ballNum;

	m_ballType = BallType::SOLID;
	switch (m_ballNum)
	{
	case 0:
		m_colour = glm::vec4(1, .992, .816, 1); // Cream
		m_ballType = BallType::CUEBALL;
		break;
	case 1:
		m_colour = glm::vec4(1, 1, 0, 1); //Yellow
		break;
	case 9:
		m_colour = glm::vec4(1, 1, 0,0)/glm::vec4(2); //Yellow
		m_ballType = BallType::STRIPED;
		break;
	case 2:
		m_colour = glm::vec4(0, 0, 1, 1); // Blue
		break;
	case 10:
		m_colour = glm::vec4(0, 0, 1, 0)/glm::vec4(2); // Blue
		m_ballType = BallType::STRIPED;

		break;
	case 3:
		m_colour = glm::vec4(1, .4, .4, 1); // Light Red
		break;
	case 11:
		m_colour = glm::vec4(1, .4, .4, 0)/glm::vec4(2); // Light Red
		m_ballType = BallType::STRIPED;

		break;
	case 4:
		m_colour = glm::vec4(.627, .125, .941, 1); // Purple
		break;
	case 12:
		m_colour = glm::vec4(.627, .125, .941, 0)/glm::vec4(2); // Purple
		m_ballType = BallType::STRIPED;

		break;
	case 5:
		m_colour = glm::vec4(1, .647, 0, 1); // Orange
		break;
	case 13:
		m_colour = glm::vec4(1, .647, 0, 0)/glm::vec4(2); // Orange
		m_ballType = BallType::STRIPED;

		break;
	case 6:
		m_colour = glm::vec4(0, 1, 0, 1); // Green
		break;
	case 14:
		m_colour = glm::vec4(0, 1, 0, 0)/glm::vec4(2); // Green
		m_ballType = BallType::STRIPED;

		break;
	case 7:
		m_colour = glm::vec4(1, 0, 0, 1); // Red
		break;
	case 15:
		m_colour = glm::vec4(1, 0, 0, 0)/glm::vec4(2); // Red
		m_ballType = BallType::STRIPED;

		break;
	case 8:
		m_colour = glm::vec4(0,0, 0, 1); // Black
		m_ballType = BallType::BLACKBALL;
		break;
	default:
		m_colour = glm::vec4(1, 1, 1, 1); // White
		break;
	}
}

PoolBall::~PoolBall()
{
}
