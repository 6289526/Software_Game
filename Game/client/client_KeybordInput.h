/*
 *  ファイル名  : client_input.h
 *  機能    : キーボード入力の定義
 */

#pragma once
#include "client_input.h"

class KeybordInput : public InputModuleBase
{
private:
    //キーボードの入力を検出するための変数
    const Uint8 *_key;
public:
    KeybordInput();
    ~KeybordInput();
    virtual void GetInput(SDL_Event);
};
