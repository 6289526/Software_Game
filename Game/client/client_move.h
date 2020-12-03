/*
 *  ファイル名  : client_move.h
 *  機能    : ClientのSystem内における移動や設置の関数を定義
 */

#pragma once
#include "client_common.h"

PlaceData BuildPlaceData(PlayerData player, float handLength);
Vector3 GetMoveDirection(PlayerData player, float angle);

int GetPutableBlockHeightFromMap(Vector2Int pos);
Vector3Int GetTopOfHeightBlockIndex(Vector3 position);
bool IsPlayerOnGroundSimple();
