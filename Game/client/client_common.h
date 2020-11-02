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
#include "client_map.hpp"

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

typedef struct{
	InputModuleBase **input;
} InitData;

/* net.c */
extern void SetupClient(char *, u_short);
extern void TerminateClient();
extern int ControlRequests();
extern int InCommand(char com);

/* system.cpp */
const PlayerData* GetPlayerData();

extern bool InitSystem(InitData *data);
extern int GetMyID();
extern void SetMyID(int);
extern PlaceData GetPlaceData();
extern void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);

extern void SystemRun(InputType data);

extern ClientMap Map;
extern void GetFlag(VelocityFlag* flag, int numClients);

/* move */
extern void UpdateFlag(VelocityFlag* flags, int numClients);
