
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
#include "../constants.h"


/*----------include 終了----------*/

/*----------define 開始-----------*/

/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/

/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/
/*変数*/
// クライアントの情報
extern CLIENT clients[MAX_NUM_CLIENTS];
// 接続してくるクライアントの数
extern int num_clients;
// ソケットの数
extern int num_socks;
// ファイルディスクリプタ
extern fd_set mask;
// ソケット間で送信される情報
extern CONTAINER data;


/* sys.c*/
extern int rps_judge(int , int[3], int);
/* net.c */
extern void setup_server(int, u_short);
extern void terminate_server();
extern void send_data(int, void *, int);
extern int receive_data(int, void *, int);

/* com.c */
extern int control_requests();

/*-----------グローバル変数 終了----------*/


