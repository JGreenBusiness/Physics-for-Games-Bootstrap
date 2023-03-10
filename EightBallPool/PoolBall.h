#pragma once
#include "Circle.h"
#include "Texture.h"
enum class BallType
{
	UNOWNED,
	CUEBALL,
	STRIPED,
	SOLID,
	BLACKBALL
};

class PoolBall : public Circle
{
public:
	PoolBall(glm::vec2 _position, float _mass, float _radius,int ballNum);
	~PoolBall();

	// Getters
	int GetBallNum() { return m_ballNum; }
	BallType GetType() { return m_ballType; }
	aie::Texture* GetTexture() { return m_texture; }

	//Setters
	void SetTexture(aie::Texture* _texture) { m_texture = _texture; }

protected:
	int m_ballNum;
	BallType m_ballType;
	aie::Texture* m_texture;
};

