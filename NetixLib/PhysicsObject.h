#pragma once
#include <glm/vec2.hpp>

enum ShapeType 
{
	JOINT = -1,
	PLANE = 0,
	CIRCLE,
	BOX,
};


const int SHAPE_COUNT = 3;

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType _shapeID) : m_shapeID(_shapeID) { m_elasticity = 1; }

public:
	

	virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) = 0;
	virtual void Draw(float _alpha) = 0;
	virtual void ResetPosition() {};

	virtual float GetKineticEnergy() = 0;
	virtual float GetEnergy() = 0;
	virtual bool IsInside(glm::vec2 worldPos) { return false; }

	ShapeType GetShapeID() { return m_shapeID; }
	float GetElasticity() { return m_elasticity; }

	void SetElastcity(float _elasticity) { m_elasticity = _elasticity; }



protected:

	ShapeType m_shapeID;
	float m_elasticity;


};

