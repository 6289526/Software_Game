#include "client_common.h"
#include "client_system.hpp"
#include "graphic.h"
#include "client_move.h"
#include "go.h"

using namespace System;

int InputThread(void *data);

ClientSystem::ClientSystem(){ 
	_MoveCalculator = new Mover::MoveCalculator(this);
}
ClientSystem::~ClientSystem(){ 
	delete _MoveCalculator;
}

// ----- * ----- //
void ClientSystem::InitControl(InitData *data)
{
	ControlSetUp();
	if(GoSInput.J){
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

bool ClientSystem::InitSystem(InitData *data)
{
	// SDL_Thread *thread;
	/*
	// グラフィックのスレッド化
	thread = SDL_CreateThread(GraphicThread, "GraphicThread", NULL);
	if (thread == NULL)
	{
		fprintf(stderr, "Failed to create a graphics red.\n");
		return false;
	}
	SDL_DetachThread(thread);
	*/
	/*入力方式の選択またwiiリモコンのアドレスを取得*/

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

	Time = new Timer();
	data->timer = Time;

	StateController = new GameState::GameStateController();
	data->stateController = StateController;
	StateController->Subscribe(&_StateOutputer);
	StateController->Subscribe(&_BgmController);
	
	StateController->OnNest(GameState::Init);
	return true;
}

// システム終了処理
void ClientSystem::ExitSystem(InitData *data)
{
	delete[] PData;
	delete data->input;
	delete data->timer;
	delete data->stateController;
}

void ClientSystem::SetNumClients(int n) // クライアント人数セット
{
	Num_Clients = n;
}

// 名前のセット
// id: クライアントのID
// clientName:クライアントの名前
void ClientSystem::SetClientName(int id, char *name)
{
	strcpy(Name_Clients[id], name);
}

void ClientSystem::InitPlayerData() // プレイヤーデータ初期化処理
{
	PData = new PlayerData[Num_Clients];
	for (int i = 0; i < Num_Clients; ++i)
	{
		strcpy(PData[i].name, Name_Clients[i]);
		PData[i].pos = Pos_Clients;
		PData[i].pos.x = Pos_Clients.x + i * PLAYER_W;
		PData[i].velocity = {0, 0, 0};
		PData[i].direction = 0;
		PData[i].rank = 0;
		PData[i].goal = false;
		PData[i].onGame = true;
	}
}

/*クライアントの位置の取得
* 引数
*   moveData[MAX_NUMCLIENTS]: 移動位置
*   numCLients : 接続しているクライアントの数
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

void ClientSystem::SetRank(int id, int rank) {
	PData[id].rank = rank;
	fprintf(stderr, "Client [%d] is rank is %d\n", id, PData[id].rank);
}

/*現在の設置データを返す
*	返り値: MyIDのキャラの設置データ
*/
PlaceData ClientSystem::GetPlaceData()
{

	PlaceData data = _MoveCalculator->BuildPlaceData(PData[GetMyID()], PLAYER_HAND_LENGTH);
	// data.object = NomalBlock;
	// data.pos = {(int)PData[GetMyID()].pos.x, (int)PData[GetMyID()].pos.y, (int)PData[GetMyID()].pos.z};

	fprintf(stderr, "(%d, %d, %d)の位置にブロック設置を要求します\n", data.pos.x, data.pos.y, data.pos.z);

	return data;
}

/*移動処理とか設置処理
* 引数
*   data: 入力データ
*
*/
void ClientSystem::SystemRun()
{
	pair<bool, bool> t = _MoveCalculator->SetPlayerVelocity(Input, &PData[MyId], Time);
	isJumped = t.second;
	// 移動処理
	if (t.first){
		// 移動コマンド実行
		InCommand(MOVE_COMMAND);
	}

	// 設置処理
	if (Input->GetInputType().Put)
	{
		InCommand(PUT_COMMAND);
	}

	// fprintf(stderr, "time: %lf[mms] | IsGround = %d \n", Time->GetDeltaTime(), IsPlayerOnGround());
}

/*各プレイヤーのvelocityを変更する
* 引数
*/
void ClientSystem::UpdateFlag(VelocityFlag *flags, int numClients)
{
	for (int i = 0; i < numClients; i++)
	{
		if (flags[i].x == false)
			PData[i].velocity.x = 0;
#if USE_GRAVITY
		if (flags[i].y == false && !isJumped){
			PData[i].velocity.y = 0;}
#else
		/*if (flags[i].y == false){
			PData[i].velocity.y = 0;
		}*/
#endif

		if (flags[i].z == false)
			PData[i].velocity.z = 0;
	}
}

// Updated place data from server
void ClientSystem::UpdatePlaceData(PlaceData data)
{
	Map.SetObjectData(&data);
	
}

// 方向の取得
void ClientSystem::SetDirection(float direction, int id)
{
	if (id != MyId)
	{
		PData[id].direction = direction;
	}
}

GameState::GameStateController System::ClientSystem::GetGameStateController() { return *StateController; }

// ===== * ===== マルチスレッド ===== * ===== //

// グラフィック用の
/*int GraphicThread(void *data){
	Disp();
	return 0;
}*/

int InputThread(void *data)
{
	SDL_mutex *mtx = (SDL_mutex *)data;

	while (1)
	{
		SDL_LockMutex(mtx);
		auto input = GetSystem().GetInput();
		if (input == NULL)
		{
			throw "system.cppに宣言されている Input がNULL";
			return -1;
		}

		// 入力受け付け
		input->UpdateInput();
		/*サーバーにリクエストを送る*/
		SDL_UnlockMutex(mtx);
	}

	return 0;
}
