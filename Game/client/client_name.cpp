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
#define MESSAGE_NUM 2
const int SCREEN_WIDTH = 1237;
const int SCREEN_HEIGHT = 696;

typedef struct
{
    int finish;
    int backspace;
    int a;
} InputData;

//　SDLのキーボード入力で必要
const Uint8 *K;
// フォント
static TTF_Font *font;
//読み取ったデータ
static InputData Data;

// メッセージ
static char Text[MESSAGE_NUM][MAX_STRING] = {
    {"Please set your name."},
    {"Only English!"},
};

char MyName[MAX_LEN_NAME] = "john";
// 何番目の文字か
static int Index = 3;
static int Start = 4;
#ifdef DEBUG
static char ImagePath[2][MAX_STRING] = {
    {"Game.png"},
    {"UI.png"},
};
#else
static char ImagePath[2][MAX_STRING] = {
    {"../Game.png"},
    {"../UI.png"},
};
#endif

static InitData NinitData;

/*プロトタイプ*/
static void InitInput();
static InputData InputEvents(SDL_Event event);
static void SetString();

void GetInitData(InitData initData)
{
    NinitData = initData;
}

/*必要な情報を描画*/
void NameSetUp()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
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

    /*ウィンドウ作ります*/
    window = SDL_CreateWindow("Draw Text Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *image[2];
    SDL_Surface *surface;
    SDL_Texture *texture;

    SDL_Event event;
    //文字を描写したTextureのサイズを取得する
    int iw, ih;
    int showCresol = 0;
    /*描画*/
    image[0] = IMG_Load(ImagePath[0]);
    image[1] = IMG_Load(ImagePath[1]);
    char st = 'a';
    fprintf(stderr, "st:%c\n", st + 1);
    // キーボード
    if (strcmp(WiiAddress, "") == 0)
    {
        SDL_StartTextInput();
        while (data.finish != 1)
        {
            /*キーボード入力取得 kキーでwhileループ抜ける*/
            data = InputEvents(event);

            SetString();

            SDL_RenderClear(renderer);

            texture = SDL_CreateTextureFromSurface(renderer, image[0]);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

            SDL_Rect imgRect = (SDL_Rect){0, 0, iw, ih};
            SDL_Rect impasteRect = (SDL_Rect){0, 0, iw, ih};
            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
            SDL_DestroyTexture(texture);

            texture = SDL_CreateTextureFromSurface(renderer, image[1]);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

            imgRect = (SDL_Rect){0, 0, iw, ih};
            impasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 6, SCREEN_HEIGHT / 2 - ih / 6, iw / 3, ih / 3};

            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
            SDL_DestroyTexture(texture);

            for (int i = 0; i < MESSAGE_NUM; i++)
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

            // 名前の表示
            //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
            surface = TTF_RenderUTF8_Blended(font, MyName, (SDL_Color){255, 255, 255, 255});
            //surfaceからTextureを作る
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);
            // 名前のサイズ
            int nameSize = iw;
            SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
            SDL_Rect pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 2 , iw, ih};

            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            if (showCresol % 50 > 25)
            {
                // 点滅するやつの表示
                //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
                surface = TTF_RenderUTF8_Blended(font, "|", (SDL_Color){255, 255, 255, 255});
                //surfaceからTextureを作る
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

                SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

                SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
                SDL_Rect pasteRect;
                if (Index == -1)
                {
                    pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 2, iw, ih};
                }
                else
                {
                    pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2 + nameSize / 2, SCREEN_HEIGHT / 2, iw, ih};
                }
                //Textureを描写する
                //描写元の描写する部分,描写先の描写する部分)
                //サイズが違うと勝手にTextureを伸展してくれる
                SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
            showCresol++;
            if (showCresol == 50)
            {
                showCresol = 0;
            }

            //windowにレンダリングする
            SDL_RenderPresent(renderer);
            /*ホット一息*/
            SDL_Delay(10);
        }
        SDL_StopTextInput();
    }
    else
    {

        while (NinitData.input->GetInputType().Forward != 1)
        {
            /*キーボード入力取得 kキーでwhileループ抜ける*/
            NinitData.input->UpdateInput();
            SDL_RenderClear(renderer);
            texture = SDL_CreateTextureFromSurface(renderer, image[0]);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

            SDL_Rect imgRect = (SDL_Rect){0, 0, iw, ih};
            SDL_Rect impasteRect = (SDL_Rect){0, 0, iw, ih};
            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
            SDL_DestroyTexture(texture);
            
            texture = SDL_CreateTextureFromSurface(renderer, image[1]);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

            imgRect = (SDL_Rect){0, 0, iw, ih};
            impasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 6, SCREEN_HEIGHT / 2 - ih / 6, iw / 3, ih / 3};

            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &imgRect, &impasteRect);
            SDL_DestroyTexture(texture);

            for (int i = 0; i < MESSAGE_NUM; i++)
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

            // 名前の表示
            //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
            surface = TTF_RenderUTF8_Blended(font, MyName, (SDL_Color){255, 255, 255, 255});
            //surfaceからTextureを作る
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);
            // 名前のサイズ
            int nameSize = iw;
            SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
            SDL_Rect pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 2, iw, ih};

            //Textureを描写する
            //描写元の描写する部分,描写先の描写する部分)
            //サイズが違うと勝手にTextureを伸展してくれる
            SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            if (showCresol % 50 > 25)
            {
                // 点滅するやつの表示
                //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
                surface = TTF_RenderUTF8_Blended(font, "|", (SDL_Color){255, 255, 255, 255});
                //surfaceからTextureを作る
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

                SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

                SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
                SDL_Rect pasteRect;
                if (Index == -1)
                {
                    pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2, SCREEN_HEIGHT / 2, iw, ih};
                }
                else
                {
                    pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - iw / 2 + nameSize / 2, SCREEN_HEIGHT / 2, iw, ih};
                }
                //Textureを描写する
                //描写元の描写する部分,描写先の描写する部分)
                //サイズが違うと勝手にTextureを伸展してくれる
                SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
            showCresol++;
            if (showCresol == 50)
            {
                showCresol = 0;
            }
            // アルファベットの何番目か
            int keynum = 0;
            /*くるくるするやつを描画*/
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    // 点滅するやつの表示
                    //TTF_SetFontOutline(font, 1);//枠抜きで描写するとき
                    if (i == 2 && j == 8)
                    {
                        surface = TTF_RenderUTF8_Blended(font, "Enter", (SDL_Color){255, 255, 255, 255});
                    }
                    else
                    {
                        char text[1];
                        // text += (i*j+1);
                        sprintf(text, "%c", 'a' + keynum);
                        surface = TTF_RenderUTF8_Blended(font, text, (SDL_Color){255, 255, 255, 255});
                    }
                    //surfaceからTextureを作る
                    texture = SDL_CreateTextureFromSurface(renderer, surface);

                    SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

                    SDL_QueryTexture(texture, NULL, NULL, &iw, &ih);

                    SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
                    SDL_Rect pasteRect;
                    pasteRect = (SDL_Rect){SCREEN_WIDTH / 2 - (iw * 4) + iw * 2 * j, SCREEN_HEIGHT * 2 / 3 + ih * i, iw, ih};

                    //Textureを描写する
                    //描写元の描写する部分,描写先の描写する部分)
                    //サイズが違うと勝手にTextureを伸展してくれる
                    SDL_RenderCopy(renderer, texture, &txtRect, &pasteRect);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);

                    keynum++;
                }
            }
            //windowにレンダリングする
            SDL_RenderPresent(renderer);
            /*ホット一息*/
            SDL_Delay(10);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
}

