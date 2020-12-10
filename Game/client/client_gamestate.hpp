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
    virtual void OnNest(GameState state);
    virtual void Subscribe(Observer *pObserver);
protected:
    std::list<Observer *> _Subscribers;
};

class Observer{
public:
    // virtual ~Observer();
    virtual void Update(GameState state) = 0;
    void SetSubject(Subject *pSubject){ _Subject = pSubject; }
protected:
    Subject *_Subject;
};

class GameStateController : public Subject{
public:
    void SetGameState(GameState state);
    GameState GetState(){ return _State; }
protected:
    GameState _State;
};

class GameStateOutputer : public Observer{
private:
    GameState _State, _PreState;
public:
    GameStateOutputer(GameState initState);
    virtual void Update(GameState state);
};
