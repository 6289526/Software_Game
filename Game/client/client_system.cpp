#include "client_common.h"
#include "graphic.h"
#include "client_move.h"
#include <string.h>

#define PLAYER_MOVE_SPEED 40	// 移動速度
#define PLAYER_ROTATE_SPEED 4	// 回転速度
#define PLAYER_JUMP_POWER 2		// ジャンプ力
#define PLAYER_HAND_LENGTH 40.0f	// 手の長さ(ブロックの設置先までの距離)

#define GRAVITY 9.8 * 0.5		// 重力
#define TERMINAL_SPEED PLAYER_Y // 終端速度

static int MyId;   // クライアントのID
PlayerData *PData; // プレイヤーのデータ

int Num_Clients;																			 // クライアント人数
static char Name_Clients[MAX_NUMCLIENTS][MAX_LEN_NAME];										 // クライアントの名前
static FloatCube Pos_Clients = {PLAYER_X, PLAYER_Y, PLAYER_Z, PLAYER_W, PLAYER_H, PLAYER_D}; // クライアント情報

ClientMap Map;			//マップ
InputModuleBase *Input; // Input Module
Timer *Time;			// FrameTimer
GameStateController *StateController;	// GameStateController

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
bool IsPlayerOnGround();
int clamp(const int __val, const int __lo, const int __hi);
int BuryCheck_Under(const int id, const int y, const int accuracy,
					int block_X, int block_Y, int block_Z,
					const float *point_X, const float *point_Z);
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
void InitControl(InitData *data){
	ControlSetUp();
	if (strcmp(WiiAddress, "") == 0)
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


	InitGraphic(); // グラフィックの初期化



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

	StateController = new GameStateController();
	data->stateController = StateController;
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
		PData[i].pos.x = Pos_Clients.x + i * 20;
		PData[i].velocity = {0, 0, 0};
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
		//fprintf(stderr, "[%d] %10s　は %f %f %f にいます。\n", i, PData[i].name, PData[i].pos.x, PData[i].pos.y, PData[i].pos.z);
	}
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
	InputType data = Input->SystemGetInputType();
	bool isOnGround;
	try
	{
		isOnGround = IsPlayerOnGround();
	}
	catch(char const* e) // エラー処理
	{
		fprintf(stderr,"%s", e);
	}
	PData[MyId].velocity.x = 0;

	if (isOnGround)
		PData[MyId].velocity.y = 0;

	PData[MyId].velocity.z = 0;
	// 移動処理
	if (Input->IsMoveButtonDown() || !isOnGround)
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
			if(strcmp(WiiAddress, "") != 0){
				PData[MyId].velocity.x += 5*GetMoveDirection(PData[MyId], 0).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].velocity.z += 5*GetMoveDirection(PData[MyId], 0).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
			}
			else{
				PData[MyId].velocity.x += GetMoveDirection(PData[MyId], 0).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].velocity.z += GetMoveDirection(PData[MyId], 0).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
			}
		}
		// 左右
		if (data.Left)
		{
			data.Left = false;
			if(strcmp(WiiAddress, "") != 0){
				PData[MyId].velocity.x += 5*GetMoveDirection(PData[MyId], 90).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].velocity.z += 5*GetMoveDirection(PData[MyId], 90).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].direction += PLAYER_ROTATE_SPEED * Time->GetDeltaTime();
			}
			else
			{
				PData[MyId].velocity.x += GetMoveDirection(PData[MyId], 90).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].velocity.z += GetMoveDirection(PData[MyId], 90).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
			}
		}

		else if (data.Right)
		{
			data.Right = false;
			if(strcmp(WiiAddress, "") != 0){
				PData[MyId].velocity.x += 5*GetMoveDirection(PData[MyId], 270).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].velocity.z += 5*GetMoveDirection(PData[MyId], 270).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
				PData[MyId].direction -= PLAYER_ROTATE_SPEED * Time->GetDeltaTime();
			}
			else{
			PData[MyId].velocity.x += GetMoveDirection(PData[MyId], 270).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
			PData[MyId].velocity.z += GetMoveDirection(PData[MyId], 270).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
			}
		}
		// ジャンプ
		if (data.Jump && isOnGround == 1)
		{
			data.Jump = false;
			PData[MyId].velocity.y += PLAYER_JUMP_POWER;
		}
		else if (!isOnGround)
		{
			PData[MyId].velocity.y -= GRAVITY * Time->GetDeltaTime();
		}

		if (data.R)
		{
			PData[MyId].direction -= PLAYER_ROTATE_SPEED * Time->GetDeltaTime();
			data.R = false;
		}
		if (data.L)
		{
			PData[MyId].direction += PLAYER_ROTATE_SPEED * Time->GetDeltaTime();
			data.L = false;
		}

		///////////////////////////////// デバッグ用 後ろに下がる
		if (data.D)
		{
			data.D = false;
			PData[MyId].velocity.x += GetMoveDirection(PData[MyId], 180).x * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
			PData[MyId].velocity.z += GetMoveDirection(PData[MyId], 180).z * PLAYER_MOVE_SPEED * Time->GetDeltaTime();
		}
		/////////////////////////////////

		if (TERMINAL_SPEED < PData[MyId].velocity.x)
		{
			PData[MyId].velocity.x = TERMINAL_SPEED;
		}
		if (TERMINAL_SPEED < PData[MyId].velocity.y)
		{
			PData[MyId].velocity.y = TERMINAL_SPEED;
		}
		if (TERMINAL_SPEED < PData[MyId].velocity.z)
		{
			PData[MyId].velocity.z = TERMINAL_SPEED;
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

		if (flags[i].y == false)
			PData[i].velocity.y = 0;

		if (flags[i].z == false)
			PData[i].velocity.z = 0;
	}
}

