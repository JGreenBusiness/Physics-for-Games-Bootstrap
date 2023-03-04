#include "EightBallPool.h"
#include "Texture.h"
#include "Font.h"
#include "Box.h"
#include "Plane.h"
#include "Gizmos.h"
#include "Circle.h"
#include <iostream>
#include "PoolBall.h"

EightBallPool::EightBallPool() 
{
	

}

EightBallPool::~EightBallPool() 
{

}

bool EightBallPool::startup() {
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	m_tableTexture = new aie::Texture("./textures/table.png");

	m_powerMax = 4.0f;
	m_power = 0.0f;
	m_lineEndPos = glm::vec2(0);
	m_switchPlayer = false;
	m_ballHit = false;
	m_ballSunk = false;
	m_gizmosOn = true;

	m_player1 = new Player();
	m_player2 = new Player();
	m_player1->SetOpponent(m_player2);
	m_player2->SetOpponent(m_player1);
	m_currentPlayer = m_player1;
	m_switchPlayer = false;
	m_gamePhase = GamePhase::START;
	m_cueBallPlaced = false;

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0));

	// Ball Spawning Logic 
	//-----------------------------------------------------------
	m_tableExtents = glm::vec2(75.0f, 38.0f);

	m_cueBallStartPos = glm::vec2(-m_tableExtents.x / 1.8f, 0);
	m_cueBall = new PoolBall(m_cueBallStartPos, .7f, 1.5f, 0);
	m_balls.push_back(m_cueBall);

	const int RACK_SIZE = 6;
	int ballOrder[15]{1,11,3,13,8,4,7,5,6,14,10,12,15,9,2};
	float radius = 1.8f;
	int ballIndex = 0;

	std::vector<glm::vec2> ballPositions;
	for (int i = 1; i < RACK_SIZE; i++)
	{
		for (int j = 0; j < i; j++)
		{
			glm::vec2 ballPos = glm::vec2(i * (radius * 2), radius + ((radius * 2) * j) -(i* (radius)));
			ballPositions.push_back(ballPos);
		}
	}

	for (int i = 0; i < ballPositions.size(); i++)
	{
		glm::vec2 pos = ballPositions[i];
		PoolBall* ball = new PoolBall(glm::vec2(pos.x, pos.y), .7f, radius, ballOrder[ballIndex]);
		ballIndex++;
		m_balls.push_back(ball);
		m_physicsScene->AddActor(ball);
	}
	//-----------------------------------------------------------
	// Boundary and hole spawning logic
	//-----------------------------------------------------------

	float boxSize = radius;

	Box* boxes[6] = {};
	glm::vec4 boxColour = glm::vec4(0, 1, 0, .8f);
	float width = m_tableExtents.x * 2;
	
	Circle* boxCaps[12] = {};
	int boxCapIndex = 0;

	Circle* holes[6] = {};
	int holeIndex = 0;
	float holeRadius = radius * 2;
	glm::vec4 holeColour = glm::vec4(0, 0, 0, .8f);

	
	// Sides
	for (int i = 0; i < 2; i++)
	{
		float dist = i * width;
		int index = i + 2;
		boxes[index] = new Box(glm::vec2(-m_tableExtents.x + dist, 0), glm::vec2(0), 0, 1, glm::vec2(boxSize, m_tableExtents.y - boxSize*5), boxColour);

		for (int j = 0; j < 2; j++)
		{
			// Initializing box caps and holes
			
			float h;
			float s;
			j % 2 == 0 ? h = -m_tableExtents.y : h = m_tableExtents.y;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCaps[boxCapIndex] = new Circle(glm::vec2(-m_tableExtents.x + dist, h - s ), glm::vec2(0), 1, boxSize, boxColour);
			boxCapIndex++;

			holes[holeIndex] = new Circle(glm::vec2(-m_tableExtents.x + dist, h), glm::vec2(0), 1, holeRadius, holeColour);
			holeIndex++;
		}

		boxes[index]->SetKinematic(true);
		m_physicsScene->AddActor(boxes[index]);

	}

	// Top 
	for (int i = 0; i < 2; i++)
	{
		float dist = i * width/2;

		int index = i + 4;
		boxes[index] = new Box(glm::vec2(m_tableExtents.x/2 - dist, m_tableExtents.y), glm::vec2(0), 0, 1, glm::vec2(m_tableExtents.x/2 - boxSize * 5, boxSize), boxColour);

		for (int j = 0; j < 2; j++)
		{
			// Initializing box caps and holes
			
			float w;
			float s;
			j % 2 == 0 ? w = -m_tableExtents.x/2 : w = m_tableExtents.x/2;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCaps[boxCapIndex] = new Circle(glm::vec2(m_tableExtents.x / 2 + w - s - dist, m_tableExtents.y), glm::vec2(0), 1, boxSize, boxColour);
			boxCapIndex++;

			if (j < 1 && i ==0)
			{
				holes[holeIndex] = new Circle(glm::vec2(m_tableExtents.x / 2 + w - dist, m_tableExtents.y + holeRadius), glm::vec2(0), 1, holeRadius, holeColour);
				holeIndex++;
			}
		}
		boxes[index]->SetKinematic(true);
		m_physicsScene->AddActor(boxes[index]);
	}
	
	// Bottom
	for (int i = 0; i < 2; i++)
	{
		float dist = i * width/2;

		boxes[i] = new Box(glm::vec2(m_tableExtents.x/2 - dist, -m_tableExtents.y), glm::vec2(0), 0, 1, glm::vec2(m_tableExtents.x/2 - boxSize * 5, boxSize), boxColour);

		for (int j = 0; j < 2; j++)
		{
			float w;
			float s;
			j % 2 == 0 ? w = -m_tableExtents.x/2 : w = m_tableExtents.x/2;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCaps[boxCapIndex]= new Circle(glm::vec2(m_tableExtents.x / 2 + w - s - dist, -m_tableExtents.y), glm::vec2(0), 1, boxSize, boxColour);
			boxCapIndex++;

			if (j < 1 && i == 0)
			{
				holes[holeIndex] = new Circle(glm::vec2(m_tableExtents.x / 2 + w - dist, -m_tableExtents.y - holeRadius), glm::vec2(0), 1, holeRadius, holeColour);
				holeIndex++;
			}
		}
		boxes[i]->SetKinematic(true);
		m_physicsScene->AddActor(boxes[i]);
	}
	//-----------------------------------------------------------

	for(int i = 0; i < 12;i++)
	{
		boxCaps[i]->SetKinematic(true);
		m_physicsScene->AddActor(boxCaps[i]);
	}

	// Set Hole Collision logic
	//-----------------------------------------------------------
	for (int i = 0; i < 6; i++)
	{
		holes[i]->SetIsTrigger(true);

		holes[i]->triggerEnter = [=](PhysicsObject* _other)
		{
			PoolBall* ball = dynamic_cast<PoolBall*>(_other);
			OnBallSunk(ball);
		};
		
		m_physicsScene->AddActor(holes[i]);
	}
	//-----------------------------------------------------------
	
	// Set CueBall Collision Logic
	//-----------------------------------------------------------
	m_cueBall->collisionCallback = [=](PhysicsObject* _other)
	{
		if ( _other->GetShapeID() == ShapeType::CIRCLE)
		{
			Rigidbody* rB = dynamic_cast<Rigidbody*>(_other);

			if (!rB->IsKinematic())
			{
				PoolBall* ball = dynamic_cast<PoolBall*>(rB);
				OnCueBallCollide(ball);
			}
		}
	};

	m_physicsScene->AddActor(m_cueBall);

	//-----------------------------------------------------------

	Plane* topPlane = new Plane(glm::vec2(0, 1), -m_tableExtents.y - boxSize, glm::vec4(1, 1, 1, 1));
	Plane* botPlane = new Plane(glm::vec2(0, -1), -m_tableExtents.y- boxSize, glm::vec4(1, 1, 1, 1));
	Plane* leftPlane = new Plane(glm::vec2(-1, 0), -m_tableExtents.x- boxSize, glm::vec4(1, 1, 1, 1));
	Plane* rightPlane = new Plane(glm::vec2(1, 0), -m_tableExtents.x- boxSize, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(topPlane);
	m_physicsScene->AddActor(botPlane);
	m_physicsScene->AddActor(leftPlane);
	m_physicsScene->AddActor(rightPlane);
	return true;
}

