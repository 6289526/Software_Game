#include "client_common.h"
#include "graphic.h"
#include <string.h>

#define PLAYER_MOVE_SPEED 15
#define PLAYER_ROTATE_SPEED 4
#define GRAVITY 9.8 * 0.5// * 3

static int MyId; // クライアントのID
// プレイヤーのデータ
PlayerData *PData;

int Num_Clients;										// クライアント人数
static char Name_Clients[MAX_NUMCLIENTS][MAX_LEN_NAME]; // クライアントの名前
static FloatCube Pos_Clients = { PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W, PLAYER_H, PLAYER_D }; // クライアント情報

ClientMap Map;			//マップ
InputModuleBase *Input; // Input Module
Timer *Time; // FrameTimer

// int GrapicThread(void *data); // This Function isn't used now.

// ===== * ===== プロパティ ===== * ===== //
// クライアント配列の先頭ポインタを返す
const PlayerData *GetPlayerData() { return PData; }

// このクライアントのIDを返す
int GetMyID() { return MyId; }

/*クライアントのID取得
* 引数
*   id: クライアントのID
*/
void SetMyID(int id){ MyId = id; }

bool IsPlayerOnGround();
// ===== * ===== プロパティ ===== * ===== //
int clamp(const int __val, const int __lo, const int __hi);
int InputThread(void *data);

bool InitSystem(InitData *data)
{
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

	Input = new WiiInput(WiiAddress);
	data->input = Input;

	SDL_Thread *inputThread;
	SDL_mutex *input_mtx = SDL_CreateMutex(); // 相互排除
	inputThread = SDL_CreateThread(InputThread, "inputThread", input_mtx);
	if (inputThread == NULL)
	{
		fprintf(stderr, "Failed to create a input thread.\n");
		return false;
	}

	Time = new Timer();
	data->timer = Time;
}

// システム終了処理
void ExitSystem(InitData *data){
	delete[] PData;
	delete data->input;
	delete data->timer;
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
		PData[i].pos.x = Pos_Clients.x + i * 20;
		Vector3 t_v = {0, 0, 0};
		PData[i].velocity = t_v;
		PData[i].direction = 0;
		PData[i].rank = 0;
		PData[i].goal = false;
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
		fprintf(stderr, "[%d] %10s　は %f %f %f にいます。\n", i, PData[i].name, PData[i].pos.x, PData[i].pos.y, PData[i].pos.z);
	}
}

/*現在の設置データを返す
*	返り値: MyIDのキャラの設置データ
*/
PlaceData GetPlaceData()
{
	PlaceData data;
	data.object = NomalBlock;
	data.pos = {PData[GetMyID()].pos.x, PData[GetMyID()].pos.y, PData[GetMyID()].pos.z};
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

	// PData[MyId].velocity.y = 0;

	PData[MyId].velocity.z = 0; 
	// 移動処理
	if (Input->IsMoveButtonDown() || !IsPlayerOnGround())
	{
		if (data.U)
		{
			data.U = false;
			PData[MyId].velocity.y -= 1;
		}
		// 前
		if (data.Forward)
		{
			data.Forward = false;
			PData[MyId].velocity.z += PLAYER_MOVE_SPEED * Time->GetDeltaTime();
		}
		// 左右
		if (data.Left)
		{
			data.Left = false;
			PData[MyId].velocity.x += PLAYER_MOVE_SPEED * Time->GetDeltaTime();
		}
		else if (data.Right)
		{
			data.Right = false;
			PData[MyId].velocity.x -= PLAYER_MOVE_SPEED * Time->GetDeltaTime();
		}
		// ジャンプ
		if (data.Jump && IsPlayerOnGround() == 1)
		{
			data.Jump = false;
			PData[MyId].velocity.y += 5;
		}
		else if(!IsPlayerOnGround()){
			PData[MyId].velocity.y -= GRAVITY * Time->GetDeltaTime();
		}

		if (data.R)
		{
			PData[MyId].direction -= PLAYER_ROTATE_SPEED * Time->GetDeltaTime();
		}
		if (data.L)
		{
			PData[MyId].direction += PLAYER_ROTATE_SPEED * Time->GetDeltaTime();
		}

		/////////////////////////////////

		if (data.D)
		{
			data.D = false;
			PData[MyId].velocity.z -= PLAYER_MOVE_SPEED * Time->GetDeltaTime();
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

	fprintf(stderr, "time: %lf[mms] | IsGround = %d \n", Time->GetDeltaTime(), IsPlayerOnGround());
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

		if (flags[i].y == false)
			PData[i].velocity.y = 0;

		if (flags[i].z == false)
			PData[i].velocity.z = 0;
	}
}

// Updated place data from server
void UpdatePlaceData(PlaceData data)
{
}

bool IsPlayerOnGround(){
	int id = GetMyID();
	int accuracy  = 2;
	int point_X[2], point_Z[2];
	int y = 0;

    const int wide = PData[id].pos.w / (accuracy - 1);  // 当たり判定を知らべる座標間距離 x座標
    const int depth = PData[id].pos.d / (accuracy - 1); // 当たり判定を知らべる座標間距離 z座標

    // 当たり判定を調べる座標をすべて格納
    for (int i = 0; i < accuracy; ++i)
    {
        point_X[i] = PData[id].pos.x + PData[id].velocity.x + wide * i;
        point_Z[i] = PData[id].pos.z + PData[id].velocity.z + depth * i;
    }

    // マップデータ入手
    const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

    // 返り値用変数を宣言，初期化
    // ゴールブロックが１個でも接触すればゴールブロックが返る
    // ノーマルブロックが１個でも接触すればノーマルブロックが返る
    BlockType result = NonBlock;

    // マップ配列の添え字用変数を宣言，初期化
    int Block_X = 0;
    int t_Block_Y = (PData[id].pos.y + PData[id].velocity.y + y);
    int Block_Y = (PData[id].pos.y + PData[id].velocity.y + y) / MAP_MAGNIFICATION;
    Block_Y = clamp(Block_Y, 0, MAP_SIZE_H - 1); 
	int Block_Z = 0;

    for (int i = 0; i < accuracy; ++i)
    {
        Block_X = point_X[i] / MAP_MAGNIFICATION;
		Block_X = clamp(Block_X, 0, MAP_SIZE_H - 1);

        for (int j = 0; j < accuracy; ++j)
        {
            Block_Z = point_Z[j] / MAP_MAGNIFICATION;
			Block_Z = clamp(Block_Z, 0, MAP_SIZE_D - 1);

            switch (terrainData[Block_X][Block_Y][Block_Z])
            {
            case GoalBlock:
                result = GoalBlock;
                break;
            case NomalBlock:
                if (result == NonBlock)
                {
                    result = NomalBlock;
                }
                break;
            case NonBlock:
                break;
            default:
                throw "マップデータ : エラー\n";
            }
        }
    }
	return result != NonBlock;
}

int clamp(const int __val, const int __lo, const int __hi)
{
  return (__val < __lo) ? __lo : (__hi < __val) ? __hi : __val;
}


// ===== * ===== マルチスレッド ===== * ===== //

// グラフィック用の
/*int GrapicThread(void *data){
	Disp();
	return 0;
}*/

int InputThread(void *data)
{
	SDL_mutex *mtx = (SDL_mutex *)data;

	while (1)
	{
		SDL_LockMutex(mtx);
		// 入力受け付け
		Input->UpdateInput();
		/*サーバーにリクエストを送る*/
		SystemRun();
		SDL_UnlockMutex(mtx);
	}

	return 0;
}
