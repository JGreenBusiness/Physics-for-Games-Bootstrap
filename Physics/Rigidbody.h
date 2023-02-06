#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>

#include "PhysicsScene.h"

#define MIN_LINEAR_THRESHOLD 0.01f
#define MIN_ANGULAR_THRESHOLD 0.01f

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType _shapeID, glm::vec2 _position,
		glm::vec2 _velocity, float _orientation, float _mass);
	 ~Rigidbody();

	void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;
	void ApplyForce(glm::vec2 force, glm::vec2 _pos);
	void ApplyForce(Rigidbody* _otherActor, glm::vec2 _force, glm::vec2 _pos);

	void ResolveCollision(Rigidbody* _otherActor, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr);

	void ResetVelocity() { m_velocity = glm::vec2(0); }

	void CalculateSmoothedPosition(float _alpha);

	float GetPotentialEnergy();
	float GetKineticEnergy() override;
	float GetEnergy() override;
	glm::vec2 GetPosition() const{ return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetMoment() {return m_moment;}
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }
	float GetAngularVelocity() { return m_angularVelocity; }
	
	void SetMass(float _mass) { m_mass = _mass; }
	

protected:
	glm::vec2 m_position;
	glm::vec2 m_lastPosition;
	glm::vec2 m_velocity;

	float m_mass;
	float m_orientation;
	float m_angularVelocity = 0;
	float m_moment;

	float m_lastOrientation;

	glm::vec2 m_smoothedPosition;
	glm::vec2 m_smoothedLocalX;
	glm::vec2 m_smoothedLocalY;

	glm::vec2 m_localX;
	glm::vec2 m_localY;

	float m_linearDrag =.3f;
	float m_angularDrag = .3f;


	void CalculateAxes();
	
private:



};

