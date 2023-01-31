#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"
#include <vector>
#include "Circle.h"

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

private:
	Circle* m_rocket;
	float m_fuelRate = 0;
	float m_MAX_FUEL_RATE = 0.1f;
};