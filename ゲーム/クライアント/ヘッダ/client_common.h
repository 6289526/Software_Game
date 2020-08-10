/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
 */

#ifndef CLIENT_COMMON_H
#define CLIENT_COMMON_H

/*----------include 開始----------*/
#include"../../header/constants.h"
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
/*----------include 終了----------*/


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
#endif
