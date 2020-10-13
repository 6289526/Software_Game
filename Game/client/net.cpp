/*
 *  ファイル名	：client.c
 *  機能	：変数と関数の定義、クライアントの処理
 * 
 * 参考文献
 *      [1] https://qiita.com/edo_m18/items/7414028fd91269e5427d
 *      [2] https://qiita.com/toshihirock/items/78286fccf07dbe6df38f#%E5%8F%82%E8%80%83
 */
#include "client_common.h"

/*変数初期化*/
// 接続してくるクライアントの数
static int num_clients;
// 自身のID
static int myid;
// ソケット
static int sock;
// ソケットの数
static int num_sock;
// ファイルディスクリプタ
static fd_set mask;
// クライアントの情報
static NetworkData clients[MAX_NUMCLIENTS];

/*関数*/
static void HandleError(char *);

/*クライアントの初期設定
* 引数
*   *server_name: 接続するサーバーの名前
*   port  : サーバーのポート番号
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
        HandleError("gethostbyname()");
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
        HandleError("socket()");
    }

    /*sv_addrの設定*/
    sv_addr.sin_family = AF_INET;                               //アドレスの種類：インターネット
    sv_addr.sin_port = htons(port);                             //ポート番号
    sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0]; //指定されたのアドレスから受付可能

    /*sv_adderの設定でソケットで接続を開く*/
    if (connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0)
    {
        HandleError("connect()");
    }

    /*名前の入力*/
    fprintf(stderr, "Input your name: ");
    // 名前を格納する変数
    char user_name[MAX_LEN_NAME];
    if (fgets(user_name, sizeof(user_name), stdin) == NULL)
    {
        HandleError("fgets()");
    }
    // 名前の後ろをナル文字にする
    user_name[strlen(user_name) - 1] = '\0';
    // 名前を送信する
    SendData(user_name, MAX_LEN_NAME);

    /** サーバーとの接続後 **/
    // 他のクライアントの接続を待つ
    fprintf(stderr, "Waiting for other clients...\n");

    /** 全員が揃ったら **/
    // チャットに参加している人数を受け取る
    ReceiveData(&num_clients, sizeof(int));
    // 受け取った人数を表示
    fprintf(stderr, "Number of clients = %d.\n", num_clients);
    // 自身のIDを受け取る
    ReceiveData(&myid, sizeof(int));
    // 受け取ったIDをシステムモジュールに渡す
    GetId(myid);
    fprintf(stderr, "Your ID = %d.\n", myid);

    /** 全クライアントの情報を受け取る **/
    int i;
    for (i = 0; i < num_clients; i++)
    {
        ReceiveData(&clients[i], sizeof(CLIENT));
    }

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    num_sock = sock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。[1]
    FD_ZERO(&mask);
    // 第一引数のファイルディスクリプタをセットに追加。[1]
    FD_SET(0, &mask);
    // 第一引数のファイルディスクリプタをセットに追加。[1]
    FD_SET(sock, &mask);

    /** 初期設定終了 **/
    fprintf(stderr, "Input command (M=message, Q=quit): \n");
}

/* クライアントのリクエスト処理
 * 返値
 *   通信継続: result = 1
 *   通信終了: result = 0
 */
int ControlRequests()
{
    /*変数*/
    // ファイルディスクリプタ
    fd_set read_flag = mask;
    // タイマー
    struct timeval timeout;
    timeout.tv_sec = 0;   //秒数
    timeout.tv_usec = 30; //マイクロ秒

    /*
   *ファイルディスクリプタの集合から読み込み可能なファイルディスクリプタを
   *探す。timeoutだけ待つ。
  */
    if (select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1)
    {
        HandleError("select()");
    }
    // 通信を継続するかを判定する変数
    int result = 1;

    // if (FD_ISSET(0, &read_flag))
    // { //入力があった場合コマンドを受け付け、それに応じた処理を行う
    //     result = InCommand();
    // }
    // else 
    if (FD_ISSET(sock, &read_flag))
    { //サーバーからのメッセージを受け取った場合
        result = ExeCommand();
    }

    return result;
}

