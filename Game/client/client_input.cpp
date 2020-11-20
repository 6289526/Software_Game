#include "client_input.h"
InputModuleBase::InputModuleBase() : _putFlag(false), _jumpFlag(false){
}

InputModuleBase::~InputModuleBase(){
    // printf("siieeeeeeee");
}

// 現在の入力情報を返す
InputType InputModuleBase::GetInputType(){
    return _Input;
}

// SystemRun内で呼ばれる専用の入力情報を返す
// SystemRun以外で呼び出さないこと!!
InputType InputModuleBase::SystemGetInputType(){
    // すでに置いたなら置かせない
    if(_putFlag){
        _Input.Put = false;
    }
    if(_Input.Put){
        _putFlag = true;
    }

    // すでにジャンプ済みならジャンプさせない
    if(_jumpFlag){ 
        _Input.Jump = false;
    }
    if(_Input.Jump){
        _jumpFlag = true;
    }
    if(_Input.Jump || _Input.Put)
    fprintf(stderr, "%d %d %d %d\n", _Input.Jump, _jumpFlag,  _Input.Put, _putFlag);
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
