/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
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
#include "../header/constants.h"
#include "client_KeybordInput.h"

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

/* net.c */
extern void SetupClient(char *, u_short);
extern void TerminateClient();
extern int ControlRequests();
extern int InCommand(char com);

/* system.cpp */
const PlayerData* GetPlayerData();
extern void GetId(int);
extern void GetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);
extern void SystemRun(InputType data);
extern void GetFlag(VelocityFlag* flag, int numClients);
/*window.cpp */
extern void InitWindowSys();
extern void RendererWindow();
extern void TerminateWindowSys();
