#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <glm/ext.hpp>

#include "Circle.h"
#include <string>
#include <Texture.h>
#include "Player.h"
#include "Input.h"


enum class GamePhase
{
	START,
	PLAY,
	FOUL,
	OVER
};
class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


protected:
	void ShootBall(aie::Input* _input, glm::vec2 _mousWorldPos);
	void PlaceCueBall(glm::vec2 _extents, glm::vec2 _mousWorldPos, float _xOrigin);
	void CallFoul();
	float ExponentialEaseIn(float _time, float _start, float _end);
	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);
	glm::vec2 WorldToScreen(glm::vec2 _screenPos);
	


	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;


	PhysicsScene* m_physicsScene;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	float m_powerMax;
	float m_power;
	bool m_increasePower;

	PoolBall* m_cueBall;
	glm::vec2 m_cueBallStartPos;
	bool m_cueBallPlaced;

	std::list<PoolBall*> m_balls;
	glm::vec2 m_lineEndPos;
	
	aie::Texture* m_tableTexture;

	Player* m_player1;
	Player* m_player2;
	Player* m_currentPlayer;
	bool m_ballHit;
	bool m_ballsStill;
	bool m_switchPlayer;
	GamePhase m_gamePhase;

	glm::vec2 m_tableExtents;
};
