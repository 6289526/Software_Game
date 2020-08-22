
/*----------include 開始----------*/
#include "../../header/client_common.h"


/*----------include 終了----------*/



/* データの受信
 *
 * 引数
 *    *data:送られるデータ
 * 返り値
 *     
 *    エラーの場合0,-1を返す
 */
 int receive_data(void *data) {
     // データのサイズ
    int size = 0;
    // dataのサイズを取得
    size = sizeof(data);
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "receive_data(): data is illeagal.\n");
        return -1;
    }
    // ソケットに送られてきたデータをdataに読み込む
    return read(sock, data, size);
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

