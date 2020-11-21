#include "client_common.h"
#include "math.h"
#include "client_move.h"

#define PI 3.14159265358979323846

// ===== * ===== プロトタイプ宣言 ===== * ===== //
float DegreeToRadian(float degree){ return degree * PI / 180.0; }
float RadianToDegree(float radian){ return radian * 180.0 / PI; }

/*ブロックの設置場所を計算する
* 引数
*   player: 設置するプレイヤーの情報
* 返り値
*   PlaceData: 設置するブロックのデータ
*/
PlaceData BuildPlaceData(PlayerData playerData, float handLength){
    PlaceData result;

    result.object = BlockType::NomalBlock;
    result.pos.x = playerData.pos.x / MAP_MAGNIFICATION + sin(playerData.direction) * handLength;
    result.pos.z = playerData.pos.z / MAP_MAGNIFICATION + cos(playerData.direction) * handLength;

    return result;
}

/*プレイヤーの移動向きを返す
* 引数
*   player: 設置するプレイヤーの情報
* 返り値
*   Vector3: 向きベクトル
*   float:   Additional Angle (base value is 0)
*/
Vector3 GetMoveDirection(PlayerData player, float angle){
    Vector3 result;
    result.z = cos(player.direction + DegreeToRadian(angle));    
    result.x = sin(player.direction + DegreeToRadian(angle));
    result.y = 0;
    return result;
}
