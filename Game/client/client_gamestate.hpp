#pragma once
#include "stdio.h"

enum GameState{ 
    Init = 0,
    OnGame,
    Exiting,
    InputSetup,
    EnterPlayerName,
};

class GameStateController
{
private:
    GameState _State = GameState::Init;

public:
    GameStateController();
    ~GameStateController();

    GameState GetGameState();
    void SetGameState(GameState state);
};
