/*
 *  ファイル名	: constants.h
 *  機能	: マクロ、構造体の定義
 */
#pragma once
/*----------include 開始----------*/
#include <netinet/in.h>

#include<stdio.h>

/*----------include 終了----------*/

/*----------define 開始-----------*/
//デフォルトのポート番号
#define DEFAULT_PORT 51000
//名前の最大文字数
#define MAX_LEN_NAME 10
//最大参加人数
#define MAX_NUM_CLIENTS 5
//メッセージの最大文字数
#define MAX_LEN_BUFFER 256
//ソケットの設定のサイズ
#define MAX_LEN_ADDR 32
//全員に送るとき
#define BROADCAST -1

/*クライアントが使用するコマンド*/
#define MOVE_COMMAND 'M'
#define PUT_COMMAND 'P'
#define GOAL_COMMAND 'G'
#define ERROR_COMMAND 'E'
#define FINISH_COMMAND 'F'
/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/
/*クライアントの情報*/
typedef struct {
    int cid;                  /*クライアントのID*/
    int connect;              /*サーバーに接続しているか*/
    int sock;                 /*使用するソケット*/
    int group;                /*どの手を出したか*/
    int wcount;               /*勝利した回数*/
    int lcount;               /*負けた回数*/
    int dcount;               /*引き分けた回数*/
    struct sockaddr_in addr;  /*ソケットの設定*/
    char name[MAX_LEN_NAME];  /*名前*/
} CLIENT;

/*サーバーとクライアント間で通信する情報*/
typedef struct {
    int cid;                      /*クライアントのID*/
    char command;                 /*入力されたコマンド*/
    int result;                   /*勝敗*/
} CONTAINER;

/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/

/*-----------グローバル変数 終了----------*/


