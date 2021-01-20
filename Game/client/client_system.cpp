/**
 * @file client_system.cpp
 * @brief 
 * クライアントシステムメインモジュールの実装
 */

#include "client_common.h"
#include "client_system.hpp"
#include "graphic.h"
#include "client_move.h"
#include "go.h"

using namespace System;

int InputThread(void *data);

ClientSystem::ClientSystem()
{
	_MoveCalculator = new Mover::MoveCalculator(this);
	_InitData.stateController = &_StateController;

}
ClientSystem::~ClientSystem()
{
	delete _MoveCalculator;
}

/**
 * @fn
 * @brief 入力端末を決定する
 * @param data 初期化用データ
 */
void ClientSystem::InitControl(InitData *data)
{
	ControlSetUp();
	if (GoSInput.J)
	{
		Input = new Smart();
	}
	else if (strcmp(WiiAddress, "") == 0)
	{
		Input = new KeybordInput();
	}
	else
	{
		Input = new WiiInput(WiiAddress);
	}

	data->input = Input;
}

/**
 * @fn
 * @brief システムを初期化する
 * @param data 初期化用データ
 */
bool ClientSystem::InitSystem(InitData *data)
{
	SDL_Thread *inputThread;

	SDL_mutex *input_mtx = SDL_CreateMutex(); // 相互排除
	SDL_ThreadFunction inputThreadFunction = (SDL_ThreadFunction)&InputThread;
	InputThreadVar = SDL_CreateThread(inputThreadFunction, "InputThread", input_mtx);
	// InputThreadVar = SDL_CreateThread(InputThread, "InputThread", input_mtx);
	if (InputThreadVar == NULL)
	{
		fprintf(stderr, "Failed to create a input thread.\n");
		return false;
	}

	data->timer = &Time;
	_SoundController.Initialize();

	_StateController.Subscribe(&_StateOutputer);
	_StateController.Subscribe(&_SoundController.GetBGMPlayer());
	_StateController.OnNest(GameState::Init);
	return true;
}

/**
 * @fn
 * @brief システム終了処理
 * @param data 初期化用データ
 */
void ClientSystem::ExitSystem(InitData *data)
{
	delete[] PData;
	delete data->input;
	_SoundController.Finalize();
}

/**
 * @fn
 * @brief クライアント人数セットする
 * @param n クライアント数
 */
void ClientSystem::SetNumClients(int n)
{
	Num_Clients = n;
}

/**
 * @fn
 * @brief 名前のセット
 * @param id クライアントのID
 * @param name クライアントの名前
 */
void ClientSystem::SetClientName(int id, char *name)
{
	strcpy(Name_Clients[id], name);
}

/**
 * @fn
 * @brief プレイヤーデータ初期化処理
 */
void ClientSystem::InitPlayerData()
{
	PData = new PlayerData[Num_Clients];
	for (int i = 0; i < Num_Clients; ++i)
	{
		strcpy(PData[i].name, Name_Clients[i]);
		PData[i].pos = Pos_Clients;
		PData[i].pos.x = Pos_Clients.x + i * PLAYER_W * 2;
		PData[i].velocity = {0, 0, 0};
		PData[i].direction.horizontal = 0;
		PData[i].rank = 0;
		PData[i].goal = false;
		PData[i].onGame = true;
	}
}

/**
 * @fn
 * @brief クライアントの位置の取得
 * @param moveData 移動位置
 * @param numClients 接続しているクライアントの数
 */
void ClientSystem::SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients)
{
	int count = 0;
	for (int i = 0; i < numClients; i++)
	{
		// 座標の代入
		PData[i].pos.x = moveData[i].x;
		PData[i].pos.y = moveData[i].y;
		PData[i].pos.z = moveData[i].z;
		//fprintf(stderr, "[%d] %10s　は %f %f %f にいます。\n", i, PData[i].name, PData[i].pos.x, PData[i].pos.y, PData[i].pos.z);
	}
}

/**
 * @fn
 * @brief ランクをセットする
 * @param id クライアントID
 * @param rank ランクング
 */
void ClientSystem::SetRank(int id, int rank)
{
	PData[id].rank = rank;
}

/**
 * @fn
 * @brief 現在の設置データを返す
 * @return MyIDのキャラの設置データ
 */
PlaceData ClientSystem::GetPlaceData()
{

	PlaceData data = _MoveCalculator->BuildPlaceData(PData[GetMyID()], PLAYER_HAND_LENGTH);
	// data.object = NomalBlock;
	// data.pos = {(int)PData[GetMyID()].pos.x, (int)PData[GetMyID()].pos.y, (int)PData[GetMyID()].pos.z};

	//fprintf(stderr, "(%d, %d, %d)の位置にブロック設置を要求します\n", data.pos.x, data.pos.y, data.pos.z);

	return data;
}

/**
 * @fn
 * @brief 移動処理や設置処理等のメイン処理を実行
 */
void ClientSystem::SystemRun()
{
	bool isMoved = _MoveCalculator->SetPlayerVelocity(Input, &PData[MyId], &Time);
	isJumped = _MoveCalculator->IsJumped();
	// 移動処理
	if (isMoved)
	{
		// 移動コマンド実行
		InCommand(MOVE_COMMAND);
	}

	// 設置処理
	if (Input->GetInputType().Put)
	{
		InCommand(PUT_COMMAND);
	}

	// fprintf(stderr, "time: %lf[mms] | IsGround = %d \n", Time->GetDeltaTime(), IsPlayerOnGround());

	_SoundController.Playing();
}

/**
 * @fn
 * @brief 各プレイヤーの速度フラグを変更する
 * @param flags 速度フラグ
 * @param numClients クライアント数
 */
void ClientSystem::UpdateVelocityFlag(VelocityFlag *flags, int numClients)
{
	for (int i = 0; i < numClients; i++)
	{
		if (flags[i].x == false)
			PData[i].velocity.x = 0;
#if USE_GRAVITY
		if (flags[i].y == false && !isJumped)
		{
			PData[i].velocity.y = 0;
		}
#else
			/*if (flags[i].y == false){
			PData[i].velocity.y = 0;
		}*/
#endif

		if (flags[i].z == false)
			PData[i].velocity.z = 0;
	}
}

/**
 * @fn
 * @brief マップに対し設置データを更新する
 * @param data 設置データ
 */
void ClientSystem::UpdatePlaceData(PlaceData data)
{
	Map.SetObjectData(&data);
}

/**
 * @fn
 * @brief クライアントの角度を設定する
 * @param direction 角度
 * @param id クライアントID
 */
void ClientSystem::SetDirection(float direction, int id)
{
	if (id != MyId)
	{
		PData[id].direction.horizontal = direction;
	}
}

// ===== * ===== マルチスレッド ===== * ===== //

/**
 * @fn
 * @brief 入力用マルチスレッド 
 * @param 
 */
int InputThread(void *data)
{
	SDL_mutex *mtx = (SDL_mutex *)data;

	while (cond)
	{
		SDL_LockMutex(mtx);
		auto input = GetSystem().GetInput();
		if (input == NULL)
		{
			throw "system.cppに宣言されている Input がNULL";
		}

		// 入力受け付け
		input->UpdateInput();
		/*サーバーにリクエストを送る*/
		SDL_UnlockMutex(mtx);
	}

	return 0;
}
