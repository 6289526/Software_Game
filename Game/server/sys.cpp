
/*----------include 開始----------*/
#include "server_common.h"
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

ServerMap Map;

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
}

void InitPlayerData() // プレイヤーデータ初期化処理
{
  PData = new PlayerData[Num_Clients];
  for (int i = 0; i < Num_Clients; ++i) {
    strcpy(PData[i].name, Name_Clients[i]);
    PData[i].pos = Pos_Clients;
    PData[i].pos.x = Pos_Clients.x + i * 20;
    Vector3 t_v = {0, 0, 0};
    PData[i].velocity = t_v;
    PData[i].direction = 0;
    PData[i].rank = 0;
    PData[i].goal = false;
  }
}

void EndSys() // システム終了処理
{
  delete[] PData;
}

static int BuryCheck_Side(const int chara_ID, const int accuracy, Vector3Int block,
                   const float *point_X, const float *point_Z,
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
      block.x = point_X[i] / MAP_MAGNIFICATION;
    } else if (flag == Right || flag == Left) {
      block.z = point_Z[i] / MAP_MAGNIFICATION;
    }

    int t_Count = 0;

    // どこまで埋まっているか調べる
    for (int j = 0; j <= chara_size; ++j) {
      if (flag == Front || flag == Back) {
        block.z = (base_point + (j * PN_flag)) / MAP_MAGNIFICATION;
      } else if (flag == Right || flag == Left) {
        block.x = (base_point + (j * PN_flag)) / MAP_MAGNIFICATION;
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

static int BuryCheck_Under(const int chara_ID, const int y, const int accuracy,
                           Vector3Int block, const float *point_X,
                           const float *point_Z, const Collision_Dire flag) {
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
  default:
    throw "BuryCheck_Under : 引数 flag が不正\n";
    break;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  int Bury_Count = 0; // 返り値　埋まり具合
  int Errer_Count = 0;

  for (int i = 1; i < (accuracy - 1); ++i) {
    block.x = point_X[i] / MAP_MAGNIFICATION;
    for (int j = 1; j < (accuracy - 1); ++j) {
      block.z = point_Z[i] / MAP_MAGNIFICATION;
      if (terrainData[block.x][static_cast<int>(base_point / MAP_MAGNIFICATION)]
                     [block.z] >= NomalBlock) {
        int t_Count = 0;
        // どこまで埋まっているか調べる
        for (int k = 0; k <= chara_size - y; ++k) {

          block.y = (base_point + (k * PN_flag)) / MAP_MAGNIFICATION;

          if (terrainData[block.x][block.y][block.z] >= NomalBlock) {
            // その点の埋まっている程度をカウント
            ++t_Count;
            // 埋まっている(かもしれない)
            if (chara_size <= k) {
              ++Errer_Count;
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
  if (Errer_Count == ((accuracy - 2) * (accuracy - 2))) {
    return -1;
  }

  return Bury_Count; // 埋まっているピクセルが返る
}


static int BuryCheck_Over(const int chara_ID, const int y, const int accuracy,
                           Vector3Int block, const float *point_X,
                           const float *point_Z, const Collision_Dire flag) {
  int chara_size;   // キャラの大きさ
  float base_point; // 計算に使う基準座標
  enum PN_sign { positive = 1, negative = -1 } PN_flag; // 正負を扱う

  // 当たり判定の向きごとの初期化
  switch (flag) {
  case Over:
    chara_size = PData[chara_ID].pos.h;
    // なぜかvelocityを足さないとグラフィクがぶれる
    // 偉い人計算してください
    base_point = PData[chara_ID].pos.y + PData[chara_ID].pos.h + PData[chara_ID].velocity.y + y;
    PN_flag = negative;
    break;
  default:
    throw "BuryCheck_Over : 引数 flag が不正\n";
    break;
  }

  // マップデータ入手
  const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

  int Bury_Count = 0; // 返り値　埋まり具合
  int Errer_Count = 0;

  for (int i = 1; i < (accuracy - 1); ++i) {
    block.x = point_X[i] / MAP_MAGNIFICATION;
    for (int j = 1; j < (accuracy - 1); ++j) {
      block.z = point_Z[i] / MAP_MAGNIFICATION;
      if (terrainData[block.x][static_cast<int>(base_point / MAP_MAGNIFICATION)]
                     [block.z] >= NomalBlock) {
        int t_Count = 0;
        // どこまで埋まっているか調べる
        for (int k = 0; k <= chara_size - y; ++k) {

          block.y = (base_point + (k * PN_flag)) / MAP_MAGNIFICATION;

          if (terrainData[block.x][block.y][block.z] >= NomalBlock) {
            // その点の埋まっている程度をカウント
            ++t_Count;
            // 埋まっている(かもしれない)
            if (chara_size <= k) {
              ++Errer_Count;
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
  if (Errer_Count == ((accuracy - 2) * (accuracy - 2))) {
    return -1;
  }

  return Bury_Count; // 埋まっているピクセルが返る
}

Collision Collision_CB_Side(const int chara_ID, const int y,
                            const int accuracy) {
  // 当たり判定の精度が正しいかどうか
  if (accuracy < 3 || PLAYER_D < accuracy || PLAYER_W < accuracy) {
    throw "Collision_CB_Side : 引数　エラー\n";
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
  int Block_X = point_X[accuracy - 1] / MAP_MAGNIFICATION;

  if (point_X[0] < 0) {
    throw "Collision_CB_Side : マップ外 : x座標 :負\n";
  } else if (MAP_SIZE_W <= Block_X) {
    throw "Collision_CB_Side : マップ外 : x座標 : 正\n";
  }

  const float t_Block_Y = (PData[chara_ID].pos.y + y);
  int Block_Y = t_Block_Y / MAP_MAGNIFICATION;

  if (t_Block_Y < 0) {
    throw "Collision_CB_Side : マップ外 : y座標 : 負\n";
  } else if (MAP_SIZE_H <= Block_Y) {
    throw "Collision_CB_Side : マップ外 : y座標 : 正\n";
  }

  int Block_Z = point_Z[accuracy - 1] / MAP_MAGNIFICATION;

  if (point_Z[0] < 0) {
    throw "Collision_CB_Side : マップ外 : z座標 :負\n";
  } else if (MAP_SIZE_D <= Block_Z) {
    throw "Collision_CB_Side : マップ外 : z座標 : 正\n";
  }

  Vector3Int block = {Block_X, Block_Y, Block_Z};

  int Count_Front =
      BuryCheck_Side(chara_ID, accuracy, block,
                     point_X.Get(), point_Z.Get(), Front);
  int Count_Right =
      BuryCheck_Side(chara_ID, accuracy, block,
                     point_X.Get(), point_Z.Get(), Right);
  int Count_Left = BuryCheck_Side(chara_ID, accuracy, block,
                                  point_X.Get(), point_Z.Get(), Left);
  int Count_Back = BuryCheck_Side(chara_ID, accuracy, block,
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

  // なんかよくわからん補正　これがないとグラフィックがぶれる
  // これでうまく行けるんで書いときます　誰か計算して理由教えて
  if (ret.dire == Left || ret.dire == Front) {
    ret.power -= 1;
  }

  return ret;
}

Collision Collision_CB_Under(const int chara_ID, const int y,
                             const int accuracy) {
  // 当たり判定の精度が正しいかどうか
  if (accuracy < 3 || PLAYER_D < accuracy || PLAYER_W < accuracy) {
    throw "Collision_CB_Under : 引数　エラー\n";
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
  int Block_X = point_X[accuracy - 1] / MAP_MAGNIFICATION;

  if (point_X[0] < 0) {
    throw "Collision_CB_Under : マップ外 : x座標 :負\n";
  } else if (MAP_SIZE_W <= Block_X) {
    throw "Collision_CB_Under : マップ外 : x座標 : 正\n";
  }

  const float t_Block_Y =
      (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y);
  int Block_Y = t_Block_Y / MAP_MAGNIFICATION;

  if (t_Block_Y < 0) {
    fprintf(stderr, "%f", PData[chara_ID].velocity.y);
    throw "Collision_CB_Under : マップ外 : y座標 : 負 \n";
  } else if (MAP_SIZE_H <= Block_Y) {
    throw "Collision_CB_Under : マップ外 : y座標 : 正\n";
  }

  int Block_Z = point_Z[accuracy - 1] / MAP_MAGNIFICATION;

  if (point_Z[0] < 0) {
    throw "Collision_CB_Under : マップ外 : z座標 :負\n";
  } else if (MAP_SIZE_D <= Block_Z) {
    throw "Collision_CB_Under : マップ外 : z座標 : 正\n";
  }

  Vector3Int block = {Block_X, Block_Y, Block_Z};

  int Count_Under =
      BuryCheck_Under(chara_ID, 0, accuracy, block,
                      point_X.Get(), point_Z.Get(), Under);

  if (Count_Under == -1) {
    // throw "Collision_CB_Under : ブロックに埋まってる\n";
    fprintf(stderr, "Collision_CB_Under : ブロックに埋まってる\n");
  }

  Collision ret = {Non, 0}; // 返り値

  if (0 < Count_Under) {
    ret.dire = Under;
    ret.power = Count_Under;
  }

  return ret;
}

Collision Collision_CB_Over(const int chara_ID, const int y,
                             const int accuracy) {
  // 当たり判定の精度が正しいかどうか
  if (accuracy < 3 || PLAYER_D < accuracy || PLAYER_W < accuracy) {
    throw "Collision_CB_Under : 引数　エラー\n";
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
  int Block_X = point_X[accuracy - 1] / MAP_MAGNIFICATION;

  if (point_X[0] < 0) {
    throw "Collision_CB_Under : マップ外 : x座標 :負\n";
  } else if (MAP_SIZE_W <= Block_X) {
    throw "Collision_CB_Under : マップ外 : x座標 : 正\n";
  }

  const float t_Block_Y =
      (PData[chara_ID].pos.y + PData[chara_ID].velocity.y + y);
  int Block_Y = t_Block_Y / MAP_MAGNIFICATION;

  if (t_Block_Y < 0) {
    throw "Collision_CB_Under : マップ外 : y座標 : 負\n";
  } else if (MAP_SIZE_H <= Block_Y) {
    throw "Collision_CB_Under : マップ外 : y座標 : 正\n";
  }

  int Block_Z = point_Z[accuracy - 1] / MAP_MAGNIFICATION;

  if (point_Z[0] < 0) {
    throw "Collision_CB_Under : マップ外 : z座標 :負\n";
  } else if (MAP_SIZE_D <= Block_Z) {
    throw "Collision_CB_Under : マップ外 : z座標 : 正\n";
  }

  Vector3Int block = {Block_X, Block_Y, Block_Z};

  int Count_Over =
      BuryCheck_Over(chara_ID, 0, accuracy, block,
                      point_X.Get(), point_Z.Get(), Over);

  if (Count_Over == -1) {
    // throw "Collision_CB_Under : ブロックに埋まってる\n";
    fprintf(stderr, "Collision_CB_Over : ブロックに埋まってる\n");
  }

  Collision ret = {Non, 0}; // 返り値

  if (0 < Count_Over) {
    ret.dire = Over;
    ret.power = Count_Over;
  }

  return ret;
}

// キャラとキャラの当たり判定
static void Collision_CC_Side(FloatCube &player_1, FloatCube &player_2) {
  // キャラの半径
  const float radius_1 =
      (player_1.w < player_1.d) ? player_1.d / 2 : player_1.w / 2;
  const float radius_2 =
      (player_2.w < player_2.d) ? player_2.d / 2 : player_2.w / 2;

  // キャラの中心座標
  Vector2 Center_1, Center_2;

  Center_1.x = player_1.x + radius_1;
  Center_2.x = player_2.x + radius_2;

  // y じゃなくて z だよ
  Center_1.y = player_1.z + radius_1;
  Center_2.y = player_2.z + radius_2;

  // キャラ間距離
  const float x_distance = fabs(Center_1.x - Center_2.x);
  const float y_distance = fabs(Center_1.y - Center_2.y);
  const float distance =
      fabs(sqrt(x_distance * x_distance + y_distance * y_distance));

  float overlap = 0; // キャラの重なり
  float angle = 0;   // 2キャラの角度

  if (distance < radius_1 + radius_2) {
    overlap = (radius_1 + radius_2) - distance;
    angle = atan(y_distance / x_distance);

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
  }
}

// 横と縦を呼び出す
static void Collision_CC(int chara_num) {
  for (int i = 0; i < chara_num; ++i) {
    for (int j = 0; j < chara_num; ++j) {
      if (i < j) {
        break;
      }

      if (i != j) {
        Collision_CC_Side(PData[i].pos, PData[j].pos);
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

  int Block_X = PlData.pos.x / MAP_MAGNIFICATION;
  int Block_Y = PlData.pos.y / MAP_MAGNIFICATION;
  int Block_Z = PlData.pos.z / MAP_MAGNIFICATION;

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
    t_Chara_Pos_in_Map.x = PData[i].pos.x / MAP_MAGNIFICATION;
    t_Chara_Pos_in_Map.y = PData[i].pos.y / MAP_MAGNIFICATION;
    t_Chara_Pos_in_Map.z = PData[i].pos.z / MAP_MAGNIFICATION;

    if (t_Chara_Pos_in_Map.x == Block_X && t_Chara_Pos_in_Map.y == Block_Y &&
        t_Chara_Pos_in_Map.z == Block_Z) {
      fprintf(stderr, "client[%d] がキャラにブロックぶつけた笑\n", i);
      return false;
    }
  }

  // 置く場所にブロックがないなら
  if (terrainData[Block_X][Block_Y][Block_Z] == NonBlock) {
    // 最下段なら
    if (Block_Y - 1 < 0) {
      // 置けません
      return false;
    }
    // それ以外
    return true;
  }

  // 置けません
  return false;
}

// キャラとブロックの当たり判定
static void Collision_CB(int chara_ID) {
  // 下の当たり判定
  Collision t_Collision_Under = Collision_CB_Under(chara_ID, 0);

  // 上の当たり判定
  Collision t_Collision_Over = Collision_CB_Over(chara_ID, 0);

  // 移動後の座標に書き換え
  PData[chara_ID].pos.y += PData[chara_ID].velocity.y;

  // 下の補正
  switch (t_Collision_Under.dire) {
  case Under:
    PData[chara_ID].pos.y =
        static_cast<int>(PData[chara_ID].pos.y + t_Collision_Under.power);
    break;
  default:
    break;
  }

  // 上の補正
  switch (t_Collision_Over.dire) {
  case Over:
    PData[chara_ID].pos.y =
        static_cast<int>(PData[chara_ID].pos.y - t_Collision_Over.power);
        PData[chara_ID].velocity.y = 0;
    break;
  default:
    break;
  }

  // 横の当たり判定
  Collision t_Collision_Side_Max = {Non, 0};
  Collision t_Collision_Side;


  for (int i = 1; i < PData[chara_ID].pos.h; ++i) {
    t_Collision_Side = Collision_CB_Side(chara_ID, i);
    if (t_Collision_Side_Max.power <= t_Collision_Side.power) {
      t_Collision_Side_Max = t_Collision_Side;
    }
  }

  // 移動後の座標に書き換え
  PData[chara_ID].pos.x += PData[chara_ID].velocity.x;
  PData[chara_ID].pos.z += PData[chara_ID].velocity.z;

  switch (t_Collision_Side_Max.dire) {
  case Front:
    PData[chara_ID].pos.z =
        static_cast<int>(PData[chara_ID].pos.z - t_Collision_Side_Max.power);
    break;
  case Right:
    PData[chara_ID].pos.x =
        static_cast<int>(PData[chara_ID].pos.x + t_Collision_Side_Max.power);
    break;
  case Back:
    PData[chara_ID].pos.z =
        static_cast<int>(PData[chara_ID].pos.z + t_Collision_Side_Max.power);
    break;
  case Left:
    PData[chara_ID].pos.x =
        static_cast<int>(PData[chara_ID].pos.x - t_Collision_Side_Max.power);
    break;
  default:
    break;
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

void MovePosition(int chara_ID) {

  // キャラキャラの当たり判定
  Collision_CC(Num_Clients);

  // キャラとブロックの当たり判定
  Collision_CB(chara_ID);

  // 速度を０に戻す
  PData[chara_ID].velocity.x = 0;
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

// システムにクライアントの角度を渡す
void SetDirection(int chara_ID, float direction) {
  PData[chara_ID].direction = direction;
}

// ネットワークにクライアントの角度を渡す
float GetDirection(int chara_ID) {
  return PData[chara_ID].direction;
}