void EightBallPool::shutdown() 
{
	delete m_tableTexture;

	if (m_player1 != nullptr)
	{
		m_player1 = nullptr;

		delete m_player1;
	}

	if (m_player2 != nullptr)
	{
		m_player2 = nullptr;
		delete m_player2;
	}	


}

bool poolBallMoving(PoolBall* pB)
{
	return (pB->GetVelocity() != glm::vec2(0));
}
void EightBallPool::update(float _deltaTime) {

	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	m_physicsScene->Draw();
	
	// Gets players mouse pos and translates from screen to world pos
	int xScreen, yScreen;
	input->getMouseXY(&xScreen, &yScreen);
	glm::vec2 mouseWPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

	// Finds the first ball in m_balls with a velocity over 0
	std::vector<PoolBall*>::iterator movingBall =  std::find_if(m_balls.begin(), m_balls.end(), poolBallMoving);
	m_ballsStill = movingBall ==  m_balls.end() && !poolBallMoving(m_balls.back());
	
	if(m_ballsStill)
	{
		switch(m_gamePhase)
		{
			// Start Phase
			//-----------------------------------------------------------	
			case GamePhase::START:
				if (m_switchPlayer)
				{
					m_currentPlayer = m_currentPlayer->GetOpponent();
					m_switchPlayer = false;
					m_ballHit = false;
					m_cueBallPlaced = false;
				}

			// Place ball to start game
			if (!m_cueBallPlaced)
			{
				PrepareToPlaceCueBall(glm::vec2(m_tableExtents.x/6, m_tableExtents.y), mouseWPos,m_cueBallStartPos.x - m_tableExtents.x/6 - (m_cueBall->GetRadius()*2));

				if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
				{
					m_cueBallPlaced = true;
				}
			}
			else
			{
				m_gamePhase = GamePhase::PLAY;
			}
			break;
			//-----------------------------------------------------------	
			// Play Phase
			//-----------------------------------------------------------	
			case GamePhase::PLAY:
				if (m_switchPlayer)
				{
					m_currentPlayer = m_currentPlayer->GetOpponent();
					m_switchPlayer = false;
				}

			PrepareToShootBall(input, mouseWPos);
			break;
			//-----------------------------------------------------------	
			// Foul Phase
			//-----------------------------------------------------------	
			case GamePhase::FOUL:
				if (m_switchPlayer)
				{
					m_currentPlayer = m_currentPlayer->GetOpponent();
					m_switchPlayer = false;
					m_ballHit = false;
					m_cueBallPlaced = false;
				}
			
			PrepareToPlaceCueBall(glm::vec2(m_tableExtents.x, m_tableExtents.y), mouseWPos, 0);

			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT) && m_cueBall->GetActive())
			{
				m_gamePhase = GamePhase::PLAY;
				m_cueBallPlaced = true;
			}
			break;
			//-----------------------------------------------------------	
			case GamePhase::OVER:
				break;
		}
	}
	
	m_physicsScene->Update(_deltaTime);
	

}
void EightBallPool::CallFoul()
{
	m_gamePhase = GamePhase::FOUL;
	m_switchPlayer = true;
}

