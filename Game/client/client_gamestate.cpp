#include "client_common.h"

// ===== * ===== EventHandler ===== * ===== //
void TestFunc(GameState state) {
    fprintf(stderr, "state type: %d\n", state);
}



// ===== * ===== Subjects ===== * ===== //
Subject::~Subject(){
    std::list<Observer*>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->SetSubject(NULL);
    }
}
void Subject::OnNest(){
    std::list<Observer *>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->Update();
    }
}
void Subject::Subscribe(Observer *observer){
    _Subscribers.push_back(observer);
    observer->SetSubject(this);
}
// ===== * ===== Subjects ===== * ===== //
