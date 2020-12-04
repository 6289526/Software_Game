#include "client_common.h"
#include "go.h"
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
    char tmp[59];
    // sprintf(command, "hcitool scan");

    if ((fp = popen(command, "r")) == NULL)
    {
        /*Failure*/
    }
    int index = 0;
    // fgets(output, 60, fp);
    // fprintf(stderr, "out:%s\n", output);
    while (fgets(output, 60, fp) != NULL)
    {
        fprintf(stderr, "out:%s\n", output);
        if (!GoSInput.J)
        {
            GoSInput = (SmartInput){false, false, false, false, false, false, false};
        }
        else
        {
            GoSInput = (SmartInput){true, false, false, false, false, false, false};
        }
        // if (index >= 1)
        // {
        sscanf(output, "%s", com);
        fprintf(stderr, "com:%s", com);
        if (strcmp(com, "J") == 0)
        {
            GoSInput.J = true;
            // fprintf(stderr, "j:%s %d\n", com, GoSInput.J);
        }
        else if (strcmp(com, "F") == 0)
        {
            GoSInput.F = true;
            // fprintf(stderr, "f:%s\n", com);
        }
        else if (strcmp(com, "L") == 0)
        {
            GoSInput.L = true;
            // fprintf(stderr, "l:%s\n", com);
        }
        else if (strcmp(com, "R") == 0)
        {
            GoSInput.R = true;
            // fprintf(stderr, "r:%s\n", com);
        }
        else if (strcmp(com, "U") == 0){
            GoSInput.U = true;
        }
        else if(strcmp(com, "P") == 0){
            GoSInput.P = true;
        }
        else if (strcmp(com, "E") == 0)
        {
            GoSInput.E = true;
            // fprintf(stderr, "e:%s\n", com);
        }
        
        // fprintf(stderr, "%d", GoSInput.J);
        // }
        // if (index == 0)
        // {
        //     index++;
        // }
    }
    fprintf(stderr, "exit\n");
}
