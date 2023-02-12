#pragma once
#include "IGameState.h"


class PlayState : public IGameState
{
public:
    PlayState(Application2D* _app);

    ~PlayState() override;

    void Load() override;
    void UnLoad() override;

    void Update(float _dt) override;
    void Draw() override;


protected:
private:
    aie::Font* m_font;

    float m_modifier = 50;
    float m_timer = 0;

};
