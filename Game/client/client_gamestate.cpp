#include "client_common.h"

using namespace GameState;

// ===== * ===== GameStateController ===== * ===== //
void GameStateController::SetGameState(GameState state){
    _PreState = _State;
    _State = state;
    OnNest(state);
}
// ===== * ===== Subjects ===== * ===== //
Subject::~Subject(){
    std::list<Observer*>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->SetSubject(NULL);
    }
}
void Subject::OnNest(GameState state){
    std::list<Observer *>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->Update(state);
    }
}
void Subject::Subscribe(Observer *observer){
    _Subscribers.push_back(observer);
    observer->SetSubject(this);
}
// ===== * ===== Observer ===== * ===== //
Observer::~Observer() { }
// ===== * ===== GameStateOutputer ===== * ===== //
GameStateOutputer::GameStateOutputer() { }
GameStateOutputer::~GameStateOutputer() { }
void GameStateOutputer::Update(GameState state){
    fprintf(stderr, "GameState: %d -> %d\n", ((GameStateController *)_Subject)->GetPreState(), ((GameStateController *)_Subject)->GetState());
} 
