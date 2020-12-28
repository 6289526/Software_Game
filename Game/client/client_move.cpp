#include "client_move.h"
using namespace Mover;

MoveCalculator::MoveCalculator(System::ClientSystem *system){
	_System = system;
}

/*ブロックの設置場所を計算する
* 引数
*   player: 設置するプレイヤーの情報
* 返り値
*   PlaceData: 設置するブロックのデータ
*/
PlaceData MoveCalculator::BuildPlaceData(PlayerData playerData, float handLength){
    PlaceData result;

    result.object = BlockType::MetalBlock;
    result.pos.x = playerData.pos.x + sin(playerData.direction.horizontal) * handLength;
	result.pos.y = playerData.pos.y + sin(playerData.direction.vertical) * handLength;
    result.pos.z = playerData.pos.z + cos(playerData.direction.horizontal) * handLength;
    // Vector2Int v2 = {result.pos.x / MAP_MAGNIFICATION, result.pos.z / MAP_MAGNIFICATION};

	// fprintf(stderr, "rot: (%.2f, %.2f)\n", playerData.direction.horizontal, playerData.direction.vertical, playerData.direction.vertical);

    return result;
}

/*プレイヤーの移動向きを返す
* 引数
*   player: 設置するプレイヤーの情報
* 返り値
*   Vector3: 向きベクトル
*   float:   Additional Angle (base value is 0)
*/
Vector3 MoveCalculator::GetMoveDirection(PlayerData player, float angle){
    Vector3 result;
    result.z = cos(player.direction.horizontal + DegreeToRadian(angle));
    result.x = sin(player.direction.horizontal + DegreeToRadian(angle));
    result.y = 0;
    return result;
}

int MoveCalculator::GetPutableBlockHeightFromMap(Vector2Int pos){
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = _System->GetClientMap().GetTerrainData();

    for (int height = 0; height < MAP_SIZE_H; height++)
    {
        if (terrainData[pos.x][height][pos.y] == BlockType::NonBlock)
        {
            return height * BLOCK_MAGNIFICATION + BLOCK_MAGNIFICATION / 2;
        }
    }

    fprintf(stderr, "(%d, %d)はすべてブロックで埋まっています\n", pos.x, pos.y);
    return MAP_SIZE_H;
}

Vector3Int MoveCalculator::GetTopOfHeightBlockIndex(Vector3 position){
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = _System->GetClientMap().GetTerrainData();

    Vector3Int result = {(int)(position.x / BLOCK_MAGNIFICATION), (int)(position.y / BLOCK_MAGNIFICATION), (int)(position.z / BLOCK_MAGNIFICATION)};

    for (; result.y >= 0; result.y--) // yを減らしていく = つけると...
    {
        if (terrainData[result.x][result.y][result.z] > BlockType::NonBlock)
            return result;
    }
    return result;
}

bool MoveCalculator::IsPlayerOnGroundSimple(){
    const PlayerData *pData = _System->GetPlayerData();
    const int myId = _System->GetMyID();
	float result = 0;

	const int accuracy = 3;
	float pointX[accuracy], pointZ[accuracy];
	float pointY = pData[myId].pos.y;
	const int width = pData[myId].pos.w / (accuracy - 1); // X座標
	const int depth = pData[myId].pos.d / (accuracy - 1);	// Z座標
	const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = _System->GetClientMap().GetTerrainData();
	const int blockX = pointX[accuracy - 1] / BLOCK_MAGNIFICATION;
	const int blockY = pointY / BLOCK_MAGNIFICATION;
	const int blockZ = pointZ[accuracy - 1] / BLOCK_MAGNIFICATION;

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
            Vector3 blockPos = {blockIndex.x * BLOCK_MAGNIFICATION, blockIndex.y * BLOCK_MAGNIFICATION, blockIndex.z * BLOCK_MAGNIFICATION};

            result = pData[myId].pos.y - blockPos.y - BLOCK_MAGNIFICATION;
            if(result == 0 )
                return true;
        }
    }


	// fprintf(stderr, "(p, b): (%.2f, %.2f), distance: %.4f\n",pData[myId].pos.y, blockPos.y, result);
	return false;
}

