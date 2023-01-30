#include "PhysicsScene.h"

#include "PhysicsObject.h"
#include "Circle.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0);

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

			// for now we can assume both shapes are Circles, 
			// since that is all we’ve implemented for now.
			Circle2Circle(object1, object2);
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
	Circle* sphere1 = dynamic_cast<Circle*>(_obj1);
	Circle* sphere2 = dynamic_cast<Circle*>(_obj2);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		// If distance of two spheres is less then sum of both sphere radius
		return glm::distance(sphere1->GetPosition(), sphere2->GetPosition()) <=
			(sphere1->GetRadius() + sphere2->GetRadius());
	}
}


