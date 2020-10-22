
/*----------include 開始----------*/
#include "server_common.h"
#include "../header/map.hpp"
#include "server_map.hpp"

/*----------include 終了----------*/
//
/*変数初期化*/
// プレイヤー情報
PlayerData PData[PLAYER_NUM] = {
    {"a", {20, 20, 20, 7, 20, 7}, {0, 0, 0}, 1, 0, false},
    {"a", {20, 20, 20, 10, 10, 10}, {0, 0, 0}, 1, 0, false}
};

ServerMap Map;

// クライアント配列の先頭ポインタを返す
const PlayerData* GetPlayerData(){
    return PData;
}

int Collision(int chara_ID, int x, int y, int z) // 当たり判定 ブロック無 0 有 0以外 ゴールブロック -1
{
    int FourBlock[4] = {0};
    int result = 0; // ブロック 無 0 有 0以外


    // 進行先のブロックを指定
    int BlockX = PData[chara_ID].pos.x + PData[chara_ID].velocity.x + x / MAP_MAGNIFICATION;
    int BlockY = PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y / MAP_MAGNIFICATION;
    int BlockZ = PData[chara_ID].pos.z + PData[chara_ID].velocity.z + z / MAP_MAGNIFICATION;

    const int (*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

     //　移動先の座標の地面４ブロックを調べる
    FourBlock[0] = terrainData[BlockX][BlockY][BlockZ];
    FourBlock[1] = terrainData[BlockX + 1][BlockY][BlockZ];
    FourBlock[2] = terrainData[BlockX][BlockY][BlockZ + 1];
    FourBlock[3] = terrainData[BlockX + 1][BlockY][BlockZ + 1];

    for (int i = 0; i < 4; ++i) {
        if (FourBlock[i] == GoalBlock) {
            return -1;
        }
        else {
            result += FourBlock[i];
        }
    }

    return result;
}

// 名前の取得
// id: クライアントのID
// clientName:クライアントの名前
void GetClientName(int id, char clientName[MAX_LEN_NAME])
{
    snprintf(PData[id].name, MAX_LEN_NAME, "%s", clientName);
}


void Goal(int chara_ID)
{
    static int rank = 1;

    if (PData[chara_ID].goal == false) {
        PData[chara_ID].goal = true;
        PData[chara_ID].rank = rank++;
        RunCommand(chara_ID, GOAL_COMMAND);
    }
}

void MovePosition(int chara_ID)
{
    // 横の当たり判定
    int block = Collision(chara_ID, 0, 1, 0);
    // ブロックがないなら移動
    if (0 == block)
    {
        // 移動後の座標に書き換え
        PData[chara_ID].pos.x += PData[chara_ID].velocity.x;
        PData[chara_ID].pos.z += PData[chara_ID].velocity.z;
    }
    // ブロックがあるなら移動せず速度を0にする
    else {
        // ゴールブロックならゴール
        if (block == -1) {
            Goal(chara_ID);
        }

        PData[chara_ID].velocity.x = 0;
        PData[chara_ID].velocity.z = 0;
    }

    // 下の当たり判定
    block = Collision(chara_ID);
    // ブロックがないなら移動
    if (0 == block)
    {
        // 移動後の座標に書き換え
        PData[chara_ID].pos.y += PData[chara_ID].velocity.y;
    }
    // ブロックがあるなら移動せず速度を0にする
    else {
        // ゴールブロックならゴール
        if (block == -1) {
            Goal(chara_ID);
        }

        PData[chara_ID].velocity.y = 0;
    }
}

int AllGoal()
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

// クライアントの速度ベクトルをセット
// chara_ID:クライアントのID
// pos:クライアントの座標
void SetVec(int chara_ID, Vector3& vec)
{
    PData[chara_ID].velocity.x = vec.x;
    PData[chara_ID].velocity.y = vec.y;
    PData[chara_ID].velocity.z = vec.z;
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

// システムにクライアントの角度を渡す
void SetDirection(int chara_ID, float direction){
    PData[chara_ID].direction = direction;
}
