/*
 *  ファイル名  : client_input.h
 *  機能    : キーボード入力の定義
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
