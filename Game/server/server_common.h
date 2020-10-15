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
// /*ネットワークモジュール用のクライアントの情報*/
// typedef struct {
//     int connect;              /*サーバーに接続しているか*/
//     int sock;                 /*使用するソケット*/
//     struct sockaddr_in addr;  /*ソケットの設定*/
// } NetworkData;
// /*システムモジュールが持ってるプレイヤーデータ*/ 
// typedef struct{
//     char name[MAX_LEN_NAME];    /*名前*/
//     FloatCube pos;              /*位置*/
//     int rank;                   /*順位*/
//     int goal;                   /*ゴールしたか*/
// } PlayerData;

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
extern void GetClientName(int id,char clientName[MAX_LEN_NAME]);
extern int Mobable(FloatPosition* pos); // 移動できるかどうか　できれば１が返る
extern void CheckGoal(int chara_ID);    // ゴールしているか判定
extern void MovePosition(int chara_ID, FloatPosition* pos); // キャラを移動させる
extern int Goal(); // 全員ゴールしていれば１
extern void GetPosition(int chara_ID, FloatPosition pos); // キャラの座標を入手
extern void SendAllPos(int client_num); // クライアント全員に全員の座標を送る

/*-----------グローバル変数 終了----------*/
