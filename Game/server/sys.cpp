
/*----------include 開始----------*/
#include "server_common.h"
#include<time.h>
#include <cmath>
#include <iostream>

/*----------include 終了----------*/
//
/*変数初期化*/
// プレイヤー情報

static int Num_Clients;                    // クライアント人数
static char *Name_Clients[MAX_NUMCLIENTS]; // クライアントの名前
static FloatCube Pos_Clients = {PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W,
                                PLAYER_H, PLAYER_D}; // クライアント情報

PlayerData *PData;

PlaceData PlData = {NonBlock, {0, 0, 0}};

Rank PRank; // キャラランク

ServerMap Map;

time_t StartTime = 0; // ゲーム開始時間

time_t NowTime = 0; // 今の時間

time_t PreveTime = 0; // 前の時間

// クライアント配列の先頭ポインタを返す
const PlayerData *GetPlayerData() { return PData; }

void SetNumClients(int n) // クライアント人数セット
{
  Num_Clients = n;
}

// 名前のセット
// id: クライアントのID
// clientName:クライアントの名前
void SetClientName(int id, char *name) { Name_Clients[id] = name; }

void InitSys(char *file) // システム初期化
{
  Map.LoadMapData(file);
  StartTime = time(NULL);
}

void InitPlayerData() // プレイヤーデータ初期化処理
{
  PData = new PlayerData[Num_Clients];
  PRank.distance = new float[Num_Clients];
  PRank.Rank = new int[Num_Clients];
  for (int i = 0; i < Num_Clients; ++i) {
    strcpy(PData[i].name, Name_Clients[i]);
    PData[i].pos = Pos_Clients;
    PData[i].pos.x = Pos_Clients.x + i * PLAYER_W * 2;
    Vector3 t_v = {0, 0, 0};
    PData[i].velocity = t_v;
    PData[i].direction.horizontal = 0;
    PData[i].rank = 0;
    PData[i].goal = false;
  }
}

void EndSys() // システム終了処理
{
  delete[] PData;
  delete[] PRank.distance;
  delete[] PRank.Rank;
}

void Start_Time(){
  StartTime = time(NULL);
}

// 時間セット
void Set_Time() {
  PreveTime = NowTime;
  NowTime = time(NULL);
}

// 経過時間入手
int Get_Time() {
  return NowTime - StartTime;
}

// タイムを送信
void Send_Per_Time() {
  if (NowTime - PreveTime) {
    RunCommand(BROADCAST, TIMER_COMMAND);
    for (int i = 0; i < Num_Clients; ++i) {
      if (!PData[i].goal) {
        RunCommand(i, RANK_COMMAND);
      }
    }
  }
}

