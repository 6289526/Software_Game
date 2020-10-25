/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
 */
#pragma once
/*----------include 開始----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../header/constants.h"
#include <SDL2/SDL.h>
//
/*----------include 終了----------*/

/*----------define 開始-----------*/

/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/


/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/
/*変数*/


/* net.c */
extern void SetupServer(int, u_short);
extern void TerminateServer();
extern int ControlRequests();
extern void RunCommand(int, char);

/* sys.cpp */
const PlayerData* GetPlayerData();
       // 当たり判定を取る座標(キャラの右後下からのずれ)を指定
       int Collision(int chara_ID, int x = 0, int y = 0, int z = 0); // 当たり判定 ブロック有 0以外 無 0 ゴールブロック -1
extern void GetClientName(int id,char clientName[MAX_LEN_NAME]);
       void Goal(int chara_ID);    // ゴールの処理
extern void MovePosition(int chara_ID, FloatPosition* pos); // キャラを移動させる
extern int AllGoal(); // 全員ゴールしていれば１
extern void SetVec(int chara_ID, Vector3& vec); // キャラの速度ベクトルをセット
extern void SendAllPos(int client_num); // クライアント全員に全員の座標を送る

extern void SetDirection(int i, float direction); // システムにクライアントの角度を渡す
/*-----------グローバル変数 終了----------*/
