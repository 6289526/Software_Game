#include "client_common.h"

GameStateController::GameStateController(){

}

GameStateController::~GameStateController(){

}

GameState GameStateController::GetGameState(){
    return _State;
}

void GameStateController::SetGameState(GameState state){
    _State = state;
}
