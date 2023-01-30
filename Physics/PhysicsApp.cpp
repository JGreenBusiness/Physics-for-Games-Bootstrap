#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Gizmos.h"
#include <glm/ext.hpp>
#include "Demos.h"
#include "Circle.h"

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
	ball2->ApplyForce(glm::vec2(30.0f, 3.0f));
#endif // NewtonsSecondLaw

#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball = new Circle(glm::vec2(4,0), glm::vec2(0, 0), 4.0f, 1, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-4,0), glm::vec2(0, 0), 4.0f, 1, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);

	ball->ApplyForce(ball2, glm::vec2(10, 0));
#endif // NewtonsThirdLaw
	
#ifdef SimulateCollision
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball = new Circle(glm::vec2(-20,0), glm::vec2(10, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10,0), glm::vec2(-10, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(ball2);

#endif // SimulateCollision


}

void PhysicsApp::DemoUpdate(aie::Input* _input, float _dt)
{
}

float PhysicsApp::DegreeToRadian(float _degree)
{
	return _degree * (PI / 180.0f);
}
