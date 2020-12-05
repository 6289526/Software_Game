#include "client_common.h"
#include "go.h"

// スマホからの入力を打ち込む構造体ちなみにextern
SmartInput GoSInput;

void KillGoServer()
{
    FILE *fp;
    char output[60];
    char tmp[64] = "";
    char tmp2[32] = "";
    // 残ってしまったサーバーを消す
    fp = popen("ps -ax | grep build/scontrol", "r");
    int index = 0;
    int pId = 0;
    while (fgets(output, 64, fp) != NULL)
    {
        //  標準出力を読み取る
        fprintf(stderr, "out:%s\n", output);
        if (index >= 1)
        {
            sscanf(output, "%s %s", tmp2, tmp);

            char kCom[32] = "";
            sprintf(kCom, "kill -9 %s", tmp2);
            fp = popen(kCom, "r");
        }
        index++;
    }
}

SmartInput GetGosInput()
{
    return GoSInput;
}

// スマホからの入力を受け取る
void Goroutine()
{
// コマンド
#ifdef DEBUG
    char command[30] = "cd dist && ./build/scontrol";
#else
    char command[30] = "cd ../chat && ./build/scontrol";
#endif

    // 残ってしまったサーバーを消す
    KillGoServer();

    FILE *fp;
    char output[60];
    

    // サーバーを実行
    if ((fp = popen(command, "r")) == NULL)
    {
        /*Failure*/
    }

    // やって来るコマンドを受け止める
    char com[1];
    char F[1] = "";
    char R[1] = "";
    char L[1] = "";
    char U[1] = "";
    char P[1] = "";
    char E[1] = "";
    // スマホからの入力を受け取る
    while (fgets(output, 60, fp) != NULL)
    {
        //  標準出力を読み取る
        fprintf(stderr, "out:%s\n", output);

        if (!GoSInput.J)
        {
            // 毎回もとに戻す
            GoSInput = (SmartInput){false, false, false, false, false, false, false};
            sscanf(output, "%s", com);
            fprintf(stderr, "com:%s\n", com);
            if (strcmp(com, "J") == 0) // Join　<<--これはスマホがサイトに入ったら
            {
                GoSInput.J = true;
            }
        }
        else
        {
            // 毎回もとに戻す
            GoSInput = (SmartInput){true, false, false, false, false, false, false};
            // output[12] = '\0';
            // U は Jump
            // F R L U P Q
            sscanf(output, "%s %s %s %s %s %s", F, R, L, U, P, E);
            fprintf(stderr, "F:%s\n", F);

            if (strcmp(F, "t") >= 0) //Forward
            {
                GoSInput.F = true;
            }
            if (strcmp(R, "ft") >= 0) //Right
            {
                GoSInput.R = true;
            }
            if (strcmp(L, "fft") >= 0) //Left
            {
                GoSInput.L = true;
            }
            if (strcmp(U, "ffft") >= 0) //Up
            {
                GoSInput.U = true;
            }
            if (strcmp(P, "fffft") >= 0) //Put
            {
                GoSInput.P = true;
            }
            if (strcmp(E, "ffffft") >= 0) //Exit <<--これはスマホがサイトから離れたら
            {
                GoSInput.E = true;
            }
        }
    }
    // 終わったこと教える
    fprintf(stderr, "exit\n");
}
