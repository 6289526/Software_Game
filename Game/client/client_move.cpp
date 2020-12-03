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

    result.object = BlockType::MetalBlock;
    result.pos.x = playerData.pos.x + sin(playerData.direction) * handLength;
    result.pos.z = playerData.pos.z + cos(playerData.direction) * handLength;
    // Vector2Int v2 = {result.pos.x / MAP_MAGNIFICATION, result.pos.z / MAP_MAGNIFICATION};
    result.pos.y = playerData.pos.y;

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

int GetPutableBlockHeightFromMap(Vector2Int pos){
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

    for (int height = 0; height < MAP_SIZE_H; height++)
    {
        if (terrainData[pos.x][height][pos.y] == BlockType::NonBlock)
        {
            return height * MAP_MAGNIFICATION + MAP_MAGNIFICATION / 2;
        }
    }

    fprintf(stderr, "(%d, %d)はすべてブロックで埋まっています\n", pos.x, pos.y);
    return MAP_SIZE_H;
}

Vector3Int GetTopOfHeightBlockIndex(Vector3 position){ 
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

    Vector3Int result = {(int)(position.x / MAP_MAGNIFICATION), (int)(position.y / MAP_MAGNIFICATION), (int)(position.z / MAP_MAGNIFICATION)};
    bool isFill = false;

    for (; result.y >= 0; result.y--) // yを減らしていく = つけると...
    {
        fprintf(stderr, "[%d, %d, %d], %d\n", result.x, result.y, result.z, terrainData[result.x][result.y][result.z]);
        if (terrainData[result.x][result.y][result.z] != BlockType::NonBlock && isFill)
            break;
        else
            isFill = false;
    }
    return result;
}

bool IsPlayerOnGroundSimple(){
    const PlayerData *pData = GetPlayerData();
    const int myId = GetMyID();
	float result = 0;
    Vector3 playerPos = {pData[myId].pos.x, pData[myId].pos.y, pData[myId].pos.z};
	Vector3Int blockIndex = GetTopOfHeightBlockIndex(playerPos);
    Vector3 blockPos = {blockIndex.x * MAP_MAGNIFICATION, blockIndex.y * MAP_MAGNIFICATION, blockIndex.z * MAP_MAGNIFICATION};

    result = pData[myId].pos.y - blockPos.y -40;

	fprintf(stderr, "(p, b): (%.2f, %.2f), distance: %.4f\n",pData[myId].pos.y, blockPos.y, result);
	return result == 0;
}
