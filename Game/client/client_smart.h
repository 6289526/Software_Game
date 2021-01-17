/**
 * @file client_smart.h
 * @brief スマートフォン入力管理
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "client_input.h"
#include "go.h"

class Smart: public InputModuleBase
{
private:
    //キーボードの入力を検出するための変数
    // const Uint8 *_key;
    

public:
    Smart();
    ~Smart();
    virtual void UpdateInput();
};
