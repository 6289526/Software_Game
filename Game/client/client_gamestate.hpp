#pragma once
#include <event.h>

enum GameState{ 
    Init = 0,
    OnGame,
    Exiting,
    InputSetup,
    EnterPlayerName,
};

// Eventの発行物を定義するクラス
class GameStateEventSource{
public:
    // ゲームステート変更時に発行するイベント
    event void OnChangeGameState(GameState state);

    GameStateEventSource();
    ~GameStateEventSource();
};

// Eventの購読を行うクラス
class GameStateEventHandler{
private:
    GameStateEventSource *_Source;

public:
    GameStateEventHandler(GameStateEventSource *source);
    ~GameStateEventHandler();
    void BindEvent(GameStateEventSource *source);
    void RemoveEvent(GameStateEventSource *source);
};

// Eventの発行を行うクラス
class GameStateController
{
private:
    GameState _State = GameState::Init;
    GameStateEventSource *_Source;
    GameStateEventHandler *_Hander;

public:
    GameStateController();
    ~GameStateController();

    GameState GetGameState();
    void SetGameState(GameState state);
};
