/*
 *  ファイル名	: client_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */

#include "client_common.h"

static int PrintError(const char *str);

// client用のmain関数
int main(int argc, char *argv[]) {
    /**********************/
    /**SDL2関連 BEGIN******/
    /**********************/
    SDL_Init(SDL_INIT_EVERYTHING);
    InitWindowSys();
    /**********************/
    /**SDL2関連 END********/
    /**********************/

    /**********************/
    /**入力関連 BEGIN******/
    /**********************/
    KeybordInput input;
    InputType data;
    /**********************/
    /**入力関連 END********/
    /**********************/

    /**********************/
    /**サーバー関連 BEGIN**/
    /**********************/
    //参加したいサーバーのポート番号
    u_short port = DEFAULT_PORT;
    //参加したいサーバーの名前
    char server_name[MAX_LEN_NAME];

    /*初期設定*/
    sprintf(server_name, "localhost");

    //参加するサーバーの名前とポート番号の設定
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
        //引数の数が足りない、もしくは多すぎるときメッセージを表示して終了
        fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
        return 1;
    }
    /*クライアントの作成*/
    //指定されたサーバー名、ポート番号に参加するクライアントとして設定する。
    SetupClient(server_name, port);
    /********************/
    /**サーバー関連 END**/
    /********************/

    SDL_Event event;
    //ループするかを判定
    int cond = 1;
    while (cond && data.end != 1) {
        RendererWindow();
        //入力受け付け
        input.GetInput(event);
        data = input.GetInputType();
        SystemRun(data);
        /*サーバーにリクエストを送る*/
        cond = ControlRequests();
        SDL_Delay(10);
    }

    
    // ウィンドウシステムの終了
    TerminateWindowSys();
    
    SDL_Quit();
    //クライアントを終了する。
    TerminateClient();
    return 0;
}
