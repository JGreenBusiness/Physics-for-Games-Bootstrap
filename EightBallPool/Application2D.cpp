#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Box.h"
#include "Plane.h"
#include "Gizmos.h"
#include "Circle.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0));
	Plane* topPlane = new Plane(glm::vec2(0, 1), -36.0f, glm::vec4(1, 1, 1, 1));
	Plane* botPlane = new Plane(glm::vec2(0, -1), -36.0f, glm::vec4(1, 1, 1, 1));
	Plane* leftPlane = new Plane(glm::vec2(-1, 0), -64.5f, glm::vec4(1, 1, 1, 1));
	Plane* rightPlane = new Plane(glm::vec2(1, 0), -64.5f, glm::vec4(1, 1, 1, 1));
	
	m_cueBall = new Circle(glm::vec2(-30, 0), glm::vec2(0), .7f, 1.5f, glm::vec4(1, 1, 1, 1));

	const int RACK_SIZE = 6;
	Circle* balls[RACK_SIZE];
	float radius = 1.8f;
	for (int i = 1; i < RACK_SIZE; i++)
	{
		glm::vec2 ballpos = glm::vec2(-cos(60) * i * (radius*2), sin(60.218f) * i * (radius * 2) + radius);
		for (int j = 0; j < i; j++)
		{
			balls[i] = new Circle(glm::vec2(ballpos.x,ballpos.y + ((radius * 2) * j)), glm::vec2(0, 0), 0.7f, radius, glm::vec4(0, 1, 0, 1));
			m_physicsScene->AddActor(balls[i]);
		}
	}

	m_physicsScene->AddActor(m_cueBall);
	m_physicsScene->AddActor(topPlane);
	m_physicsScene->AddActor(botPlane);
	m_physicsScene->AddActor(leftPlane);
	m_physicsScene->AddActor(rightPlane);
	
	return true;
}

void Application2D::shutdown() {
	
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

		aie::Gizmos::add2DLine(m_cueBall->GetPosition(), m_cueBall->GetPosition() - (dir * glm::vec2(20)), glm::vec4(1));

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

	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));

	
	
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Welcome to pool!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
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


