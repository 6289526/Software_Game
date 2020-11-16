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
    bool result = false;
    result = result | _Input.Forward; // 
    result = result | _Input.Right;
    result = result | _Input.Left;
    result = result | _Input.Jump;
    result = result | _Input.U;
    result = result | _Input.D;
    result = result | _Input.R;
    result = result | _Input.L;
    return result;
}
