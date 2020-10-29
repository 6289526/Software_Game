/*
 *  ファイル名	: client_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */

#include "client_common.h"

static int PrintError(const char *str);

// client用のmain関数
int main(int argc, char *argv[]) {
	/**SDL2関連 BEGIN******/
	SDL_Init(SDL_INIT_EVERYTHING);
	/**SDL2関連 END********/

	InputModuleBase *input;
	InitData initData = {&input};

	/**サーバー関連 BEGIN**/
	// 参加したいサーバーのポート番号
	u_short port = DEFAULT_PORT;
	// 参加したいサーバーの名前
	char server_name[MAX_LEN_NAME];

	/*初期設定*/
	sprintf(server_name, "localhost");

	// 参加するサーバーの名前とポート番号の設定
	switch (argc) {
		case 1:
			break;
		case 2:
			sprintf(server_name, "%s", argv[1]);
			break;
		case 3:
			sprintf(server_name, "%s", argv[1]);
			port = (u_short)atoi(argv[2]);
			break;
		default:
			// 引数の数が足りない、もしくは多すぎるときメッセージを表示して終了
			fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
			return 1;
	}
	/*クライアントの作成*/
	// 指定されたサーバー名、ポート番号に参加するクライアントとして設定する。
	SetupClient(server_name, port);
	/**サーバー関連 END**/

	InitSystem(&initData);

	// ループするかを判定
	int cond = 1;
	while (cond && !input->GetInputType().End) {
		// 入力受け付け
		input->UpdateInput(NULL);
		SystemRun(input->GetInputType());
		/*サーバーにリクエストを送る*/
		cond = ControlRequests();
		SDL_Delay(10);
	}

	// ウィンドウシステムの終了
	//TerminateWindowSys();

	// クライアントを終了する。
	TerminateClient();
	SDL_Quit();
	delete input;
	return 0;
}