static int BuryCheck_Horizontal(const int chara_ID, const int accuracy,
                                Vector3Int block, const float *point_X,
                                const float *point_Z,
                                const Collision_Dire flag) {
  int chara_size;   // キャラの大きさ
  float base_point; // 計算に使う基準座標
  enum PN_sign { positive = 1, negative = -1 } PN_flag; // 正負を扱う

  // 当たり判定の向きごとの初期化
  switch (flag) {
  case Front:
    chara_size = PData[chara_ID].pos.d;
    base_point = point_Z[accuracy - 1];
    PN_flag = negative;
    break;
  case Right:
    chara_size = PData[chara_ID].pos.w;
    base_point = point_X[0];
    PN_flag = positive;
    break;
  case Left:
    chara_size = PData[chara_ID].pos.w;
    base_point = point_X[accuracy - 1];
    PN_flag = negative;
    break;
  case Back:
    chara_size = PData[chara_ID].pos.d;
    base_point = point_Z[0];
    PN_flag = positive;
    break;
  default:
    throw "BuryCheck_Side : 引数 flag が不正\n";
    break;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  int Bury_Count = 0; // 返り値　埋まり具合

  // 調べる点を順にスキャンしていく
  for (int i = 1; i < (accuracy - 1); ++i) {
    if (flag == Front || flag == Back) {
      block.x = point_X[i] / BLOCK_MAGNIFICATION;
    } else if (flag == Right || flag == Left) {
      block.z = point_Z[i] / BLOCK_MAGNIFICATION;
    }

    int t_Count = 0;

    // どこまで埋まっているか調べる
    for (int j = 0; j <= chara_size; ++j) {
      if (flag == Front || flag == Back) {
        block.z = (base_point + (j * PN_flag)) / BLOCK_MAGNIFICATION;
      } else if (flag == Right || flag == Left) {
        block.x = (base_point + (j * PN_flag)) / BLOCK_MAGNIFICATION;
      }

      if (terrainData[block.x][block.y][block.z] >= NomalBlock) {
        // 最も深くめり込んだ深さを計算
        if (Bury_Count < ++t_Count) {
          ++Bury_Count;

          // 埋まっている(かもしれない)
          if (chara_size <= j) {
            return -1;
          }
        }
      }
      // 埋まっていなければ抜ける
      else {
        break;
      }
    }
  }

  return Bury_Count; // 埋まっているピクセルが返る
}

static int BuryCheck_Vertical(const int chara_ID, const int y,
                              const int accuracy, Vector3Int block,
                              const float *point_X, const float *point_Z,
                              const Collision_Dire flag) {
  int chara_size;   // キャラの大きさ
  float base_point; // 計算に使う基準座標
  enum PN_sign { positive = 1, negative = -1 } PN_flag; // 正負を扱う

  // 当たり判定の向きごとの初期化
  switch (flag) {
  case Under:
    chara_size = PData[chara_ID].pos.h;
    // なぜかvelocityを足さないとグラフィクがぶれる
    // 偉い人計算してください
    base_point = PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y;
    PN_flag = positive;
    break;
  case Over:
    chara_size = PData[chara_ID].pos.h;
    // なぜかvelocityを足さないとグラフィクがぶれる
    // 偉い人計算してください
    base_point = PData[chara_ID].pos.y + PData[chara_ID].pos.h +
                 PData[chara_ID].velocity.y + y;
    PN_flag = negative;
    break;
  default:
    throw "BuryCheck_Under : 引数 flag が不正\n";
    break;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  int Bury_Count = 0; // 返り値　埋まり具合
  int Error_Count = 0;

  for (int i = 1; i < (accuracy - 1); ++i) {
    block.x = point_X[i] / BLOCK_MAGNIFICATION;
    for (int j = 1; j < (accuracy - 1); ++j) {
      block.z = point_Z[i] / BLOCK_MAGNIFICATION;
      if (terrainData[block.x][static_cast<int>(
              base_point / BLOCK_MAGNIFICATION)][block.z] >= NomalBlock) {
        int t_Count = 0;
        // どこまで埋まっているか調べる
        for (int k = 0; k <= chara_size - y; ++k) {

          block.y = (base_point + (k * PN_flag)) / BLOCK_MAGNIFICATION;

          if (terrainData[block.x][block.y][block.z] >= NomalBlock) {
            // その点の埋まっている程度をカウント
            ++t_Count;
            // 埋まっている(かもしれない)
            if (chara_size <= k) {
              ++Error_Count;
              t_Count = 0;
            }
          }
          // 埋まっていなければ抜ける
          else {
            break;
          }
        }
        // 最も埋まっている部分の埋まっている程度にする
        if (std::max(t_Count, Bury_Count) == t_Count) {
          Bury_Count = t_Count;
        }
      } else if (terrainData[block.x][block.y][block.z] == GoalBlock) {
        if (PData[chara_ID].goal == false) {
          Goal(chara_ID);
        }
      }
    }
  }

  // 全体が埋まっていたら
  if (Error_Count == ((accuracy - 2) * (accuracy - 2))) {
    return -1;
  }

  return Bury_Count; // 埋まっているピクセルが返る
}

Collision Collision_CB_Horizontal(const int chara_ID, const int y,
                                  const int accuracy) {
  // 当たり判定の精度が正しいかどうか
  if (accuracy < 3 || PLAYER_D < accuracy || PLAYER_W < accuracy) {
    throw "Collision_CB_Horizontal : 引数　エラー\n";
  }

  Pointer<float> point_X(accuracy); // 調べる座標ｘ
  Pointer<float> point_Z(accuracy); // 調べる座標ｚ

  const float wide = PData[chara_ID].pos.w /
                     (accuracy - 1); // 当たり判定を知らべる座標間距離 x座標
  const float depth = PData[chara_ID].pos.d /
                      (accuracy - 1); // 当たり判定を知らべる座標間距離 z座標

  // 当たり判定を調べる座標をすべて格納
  for (int i = 0; i < accuracy; ++i) {
    point_X[i] = PData[chara_ID].pos.x + PData[chara_ID].velocity.x + wide * i;
    point_Z[i] = PData[chara_ID].pos.z + PData[chara_ID].velocity.z + depth * i;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  // マップ配列用添字　の宣言　と　範囲のエラー処理
  int Block_X = point_X[accuracy - 1] / BLOCK_MAGNIFICATION;

  if (point_X[0] < 0) {
    // throw "Collision_CB_Horizontal : マップ外 : x座標 :負\n";
    fprintf(stderr, "Collision_CB_Horizontal : マップ外 : x座標 :負\n");
    throw error::OutSide_Map_x;
  } else if (MAP_SIZE_W <= Block_X) {
    // throw "Collision_CB_Horizontal : マップ外 : x座標 : 正\n";
    fprintf(stderr, "Collision_CB_Horizontal : マップ外 : x座標 :正\n");
    throw error::OutSide_Map_x;
  }

  const float t_Block_Y = (PData[chara_ID].pos.y + y);
  int Block_Y = t_Block_Y / BLOCK_MAGNIFICATION;

  if (t_Block_Y < 0) {
    // throw "Collision_CB_Horizontal : マップ外 : y座標 : 負\n";
    fprintf(stderr, "Collision_CB_Horizontal : マップ外 : y座標 :負\n");
    throw error::OutSide_Map_y;
  } else if (MAP_SIZE_H <= Block_Y) {
    // throw "Collision_CB_Horizontal : マップ外 : y座標 : 正\n";
    fprintf(stderr, "Collision_CB_Horizontal : マップ外 : y座標 :正\n");
    throw error::OutSide_Map_y;
  }

  int Block_Z = point_Z[accuracy - 1] / BLOCK_MAGNIFICATION;

  if (point_Z[0] < 0) {
    // throw "Collision_CB_Horizontal : マップ外 : z座標 :負\n";
    fprintf(stderr, "Collision_CB_Horizontal : マップ外 : z座標 :負\n");
    throw error::OutSide_Map_z;
  } else if (MAP_SIZE_D <= Block_Z) {
    // throw "Collision_CB_Horizontal : マップ外 : z座標 : 正\n";
    fprintf(stderr, "Collision_CB_Horizontal : マップ外 : z座標 :正\n");
    throw error::OutSide_Map_z;
  }

  Vector3Int block = {Block_X, Block_Y, Block_Z};

  int Count_Front = BuryCheck_Horizontal(chara_ID, accuracy, block,
                                         point_X.Get(), point_Z.Get(), Front);
  int Count_Right = BuryCheck_Horizontal(chara_ID, accuracy, block,
                                         point_X.Get(), point_Z.Get(), Right);
  int Count_Left = BuryCheck_Horizontal(chara_ID, accuracy, block,
                                        point_X.Get(), point_Z.Get(), Left);
  int Count_Back = BuryCheck_Horizontal(chara_ID, accuracy, block,
                                        point_X.Get(), point_Z.Get(), Back);

  if (Count_Front + Count_Right + Count_Left + Count_Back == -4) {
    // throw "Collision_CB_Side : ブロックに埋まってる\n";
    fprintf(stderr, "Collision_CB_Side : ブロックに埋まってる\n");
    Count_Front = 0;
    Count_Right = 0;
    Count_Left = 0;
    Count_Back = 0;
  }

  Collision_Dire t_dire = Non; // 当たり判定で押し戻す方向
  int t_Max_Count = 0;         // 最も多いカウント

  // 最もブロックと重なっているキャラの方向を調べる
  t_Max_Count = std::max(Count_Right, Count_Back);
  if (Count_Right == t_Max_Count) {
    t_dire = Right;
  } else {
    t_dire = Back;
  }

  t_Max_Count = std::max(t_Max_Count, Count_Left);
  if (Count_Left == t_Max_Count) {
    t_dire = Left;
  }

  t_Max_Count = std::max(t_Max_Count, Count_Front);
  if (Count_Front == t_Max_Count) {
    t_dire = Front;
  }

  if (t_Max_Count == 0) {
    t_dire = Non;
  }

  Collision ret; // 返り値
  ret.dire = t_dire;
  ret.power = t_Max_Count;

  // 謎補正
  if (ret.dire == Right || ret.dire == Back) {
    ret.power += 1;
  }

  return ret;
}

Collision Collision_CB_Vertical(const int chara_ID, const int y,
                                const int accuracy) {
  // 当たり判定の精度が正しいかどうか
  if (accuracy < 3 || PLAYER_D < accuracy || PLAYER_W < accuracy) {
    throw "Collision_CB_Vertical : 引数　エラー\n";
  }

  Pointer<float> point_X(accuracy); // 調べる座標ｘ
  Pointer<float> point_Z(accuracy); // 調べる座標ｚ

  const int wide = PData[chara_ID].pos.w /
                   (accuracy - 1); // 当たり判定を知らべる座標間距離 x座標
  const int depth = PData[chara_ID].pos.d /
                    (accuracy - 1); // 当たり判定を知らべる座標間距離 z座標

  // 当たり判定を調べる座標をすべて格納
  for (int i = 0; i < accuracy; ++i) {
    point_X[i] = PData[chara_ID].pos.x + PData[chara_ID].velocity.x + wide * i;
    point_Z[i] = PData[chara_ID].pos.z + PData[chara_ID].velocity.z + depth * i;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  // マップ配列用添字　の宣言　と　範囲のエラー処理
  int Block_X = point_X[accuracy - 1] / BLOCK_MAGNIFICATION;

  if (point_X[0] < 0) {
    // throw "Collision_CB_Vertical : マップ外 : x座標 :負\n";
    fprintf(stderr, "Collision_CB_Vertical : マップ外 : x座標 :負\n");
    throw error::OutSide_Map_x;

  } else if (MAP_SIZE_W <= Block_X) {
    // throw "Collision_CB_Vertical : マップ外 : x座標 : 正\n";
    fprintf(stderr, "Collision_CB_Vertical : マップ外 : x座標 :正\n");
    throw error::OutSide_Map_x;
  }

  const float t_Block_Y =
      (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y);
  int Block_Y = t_Block_Y / BLOCK_MAGNIFICATION;

  if (t_Block_Y < 0) {
    // throw "Collision_CB_Vertical : マップ外 : y座標 : 負\n";
    fprintf(stderr, "Collision_CB_Vertical : マップ外 : y座標 :負\n");
    throw error::OutSide_Map_y;

  } else if (MAP_SIZE_H <= Block_Y) {
    // throw "Collision_CB_Vertical : マップ外 : y座標 : 正\n";
    fprintf(stderr, "Collision_CB_Vertical : マップ外 : x座標 :正\n");
    throw error::OutSide_Map_y;
  }

  int Block_Z = point_Z[accuracy - 1] / BLOCK_MAGNIFICATION;

  if (point_Z[0] < 0) {
    // throw "Collision_CB_Vertical : マップ外 : z座標 :負\n";
    fprintf(stderr, "Collision_CB_Vertical : マップ外 : z座標 :負\n");
    throw error::OutSide_Map_z;
  } else if (MAP_SIZE_D <= Block_Z) {
    // throw "Collision_CB_Vertical : マップ外 : z座標 : 正\n";
    fprintf(stderr, "Collision_CB_Vertical : マップ外 : z座標 :正\n");
    throw error::OutSide_Map_z;
  }

  Vector3Int block = {Block_X, Block_Y, Block_Z};

  int Count_Under = BuryCheck_Vertical(chara_ID, 0, accuracy, block,
                                       point_X.Get(), point_Z.Get(), Under);
  int Count_Over = BuryCheck_Vertical(chara_ID, 0, accuracy, block,
                                      point_X.Get(), point_Z.Get(), Over);

  if (Count_Under == -1) {
    // throw "Collision_CB_Vertical : ブロックに埋まってる\n";
    fprintf(stderr, "Collision_CB_Vertical : ブロックに埋まってる 上\n");
  }
  if (Count_Over == -1) {
    // throw "Collision_CB_Vertical : ブロックに埋まってる\n";
    fprintf(stderr, "Collision_CB_Vertical : ブロックに埋まってる 下\n");
  }

  Collision ret = {Non, 0}; // 返り値

  if (0 < Count_Under) {
    ret.dire = Under;
    ret.power = Count_Under;
  }
  if (0 < Count_Over) {
    ret.dire = Over;
    ret.power = Count_Over;
  }

  return ret;
}

// 中心座標を入れます
static void Get_Chara_Center(const FloatCube player_1, const FloatCube player_2,
                             FloatCube &pos_1, FloatCube &pos_2) {
  // キャラの半径
  pos_1.w = player_1.w / 2;
  pos_2.w = player_2.w / 2;
  pos_1.h = player_1.h / 2;
  pos_2.h = player_2.h / 2;
  pos_1.d = player_1.d / 2;
  pos_2.d = player_2.d / 2;

  // キャラの中心座標
  pos_1.x = player_1.x + pos_1.w;
  pos_2.x = player_2.x + pos_2.w;
  pos_1.y = player_1.y + pos_1.h;
  pos_2.y = player_2.y + pos_2.h;
  pos_1.z = player_1.z + pos_1.d;
  pos_2.z = player_2.z + pos_2.d;
}

// キャラとキャラの当たり判定
static float Collision_CC_Horizontal(FloatCube &player_1, FloatCube &player_2) {
  // キャラの中心座標
  FloatCube Center_1, Center_2;

  // 中心座標 と 半径
  Get_Chara_Center(player_1, player_2, Center_1, Center_2);

  // キャラ間距離
  const float x_distance = fabs(Center_1.x - Center_2.x);
  const float z_distance = fabs(Center_1.z - Center_2.z);

  // キャラ中心間距離
  const float distance =
      fabs(sqrt(x_distance * x_distance + z_distance * z_distance));

  const float radius_1 = (Center_1.w < Center_1.d) ? Center_1.d : Center_2.w;
  const float radius_2 = (Center_2.w < Center_2.d) ? Center_2.d : Center_2.w;

  float overlap = 0; // キャラの重なり
  float angle = 0;   // 2キャラの角度

  if (distance < radius_1 + radius_2) {
    overlap = (radius_1 + radius_2) - distance;
    angle = atan(z_distance / x_distance);

    if (Center_1.x < Center_2.x) {
      player_1.x -= overlap * cos(angle) / 2;
      player_2.x += overlap * cos(angle) / 2;
    } else {
      player_1.x += overlap * cos(angle) / 2;
      player_2.x -= overlap * cos(angle) / 2;
    }

    if (Center_1.y < Center_2.y) {
      player_1.z -= overlap * sin(angle) / 2;
      player_2.z += overlap * sin(angle) / 2;
    } else {
      player_1.z += overlap * sin(angle) / 2;
      player_2.z -= overlap * sin(angle) / 2;
    }

    return overlap / 2;
  }

  return 0;
}

static float Collision_CC_Vertical(FloatCube &player_1, FloatCube &player_2) {
  // キャラの中心座標
  FloatCube Center_1, Center_2;

  // 中心座標 と 半径
  Get_Chara_Center(player_1, player_2, Center_1, Center_2);

  // キャラ間距離
  const float x_distance = fabs(Center_1.x - Center_2.x);
  const float y_distance = fabs(Center_1.y - Center_2.y);
  const float z_distance = fabs(Center_1.z - Center_2.z);

  // キャラ間距離 ２次元平面
  const float xz_distance =
      fabs(sqrt(x_distance * x_distance + z_distance * z_distance));

  // キャラ中心間距離
  const float distance =
      fabs(sqrt(xz_distance * xz_distance + y_distance * y_distance));

  const float radius_1_xz = (Center_1.w < Center_1.d) ? Center_1.d : Center_2.w;
  const float radius_2_xz = (Center_2.w < Center_2.d) ? Center_2.d : Center_2.w;

  float angle = atan(y_distance / xz_distance); // 2キャラの角度 縦
  float overlap =
      (Center_1.h + Center_2.h) - (distance * sin(angle)); // キャラの重なり

  // ２次元平面上で重なっていれば
  if (xz_distance < radius_1_xz + radius_2_xz && 0 < overlap) {
    if (Center_1.y < Center_2.y) {
      player_2.y += overlap / 2;
      player_1.y -= overlap / 2;
    } else {
      player_1.y += overlap / 2;
      player_2.y -= overlap / 2;
    }
    return overlap / 2;
  }

  return 0;
}

// 横と縦を呼び出す
static void Collision_CC(int chara_num) {
  for (int i = 0; i < chara_num; ++i) {
    for (int j = 0; j < chara_num; ++j) {
      if (i < j) {
        break;
      }

      if (i != j) {
        float t = Collision_CC_Horizontal(PData[i].pos, PData[j].pos);
        if (t <= 0) {
          Collision_CC_Vertical(PData[i].pos, PData[j].pos);
        }
      }
    }
  }
}

bool Collision_BB() // ブロックを置けるかどうかの判定
{
  if (PlData.object == NonBlock) {
    return false;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  int Block_X = PlData.pos.x / BLOCK_MAGNIFICATION;
  int Block_Y = PlData.pos.y / BLOCK_MAGNIFICATION;
  int Block_Z = PlData.pos.z / BLOCK_MAGNIFICATION;

  if (Block_X < 0) {
    throw "Collision_BB : マップ外 : x座標 : 負\n";
  } else if (MAP_SIZE_W <= Block_X) {
    throw "Collision_BB : マップ外 : x座標 : 正\n";
  }

  if (Block_Y < 0) {
    throw "Collision_BB : マップ外 : y座標 : 負\n";
  } else if (MAP_SIZE_H <= Block_Y) {
    throw "Collision_BB : マップ外 : y座標 : 正\n";
  }

  if (Block_Z < 0) {
    throw "Collision_BB : マップ外 : z座標 : 負\n";
  } else if (MAP_SIZE_D <= Block_Z) {
    throw "Collision_BB : マップ外 : z座標 : 正\n";
  }

  for (int i = 0; i < Num_Clients; ++i) {
    // キャラの位置をブロックに直したものが入る
    Vector3Int t_Chara_Pos_in_Map;
    t_Chara_Pos_in_Map.x = PData[i].pos.x / BLOCK_MAGNIFICATION;
    t_Chara_Pos_in_Map.y = PData[i].pos.y / BLOCK_MAGNIFICATION;
    t_Chara_Pos_in_Map.z = PData[i].pos.z / BLOCK_MAGNIFICATION;

    if (t_Chara_Pos_in_Map.x == Block_X && t_Chara_Pos_in_Map.y == Block_Y &&
        t_Chara_Pos_in_Map.z == Block_Z) {
      fprintf(stderr, "client[%d] がキャラにブロックぶつけた笑\n", i);
      return false;
    }
  }

  // 置く場所にブロックがないなら
  if (terrainData[Block_X][Block_Y][Block_Z] == NonBlock) {
    // 最下段なら
    // if (Block_Y - 1 < 0) {
    //   // 置けません
    //   return false;
    // }
    // それ以外
    return true;
  }

  // 置けません
  return false;
}

// キャラとブロックの当たり判定
static void Collision_CB(int chara_ID) {
  bool t_bool = MoveHorizontal(chara_ID);
  if (!t_bool) {
    MoveVertical(chara_ID);
  }
}

void Goal(int chara_ID) {
  static int rank = 1;

  if (PData[chara_ID].goal == false) {
    PData[chara_ID].goal = true;
    PData[chara_ID].rank = rank++;
    RunCommand(chara_ID, GOAL_COMMAND);
  }
}

// キャラの縦方向移動
bool MoveVertical(int chara_ID) {
  // 下の当たり判定
  Collision t_Collision_Vertical = Collision_CB_Vertical(chara_ID, 0);

  // 移動後の座標に書き換え
  PData[chara_ID].pos.y += PData[chara_ID].velocity.y;

  bool ret = false;

  // 下の補正
  switch (t_Collision_Vertical.dire) {
  case Under:
    PData[chara_ID].pos.y =
        static_cast<int>(PData[chara_ID].pos.y + t_Collision_Vertical.power);
    ret = true;
    break;
  default:
    break;
  }

  // 上の補正
  switch (t_Collision_Vertical.dire) {
  case Over:
    PData[chara_ID].pos.y =
        static_cast<int>(PData[chara_ID].pos.y - t_Collision_Vertical.power);
    PData[chara_ID].velocity.y = 0;
    ret = true;
    break;
  default:
    break;
  }

  return ret;
}

// キャラの横方向移動
bool MoveHorizontal(int chara_ID) {
  // 横の当たり判定
  Collision t_Collision_Side_Max = {Non, 0};
  Collision t_Collision_Side;

  for (int i = 1; i < PData[chara_ID].pos.h; ++i) {
    t_Collision_Side = Collision_CB_Horizontal(chara_ID, i);
    if (t_Collision_Side_Max.power <= t_Collision_Side.power) {
      t_Collision_Side_Max = t_Collision_Side;
    }
  }

  // 移動後の座標に書き換え
  PData[chara_ID].pos.x += PData[chara_ID].velocity.x;
  PData[chara_ID].pos.z += PData[chara_ID].velocity.z;

  bool ret = false;

  switch (t_Collision_Side_Max.dire) {
  case Front:
    PData[chara_ID].pos.z =
        static_cast<int>(PData[chara_ID].pos.z - t_Collision_Side_Max.power);
    ret = true;
    break;
  case Right:
    PData[chara_ID].pos.x =
        static_cast<int>(PData[chara_ID].pos.x + t_Collision_Side_Max.power);
    ret = true;
    break;
  case Back:
    PData[chara_ID].pos.z =
        static_cast<int>(PData[chara_ID].pos.z + t_Collision_Side_Max.power);
    ret = true;
    break;
  case Left:
    PData[chara_ID].pos.x =
        static_cast<int>(PData[chara_ID].pos.x - t_Collision_Side_Max.power);
    ret = true;
    break;
  default:
    break;
  }

  return ret;
}

void MovePosition(int chara_ID) try {

  // キャラとブロックの当たり判定
  Collision_CB(chara_ID);

  // キャラキャラの当たり判定
  Collision_CC(Num_Clients);

  // 速度を０に戻す
  PData[chara_ID].velocity.x = 0;
  PData[chara_ID].velocity.z = 0;
} catch (error::error e) {
  if (e == error::OutSide_Map_x || e == error::OutSide_Map_y || e == error::OutSide_Map_z) {
    PData[chara_ID].pos.x = PLAYER_X + chara_ID * PLAYER_W;
    PData[chara_ID].pos.y = PLAYER_Y;
    PData[chara_ID].pos.z = PLAYER_Z;if (e == error::OutSide_Map_y) {
    PData[chara_ID].pos.x = PLAYER_X + chara_ID * PLAYER_W;
    PData[chara_ID].pos.y = PLAYER_Y;
    PData[chara_ID].pos.z = PLAYER_Z;
    PData[chara_ID].velocity.x = 0;
    // PData[chara_ID].velocity.y = 0;
    PData[chara_ID].velocity.z = 0;
  }
    PData[chara_ID].velocity.x = 0;
    // PData[chara_ID].velocity.y = 0;
    PData[chara_ID].velocity.z = 0;
  }
} catch (const char *const e) {
  fprintf(stderr, "%s", e);
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
  else {
    // データ書き換え
    PlData.object = NonBlock;
    // ブロックを置くクライアントのみに送信
    RunCommand(chara_ID, PUT_COMMAND);
  }
}

int AllGoal() {
  for (int i = 0; i < Num_Clients; ++i) {
    // ゴールしていないクライアントがいれば
    if (!PData[i].goal) {
      return 0;
    }
  }

  RunCommand(BROADCAST, FINISH_COMMAND);

  return 1;
}

// クライアントの速度ベクトルをセット
// chara_ID:クライアントのID
// pos:クライアントの座標
void SetVec(int chara_ID, Vector3 &vec) {
  PData[chara_ID].velocity.x = vec.x;
  PData[chara_ID].velocity.y = vec.y;
  PData[chara_ID].velocity.z = vec.z;
}

PlaceData GetPlaceData() { return PlData; }

void SetPlaceData(PlaceData &data) { PlData = data; }

/*全員に座標を送る
 *
 * 引数
 *     int client_num: クライアントの人数
 */
void SendAllPos(int client_num) {
  char com = MOVE_COMMAND;
  // 全員に座標を送る
  for (int i = 0; i < client_num; ++i) {
    // 特定のIDにコマンドを送る
    RunCommand(i, com);
  }
}

// 順位取得
void SetRank() {
  for (int i = 0; i < Num_Clients; ++i) {
    PRank.distance[i] = Map.GetMapD() * BLOCK_MAGNIFICATION - PData[i].pos.z;
    if (PData[i].rank != 0) {
      PRank.distance[i] = PData[i].rank;
    }
  }

  float max_distance = 0;
  int rank = Num_Clients;
  for (int i = 0; i < Num_Clients; ++i) {
    for (int j = 0; j < Num_Clients; ++j) {
      max_distance = std::max(max_distance, PRank.distance[j]);
    }
    if (max_distance == 0) {
      break;
    }
    for (int k = 0; k < Num_Clients; ++k) {
      if (max_distance == PRank.distance[k]) {
        PRank.Rank[k] = rank;
        --rank;
      //  fprintf(stderr, "%d %f %f\n", PRank.Rank[k], PRank.distance[k], max_distance);
        PRank.distance[k] = 0;
        max_distance = 0;
        break;
      }
    }
    // fprintf(stderr, "\n");
  }
}

// ネットワークにあげる
int GetRank(int chara_ID) {
  return PRank.Rank[chara_ID];
}

// 順位送信
void SendRank(int chara_ID) {
  for (int i = 0; i < Num_Clients; ++i) {
    //fprintf(stderr, "[%d]  %d\n", i, PRank.Rank[i]);
    RunCommand(i, RANK_COMMAND);
  }
}

// システムにクライアントの角度を渡す
void SetDirection(int chara_ID, float direction) {
  PData[chara_ID].direction.horizontal = direction;
}

// ネットワークにクライアントの角度を渡す
float GetDirection(int chara_ID) {
  return PData[chara_ID].direction.horizontal;
}
