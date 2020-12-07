/*

 *  ファイル名	：net.cpp
 *  機能	：ネットワーク処理
 *
 */
#include "server_common.h"

/*変数初期化*/
// クライアントの情報
static NetworkData Clients[MAX_NUMCLIENTS];
// 接続してくるクライアントの数
static int NumClient;
// ソケットの数
static int NumSock;
// ファイルディスクリプタ
static fd_set Mask;
// サーバーが強制終了状態か
static int TerminateFlag;

static int ClientCount;

static char name[MAX_NUMCLIENTS][MAX_LEN_NAME];

/*関数*/
static int HandleError(char *);
static void SendData(int cid, const void *data, int size);
static int ReceiveData(int cid, void *data, int size);

/* サーバー初期化
 *
 * 引数
 *   num_cl: ゲームに参加する人数
 *   port  : サーバーのポート番号
 *
 */
void SetupServer(int num_cl, u_short port)
{
    /*変数初期化*/
    /*ソケット
  * rsock: 接続リクエストを受け取るソケット
  * sock: 通信を行うためのソケット
  */
    int rsock, sock = 0;
    /*インタネットソケットの設定
  * sv_addr: rsockの設定
  * cl_addr: sockの設定
  */
    struct sockaddr_in sv_addr, cl_addr;

    /** 接続リクエストを受け取るためのソケットを作る **/
    //サーバーのセットアップが開始されたことを表示する
    fprintf(stderr, "Server setup is started.\n");

    //ゲームに参加する人数
    NumClient = num_cl;

    SetNumClients(NumClient); // システムに渡す

    /*
  * ソケットの生成
  * ネットワークアドレスの種類：インタネット
  * ソケットの種類：TCP用ストリームソケット
  * プロトコル：自動選択でTCPが選ばれる
  */
    rsock = socket(AF_INET, SOCK_STREAM, 0);
    if (rsock < 0)
    {
        char s[] = "socket()";
        HandleError(s);
    }
    fprintf(stderr, "sock() for request socket is done successfully.\n");

    /*sv_addrの設定*/
    sv_addr.sin_family = AF_INET;         //アドレスの種類：インターネット
    sv_addr.sin_port = htons(port);       //ポート番号
    sv_addr.sin_addr.s_addr = INADDR_ANY; //任意のアドレスから受付可能

    /*変数*/
    // 要求されたオプションのための値が指定されるバッファ
    int opt = 1;
    // ソケットオプションをセット
    setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /*ソケットにsv_adderの設定を結びつける*/
    if (bind(rsock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
    {
        char s[] = "bind()";
        HandleError(s);
    }
    fprintf(stderr, "bind() is done successfully.\n");

    /*接続準備*/
    if (listen(rsock, NumClient) != 0)
    {
        char s[] = "listen()";
        HandleError(s);
    }
    fprintf(stderr, "listen() is started.\n");

    /** クライアントからの通信リクエストに対する処理 **/
    /*変数*/
    int max_sock = 0;
    // 接続先アドレス情報のサイズ
    socklen_t len;

    char src[MAX_LEN_ADDR];
    /*
  *  NumClientの人数のクライアントが通信を要求してくるまで
  *  ここから先には進まない
  */
    for (int i = 0; i < NumClient; i++)
    {
        // 接続先アドレス情報のサイズ
        len = sizeof(cl_addr);
        /*接続するソケットの生成と接続の確立*/
        sock = accept(rsock, (struct sockaddr *)&cl_addr, &len);
        if (sock < 0)
        {
            char s[] = "accept()";
            HandleError(s);
        }
        /*ソケットの最大個数を更新*/
        if (max_sock < sock)
        {
            max_sock = sock;
        }

        /*クライアントからの名前を読み込む*/
        if (read(sock, name[i], MAX_LEN_NAME) == -1)
        {
            char s[] = "read()";
            HandleError(s);
        }
        // 名前をシステムモジュールに渡す
        SetClientName(i, name[i]);
        // 接続していることを示す
        Clients[i].connect = 1;
        // 使用するソケット
        Clients[i].sock = sock;
        // ソケットの設定
        Clients[i].addr = cl_addr;
        //srcの初期化
        memset(src, 0, sizeof(src));
        /*
    * ネットワークアドレス構造体を、そのアドレスを表す文字列に変換する
    * 構造体の内容を IPv4 ネットワークアドレスの ドット区切り 4 分割形式 "ddd.ddd.ddd.ddd" に変換[1]
    *
    */
        inet_ntop(AF_INET, (struct sockaddr *)&cl_addr.sin_addr, src, sizeof(src));
        //通信を受け入れたことを知らせる
        fprintf(stderr, "Client %d is accepted (name=%s, address=%s, port=%d).\n", i, name[i], src, ntohs(cl_addr.sin_port));
    }
    //接続リクエストを受け取るソケットを閉じる
    close(rsock);

    /** この段階で設定した人数分のクライアントが接続している **/
    ClientCount = NumClient;

    PlayerName *Cname = new PlayerName[NumClient];
    for (int i = 0; i < NumClient; ++i)
    {
        Cname[i].id = i;
        strcpy(Cname[i].name, name[i]);
    }

    /*接続したクライアントに情報を送る*/
    for (int i = 0; i < NumClient; i++)
    {
        // 接続しているクライアントの人数を送る
        SendData(i, &NumClient, sizeof(int));
        // 自身のIDを送る
        SendData(i, &i, sizeof(int));
        for (int j = 0; j < NumClient; j++)
        {
            // 接続しているクライアントの情報を送る
            SendData(i, &Clients[j], sizeof(NetworkData));
            SendData(i, &Cname[j], sizeof(PlayerName));
        }

        // マップデータ入手
        const int(*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();
        for (int l = 0; l < MAP_SIZE_W; ++l)
        {
            for (int j = 0; j < MAP_SIZE_H; ++j)
            {
                for (int k = 0; k < MAP_SIZE_D; ++k)
                {
                    SendData(i, &(terrainData[l][j][k]), sizeof(int));
                }
            }
        }
    }
    delete[] Cname;

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    NumSock = max_sock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。
    FD_ZERO(&Mask);
    // 第一引数のファイルディスクリプタをセットに追加。
    FD_SET(0, &Mask);

    for (int i = 0; i < NumClient; i++)
    {
        // 第一引数のファイルディスクリプタをセットに追加。
        FD_SET(Clients[i].sock, &Mask);
    }

    /** セットアップ終了 **/
    // サーバーのセットアップが完了したことを知らせる
    fprintf(stderr, "Server setup is done.\n");
}

/* クライアントからのリクエストに対応する
 * 返値
 *   通信継続: result = 1
 *   通信終了: result = 0
 */
int ControlRequests()
{
    /*変数*/
    char com;
    // ファイルディスクリプタ
    fd_set read_flag = Mask;
    // 受け取るデータ
    FloatPosition data;
    PlaceData placeData;
    float direction;

    // タイマー
    struct timeval timeout;
    timeout.tv_sec = 1;  //秒数
    timeout.tv_usec = 0; //マイクロ秒

    memset(&data, 0, sizeof(FloatPosition));
    memset(&placeData, 0, sizeof(PlaceData));
    /** ソケット通信の多重化 **/
    //fprintf(stderr, "select() is started.\n");
    /* ファイルディスクリプタの集合から読み込み可能なファイルディスクリプタを見つける*/
    if (select(NumSock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1)
    {
        char s[] = "select()";
        HandleError(s);
    }

    /** データの受信と送信 **/
    /*変数*/
    int i, result = 1;
    // 残っているクライアントの数
    int count = 0;
    if (TerminateFlag)
    {
        // 通信終了
        return result = 0;
    }

    for (i = 0; i < NumClient; i++)
    {
        if (Clients[i].connect == 1)
        {
            /* 第一引数のファイルディスクリプタがセット内にあるかを調べ、接続しているかを確認する*/
            if (FD_ISSET(Clients[i].sock, &read_flag))
            {
                ReceiveData(i, &com, sizeof(char));
                switch (com)
                {
                case MOVE_COMMAND:
                    ReceiveData(i, &data, sizeof(FloatPosition));
                    ReceiveData(i, &direction, sizeof(float));

                    // fprintf(stderr, " [%d] %10s: message = x:%6.3f   y:%6.3f   z:%6.3f   dir:%6.3f\n", i, name[i], data.x, data.y, data.z, direction);
                    // 受け取った座標をシステムモジュールに渡す
                    SetVec(i, data);
                    SetDirection(i, direction);

                    // ゲームの継続
                    result = 1;
                    break;
                case PUT_COMMAND:
                    ReceiveData(i, &placeData, sizeof(PlaceData));
                    SetPlaceData(placeData);
                    PutBlock(i);
                    result = 1;
                    break;
                case QUIT_COMMAND: //通信の終了を要求された場合
                    fprintf(stderr, " [%d] %10s: quit\n", i, name[i]);
                    // 接続を切る
                    Clients[i].connect = 0;

                    /** ファイルディスクリプタの操作 **/
                    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。
                    FD_ZERO(&Mask);
                    // 第一引数のファイルディスクリプタをセットに追加。
                    FD_SET(0, &Mask);

                    for (int j = 0; j < NumClient; j++)
                    {
                        // 接続されている場合
                        if (Clients[j].connect == 1)
                        {
                            // 第一引数のファイルディスクリプタをセットに追加。
                            FD_SET(Clients[j].sock, &Mask);
                            // 数える
                            count++;
                        }
                    }
                    // 誰も接続していない場合
                    if (count == 0)
                    {
                        // 終了
                        result = 0;
                    }
                    else
                    {

                        // ゲームの継続
                        result = 1;
                    }
                    com = QUIT_COMMAND;
                    SendData(BROADCAST, &com, sizeof(char));
                    SendData(BROADCAST, &i, sizeof(int));
                    
                    break;
                default:
                    // コマンドは上記の2種類しか無いので、それ以外の場合はエラーが生じている　
                    fprintf(stderr, "ControlRequests(): %c is not a valid command.\n", com);
                    break;
                }
            }
        }
    }

    //
    return result;
}

/*コマンドの実行
*引数
*   int id: 送り先のID
*   char com: コマンド
*/
void RunCommand(int id, char com)
{
    /* 変数 */
    const PlayerData *pData = GetPlayerData();
    // 送るデータ
    FloatPosition posData;
    float direction;
    VelocityFlag flag = {false, false, false};
    PlaceData placeData = GetPlaceData();
    bool goal = pData[id].goal;
    // コマンドに応じた処理
    switch (com)
    {
    case MOVE_COMMAND:

        //コマンド送信
        SendData(id, &com, sizeof(char));

        for (int i = 0; i < NumClient; ++i)
        {
            // 座標に変更
            posData.x = pData[i].pos.x;
            posData.y = pData[i].pos.y;
            posData.z = pData[i].pos.z;

            //  方向の取得
            direction = GetDirection(i);

            // フラッグ設定
            if (pData->velocity.x != 0)
            {
                flag.x = true;
            }
            if (pData->velocity.y != 0)
            {
                flag.y = true;
            }
            if (pData->velocity.z != 0)
            {
                flag.z = true;
            }
            // 座標と方向とフラッグを送信
            SendData(id, &posData, sizeof(FloatPosition));
            SendData(id, &direction, sizeof(float));
            SendData(id, &flag, sizeof(VelocityFlag));
        }
        break;
    case PUT_COMMAND:
        if (placeData.object == NonBlock)
        {
            fprintf(stderr, "%d can't put\n", id);
        }
        else
        {
            fprintf(stderr, "%d put.\n", id);
        }
        //コマンド送信
        SendData(id, &com, sizeof(char));
        SendData(id, &placeData, sizeof(PlaceData));

        break;

    case FINISH_COMMAND:
        fprintf(stderr, "All clients goaled.\n");
        SendData(id, &com, sizeof(com));
        break;
    case GOAL_COMMAND:
        SendData(id, &com, sizeof(&com));
        fprintf(stderr, "clinet%d goaled!", id);
        // SendData(id, &goal, sizeof(bool));
        break;
    case TERMINATE_COMMAND:
        fprintf(stderr, "Terminate!");
        TerminateFlag = 1;
        // コマンド送信 この時全員
        SendData(id, &com, sizeof(com));
        break;

    default:
        break;
    }
}

/* ソケットにデータを送信
 *
 * 引数
 *    int cid:送り先
 *    void *data:送られるデータ
 *    int size:dataの型のサイズ
 */
void SendData(int cid, const void *data, int size)
{
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= NumClient))
    {
        fprintf(stderr, "SendData(): client id is illegal.\n");
        // 終了
        exit(1);
    }
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0))
    {
        fprintf(stderr, "SendData(): data is illegal.\n");
        // 終了
        exit(1);
    }
    if (cid == BROADCAST)
    { //全員に送るとき
        int i;
        //すべてのクライアントのソケットに情報を送る
        for (i = 0; i < NumClient; i++)
        {
            if (Clients[i].connect)
            {
                if (write(Clients[i].sock, data, size) < 0)
                {
                    char s[] = "write()";
                    HandleError(s);
                }
            }
        }
    }
    else
    {
        if (Clients[cid].connect)
        {
            //特定のクライアントに送るとき
            //特定のソケットに情報を送る
            if (write(Clients[cid].sock, data, size) < 0)
            {
                char s[] = "write()";
                HandleError(s);
            }
        }
    }
}

/* データの受信
 *
 * 引数
 *    int cid:送り先
 *    void *data:送られるデータ
 *    int size:dataの型のサイズ
 * 返り値
 *
 *    エラーの場合0,-1を返す
 */
int ReceiveData(int cid, void *data, int size)
{
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= NumClient))
    {
        fprintf(stderr, "ReceiveData(): client id is illegal.\n");
        // 終了
        exit(1);
    }
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0))
    {
        fprintf(stderr, "ReceiveData(): data is illegal.\n");
        // 終了
        exit(1);
    }
    // ソケットに送られてきたデータをdataに読み込む
    return read(Clients[cid].sock, data, size);
}

/* エラーの表示
 *
 * 引数
 *   char *message: エラーの発生した段階
 */
static int HandleError(char *message)
{
    // エラーメッセージを表示
    perror(message);
    fprintf(stderr, "%d\n", errno);
    //
    return -1;
}

/* サーバーの終了 */
void TerminateServer(void)
{
    /*変数*/
    int i;
    /*すべてのソケットを閉じる*/
    for (i = 0; i < NumClient; i++)
    {
        close(Clients[i].sock);
    }
    // メッセージの表示
    fprintf(stderr, "All connections are closed.\n");
    // 正常終了
    exit(0);
}
