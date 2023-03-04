#pragma once

#include "Renderer2D.h"
#include "PhysicsScene.h"
#include <glm/ext.hpp>

#include "Circle.h"
#include <string>
#include <Texture.h>
#include "Player.h"
#include "Input.h"
#include "PhysicsApp.h"

enum class GamePhase
{
	START,
	PLAY,
	FOUL,
	OVER
};
class EightBallPool : public aie::Application 
{
public:

    EightBallPool();
    virtual ~EightBallPool();

    bool startup() override;
    void shutdown() override;

    void update(float deltaTime) override;
    void draw() override;


protected:

    //Helper Functions
	
    /// <summary>Waits on player input to push Cue Ball with force determined by distance from mouse to ball.</summary>
    void PrepareToShootBall(aie::Input* _input, glm::vec2 _mouseWorldPos);

    /// <summary>Sets Cue Ball's position to mouse pos if mouse is within extents.</summary>
    void PrepareToPlaceCueBall(glm::vec2 _extents, glm::vec2 _mouseWorldPos, float _xOrigin);
	
    /// <summary>Sets game phase to Foul and switchPlayer to true.</summary>
    void CallFoul();

    /// <summary>Logic for sinking all ball types, setting players owned type and switching player if player sinks opponents ball.</summary>
    void OnBallSunk(PoolBall* _sunkBall);

    /// <summary>Sets ballHit to true and calls a foul if the first ball is not owned by player.</summary>
    void OnCueBallCollide(PoolBall* _otherBall);
	
    glm::vec2 ScreenToWorld(glm::vec2 _screenPos);
    glm::vec2 WorldToScreen(glm::vec2 _screenPos);
	


    aie::Renderer2D*	m_2dRenderer;
    aie::Font*			m_font;


    PhysicsScene* m_physicsScene;

    const float m_extents = 100;
    const float m_aspectRatio = 16.0f / 9.0f;

    float m_powerMax;
    float m_power;
	
    glm::vec2 m_lineEndPos;
    glm::vec2 m_tableExtents;

    glm::vec2 m_cueBallStartPos;
	
    PoolBall* m_cueBall;

    std::vector<PoolBall*> m_balls;
	
    aie::Texture* m_tableTexture;

    Player* m_player1;
    Player* m_player2;
    Player* m_currentPlayer;

    bool m_gizmosOn;
    bool m_switchPlayer;
    bool m_cueBallPlaced;
    bool m_ballHit;
    bool m_ballSunk;
    bool m_ballsStill;
	
    GamePhase m_gamePhase;

    
};
