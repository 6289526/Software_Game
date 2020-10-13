/*
 *  ファイル名	：server.c
 *  機能	：変数と関数の定義、サーバーの処理
 * 
 * 参考文献
 *     [1] https://kazmax.zpp.jp/cmd/i/inet_ntop.3.html
 *     [2] https://qiita.com/edo_m18/items/7414028fd91269e5427d
 */
#include "server_common.h"

/*変数初期化*/
// クライアントの情報
static NetworkData clients[MAX_NUMCLIENTS];
// 接続してくるクライアントの数
static int num_clients;
// ソケットの数
static int num_socks;
// ファイルディスクリプタ
static fd_set mask;
// // チャット用のソケット間で送信される情報
// static CONTAINER data;

/*関数*/
static void HandleError(char *);

/* サーバー初期化
 *
 * 引数
 *   num_cl: チャットに参加する人数
 *   port  : サーバーのポート番号
 * 
 */
void SetupServer(int num_cl, u_short port)
{
    /*変数初期化*/
    /*ソケット
  * rsock: 接続リクエストを受け取るソケット
  * sock: チャットを行うためのソケット
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

    //チャットに参加する人数
    num_clients = num_cl;

    /*
  * ソケットの生成
  * ネットワークアドレスの種類：インタネット
  * ソケットの種類：TCP用ストリームソケット
  * プロトコル：自動選択でTCPが選ばれる
  */
    rsock = socket(AF_INET, SOCK_STREAM, 0);
    if (rsock < 0)
    {
        HandleError("socket()");
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
        HandleError("bind()");
    }
    fprintf(stderr, "bind() is done successfully.\n");

    /*接続準備*/
    if (listen(rsock, num_clients) != 0)
    {
        HandleError("listen()");
    }
    fprintf(stderr, "listen() is started.\n");

    /** クライアントからの通信リクエストに対する処理 **/
    /*変数*/
    int max_sock = 0;
    // 接続先アドレス情報のサイズ
    socklen_t len;

    char src[MAX_LEN_ADDR];
    char name[MAX_NUMCLIENTS][MAX_LEN_NAME];
    /* 
  *  num_clientsの人数のクライアントが通信を要求してくるまで
  *  ここから先には進まない
  */
    for (int i = 0; i < num_clients; i++)
    {
        // 接続先アドレス情報のサイズ
        len = sizeof(cl_addr);
        /*接続するソケットの生成と接続の確立*/
        sock = accept(rsock, (struct sockaddr *)&cl_addr, &len);
        if (sock < 0)
        {
            HandleError("accept()");
        }
        /*ソケットの最大個数を更新*/
        if (max_sock < sock)
        {
            max_sock = sock;
        }

        /*クライアントからの名前を読み込む*/
        if (read(sock, name[i], MAX_LEN_NAME) == -1)
        {
            HandleError("read()");
        }
        // 名前をシステムモジュールに渡す
        GetClientName(i, name[i]);
        // 接続していることを示す
        clients[i].connect = 0;
        // 使用するソケット
        clients[i].sock = sock;
        // ソケットの設定
        clients[i].addr = cl_addr;
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

    /*接続したクライアントに情報を送る*/
    for (int i = 0; i < num_clients; i++)
    {
        // 接続しているクライアントの人数を送る
        SendData(i, &num_clients, sizeof(int));
        // 自身のIDを送る
        SendData(i, &i, sizeof(int));
        for (int j = 0; j < num_clients; j++)
        {
            // 接続しているクライアントの情報を送る
            SendData(i, &clients[j], sizeof(CLIENT));
        }
    }

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    num_socks = max_sock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。[2]
    FD_ZERO(&mask);
    // 第一引数のファイルディスクリプタをセットに追加。[2]
    FD_SET(0, &mask);

    for (int i = 0; i < num_clients; i++)
    {
        // 第一引数のファイルディスクリプタをセットに追加。[2]
        FD_SET(clients[i].sock, &mask);
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
    fd_set read_flag = mask;
    // dataの初期化
    FloatPosition data;
    memset(&data, 0, sizeof(FloatPosition));

    /** ソケット通信の多重化 **/
    fprintf(stderr, "select() is started.\n");
    /* ファイルディスクリプタの集合から読み込み可能なファイルディスクリプタを見つける*/
    if (select(num_socks, (fd_set *)&read_flag, NULL, NULL, NULL) == -1)
    {
        HandleError("select()");
    }

    /** データの受信と送信 **/
    /*変数*/
    int i, result = 1;
    for (i = 0; i < num_clients; i++)
    {
        /* 第一引数のファイルディスクリプタがセット内にあるかを調べる。[2]*/
        if (FD_ISSET(clients[i].sock, &read_flag))
        {
            /*データを受け取る
      * クライアントのID
      * メッセージを送信するか、通信を終了するかのコマンド
      * メッセージ
       */
            ReceiveData(i, &com, sizeof(char));
            switch (com)
            {
            case MOVE_COMMAND: //メッセージ送信を要求された場合
                ReceiveData(i, &data, sizeof(FloatPosition));
                fprintf(stderr, "client[%d]: message = %f %f %f\n", i, data.x, data.y, data.z);
                // 受け取った座標をシステムモジュールに渡す
                GetPosition(i, data);
                // SendAllPos(num_clients);
                // // 全員にメッセージを送信する
                // SendData(BROADCAST, &com, sizeof(com));
                // SendData(BROADCAST, &data, sizeof(data));
                // チャットの継続
                result = 1;
                break;
            case QUIT_COMMAND: //通信の終了を要求された場合
                fprintf(stderr, "client[%d]: quit\n", i);
                // 通信が終了したクライアントがあることを全員に知らせる
                SendData(BROADCAST, &com, sizeof(com));
                // チャットの終了
                result = 0;
                break;
            default:
                // コマンドは上記の2種類しか無いので、それ以外の場合はエラーが生じている　
                fprintf(stderr, "ControlRequests(): %c is not a valid command.\n", com);
                break;
            }
        }
    }

    // 値を返す
    return result;
}

/* ソケットにデータを送信
 *
 * 引数
 *    cid:送り先
 *    *data:送られるデータ
 *    size:dataの型のサイズ
 */
void SendData(int cid, void *data, int size)
{
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= num_clients))
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
        for (i = 0; i < num_clients; i++)
        {
            if (write(clients[i].sock, data, size) < 0)
            {
                HandleError("write()");
            }
        }
    }
    else
    { //特定のクライアントに送るとき
        //特定のソケットに情報を送る
        if (write(clients[cid].sock, data, size) < 0)
        {
            HandleError("write()");
        }
    }
}

/* データの受信
 *
 * 引数
 *    cid:送り先
 *    *data:送られるデータ
 *    size:dataの型のサイズ
 * 返り値
 *     
 *    エラーの場合0,-1を返す
 */
int ReceiveData(int cid, void *data, int size)
{
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= num_clients))
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
    return read(clients[cid].sock, data, size);
}

/* エラーの表示
 *
 * 引数
 *   *message: エラーの発生した段階
 */
static void HandleError(char *message)
{
    // エラーメッセージを表示
    perror(message);
    fprintf(stderr, "%d\n", errno);
    // プログラム終了
    exit(1);
}

/* サーバーの終了 */
void TerminateServer(void)
{
    /*変数*/
    int i;
    /*すべてのソケットを閉じる*/
    for (i = 0; i < num_clients; i++)
    {
        close(clients[i].sock);
    }
    // メッセージの表示
    fprintf(stderr, "All connections are closed.\n");
    // 正常終了
    exit(0);
}
