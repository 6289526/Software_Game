/*
 *  ファイル名  : client_input.h
 *  機能    : 入力モジュールの定義
 */

#pragma once
/* error: expected class-name before ‘{’ token
   10 | { 
      | ^
      って出るからclient_common.hはインクルードしない。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../header/constants.h"

typedef struct InputType{
    bool Forward;   // 前進
    bool Right;     // 右
    bool Left;      // 左
    bool Jump;      // 跳
    bool Put;       // 置く
    bool U;         // 上
    bool D;         // 下
    bool R;         // 右
    bool L;         // 左
    bool End;       // 終了

    bool Up;        //wiiリモコンの十字キー上
    bool Down;      //wiiリモコンの下
    InputType() : 
        Forward(false),
        Right(false),
        Left(false),
        Jump(false),
        Put(false),
        U(false),
        D(false),
        R(false),
        L(false),
        End(false),
        Up(false),
        Down(false)
        { };
}InputType;

class InputModuleBase
{
protected:
    InputType _Input;
    SDL_Event _Event;
    bool _putFlag;
    bool _jumpFlag;
public:
    bool _setname;  //名前入力中か
    InputModuleBase();
    virtual ~InputModuleBase();
    virtual void UpdateInput() = 0;

    InputType GetInputType();
    InputType SystemGetInputType();
    bool IsMoveButtonDown();
};
