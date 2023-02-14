#include "Player.h"

Player::Player()
{
	m_ownedBallType = Unowned;
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
		 if (_ballType == Solid)
		 {
			 m_opponent->SetOwnedBallType(Stripped);
		 }
		 else
		 {
			 m_opponent->SetOwnedBallType(Solid);
		 }
	 }


}
