#pragma once
#include "Application2D.h"
#include "Input.h"
#include "Renderer2D.h"


class Input;
class Application2D;
class  IGameState
{
public:
    IGameState(Application2D* _app)
    {
        m_app = _app;
        m_2dRenderer = m_app->GetRenderer();
        m_input = aie::Input::getInstance();
    }

    virtual ~IGameState() = default;

    virtual void Load(){}
    virtual void UnLoad(){}

    virtual void Update(float _dt){}
    virtual void Draw(){ }

    void Reload(){UnLoad();Load();}
    

    
    
    
protected:
    aie::Renderer2D*	m_2dRenderer;
    Application2D* m_app;
    aie::Input* m_input;
private:

    
    
};
