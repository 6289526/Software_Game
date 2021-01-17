/**
 * @file net.cpp
 * @brief クライアントのネットワークモジュール
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
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

/**
 * @brief クライアントのソケットの構築
 * 
 * @param server_name 
 * @param port 
 */
void SetupClient(char *server_name, u_short port)
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
    PlayerName *Name_Clients = new PlayerName[NumClients];

    for (i = 0; i < NumClients; i++)
    {
        ReceiveData(&Clients[i], sizeof(NetworkData));
        ReceiveData(&Name_Clients[i], sizeof(PlayerName));
    }
    for (int i = 0; i < NumClients; ++i)
    {
        GetSystem().SetClientName(i, Name_Clients[i].name);
    }
    delete[] Name_Clients;
    // マップデータ入手
    int terrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D];
    int w,h,d;
    ReceiveData(&w,sizeof(int));
    ReceiveData(&h,sizeof(int));
    ReceiveData(&d,sizeof(int));

    for(int l = 0; l < MAP_SIZE_W; ++l) {
        for(int j = 0; j < MAP_SIZE_H; ++j) {
            for(int k = 0; k < MAP_SIZE_D; ++k) {

                ReceiveData(&(terrainData[l][j][k]), sizeof(int));
            }
        }
    }
    GetSystem().GetClientMap().SetMapData(w, h, d, terrainData);

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    NumSock = sock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。[1]
    FD_ZERO(&Mask);
    // 第一引数のファイルディスクリプタをセットに追加。[1]
    FD_SET(0, &Mask);
    // 第一引数のファイルディスクリプタをセットに追加。[1]
    FD_SET(sock, &Mask);

    
    fprintf(stderr, "Input command (M=message, Q=quit): \n");
}

/**
 * @brief リクエスト管理
 * 
 * @return int 
 */
int ControlRequests()
{
    /*変数*/
    // ファイルディスクリプタ
    fd_set read_flag = Mask;
    // タイマー
    struct timeval timeout;
    timeout.tv_sec = 0;   //秒数
    timeout.tv_usec = 30; //マイクロ秒

    /*
   *ファイルディスクリプタの集合から読み込み可能なファイルディスクリプタを
   *探す。timeoutだけ待つ。
  */
    if (select(NumSock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1)
    {
        char m[] = "select()";
        HandleError(m);
    }
    // 通信を継続するかを判定する変数
    int result = 1;

    if (FD_ISSET(sock, &read_flag))
    { //サーバーからのメッセージを受け取った場合
        result = ExeCommand();
    }

    return result;
}

/**
 * @brief コマンドを受け取る
 * 
 * @param com 
 * @return int 
 */
int InCommand(char com)
{
    /*変数*/
    // システムモジュールからデータをもらう
    const PlayerData *pData = GetSystem().GetPlayerData();
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
    case QUIT_COMMAND:
        // データを送信する
        SendData(&com, sizeof(char));
        break;
    default:
        // 存在しないコマンドの場合はメッセージを表示して、再入力させる
        fprintf(stderr, "%c is not a valid command.\n", com);
        break;
    }

    return 1;
}

/**
 * @brief 受け取ったコマンドを実行
 * 
 * @return int 通信終了->0
 */
int ExeCommand()
{
    /*変数*/
    char com;
    // ソケットから来るデータ
    FloatPosition data[MAX_NUMCLIENTS];
    PlaceData placeData;
    float direction = 0;
    VelocityFlag flags[MAX_NUMCLIENTS];
    int id = 0;
    int rank = 0;
    time_t timer;

    // 通信を継続するかを判定する変数
    int result = 1;
    // dataの初期化
    for (int i = 0; i < MAX_NUMCLIENTS; i++)
    {
        memset(&data[i], 0, sizeof(FloatPosition));
    }
    memset(&placeData, 0, sizeof(PlaceData));
    // dataを受信する
    ReceiveData(&com, sizeof(com));
    /** 受信したデータに含まれるコマンドに応じて分岐 **/
    switch (com)
    {
    case MOVE_COMMAND: // 座標受信
        for (int i = 0; i < NumClients; i++)
        {
            ReceiveData(&data[i], sizeof(FloatPosition));
            ReceiveData(&direction, sizeof(float));
            GetSystem().SetDirection(direction, i);
            ReceiveData(&flags[i], sizeof(VelocityFlag));
        }
        // 受け取った座標とフラッグをシステムモジュールにわたす
        GetSystem().SetPlace(data, NumClients);

        GetSystem().UpdateFlag(flags, NumClients);

        // 通信継続
        result = 1;
        break;
    case PUT_COMMAND:
        ReceiveData(&placeData, sizeof(PlaceData));

        if (placeData.object != NonBlock)
        {
            fprintf(stderr, "ブロック置けた！\n");
            GetSystem().UpdatePlaceData(placeData);
        }
        else
        {
            fprintf(stderr, "ブロックがおけなかった\n");
        }

        result = 1;
        break;
    case QUIT_COMMAND: // 通信終了
        ReceiveData(&id, sizeof(int));
        // 通信を終了したことを表示
        fprintf(stderr, "client %d sent quit command.\n", id);
        // 通信継続
        result = 1;
        break;
    case RANK_COMMAND:
            ReceiveData(&rank, sizeof(int));
            GetSystem().SetRank(MyId, rank);
        break;
    case TIMER_COMMAND: // 通信終了
        ReceiveData(&timer, sizeof(time_t));
        GetSystem().GetTimer().SetCurrentTime(timer);
        // 通信継続
        result = 1;
        break;
    case GOAL_COMMAND:
        ReceiveData(&rank, sizeof(int));
        fprintf(stderr, "you goaled.\n");
        GetSystem().SetRank(MyId, rank);
        GetSystem().GetSoundController().GetSoundEffectSubject().OnNest(Sound::Goal);
        result = 1;
        break;
    case FINISH_COMMAND:
        fprintf(stderr, "All clients goaled.\n");
        for (int i = 0; i < NumClients; ++i)
        {
            ReceiveData(&rank, sizeof(int));
            GetSystem().SetRank(i, rank);
        }
        result = 0;

        break;
    case TERMINATE_COMMAND:
        // サーバーが通信を終了したことを表示
        fprintf(stderr, "server sent terminate command.\n");
        // 通信終了
        result = 0;

        break;
    default:
        // 上記以外のコマンドは存在しないので、エラーを表示して終了
        fprintf(stderr, "ExeCommand(): %c, %d is not a valid command.\n", com, com);
        // 通信継続
        result = 1;
    }

    // 値を返す
    return result;
}

/**
 * @brief データを送る
 * 
 * @param data 
 * @param size 
 */
void SendData(void *data, int size)
{
    // データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0))
    {
        //メッセージを表示して終了
        fprintf(stderr, "SendData(): data is illegal.\n");
    }

    /*ソケットにデータを送る*/
    if (write(sock, data, size) == -1)
    {
        char m[] = "write()";
        HandleError(m);
    }
}

/**
 * @brief データを受け取る
 * 
 * @param data 
 * @param size 
 * @return int 
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

/**
 * @brief エラーを表示
 * 
 * @param message 
 * @return int 
 */
static int HandleError(char *message)
{
    // エラーメッセージを表示
    perror(message);
    fprintf(stderr, "%d\n", errno);
    //
    return -1;
}

/**
 * @brief クライアントのソケットを閉じる
 * 
 */
void TerminateClient()
{
    // メッセージを表示
    fprintf(stderr, "Connection is closed.\n");
    // ソケットを閉じる
    close(sock);
}
