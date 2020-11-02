
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
    {"a", {20, 20, 20, 10, 10, 10}, {0, 0, 0}, 1, 0, false}};

ServerMap Map;

// クライアント配列の先頭ポインタを返す
const PlayerData *GetPlayerData()
{
    return PData;
}

BlockType Collision_CM(int chara_ID, int y, int accuracy)
{

    if (accuracy <= 1) {
        throw "Collision_CM : 引数　エラー\n";
    }

    Pointer<int> point_X(accuracy);
    Pointer<int> point_Z(accuracy);


    const int wide = PData[chara_ID].pos.w / (accuracy - 1);  // 当たり判定を知らべる座標間距離 x座標
    const int depth = PData[chara_ID].pos.d / (accuracy - 1); // 当たり判定を知らべる座標間距離 z座標

    // 当たり判定を調べる座標をすべて格納
    for (int i = 0; i < accuracy; ++i)
    {
        point_X[i] = PData[chara_ID].pos.x + PData[chara_ID].velocity.x + wide * i;
        point_Z[i] = PData[chara_ID].pos.z + PData[chara_ID].velocity.z + depth * i;
    }

    // マップデータ入手
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

    // 返り値用変数を宣言，初期化
    // ゴールブロックが１個でも接触すればゴールブロックが返る
    // ノーマルブロックが１個でも接触すればノーマルブロックが返る
    BlockType result = NonBlock;

    // マップ配列の添え字用変数を宣言，初期化
    int Block_X = 0;
    int Block_Y = (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y) / MAP_MAGNIFICATION;
    int Block_Z = 0;

    if (Block_Y < 0)
    {
        throw "マップ外 : y座標 : 負\n";
    }
    else if (MAP_SIZE_H <= Block_Y)
    {
        throw "マップ外 : y座標 : 正\n";
    }

    for (int i = 0; i < accuracy; ++i)
    {
        Block_X = point_X[i] / MAP_MAGNIFICATION;

        if (Block_X < 0)
        {
            throw "マップ外 : x座標 :負\n";
        }
        else if (MAP_SIZE_W <= Block_X)
        {
            throw "マップ外 : x座標 : 正\n";
        }

        for (int j = 0; j < accuracy; ++j)
        {
            Block_Z = point_Z[j] / MAP_MAGNIFICATION;

            if (Block_Z < 0)
            {
                throw "マップ外 : z座標 : 負\n";
            }
            else if (MAP_SIZE_D <= Block_Z)
            {
                throw "マップ外 : z座標 : 正\n";
            }

            switch (terrainData[Block_X][Block_Y][Block_Z])
            {
            case GoalBlock:
                result = GoalBlock;
                break;
            case NomalBlock:
                if (result == NonBlock)
                {
                    result = NomalBlock;
                }
                break;
            case NonBlock:
                break;
            default:
                throw "マップデータ : エラー\n";
            }
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

    if (PData[chara_ID].goal == false)
    {
        PData[chara_ID].goal = true;
        PData[chara_ID].rank = rank++;
        RunCommand(chara_ID, GOAL_COMMAND);
    }
}

void MovePosition(int chara_ID)
{

    // 横の当たり判定
    BlockType block = Collision_CM(chara_ID, 3);

    // ブロックがないなら移動
    if (block == NonBlock)
    {
        // 移動後の座標に書き換え
        PData[chara_ID].pos.x += PData[chara_ID].velocity.x;
        PData[chara_ID].pos.z += PData[chara_ID].velocity.z;
    }
    // ゴールブロックならゴール
    if (block == GoalBlock)
    {
        Goal(chara_ID);
    }

    // 下の当たり判定
    block = Collision_CM(chara_ID, 0, 3);

    // ブロックがないなら移動
    if (block == NonBlock)
    {
        // 移動後の座標に書き換え
        PData[chara_ID].pos.y += PData[chara_ID].velocity.y;
    }
    // ゴールブロックならゴール
    if (block == GoalBlock)
    {
        Goal(chara_ID);
    }

    // 速度を０に戻す
    PData[chara_ID].velocity.x = 0;
    PData[chara_ID].velocity.y = 0;
    PData[chara_ID].velocity.z = 0;
}

int AllGoal()
{
    for (int i = 0; i < PLAYER_NUM; ++i)
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
void SetVec(int chara_ID, Vector3 &vec)
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
void SetDirection(int chara_ID, float direction)
{
    PData[chara_ID].direction = direction;
}
