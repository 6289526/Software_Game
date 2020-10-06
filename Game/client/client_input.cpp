#include "client_input.h"
InputModuleBase::InputModuleBase(){   
}
InputModuleBase::~InputModuleBase(){
    // printf("siieeeeeeee");    
}
InputType InputModuleBase::GetInputType(){
    // 現在の入力情報を返す
    return _Input;
}
