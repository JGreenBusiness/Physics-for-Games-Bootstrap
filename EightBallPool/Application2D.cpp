#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Box.h"
#include "Plane.h"
#include "Gizmos.h"
#include "Circle.h"
#include <iostream>
#include "PoolBall.h"

Application2D::Application2D() 
{
	

}

Application2D::~Application2D() 
{

}

bool Application2D::startup() {
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	m_tableTexture = new aie::Texture("./textures/table.png");

	m_powerMax = 4.0f;
	m_power = 0.0f;
	m_increasePower = true;
	m_lineEndPos = glm::vec2(0);
	m_switchPlayer = false;
	m_ballHit = false;

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

	m_cueBallStartPos = glm::vec2(-m_tableExtents.x / 1.8, 0);
	m_cueBall = new PoolBall(m_cueBallStartPos, .7f, 1.5f, 0);
	m_balls.push_back(m_cueBall);

	const int RACK_SIZE = 6;
	int ballOrder[15]{ 1,11,3,13,8,4,7,5,6,14,10,12,15,9,2};
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

	for(auto pos : ballPositions)
	{
		PoolBall* ball = new PoolBall(glm::vec2(pos.x, pos.y), .7f, radius, ballOrder[ballIndex]);
		ballIndex++;
		m_balls.push_back(ball);
		m_physicsScene->AddActor(ball);
	}



	//-----------------------------------------------------------
	
	// Ball Texture Assignment
	//-----------------------------------------------------------
	ballIndex = 0;
	for (auto ball : m_balls)
	{
		
		std::string fileName = "./textures/ball_";
		
		fileName += std::to_string(ball->GetBallNum());
		fileName += ".png";

		aie::Texture* ballTexture = new aie::Texture(fileName.c_str());
		ball->SetTexture(ballTexture);
		ballIndex++;
	}

	//-----------------------------------------------------------



	// Boundary and hole spawning logic
	//-----------------------------------------------------------
	float boxSize = radius;

	Box* boxes[6] = {};
	glm::vec4 boxColour = glm::vec4(0, 1, 0, .8f);
	float width = m_tableExtents.x * 2;
	float height = m_tableExtents.y * 2;

	Circle* holes[6] = {};
	int holeIndex = 0;
	float holeRadius = radius * 2;
	glm::vec4 holeColour = glm::vec4(0, 0, 0, .8f);

	// Sides
	for (int i = 0; i < 2; i++)
	{
		float dist = i * width;
		int index = i + 2;
		boxes[index] = new Box(glm::vec2(-m_tableExtents.x + dist, 0), glm::vec2(0), 0, 1, glm::vec2(boxSize, m_tableExtents.y - boxSize*4), boxColour);

		Circle* boxCap;
		for (int j = 0; j < 2; j++)
		{
			float h;
			float s;
			j % 2 == 0 ? h = -m_tableExtents.y : h = m_tableExtents.y;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCap = new Circle(glm::vec2(-m_tableExtents.x + dist, h - s ), glm::vec2(0), 1, boxSize, boxColour);
			boxCap->SetKinematic(true);
			m_physicsScene->AddActor(boxCap);



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
		boxes[index] = new Box(glm::vec2(m_tableExtents.x/2 - dist, m_tableExtents.y), glm::vec2(0), 0, 1, glm::vec2(m_tableExtents.x/2 - boxSize * 4, boxSize), boxColour);

		Circle* boxCap;
		for (int j = 0; j < 2; j++)
		{
			float w;
			float s;
			j % 2 == 0 ? w = -m_tableExtents.x/2 : w = m_tableExtents.x/2;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCap = new Circle(glm::vec2(m_tableExtents.x / 2 + w - s - dist, m_tableExtents.y), glm::vec2(0), 1, boxSize, boxColour);
			boxCap->SetKinematic(true);
			m_physicsScene->AddActor(boxCap);

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

		boxes[i] = new Box(glm::vec2(m_tableExtents.x/2 - dist, -m_tableExtents.y), glm::vec2(0), 0, 1, glm::vec2(m_tableExtents.x/2 - boxSize * 4, boxSize), boxColour);

		Circle* boxCap;
		for (int j = 0; j < 2; j++)
		{
			float w;
			float s;
			j % 2 == 0 ? w = -m_tableExtents.x/2 : w = m_tableExtents.x/2;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCap = new Circle(glm::vec2(m_tableExtents.x / 2 + w - s - dist, -m_tableExtents.y), glm::vec2(0), 1, boxSize, boxColour);
			boxCap->SetKinematic(true);
			m_physicsScene->AddActor(boxCap);

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

	// Hole logic
	//-----------------------------------------------------------

	for (int i = 0; i < 6; i++)
	{
		holes[i]->SetIsTrigger(true);

		holes[i]->triggerEnter = [=](PhysicsObject* _other)
		{
			PoolBall* ball = dynamic_cast<PoolBall*>(_other);

			if (ball->GetType() == BallType::BLACKBALL && m_currentPlayer->GetBallsSunk() != 7)
			{
				m_gamePhase = GamePhase::OVER;
				m_currentPlayer = m_currentPlayer->GetOpponent();
			}
			
			

			// Player Has not been assigned a poolball type
			if (!m_currentPlayer->OwnsBallType())
			{
				if (ball == m_cueBall)
				{
					m_gamePhase = GamePhase::START;
					m_switchPlayer = true;
					m_cueBall->ResetVelocity();
					return;
				}
				else
				{
					m_currentPlayer->SetOwnedBallType(ball->GetType());
					m_currentPlayer->AddSunkBall();
				}
			}// Both players have an owned type
			else
			{
				if (ball == m_cueBall)
				{
					CallFoul();
				}
				else
				{
					// Did current player sink their ball, if not other player's ball must have been sunk
					m_currentPlayer->GetOwnedBallType() == ball->GetType() ? m_currentPlayer->AddSunkBall() : m_currentPlayer->GetOpponent()->AddSunkBall();
				}
				
			}

			
			
			ball->SetPosition(glm::vec2(-m_tableExtents.x - ball->GetRadius() * 4, (ball->GetRadius()*2)* (m_player1->GetBallsSunk() + m_player2->GetBallsSunk())));
			ball->ResetVelocity();
			
		};
		
		m_physicsScene->AddActor(holes[i]);
	}
	//-----------------------------------------------------------
	
	// CueBallCollision
	//-----------------------------------------------------------
	m_cueBall->collisionCallback = [=](PhysicsObject* _other)
	{
		if ( _other->GetShapeID() == ShapeType::CIRCLE && !m_ballHit)
		{
			Rigidbody* rB = dynamic_cast<Rigidbody*>(_other);

			if (!rB->IsKinematic())
			{
				PoolBall* ball = dynamic_cast<PoolBall*>(rB);

				if (m_currentPlayer->GetOwnedBallType() != ball->GetType() && m_currentPlayer->GetOwnedBallType() != BallType::UNOWNED)
				{
					CallFoul();
				}

				m_ballHit = true;
			}
		}
		
	};

	m_physicsScene->AddActor(m_cueBall);

	//-----------------------------------------------------------
	return true;
}

void Application2D::shutdown() 
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

	for (auto ball : m_balls)
	{
		if (ball->GetTexture())
		{
			delete ball->GetTexture();
			ball->SetTexture(nullptr);
		}
	}
}

bool poolBallMoving(PoolBall* pB)
{
	return (pB->GetVelocity() != glm::vec2(0));
}
void Application2D::update(float _deltaTime) {

	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	m_physicsScene->Draw();

	// Gets players mouse pos and translates from screen to world pos
	int xScreen, yScreen;
	input->getMouseXY(&xScreen, &yScreen);
	glm::vec2 mouseWPos = ScreenToWorld(glm::vec2(xScreen, yScreen));


	auto movingBall =  std::find_if(m_balls.begin(),m_balls.end(),poolBallMoving);
	m_ballsStill = movingBall ==  m_balls.end() && !poolBallMoving(m_balls.back());
	
	// Start of game logic
	
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
			
			if (!m_cueBallPlaced)
			{
				PlaceCueBall(glm::vec2(0, m_tableExtents.y), mouseWPos,m_cueBallStartPos.x - m_cueBall->GetRadius()*2);

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
			// Switch Current Player
			if (m_switchPlayer && m_cueBallPlaced)
			{
				if (m_ballHit == false)
				{
					m_currentPlayer->OwnsBallType() ?  CallFoul(): m_gamePhase = GamePhase::START, 	m_cueBallPlaced = false;
;
				}
				
				m_currentPlayer = m_currentPlayer->GetOpponent();
				m_switchPlayer = false;
				m_ballHit = false;
			}

			ShootBall(input, mouseWPos);
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
			
			PlaceCueBall(glm::vec2(m_tableExtents.x, m_tableExtents.y), mouseWPos, 0);

			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT) && m_cueBall->GetActive())
			{
				m_gamePhase = GamePhase::PLAY;
				m_cueBallPlaced = true;
			}
			break;
			//-----------------------------------------------------------	
		default: ;
			std::cout << "ERROR: GamePhaseUnknown" << std::endl;
		}
	}
	
	m_physicsScene->Update(_deltaTime);

}
void Application2D::CallFoul()
{
	m_gamePhase = GamePhase::FOUL;
	m_switchPlayer = true;
}

