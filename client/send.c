
/*----------include 開始----------*/
#include "send.h"

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

/* ソケットにデータを送信
 *
 * 引数
 *    cid:送り先
 *    *data:送られるデータ
 */
void send_data(int cid, void *data) {
    // データのサイズ
    int size = 0;
    // dataのサイズを取得
    size = sizeof(*data);
    //全員に送らないかつ、cidが負もしくは最大人数の値を超えているとき
    if ((cid != BROADCAST) && (0 > cid || cid >= num_clients)) {
        fprintf(stderr, "send_data(): client id is illeagal.\n");
        return -1;
    }
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "send_data(): data is illeagal.\n");
        return -1;
    }

    if (cid == BROADCAST) { //全員に送るとき
        int i;
        /*クライアントのソケットに情報を送る*/
        for (i = 0; i < num_clients; i++) {
        // 接続されているクライアントのみに送る
        if(clients[i].connect == 1){
            if (write(clients[i].sock, data, size) < 0) {
            handle_error("write()");
            }           
        }
        }
    } else { //特定のクライアントに送るとき
        // 接続されているクライアントであるか確認
        if(clients[cid].connect == 1){
        //特定のソケットに情報を送る
        if (write(clients[cid].sock, data, size) < 0) {
            handle_error("write()");
        }
        }
    }
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