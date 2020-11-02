/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
 */

#pragma once    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <netdb.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"

/* ウインドウサイズ */
enum {
    Wd_Width  = 640,
    Wd_Height = 480
};

/* ゲームの状態 */
enum {
    Gs_Setting = 0, //ゲームの設定段階
    Gs_Result  = 3
};


// /*変数*/
// // 接続してくるクライアントの数
// extern int NumClients;
// // 自身のID
// extern int MyId;
// // ソケット
// extern int Sock;
// // ソケットの数
// extern int NumSock;
// // ファイルディスクリプタ
// //extern fd_set Mask;
// // クライアントの情報
// extern Client Clients[MAX_NUMCLIENTS];

// // ゲームの状態
// extern int GameStts;

// extern int PrintError(const char *str);


// /* net.c */
// // extern void SetupClient(char *, u_short);
// // extern int ControlRequests();
// // extern void terminate_client();

// // extern int SendData(void *);
// // extern int ReceiveData(void *);

// // extern int InCommand(char);
// // extern int ExeCommand(void);