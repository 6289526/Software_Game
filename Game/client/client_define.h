#pragma once
#include "../header/constants.h"
#include "client_input.h"

#define PLAYER_MOVE_SPEED 15					   // 移動速度
#define PLAYER_ROTATE_SPEED 2					   // 回転速度
#define PLAYER_JUMP_POWER 1						   // ジャンプ力
#define PLAYER_HAND_LENGTH (BLOCK_MAGNIFICATION + PLAYER_W) // 手の長さ(ブロックの設置先までの距離)

#define GRAVITY 9.8 * 0.25		// 重力
#define TERMINAL_SPEED (BLOCK_MAGNIFICATION - 1) // 終端速度

extern int Num_Clients; // クライアント人数
extern char WiiAddress[18];
// クライアントの名前
extern char MyName[MAX_LEN_NAME];
extern InputType _______Type;
/* ウインドウサイズ */
enum {
    Wd_Width  = 928,
    Wd_Height = 696
};

/* ゲームの状態 */
enum {
    Gs_Setting = 0, //ゲームの設定段階
    Gs_Result  = 3
};
