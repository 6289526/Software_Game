#include "client_common.h"

// クライアントのID
static int MyId;
// プレイヤーのデータ
PlayerDatas PlayerData[5];

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
        PlayerData[i].pos.x = moveData[i].x;
        PlayerData[i].pos.y = moveData[i].y;
        PlayerData[i].pos.z = moveData[i].z;
        fprintf(stderr, "client%d　は %f %f %f にいます。\n", i, PlayerData[i].pos.x, PlayerData[i].pos.y, PlayerData[i].pos.z);
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
            PlayerData[MyId].pos.z += 10;
        }
        // 左右
        if (data.Left)
        {
            data.Left = false;
            PlayerData[MyId].pos.x -= 10;
        }
        else if (data.Right)
        {
            data.Right = false;
            PlayerData[MyId].pos.x += 10;
        }
        // ジャンプ
        if (data.Jump)
        {
            data.Jump = false;
            PlayerData[MyId].pos.y += 10;
        }
        
        // PlayerData[MyId].posはFloatCubeなのでFloatPositionにする
        FloatPosition pos;
        pos.x = PlayerData[MyId].pos.x;
        pos.y = PlayerData[MyId].pos.y;
        pos.z = PlayerData[MyId].pos.z;
        // 移動コマンド実行
        InCommand(MOVE_COMMAND, pos);
    }

    
}
