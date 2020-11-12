#include "client_input.h"
InputModuleBase::InputModuleBase(){
}

InputModuleBase::~InputModuleBase(){
    // printf("siieeeeeeee");
}

// 現在の入力情報を返す
InputType InputModuleBase::GetInputType(){
    return _Input;
}

bool InputModuleBase::IsMoveButtonDown(){
    int result = 0;
    result += _Input.Forward;
    result += _Input.Right;
    result += _Input.Left;
    result += _Input.Jump;
    result += _Input.U;
    result += _Input.D;
    result += _Input.R;
    result += _Input.L;
    return result > 0;
}