void EightBallPool::OnBallSunk(PoolBall* _sunkBall)
{
	_sunkBall->SetPosition(glm::vec2(-m_tableExtents.x - _sunkBall->GetRadius() * 4, ((_sunkBall->GetRadius()*2) *(m_player1->GetBallsSunk() + m_player2->GetBallsSunk())-m_tableExtents.y/1.2f)));
	_sunkBall->ResetVelocity();
	
	BallType opponentBallType = m_currentPlayer->GetOpponent()->GetOwnedBallType();
	BallType sunkBallType = _sunkBall->GetType();

	bool firstBallSunk = false;
	// If player has not sunk a ball yet then _sunkBall is the first.
	if(!m_ballSunk)
	{
		firstBallSunk = true;
		m_ballSunk = true;
	}

	//Adds to either player's or opponent's sunk ball fi sunk ball equals player's owned ball
	if(sunkBallType == m_currentPlayer->GetOwnedBallType() )
	{
		m_currentPlayer->AddSunkBall();

		// If the first ball player sinks is theirs set switchPlayer to false
		if(firstBallSunk)
		{
			m_switchPlayer = false;
		}
		return;
	}
	else if(sunkBallType == opponentBallType)
	{
		m_currentPlayer->GetOpponent()->AddSunkBall();

		// If player sinks opponents ball set switchPlayer to true
		m_switchPlayer = true;
		return;
	}

	// If player did not own the sunk ball, it was either the Cue ball, Black ball, or players ball types have not been set
	switch(sunkBallType)
	{
	case BallType::CUEBALL:
		if(!m_currentPlayer->OwnsBallType())
		{
			m_gamePhase = GamePhase::START;
			_sunkBall->ResetVelocity();
			m_switchPlayer = true;
			return;
		}
		else if(m_gamePhase == GamePhase::OVER && m_currentPlayer->GetBallsSunk() == 7)
		{
			m_currentPlayer = m_currentPlayer->GetOpponent();
		}
		else
		{
			CallFoul();
		}
		break;
	case BallType::BLACKBALL:
		if(m_currentPlayer->GetBallsSunk() != 7)
		{
			m_currentPlayer = m_currentPlayer->GetOpponent();
		}
		m_gamePhase = GamePhase::OVER;
		break;
	case BallType::STRIPED: 
		m_currentPlayer->SetOwnedBallType(sunkBallType);
		m_currentPlayer->GetOpponent()->SetOwnedBallType(BallType::SOLID);
		m_currentPlayer->AddSunkBall();
		m_switchPlayer = false;
		break;
	case BallType::SOLID:
		m_currentPlayer->SetOwnedBallType(sunkBallType);
		m_currentPlayer->GetOpponent()->SetOwnedBallType(BallType::STRIPED);
		m_currentPlayer->AddSunkBall();
		m_switchPlayer = false;
		break;
	case BallType::UNOWNED:
		std::cout << "ERROR: SunkBall UNOWNED" << std::endl;
		break;
	}
}

