
/*----------include 開始----------*/
#include "receive.h"

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
CLIENT clients[MAX_NUM_CLIENTS];

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
    
    /** 全員が揃ったら **/
    // チャットに参加している人数を受け取る
    receive_data(&num_clients, sizeof(int));
    // 受け取った人数を表示
    fprintf(stderr, "Number of clients = %d.\n", num_clients);
    // 自身のIDを受け取る
    receive_data(&myid, sizeof(int));
    // 受け取ったIDを受け取る
    fprintf(stderr, "Your ID = %d.\n", myid);

    
    /** 全クライアントの情報を受け取る **/
    int i;
    for (i = 0; i < num_clients; i++) {
        receive_data(&clients[i], sizeof(CLIENT));
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

    //game_stts = GS_COMMAND;
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
    
    // if (FD_ISSET(0, &read_flag)) { //入力があった場合コマンドを受け付けメッセージを送信する
    //   game_stts = GS_COMMAND;
    //   result = in_command();
    // } else if (FD_ISSET(sock, &read_flag)) {  //メッセージを受け取った場合表示する
    //   result = exe_command();
    // }
    if (FD_ISSET(sock, &read_flag)) {  //メッセージを受け取った場合表示する
        result = exe_command();
    }
    return result;
}

