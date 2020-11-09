
/*----------include 開始----------*/
#include "server_common.h"


/*----------include 終了----------*/
//
/*変数初期化*/
// プレイヤー情報

static int Num_Clients; // クライアント人数
static char* Name_Clients[MAX_NUMCLIENTS]; // クライアントの名前
static FloatCube Pos_Clients = { PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W, PLAYER_H, PLAYER_D }; // クライアント情報


PlayerData* PData;

PlaceData PlData = {
    NonBlock,
    {0, 0, 0}
};

ServerMap Map;

// クライアント配列の先頭ポインタを返す
const PlayerData *GetPlayerData()
{
    return PData;
}

void SetNumClients(int n) // クライアント人数セット
{
    Num_Clients = n;
}

// 名前のセット
// id: クライアントのID
// clientName:クライアントの名前
void SetClientName(int id, char* name)
{
    Name_Clients[id] = name;
}

void InitSys(char* file) // システム初期化
{
    Map.LoadMapData(file);
}

void InitPlayerData()// プレイヤーデータ初期化処理
{
    PData = new PlayerData[Num_Clients];
    for (int i = 0; i < Num_Clients; ++i) {
        strcpy(PData[i].name, Name_Clients[i]);
        PData[i].pos = Pos_Clients;
        PData[i].pos.x = Pos_Clients.x + i * 20;
        Vector3 t_v = { 0, 0, 0 };
        PData[i].velocity = t_v;
        PData[i].direction = 0;
        PData[i].rank = 0;
        PData[i].goal = false;
    }
}

void EndSys()// システム終了処理
{
    delete[] PData;
}


BlockType Collision_CB(int chara_ID, int y, int accuracy)
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
    int t_Block_Y = (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y);
    int Block_Y = (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y) / MAP_MAGNIFICATION;
    int Block_Z = 0;

    if (t_Block_Y < 0)
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

        if (point_X[i] < 0)
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

            if (point_Z[j] < 0)
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

bool Collision_BB() // ブロックを置けるかどうかの判定
{
    if (PlData.object == NonBlock) {
        return false;
    }

    // マップデータ入手
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

    int Block_X = PlData.pos.x / MAP_MAGNIFICATION;
    int Block_Y = PlData.pos.y / MAP_MAGNIFICATION;
    int Block_Z = PlData.pos.z / MAP_MAGNIFICATION;

    if (Block_X < 0)
    {
        throw "マップ外 : x座標 : 負\n";
    }
    else if (MAP_SIZE_H <= Block_X)
    {
        throw "マップ外 : x座標 : 正\n";
    }

    if (Block_Y < 0)
    {
        throw "マップ外 : y座標 : 負\n";
    }
    else if (MAP_SIZE_H <= Block_Y)
    {
        throw "マップ外 : y座標 : 正\n";
    }

    if (Block_Z < 0)
    {
        throw "マップ外 : z座標 : 負\n";
    }
    else if (MAP_SIZE_H <= Block_Z)
    {
        throw "マップ外 : z座標 : 正\n";
    }

    // 置く場所にブロックがないなら
    if (terrainData[Block_X][Block_Y][Block_Z] == NonBlock) {
        // 最下段なら
        if (Block_Y - 1 < 0)
        {
            // 置けます
            return true;
        }
        //　下の段にブロックがあるなら
        if (terrainData[Block_X][Block_Y - 1][Block_Z] != NonBlock) {
            // 置けます
            return true;
        }

        // 置けません
        return false;
    }
    else {
        // 置けません
        return false;
    }


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
    BlockType block = Collision_CB(chara_ID, 2);

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
    block = Collision_CB(chara_ID, 0, 3);

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
    //PData[chara_ID].velocity.y = 0;
    PData[chara_ID].velocity.z = 0;
}

void PutBlock(int chara_ID) // ブロックを置けるなら置く
{
    // ブロックを置けるなら
    if (Collision_BB()) {
        // マップに追加
        Map.SetObjectData(&PlData);
        // クライアント全員に送信
        RunCommand(BROADCAST, PUT_COMMAND);
    }
    // ブロックを置けないなら
    else{
        // データ書き換え
        PlData.object = NonBlock;
        // ブロックを置くクライアントのみに送信
        RunCommand(chara_ID, DO_NOT_PUT_COMMAND);
    }

    // PlaceData を初期化
    PlData.object = NonBlock;
    PlData.pos.x = 0;
    PlData.pos.y = 0;
    PlData.pos.z = 0;
}


int AllGoal()
{
    for (int i = 0; i < Num_Clients; ++i)
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


void SetPlaceData(PlaceData& data)
{
    PlData = data;
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
