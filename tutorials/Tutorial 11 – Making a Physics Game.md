# Tutorial – Making a Physics Game

## Introduction and Objective:
At this point we have a working physics engine that can simulate Planes, Circles, Boxes and Springs. As game programmers, we should use these features to make a minigame of some kind, both to submit as an assessment but also as a potential portfolio piece.
This is a good chance to demonstrate a working C++ project for your portfolio.
If you provide a scoring system of some kind (and clear instructions and a retry loop), you’re almost guaranteed that anyone looking at it will spend longer playing it and form a good impression of you and your work.

## Prerequisites:
We will be using the DIY Physics engine that you have been writing.
Ensure you have completed the following tutorials:
- ***Fixed Timestep Tutorial. Available in the Introduction to Physics session.***
- ***Linear Force and Momentum Tutorial. Available in the Linear Force and Momentum session.***
- ***Collision Detection Tutorial. Available in the Collision Detection session.***
- ***Collision Resolution: Introduction Tutorial. Available in the Collision Resolution session.***
- ***Collision Resolution: Circle-to-Circle Tutorial. Available in the Collision Resolution session.***
- ***Collision Resolution: Circle-to-Plane Tutorial. Available in the Collision Resolution session.***
- ***Rotational Force, part 1. Available in the Rotational Force session.***
- ***Rotational Force, part 2. Available in the Rotational Force session.***
- ***Collision Resolution: Static and Dynamic Friction. Available in the Collision Resolution session.***
- ***Collision Resolution: Contact Forces. Available in the Contact Forces session.***
- ***Static and Dynamic Rigid Bodies. Available in the Static Rigid Bodies session.***
- ***Springs. Available in the Joints and Springs session.***

Although recommended, it is not necessary to have completed the following previous tutorials:
- ***Projectile Physics part 1: Analytical Solution. Available in the Projectile Physics session.***
- ***Projectile Physics part 2: Numerical Integration. Available in the Projectile Physics session.***
- ***Softbodies. Available in the Softbodes session.***

---

## Keyboard Input
You can derive classes from Box or Circle which has additional fixedUpdate code used to read the keyboard and move the object directly, or apply a force to it or something using the aie::input class.

## Mouse Input
To use mouse input we need a way of converting the mouse screen position to world space.
The conversion from world space to screen space happens in the glm::ortho call in your application in PhysicsApp::draw()
``` c++
// Set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
```

Our screen goes from -100 to 100 in world coordinates in X, and -56.25 to 56.25 ( 100/aspectRatio) in y.

We can declare these hardcoded variables as constants in our PhysicsApp file
``` c++
const float m_extents = 100;
const float m_aspectRatio = 16.0f / 9.0f;
```

And use them in our draw function:
``` c++
aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));
```

We can then use these same constants in a function of PhysicsApp to convert from screen coordinates to world coordinates.
``` c++
glm::vec2 PhysicsApp::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}
```

You can test this by adding a few lines in PhysicsApp::update to draw a circle at the mouse position when the mouse is held down.
``` c++
    if (input->isMouseButtonDown(0))
	{
		int xScreen, yScreen;
		input->getMouseXY(&xScreen, &yScreen);
		glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));

		aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(0, 0, 1, 1));
	}
```

This provides a powerful basis for gameplay. You can add a virtual IsInside(glm::vec2 worldPos) function to PhysicsObject (with a default implemantation that returns false) and override it for Circle and Box.
``` c++
bool Circle::IsInside(glm::vec2 point)
{
	return glm::distance(point, m_position) <= m_radius;
}
```
(Box is doable but a little bit trickier. You will need to convert the world position to box space, if you remember we’ve already done in some of the collision code, so you can go back and compare the result to the extents.) 

## Extension Activity 1: How to take the Mouse further

You could then write a function in PhysicsScene to determine the object under the mouse by iterating over all objects and calling IsInside(). This can be used for a number of purposes:

- Clicking on an object makes it kinematic, and its position follows the mouse until released
- Clicking on an object makes it the current object. The current object experiences a force pulling it towards the current mouse position as long as it’s held down. This should allow you to throw Rigidbodies with the mouse.
- Clicking on an object starts up a pool cue/whacking stick and sets its start point. While the mouse is held down, draw a line from the start point to the current mouse position. When the mouse is released, apply a force to any object under either the start or end point based on the vector from start to end. (Try both and see which feels better)
- Add an implementation of IsInside() for springs that detects if the point is close to the spring within a fixed tolerance. Make a puzzle game where you cut springs (or even drag new springs between selected objects) to get a ball into a goal.

---

## Adding gameplay events
You may well want a callback function in your Application code when two particular bodies collide, so that you can keep score, change object states, and so on.

We’ll often want to call member functions of the Application class when this happens, so the STL libraries have some useful code to help us here. 

