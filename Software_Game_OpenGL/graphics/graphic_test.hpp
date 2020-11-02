#pragma once

#include "graphic.h"

//仮宣言
typedef struct{
    char name[20];
    FloatCube pos;
    int rank;
    int goal;
    float direction;
}PlayerData;
const PlayerData* GetPlayerData();
void SetPlayerPos(int id, double x, double y, double z, double dir);
int GetMyID();


extern ClientMap Map;