//入力システムの初期化
void InitInput()
{
    K = SDL_GetKeyboardState(NULL);
}

/*キーボードの読み取り*/
InputData InputEvents(SDL_Event event)
{
    if (SDL_PollEvent(&event))
    {
        /**キーボードを使う場合**/
        if (K[SDL_SCANCODE_RETURN])
        {
            Data.finish = 1;
        }
        if (K[SDL_SCANCODE_BACKSPACE])
        {
            if (Data.backspace == 0)
            {
                MyName[Index] = '\0';
                Index--;
                Data.backspace = 1;
            }
        }
        else
        {
            Data.backspace = 0;
        }

        // if (K[SDL_SCANCODE_A])
        // {
        //     if (Data.a == 0)
        //     {
        //         Index++;
        //         MyName[Index] = 'a';
        //         Data.a = 1;
        //     }
        // }
        // else
        // {
        //     Data.a = 0;
        // }
    }
    return Data;
}

void SetString()
{
    // char text[32] = {};
    char texted[32] = {};
    char composition[32] = {};
    int flug = 1;
    SDL_Event ev;

    int count = 0;
    int sflug = 0;

    if (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_TEXTINPUT:
            if (strlen(MyName) >= MAX_LEN_NAME)
                break;
            sflug = 0;
            /* テキストの末尾に新しいテキストを追加する */
            sprintf(MyName, "%s%s", MyName, ev.text.text);
            Index++;
            break;
            // case SDL_TEXTEDITING:
            //     Start = ev.edit.start;
            //     fprintf(stderr, "%d\n", Start);
            //     if (strlen(MyName) >= MAX_LEN_NAME)
            //         break;
            //     sprintf(composition, "%s%s", MyName, ev.edit.text);
            //     sprintf(MyName, composition);
            //     Index++;
            //     break;
        }
    }
}
