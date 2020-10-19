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
