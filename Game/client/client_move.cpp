#include "client_common.h"
#include "math.h"

#define PI 3.14159265358979323846

float DegreeToRadian(float degree){ return degree * PI / 180.0; }
float RadianToDegree(float radian){ return radian * 180.0 / PI; }

/*ブロックの設置場所を計算する
* 引数
*   player: 設置するプレイヤーの情報
* 返り値
*   PlaceData: 設置するブロックのデータ
*/
PlaceData BuildPlaceData(PlayerData player){
    PlaceData data;
    float handLength = 3.0f;

    data.object = BlockType::NomalBlock;
    data.pos.x = player.pos.x / MAP_MAGNIFICATION + sin(player.direction) * handLength;
    data.pos.z = player.pos.z / MAP_MAGNIFICATION + cos(player.direction) * handLength;

    return data;
}

/*プレイヤーの移動向きを返す
* 引数
*   player: 設置するプレイヤーの情報
* 返り値
*   Vector3: 向きベクトル
*   float:   Additional Angle (base value is 0)
*/
Vector3 GetMoveDirection(PlayerData player, float angle = 0.0f){
    Vector3 result;
    result.z = cos(player.direction + DegreeToRadian(angle));    
    result.x = sin(player.direction + DegreeToRadian(angle));
    result.y = 0;
    return result;
}