// Updated place data from server
void UpdatePlaceData(PlaceData data)
{
	Map.SetObjectData(&data);
}

GameStateController GetGameStateController(){ return *StateController; }

bool IsPlayerOnGround()
{
	int id = GetMyID();
	int accuracy = 3;
	float point_X[accuracy], point_Z[accuracy]; // 調べる座標
	int y = 0;

	// 当たり判定の精度が正しいかどうか
	if (accuracy < 3 || PLAYER_D < accuracy || PLAYER_W < accuracy)
	{
		throw "Collision_CB_Under : 引数　エラー\n";
	}

	const int wide = PData[id].pos.w /
					(accuracy - 1); // 当たり判定を知らべる座標間距離 x座標
	const int depth = PData[id].pos.d /
						(accuracy - 1); // 当たり判定を知らべる座標間距離 z座標

	// 当たり判定を調べる座標をすべて格納
	for (int i = 0; i < accuracy; ++i)
	{
		point_X[i] = PData[id].pos.x + PData[id].velocity.x + wide * i;
		point_Z[i] = PData[id].pos.z + PData[id].velocity.z + depth * i;
	}

	// マップデータ入手
	const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

	// マップ配列用添字　の宣言　と　範囲のエラー処理
	int Block_X = point_X[accuracy - 1] / MAP_MAGNIFICATION;

	if (point_X[0] < 0)
	{
		throw "マップ外 : x座標 :負\n";
	}
	else if (MAP_SIZE_W <= Block_X)
	{
		throw "マップ外 : x座標 : 正\n";
	}

	const float t_Block_Y =
		(PData[id].pos.y + PData[id].velocity.y + y);
	int Block_Y = t_Block_Y / MAP_MAGNIFICATION;

	if (t_Block_Y < 0)
	{
		throw "マップ外 : y座標 : 負\n";
	}
	else if (MAP_SIZE_H <= Block_Y)
	{
		throw "マップ外 : y座標 : 正\n";
	}

	int Block_Z = point_Z[accuracy - 1] / MAP_MAGNIFICATION;

	if (point_Z[0] < 0)
	{
		throw "マップ外 : z座標 :負\n";
	}
	else if (MAP_SIZE_W <= Block_Z)
	{
		throw "マップ外 : z座標 : 正\n";
	}

	int Count_Under =
		BuryCheck_Under(id, 0, accuracy, Block_X, Block_Y, Block_Z, point_X, point_Z);

	if (Count_Under == -1)
	{
		throw "Collision_CB_Under : ブロックに埋まってる\n";
		// fprintf(stderr, "Collision_CB_Under : ブロックに埋まってる\n");
	}

	if (0 < Count_Under)
	{
		return true; // やや埋まっている or ピッタリ
	}

	return false;
}

int clamp(const int __val, const int __lo, const int __hi)
{
	return (__val < __lo) ? __lo : (__hi < __val) ? __hi : __val;
}

// 埋まっているピクセルが返る
int BuryCheck_Under(const int id, const int y, const int accuracy,
					int block_X, int block_Y, int block_Z,
					const float *point_X, const float *point_Z)
{
	int chara_size;   // キャラの大きさ
	float base_point; // 計算に使う基準座標
	enum PN_sign
	{
		positive = 1,
		negative = -1
	} PN_flag; // 正負を扱う

	chara_size = PData[id].pos.h;
	// なぜかvelocityを足さないとグラフィクがぶれる
	// 偉い人計算してください
	base_point = PData[id].pos.y + PData[id].velocity.y + y;
	PN_flag = positive;

	// マップデータ入手
	const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();

	int Bury_Count = 0; // 返り値　埋まり具合
	int Error_Count = 0;

	for (int i = 1; i < (accuracy - 1); ++i)
	{
		block_X = point_X[i] / MAP_MAGNIFICATION;
		for (int j = 1; j < (accuracy - 1); ++j)
		{
			block_Z = point_Z[i] / MAP_MAGNIFICATION;
			if (terrainData[block_X][static_cast<int>(base_point / MAP_MAGNIFICATION)]
							[block_Z] == NomalBlock)
			{
				int t_Count = 0;
				// どこまで埋まっているか調べる
				for (int k = 0; k <= chara_size - y; ++k)
				{
					block_Y = (base_point + (k * PN_flag)) / MAP_MAGNIFICATION;

					if (terrainData[block_X][block_Y][block_Z] == NomalBlock)
					{
						// その点の埋まっている程度をカウント
						++t_Count;
						// 埋まっている(かもしれない)
						if (chara_size <= k)
						{
						++Error_Count;
						t_Count = 0;
						}
					}
					// 埋まっていなければ抜ける
					else
					{
						break;
					}
				}
				// 最も埋まっている部分の埋まっている程度にする
				if (std::max(t_Count, Bury_Count) == t_Count)
				{
					Bury_Count = t_Count;
				}
			}
		}
	}

	// 全体が埋まっていたら
	if (Error_Count == ((accuracy - 2) * (accuracy - 2)))
	{
		return -1;
	}

	return Bury_Count; // 埋まっているピクセルが返る
}

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
		// 入力受け付け
		Input->UpdateInput();
		/*サーバーにリクエストを送る*/
		SDL_UnlockMutex(mtx);
	}

	return 0;
}
