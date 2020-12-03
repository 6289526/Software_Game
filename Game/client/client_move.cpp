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

    for (; result.y >= 0; result.y--) // yを減らしていく = つけると...
    {
        if (terrainData[result.x][result.y][result.z] != BlockType::NonBlock)
            return result;
    }
    return result;
}

bool IsPlayerOnGroundSimple(){
    const PlayerData *pData = GetPlayerData();
    const int myId = GetMyID();
	float result = 0;

	const int accuracy = 3; 
	float pointX[accuracy], pointZ[accuracy];
	float pointY = pData[myId].pos.y;
	const int width = pData[myId].pos.w / (accuracy - 1); // X座標
	const int depth = pData[myId].pos.d / (accuracy - 1);	// Z座標
	const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();
	const int blockX = pointX[accuracy - 1] / MAP_MAGNIFICATION;
	const int blockY = pointY / MAP_MAGNIFICATION;
	const int blockZ = pointZ[accuracy - 1] / MAP_MAGNIFICATION;

	for (int i = 0; i < accuracy; i++)
	{
		pointX[i] = pData[myId].pos.x + width * i;
		pointZ[i] = pData[myId].pos.z + depth * i;
	}

#pragma region 範囲エラー処理
	if (pointX[0] < 0)
		throw "マップ外 : x座標 負\n";
	else if (MAP_SIZE_W <= blockX)
		throw "マップ外 : x座標 正\n";

	if (pointY < 0)
		throw "マップ外 : y座標 : 負\n";
	else if (MAP_SIZE_H <= blockY)
		throw "マップ外 : y座標 : 正\n";
	
	if (pointZ[0] < 0)
		throw "マップ外 : z座標 :負\n";
	else if (MAP_SIZE_D <= blockZ)
		throw "マップ外 : z座標 :正\n";
#pragma endregion

    for (int i = 0; i < accuracy; i++)
    {
        for (int j = 0; j < accuracy; j++)
        {
            Vector3 playerPos = {pointX[i], pointY, pointZ[j]};
            Vector3Int blockIndex = GetTopOfHeightBlockIndex(playerPos);
            Vector3 blockPos = {blockIndex.x * MAP_MAGNIFICATION, blockIndex.y * MAP_MAGNIFICATION, blockIndex.z * MAP_MAGNIFICATION};

            result = pData[myId].pos.y - blockPos.y - MAP_MAGNIFICATION;
            if(result == 0 )
                return true;
        }
    }
    

	// fprintf(stderr, "(p, b): (%.2f, %.2f), distance: %.4f\n",pData[myId].pos.y, blockPos.y, result);
	return false;
}
