#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <glm/ext.hpp>

#include "Circle.h"
#include <string>


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

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;
	aie::Font*			m_font;


	PhysicsScene* m_physicsScene;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;

	float m_powerMax = 140.0f;
	float m_power = 0.0f;
	bool m_increasePower = true;

	Circle* m_cueBall;
};