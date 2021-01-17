/**
 * @file client_KeybordInput.h
 * @brief キーボード入力クラス
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
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
    virtual void UpdateInput();
};
