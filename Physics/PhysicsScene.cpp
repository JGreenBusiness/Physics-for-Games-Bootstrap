#include "PhysicsScene.h"

#include "PhysicsObject.h"
#include "Circle.h"
#include "Plane.h"
#include "Demos.h"


glm::vec2 PhysicsScene::m_gravity = glm::vec2(0);
PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		if (pActor != nullptr)
		{
			delete pActor;
		}
	}
}

void PhysicsScene::AddActor(PhysicsObject* _actor)
{
	m_actors.push_back(_actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* _actor)
{
	m_actors.erase(std::find(m_actors.begin(), m_actors.end(), _actor));
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,
};

void PhysicsScene::Update(float _dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += _dt;
	while (accumulatedTime >= m_timeStep)
	{
		for (PhysicsObject* pActor : m_actors) 
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}

	// check for collisions (ideally you'd want to have some sort of 
	// scene management in place)
	int actorCount = m_actors.size();

	// need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1;outer++)
	{
		for (int inner = outer + 1; inner < actorCount;inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->GetShapeID();
			int shapeId2 = object2->GetShapeID();

			// using function pointer 
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
#ifndef SimulateRocket
				collisionFunctionPtr(object1, object2);
#endif // !SimulateRocket
			}
		}
	}
}

void PhysicsScene::Draw()
{
	for (PhysicsObject* pActor : m_actors)
	{
		pActor->Draw(1);
	}

}

bool PhysicsScene::Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	Circle* circle1 = dynamic_cast<Circle*>(_obj1);
	Circle* circle2 = dynamic_cast<Circle*>(_obj2);

	
	if (circle1 != nullptr && circle2 != nullptr)
	{
		// If distance of two spheres is less then sum of both sphere radius
		if (glm::distance(circle1->GetPosition(), circle2->GetPosition()) <=
			(circle1->GetRadius() + circle2->GetRadius()))
		{

			circle1->ResolveCollision(circle2, circle1->GetPosition() + circle2->GetPosition());
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	Circle* circle = dynamic_cast<Circle*>(_obj1);
	Plane* plane = dynamic_cast<Plane*>(_obj2);
	//if successful then test for collision
	if (circle != nullptr && plane != nullptr)
	{
		float sphereToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = circle->GetRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			glm::vec2 contact = circle->GetPosition() + (plane->GetNormal() * -circle->GetRadius());
			plane->ResolveCollision(circle,contact);
			return true;
		}
	}
	return false;
}
bool PhysicsScene::Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
	// reverse the order of arguments, as obj1 is the plane and obj2 is the Circle
	return Circle2Plane(_obj2, _obj1);
}

float PhysicsScene::GetTotalEnergy()
{
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->GetEnergy();
	}
	return total;
}




