/*
 *  ファイル名  : client_input.h
 *  機能    : 入力モジュールの定義
 */

#pragma once
#include "client_common.h"
typedef struct{
    bool Forward;   // 前進
    bool Right;     // 右
    bool Left;      // 左
    bool Jump;      // 跳
    bool Put;       //置く
    bool U;         //上
    bool D;         //下
    bool R;         //右
    bool L;         //左
}InputType;

class InputModuleBase
{
protected:
    InputType _Input;
public:
    InputModuleBase();
    ~InputModuleBase();
    virtual void GetInput(SDL_Event) = 0;

    InputType GetInputType();
};
