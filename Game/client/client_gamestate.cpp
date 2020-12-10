#include "client_common.h"

// ===== * ===== GameStateController ===== * ===== //
void GameStateController::SetGameState(GameState state){
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
StateOutputer::StateOutputer(GameState initState){
    _PreState = initState;
    _State = initState;
}
void StateOutputer::Update(GameState state){
    _PreState = _State;
    _State = state;
    fprintf(stderr, "GameState: %d -> %d", _PreState, _State);
} 
