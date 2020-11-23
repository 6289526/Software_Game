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
#include "client_map.hpp"
#include "client_system.h"
#include "client_WiiInput.h"
#include "client_KeybordInput.h"
#include "client_time.hpp"

extern int Num_Clients; // クライアント人数
extern char WiiAddress[17];
extern InputType _______Type;
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

typedef struct InitData{
	InputModuleBase *input;
    Timer *timer;

    InitData() :
        input(NULL),
        timer(NULL)
        { };
} InitData;

/* net.c */
extern void SetupClient(char *, u_short);
extern void TerminateClient();
extern int ControlRequests();
extern int InCommand(char com);

/* system.cpp */
extern void InitControl(InitData *data);
const PlayerData* GetPlayerData();

extern bool InitSystem(InitData *data);
extern void ExitSystem(InitData *data);
extern ClientMap Map;
extern int GetMyID();
extern void SetMyID(int);
extern void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);
extern void UpdateFlag(VelocityFlag* flag, int numClients);
extern PlaceData GetPlaceData();
extern void SystemRun();
extern void UpdatePlaceData(PlaceData data);
// extern void SetPlaceData(PlaceData data);
/* move */

PlaceData BuildPlaceData(PlayerData player);
Vector3 GetMoveDirection(PlayerData player, float angle);

/*control_setup*/
extern int ControlSetUp();