/// <summary>Takes player input to shoot pool ball</summary><
void Application2D::ShootBall(aie::Input* _input,glm::vec2 _mousWorldPos)
{
	// Logic for power and distence from ball to mouse
	glm::vec2 dir = glm::normalize(_mousWorldPos - m_cueBall->GetPosition());
	float dist = glm::distance(m_cueBall->GetPosition(), _mousWorldPos);
	float distCap = 50;
	dist > distCap ? m_power = distCap * 2, dist = distCap : m_power = dist * 2;

	m_lineEndPos = m_cueBall->GetPosition() - (dir * glm::vec2(dist));
	m_lineEndPos = WorldToScreen(m_lineEndPos);

	// Player shot ball
	if (_input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT) && m_cueBall->GetActive())
	{
		glm::dot(_mousWorldPos, m_cueBall->GetPosition());
		m_cueBall->ApplyForce(-dir * (m_power), dir * (m_cueBall->GetRadius()));
		m_ballHit = false;
		m_switchPlayer = true;
	}
}

void Application2D::PlaceCueBall(glm::vec2 _extents, glm::vec2 _mousWorldPos, float _xOrigin)
{
	//Logic for cue ball placing at start
	glm::vec2 pos = glm::vec2(_mousWorldPos.x, _mousWorldPos.y);

	float top = _extents.y - m_cueBall->GetRadius() * 2;
	float bot = -_extents.y + m_cueBall->GetRadius() * 2;
	float sL = _extents.x - m_cueBall->GetRadius() * 2;
	float sR = -_extents.x + m_cueBall->GetRadius() * 2;

	// position cant exceed extents
	pos.y > top ? pos.y = top :
		pos.y < bot ? pos.y = bot
		: pos.y = pos.y;

	pos.x < sR ? pos.x = sR :
		pos.x > sL ? pos.x = sL
		: pos.x = pos.x;


	pos.x += _xOrigin;

	bool insideBall = false;
	for (auto ball : m_balls)
	{
 		if (ball != m_cueBall) 
		{
			glm::vec2 dir = glm::normalize(ball->GetPosition() - pos);
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

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	m_2dRenderer->drawSprite(m_tableTexture, getWindowWidth()/2 - 5.5f, getWindowHeight()/2, (getWindowWidth() / m_aspectRatio) * 1.5, (getWindowHeight() / m_aspectRatio)*1.5, 3.14159265, 1);
	
	if (m_gamePhase == GamePhase::OVER)
	{
		std::string winner = "Winner :";
		m_currentPlayer == m_player1 ? winner += "Player 1" : winner += "Player 2";
		m_2dRenderer->drawText(m_font, winner.c_str(), getWindowWidth() / 2, getWindowHeight() / 2);
	}
	else
	{
		int ballIndex = 0;
		for (auto ball : m_balls)
		{
			if (ball->GetActive())
			{
				glm::vec2 pos = WorldToScreen(ball->GetPosition());
				float scale = 13.0f;
				m_2dRenderer->drawSprite(ball->GetTexture(), pos.x, pos.y, ball->GetRadius() * scale, ball->GetRadius() * scale, 0, 1);
			}
		}

		if (m_ballsStill && m_gamePhase == GamePhase::PLAY)
		{
			glm::vec2 cueBallPos = WorldToScreen(m_cueBall->GetPosition());
			m_2dRenderer->drawLine(cueBallPos.x, cueBallPos.y, m_lineEndPos.x, m_lineEndPos.y, 1);
		}

		std::string turnText;
		m_currentPlayer == m_player1 ? turnText = "Player 1" : turnText = "Player 2";
		turnText += "'s Turn";
		m_2dRenderer->drawText(m_font, turnText.c_str(), 0, 16);

		std::string p1Text = "Player 1: ";
		std::string p2Text = "Player 2: ";


		if (m_currentPlayer->OwnsBallType())
		{
			m_player1->GetOwnedBallType() == BallType::STRIPED ? p1Text += "Striped " : p1Text += "Solid ";
			m_player2->GetOwnedBallType() == BallType::STRIPED ? p2Text += "Striped " : p2Text += "Solid ";

			p1Text += std::to_string(m_player1->GetBallsSunk());
			p2Text += std::to_string(m_player2->GetBallsSunk());
		}
		m_2dRenderer->drawText(m_font, p1Text.c_str(), 0, getWindowHeight() - 32);
		m_2dRenderer->drawText(m_font, p2Text.c_str(), getWindowWidth() / 2, getWindowHeight() - 32);
	}
	
	

	

	// done drawing sprites
	m_2dRenderer->end();
	//aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));


}

float Application2D::ExponentialEaseIn(float _time, float _start, float _end)
{
	if (_time <= 0.0f) return _start;
	else if (_time >= 1.0f) return _end;

	return _end * powf(2.0f, 10.0f * (_time - 1.0f)) + _start;
}

glm::vec2 Application2D::ScreenToWorld(glm::vec2 _screenPos)
{
	glm::vec2 worldPos = _screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

glm::vec2 Application2D::WorldToScreen(glm::vec2 _worldPos)
{
	glm::vec2 screenPos = _worldPos;


	screenPos.x /= 2.0f;
	screenPos.x /= m_extents;
	screenPos.x *= getWindowWidth();
	
	screenPos.y /= 2.0f;
	screenPos.y /= m_extents;
	screenPos.y *= (m_aspectRatio * getWindowHeight());


	screenPos.x += getWindowWidth() / 2;
	screenPos.y += getWindowHeight() / 2;


	return screenPos;
}



