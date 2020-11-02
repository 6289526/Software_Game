/*
 *  ファイル名  : client_input.h
 *  機能    : ClientSystemにある変数と関数の定義
 */

#pragma once
#include "client_common.h"

const PlayerData* GetPlayerData();
extern bool InitSystem(InitData *data);
extern void SetMyID(int);
extern void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);
extern void SystemRun(InputType data);
extern void UpdateFlag(VelocityFlag* flags, int numClients);
extern int GetMyID();
