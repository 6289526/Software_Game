
/*
 *  ファイル名	: server_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */

#include "../header/server_common.h"

int PlayerNum = 3;
int PortNum = 0;

// server用のmain関数
int main(int argc, char *argv[]) {

    SetupServer(PlayerNum, PortNum); // サーバー初期化

    int end = 0;

    while (!end) {
        ControlRequests(); // クライアントからのリクエストに対応

        // 全員ゴール
        if (Goal()) {
            end = 1;
        }
    }

    TerminateServer(); // サーバー終了処理

    return 0;
}
