#pragma once

#include "IGameState.h"
#include "PlayState.h"

class MenuState : public IGameState
{
public:
    MenuState(Application2D* _app);

    ~MenuState() override;

    void Load() override;
    void UnLoad() override;

    void Update(float _dt) override;
    void Draw() override;

protected:
private:
    aie::Font* m_font;
};
