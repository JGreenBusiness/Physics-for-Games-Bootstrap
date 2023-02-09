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
#include "Spring.h"
#include "SoftBody.h"

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
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents,
		-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));
	
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
	
#ifdef SimulateBox
	m_physicsScene->SetGravity(glm::vec2(0, -9.8));

	Box* box1 = new Box(glm::vec2(0), glm::vec2(0),90,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Box* box2 = new Box(glm::vec2(15,0), glm::vec2(0),30,4, glm::vec2(7,3),glm::vec4(0,0,1,1));
	Circle* ball1 = new Circle(glm::vec2(0, 15), glm::vec2(3, 0), 1.0f, 4, glm::vec4(0, 1, 0, 1));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -20, glm::vec4(1, 1, 1, 1));
	plane1->SetElastcity(0.3);

	Plane* plane2 = new Plane(glm::vec2(-1, 0), -40, glm::vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(1, 0), -40, glm::vec4(1, 1, 1, 1));
	
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);

#endif // SimulateBox
	
#ifdef Bounce2Stop
	m_physicsScene->SetGravity(glm::vec2(0, -9.8));

	Box* box1 = new Box(glm::vec2(0), glm::vec2(-6),45,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Circle* ball1 = new Circle(glm::vec2(0, 15), glm::vec2(0, 0), 1.0f, 4, glm::vec4(0, 1, 0, 1));

	Plane* plane1 = new Plane(glm::vec2(0, 1), -20, glm::vec4(1, 1, 1, 1));
	plane1->SetElastcity(0.3);

	Plane* plane2 = new Plane(glm::vec2(-1, 0), -40, glm::vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(glm::vec2(1, 0), -40, glm::vec4(1, 1, 1, 1));
	
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);

#endif // Bounce2Stop
		
#ifdef PoolTable

	m_physicsScene->SetGravity(glm::vec2(0));
	Plane* topPlane = new Plane(glm::vec2(0, 1), -36.0f, glm::vec4(1, 1, 1, 1));
	Plane* botPlane = new Plane(glm::vec2(0, -1), -36.0f, glm::vec4(1, 1, 1, 1));
	Plane* leftPlane = new Plane(glm::vec2(-1, 0), -64.5f, glm::vec4(1, 1, 1, 1));
	Plane* rightPlane = new Plane(glm::vec2(1, 0), -64.5f, glm::vec4(1, 1, 1, 1));
	
	Circle* ball1 = new Circle(glm::vec2(-30, 0), glm::vec2(80, 0), 1.5f, 1.5f, glm::vec4(1, 1, 1, 1));

	const int RACK_SIZE = 6;
	Circle* balls[RACK_SIZE];
	int dist = 4;
	for (int i = 0; i < RACK_SIZE; i++)
	{
		glm::vec2 ballpos = glm::vec2(-cos(60) * i * dist, sin(60) * i * dist);
		for (int j = 0; j < i; j++)
		{
			balls[i] = new Circle(glm::vec2(ballpos.x,ballpos.y + (dist * j)), glm::vec2(0, 0), 1.8f, 1.8, glm::vec4(0, 1, 0, 1));
			m_physicsScene->AddActor(balls[i]);
		}
	}

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(topPlane);
	m_physicsScene->AddActor(botPlane);
	m_physicsScene->AddActor(leftPlane);
	m_physicsScene->AddActor(rightPlane);

#endif // PoolTable
		
#ifdef SimulateContactForces

	m_physicsScene->SetGravity(glm::vec2(0,-15.0f));
	Plane* botPlane = new Plane(glm::vec2(0, 1), -50.0f, glm::vec4(1, 1, 1, 1));
	Box* box1 = new Box(glm::vec2(0, 50), glm::vec2(-6),45,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Box* box2 = new Box(glm::vec2(3,-20), glm::vec2(-6),45,4, glm::vec2(4,7),glm::vec4(1,0,0,1));


	m_physicsScene->AddActor(botPlane);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

#endif // SimulateContactForces

			
#ifdef BouncePads

	m_physicsScene->SetGravity(glm::vec2(0,-15.0f));
	Plane* botPlane = new Plane(glm::vec2(0, 1), -50.0f, glm::vec4(1, 1, 1, 1));
	Box* box1 = new Box(glm::vec2(0, -30), glm::vec2(0), 1.5708,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Box* box2 = new Box(glm::vec2(3,20), glm::vec2(0),0,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	box1->SetKinematic(true);
	box2->SetElastcity(.9);

	m_physicsScene->AddActor(botPlane);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

#endif // BouncePads
			
#ifdef PhysicsBuckets

	m_physicsScene->SetGravity(glm::vec2(0,-9.8));
	Plane* botPlane = new Plane(glm::vec2(0, 1), -50.0f, glm::vec4(1, 1, 1, 1));
	Box* box1 = new Box(glm::vec2(0, 30), glm::vec2(0,0), 1.9708f,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Box* box2 = new Box(glm::vec2(30, 30), glm::vec2(0,0), 1.9708f,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Box* box3 = new Box(glm::vec2(-30, 30), glm::vec2(0,0), 1.9708f,4, glm::vec2(4,7),glm::vec4(1,0,0,1));
	Circle* ball1 = new Circle(glm::vec2(0, 15), glm::vec2(3, 0), 1.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(15, 30), glm::vec2(3, 0), 1.0f, 4, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(-15, 30), glm::vec2(3, 0), 1.0f, 4, glm::vec4(0, 1, 0, 1));

	const int RACK_SIZE = 20;
	Circle* balls[RACK_SIZE];
	int dist = 8;
	glm::vec2 startPos = glm::vec2(-80, 0);

	for(int j = 0; j < 3; j++)
	{
		for (int i = 0; i < RACK_SIZE; i++)
		{
			if (i % 2 ==0)
			{
				balls[i] = new Circle(glm::vec2(startPos.x + (i * dist), startPos.y - (j*dist*2) ), glm::vec2(0), 1, 1, glm::vec4(1, 1, 1, 1));
			}
			else
			{
				balls[i] = new Circle(glm::vec2(startPos.x + (i * dist), -dist - (j*dist*2)), glm::vec2(0), 1, 1, glm::vec4(1, 1, 1, 1));
			}
			balls[i]->SetKinematic(true);
			//balls[i]->SetElastcity(.4f);
			m_physicsScene->AddActor(balls[i]);
			
			
		}
	}
	


	m_physicsScene->AddActor(botPlane);

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(box3);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	

#endif // PhysicsBuckets
			
#ifdef SimulateSprings

	m_physicsScene->SetGravity(glm::vec2(0,-9.8));
	Plane* botPlane = new Plane(glm::vec2(0, 1), -50.0f, glm::vec4(1, 1, 1, 1));
	
	Circle* staticBall = new Circle(glm::vec2(0,30),glm::vec2(0),2,3,glm::vec4(0,1,0,1));
	staticBall->SetKinematic(true);
	
	Circle* ball = new Circle(glm::vec2(0,15),glm::vec2(30,-30),2,3,glm::vec4(1,0,0,1));
	Spring* spring = new Spring(staticBall, ball, 3, .5, 3, glm::vec4(1, 1, 1, 1), staticBall->GetPosition(), ball->GetPosition());

	m_physicsScene->AddActor(botPlane);
	m_physicsScene->AddActor(staticBall);
	m_physicsScene->AddActor(ball);
	m_physicsScene->AddActor(spring);



#endif // SimulateSprings
			
#ifdef SimulateRope

	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* prev = nullptr;
	int num = 10;
	for (int i = 0; i < num; i++)
	{
		// spawn a circle to the right and below the previous one, so that the whole rope acts under gravity and swings

		Circle* circle = new Circle(glm::vec2(30 - i * 5,i * 3), glm::vec2(0), 10, 2, glm::vec4(1, 0, 0, 1));
		if (i == 0)
		{
			circle->SetKinematic(true);
			circle->SetElastcity(.1f);
		}

		m_physicsScene->AddActor(circle);

		if (prev)
		{
			m_physicsScene->AddActor(new Spring(circle, prev, 500, 10, 7, glm::vec4(1, 1, 1, 1),circle->GetPosition(),prev->GetPosition()));
		}

		prev = circle;

		
	}

	// add a kinematic box at an angle for the rope to drape over
	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 90, 4, glm::vec2(4, 4), glm::vec4(1, 0, 0, 1));
	box->SetKinematic(true);
	Box* box1 = new Box(glm::vec2(0, 50), glm::vec2(5,0), 90, 4, glm::vec2(8, 8), glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(box);
	m_physicsScene->AddActor(box1);
#endif // SimulateRope
			
#ifdef SimulateSoftBody
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	std::vector<std::string> J;
	J.push_back("0000..");
	J.push_back("0000..");
	J.push_back("..00..");
	J.push_back("..00..");
	J.push_back("000000");
	J.push_back("000000");

	std::vector<std::string> U;
	U.push_back(".0000.");
	U.push_back("000000");
	U.push_back("00..00");
	U.push_back("00..00");
	U.push_back("00..00");
	U.push_back("00..00");
	
	std::vector<std::string> S;
	S.push_back("000000");
	S.push_back("000000");
	S.push_back("..00..");
	S.push_back(".00...");
	S.push_back("000000");
	S.push_back("000000");
	
	std::vector<std::string> T;
	T.push_back("..00..");
	T.push_back("..00..");
	T.push_back("..00..");
	T.push_back("..00..");
	T.push_back("000000");
	T.push_back("000000");
	
	std::vector<std::string> I;
	I.push_back("000000");
	I.push_back("000000");
	I.push_back("..00..");
	I.push_back("..00..");
	I.push_back("000000");
	I.push_back("000000");
	
	std::vector<std::string> N;
	N.push_back("00..00");
	N.push_back("00..00");
	N.push_back("00..00");
	N.push_back("00..00");
	N.push_back("000000");
	N.push_back(".0000.");

	SoftBody::Build(m_physicsScene, glm::vec2(-105, 0), 50.0f, 10.0f, 6.0f, J);
	SoftBody::Build(m_physicsScene, glm::vec2(-70, 0), 50.0f, 10.0f, 6.0f, U);
	SoftBody::Build(m_physicsScene, glm::vec2(-35, 0), 50.0f, 10.0f, 6.0f, S);
	SoftBody::Build(m_physicsScene, glm::vec2(0, 0), 50.0f,10.0f, 6.0f, T);
	SoftBody::Build(m_physicsScene, glm::vec2(35, 0), 50.0f, 10.0f, 6.0f, I);
	SoftBody::Build(m_physicsScene, glm::vec2(70, 0), 50.0f, 10.0f, 6.0f, N);

	Plane* botPlane = new Plane(glm::vec2(0, 1), -50.0f, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(botPlane);

#endif // SimulateSoftBody

	
#ifdef InputTest

	m_box1 = new Box(glm::vec2(0, 0), glm::vec2(0), 90, 4, glm::vec2(40, 40), glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(m_box1);

#endif // InputTest

#ifdef ObjectTest

	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(20,0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	//ball2->SetKinematic(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30,glm::vec4(1, 1, 1, 1)));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50,glm::vec4(1, 1, 1, 1)));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50,glm::vec4(1, 1, 1, 1)));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(3, 0), 0.5f, 4, glm::vec2(8, 4), glm::vec4(1, 1, 0, 1)));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(3, 0), 0.5f, 4, glm::vec2(8, 4), glm::vec4(1, 0, 2, 1)));

	ball1->collisionCallback = [=](PhysicsObject* other) {
		if (other == ball2)
		{
			std::cout << "Howzat!!?" << std::endl;
		}
		return;
	};

	ball2->collisionCallback = std::bind(&PhysicsApp::OnBall2Check, this, std::placeholders::_1);
#endif // ObjectTest
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

#ifdef InputTest

	if (_input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		_input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

		if (m_box1->IsInside(worldPos))
		{
			aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(0, 0, 1, 1));
		}
	}
#endif // InputTest



		
}

float PhysicsApp::DegreeToRadian(float _degree)
{
	return _degree * (PI / 180.0f);
}

glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 _screenPos)
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

void PhysicsApp::OnBall2Check(PhysicsObject* _other)
{
		Plane* plane = dynamic_cast<Plane*>(_other);
		if (plane != nullptr)
			std::cout << "Pong!" << std::endl;
}
