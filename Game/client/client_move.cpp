#include "client_common.h"
#include "math.h"

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
