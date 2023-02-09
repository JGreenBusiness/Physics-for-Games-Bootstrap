#pragma once
#include <functional>

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

	void ResolveCollision(Rigidbody* _otherActor, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr, float _pen = 0.0f);

	void ResetVelocity() { m_velocity = glm::vec2(0); }

	void CalculateSmoothedPosition(float _alpha);

	glm::vec2 ToWorldSmoothed(glm::vec2 _localPos);
	glm::vec2 ToWorld(glm::vec2 _localPos);

	// Getters
	float GetPotentialEnergy();
	float GetKineticEnergy() override;
	float GetEnergy() override;
	glm::vec2 GetPosition() const{ return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass;}
	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment;}
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }
	float GetAngularVelocity() { return m_angularVelocity; }
	bool IsKinematic() { return m_isKinematic; }
	
	// Setters
	void SetMass(float _mass) { m_mass = _mass; }
	void SetPosition(glm::vec2 _pos) { m_position = _pos; }
	void SetKinematic(bool _state) { m_isKinematic = _state; }

	void CalculateAxes();


	std::function<void(PhysicsObject*)> collisionCallback;
protected:
	glm::vec2 m_position = glm::vec2(0);
	glm::vec2 m_lastPosition = glm::vec2(0);
	glm::vec2 m_velocity = glm::vec2(0);

	float m_mass = 0;
	float m_orientation = 0;
	float m_angularVelocity = 0;
	float m_moment = 0;

	float m_lastOrientation = 0;

	glm::vec2 m_smoothedPosition = glm::vec2(0);
	glm::vec2 m_smoothedLocalX = glm::vec2(0);
	glm::vec2 m_smoothedLocalY = glm::vec2(0);

	glm::vec2 m_localX = glm::vec2(0);
	glm::vec2 m_localY = glm::vec2(0);

	float m_linearDrag =.3f;
	float m_angularDrag = .3f;

	bool m_isKinematic;

	
private:



};

