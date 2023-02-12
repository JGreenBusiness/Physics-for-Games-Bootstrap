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
	
	m_timer = 0;

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0));
	Plane* topPlane = new Plane(glm::vec2(0, 1), -36.0f, glm::vec4(1, 1, 1, 1));
	Plane* botPlane = new Plane(glm::vec2(0, -1), -36.0f, glm::vec4(1, 1, 1, 1));
	Plane* leftPlane = new Plane(glm::vec2(-1, 0), -64.5f, glm::vec4(1, 1, 1, 1));
	Plane* rightPlane = new Plane(glm::vec2(1, 0), -64.5f, glm::vec4(1, 1, 1, 1));
	
	m_cueBall = new Circle(glm::vec2(-30, 0), glm::vec2(0), .7f, 1.5f, glm::vec4(1, 1, 1, 1));

	const int RACK_SIZE = 6;
	Circle* balls[RACK_SIZE];
	int dist = 4;
	for (int i = 0; i < RACK_SIZE; i++)
	{
		glm::vec2 ballpos = glm::vec2(-cos(60) * i * dist, sin(60) * i * dist);
		for (int j = 0; j < i; j++)
		{
			balls[i] = new Circle(glm::vec2(ballpos.x,ballpos.y + (dist * j)), glm::vec2(0, 0), 0.7f, 1.8, glm::vec4(0, 1, 0, 1));
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

	m_timer += _deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	if(input->wasKeyPressed(aie::INPUT_KEY_SPACE))
	{
		m_cueBall->ApplyForce(glm::vec2(70.0f,0),glm::vec2(m_cueBall->GetPosition().x - m_cueBall->GetRadius(),m_cueBall->GetPosition().y));
	}

	aie::Gizmos::clear();
	m_physicsScene->Draw();

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
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
}