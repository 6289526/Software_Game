#include "client_common.h"
#include "graphic.h"
#include "client_move.h"
#include <string.h>
#include "go.h"

static int MyId;   // クライアントのID
PlayerData *PData; // プレイヤーのデータ

int Num_Clients;																			 // クライアント人数
static char Name_Clients[MAX_NUMCLIENTS][MAX_LEN_NAME];										 // クライアントの名前
static FloatCube Pos_Clients = {PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W, PLAYER_H, PLAYER_D}; // クライアント情報

ClientMap Map;						  	// マップ
InputModuleBase *Input;				  	// Input Module
Timer *Time;						  	// FrameTimer
GameStateController *StateController; 	// GameStateController
GameStateOutputer StateOutputer;	  	// StateOutputer

SDL_Thread *InputThreadVar;
static bool isJumped = false;

// ===== * ===== プロトタイプ宣言 ===== * ===== //
const PlayerData *GetPlayerData();
extern int GetMyID();
extern void SetMyID(int id);
extern bool InitSystem(InitData *data);
extern void ExitSystem(InitData *data);
void SetNumClients(int n);
void SetClientName(int id, char *name);
void InitPlayerData();
extern void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients);
extern PlaceData GetPlaceData();
extern void SystemRun();
extern void UpdateFlag(VelocityFlag *flags, int numClients);
extern void UpdatePlaceData(PlaceData data);
extern GameStateController GetGameStateController();
<<<<<<< HEAD
extern void SetRemoveClient(int id);
int clamp(const int value, const int low, const int hight);
=======
>>>>>>> 6df52397033cad521b5bdd2c77ee44e9f884afd8
template <class T>
T Abs(T value){ return value  < 0 ? -value : value; }
// int GraphicThread(void *data); // This Function isn't used now.
int InputThread(void *data);

// ===== * ===== プロパティ ===== * ===== //
// クライアント配列の先頭ポインタを返す
const PlayerData *GetPlayerData() { return PData; }

// このクライアントのIDを返す
int GetMyID() { return MyId; }

/*クライアントのID取得
* 引数
*   id: クライアントのID
*/
void SetMyID(int id) { MyId = id; }

// ----- * ----- //
void InitControl(InitData *data)
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

bool InitSystem(InitData *data)
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
	InputThreadVar = SDL_CreateThread(InputThread, "InputThread", input_mtx);
	if (InputThreadVar == NULL)
	{
		fprintf(stderr, "Failed to create a input thread.\n");
		return false;
	}

	Time = new Timer();
	data->timer = Time;

	StateController = new GameStateController();
	data->stateController = StateController;
	StateController->Subscribe(&StateOutputer);
	return true;
}

// システム終了処理
void ExitSystem(InitData *data)
{
	delete[] PData;
	delete data->input;
	delete data->timer;
	delete data->stateController;
}

void SetNumClients(int n) // クライアント人数セット
{
	Num_Clients = n;
}

// 名前のセット
// id: クライアントのID
// clientName:クライアントの名前
void SetClientName(int id, char *name)
{
	strcpy(Name_Clients[id], name);
}

void InitPlayerData() // プレイヤーデータ初期化処理
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
void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients)
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

extern void SetRank(int id, int rank) {
	PData[id].rank = rank;
	fprintf(stderr, "Client [%d] is rank is %d\n", id, PData[id].rank);
}

/*現在の設置データを返す
*	返り値: MyIDのキャラの設置データ
*/
PlaceData GetPlaceData()
{

	PlaceData data = BuildPlaceData(PData[GetMyID()], PLAYER_HAND_LENGTH);
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
void SystemRun()
{
	pair<bool, bool> t = SetPlayerVelocity(Input, &PData[MyId], Time);
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
void UpdateFlag(VelocityFlag *flags, int numClients)
{
	for (int i = 0; i < numClients; i++)
	{
		if (flags[i].x == false)
			PData[i].velocity.x = 0;

		if (flags[i].y == false && !isJumped){
			PData[i].velocity.y = 0;}

		if (flags[i].z == false)
			PData[i].velocity.z = 0;
	}
}

// Updated place data from server
void UpdatePlaceData(PlaceData data)
{
	Map.SetObjectData(&data);
}

// 方向の取得
void SetDirection(float direction, int id)
{
	if (id != MyId)
	{
		PData[id].direction = direction;
	}
}

<<<<<<< HEAD
void SetRemoveClient(int id){
	PData[id].onGame = false;
	fprintf(stderr, "%sはゲームから退出しました。\n", PData[id].name);
}

int GetDistanceFromGround(){
	const int accuracy = 3;
	float pointX[accuracy], pointZ[accuracy];
	float pointY = PData[MyId].pos.y + PData[MyId].velocity.y;
	const int width = PData[MyId].pos.w / (accuracy - 1); // X座標
	const int depth = PData[MyId].pos.d / (accuracy - 1);	// Z座標
	const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();
	const int blockX = pointX[accuracy - 1] / MAP_MAGNIFICATION;
	const int blockY = pointY / MAP_MAGNIFICATION;
	const int blockZ = pointZ[accuracy - 1] / MAP_MAGNIFICATION;

	int result = 0;

	for (int i = 0; i < accuracy; i++)
	{
		pointX[i] = PData[MyId].pos.x + PData[MyId].velocity.x + width * i;
		pointZ[i] = PData[MyId].pos.z + PData[MyId].velocity.z + depth * i;
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
		int blockPosX = pointX[i] / MAP_MAGNIFICATION;
		for (int j = 0; j < accuracy; j++)
		{
			int blockPosZ = pointZ[j] / MAP_MAGNIFICATION;
			int blockPosY = (PData[MyId].pos.y + PData[MyId].velocity.y) / MAP_MAGNIFICATION;
=======
>>>>>>> 6df52397033cad521b5bdd2c77ee44e9f884afd8

GameStateController GetGameStateController() { return *StateController; }

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
		if (Input == NULL)
		{
			throw "system.cppに宣言されている Input がNULL";
			return -1;
		}

		// 入力受け付け
		Input->UpdateInput();
		/*サーバーにリクエストを送る*/
		SDL_UnlockMutex(mtx);
	}

	return 0;
}
