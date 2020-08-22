//ポート番号をもう一つ作ってソケットを２つ作ればいい
/*----------include 開始----------*/
#include "../../header/client_common.h"

/*----------include 終了----------*/

/*変数初期化*/
// 接続してくるクライアントの数
int num_clients;
// 自身のID
int myid;
// ソケット
int sock;
// ソケットの数
int num_sock;
// ファイルディスクリプタ
fd_set mask;
// クライアントの情報
Client clients[MAX_NUM_CLIENTS];

/*関数*/
static void handle_error(char *);

/*クライアントの初期設定
* 引数
*   *server_name: 接続するサーバーの名前
*   port  : サーバーのポート番号
*/
void setup_client(char *server_name, u_short port) {
    /*変数*/
    // サーバー
    struct hostent *server;
    // サーバーの設定
    struct sockaddr_in sv_addr;

    /** サーバーに接続 **/
    fprintf(stderr, "Trying to connect server %s (port = %d).\n", server_name, port);
    // サーバーの名前を取得する
    if ((server = gethostbyname(server_name)) == NULL) {
        handle_error("gethostbyname()");
    }
    /*
    * ソケットの生成
    * ネットワークアドレスの種類：インタネット
    * ソケットの種類：TCP用ストリームソケット
    * プロトコル：自動選択でTCPが選ばれる
    */ 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        handle_error("socket()");
    }
    
    /*sv_addrの設定*/
    sv_addr.sin_family = AF_INET;                               //アドレスの種類：インターネット
    sv_addr.sin_port = htons(port);                             //ポート番号
    sv_addr.sin_addr.s_addr = *(u_int *)server->h_addr_list[0]; //指定されたのアドレスから受付可能

    /*sv_adderの設定でソケットで接続を開く*/
    if (connect(sock, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) != 0) {
        handle_error("connect()");
    }

    /** サーバーとの接続後 **/
    /*名前の入力*/
    fprintf(stderr, "Input your name: ");
    // 名前を格納する変数
    char user_name[MAX_LEN_NAME];
    if (fgets(user_name, sizeof(user_name), stdin) == NULL) {
        handle_error("fgets()");
    }
    // 名前の後ろをナル文字にする
    user_name[strlen(user_name) - 1] = '\0';
    // 名前を送信する
    send_data(user_name);

    /** 全員が揃ったら **/
    // チャットに参加している人数を受け取る
    receive_data(&num_clients);
    // 受け取った人数を表示
    fprintf(stderr, "Number of clients = %d.\n", num_clients);
    // 自身のIDを受け取る
    receive_data(&myid);
    // 受け取ったIDを表示
    fprintf(stderr, "Your ID = %d.\n", myid);

    
    /** 全クライアントの情報を受け取る **/
    int i;
    for (i = 0; i < num_clients; i++) {
        receive_data(&clients[i]);
    }

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    num_sock = sock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。
    FD_ZERO(&mask);
    // 第一引数のファイルディスクリプタをセットに追加。
    FD_SET(0, &mask);
    // 第一引数のファイルディスクリプタをセットに追加。
    FD_SET(sock, &mask);

    
    /** 初期設定終了 **/
    fprintf(stderr, "Input command (R,P,S, Q=quit): \n");
    
}

/* クライアントのリクエスト処理
* 返値
*   通信継続: result = 1
*   通信終了: result = 0
*/
int control_requests () {
    /*変数*/
    // ファイルディスクリプタ
    fd_set read_flag = mask;
    // タイマー
    struct timeval timeout;
    timeout.tv_sec = 0;//秒数
    timeout.tv_usec = 30;//マイクロ秒

    /*
    *ファイルディスクリプタの集合から読み込み可能なファイルディスクリプタを
    *探す。timeoutだけ待つ。
    */
    if (select(num_sock, (fd_set *)&read_flag, NULL, NULL, &timeout) == -1) {
        handle_error("select()");
    }
    // 通信を継続するかを判定する変数
    int result = 1;
    
    if (FD_ISSET(0, &read_flag)) { //入力があった場合コマンドを受け付け、それに応じた処理を行う
        result = in_command();
    } else if (FD_ISSET(sock, &read_flag)) {  //サーバーからのメッセージを受け取った場合
        result = exe_command();
    }
    return result;
}

/*
* 入力があった場合コマンドを受け付けメッセージを送信する
* 返り値
*    通信継続：result = 1
*/
static int in_command() {
    /*変数*/
    //コマンド
    Command command;
    // キーボードの入力を格納する変数
    char com;
    // commandの初期化
    memset(&command, 0, sizeof(Command));
    /** キーボードからの入力を受け付ける **/
    com = getchar();
    while(getchar()!='\n');

    /** 入力されたコマンドに応じて分岐 **/
    switch (com) {
    case MOVE_COMMAND: //移動コマンド
        //送るデータ
        Move_Data data;
        // dataの初期化
        memset(&data, 0, sizeof(Move_Data));
        // コマンドを送信するデータの中に格納
        command.command = MOVE_COMMAND;
        // 自分のIDを送信するデータの中に格納
        command.cid = myid;
        data.cid = myid;
        //移動する場所を入力
        data.pos = clients[myid].pos;
        // データを送信する
        send_data(&command);
        send_data(&data);
        break;
    case PUT_COMMAND: //移動コマンド
        //送るデータ
        Place_Data data;
        // dataの初期化
        memset(&data, 0, sizeof(Place_Data));
        // コマンドを送信するデータの中に格納
        command.command = PUT_COMMAND;
        // 自分のIDを送信するデータの中に格納
        command.id = myid;
        data.cid = myid;
        //配置する場所を入力
        data.pos = {clients[myid].pos.x, clients[myid].pos.y, clients[myid].pos.z - 100};
        // データを送信する
        send_data(&command);
        send_data(&data);
        break;
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
static int exe_command() {
    /*変数*/
    // ソケットから来るデータ
    Command command;
    // 通信を継続するかを判定する変数
    int result = 1;
    // dataの初期化
    memset(command, 0, sizeof(Command));
    // dataを受信する
    receive_data(&command);

    /** 受信したデータに含まれるコマンドに応じて分岐 **/
    switch (command.command) {
    case MOVE_COMMAND: // メッセージ受信
        Move_Data data;
        memset(data, 0, sizeof(Move_Data));
        receive_data(&data);
        if(command.able){ //コマンドが実行可能なら
            fprintf(stderr, "%d,%d,%dに移動しました\n", data.pos.x, data.pos.y, data.pos.z);
        }else
        {
            clients[data.id].pos = data.pos;
            fprintf(stderr, "移動できませんでした。\n");
        }
        
        // 通信継続
        result = 1;
        break;
    case PUT_COMMAND: // 通信終了
        Place_Data data;
        memset(data, 0, sizeof(Place_Data));
        receive_data(&data);
        if(command.able){ //コマンドが実行可能なら
            
            fprintf(stderr, "%d,%d,%dに%dを置きました\n", data.pos.x, data.pos.y, data.pos.z, data.object);
        }else{
            fprintf(stderr, "置けませんでした\n");
        }
        // 通信終了
        result = 0;
        break;
    default:
        // 上記以外のコマンドは存在しないので、エラーを表示して終了
        fprintf(stderr, "exe_command(): %c is not a valid command.\n", data.command);
        exit(1);
    }

    // 値を返す
    return result;
}