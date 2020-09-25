/*
 *  ファイル名  : client_input.h
 *  機能    : 入力モジュールの定義
 */

#pragma once

typedef struct{
    bool Forward;   // 前進
    bool Right;     // 右
    bool Left;      // 左
    bool Jump;      // 跳
}InputType;

class InputModuleBase
{
protected:
    InputType _Input;
public:
    InputModuleBase();
    ~InputModuleBase();

    virtual void GetInput() = 0;

    InputType GetInputType();
};

InputType InputModuleBase::GetInputType(){
    // 現在の入力情報を返す
    return _Input;
}
