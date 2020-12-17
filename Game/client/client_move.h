/*
 *  ファイル名  : client_move.h
 *  機能    : ClientのSystem内における移動や設置の関数を定義
 */

#pragma once
#include <cmath>
#include "client_system.hpp"

#define USE_GRAVITY true
#define PI 3.14159265358979323846

namespace Mover{
    class MoveCalculator {
        private:
        System::ClientSystem *_System = NULL;
        bool isOnGround = true;
        bool isJumped = false;
        bool isPreGround;

        float DegreeToRadian(float degree){ return degree * PI / 180.0; }
        float RadianToDegree(float radian){ return radian * 180.0 / PI; }
        bool DisUseGravity(InputModuleBase *inputModule, PlayerData *pData, Timer *timer);

        public:
        MoveCalculator(const System::ClientSystem *system);

        PlaceData BuildPlaceData(PlayerData player, float handLength);
        Vector3 GetMoveDirection(PlayerData player, float angle);

        int GetPutableBlockHeightFromMap(Vector2Int pos);
        Vector3Int GetTopOfHeightBlockIndex(Vector3 position);
        bool IsPlayerOnGroundSimple();

        std::pair<bool, bool> SetPlayerVelocity(InputModuleBase *inputModule, PlayerData *pData, Timer *timer);
    };
}
