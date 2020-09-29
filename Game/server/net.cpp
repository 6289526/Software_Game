
/*----------include 開始----------*/
#include "server_common.h"

/*----------include 終了----------*/

/*変数初期化*/
// クライアントの情報
NetworkData ClientsNet[MAX_NUMCLIENTS];
// 接続してくるクライアントの数
int NumClients;
// ソケットの数
int NumSocks;
// ファイルディスクリプタ
fd_set Mask;
int NumGoal = 0;
/*関数*/
static void HandleError(char *);

/* サーバー初期化
 *
 * 引数
 *   numCl: チャットに参加する人数
 *   port  : サーバーのポート番号
 *
 */
void SetupServer(int numCl, u_short port) {
    /*変数初期化*/
    /*ソケット
    * rsock: 接続リクエストを受け取るソケット
    * sock: 通信を行うためのソケット
    */
    int rsock, sock = 0;
    /*インタネットソケットの設定
    * svAddr: rsockの設定
    * clAddr: sockの設定
    */
    struct sockaddr_in svAddr, clAddr;

    /** 接続リクエストを受け取るためのソケットを作る **/
    //サーバーのセットアップが開始されたことを表示する
    fprintf(stderr, "Server setup is started.\n");

    //参加する人数
    NumClients = numCl;

    /*
    * ソケットの生成
    * ネットワークアドレスの種類：インタネット
    * ソケットの種類：TCP用ストリームソケット
    * プロトコル：自動選択でTCPが選ばれる
    */
    rsock = socket(AF_INET, SOCK_STREAM, 0);
    if (rsock < 0) {
        HandleError("socket()");
    }
    fprintf(stderr, "sock() for request socket is done successfully.\n");

    /*svAddrの設定*/
    svAddr.sin_family = AF_INET;         //アドレスの種類：インターネット
    svAddr.sin_port = htons(port);       //ポート番号
    svAddr.sin_addr.s_addr = INADDR_ANY; //任意のアドレスから受付可能

    /*変数*/
    // 要求されたオプションのための値が指定されるバッファ
    int opt = 1;
    // ソケットオプションをセット
    setsockopt(rsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /*ソケットにsv_adderの設定を結びつける*/
    if (bind(rsock, (struct sockaddr *)&svAddr, sizeof(svAddr)) != 0) {
        HandleError("bind()");
    }
    fprintf(stderr, "bind() is done successfully.\n");

    /*接続準備*/
    if (listen(rsock, NumClients) != 0) {
        HandleError("listen()");
    }
    fprintf(stderr, "listen() is started.\n");

    /** クライアントからの通信リクエストに対する処理 **/
    /*変数*/
    int i, maxSock = 0;
    // 接続先アドレス情報のサイズ
    socklen_t len;

    char src[MAX_LEN_ADDR];
    //クライアントの名前
    char name[NumClients][MAX_LEN_NAME];
    //クライアントのID
    // int id[NumClients];

    /*
    *  NumClientsの人数のクライアントが通信を要求してくるまで
    *  ここから先には進まない
    */
    for (i = 0; i < NumClients; i++) {
        // 接続先アドレス情報のサイズ
        len = sizeof(clAddr);
        /*接続するソケットの生成と接続の確立*/
        sock = accept(rsock, (struct sockaddr *)&clAddr, &len);
        if (sock < 0) {
        HandleError("accept()");
        }
        /*ソケットの最大個数を更新*/
        if (maxSock < sock) {
        maxSock = sock;
        }

        /*クライアントからの名前を読み込む*/
        if (read(sock, name[i], MAX_LEN_NAME) == -1) {
        HandleError("read()");
        }
        // 接続
        ClientsNet[i].connect = 1;
        // 使用するソケット
        ClientsNet[i].sock = sock;
        // ソケットの設定
        ClientsNet[i].addr = clAddr;



        //srcの初期化
        memset(src, 0, sizeof(src));
        /*
        * ネットワークアドレス構造体を、そのアドレスを表す文字列に変換する
        * 構造体の内容を IPv4 ネットワークアドレスの ドット区切り 4 分割形式 "ddd.ddd.ddd.ddd" に変換
        *
        */
        inet_ntop(AF_INET, (struct sockaddr *)&clAddr.sin_addr, src, sizeof(src));
        //通信を受け入れたことを知らせる
        fprintf(stderr, "Client %d is accepted (address=%s, port=%d).\n", i, src, ntohs(clAddr.sin_port));

    }
    /*システムモジュールにクライアントの情報を渡す*/
    // ???(id, name);

    //接続リクエストを受け取るソケットを閉じる
    close(rsock);

    /** この段階で設定した人数分のクライアントが接続している **/
    /*変数*/
    int j;
    /*接続したクライアントに情報を送る*/
    for (i = 0; i < NumClients; i++) {
        // 接続しているクライアントの人数を送る
        SendData(i, &NumClients);
        
        for (j = 0; j < NumClients; j++) {
        // 接続しているクライアントの情報を送る
        SendData(i, &ClientsNet[j]);
        }
    }

    /** ファイルディスクリプタの操作 **/
    // select関数の第一引数ので必要
    NumSocks = maxSock + 1;
    // ファイルディスクリプタセットからすべてのファイルディスクリプタを削除。
    FD_ZERO(&Mask);
    // 第一引数のファイルディスクリプタをセットに追加。
    FD_SET(0, &Mask);

    for (i = 0; i < NumClients; i++) {
        // 第一引数のファイルディスクリプタをセットに追加。
        FD_SET(ClientsNet[i].sock, &Mask);
    }

    /** セットアップ終了 **/
    // サーバーのセットアップが完了したことを知らせる
    fprintf(stderr, "Server setup is done.\n");
}

/* サーバーの終了 */
void TerminateServer(void) {
    /*変数*/
    int i;
    /*すべてのソケットを閉じる*/
    for (i = 0; i < NumClients; i++) {
        close(ClientsNet[i].sock);
    }
    // メッセージの表示
    fprintf(stderr, "All connections are closed.\n");
    // 正常終了
    exit(0);
}


/* クライアントからのリクエストに対応する
 * 返値
 *   通信継続: result = 1
 *   通信終了: result = 0
*/
int ControlRequests() {
    /*変数*/
    // ファイルディスクリプタ
    fd_set ReadFlag = Mask;
    //コマンド
    char command;

    /** ソケット通信の多重化 **/
    fprintf(stderr, "select() is started.\n");
    /* ファイルディスクリプタの集合から読み込み可能なファイルディスクリプタを見つける*/
    if (select(NumSocks, (fd_set *)&ReadFlag, NULL, NULL, NULL) == -1) {
        HandleError("select()");
    }

    /** データの受信と送信 **/
    /*変数*/
    int i, result = 1;
    for (i = 0; i < NumClients; i++) {
        /* 第一引数のファイルディスクリプタがセット内にあるかを調べる。[2]*/
        if (FD_ISSET(ClientsNet[i].sock, &ReadFlag)) {
        /*データを受け取る
        * クライアントのID
        * メッセージを送信するか、通信を終了するかのコマンド
        * メッセージ
        */
        ReceiveData(i, &command);
        switch (command) {
        case MOVE_COMMAND:  //移動
            //受け取るデータ
            FloatPosition moveData;
            // dataの初期化
            memset(&moveData, 0, sizeof(FloatPosition));
            //受け取る
            printf("recieve MOVE_COMMAND  id:%d x:%f y:%f z:%f\n", i, moveData.x, moveData.y, moveData.z);
            ReceiveData(i, &moveData);

            // 移動できるなら移動
            MovePosition(i, &moveData);

            // ゲームの継続
            result = 1;
            break;
        case PUT_COMMAND:  //配置
            //受け取るデータ
            PlaceData putData;
            // dataの初期化
            memset(&putData, 0, sizeof(PlaceData));
            //受け取る
            ReceiveData(i, &putData);
            printf("recieve PUT_COMMAND id:%d x:%d y:%d z:%d\n", i, putData.pos.x, putData.pos.y, putData.pos.z);
            /*配置できるかを尋ねる*/

            /*配置できるかを訪ねた後*/
            //配置可能

            //配置不可
            command = DO_NOT_PUT_COMMAND;
            //コマンドの実行可不可を伝える
            SendData(i, &command);
            // 全員に配置情報を送信する
            SendData(BROADCAST, &putData);
            // ゲーム継続
            result = 1;
            break;
        default:
            // コマンドは上記の2種類しか無いので、それ以外の場合はエラーが生じている　
            fprintf(stderr, "ControlRequests(): %c is not a valid command.\n", command);
            command = ERROR_COMMAND;
            //エラーコマンドを伝える
            SendData(i, &command);
        }
        }
    }

    // 値を返す
    return result;
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
 int ReceiveData(int cid, void *data) {
     // データのサイズ
    int size = 0;
    // dataのサイズを取得
    size = sizeof(data);
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= NumClients)) {
        fprintf(stderr, "ReceiveData(): client id is illeagal.\n");
        return -1;
    }
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "ReceiveData(): data is illeagal.\n");
        return -1;
    }
    // ソケットに送られてきたデータをdataに読み込む
    return read(ClientsNet[cid].sock, data, size);
}

