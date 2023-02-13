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

	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0));

	

	float height = 38.0f;
	float width = 68.0f;

	Plane* topPlane = new Plane(glm::vec2(0, 1), -height, glm::vec4(1, 1, 1, 1));
	Plane* botPlane = new Plane(glm::vec2(0, -1), -height, glm::vec4(1, 1, 1, 1));
	Plane* leftPlane = new Plane(glm::vec2(-1, 0), -width, glm::vec4(1, 1, 1, 1));
	Plane* rightPlane = new Plane(glm::vec2(1, 0), -width, glm::vec4(1, 1, 1, 1));


	
	m_cueBall = new Circle(glm::vec2(-30, 0), glm::vec2(0), .7f, 1.5f, glm::vec4(1, 1, 1, 1));
	m_projection = new Circle(glm::vec2(0, 0), glm::vec2(0), .7f, 1.5f, glm::vec4(1, 1, 1, 0));
	m_projection->SetIsTrigger(true);
	

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

	float boxSize = radius;

	Box* boxes[6];
	boxes[0] = new Box(glm::vec2(-width, 0), glm::vec2(0), 0, 1, glm::vec2(boxSize, height - boxSize * 4), glm::vec4(0, 1, 0, .8f));			//Left
	boxes[1] = new Box(glm::vec2(width, 0), glm::vec2(0), 0, 1, glm::vec2(boxSize, height - boxSize * 4), glm::vec4(0, 1, 0, .8f));				//right
	boxes[2] = new Box(glm::vec2(width/2, -height), glm::vec2(0), 0, 1, glm::vec2(width/2 - boxSize * 4, boxSize), glm::vec4(0, 1, 0, .8f));	//bot1
	boxes[3] = new Box(glm::vec2(-width/2, -height), glm::vec2(0), 0, 1, glm::vec2(width/2 - boxSize * 4, boxSize), glm::vec4(0, 1, 0, .8f));	//bot2 
	boxes[4] = new Box(glm::vec2(width / 2, height), glm::vec2(0), 0, 1, glm::vec2(width/2 - boxSize * 4, boxSize), glm::vec4(0, 1, 0, .8f));	//top1
	boxes[5] = new Box(glm::vec2(-width / 2, height), glm::vec2(0), 0, 1, glm::vec2(width/2 - boxSize * 4, boxSize), glm::vec4(0, 1, 0, .8f));	//top2
	

	Circle* holes[6];
	Box* topBox1 = boxes[4]; Box* topBox2 = boxes[5]; Box* botBox1 = boxes[2]; Box* botBox2 = boxes[3];
	holes[0] = new Circle(glm::vec2(topBox1->GetPosition().x + topBox1->GetExtents().x + radius * 4, topBox1->GetPosition().y), glm::vec2(0, 0), 0.7f, radius*4, glm::vec4(.5f, 1, .5f, .7));				//holeTR
	holes[1] = new Circle(glm::vec2(topBox2->GetPosition().x + topBox2->GetExtents().x + radius * 4, topBox2->GetPosition().y + radius * 2), glm::vec2(0, 0), 0.7f, radius * 4, glm::vec4(.5f, 1, .5f, .7));//holeT 
	holes[2] = new Circle(glm::vec2(topBox2->GetPosition().x - topBox2->GetExtents().x - radius * 4, topBox2->GetPosition().y), glm::vec2(0, 0), 0.7f, radius * 4, glm::vec4(.5f, 1, .5f, .7));				//holeTL
	holes[3] = new Circle(glm::vec2(botBox1->GetPosition().x + botBox1->GetExtents().x + radius * 4, botBox1->GetPosition().y), glm::vec2(0, 0), 0.7f, radius * 4, glm::vec4(.5f, 1, .5f, .7));				//holeBR
	holes[4] = new Circle(glm::vec2(botBox2->GetPosition().x + botBox2->GetExtents().x + radius * 4, botBox2->GetPosition().y - radius * 2), glm::vec2(0, 0), 0.7f, radius * 4, glm::vec4(.5f, 1, .5f, .7));//holeB 
	holes[5] = new Circle(glm::vec2(botBox2->GetPosition().x - botBox2->GetExtents().x - radius * 4, botBox2->GetPosition().y), glm::vec2(0, 0), 0.7f, radius * 4, glm::vec4(.5f, 1, .5f, .7));				//holeBL 

	for (int i = 0; i < 6; i++)
	{
		holes[i]->SetIsTrigger(true);

		holes[i]->triggerEnter = [=](PhysicsObject* _other)
		{
			Circle* ball = dynamic_cast<Circle*>(_other);

			std::cout << "Enter:" << (ShapeType)_other->GetShapeID() << std::endl;

			ball->SetKinematic(true);
			ball->SetPosition(glm::vec2(100));
		};

		m_physicsScene->AddActor(holes[i]);

		boxes[i]->SetKinematic(true);
		m_physicsScene->AddActor(boxes[i]);
	}

	m_physicsScene->AddActor(m_cueBall);
	//m_physicsScene->AddActor(m_projection);
	//m_physicsScene->AddActor(topPlane);
	//m_physicsScene->AddActor(botPlane);
	//m_physicsScene->AddActor(leftPlane);
	//m_physicsScene->AddActor(rightPlane);

	
	return true;
}

void Application2D::shutdown() 
{
	delete m_tableTexture;
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
			m_cueBall->ApplyForce(-dir * (m_power * 20), m_cueBall->GetPosition() - (dir * glm::vec2(m_cueBall->GetRadius())));
		}
	}

	m_physicsScene->Update(_deltaTime);

}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	m_2dRenderer->drawSprite(m_tableTexture, getWindowWidth()/2, getWindowHeight()/2, (getWindowWidth() / m_aspectRatio) * 1.5, (getWindowHeight() / m_aspectRatio)*1.5, 3.14159265, 1);

	


	
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Welcome to pool!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
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



