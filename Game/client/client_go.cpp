#include "client_common.h"
#include "go.h"

// スマホからの入力を打ち込む構造体ちなみにextern
SmartInput GoSInput;

void KillGoServer()
{
    FILE *fp;
    char output[128];
    char tmp[128] = "";
    char tmp2[32] = "";
    // 残ってしまったサーバーを消す
    fp = popen("ps -ax | grep scontrol", "r");
    int index = 0;
    int pId = 0;
    while (fgets(output, 128, fp) != NULL)
    {
        //  標準出力を読み取る
        fprintf(stderr, "out:%s\n", output);
        if (index == 1)
        {
            sscanf(output, "%s %s", tmp2, tmp);

            char kCom[64] = "";
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
    char command[64] = "cd dist && ./build/scontrol";
#else
    char command[64] = "cd ../dist && ./build/scontrol";
#endif

    // 残ってしまったサーバーを消す
    KillGoServer();

    FILE *fp;
    char output[128];


    // サーバーを実行
    if ((fp = popen(command, "r")) == NULL)
    {
        /*Failure*/
    }

    // やって来るコマンドを受け止める
    char com[1];
    char L1[1] = "";
    char R1[1] = "";
    char R2[1] = "";
    char R3[1] = "";
    char R4[1] = "";
    char R5[1] = "";
    char R6[1] = "";
    char Q[1] = "";
    // スマホからの入力を受け取る
    while (fgets(output, 128, fp) != NULL)
    {
        //  標準出力を読み取る
        // fprintf(stderr, "out:%s\n", output);

        if (!GoSInput.J)
        {
            // 毎回もとに戻す
            GoSInput = (SmartInput){false, false, false, false, false, false, false, false, false};
            sscanf(output, "%s", com);
            // fprintf(stderr, "com:%s\n", com);
            if (strcmp(com, "J") == 0) // Join　<<--これはスマホがサイトに入ったら
            {
                GoSInput.J = true;
            }
        }
        else
        {
            // 毎回もとに戻す
            GoSInput = (SmartInput){true, false, false, false, false, false, false, false, false};
            // output[12] = '\0';
            // U は Jump
            // F R L U P Q
            sscanf(output, "%s %s %s %s %s %s %s %s", L1, R1, R2, R3, R4, R5, R6, Q);
            // fprintf(stderr, "F:%s %s %s %s %s %s\n", F, R, L, U, P, E);

            if (strcmp(L1, "tfffffff") >= 0) //Forward
            {
                GoSInput.L1 = true;
            }
            if (strcmp(R1, "tffffff") >= 0) //Left
            {
                GoSInput.R1 = true;
            }
            if (strcmp(R2, "tfffff") >= 0) //Up
            {
                GoSInput.R2 = true;
            }
            if (strcmp(R3, "tffff") >= 0) //Put
            {
                GoSInput.R3 = true;
            }
            if (strcmp(R4, "tfff") >= 0) //Exit <<--これはスマホがサイトから離れたら
            {
                GoSInput.R4 = true;
            }
            if(strcmp(R5, "tff") >= 0) //Exit <<--これはスマホがサイトから離れたら
            {
                GoSInput.R5 = true;
            }
            if(strcmp(R6, "tf") >= 0) //Exit <<--これはスマホがサイトから離れたら
            {
                GoSInput.R6 = true;
            }
            if (strcmp(Q, "t") >= 0)
            {
                GoSInput.Q = true;
            }
            
        }
    }
    // 終わったこと教える
    fprintf(stderr, "exit\n");
}
