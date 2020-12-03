/*
    キーボードかwiiリモコンかを選択でき,Wiiリモコンのアドレスを取得する
*/
#include "client_common.h"

#ifdef DEBUG
#define FONT_PATH "fonts/PixelMplus12-Regular.ttf"
#else
#define FONT_PATH "../fonts/PixelMplus12-Regular.ttf"
#endif
#define MAX_STRING 128
#define MESSAGE_NUM 5
const int SCREEN_WIDTH = Wd_Width;
const int SCREEN_HEIGHT = Wd_Height;

typedef struct
{
    int finish;
} InputData;

//　SDLのキーボード入力で必要
const Uint8 *Key;
// フォント
TTF_Font *font;
//読み取ったデータ
static InputData Data;
// WIIリモコンを見つけられたか
static int GetResult;
// メッセージ
static char Text[MESSAGE_NUM][MAX_STRING] = {
    {"If you want to use Wiilimocon,"},
    {"please push 1 and 2."},
    {"If you want to use Keyboard,"},
    {"please push K Key."},
    {"*"}};

#ifdef DEBUG
static char ImagePath[2][MAX_STRING] = {
    {"UI.png"},
    {"TITLE.png"},
};
#else
static char ImagePath[2][MAX_STRING] = {
    {"../UI.png"},
    {"../TITLE.png"},
};
#endif
/*プロトタイプ*/
static void InitInput();
static InputData InputEvents(SDL_Event event);
static int GetWiiAddress();

/*wiiリモコンのアドレスを取得できる*/
int GetAddress(void *args)
{
    SDL_mutex *mtx = (SDL_mutex *)args;

    SDL_LockMutex(mtx);

    GetResult = GetWiiAddress();

    SDL_UnlockMutex(mtx);

    return 0;
}

