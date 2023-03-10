#pragma once
#include "PoolBall.h"
class Player
{
public:
	Player();
	~Player();

	bool OwnsBallType() { return m_ownedBallType != BallType::UNOWNED; }
	
	// Getters
	int GetBallsSunk() { return m_ballsSunk; }
	BallType GetOwnedBallType() { return m_ownedBallType; }
	Player* GetOpponent() { return m_opponent; }

	// Setters
	void SetBallsSunk(int _sunkBalls) { m_ballsSunk = _sunkBalls; }
	void AddSunkBall() { m_ballsSunk++; }
	void SetOwnedBallType(BallType _ballType);
	void SetOpponent(Player* _player) {m_opponent = _player; }

protected:
	int m_ballsSunk = 0;
	BallType m_ownedBallType = BallType::UNOWNED;
	Player* m_opponent;



};