void EightBallPool::OnCueBallCollide(PoolBall* _otherBall)
{
	BallType playerBallType = m_currentPlayer->GetOwnedBallType();

	
	if(!m_ballHit)
	{
		if (playerBallType != _otherBall->GetType() && m_currentPlayer->OwnsBallType() && m_currentPlayer->GetBallsSunk() != 7)
		{
			CallFoul();
		}
	
		m_ballHit = true;
	}
}

void EightBallPool::PrepareToShootBall(aie::Input* _input,glm::vec2 _mouseWorldPos)
{
	// Logic for power and distance from ball to mouse
	glm::vec2 dir = glm::normalize(_mouseWorldPos - m_cueBall->GetPosition());
	float dist = glm::distance(m_cueBall->GetPosition(), _mouseWorldPos);
	float distCap = 50;
	dist > distCap ? m_power = distCap * 2, dist = distCap : m_power = dist * 2;

	m_lineEndPos = m_cueBall->GetPosition() - (dir * glm::vec2(dist));
	m_lineEndPos = WorldToScreen(m_lineEndPos);

	// Player shot ball
	if (_input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT) && m_cueBall->GetActive())
	{
		glm::dot(_mouseWorldPos, m_cueBall->GetPosition());
		m_cueBall->ApplyForce(-dir * (m_power), dir * (m_cueBall->GetRadius()));
		m_ballHit = false;
		m_ballSunk = false;
		m_switchPlayer = true;
	}
}

