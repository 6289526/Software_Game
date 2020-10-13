
/*----------include 開始----------*/
#include "server_common.h"

/*----------include 終了----------*/
//
/*変数初期化*/
PlayerDatas Clients[MAX_NUMCLIENTS];

// 名前の取得
// id: クライアントのID
// clientName:クライアントの名前
void GetClientName(int id, char clientName[MAX_LEN_NAME])
{
    snprintf(Clients[id].name, MAX_LEN_NAME, "%s", clientName);
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
        if (!Clients[i].goal)
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
    Clients[chara_ID].pos.x = pos.x;
    Clients[chara_ID].pos.y = pos.y;
    Clients[chara_ID].pos.z = pos.z;
}

void SendAllPos(int client_num)
{
    char com = MOVE_COMMAND;
    SendData(BROADCAST, &com, sizeof(char));
    FloatPosition pos;
    for (int i = 0; i < client_num; ++i)
    {
        pos.x = Clients[i].pos.x;
        pos.y = Clients[i].pos.y;
        pos.z = Clients[i].pos.z;
        SendData(BROADCAST, &pos, sizeof(FloatPosition));
    }
}
