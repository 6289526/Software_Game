#include "client_common.h"
#include "graphic.h"

// クライアントのID
static int MyId;
// プレイヤーのデータ
PlayerData PData[PLAYER_NUM] = {
	{"a", {20, 20, 20, 7, 20, 7}, {0, 0, 0},0, 1, 0},
	{"a", {20, 20, 20, 10, 10, 10}, 1, 0}
};

//マップ
ClientMap Map;

// クライアント配列の先頭ポインタを返す
const PlayerData* GetPlayerData(){
	return PData;
}

int GetMyID(){
	return MyId;
}

int GrapicThread(void *data);

/*クライアントのID取得
* 引数
*   id: クライアントのID
*/
void SetMyID(int id)
{
	MyId = id;
}

/*クライアントの位置の取得
* 引数
*   moveData[MAX_NUMCLIENTS]: 移動位置
*   numCLients : 接続しているクライアントの数
*/
int count = 0;
void SetPlace(FloatPosition moveData[MAX_NUMCLIENTS], int numClients)
{
	for (int i = 0; i < numClients; i++)
	{
		// 座標の代入
		PData[i].pos.x = moveData[i].x;
		PData[i].pos.y = moveData[i].y;
		PData[i].pos.z = moveData[i].z;
		if(count % 10 == 0) fprintf(stderr, "%d client%d　は %f %f %f にいます。\n",count, i, PData[i].pos.x, PData[i].pos.y, PData[i].pos.z);
		count++;
	}
}
/*クライアントの速度フラグの取得
* 引数
*
*/
void SetFlag(VelocityFlag* flag, int numClients){

}


/*移動処理とか設置処理
* 引数
*   data: 入力データ
*
*/
void SystemRun(InputType data)
{
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
			//PData[MyId].pos.z += 10;
			PData[MyId].velocity.z = 1;
		}
		// 左右
		if (data.Left)
		{
			data.Left = false;
			//PData[MyId].pos.x -= 10;
			PData[MyId].velocity.x = -1;
		}
		else if (data.Right)
		{
			data.Right = false;
			//PData[MyId].pos.x += 10;
			PData[MyId].velocity.x = 1;
		}
		// ジャンプ
		if (data.Jump)
		{
			data.Jump = false;
			//PData[MyId].pos.y += 10;
			PData[MyId].velocity.y = 1;
		}

		// PData[MyId].posはFloatCubeなのでFloatPositionにする
		FloatPosition pos;
		pos.x = PData[MyId].pos.x;
		pos.y = PData[MyId].pos.y;
		pos.z = PData[MyId].pos.z;
		// 移動コマンド実行
		InCommand(MOVE_COMMAND);
	}
}

bool InitSystem(InitData *data){
	SDL_Thread *thread;

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

	*(data->input) = new KeybordInput();
}

// グラフィック用の
int GrapicThread(void *data){
	Disp();
	return 0;
}
