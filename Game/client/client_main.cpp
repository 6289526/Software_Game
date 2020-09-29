
/*
 *  ファイル名	: client_main.cpp
 *  機能	: クライアント用のメイン関数を記述
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../../header/client_common.h"

extern void SetupClient(char *, u_short);
extern int ControlRequests();
extern void terminate_client();


typedef struct UserData{
    u_short port;
    char serverName[MAX_LEN_NAME];
}

// client用のmain関数
int main(int argc, char *argv[]) {
    printf("execute client's main function.\n"); // main関数の実行宣言

    struct UserData userData={DEFAULT_PORT, ""};

    switch (argc)
    {
        case 1:
            break;
        case 2:
            sprintf(userData.serverName, "%s", argv[1]);
            break;
        case 3:
            sprintf(userData.serverName, "%s", argv[1]);
            userData.port = (u_short)atoi(argv[2]);
            break;
        default:
            break;
    }

    SetupClient(userData.serverName, userData.port)

    int cond = true;
    while (cond)
    {
        cond = ControlRequests();
    }
    
    terminate_client();
  
    return 0;
}
