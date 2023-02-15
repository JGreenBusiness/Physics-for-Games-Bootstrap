#include "Player.h"

Player::Player()
{
	m_ownedBallType = UNOWNED;
	m_ballsSunk = 0;
	m_opponent = nullptr;
}
Player::~Player()
{

}


void Player::SetOwnedBallType(BallType _ballType)
{
	m_ownedBallType = _ballType;

	 if (!m_opponent->OwnsBallType())
	 {
		 if (_ballType == SOLID)
		 {
			 m_opponent->SetOwnedBallType(STRIPED);
		 }
		 else
		 {
			 m_opponent->SetOwnedBallType(SOLID);
		 }
	 }


}
