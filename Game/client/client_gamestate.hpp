#pragma once
#include <stdio.h>
#include <list>
#include <event.h>

enum GameState{ 
    Init = 0,
    OnGame,
    Exiting,
    InputSetup,
    EnterPlayerName,
};

class Observer;

class Subject{
public:
    virtual ~Subject();
    virtual void OnNest();
    virtual void Subscribe(Observer *pObserver);
protected:
    std::list<Observer *> _Subscribers;
};

class Observer{
public:
    virtual ~Observer();
    virtual void Update() = 0;
    void SetSubject(Subject *pSubject){ _Subject = pSubject; }
protected:
    Subject *_Subject;
};

class GameStateController : public Subject{
public:
    void SetGameState(GameState state){ _State = state; }
    GameState GetState(){ return _State; }
protected:
    GameState _State;
};
