
/*----------include 開始----------*/
#include "server_common.h"

/*----------include 終了----------*/
//
/*変数初期化*/
PlayerData PData[PLAYER_NUM] = {
    {"a", {20, 20, 20, 7, 20, 7}, 1, 0},
    {"a", {20, 20, 20, 10, 10, 10}, 1, 0}
};

// クライアント配列の先頭ポインタを返す
const PlayerData* GetPlayerData(){
    return PData;
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
void GetPosition(int chara_ID, FloatPosition pos)
{
    PData[chara_ID].pos.x = pos.x;
    PData[chara_ID].pos.y = pos.y;
    PData[chara_ID].pos.z = pos.z;
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
