#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Box.h"
#include "Plane.h"
#include "Gizmos.h"
#include "Circle.h"
#include <iostream>
#include "PoolBall.h"
Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	m_tableTexture = new aie::Texture("./textures/table.png");

	m_player1 = new Player();
	m_player2 = new Player();
	m_player1->SetOpponent(m_player2);
	m_player2->SetOpponent(m_player1);

	m_currentPlayer = m_player1;

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0));
	
	m_cueBall = new PoolBall(glm::vec2(-30, 0), .7f, 1.5f,0);
	

	// Ball Spawning Logic 
	//-----------------------------------------------------------
	glm::vec2 tableExtents = glm::vec2(75.0f, 38.0f);

	const int RACK_SIZE = 6;
	PoolBall* rack[RACK_SIZE];
	float radius = 1.8f;
	int ballNum = 1;
	for (int i = 1; i < RACK_SIZE; i++)
	{
		glm::vec2 ballpos = glm::vec2(-cos(60) * i * (radius*2), sin(60.218f) * i * (radius * 2) + radius);
		for (int j = 0; j < i; j++)
		{
			rack[i] = new PoolBall(glm::vec2(ballpos.x, ballpos.y + ((radius * 2) * j)), .7f, radius, ballNum);
			m_balls.push_back(rack[i]);
			ballNum++;
			m_physicsScene->AddActor(rack[i]);
		}
	}
	//-----------------------------------------------------------

	// Boundary and hole spawning logic
	//-----------------------------------------------------------
	float boxSize = radius;

	Box* boxes[6];
	glm::vec4 boxColour = glm::vec4(0, 1, 0, .8f);
	float width = tableExtents.x * 2;
	float height = tableExtents.y * 2;

	Circle* holes[6];
	int holeIndex = 0;
	float holeRadius = radius * 2;
	glm::vec4 holeColour = glm::vec4(0, 0, 0, .8f);

	// Sides
	for (int i = 0; i < 2; i++)
	{
		float dist = i * width;
		int index = i + 2;
		boxes[index] = new Box(glm::vec2(-tableExtents.x + dist, 0), glm::vec2(0), 0, 1, glm::vec2(boxSize, tableExtents.y - boxSize*4), boxColour);

		Circle* boxCap;
		for (int j = 0; j < 2; j++)
		{
			float h;
			float s;
			j % 2 == 0 ? h = -tableExtents.y : h = tableExtents.y;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCap = new Circle(glm::vec2(-tableExtents.x + dist, h - s ), glm::vec2(0), 1, boxSize, boxColour);
			boxCap->SetKinematic(true);
			m_physicsScene->AddActor(boxCap);



			holes[holeIndex] = new Circle(glm::vec2(-tableExtents.x + dist, h), glm::vec2(0), 1, holeRadius, holeColour);
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
		boxes[index] = new Box(glm::vec2(tableExtents.x/2 - dist, tableExtents.y), glm::vec2(0), 0, 1, glm::vec2(tableExtents.x/2 - boxSize * 4, boxSize), boxColour);

		Circle* boxCap;
		for (int j = 0; j < 2; j++)
		{
			float w;
			float s;
			j % 2 == 0 ? w = -tableExtents.x/2 : w = tableExtents.x/2;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCap = new Circle(glm::vec2(tableExtents.x / 2 + w - s - dist, tableExtents.y), glm::vec2(0), 1, boxSize, boxColour);
			boxCap->SetKinematic(true);
			m_physicsScene->AddActor(boxCap);

			if (j < 1 && i ==0)
			{
				holes[holeIndex] = new Circle(glm::vec2(tableExtents.x / 2 + w - dist, tableExtents.y + holeRadius), glm::vec2(0), 1, holeRadius, holeColour);
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

		boxes[i] = new Box(glm::vec2(tableExtents.x/2 - dist, -tableExtents.y), glm::vec2(0), 0, 1, glm::vec2(tableExtents.x/2 - boxSize * 4, boxSize), boxColour);

		Circle* boxCap;
		for (int j = 0; j < 2; j++)
		{
			float w;
			float s;
			j % 2 == 0 ? w = -tableExtents.x/2 : w = tableExtents.x/2;
			j % 2 == 0 ? s = -boxSize * 4 : s = boxSize * 4;

			boxCap = new Circle(glm::vec2(tableExtents.x / 2 + w - s - dist, -tableExtents.y), glm::vec2(0), 1, boxSize, boxColour);
			boxCap->SetKinematic(true);
			m_physicsScene->AddActor(boxCap);

			if (j < 1 && i == 0)
			{
				holes[holeIndex] = new Circle(glm::vec2(tableExtents.x / 2 + w - dist, -tableExtents.y - holeRadius), glm::vec2(0), 1, holeRadius, holeColour);
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

			ball->SetKinematic(true);
			ball->ResetPosition();
			ball->SetActive(false);

			if (!m_currentPlayer->OwnsBallType())
			{
				if (ball->GetType() == BallType::CueBall || ball->GetType() == BallType::BlackBall)
					return;

				m_currentPlayer->SetOwnedBallType(ball->GetType());
				m_currentPlayer->AddSunkBall();
			}

			std::cout << "Balls sunk: " << m_currentPlayer->GetBallsSunk() << std::endl << "Owned BallType:" << (BallType)m_currentPlayer->GetOwnedBallType() << std::endl;
		};

		m_physicsScene->AddActor(holes[i]);
	}
	//-----------------------------------------------------------

	m_physicsScene->AddActor(m_cueBall);


	
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
}

void Application2D::update(float _deltaTime) {

	if (m_increasePower && m_power < m_powerMax)
	{
		m_power += ExponentialEaseIn(_deltaTime,0,m_powerMax*10);
	}
	else if(m_power >= 0)
	{
		m_increasePower = false;
		m_power -= ExponentialEaseIn(_deltaTime, 0, m_powerMax*10);

	}
	else
	{
		m_increasePower = true;
	}


	aie::Input* input = aie::Input::getInstance();

	

	aie::Gizmos::clear();
	m_physicsScene->Draw();

	aie::Gizmos::add2DCircle(glm::vec2(-m_extents, 0), m_powerMax*2, 25, glm::vec4(1, 1, 1, 0));
	aie::Gizmos::add2DCircle(glm::vec2(-m_extents, 0), m_power * 2, 25, glm::vec4(1, 1, 1, 1));

	
	if (m_cueBall->GetVelocity() == glm::vec2(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
		glm::vec2 dir = glm::normalize(worldPos - m_cueBall->GetPosition());

		float lineDist = 50;
		glm::vec2 endPos = m_cueBall->GetPosition() - (dir * glm::vec2(lineDist));
		aie::Gizmos::add2DLine(m_cueBall->GetPosition(),endPos, glm::vec4(1));

		// Code which moves a m_projection allong the projection line
		//for (int i = 1; i <= glm::distance(m_cueBall->GetPosition(), endPos) / (m_projection->GetRadius() * 2); i++)
		//{
		//		m_projection->SetPosition(m_cueBall->GetPosition() - (dir * glm::vec2((m_projection->GetRadius() * 2) * i)));
		//}

		if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
		{
			glm::dot(worldPos, m_cueBall->GetPosition());
			m_cueBall->ApplyForce(-dir * (m_power * 20), dir * (m_cueBall->GetRadius()));
		}
	}

	m_physicsScene->Update(_deltaTime);

}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	m_2dRenderer->drawSprite(m_tableTexture, getWindowWidth()/2 - 5.5f, getWindowHeight()/2, (getWindowWidth() / m_aspectRatio) * 1.5, (getWindowHeight() / m_aspectRatio)*1.5, 3.14159265, 1);

	


	
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Welcome to pool!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
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



