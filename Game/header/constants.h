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
#define MAX_NUMCLIENTS 5
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
/*座標情報*/
typedef struct{
    int x;       //x座標
    int y;       //y座標
    int z;       //z座標
} IntPosition;

typedef struct{
    float x;       //x座標
    float y;       //y座標
    float z;       //z座標
}FloatPosition;

typedef struct{
    int x;       //x座標
    int y;       //y座標
    int z;       //z座標
    int w;       //x方向の長さ
    int h;       //y方向の長さ
    int d;       //z方向の長さ
}IntCube;

typedef struct{
    float x;       //x座標
    float y;       //y座標
    float z;       //z座標
    float w;       //x方向の長さ
    float h;       //y方向の長さ
    float d;       //z方向の長さ
}FloatCube;


/*クライアントの情報*/
typedef struct {
    int cid;                  /*クライアントのID*/
    int connect;              /*サーバーに接続しているか*/
    int sock;                 /*使用するソケット*/
    struct sockaddr_in addr;  /*ソケットの設定*/
    char name[MAX_LEN_NAME];  /*名前*/
    int rank;                 /*順位*/
    int goal;                 /*ゴールしているか*/
    FloatCube pos;             /*場所*/
} Client;


/**サーバーとクライアント間で通信する情報**/

/*コマンド*/
//
// typedef struct
// {
// //    int cid;                      /*クライアントのID*/
//     char command;                 /*入力されたコマンド*/
//     int able;                     /*コマンドが実行可能か*/
// } Command;


/*MoveData*/
// typedef struct {
//     int cid;                      /*クライアントのID*/
//     Position pos;                 /*場所*/
//     int goal;                     /*ゴールしたか*/
// } MoveData;

/*PlaceData*/
typedef struct {
//    int cid;                      /*クライアントのID*/
    int object;                   /*置く物のID*/
    IntPosition pos;                 /*場所*/
} PlaceData;





/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/

/*-----------グローバル変数 終了----------*/
