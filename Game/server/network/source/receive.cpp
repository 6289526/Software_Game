
/*----------include 開始----------*/
#include "../../header/server_common.h"


/*----------include 終了----------*/


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
    return read(Clients[cid].sock, data, size);
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

