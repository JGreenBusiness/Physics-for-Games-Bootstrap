#include "PhysicsApp.h"

#include <ostream>

#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Gizmos.h"
#include <glm/ext.hpp>
#include "Demos.h"
#include "glm/vec3.hpp"
#include "Plane.h"
#include <iostream>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() 
{
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01);



	glm::vec3 spherePos = glm::vec3(10, 5, 8);
	glm::vec3 planeNormal = glm::vec3(0.707, 0, 707);

	

	DemoStartup(1);

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float _deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(_deltaTime);
	m_physicsScene->Draw();

	DemoUpdate(input, _deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100.f, 100.f,
		-100.f / aspectRatio, 100.f / aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartup(int _num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif // NewtonsFirstLaw

#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0,-10));
	
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
	ball->ApplyForce(glm::vec2(30.0f, 3.0f));
#endif // NewtonsSecondLaw

#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball = new Circle(glm::vec2(4,0), glm::vec2(0, 0), 4.0f, 1, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-4,0), glm::vec2(0, 0), 4.0f, 1, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);

	ball->ApplyForce(ball2, glm::vec2(10, 0));
#endif // NewtonsThirdLaw
	
#ifdef SimulateC2CCollision
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Circle* ball = new Circle(glm::vec2(-20,0), glm::vec2(10, 15), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10,0), glm::vec2(-10, 15), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);

#endif // SimulateC2CCollision

#ifdef SimulateRocket
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	m_rocket = new Circle(glm::vec2(0,-30), glm::vec2(0), 40.0f, 15, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(m_rocket);

#endif // SimulateRocket

#ifdef CircleToPlaneCollision
	m_physicsScene->SetGravity(glm::vec2(0, -105.97f));

	Circle* ball0 = new Circle(glm::vec2(0, 50), glm::vec2(.5f,0), 4.0f, 4, glm::vec4(1, 1, 1, 1));
	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 10), glm::vec2(-3,0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(15, 30), glm::vec2(-3,0), 4.0f, 4, glm::vec4(1, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(3, 6), glm::vec2(-3,0), 4.0f, 4, glm::vec4(0, 0, 1, 1));
	Circle* ball5 = new Circle(glm::vec2(30, -6), glm::vec2(-3,0), 4.0f, 4, glm::vec4(0, 1, 1, 1));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -30, glm::vec4(1, 1, 1, 1));
	m_physicsScene->AddActor(ball0);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(ball5);
	m_physicsScene->AddActor(plane1);


#endif // CircleToPlane

#ifdef NewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball1 = new Circle(glm::vec2(-12, 0), glm::vec2(0,0), 1.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-4, 0), glm::vec2(0,0), 1.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(4, 0), glm::vec2(0,0), 1.0f, 4, glm::vec4(0, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(12, 0), glm::vec2(-8,0), 1.0f, 4, glm::vec4(1, 1, 0, 1));

	Plane* plane1 = new Plane(glm::vec2(1, 0), -20, glm::vec4(1, 1, 1, 1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -20, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);

#endif // NewtonsCradle

#ifdef AsymmetricalNewtonsCradle
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball1 = new Circle(glm::vec2(-12, 0), glm::vec2(0,0), 1.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-4, 0), glm::vec2(0,0), 1.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(4, 0), glm::vec2(0,0), 3.0f, 4, glm::vec4(0, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(12, 0), glm::vec2(-8,0), 1.0f, 4, glm::vec4(1, 1, 0, 1));

	Plane* plane1 = new Plane(glm::vec2(1, 0), -20, glm::vec4(1, 1, 1, 1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -20, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);

#endif // AsymmetricalNewtonsCradle

#ifdef KPEDiagnostic
	m_physicsScene->SetGravity(glm::vec2(0, -9.8));

	m_ball1 = new Circle(glm::vec2(-12, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(8, 0), glm::vec2(-6,0), 1.0f, 4, glm::vec4(0, 1, 0, 1));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -20, glm::vec4(1, 1, 1, 1));

	Plane* plane2 = new Plane(glm::vec2(-1, 0), -40, glm::vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(1, 0), -40, glm::vec4(1, 1, 1, 1));
	
	m_physicsScene->AddActor(m_ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);

#endif // KPEDiagnostic


}

void PhysicsApp::DemoUpdate(aie::Input* _input, float _dt)
{
#ifdef SimulateRocket
	if (m_rocket->GetMass() >= 0)
	{
		float fuelUse = 1.f;
		m_fuelRate -= _dt;

		if (m_fuelRate <= 0)
		{
			m_rocket->SetMass(m_rocket->GetMass() - fuelUse);

			Circle* fuelParticle = new Circle(m_rocket->GetPosition() + glm::vec2(0, -m_rocket->GetRadius()),
				glm::vec2(0), fuelUse, 1, glm::vec4(0, 1, 0, 1));

			m_physicsScene->AddActor(fuelParticle);
			m_rocket->ApplyForce(fuelParticle, glm::vec2(0, 30));

			m_fuelRate = m_MAX_FUEL_RATE;
	}
#endif // SimulateRocket

#ifdef KPEDiagnostic

	if((int)m_physicsScene->GetTotalEnergy() != m_energyLast)
	{
		
		std::cout << m_physicsScene->GetTotalEnergy()<<  std::endl;
		m_energyLast = (int)m_physicsScene->GetTotalEnergy();
	}

#endif // KPEDiagnostic

		
}

float PhysicsApp::DegreeToRadian(float _degree)
{
	return _degree * (PI / 180.0f);
}