Add a public collisionCallback function pointer to Rigidbody. This will get called (if its set) whenever the body collides with another body or plane.

``` c++
std::function<void(PhysicsObject*)> collisionCallback;
```

(You’ll need to include *< functional >* at the top of your .h file for this.) 

We now need to call this in Rigidbody::ResolveCollision for both bodies, passing through the other body that we’ve encountered.
``` c++
if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / GetMoment());
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		float kePre = GetKineticEnergy() + actor2->GetKineticEnergy();

		//Apply equal and opposite forces
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);

		if (collisionCallback != nullptr) 
			collisionCallback(actor2);
		if (actor2->collisionCallback)
			actor2->collisionCallback(this);

		float kePost = GetKineticEnergy() + actor2->GetKineticEnergy();

		float deltaKE = kePost - kePre;
		if (deltaKE > kePost * 0.01f)
			std::cout << "Kinetic Energy discrepancy greater than 1% detected!!";

		if (pen > 0)
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}
```

And also in Plane::ResolveCollision for the Rigidbody involved, passing the Plane through as its collision partner this time.
``` c++
actor2->ApplyForce(force, contact - actor2->GetPosition());

if (actor2->collisionCallback)
	actor2->collisionCallback(this);

float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
```

We can then attach either member functions or Lambda functions to this pointer in the Application class.

Take the following simple ObjectTest function which I put together when testing basic object collision (which I call from startup())
``` c++
void PhysicsApp::ObjectTest()
{
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	ball2->SetKinematic(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(3, 0), 0.5f, 4, 8, 4, glm::vec4(1, 1, 0, 1)));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(3, 0), 0.5f, 4, 8, 4, glm::vec4(1, 0, 2, 1)));
}
```

Say I want something to happen when the two balls collide with each other. I could attach a lambda function to ball1 to get this to happen.
``` c++
ball1->collisionCallback = [=](PhysicsObject* other) 
{
	if (other == ball2)
	{
		std::cout << "Howzat!!?" << std::endl;
	}
	return;
};
```

The global scope of the lambda ( [=] ) means I have access to the this pointer and any local variables like ball 2. Alternatively I could pass [this] as the scope and make ball2 a member variable. The choice is yours.

If we want to attack a member function to a collision callback we can use std::bind to do this. Here’s an example member function that will print a message when ball 2 collides with one of the Planes making up the walls of the simulation:
``` c++
void PhysicsApp::OnBall2Check(PhysicsObject* other)
{
	Plane* plane = dynamic_cast<Plane*>(other);
	if (plane != nullptr)
		std::cout << "Pong!" << std::endl;
}
```

A member function effectively has an extra parameter, the this pointer, which we specify when we do the bind. Explicit parameters of the function (the other PhysicsObject pointer in this case) require a placeholder parameter in the bind.

This is how we attach it in ObjectTest()
``` c++
void PhysicsApp::ObjectTest()
{
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, -20), glm::vec2(0), 4.0f, 4, vec4(0, 1, 0, 1));
	//ball2->SetKinematic(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(3, 0), 0.5f, 4, 8, 4, glm::vec4(1, 1, 0, 1)));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(3, 0), 0.5f, 4, 8, 4, glm::vec4(1, 0, 1, 1)));

	ball1->collisionCallback = [=](PhysicsObject* other) {
		if (other == ball2)
		{
			std::cout << "Howzat!!?" << std::endl;
		}
		return;
	};

	ball2->collisionCallback = std::bind(&PhysicsApp::OnBall2Check, this, std::placeholders::_1);
}
```
## Extension Activity 2: How to take Collision Callbacks further

If you want to, you could add another parameter to the collision callback, say a float for the magnitude of the force applied. You’d then need to add a std::placeholders::_2 to the std::bind call. 

---

## Triggers

You may well want to have triggers (ie PhysicsObjects that call a callback but don’t have a physical presence)

For example, the pockets in a pool game should notify the Application when a ball goes in, but not bounce the ball out again.

Add an m_isTrigger variable to Rigidbody, with appropriate accessors. 
``` c++
    std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerExit;

protected:
	…

	bool m_isTrigger;
```

Don’t forget to set this to false in the constructor!

In ResolveCollision, make sure that this is false on both bodies before applying any forces, or calling the collision callbacks.
``` c++
if (!m_isTrigger && !actor2->m_isTrigger)
{
	//Apply equal and opposite forces
	ApplyForce(-force, contact - m_position);
	actor2->ApplyForce(force, contact - actor2->m_position);

	if (collisionCallback != nullptr)
		collisionCallback(actor2);
	if (actor2->collisionCallback)
		actor2->collisionCallback(this);
}
```

