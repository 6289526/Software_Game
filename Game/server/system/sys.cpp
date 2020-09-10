
/*----------include 開始----------*/
#include "../header/server_common.h"

/*----------include 終了----------*/

/*変数初期化*/
Client Clients[MAX_NUMCLIENTS];

int Mobable(FloatPosition pos) {
    /*
    if (移動できる) {
        return 1;
    }
    else {
        return 0;
    }
    */
}

void MovePosition(int chara_ID, FloatPosition pos) {
    /*移動できるなら*/
    if (Mobable(pos)) {
        // 移動後の座標に書き換え
        Clients[chara_ID].pos.x = pos.x;
        Clients[chara_ID].pos.y = pos.y;
        Clients[chara_ID].pos.z = pos.z;
    }
}

void CheckGoal(int chara_num) {
    static int rank = 1;
    /*
    for (int i = 0; i < chara_num; +;i) {
        if (ゴールしているなら) {
            Clients[i].goal = 1;
            Clients[i].rank = rank++;
            SendData(i, GOAL_COMMAND);
        }
    }

    */
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


FloatPosition GetPosition(int chara_num) {
    FloatPosition pos;
    pos.x = Clients[chara_num].pos.x;
    pos.y = Clients[chara_num].pos.y;
    pos.z = Clients[chara_num].pos.z;
    return pos;
}
