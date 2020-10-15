#include "client_common.h"

// クライアントのID
static int MyId;
// プレイヤーのデータ
PlayerData PData[PLAYER_NUM] = {
    {"a", {20, 20, 20, 7, 20, 7}, 1, 0},
    {"a", {20, 20, 20, 10, 10, 10}, 1, 0}
};

// クライアント配列の先頭ポインタを返す
const PlayerData* GetPlayerData(){
    return PData;
}

/*クライアントのID取得
* 引数
*   id: クライアントのID
*/
void GetId(int id)
{
    MyId = id;
}

/*クライアントの位置の取得
* 引数
*   moveData[MAX_NUMCLIENTS]: 移動位置
*   numCLients : 接続しているクライアントの数
*/
void GetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients)
{
    for (int i = 0; i < numClients; i++)
    {
        // 座標の代入
        PData[i].pos.x = moveData[i].x;
        PData[i].pos.y = moveData[i].y;
        PData[i].pos.z = moveData[i].z;
        fprintf(stderr, "client%d　は %f %f %f にいます。\n", i, PData[i].pos.x, PData[i].pos.y, PData[i].pos.z);
    }
}

/*移動処理とか設置処理
* 引数
*   data: 入力データ 
*   
*/
void SystemRun(InputType data)
{
    // 移動処理
    if (data.Forward || data.Left || data.Right || data.Left || data.Jump)
    {
        // 前
        if (data.Forward)
        {
            data.Forward = false;
            PData[MyId].pos.z += 10;
        }
        // 左右
        if (data.Left)
        {
            data.Left = false;
            PData[MyId].pos.x -= 10;
        }
        else if (data.Right)
        {
            data.Right = false;
            PData[MyId].pos.x += 10;
        }
        // ジャンプ
        if (data.Jump)
        {
            data.Jump = false;
            PData[MyId].pos.y += 10;
        }

        // PData[MyId].posはFloatCubeなのでFloatPositionにする
        FloatPosition pos;
        pos.x = PData[MyId].pos.x;
        pos.y = PData[MyId].pos.y;
        pos.z = PData[MyId].pos.z;
        // 移動コマンド実行
        InCommand(MOVE_COMMAND);
    }
}
