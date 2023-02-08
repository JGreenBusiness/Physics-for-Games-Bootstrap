#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include <vector>
#include "Circle.h"
#include "Box.h"

class PhysicsScene;
class PhysicsObject;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	std::vector<PhysicsObject*> m_objects;

public:
	void DemoStartup(int _num);
	void DemoUpdate(aie::Input* _input, float _dt);
	float DegreeToRadian(float _degree);

	glm::vec2 ScreenToWorld(glm::vec2 _screenPos);

private:

	Circle* m_rocket;
	float m_fuelRate = 0;
	float m_MAX_FUEL_RATE = 0.1f;

	Circle* m_ball1;
	Box* m_box1;
	int m_energyLast = 0;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;
	
};