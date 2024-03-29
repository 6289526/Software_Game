#include "client_common.h"

// ループするかを判定
int cond = 1;

static int PrintError(const char *str);
int Select(void *args);
int Go(void *args);

char WiiAddress[18];

System::ClientSystem _System;
// ClientSystemを返す
System::ClientSystem &GetSystem() { return _System; }

// client用のmain関数
int main(int argc, char *argv[])
{
	/**SDL2関連 BEGIN******/
	SDL_Init(SDL_INIT_EVERYTHING);
	/**SDL2関連 END********/

	// スマートフォン用のサーバーを作る
	SDL_Thread *goThread;
	SDL_mutex *gMtx = SDL_CreateMutex();
	goThread = SDL_CreateThread(Go, "Go!", gMtx);

	/**サーバー関連 BEGIN**/
	// 参加したいサーバーのポート番号
	u_short port = DEFAULT_PORT;

	char server_name[MAX_LEN_NAME];

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
	// グラフィックの初期化
	InitGraphic();
	/*クライアントの作成*/
	_System.InitControl(_System.GetInitData());
	// 指定されたサーバー名、ポート番号に参加するクライアントとして設定する。
	GetInitData(*_System.GetInitData());
	SetupClient(server_name, port);
	_System.InitPlayerData(); // プレイヤーデータ初期化処理
	/**サーバー関連 END**/
	_System.InitSystem(_System.GetInitData());
	Init2dGraphic();

	//multithread
	SDL_Thread *SelectThread;
	SDL_mutex *mtx1 = SDL_CreateMutex();
	SelectThread = SDL_CreateThread(Select, "getCommand", mtx1);

	// End入力が入っていればループから抜ける
	while (cond && !_System.GetInitData()->input->GetInputType().End)
	{
		Disp();					// 画面の表示
		_System.SystemRun();	// メインシステムの実行
		SDL_Delay(10);			// 遅延
		_System.GetInitData()->timer->UpdateFrame(); // Timerの更新
	}

	if (_System.GetInitData()->input->GetInputType().End)
	{
		cond = 0;
		InCommand(QUIT_COMMAND);
		fprintf(stderr, "Skip Result\n");
	}
	else
	{
		ShowResult();
		_System.GetSoundController().GetSoundEffectSubject().OnNest(Sound::Result);
		fprintf(stderr, "Show Result\n");
	}

	// ウィンドウシステムの終了
	// TerminateWindowSys();

	// クライアントを終了する。
	TerminateClient();
	KillGoServer();
	fprintf(stderr, "GoServer\n");
	_System.ExitSystem(_System.GetInitData());
	fprintf(stderr, "ExitSystem\n");

	TerminateGraphic();
	fprintf(stderr, "Terminate Graphic\n");
	TTF_Quit();
	SDL_Quit();
	fprintf(stderr, "SDL Quit\n");
	return 0;
}

int Select(void *args)
{
	SDL_mutex *mtx = (SDL_mutex *)args;

	while (cond)
	{
		if (GetSystem().GetGameStateController().GetState() == GameState::Init)
		{
			SDL_LockMutex(mtx);
			cond = ControlRequests();

			SDL_UnlockMutex(mtx);
			SDL_Delay(10);
		}
	}

	return 0;
}

// スマホからの入力を受け取る
int Go(void *args)
{
	SDL_mutex *mtx = (SDL_mutex *)args;
	SDL_LockMutex(mtx);
	Goroutine();
	SDL_UnlockMutex(mtx);

	return 0;
}
