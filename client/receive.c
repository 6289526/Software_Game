
/*----------include 開始----------*/
#include "receive.h"

/*----------include 終了----------*/

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
static CLIENT clients[MAX_NUM_CLIENTS];

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
 int receive_data(int cid, void *data) {
     // データのサイズ
    int size = 0;
    // dataのサイズを取得
    size = sizeof(*data);
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) {
        fprintf(stderr, "receive_data(): client id is illeagal.\n");
        return -1;
    }
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "receive_data(): data is illeagal.\n");
        return -1;
    }
    // ソケットに送られてきたデータをdataに読み込む
    return read(clients[cid].sock, data, size);
}

/* エラーの表示
 *
 * 引数
 *   *message: エラーの発生した段階
 */
static void handle_error(char *message) {
    // エラーメッセージを表示
    perror(message);
    fprintf(stderr, "%d\n", errno);
    
}

