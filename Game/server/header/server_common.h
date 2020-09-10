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
#include "../../header/constants.h"


/*----------include 終了----------*/

/*----------define 開始-----------*/

/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/

/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/
/*変数*/
// クライアントの情報
extern Client Clients[MAX_NUMCLIENTS];
// 接続してくるクライアントの数
extern int NumClients;
// ソケットの数
extern int NumSocks;
// ファイルディスクリプタ
extern fd_set Mask;
//ゴールした人数
extern int NumGoal;


/* net.c */
extern void SetupServer(int, u_short);
extern void TerminateServer();
extern int SendData(int, void *);
extern int ReceiveData(int, void *);

extern int ControlRequests();

/* sys.cpp */
int Mobable(FloatPosition* pos); // 移動できるかどうか　できれば１が返る
void CheckGoal(int chara_ID);    // ゴールしているか判定
void MovePosition(int chara_ID, FloatPosition* pos); // キャラを移動させる
int Goal(); // 全員ゴールしていれば１
FloatPosition GetPosition(int chara_ID); // キャラの座標を入手

/*-----------グローバル変数 終了----------*/
