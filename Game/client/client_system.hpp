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

namespace System{
    typedef struct InitData{
        InputModuleBase *input;
        Timer *timer;
        GameState::GameStateController *stateController;

        InitData() :
            input(NULL),
            timer(NULL),
            stateController(NULL)
            { };
    } InitData;

    class ClientSystem{
    private:
        int MyId;   // クライアントのID
        PlayerData *PData; // プレイヤーのデータ

        int Num_Clients;																			 // クライアント人数
        char Name_Clients[MAX_NUMCLIENTS][MAX_LEN_NAME];										 // クライアントの名前
        FloatCube Pos_Clients = {PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W, PLAYER_H, PLAYER_D}; // クライアント情報

        ClientMap Map;						  	// マップ
        InputModuleBase *Input;				  	// Input Module
        Timer *Time;						  	// FrameTimer
        GameState::GameStateController *StateController; 	// GameStateController
        GameState::GameStateOutputer StateOutputer;	  	// StateOutputer
        Sound::BGMController BgmController;			// BGM Controller

        SDL_Thread *InputThreadVar;
        bool isJumped = false;

        void Initialize();
        void Terminate();
        void SetNumClients(int n); // クライアント人数セット

        // 名前のセット
        // id: クライアントのID
        // clientName:クライアントの名前
        void SetClientName(int id, char* name);

        void InitPlayerData(); // プレイヤーデータ初期化処理

        void EndSys(); // システム終了処理
    public:
        const PlayerData *GetPlayerData() { return PData; }
        int GetMyID() { return MyId; }
        void SetMyID(int id) { MyId = id; }
        const MapData* GetClientMap() { return &Map; }

        bool InitSystem(InitData *data);
        void ExitSystem(InitData *data);
        void InitControl(InitData *data);
        void SetNumClients(int n);
        void SetClientName(int id, char *name);
        void InitPlayerData();
        void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);
        PlaceData GetPlaceData();
        void SystemRun();
        void UpdateFlag(VelocityFlag *flags, int numClients);
        void UpdatePlaceData(PlaceData data);
        GameState::GameStateController GetGameStateController();
        template <class T>
        T Abs(T value){ return value  < 0 ? -value : value; }
        // int GraphicThread(void *data); // This Function isn't used now.
        int InputThread(void *data);

        ClientSystem &operator =(const ClientSystem &p);
    };
}
