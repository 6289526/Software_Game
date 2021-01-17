/**
 * @file client_move.h
 * @brief 
 * ClientのSystem内における移動や設置の関数を定義
 */

#pragma once
#include <cmath>
#include "client_system.hpp"

#define USE_GRAVITY true
#define PI 3.14159265358979323846

System::ClientSystem& GetSystem();

namespace Mover{
    class MoveCalculator {
    private:
        System::ClientSystem *_System;

        bool isOnGround = true;
        bool isJumped = false;
        bool isPreGround = false;

        inline float DegreeToRadian(float degree){ return degree * PI / 180.0; }
        inline float RadianToDegree(float radian){ return radian * 180.0 / PI; }
        bool DisUseGravity(InputModuleBase *inputModule, PlayerData *pData, Timer *timer);

    public:
        MoveCalculator(System::ClientSystem *system);

        PlaceData BuildPlaceData(PlayerData player, float handLength);
        Vector3 GetMoveDirection(PlayerData player, float angle);

        int GetPutableBlockHeightIndexFromMap(Vector2Int pos);
        Vector3Int GetTopOfHeightBlockIndex(Vector3 position);
        bool IsPlayerOnGroundSimple();

        std::pair<bool, bool> SetPlayerVelocity(InputModuleBase *inputModule, PlayerData *pData, Timer *timer);
    };
}
