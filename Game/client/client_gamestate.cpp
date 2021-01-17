#include "client_common.h"

using namespace GameState;

// ===== * ===== GameStateController ===== * ===== //
/**
 * @fn
 * @brief ゲームの状態を設定する
 * @param state ゲームの状態
 */
void GameStateController::SetGameState(GameState state){
    _PreState = _State;
    _State = state;
    OnNest(state);
}
// ===== * ===== Subjects ===== * ===== //
/**
 * @fn
 * Subscribersを開放する
 * @brief Subjectのデストラクタ
 */
Subject::~Subject(){
    std::list<Observer*>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->SetSubject(NULL);
    }
}

/**
 * @fn
 * @brief ゲームの状態変更を発行する
 * @param state ゲームの状態
 */
void Subject::OnNest(GameState state){
    std::list<Observer *>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->Update(state);
    }
}

/**
 * @fn
 * @brief 購読する
 * @param observer 購読者
 */
void Subject::Subscribe(Observer *observer){
    _Subscribers.push_back(observer);
    observer->SetSubject(this);
}
// ===== * ===== GameStateOutputer ===== * ===== //
/**
 * @fn
 * @brief ゲームの状態をprintする
 * @param state ゲームの状態
 */
void GameStateOutputer::Update(GameState state){
    fprintf(stderr, "GameState: %d -> %d\n", ((GameStateController *)_Subject)->GetPreState(), ((GameStateController *)_Subject)->GetState());
} 
