#pragma once
#include "Rigidbody.h"

class Spring : public PhysicsObject
{
public:
    Spring(Rigidbody* _body1, Rigidbody* _body2,
        float _springCoefficient, float _damping, float _restLength, glm::vec4 _colour,
        glm::vec2 _contact1 = glm::vec2(0, 0), glm::vec2 _contact2 = glm::vec2(0, 0));
    ~Spring();
    
    void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;

    void Draw(float _alpha) override;

    float GetKineticEnergy() override { return false; }

    float GetEnergy() override { return false; }

    glm::vec2 GetContact1();
    glm::vec2 GetContact2();
protected:

    Rigidbody* m_body1 = nullptr;
    Rigidbody* m_body2 = nullptr;

    glm::vec2 m_contact1 = glm::vec2(0);
    glm::vec2 m_contact2 = glm::vec2(0);

    glm::vec4 m_colour = glm::vec4(1);

    float m_damping = 0;
    float m_restLength = 0;

    // the restoring force;
    float m_springCoefficient = 0;		

};
