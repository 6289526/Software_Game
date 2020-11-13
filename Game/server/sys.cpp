
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

Collision Collision_CB_Side(const int chara_ID, const int y, const int accuracy)
{
    // 当たり判定の精度が正しいかどうか
    if (accuracy < 1 || PLAYER_D < accuracy || PLAYER_W < accuracy) {
        throw "Collision_CM : 引数　エラー\n";
    }

    Pointer<float> point_X(accuracy); // 調べる座標ｘ
    Pointer<float> point_Z(accuracy); // 調べる座標ｚ


    const float wide = PData[chara_ID].pos.w / (accuracy - 1);  // 当たり判定を知らべる座標間距離 x座標
    const float depth = PData[chara_ID].pos.d / (accuracy - 1); // 当たり判定を知らべる座標間距離 z座標

    // 当たり判定を調べる座標をすべて格納
    for (int i = 0; i < accuracy; ++i)
    {
        point_X[i] = PData[chara_ID].pos.x + PData[chara_ID].velocity.x + wide * i;
        point_Z[i] = PData[chara_ID].pos.z + PData[chara_ID].velocity.z + depth * i;
    }

    // マップデータ入手
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

    // マップ配列用添字　の宣言　と　範囲のエラー処理
    int Block_X = point_X[accuracy - 1] / MAP_MAGNIFICATION;

    if (point_X[0] < 0)
    {
        throw "マップ外 : x座標 :負\n";
    }
    else if (MAP_SIZE_W <= Block_X)
    {
        throw "マップ外 : x座標 : 正\n";
    }

    const float t_Block_Y = (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y);
    int Block_Y = t_Block_Y / MAP_MAGNIFICATION;

    if (t_Block_Y < 0)
    {
        throw "マップ外 : y座標 : 負\n";
    }
    else if (MAP_SIZE_H <= Block_Y)
    {
        throw "マップ外 : y座標 : 正\n";
    }

    int Block_Z = point_Z[accuracy - 1] / MAP_MAGNIFICATION;

    if (point_Z[0] < 0)
    {
        throw "マップ外 : z座標 :負\n";
    }
    else if (MAP_SIZE_W <= Block_Z)
    {
        throw "マップ外 : z座標 : 正\n";
    }

    int Buried_Flag = 0; // 4以上ならキャラがブロックに埋まっている

    int t_Count = 0; // 一時的なカウンタ
    int Count_Back  = 0; // キャラの後からの めり込み度
    int Count_Front = 0; // キャラの前からの めり込み度

    // キャラの前後側判定
    for (int i = 1; i < (accuracy - 1); ++i) {
        // キャラの右側から左側へ　調べる座標点で移り変わる
        Block_X = point_X[i] / MAP_MAGNIFICATION;

        //　めり込み度計算 後
        for (int j = 0; j <= PData[chara_ID].pos.d; ++j) {
            // ブロックがある限りキャラの内側に変化
            Block_Z = (point_Z[0] + j) / MAP_MAGNIFICATION;

            if (terrainData[Block_X][Block_Y][Block_Z] == NomalBlock) {
                // 最も深くめり込んだ深さを計算
                if (Count_Back < ++t_Count) {
                    ++Count_Back;
                    // この場合は　右左判定　で判定する
                    if (PData[chara_ID].pos.d <= j) {
                        Count_Back = 0;
                        ++Buried_Flag;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }

        t_Count = 0; // 使い回すため 最初期化

        //　めり込み度計算　前
        for (int j = 0; j <= PData[chara_ID].pos.d; ++j) {
            // ブロックがある限りキャラの内側に変化
            Block_Z = (point_Z[accuracy - 1] - j) / MAP_MAGNIFICATION;

            if (terrainData[Block_X][Block_Y][Block_Z] == NomalBlock) {
                // 最も深くめり込んだ深さを計算
                if (Count_Front < ++t_Count) {
                    ++Count_Front;
                    // この場合は　右左判定　で判定する
                    if (PData[chara_ID].pos.d <= j) {
                        Count_Front = 0;
                        ++Buried_Flag;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
        t_Count = 0; // 使い回すため 最初期化
    }

    int Count_Right = 0; // キャラの右からの めり込み度
    int Count_Left  = 0; // キャラの左からの めり込み度

    // キャラの左右側判定
    for (int i = 1; i < accuracy - 1; ++i) {
        // キャラの後側から前側へ　調べる座標点で移り変わる
        Block_Z = point_Z[i] / MAP_MAGNIFICATION;

        //　めり込み度計算 右
        for (int j = 0; j <= PData[chara_ID].pos.w; ++j) {
            // ブロックがある限りキャラの内側に変化
            Block_X = (point_X[0] + j) / MAP_MAGNIFICATION;

            if (terrainData[Block_X][Block_Y][Block_Z] == NomalBlock) {
                // 最も深くめり込んだ深さを計算
                if (Count_Right < ++t_Count) {
                    ++Count_Right;
                    // この場合は　前後判定　で判定する
                    if (PData[chara_ID].pos.w <= j) {
                        Count_Right = 0;
                        ++Buried_Flag;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }

        t_Count = 0; // 使い回すため 最初期化

        //　めり込み度計算　左
        for (int j = 0; j <= PData[chara_ID].pos.w; ++j) {
            // ブロックがある限りキャラの内側に変化
            Block_X = (point_X[accuracy - 1] - j) / MAP_MAGNIFICATION;

            if (terrainData[Block_X][Block_Y][Block_Z] == NomalBlock) {
                // 最も深くめり込んだ深さを計算
                if (Count_Left < ++t_Count) {
                    ++Count_Left;
                    // この場合は　前後判定　で判定する
                    if (PData[chara_ID].pos.w <= j) {
                        Count_Left = 0;
                        ++Buried_Flag;
                        break;
                    }
                }
            }
            else {
                break;
            }
        }

        t_Count = 0; // 使い回すため 最初期化
    }

    if (4 <= Buried_Flag) {
        throw "Collision_CB_Side : キャラがブロックに埋まってる\n";
    }

    Collision_Dire t_dire = Non; // 当たり判定で押し戻す方向
    int t_Max_Count = 0; // 最も多いカウント

    // 最もブロックと重なっているキャラの方向を調べる
    t_Max_Count = std::max(Count_Right,Count_Back);
    if (Count_Right == t_Max_Count) {
        t_dire = Right;
    }
    else {
        t_dire = Back;
    }

    t_Max_Count = std::max(t_Max_Count,Count_Left);
    if (Count_Left == t_Max_Count) {
        t_dire = Left;
    }

    t_Max_Count = std::max(t_Max_Count,Count_Front);
    if (Count_Front == t_Max_Count) {
        t_dire = Front;
    }

    if (t_Max_Count == 0) {
        t_dire = Non;
    }

    Collision ret; // 返り値
    ret.dire = t_dire;
    ret.power = t_Max_Count;

    // なんかよくわからん補正　これがないとグラフィックがぶれる
    // これでうまく行けるんで書いときます　誰か計算して理由教えて
    if (ret.dire == Left || ret.dire == Front) {
        ret.power -= 1;
    }

    return ret;
}

BlockType Collision_CB_Under(int chara_ID, int y, int accuracy)
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
    Collision t_Collision = Collision_CB_Side(chara_ID, 3);

    // 移動後の座標に書き換え
    PData[chara_ID].pos.x += PData[chara_ID].velocity.x;
    PData[chara_ID].pos.z += PData[chara_ID].velocity.z;


    switch (t_Collision.dire) {
        case Front:
            PData[chara_ID].pos.z = static_cast<int>(PData[chara_ID].pos.z - t_Collision.power);
            break;
        case Right:
            PData[chara_ID].pos.x = static_cast<int>(PData[chara_ID].pos.x + t_Collision.power);
            break;
        case Back:
            PData[chara_ID].pos.z = static_cast<int>(PData[chara_ID].pos.z + t_Collision.power);
            break;
        case Left:
            PData[chara_ID].pos.x = static_cast<int>(PData[chara_ID].pos.x - t_Collision.power);
            break;
        default:
            break;
    }

    // 下の当たり判定
    BlockType block = Collision_CB_Under(chara_ID, 0, 3);

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
