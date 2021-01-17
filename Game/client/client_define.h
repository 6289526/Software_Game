/**
 * @file client_define.h
 * @brief 
 * クライアントの定義部分をまとめたファイル
 */

#pragma once
#include "../header/constants.h"
#include "client_input.h"

#define PLAYER_MOVE_SPEED 40	// 移動速度
#define PLAYER_ROTATE_SPEED 2	// 回転速度
#define PLAYER_JUMP_POWER 2		// ジャンプ力
#define PLAYER_HAND_LENGTH (BLOCK_MAGNIFICATION + PLAYER_W) // 手の長さ(ブロックの設置先までの距離)

#define GRAVITY 9.8 * 0.25		    // 重力
#define TERMINAL_SPEED (BLOCK_MAGNIFICATION - 1) // 終端速度
#define MAXIMUM_ANGLE_UP PI / 2     // 縦方向の角度制限(上限)
#define MAXIMUM_ANGLE_DOWN -PI / 2  // 縦方向の角度制限(下限)

extern int Num_Clients;     // クライアント人数
extern char WiiAddress[18]; // Wiiリモコンのアドレス
extern char MyName[MAX_LEN_NAME];   // クライアントの名前
extern InputType _______Type;       // 入力タイプ

/**
 * @brief 
 * ウインドウサイズ
 */
enum WindowSize {
    Wd_Width  = 1600,
    Wd_Height = 900
};

/**
 * @brief 
 * ゲームの状態
 */
enum {
    Gs_Setting = 0, //ゲームの設定段階
    Gs_Result  = 3
};
