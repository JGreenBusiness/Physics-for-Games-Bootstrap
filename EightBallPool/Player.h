#pragma once
#include "PoolBall.h"
class Player
{
public:
	Player();
	~Player();

	// Getters
	int GetBallsSunk() { return m_ballsSunk; }
	BallType GetOwnedBallType() { return m_ownedBallType; }
	bool OwnsBallType() { return m_ownedBallType != Unowned; }
	Player* GetOpponent() { return m_opponent; }

	// Setters
	void SetBallsSunk(int _sunkBalls) { m_ballsSunk = _sunkBalls; }
	void AddSunkBall() { m_ballsSunk++; }
	void SetOwnedBallType(BallType _ballType);
	void SetOpponent(Player* _player) {m_opponent = _player; }

protected:
	int m_ballsSunk;
	BallType m_ownedBallType;
	Player* m_opponent;



};

