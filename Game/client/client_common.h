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
#include "client_gamestate.hpp"
#include "graphic.h"

extern int Num_Clients; // クライアント人数
extern char WiiAddress[18];
// クライアントの名前
extern char MyName[MAX_LEN_NAME];
extern InputType _______Type;
/* ウインドウサイズ */
enum {
    Wd_Width  = 928,
    Wd_Height = 696
};

/* ゲームの状態 */
enum {
    Gs_Setting = 0, //ゲームの設定段階
    Gs_Result  = 3
};

typedef struct InitData{
	InputModuleBase *input;
    Timer *timer;
    GameStateController *stateController;

    InitData() :
        input(NULL),
        timer(NULL),
        stateController(NULL)
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
extern GameStateController GetGameStateController();
extern void SetDirection(float direction, int id);
/*control_setup*/
extern int ControlSetUp();

/*name_setup*/
extern void GetInitData(InitData initData);
extern void NameSetUp();