pair<bool, bool> MoveCalculator::SetPlayerVelocity(InputModuleBase *inputModule, PlayerData *pData, Timer *timer){

	if (inputModule == NULL || pData == NULL || timer == NULL) {
		fprintf(stderr,"Error: input, pData, timerのいずれかがNULLです\n");
		return make_pair(false, false);
	}

	// fprintf(stderr,"vel: (%.3f, %.3f, %.3f), pos: (%.2f, %.2f, %.2f), rot: (%.2f, %.2f), JPG: (%d, %d, %d)\n", pData->velocity.x, pData->velocity.y, pData->velocity.z, pData->pos.x, pData->pos.y, pData->pos.z, pData->direction.horizontal, pData->direction.vertical, isJumped, isPreGround, isOnGround);

	if (!USE_GRAVITY){
		return make_pair(DisUseGravity(inputModule, pData, timer), false);
	}
	else
	{
		InputType data = inputModule->SystemGetInputType();

		try
		{
			isPreGround = isOnGround;
			isOnGround = IsPlayerOnGroundSimple();
		}
		catch (char const *e) // エラー処理
		{
			fprintf(stderr, "%s", e);
		}

		pData->velocity.x = 0;

		if (isOnGround && !isJumped){
			pData->velocity.y = 0;}

		if (!isPreGround && isOnGround)
			isJumped = false;

		pData->velocity.z = 0;

		if (inputModule->IsMoveButtonDown() || !isOnGround)
		{
			if (data.Forward) // 前進 : W Key
			{
				data.Forward = false;
				if (strcmp(WiiAddress, "") != 0)
				{
					pData->velocity.x += GetMoveDirection(*pData, 0).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
					pData->velocity.z += GetMoveDirection(*pData, 0).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
				}
				else
				{
					pData->velocity.x += GetMoveDirection(*pData, 0).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
					pData->velocity.z += GetMoveDirection(*pData, 0).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
				}
			}
			if (data.Left || data.L) // 左 : A Key
			{
				data.Left = false;
				data.L = false;
				pData->direction.horizontal += PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
			}
			if (data.Right || data.R) // 右 : D Key
			{
				data.Right = false;
				data.R = false;
				pData->direction.horizontal -= PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
			}
			if (data.U) // 上回転 : Up Key
			{
				data.U = false;
				pData->direction.vertical += PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
			}
			if (data.D) // 下回転 : Down Key
			{
				data.D = false;
				pData->direction.vertical -= PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
			}

			// ジャンプ
			if (data.Jump && isOnGround == 1)
			{
				data.Jump = false;
				pData->velocity.y += PLAYER_JUMP_POWER;
				isJumped = true;
				_System->GetSounfController().GetSoundEffectSubject().OnNest(Sound::Jump);
			}
			else if (!isOnGround)
			{
				pData->velocity.y -= GRAVITY * timer->GetDeltaTime();
			}

#pragma region 速度制限, 角度制限
			if (TERMINAL_SPEED < pData->velocity.x)
				pData->velocity.x = TERMINAL_SPEED;
			if (TERMINAL_SPEED < pData->velocity.y)
				pData->velocity.y = TERMINAL_SPEED;
			if (TERMINAL_SPEED < pData->velocity.z)
				pData->velocity.z = TERMINAL_SPEED;
				
			if (pData->direction.vertical > MAXIMUM_ANGLE)
				pData->direction.vertical = MAXIMUM_ANGLE;
			if (pData->direction.vertical < -MAXIMUM_ANGLE)
				pData->direction.vertical = -MAXIMUM_ANGLE;
#pragma endregion

			return make_pair(true, isJumped);
		}
	}

	return make_pair(false, isJumped);
}

bool MoveCalculator::DisUseGravity(InputModuleBase *inputModule, PlayerData *pData, Timer *timer){
	pData->velocity = {0, 0, 0};

	InputType data = inputModule->SystemGetInputType();

	if (inputModule->IsMoveButtonDown())
	{
		if (data.U)
		{
			pData->velocity.y -= 1;
		}
		// 前
		if (data.Forward)
		{
			if (strcmp(WiiAddress, "") != 0)
			{
				pData->velocity.x += GetMoveDirection(*pData, 0).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
				pData->velocity.z += GetMoveDirection(*pData, 0).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			}
			else
			{
				pData->velocity.x += GetMoveDirection(*pData, 0).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
				pData->velocity.z += GetMoveDirection(*pData, 0).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			}
		}
		// 左右
		if (data.Left)
		{
			// if(strcmp(WiiAddress, "") != 0){
			// 	pData->velocity.x += 5*GetMoveDirection(*pData, 90).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			// 	pData->velocity.z += 5*GetMoveDirection(*pData, 90).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
				pData->direction.horizontal += PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
			// }
			// else
			// {
			// 	pData->velocity.x += GetMoveDirection(*pData, 90).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			// 	pData->velocity.z += GetMoveDirection(*pData, 90).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			// }
		}

		else if (data.Right)
		{
			// if(strcmp(WiiAddress, "") != 0){
			// 	pData->velocity.x += 5*GetMoveDirection(*pData, 270).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			// 	pData->velocity.z += 5*GetMoveDirection(*pData, 270).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
				pData->direction.horizontal -= PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
			// }
			// else{
			// pData->velocity.x += GetMoveDirection(*pData, 270).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			// pData->velocity.z += GetMoveDirection(*pData, 270).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			// }
		}

		if (data.R){
			pData->velocity.y += 1;
		}
		if (data.L)
		{
			pData->direction.horizontal += PLAYER_ROTATE_SPEED * timer->GetDeltaTime();
		}

		///////////////////////////////// デバッグ用 後ろに下がる
		if (data.D)
		{
			pData->velocity.x += GetMoveDirection(*pData, 180).x * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
			pData->velocity.z += GetMoveDirection(*pData, 180).z * PLAYER_MOVE_SPEED * timer->GetDeltaTime();
		}
		/////////////////////////////////

		if (TERMINAL_SPEED < pData->velocity.x)
		{
			pData->velocity.x = TERMINAL_SPEED;
		}
		if (TERMINAL_SPEED < pData->velocity.y)
		{
			pData->velocity.y = TERMINAL_SPEED;
		}
		if (TERMINAL_SPEED < pData->velocity.z)
		{
			pData->velocity.z = TERMINAL_SPEED;
		}
	}
	return true;
}
