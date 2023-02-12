#pragma once
#include <map>
#include <list>
#include <functional>

class IGameState;
class GameStateManager
{
public:
    GameStateManager();
    ~GameStateManager();

    void Update(float _deltaTime);
    void Draw();

    void SetState(const char* _name, IGameState* _state);
    void PushState(const char* _name);
    void ReloadState(const char* _name);
    void PopState();
protected:
    std::map<const char*, IGameState*> m_states;
    std::list<IGameState*> m_stack;
    std::list < std::function<void()> > m_commands;
private:
    
};
