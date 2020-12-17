/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <tuple>
#include "../header/constants.h"
#include "client_map.hpp"
#include "client_system.h"
#include "client_WiiInput.h"
#include "client_KeybordInput.h"
#include "client_time.hpp"
#include "client_gamestate.hpp"
#include "graphic.h"
#include "client_smart.h"
#include "client_music.hpp"

#define PLAYER_MOVE_SPEED 15					   // 移動速度
#define PLAYER_ROTATE_SPEED 2					   // 回転速度
#define PLAYER_JUMP_POWER 1						   // ジャンプ力
#define PLAYER_HAND_LENGTH (BLOCK_MAGNIFICATION + PLAYER_W) // 手の長さ(ブロックの設置先までの距離)

#define GRAVITY 9.8 * 0.25		// 重力
#define TERMINAL_SPEED (BLOCK_MAGNIFICATION - 1) // 終端速度

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
    GameState::GameStateController *stateController;

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
extern void SetRank(int id, int rank);
extern void UpdateFlag(VelocityFlag* flag, int numClients);
extern PlaceData GetPlaceData();
extern void SystemRun();
extern void UpdatePlaceData(PlaceData data);
extern GameState::GameStateController GetGameStateController();
extern void SetDirection(float direction, int id);
extern void SetRemoveClient(int id);

/*control_setup*/
extern int ControlSetUp();

/*name_setup*/
extern void GetInitData(InitData initData);
extern void NameSetUp();


extern void KillGoServer();
extern void Goroutine();
