#include "client_common.h"
#include "graphic.h"
#include <string.h>

static int MyId; // クライアントのID
// プレイヤーのデータ
PlayerData* PData;

int Num_Clients; // クライアント人数
static char Name_Clients[MAX_NUMCLIENTS][MAX_LEN_NAME]; // クライアントの名前
static FloatCube Pos_Clients = { 20, 20, 20, 9, 9, 9 }; // クライアント情報

ClientMap Map; //マップ
InputModuleBase *Input; // Input Module

// int GrapicThread(void *data); // This Function isn't used now.

// ===== * ===== プロパティ ===== * ===== //
// クライアント配列の先頭ポインタを返す
const PlayerData* GetPlayerData(){ return PData; }

// このクライアントのIDを返す
int GetMyID(){ return MyId; }

/*クライアントのID取得
* 引数
*   id: クライアントのID
*/
void SetMyID(int id){ MyId = id; }
// ===== * ===== プロパティ ===== * ===== //

int InputThread(void *data);


bool InitSystem(InitData *data){
	// SDL_Thread *thread;

	InitGraphic(); // グラフィックの初期化
	/*
	// グラフィックのスレッド化
	thread = SDL_CreateThread(GrapicThread, "GrapicThread", NULL);
	if (thread == NULL)
	{
		fprintf(stderr, "Failed to create a graphics red.\n");
		return false;
	}
	SDL_DetachThread(thread);
	*/

	Input = new KeybordInput();
	data->input = Input;

	SDL_Thread *inputThread;
    SDL_mutex *input_mtx = SDL_CreateMutex(); // 相互排除
	inputThread = SDL_CreateThread(InputThread, "inputThread", input_mtx);
	if (inputThread == NULL)
	{
		fprintf(stderr, "Failed to create a input thread.\n");
		return false;
	}
}

void SetNumClients(int n) // クライアント人数セット
{
    Num_Clients = n;
}

// 名前のセット
// id: クライアントのID
// clientName:クライアントの名前
void SetClientName(int id, char* name)
{
	strcpy(Name_Clients[id], name);
}

void InitPlayerData()// プレイヤーデータ初期化処理
{
    PData = new PlayerData[Num_Clients];
    for (int i = 0; i < Num_Clients; ++i) {
		strcpy(PData[i].name, Name_Clients[i]);
        PData[i].pos = Pos_Clients;
        PData[i].pos.x = Pos_Clients.x + i * 20;
        Vector3 t_v = { 0, 0, 0 };
        PData[i].velocity = t_v;
        PData[i].direction = 0;
        PData[i].rank = 0;
        PData[i].goal = false;
    }
}

void EndSys()// システム終了処理
{
    delete[] PData;
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
		fprintf(stderr, "[%d] %10s　は %f %f %f にいます。\n", i, PData[i].name, PData[i].pos.x, PData[i].pos.y, PData[i].pos.z);
	}
}

/*現在の設置データを返す
*	返り値: MyIDのキャラの設置データ
*/
PlaceData GetPlaceData(){
	PlaceData data;
	data.object = NomalBlock;
	data.pos = { PData[GetMyID()].pos.x, PData[GetMyID()].pos.y, PData[GetMyID()].pos.z };
	return data;
}

/*移動処理とか設置処理
* 引数
*   data: 入力データ
*
*/
void SystemRun()
{
	InputType data = Input->GetInputType();

	PData[MyId].velocity.x = 0;

	PData[MyId].velocity.y = 0;

	PData[MyId].velocity.z = 0;
	// 移動処理
	if (data.Forward || data.Left || data.Right || data.Left || data.Jump)
	{
		// 前
		if (data.Forward)
		{
			data.Forward = false;
			PData[MyId].velocity.z += 1;
		}
		// 左右
		if (data.Left)
		{
			data.Left = false;
			PData[MyId].velocity.x += 1;
		}
		else if (data.Right)
		{
			data.Right = false;
			PData[MyId].velocity.x -= 1;
		}
		// ジャンプ
		if (data.Jump)
		{
			data.Jump = false;
			PData[MyId].velocity.y -= 5;
		}

		// 移動コマンド実行
		InCommand(MOVE_COMMAND);
	}

	// 設置処理
	if (data.Put)
	{
		data.Put = false;
		InCommand(PUT_COMMAND);
	}
}

/*各プレイヤーのvelocityを変更する
* 引数
*/
void UpdateFlag(VelocityFlag* flags, int numClients){
	for (int i = 0; i < numClients; i++)
	{
		if (flags[i].x == false)
			PData[i].velocity.x = 0;

		if (flags[i].y == false)
			PData[i].velocity.y = 0;

		if (flags[i].z == false)
			PData[i].velocity.z = 0;
	}
}

// Updated place data from server
void UpdatePlaceData(PlaceData data){

}

// ===== * ===== マルチスレッド ===== * ===== //

// グラフィック用の
/*int GrapicThread(void *data){
	Disp();
	return 0;
}*/

int InputThread(void *data){
    SDL_mutex *mtx = (SDL_mutex *)data;

    while (1)
    {
        SDL_LockMutex(mtx);
		// 入力受け付け
		Input->UpdateInput();
		SDL_UnlockMutex(mtx);
	}

	return 0;
}
