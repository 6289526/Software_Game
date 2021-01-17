/*
 *  ファイル名  : client_input.h
 *  機能    : ClientSystemにある変数と関数の定義
 */

#pragma once
#include <map>
#include <tuple>
#include "../header/constants.h"
#include "client_map.hpp"
#include "client_define.h"
#include "client_time.hpp"
#include "client_input.h"
#include "client_gamestate.hpp"
#include "client_music.hpp"

namespace Mover
{
    class MoveCalculator;
}

namespace System
{
    typedef struct InitData
    {
        InputModuleBase *input;
        Timer *timer;
        GameState::GameStateController *stateController;

        InitData() : input(NULL),
                     timer(NULL),
                     stateController(NULL){};
    } InitData;

    class ClientSystem
    {
    private:
        int MyId;          // クライアントのID
        PlayerData *PData; // プレイヤーのデータ

        FloatCube Pos_Clients = {PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W, PLAYER_H, PLAYER_D}; // クライアント情報

        ClientMap Map;                                   // マップ
        InputModuleBase *Input;                          // Input Module
        Timer Time;                                      // FrameTimer
        GameState::GameStateController _StateController; // GameStateController
        GameState::GameStateOutputer _StateOutputer;     // _StateOutputer
        Sound::SoundController _SoundController;         // Sound Controller
        Mover::MoveCalculator *_MoveCalculator;          // MoveCalculator

        SDL_Thread *InputThreadVar;
        bool isJumped = false;

        InitData _InitData;

    public:
        ClientSystem();
        ~ClientSystem();

        int Num_Clients;                                 // クライアント人数
        char Name_Clients[MAX_NUMCLIENTS][MAX_LEN_NAME]; // クライアントの名前

        inline const PlayerData *GetPlayerData() { return PData; }
        inline const int GetMyID() { return MyId; }
        inline void SetMyID(int id) { MyId = id; }
        inline ClientMap &GetClientMap() { return Map; }
        inline InputModuleBase *GetInput() { return Input; }
        inline InitData *GetInitData() { return &_InitData; }
        inline Timer &GetTimer() { return Time; }
        GameState::GameStateController &GetGameStateController(){ return _StateController; }
        Sound::SoundController &GetSoundController(){ return _SoundController; }

        bool InitSystem(InitData *data);
        void ExitSystem(InitData *data);
        void InitControl(InitData *data);
        void SetNumClients(int n); // クライアント人数セット
        // 名前のセット
        // id: クライアントのID
        // clientName:クライアントの名前
        void SetClientName(int id, char *name);
        void InitPlayerData();
        void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);
        PlaceData GetPlaceData();
        void SystemRun();
        void UpdateVelocityFlag(VelocityFlag *flags, int numClients);
        void UpdatePlaceData(PlaceData data);
        void SetRank(int id, int rank);
        void SetDirection(float direction, int id);
        template <class T>
        T Abs(T value) { return value < 0 ? -value : value; }
        // int GraphicThread(void *data); // This Function isn't used now
    };
} // namespace System
