
/*----------include 開始----------*/
#include "../header/server_common.h"

/*----------include 終了----------*/

/*変数初期化*/
Client Clients[MAX_NUMCLIENTS];

int Mobable(FloatPosition* pos) {
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

void CheckGoal(int chara_ID) {
    static int rank = 1;
    /*
    if (ゴールしているなら) {
        Clients[chara_ID].goal = 1;
        Clients[chara_ID].rank = rank++;
        SendData(chara_ID, GOAL_COMMAND);
    }
    */
}

void MovePosition(int chara_ID, FloatPosition* pos) {
    /*移動できるなら*/
    if (Mobable(pos)) {
        // 移動後の座標に書き換え
        Clients[chara_ID].pos.x = pos->x;
        Clients[chara_ID].pos.y = pos->y;
        Clients[chara_ID].pos.z = pos->z;
    }

    CheckGoal(chara_ID);
}

int Goal() {
    for (int i = 0; i < MAX_NUMCLIENTS; ++i) {
        // ゴールしていないクライアントがいれば
        if (!Clients[i].goal) {
            return 0;
        }
    }

    return 1;
}


FloatPosition GetPosition(int chara_ID) {
    FloatPosition pos;
    pos.x = Clients[chara_ID].pos.x;
    pos.y = Clients[chara_ID].pos.y;
    pos.z = Clients[chara_ID].pos.z;
    return pos;
}

void SendAllPos(int client_num) {
    for (int i = 0; i < client_num; ++i) {
        SendData(BROADCAST, &Clients[i].pos);
    }
}
