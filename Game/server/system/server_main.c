
/*
 *  ファイル名	: server_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */

#include "../header/server_common.h"

// ./server_main PlayerNum PortNum

int PlayerNum;
int PortNum;

int SendPosFunc(void* args){
    SDL_mutex* mtx = (SDL_mutex*)args;

    while(1){
        SDL_LockMutex(mtx);

        SendAllPos(PlayerNum); // クライアントの座標を全員に送る(別スレッドにする予定)

        SDL_UnlockMutex(mtx);
    }

    return 0;
}

// server用のmain関数
int main(int argc, char *argv[]) {

    if (3 < argc) {
        printf("コマンドライン引数が多いんじゃああああ\n");
        return 0;
    }
　　
    PlayerNum = atoi(argv[1]);
    PortNum = atoi(argv[2]);

    SetupServer(PlayerNum, PortNum); // サーバー初期化

    /**SDL BEGIN**/
    //初期化
    SDL_Init(SDL_INIT_EVERYTHING);
    // 座標を送るスレッド
    SDL_Thread* sendPosThread;
    // 相互排除
    SDL_mutex *mtx = SDL_CreateMutex();
    // スレッド作成
    sendPosThread = SDL_CreateThread(SendPosFunc, "sendPosThread", mtx);
    /**SDL END**/

    int end = 0;

    while (!end) {
        ControlRequests(); // クライアントからのリクエストに対応

        // 全員ゴール
        if (Goal()) {
            printf("全員ゴール!!!!!!!!!!!!\n");
            end = 1;
        }
    }

    TerminateServer(); // サーバー終了処理
    SDL_DestroyMutex(mtx);
    SDL_Quit();
    return 0;
}
