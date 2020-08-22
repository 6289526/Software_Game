
/*----------include 開始----------*/
#include "../../header/client_common.h"


/*----------include 終了----------*/


/*データを送信する
 * 引数
 *    *data: 送信するデータ
 *    size: データのサイズ
 * 
*/
int send_data(void *data) {
    // データのサイズ
    int size = 0;
    // dataのサイズを取得
    size = sizeof(data);
    //データが無いもしくはサイズが負のとき
    if ((data == NULL) || (size <= 0)) {
        fprintf(stderr, "send_data(): data is illeagal.\n");
        return -1;
    }

    /*ソケットにデータを送る*/
    if (write(sock, data, size) == -1) {
        handle_error("write()");
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