void EightBallPool::PrepareToPlaceCueBall(glm::vec2 _extents, glm::vec2 _mouseWorldPos, float _xOrigin)
{
	//Logic for cue ball placing at start
	glm::vec2 pos = glm::vec2(_mouseWorldPos.x, _mouseWorldPos.y);

	float top = _extents.y - m_cueBall->GetRadius() * 2;
	float bot = -_extents.y + m_cueBall->GetRadius() * 2;
	float sL = (_extents.x + m_cueBall->GetRadius() * 2) + _xOrigin;
	float sR = (-_extents.x - m_cueBall->GetRadius() * 2) + _xOrigin;

	// position cant exceed extents
	pos.y > top ? pos.y = top :
		pos.y < bot ? pos.y = bot
		: pos.y = pos.y;

	pos.x < sR ? pos.x = sR :
		pos.x > sL ? pos.x = sL
		: pos.x = pos.x;

	for (int i = 0; i < m_balls.size(); i++)
	{
		PoolBall* ball = m_balls[i];

		if (ball != m_cueBall)
		{
			float dist = glm::distance(pos, ball->GetPosition());

			if (dist <= m_cueBall->GetRadius() + ball->GetRadius())
			{
				m_cueBall->SetActive(false);
				break;
			}
			else
			{
				m_cueBall->SetPosition(pos);
				m_cueBall->SetActive(true);

			}
		}
	}
}

void EightBallPool::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	m_2dRenderer->drawSprite(m_tableTexture, getWindowWidth()/2.0f - 5.5f, getWindowHeight()/2.0f, (getWindowWidth() / m_aspectRatio) * 1.5f, (getWindowHeight() / m_aspectRatio)*1.5f, 3.14159265f, 1.0f);

	// Winning Text
	if (m_gamePhase == GamePhase::OVER && m_cueBall->GetVelocity() == glm::vec2(0))
	{
		std::string winner = "Winner :";
		m_currentPlayer == m_player1 ? winner += "Player 1" : winner += "Player 2";
		m_2dRenderer->drawText(m_font, winner.c_str(), getWindowWidth() / 2.0f, getWindowHeight() / 2.0f);
	}
	else
	{
		// CueBall line
		if (m_ballsStill && m_gamePhase == GamePhase::PLAY)
		{
			glm::vec2 cueBallPos = WorldToScreen(m_cueBall->GetPosition());
			m_2dRenderer->drawLine(cueBallPos.x, cueBallPos.y, m_lineEndPos.x, m_lineEndPos.y, 1);
		}

		// Turn Text
		std::string turnText;
		m_currentPlayer == m_player1 ? turnText = "Player 1" : turnText = "Player 2";
		turnText += "'s Turn";
		m_2dRenderer->drawText(m_font, turnText.c_str(),getWindowWidth() / 4.0f , 16);

		std::string p1Text = "Player 1: ";
		std::string p2Text = "Player 2: ";


		if (m_currentPlayer->OwnsBallType())
		{
			// Player BallType Text
			m_player1->GetOwnedBallType() == BallType::STRIPED ? p1Text += "Striped " : p1Text += "Solid ";
			m_player2->GetOwnedBallType() == BallType::STRIPED ? p2Text += "Striped " : p2Text += "Solid ";

			p1Text += std::to_string(m_player1->GetBallsSunk());
			p2Text += std::to_string(m_player2->GetBallsSunk());
		}
		m_2dRenderer->drawText(m_font, p1Text.c_str(), getWindowWidth() / 4.0f , getWindowHeight() - 32.0f);
		m_2dRenderer->drawText(m_font, p2Text.c_str(), getWindowWidth() / 2.0f + 32, getWindowHeight() - 32.0f);
	}
	
	

	

	// done drawing sprites
	m_2dRenderer->end();

	if(m_gizmosOn)
	{
		aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
	}

}

glm::vec2 EightBallPool::ScreenToWorld(glm::vec2 _screenPos)
{
	glm::vec2 worldPos = _screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2.0f;
	worldPos.y -= getWindowHeight() / 2.0f;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

glm::vec2 EightBallPool::WorldToScreen(glm::vec2 _worldPos)
{
	glm::vec2 screenPos = _worldPos;


	screenPos.x /= 2.0f;
	screenPos.x /= m_extents;
	screenPos.x *= getWindowWidth();
	
	screenPos.y /= 2.0f;
	screenPos.y /= m_extents;
	screenPos.y *= (m_aspectRatio * getWindowHeight());


	screenPos.x += getWindowWidth() / 2.0f;
	screenPos.y += getWindowHeight() / 2.0f;


	return screenPos;
}