
/*
 *  ファイル名	: server_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */

#include "server_common.h"
#include <iostream>
//
// ./server_main PlayerNum PortNum

int PlayerNum;
int PortNum;

int result = 1;

int GetCommand(void *args){
    SDL_mutex *mtx = (SDL_mutex *)args;
     // コマンド入力処理
    char str[25];
    char com, buf[24];

    while (1)
    {
        SDL_LockMutex(mtx);

        /**コマンド入力**/
        if(fgets(str, 25, stdin)){
            sscanf(str, "%c %s", &com, buf);
            fprintf(stderr, "%c\n", com);
            if(com == TERMINATE_COMMAND){
                RunCommand(BROADCAST, com);
                break;
            }
        }
        SDL_UnlockMutex(mtx);
    }

    return 0;
}

int SendPosFunc(void *args)
{
    SDL_mutex *mtx = (SDL_mutex *)args;

    while (1)
    {
        SDL_LockMutex(mtx);

        SendAllPos(PlayerNum); // クライアントの座標を全員に送る(別スレッドにする予定)

        SDL_UnlockMutex(mtx);
        SDL_Delay(10);
    }

    return 0;
}

int Select(void *args){
    SDL_mutex *mtx = (SDL_mutex *)args;

    while (1)
    {
        SDL_LockMutex(mtx);
        
        result = ControlRequests(); // クライアントからのリクエストに対応

        SDL_UnlockMutex(mtx);
        SDL_Delay(10);
    }

    return 0;

}

// server用のmain関数
int main(int argc, char *argv[])
{

    if (3 < argc)
    {
        printf("コマンドライン引数が多いんじゃああああ\n");
        return 0;
    }

    PlayerNum = atoi(argv[1]);
    PortNum = atoi(argv[2]);
    //チャットの参加人数の表示
    fprintf(stderr, "Number of clients = %d\n", PlayerNum);
    //サーバーのポート番号を表示
    fprintf(stderr, "Port number = %d\n", PortNum);
    SetupServer(PlayerNum, PortNum); // サーバー初期化

    /**SDL BEGIN**/
    //初期化
    SDL_Init(SDL_INIT_EVERYTHING);
    // 座標を送るスレッド
    SDL_Thread *sendPosThread;
    SDL_Thread *getCommand;
    SDL_Thread *SelectThread;
    // 相互排除
    SDL_mutex *mtx1 = SDL_CreateMutex();
    SDL_mutex *mtx2 = SDL_CreateMutex();
    SDL_mutex *mtx3 = SDL_CreateMutex();
    // スレッド作成
    sendPosThread = SDL_CreateThread(SendPosFunc, "sendPosThread", mtx1);
    getCommand = SDL_CreateThread(GetCommand, "getCommand", mtx2);
    SelectThread = SDL_CreateThread(Select, "getCommand", mtx3);
    /**SDL END**/

    int end = 0;

    while (!end && result)
    {
        for (int i = 0; i < PlayerNum; ++i) {
            try {
                MovePosition(i);
            }
            catch (const char* const e) {
                fprintf(stderr, e);
            }
        }

        // 全員ゴール
        if (AllGoal())
        {
            fprintf(stderr, "全員ゴール\n");
            end = 1;
        }
    }

    TerminateServer(); // サーバー終了処理
    SDL_DestroyMutex(mtx1);
    SDL_DestroyMutex(mtx2);
    SDL_Quit();
    return 0;
}