/*必要な情報を描画*/
int ControlSetUp()
{
    SDL_Renderer *renderer = GetWindowRenderer();
    // 入力モジュール初期化
    InputData data;

    InitInput();
    //Initialize TTF
    TTF_Init();

    font = TTF_OpenFont(FONT_PATH, 40);

    if (!font)
    {
        printf("TTF_OpenFont: %s %s\n", TTF_GetError(), FONT_PATH);

        exit(1);
    }

    /*プログラムが止まってしまうのでマルチスレッドにします*/
    SDL_Thread *GetAddressThread;
    SDL_mutex *mtx1 = SDL_CreateMutex();
    GetAddressThread = SDL_CreateThread(GetAddress, "Draw", mtx1);

    SDL_Surface *image[2];
    SDL_Surface *surface;
    SDL_Texture *texture;

    SDL_Event event;
    //文字を描写したTextureのサイズを取得する
    int iw, ih;

    /*くるくる回るやつの角度*/
    float angle[3] = {30, 20, 10};
    /*角加速度*/
    float alpha = 0;

    /*描画*/
    image[0] = IMG_Load(ImagePath[0]);
    image[1] = IMG_Load(ImagePath[1]);
    while (data.finish != 1 && GetResult == 0)
    {
        /*キーボード入力取得 kキーでwhileループ抜ける*/
        data = InputEvents(event);

        SDL_RenderClear(renderer);
        texture = SDL_CreateTextureFromSurface(renderer, image[1]);

        SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

        SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

        SDL_Rect imgRect = (SDL_Rect){0, 0, iw, ih};
        SDL_Rect impasteRect = (SDL_Rect){0, 0, iw, ih};
        //Textureを描写する
        //描写元の描写する部分,描写先の描写する部分)
        //サイズが違うと勝手にTextureを伸展してくれる
        SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
        SDL_DestroyTexture(texture);

        texture = SDL_CreateTextureFromSurface(renderer, image[0]);

        SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

        SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

        imgRect = (SDL_Rect){0, 0, iw, ih};
        impasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 8, SCREEN_HEIGHT / 2 - ih / 8, iw / 4, ih / 4};

        //Textureを描写する
        //描写元の描写する部分,描写先の描写する部分)
        //サイズが違うと勝手にTextureを伸展してくれる
        SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
        SDL_DestroyTexture(texture);

        for (int i = 0; i < MESSAGE_NUM - 1; i++)
        {
            //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
            surface = TTF_RenderUTF8_Blended(font, Text[i], (SDL_Color){255, 255, 255, 255});
            //surfaceからTextureを作る
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

            SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
            SDL_Rect pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 5 + i * ih + i * ih / 2, iw, ih};

            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        /*くるくるするやつを描画*/
        for (int i = 0; i < 3; i++)
        {

            //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
            surface = TTF_RenderUTF8_Blended(font, Text[4], (SDL_Color){255, 255, 255, 255});
            //surfaceからTextureを作る
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

            SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
            SDL_Rect pasteRect = (SDL_Rect){(int)(SCREEN_WIDTH / 2 + 100 * sin(angle[i] * M_PI / 180.0)), (int)(SCREEN_HEIGHT * 2 / 3 - 100 * cos(angle[i] * M_PI / 180.0))*0+SCREEN_HEIGHT * 2 / 3, iw, ih};

            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            // index -= 1;
        }

        /*くるくるするやつの角度計算*/
        for (int i = 0; i < 3; i++)
        {

            if (angle[i] >= 360)
            {
                alpha = 0;
                angle[i] = 0.0;
            }
            if (angle[i] > 30 && angle[i] < 330)
            {
                angle[i] += 0.3 + alpha;
            }
            else
            {
                // alpha = 0;
                angle[i] += 1.2 - angle[i] / 360.0;
            }
        }
        alpha += 0.5;

        //windowにレンダリングする
        SDL_RenderPresent(renderer);
        /*ホット一息*/
        SDL_Delay(10);
    }

    /*結果の描画*/
    SDL_RenderClear(renderer);
    SDL_RenderClear(renderer);
    //surfaceからTextureを作る
    texture = SDL_CreateTextureFromSurface(renderer, image[1]);

    SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

    SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

    SDL_Rect imgRect = (SDL_Rect){0, 0, iw, ih};
    SDL_Rect impasteRect = (SDL_Rect){0, 0, iw, ih};
    //Textureを描写する
    //描写元の描写する部分,描写先の描写する部分)
    //サイズが違うと勝手にTextureを伸展してくれる
    SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
    SDL_DestroyTexture(texture);

    texture = SDL_CreateTextureFromSurface(renderer, image[0]);

    SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

    SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

    imgRect = (SDL_Rect){0, 0, iw, ih};
    impasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 8, SCREEN_HEIGHT / 2 - ih / 8, iw / 4, ih / 4};

    //Textureを描写する
    //描写元の描写する部分,描写先の描写する部分)
    //サイズが違うと勝手にTextureを伸展してくれる
    SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
    SDL_DestroyTexture(texture);
    if (GetResult == 1)
    {
        //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
        surface = TTF_RenderUTF8_Blended(font, "Find your Wiilimocon!", (SDL_Color){255, 255, 255, 255});
        fprintf(stderr, "Find your Wiilimocon!\n");
        //surfaceからTextureを作る
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

        SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

        SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
        SDL_Rect pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 2 - ih / 2, iw, ih};

        //Textureを描写する
        //描写元の描写する部分,描写先の描写する部分)
        //サイズが違うと勝手にTextureを伸展してくれる
        SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    else
    {
        //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
        surface = TTF_RenderUTF8_Blended(font, "Select Keyboard", (SDL_Color){255, 255, 255, 255});
        fprintf(stderr, "Select Keyboard!\n");
        //surfaceからTextureを作る
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

        SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

        SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
        SDL_Rect pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 2 - ih / 2, iw, ih};

        //Textureを描写する
        //描写元の描写する部分,描写先の描写する部分)
        //サイズが違うと勝手にTextureを伸展してくれる
        SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    //windowにレンダリングする
    SDL_RenderPresent(renderer);
    /*一秒待つ*/
    SDL_Delay(1000);

    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}

//入力システムの初期化
void InitInput()
{
    Key = SDL_GetKeyboardState(NULL);
}

/*キーボードの読み取り*/
InputData InputEvents(SDL_Event event)
{
    if (SDL_PollEvent(&event))
    {
        /**キーボードを使う場合**/
        if (Key[SDL_SCANCODE_K])
        {
            Data.finish = 1;
        }
    }
    return Data;
}

/*
wiiリモコンのMACアドレスの取得
*/
int GetWiiAddress()
{

    FILE *fp;
    char command[MAX_STRING];
    char output[MAX_STRING];
    sprintf(command, "hcitool scan");

    if ((fp = popen(command, "r")) == NULL)
    {
        /*Failure*/
        return -1;
    }
    int index = 0;
    char address[18];
    char trash1[50];
    char trash2[50];
    while (fgets(output, MAX_STRING, fp) != NULL)
    {
        if (index >= 1)
        {
            fprintf(stderr, "%s\n", output);
            sscanf(output, "%s %s %s", address, trash1, trash2);
            fprintf(stderr, "%s\n", trash1);
            if (strcmp(trash1, "Nintendo") == 0)
            {
                sprintf(WiiAddress, "%s", address);
                fprintf(stderr, "%s\n", WiiAddress);
                return 1;
                // break;
            }
        }
        index++;
    }
    if (pclose(fp) == -1)
    {
        /*Failure*/
        return -1;
    }

    return -1;
    // if (strcmp(WiiAddress, "") == 0)
    // {
    //     return -1;
    // }
    // else
    // {
    //     return 1;
    // }
}
