#include "PoolBall.h"


PoolBall::PoolBall(glm::vec2 _position, float _mass, float _radius, int _ballNum) : Circle(_position,glm::vec2(0), _mass, _radius, glm::vec4(1))
{
	m_ballNum = _ballNum;

	m_ballType = Solid;
	switch (m_ballNum)
	{
	case 0:
		m_colour = glm::vec4(1, .992, .816, 1); // Cream
		m_ballType = CueBall;
		break;
	case 1:
		m_colour = glm::vec4(1, 1, 0, 1); //Yeallow
		break;
	case 9:
		m_colour = glm::vec4(1, 1, 0,0); //Yeallow
		m_ballType = Striped;
		break;
	case 2:
		m_colour = glm::vec4(0, 0, 1, 1); // Blue
		break;
	case 10:
		m_colour = glm::vec4(0, 0, 1, 0); // Blue
		m_ballType = Striped;

		break;
	case 3:
		m_colour = glm::vec4(1, .4, .4, 1); // Light Red
		break;
	case 11:
		m_colour = glm::vec4(1, .4, .4, 0); // Light Red
		m_ballType = Striped;

		break;
	case 4:
		m_colour = glm::vec4(.627, .125, .941, 1); // Purple
		break;
	case 12:
		m_colour = glm::vec4(.627, .125, .941, 0); // Purple
		m_ballType = Striped;

		break;
	case 5:
		m_colour = glm::vec4(1, .647, 0, 1); // Orange
		break;
	case 13:
		m_colour = glm::vec4(1, .647, 0, 0); // Orange
		m_ballType = Striped;

		break;
	case 6:
		m_colour = glm::vec4(0, 1, 0, 1); // Green
		break;
	case 14:
		m_colour = glm::vec4(0, 1, 0, 0); // Green
		m_ballType = Striped;

		break;
	case 7:
		m_colour = glm::vec4(1, 0, 0, 1); // Red
		break;
	case 15:
		m_colour = glm::vec4(1, 0, 0, 0); // Red
		m_ballType = Striped;

		break;
	case 8:
		m_colour = glm::vec4(0,0, 0, 1); // Black
		break;
	default:
		m_colour = glm::vec4(1, 1, 1, 1); // White
		break;
	}


}

PoolBall::~PoolBall()
{
}
