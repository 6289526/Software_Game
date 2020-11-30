/*
 *  ファイル名	: client_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */

#include "client_common.h"
#include "graphic.h"


// ループするかを判定
int cond = 1;

static int PrintError(const char *str);
int Select(void *args);

char WiiAddress[18];
InputType _______Type;


// client用のmain関数
int main(int argc, char *argv[])
{
	/**SDL2関連 BEGIN******/
	SDL_Init(SDL_INIT_EVERYTHING);
	/**SDL2関連 END********/

	/**サーバー関連 BEGIN**/
	// 参加したいサーバーのポート番号
	u_short port = DEFAULT_PORT;
	
	char server_name[MAX_LEN_NAME];
	InitData initData;
	//multithread
	SDL_Thread *SelectThread;
	SDL_mutex *mtx1 = SDL_CreateMutex();
	SelectThread = SDL_CreateThread(Select, "getCommand", mtx1);

	/*初期設定*/
	sprintf(server_name, "localhost");

	// 参加するサーバーの名前とポート番号の設定
	switch (argc)
	{
	case 1:
		break;
	case 2:
		sprintf(server_name, "%s", argv[1]);
		break;
	case 3:
		sprintf(server_name, "%s", argv[1]);
		port = (u_short)atoi(argv[2]);
		break;
	// case 4://Wiiリモコン使うとき
	// 	sprintf(server_name, "%s", argv[1]);
	// 	port = (u_short)atoi(argv[2]);
	// 	sprintf(WiiAddress, "%s", argv[3]);
	// 	break;
	default:
		// 引数の数が足りない、もしくは多すぎるときメッセージを表示して終了
		fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
		return 1;
	}
	/*クライアントの作成*/
	InitControl(&initData);
	// 指定されたサーバー名、ポート番号に参加するクライアントとして設定する。
	GetInitData(initData);
	SetupClient(server_name, port);
	InitPlayerData(); // プレイヤーデータ初期化処理
	/**サーバー関連 END**/
	InitSystem(&initData);

	while (cond && !initData.input->GetInputType().End)
	{
		SystemRun();

		Disp();
		SDL_Delay(10);
		initData.timer->UpdateFrame(); // Update the game frame.
	}

	// ウィンドウシステムの終了
	// TerminateWindowSys();

	// クライアントを終了する。
	TerminateClient();
	SDL_Quit();
	ExitSystem(&initData);
	return 0;
}

int Select(void *args)
{
	SDL_mutex *mtx = (SDL_mutex *)args;

	while (1)
	{
		SDL_LockMutex(mtx);

		cond = ControlRequests();

		SDL_UnlockMutex(mtx);
		SDL_Delay(10);
	}

	return 0;
}

// /*
// wiiリモコンのMACアドレスの取得
// */
// void GetWiiAddress(){

// 	FILE *fp;
// 	char command[MAX_STRING];
// 	char output[MAX_STRING];
// 	sprintf(command, "hcitool scan");

// 	if ((fp = popen(command, "r")) == NULL) {
// 	/*Failure*/
// 	}
// 	int index = 0;
// 	char trash[50];
// 	while (fgets(output, MAX_STRING, fp) != NULL) {
// 		if(index == 1){
// 			sscanf(output, "%s %s", WiiAddress, trash);
// 			fprintf(stderr, "%s\n", WiiAddress);
// 		}
// 		index++;
// 	}
// 	if (pclose(fp) == -1) {
// 	/*Failure*/
// 	}
// }
