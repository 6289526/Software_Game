/*
 *  ファイル名  : client_input.h
 *  機能    : キーボード入力の定義
 */

#pragma once
#include "client_input.h"

class KeybordInput : public InputModuleBase
{
private:
    
public:
    KeybordInput();
    ~KeybordInput();

    virtual void GetInput();
};

KeybordInput::KeybordInput()
{
}

KeybordInput::~KeybordInput()
{
}

void KeybordInput::GetInput(){
    //Ex) 前入力取得
    //_Input.Forward = true; 
}