/* ソケットにデータを送信
 *
 * 引数
 *    cid:送り先
 *    *data:送られるデータ
 */
int SendData(int cid, void *data) {
    // データのサイズ
    int size = 0;
    // dataのサイズを取得
    size = sizeof(data);
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= NumClients)) {
        fprintf(stderr, "SendData(): client id is illeagal.\n");
        return -1;
    }
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "SendData(): data is illeagal.\n");
        return -1;
    }

    if (cid == BROADCAST) { //全員に送るとき
        int i;
        /*クライアントのソケットに情報を送る*/
        for (i = 0; i < NumClients; i++) {
        // 接続されているクライアントのみに送る
        if(ClientsNet[i].connect == 1){
            if (write(ClientsNet[i].sock, data, size) < 0) {
            HandleError("write()");
            }
        }
        }
    } else { //特定のクライアントに送るとき
        // 接続されているクライアントであるか確認
        if(ClientsNet[cid].connect == 1){
        //特定のソケットに情報を送る
        if (write(ClientsNet[cid].sock, data, size) < 0) {
            HandleError("write()");
        }
        }
    }
    return 0;
}

/* エラーの表示
 *
 * 引数
 *   *message: エラーの発生した段階
 */
static void HandleError(char *message) {
    // エラーメッセージを表示
    perror(message);
    fprintf(stderr, "%d\n", errno);
    // プログラム終了
    exit(1);
}
