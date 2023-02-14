#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <glm/ext.hpp>

#include "Circle.h"
#include <string>
#include <Texture.h>
#include "Player.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


protected:
	float ExponentialEaseIn(float _time, float _start, float _end);
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;


	PhysicsScene* m_physicsScene;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	float m_powerMax = 4.0f;
	float m_power = 0.0f;
	bool m_increasePower = true;

	PoolBall* m_cueBall;
	std::list<Circle*> m_balls;

	aie::Texture* m_tableTexture;

	Player* m_player1;
	Player* m_player2;
	Player* m_currentPlayer;
};
