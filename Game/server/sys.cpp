
/*----------include 開始----------*/
#include "server_common.h"
#include "../header/map.hpp"
#include "server_map.hpp"

/*----------include 終了----------*/
//
/*変数初期化*/
// プレイヤー情報
PlayerData PData[PLAYER_NUM] = {
    {"a", {20, 20, 20, 7, 20, 7}, 1, 0, 0.0},
    {"a", {20, 20, 20, 10, 10, 10}, 1, 0, 0.0}
};

// プレイヤーの移動先座標
FloatPosition AfterPlayerPos[PLAYER_NUM] = {
    {PData[0].pos.x, PData[0].pos.y, PData[0].pos.z},
    {PData[1].pos.x, PData[1].pos.y, PData[1].pos.z},
};

ServerMap Map;

// クライアント配列の先頭ポインタを返す
const PlayerData* GetPlayerData(){
    return PData;
}

int CollisionUnder(int chara_ID) // 地面との当たり判定 ブロック無 0 有 0以外
{
    int result = 0; // ブロック 無 0 有 0以外


    // 進行先のブロックを指定
    int BlockX = AfterPlayerPos[chara_ID].x / MAP_MAGNIFICATION;
    int BlockY = AfterPlayerPos[chara_ID].y / MAP_MAGNIFICATION;
    int BlockZ = AfterPlayerPos[chara_ID].z / MAP_MAGNIFICATION;

     const int (*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();


     //　移動先の座標の地面４ブロックを調べる
     result = terrainData[BlockX]    [BlockY][BlockZ]     |
              terrainData[BlockX + 1][BlockY][BlockZ]     |
              terrainData[BlockX]    [BlockY][BlockZ + 1] |
              terrainData[BlockX + 1][BlockY][BlockZ + 1];

    return result;
}

int CollisionSide(int chara_ID) // 壁との当たり判定 ブロック有 1 無 0
{

}

// 名前の取得
// id: クライアントのID
// clientName:クライアントの名前
void GetClientName(int id, char clientName[MAX_LEN_NAME])
{
    snprintf(PData[id].name, MAX_LEN_NAME, "%s", clientName);
}


int Mobable(FloatPosition *pos)
{
    /*
    if (移動できる) {
        return 1;
    }
    else {
        return 0;
    }
    */

    return 1;
}

void CheckGoal(int chara_ID)
{
    static int rank = 1;
    /*
    if (ゴールしているなら) {
        Clients[chara_ID].goal = 1;
        Clients[chara_ID].rank = rank++;
        SendData(chara_ID, GOAL_COMMAND);
    }
    */
}

void MovePosition(int chara_ID, FloatPosition *pos)
{
    /*移動できるなら*/
    if (Mobable(pos))
    {
        // 移動後の座標に書き換え
        // Clients[chara_ID].pos.x = pos->x;
        // Clients[chara_ID].pos.y = pos->y;
        // Clients[chara_ID].pos.z = pos->z;
    }
    printf("OK\n");
    CheckGoal(chara_ID);
}

int Goal()
{
    for (int i = 0; i < MAX_NUMCLIENTS; ++i)
    {
        // ゴールしていないクライアントがいれば
        if (!PData[i].goal)
        {
            return 0;
        }
    }

    return 1;
}

// クライアントの座標を取得
// chara_ID:クライアントのID
// pos:クライアントの座標
void SetPosition(int chara_ID, FloatPosition pos)
{
    AfterPlayerPos[chara_ID].x = pos.x;
    AfterPlayerPos[chara_ID].y = pos.y;
    AfterPlayerPos[chara_ID].z = pos.z;
}

// システムにクライアントの角度を渡す
void SetDirection(int i, float direction){

}

/*全員に座標を送る
*
* 引数
*     int client_num: クライアントの人数
*/
void SendAllPos(int client_num)
{
    char com = MOVE_COMMAND;
    // 全員に座標を送る
    for (int i = 0; i < client_num; ++i)
    {
        // 特定のIDにコマンドを送る
        RunCommand(i, com);
    }
}
