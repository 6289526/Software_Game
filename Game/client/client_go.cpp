#include "client_common.h"
#include "go.h"

// スマホからの入力を打ち込む構造体ちなみにextern
SmartInput GoSInput;

// スマホからの入力を受け取る
void Goroutine()
{
// コマンド
#ifdef DEBUG
    char command[30] = "cd chat && ./build/chat";
#else
    char command[30] = "cd ../chat && ./build/chat";
#endif

    FILE *fp;
    char output[60];
    char com[1];
    char tmp[50] = "";
    char tmp2[30];
    

    // 残ってしまったサーバーを消す
    fp = popen("ps -ax | grep build/chat", "r");
    int index = 0;
    int pId = 0 ;
    while (fgets(output, 60, fp) != NULL)
    {
        if(index >= 1){
            sscanf(output, "%s %s", tmp, tmp2);
            
            char kCom[30] = "";
            sprintf(kCom, "kill -9 %s", tmp);
            fp = popen(kCom, "r");
        }
        index++;
    }

    // サーバーを実行
    if ((fp = popen(command, "r")) == NULL)
    {
        /*Failure*/
    }
    
    // スマホからの入力を受け取る
    while (fgets(output, 60, fp) != NULL)
    {
        fprintf(stderr, "out:%s\n", output);
        
        // 毎回もとに戻す
        if (!GoSInput.J)
        {
            GoSInput = (SmartInput){false, false, false, false, false, false, false};
        }
        else
        {
            GoSInput = (SmartInput){true, false, false, false, false, false, false};
        }

        sscanf(output, "%s", com);
        fprintf(stderr, "com:%s", com);
        if (strcmp(com, "J") == 0)// Join　<<--これはスマホがサイトに入ったら
        {
            GoSInput.J = true;
            
        }
        else if (strcmp(com, "F") == 0) //Forward
        {
            GoSInput.F = true;
            
        }
        else if (strcmp(com, "L") == 0) //Left
        {
            GoSInput.L = true;
            
        }
        else if (strcmp(com, "R") == 0) //Right
        {
            GoSInput.R = true;
            
        }
        else if (strcmp(com, "U") == 0) //Up
        {
            GoSInput.U = true;
        }
        else if (strcmp(com, "P") == 0) //Put
        {
            GoSInput.P = true;
        }
        else if (strcmp(com, "E") == 0) //Exit <<--これはスマホがサイトから離れたら
        {
            GoSInput.E = true;
            
        }

    }
    // 終わったこと教える
    fprintf(stderr, "exit\n");
}
