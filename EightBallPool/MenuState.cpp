#include "MenuState.h"

#include <iostream>

#include "Font.h"
#include "Input.h"
#include "PlayState.h"

MenuState::MenuState(Application2D* _app) : IGameState(_app)
{


}

MenuState::~MenuState()
{


}

void MenuState::Load()
{
    //std::cout << "Loading Menu" << std::endl;
    
}

void MenuState::UnLoad()
{
    //std::cout << "Unloading Menu" << std::endl;
}

void MenuState::Update(float _dt)
{
    aie::Input* input = aie::Input::getInstance();

    if (input->isKeyDown(aie::INPUT_KEY_SPACE))
    {
        m_app->GetGameStateManager()->PopState();
        m_app->GetGameStateManager()->PushState("Play");
        m_app->GetGameStateManager()->ReloadState("Play");
    }

        

}

void MenuState::Draw()
{
    m_2dRenderer->drawText(m_font, "Menu", m_app->getWindowWidth() / 2, m_app->getWindowHeight() / 2, 1);
}
