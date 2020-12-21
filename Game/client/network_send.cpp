/*
 *  ファイル名	：net.cpp
 *  機能	：ネットワークの処理
 *
 */
#include "client_common.h"

/*変数初期化*/
// 接続してくるクライアントの数
static int NumClients;
// 自身のID
static int MyId;
// ソケット
static int sock;
// ソケットの数
static int NumSock;
// ファイルディスクリプタ
static fd_set Mask;
// クライアントの情報
static NetworkData Clients[MAX_NUMCLIENTS];


/*関数*/
static int HandleError(char *);
static void SendData(void *data, int size);
static int ReceiveData(void *data, int size);
static int ExeCommand(void);


/*クライアントの初期設定
* 引数
*   *server_name: 接続するサーバーの名前
*   port  : サーバーのポート番号
*/
void SetupSendSock(char *server_name, u_short port)
{
    /*変数*/
    // サーバー
    struct hostent *server;
    // サーバーの設定
    struct sockaddr_in sv_addr;

    /** サーバーに接続 **/
    fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
    // サーバーの名前を取得する
    if ((server = gethostbyname(server_name)) == NULL)
    {
        char m[] = "gethostbyname()";
        HandleError(m);
    }
    /*
  * ソケットの生成
  * ネットワークアドレスの種類：インタネット
  * ソケットの種類：TCP用ストリームソケット
  * プロトコル：自動選択でTCPが選ばれる
  */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        char m[] = "socket()";
        HandleError(m);
    }

    /*sv_addrの設定*/
    sv_addr.sin_family = AF_INET;                               //アドレスの種類：インターネット
    sv_addr.sin_port = htons(port);                             //ポート番号
    sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0]; //指定されたのアドレスから受付可能

    /*sv_adderの設定でソケットで接続を開く*/
    if (connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
    {
        char m[] = "connect()";
        HandleError(m);
    }

    /*名前の入力*/
    // fprintf(stderr, "Input your name: ");
    // 名前を格納する変数
    char user_name[MAX_LEN_NAME];
    NameSetUp();
    sprintf(user_name, "%s", MyName);

    // 名前を送信する
    SendData(user_name, MAX_LEN_NAME);

    /** サーバーとの接続後 **/
    // 他のクライアントの接続を待つ
    fprintf(stderr, "Waiting for other Clients...\n");

    /** 全員が揃ったら **/
    // ゲームに参加している人数を受け取る
    ReceiveData(&NumClients, sizeof(int));
    // 受け取った人数を表示
    fprintf(stderr, "Number of Clients = %d.\n", NumClients);
    GetSystem().SetNumClients(NumClients); // システムに渡す
    // 自身のIDを受け取る
    ReceiveData(&MyId, sizeof(int));
    // 受け取ったIDをシステムモジュールに渡す
    GetSystem().SetMyID(MyId);
    fprintf(stderr, "Your ID = %d.\n", MyId);

    /** 全クライアントの情報を受け取る **/
    int i;
    PlayerName* Name_Clients = new PlayerName[NumClients];

    for (i = 0; i < NumClients; i++)
    {
        ReceiveData(&Clients[i], sizeof(NetworkData));
        ReceiveData(&Name_Clients[i], sizeof(PlayerName));
    }
    for (int i = 0; i < NumClients; ++i) {
        GetSystem().SetClientName(i, Name_Clients[i].name);
    }
    delete[] Name_Clients;
    // マップデータ入手
    int terrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D];
    for(int l = 0; l < MAP_SIZE_W; ++l) {
        for(int j = 0; j < MAP_SIZE_H; ++j) {
            for(int k = 0; k < MAP_SIZE_D; ++k) {
                ReceiveData(&(terrainData[l][j][k]), sizeof(int));
            }
        }
    }
    GetSystem().GetClientMap().SetMapData(MAP_SIZE_W, MAP_SIZE_H, MAP_SIZE_D, terrainData);

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    NumSock = sock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。[1]
    FD_ZERO(&Mask);
    // 第一引数のファイルディスクリプタをセットに追加。[1]
    FD_SET(0, &Mask);
    // 第一引数のファイルディスクリプタをセットに追加。[1]
    FD_SET(sock, &Mask);

    /** 初期設定終了 **/
    fprintf(stderr, "Input command (M=message, Q=quit): \n");
}

/*
* 入力があった場合コマンドを受け付けメッセージを送信する
* 引数
*    char com: コマンド
*    FloatPosition data: 座標
* 返り値
*    通信継続：result = 1
*/
int InCommand(char com)
{
    /*変数*/
    // システムモジュールからデータをもらう
    const PlayerData* pData = GetSystem().GetPlayerData();
    // ソケットに送るデータ達
    FloatPosition posData = {pData[MyId].velocity.x, pData[MyId].velocity.y, pData[MyId].velocity.z};
    float direction = pData[MyId].direction.horizontal;

    /** 入力されたコマンドに応じて分岐 **/
    switch (com)
    {
    case MOVE_COMMAND:
        // コマンド送信
        SendData(&com, sizeof(char));
        // データを送信する
        SendData(&posData, sizeof(FloatPosition));
        SendData(&direction, sizeof(float));
        break;
    case PUT_COMMAND:
        PlaceData placeData;
        placeData = GetSystem().GetPlaceData();
        SendData(&com, sizeof(char));
        SendData(&placeData, sizeof(PlaceData));
        break;
    default:
        // 存在しないコマンドの場合はメッセージを表示して、再入力させる
        fprintf(stderr, "%c is not a valid command.\n", com);
        break;
    }

    return 1;
}

/*データを送信する
 * 引数
 *    void *data: 送信するデータ
 *    int size: データのサイズ
 *
*/
void SendData(void *data, int size)
{
    // データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0))
    {
        //メッセージを表示して終了
        fprintf(stderr, "SendData(): data is illegal.\n");
        exit(1);
    }

    /*ソケットにデータを送る*/
    if (write(sock, data, size) == -1)
    {
        char m[] = "write()";
        HandleError(m);
    }
}

/*データを送る
 * 引数
 *    *data: 送信するデータ
 *    size: データのサイズ
 *
*/
int ReceiveData(void *data, int size)
{
    // データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0))
    {
        //メッセージを表示して終了
        fprintf(stderr, "ReceiveData(): data is illegal.\n");
        return -1;
    }
    // ソケットに送られてきたデータをdataに読み込む
    return (read(sock, data, size));
}


/* エラーの表示
 *
 * 引数
 *   *message: エラーの発生した段階
 */
static int HandleError(char *message)
{
    // エラーメッセージを表示
    perror(message);
    fprintf(stderr, "%d\n", errno);
    //
    return -1;
}

/*
* クライアントの削除
*/
void TerminateSendSock()
{
    char com = QUIT_COMMAND;
    // データを送信する
    SendData(&com, sizeof(char));

    // SDL_Delay(1000);
    // メッセージを表示
    fprintf(stderr, "Connection is closed.\n");
    // ソケットを閉じる
    close(sock);
    // プログラムの終了
    // exit(0);
}
