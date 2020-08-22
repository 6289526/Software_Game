
/*----------include 開始----------*/
#include "../../header/server_common.h"


/*----------include 終了----------*/


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
        if(Clients[i].connect == 1){
            if (write(Clients[i].sock, data, size) < 0) {
            HandleError("write()");
            }           
        }
        }
    } else { //特定のクライアントに送るとき
        // 接続されているクライアントであるか確認
        if(Clients[cid].connect == 1){
        //特定のソケットに情報を送る
        if (write(Clients[cid].sock, data, size) < 0) {
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
    // // プログラム終了
    // exit(1);
}