/*
* 入力があった場合コマンドを受け付けメッセージを送信する
* 返り値
*    通信継続：result = 1
*/
int InCommand(char com, FloatPosition data)
{
    /*変数*/
    // // ソケットに送るデータ
    // FloatPosition data;
    // // キーボードの入力を格納する変数
    // char com;

    // // dataの初期化
    // memset(&data, 0, sizeof(FloatPosition));

    /** キーボードからの入力を受け付ける **/
    // com = getchar();
    // while (getchar() != '\n')
    //     ;

    /** 入力されたコマンドに応じて分岐 **/
    switch (com)
    {
    case MOVE_COMMAND: //Mが入力されたとき
        // fprintf(stderr, "Input message: ");
        // char c[MAX_LEN_BUFFER];
        // /*送信したいメッセージを送信するデータの中に格納*/
        // if (fgets(c, MAX_LEN_BUFFER, stdin) == NULL)
        // {
        //     HandleError("fgets()");
        // }
        // c[strlen(c) - 1] = '\0';
        // int x, y, z;
        // sscanf(c, "%f %f %f", &data.x, &data.y, &data.z);
        // // コマンドを送信するデータの中に格納
        // data.command = MESSAGE_COMMAND;
        // // メッセージの後ろをナル文字列にする
        // data.message[strlen(data.message) - 1] = '\0';
        // // 自分のIDを送信するデータの中に格納
        // data.cid = myid;
        // コマンド送信
        SendData(&com, sizeof(char));
        // データを送信する
        SendData(&data, sizeof(FloatPosition));
        break;
    // case QUIT_COMMAND: //Qが入力されたとき
    //     // // コマンドを送信するデータの中に格納
    //     // data.command = QUIT_COMMAND;
    //     // // 自分のIDを送信するデータの中に格納
    //     // data.cid = myid;

    //     SendData(&com, sizeof(char));
    //     // データを送信する
    //     // SendData(&data, sizeof(CONTAINER));
    //     break;
    default:
        // 存在しないコマンドの場合はメッセージを表示して、再入力させる
        fprintf(stderr, "%c is not a valid command.\n", com);
    }

    return 1;
}

/*
* メッセージを受け取った場合表示する
* 返り値
*   通信継続: result = 1
*   通信終了: result = 0
*/
int ExeCommand()
{
    /*変数*/
    char com;
    // ソケットに送るデータ
    FloatPosition data[MAX_NUMCLIENTS];
    // 通信を継続するかを判定する変数
    int result = 1;
    // dataの初期化
    for (int i = 0; i < MAX_NUMCLIENTS; i++)
    {
        memset(&data[i], 0, sizeof(FloatPosition));
    }
    // dataを受信する
    ReceiveData(&com, sizeof(com));
    /** 受信したデータに含まれるコマンドに応じて分岐 **/
    switch (com)
    {
    case MOVE_COMMAND: // メッセージ受信
        for (int i = 0; i < num_clients; i++)
        {
            ReceiveData(&data[i], sizeof(FloatPosition));
        }
        // 受け取った座標をシステムモジュールにわたす
        GetPlace(data, num_clients);
        // 受け取ったメッセージを表示する
        // fprintf(stderr, "client: message = %f %f %f\n", data.x, data.y, data.z);
        // 通信継続
        result = 1;
        break;
    case QUIT_COMMAND: // 通信終了
        // 通信を終了したクライアントを表示
        fprintf(stderr, "client sent quit command.\n");
        // 通信終了
        result = 0;
        break;
    default:
        // 上記以外のコマンドは存在しないので、エラーを表示して終了
        fprintf(stderr, "ExeCommand(): %c is not a valid command.\n", com);
        exit(1);
    }

    // 値を返す
    return result;
}

/*データを送信する
 * 引数
 *    *data: 送信するデータ
 *    size: データのサイズ
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
        HandleError("write()");
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
        exit(1);
    }
    // ソケットに送られてきたデータをdataに読み込む
    return (read(sock, data, size));
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

/*
* クライアントの削除
*/
void TerminateClient()
{

    // // ソケットに送るデータ
    // CONTAINER data;
    // // dataの初期化
    // memset(&data, 0, sizeof(CONTAINER));
    // // コマンドを送信するデータの中に格納
    // data.command = QUIT_COMMAND;
    // // 自分のIDを送信するデータの中に格納
    // data.cid = myid;
    char com = QUIT_COMMAND;
    // データを送信する
    SendData(&com, sizeof(char));

    SDL_Delay(1000);
    // メッセージを表示
    fprintf(stderr, "Connection is closed.\n");
    // ソケットを閉じる
    close(sock);
    // プログラムの終了
    exit(0);
}
