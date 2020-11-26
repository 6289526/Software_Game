#include "client_common.h"


// ===== * ===== EventHandler ===== * ===== //
void TestFunc(GameState state) {
    fprintf(stderr, "state type: %d\n", state);
}

void GameStateEventHandler::BindEvent(GameStateEventSource *source){
    __hook(&GameStateEventSource::OnChangeGameState, source, TestFunc);
}

void GameStateEventHandler::RemoveEvent(GameStateEventSource *source){
    __unhook(&GameStateEventSource::OnChangeGameState, source, TestFunc);
}

GameStateEventHandler::GameStateEventHandler(GameStateEventSource *source){
    BindEvent(source);
}

GameStateEventHandler::~GameStateEventHandler(){
    RemoveEvent(_Source);
}
// ===== * ===== EventController ===== * ===== //
GameStateController::GameStateController(){
    _Source = new GameStateEventSource();
    _Handler = new GameStateEventHandler(_Source);
}

GameStateController::~GameStateController(){
    delete _Handler;
    delete _Source;
}

GameState GameStateController::GetGameState(){
    return _State;
}

void GameStateController::SetGameState(GameState state){
    _State = state;
}