Similarly, make sure we don’t apply any contact forces on Rigidbodies if either one is a trigger.
``` c++
void PhysicsScene::ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen)
{
	if ((body1 && body1->IsTrigger()) || (body2 && body2->IsTrigger()))
		return;
```

We can add triggerEnter and triggerEnter functions to our Rigidbody class now. At first thought, we just need to  call triggerEnter if the two bodies collide and one of them is a trigger. This is not quite what we want though. We only want the triggerEnter callback to activate on the first frame that they collide. We should maintain a list of all other bodies currently inside the trigger, and check that first before we activate the triggerEnter callback.
``` c++
bool m_isTrigger;
std::list<PhysicsObject*> m_objectsInside;

```

Add a function to Rigidbody to check when another body has entered it as a Trigger.
``` c++
void Rigidbody::TriggerEnter(PhysicsObject* actor2)
{
	if (m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), actor2) == m_objectsInside.end())
	{
		m_objectsInside.push_back(actor2);
		if (triggerEnter != nullptr)
			triggerEnter(actor2);
	}
}
```

Here we’re checking if the body is already in our list of objects inside, and only calling the trigger if it isn’t. (Read up on the documentation for std::find if it looks unfamiliar or confusing)

We can call this from ResolveCollision when we have a collision and one body is a trigger. We need to check both ways, of course.
``` c++
if (!m_isTrigger && !actor2->m_isTrigger)
{
	//Apply equal and opposite forces
	ApplyForce(-force, contact - m_position);
	actor2->ApplyForce(force, contact - actor2->m_position);

	if (collisionCallback != nullptr)
		collisionCallback(actor2);
	if (actor2->collisionCallback)
		actor2->collisionCallback(this);
	}
else
{
   	TriggerEnter(actor2);
	actor2->TriggerEnter(this);
}
```

All that’s left to do is remove objects from the list when they are no longer colliding. We don’t have a convenient point where the objects leave so we’ll take the following approach.

Add a list of objects that we have detected a collision with this frame.
``` c++
bool m_isTrigger;
std::list<PhysicsObject*> m_objectsInside;
std::list<PhysicsObject*> m_objectsInsideThisFrame;
```

We’ll clear this list every frame, and let ResolveCollision add to it immediately on being called, so we have a list of everything we’ve touched on a frame by frame basis.
``` c++
void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal /*= nullptr*/, float pen /*= 0*/)
{
	// register that these two objects have overlapped this frame
	m_objectsInsideThisFrame.push_back(actor2);
	actor2->m_objectsInsideThisFrame.push_back(this);

	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
		actor2->m_position - m_position);
```

In the fixedUpdate we can then check every object in our m_objectsInside list and make sure that it’s also in our m_objectsInsideThisFrame list. If its not, then we know that the object has just left our trigger, and we can remove it, and call the callback.
``` c++
void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	//store the local axes
	…

	// trigger checks
	if (m_isTrigger)
	{
		// check every object that is inside us and called triggerEnter on
		// if they haven’t registered inside us this frame, they must have exited
		// so remove them from our list and call triggerExit
		for (auto it = m_objectsInside.begin(); it != m_objectsInside.end(); it++)
		{
			if (std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it) == m_objectsInsideThisFrame.end())
			{
				if (triggerExit)
					triggerExit(*it);
				it = m_objectsInside.erase(it);
				if (it == m_objectsInside.end())
					break;
			}
		}
	}

	// clear this list now for next frame
	m_objectsInsideThisFrame.clear();

	if (m_isKinematic)
	{
		…
```

Note how we’ve called std::list.erase here. If we don’t reassign the return value to our iterator, we end up with a dangling iterator that crashes when we try to increment it.

Also, we have to provide the end-of-list check and break to avoid having an end() iterator which we try to increment at the end of the list. Keep this as reference for other projects that may use STL lists.

We should now test the triggers in a simple debuggable situation before using them in our minigame. Here’s my test code for doing so, which just prints our enter and exit messages with pointer values:
``` c++
void PhysicsApp::ObjectTest()
{
	m_physicsScene->SetGravity(glm::vec2(0, -9.82f));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	ball2->SetKinematic(true);
	ball2->SetTrigger(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(3, 0), 0.5f, 4, 8, 4, glm::vec4(1, 1, 0, 1)));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(3, 0), 0.5f, 4, 8, 4, glm::vec4(1, 0, 1, 1)));

	ball2->triggerEnter = [=](PhysicsObject* other) { std::cout << "Enter:" << other << std::endl; };
	ball2->triggerExit  = [=](PhysicsObject* other) { std::cout << "Exit:"  << other << std::endl; };
}
```

# Assessment Task 1: 
Using the Custom Physics Engine you have made so far, implement a working game of [8-Ball Pool](https://en.wikipedia.org/wiki/Eight-ball), which follows the rules of the game.