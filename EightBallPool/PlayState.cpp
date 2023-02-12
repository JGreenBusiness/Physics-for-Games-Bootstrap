#include "PlayState.h"
#include <iostream>


#include "Input.h"
#include <Gizmos.h>

;

PlayState::PlayState(Application2D* _app)  : IGameState(_app)
{
   

}

PlayState::~PlayState()
{
    
}

void PlayState::Load()
{
    aie::Gizmos::create(225U, 225U, 65535U, 65535U);
    m_physicsScene = new PhysicsScene();
    m_physicsScene->SetTimeStep(0.01);
}

void PlayState::UnLoad()
{
    aie::Gizmos::destroy();
    delete m_physicsScene;
    m_physicsScene = nullptr;
}

void PlayState::Update(float _dt)
{
    
    m_timer += _dt;
    
    if(m_input->wasKeyPressed(aie::INPUT_KEY_SPACE))
    {
        m_app->GetGameStateManager()->PopState();
        m_app->GetGameStateManager()->PushState("Menu");
    }

    aie::Gizmos::clear();

    m_physicsScene->Update(_dt);
    m_physicsScene->Draw();
    
}
void PlayState::Draw()
{
    m_2dRenderer->drawText(m_font,"Play", m_app->getWindowWidth()/2,m_app->getWindowHeight()/2, 1);

    int timer = m_timer;
    char timerChar[2 + sizeof(char)];
    timer = timer < 60 ? timer : (int)m_timer/60;
    std::sprintf(timerChar, "%d", timer);
    
    m_2dRenderer->drawText(m_font,timerChar, 20,20, 1);

}