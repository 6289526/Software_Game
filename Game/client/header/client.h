/*
 *  ファイル名	：client.h
 *  機能	： 変数、関数の宣言
 *  
 */
#pragma once    
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
#include "../../header/constants.h"

/* ウインドウサイズ */
enum {
    WD_Width  = 640,
    WD_Height = 480
};

/* ゲームの状態 */
enum {
    GS_SETTING = 0, //ゲームの設定段階
    GS_RESULT  = 3
};


/*変数*/
// 接続してくるクライアントの数
extern int num_clients;
// 自身のID
extern int myid;
// ソケット
extern int sock;
// ソケットの数
extern int num_sock;
// ファイルディスクリプタ
extern fd_set mask;
// ゲームの結果
extern char game_result[10];
// クライアントの情報
extern CLIENT clients[MAX_NUM_CLIENTS];
// 画像テクスチャ
extern SDL_Texture* rps_texture[5];
// 表示する文字列
extern SDL_Texture* message;
//ウィンドウ
extern SDL_Window* window;
//レンダラー
extern SDL_Renderer* renderer;


// フォント
TTF_Font* font;
// ゲームの状態
extern int game_stts;

extern int PrintError(const char *str);

/* sys.c */
extern int push_button(int, int);


/* win.c */
extern int init_window(void);
extern int draw_window(void);

/* net.c */
extern void setup_client(char *, u_short);
extern int control_requests();
extern void terminate_client();

extern void send_data(void *, int);
extern int receive_data(void *, int);

/* com.c */
extern int in_command(char);
extern int exe_command(void);