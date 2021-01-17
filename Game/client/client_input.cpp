#include "client_input.h"

/**
 * @brief 
 * 入力モジュールのコンストラクタ
 */
InputModuleBase::InputModuleBase() : _putFlag(false), _jumpFlag(false), _setname(false)
{
}

/**
 * @brief 
 * 入力モジュールのデストラクタ
 */
InputModuleBase::~InputModuleBase()
{
}

/**
 * @brief 
 * 現在の入力情報を返す
 * @return InputType 入力タイプ
 */
InputType InputModuleBase::GetInputType()
{
    return _Input;
}

/**
 * @brief 
 * SystemRun内で呼ばれる。専用の処理を行い、入力情報を返す。
 * SystemRun以外で呼び出さないこと!!
 * @return InputType 入力タイプ
 */
InputType InputModuleBase::SystemGetInputType()
{
    // すでに置いたなら置かせない
    if (_putFlag)
    {
        _Input.Put = false;
    }
    if (_Input.Put)
    {
        _putFlag = true;
    }

    // すでにジャンプ済みならジャンプさせない
    if (_jumpFlag)
    {
        _Input.Jump = false;
    }
    if (_Input.Jump)
    {
        _jumpFlag = true;
    }

    

    return _Input;
}

/**
 * @brief 
 * 移動に関する入力が入っているかを返す
 * @return true 入っている
 * @return false 入っていない
 */
bool InputModuleBase::IsMoveButtonDown()
{
    bool result = false;
    result = result | _Input.Forward;
    result = result | _Input.Right;
    result = result | _Input.Left;
    result = result | _Input.Jump;
    result = result | _Input.U;
    result = result | _Input.D;
    result = result | _Input.R;
    result = result | _Input.L;
    return result;
}
