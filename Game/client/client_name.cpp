/*
    キーボードかwiiリモコンかを選択でき,Wiiリモコンのアドレスを取得する
*/
#include "component.h"

#ifdef DEBUG
#define FONT_PATH "fonts/PixelMplus12-Regular.ttf"
#else
#define FONT_PATH "../fonts/PixelMplus12-Regular.ttf"
#endif
#define MAX_STRING 128
#define MESSAGE_NUM 2
const int SCREEN_WIDTH = Wd_Width;
const int SCREEN_HEIGHT = Wd_Height;

typedef struct
{
    int finish;
    int backspace;
    int a;
} InputData;

typedef struct
{
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool two = false;
    bool one = false;
} WiiInputData;
//　SDLのキーボード入力で必要
const Uint8 *K;
// フォント
static TTF_Font *font;

// メッセージ
static char Text[MESSAGE_NUM][MAX_STRING] = {
    {"Please set your name."},
    {"Only English!"},
};

char MyName[MAX_LEN_NAME] = "john";
// 何番目の文字か
static int Index = 3;
// 何番目のキーボードか
static int KeyboardNum = 0;

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
//読み取った入力データ
static InputData Data;
static WiiInputData WIData;

/*プロトタイプ*/
static void InitInput();
static InputData InputEvents(SDL_Event event);
static void SetString();
static void SetWiiString();

void GetInitData(InitData initData)
{
    NinitData = initData;
}

/*必要な情報を描画*/
void NameSetUp()
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

    SDL_Surface *image[2];
    
    SDL_Event event;
    
    using namespace React;
    // 描画要素の宣言
    int componentNum = 0;
    vector<Component> allComponents;
    componentNum++;
    image[0] = IMG_Load(ImagePath[1]);
    Component subBackGround;
    subBackGround.CreateComponent(renderer, image[0]);
    allComponents.push_back(subBackGround);

    componentNum++;
    image[1] = IMG_Load(ImagePath[0]);
    Component titleBackGround;
    titleBackGround.CreateComponent(renderer, image[1]);
    allComponents.push_back(titleBackGround);

    
    Component messages[MESSAGE_NUM];
    for (int i = 0; i < MESSAGE_NUM; i++)
    {
        componentNum++;
        messages[i].CreateComponent(renderer, TTF_RenderUTF8_Blended(font, Text[i], (SDL_Color){255, 255, 255, 255}));
        allComponents.push_back(messages[i]);
    }

    componentNum++;
    Component inputName;
    inputName.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, MyName, (SDL_Color){255, 255, 255, 255}));
    allComponents.push_back(inputName);
    
    componentNum++;
    Component inputCresol;
    inputCresol.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, "|", (SDL_Color){255, 255, 255, 255}));
    allComponents.push_back(inputCresol);

    int showCresol = 0;
    /*描画*/
    // キーボード
    if (strcmp(WiiAddress, "") == 0 && !GoSInput.J)
    {
        SDL_StartTextInput();
        while (data.finish != 1)
        {
            
            /*キーボード入力取得 kキーでwhileループ抜ける*/
            data = InputEvents(event);
            if (Index <= 8)
            {
                SetString();
            }

            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            titleBackGround.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);

            subBackGround.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4, 4, -1);

            for (int i = 0; i < MESSAGE_NUM; i++)
            {
                messages[i].RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 + i * messages[i]._height + i * messages[i]._height / 2, 1, 1, -1);
            }

            // 名前の表示
            // テクスチャ更新
            inputName.UpdateComponent(TTF_RenderUTF8_Blended(font, MyName, (SDL_Color){255, 255, 255, 255}));
            // 名前のサイズ
            int nameSize = inputName._width;
            inputName.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);

            // 点滅するやつの表示
            if (showCresol % 50 > 25)
            {

                if (Index == -1)
                {
                    inputCresol.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
                }
                else
                {
                    inputCresol.RenderingComponent(SCREEN_WIDTH / 2 + nameSize / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
                }
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
        NinitData.input->_setname = true;
        
        componentNum++;
        Component keyMap;
        keyMap.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, "Enter", (SDL_Color){255, 255, 0, 255}));
        allComponents.push_back(keyMap);

        while (KeyboardNum != 26 || !NinitData.input->GetInputType().Forward)
        {
            //wii入力取得
            NinitData.input->UpdateInput();

            // 入力
            SetWiiString();

            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

            titleBackGround.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);

            subBackGround.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4, 4, -1);

            for (int i = 0; i < MESSAGE_NUM; i++)
            {
                messages[i].RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 + i * messages[i]._height + i * messages[i]._height / 2, 1, 1, -1);
            }

            // 名前の表示
            // テクスチャ更新
            inputName.UpdateComponent(TTF_RenderUTF8_Blended(font, MyName, (SDL_Color){255, 255, 255, 255}));
            // 名前のサイズ
            int nameSize = inputName._width;
            inputName.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);

            // 点滅するやつの表示
            if (showCresol % 50 > 25)
            {

                if (Index == -1)
                {
                    inputCresol.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
                }
                else
                {
                    inputCresol.RenderingComponent(SCREEN_WIDTH / 2 + nameSize / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
                }
            }
            showCresol++;
            if (showCresol == 50)
            {
                showCresol = 0;
            }
            // アルファベットの何番目か
            int keynum = 0;
            // キーボードの入力
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if (i == 2 && j == 8)
                    {
                        if (keynum == KeyboardNum)
                        {
                            keyMap.UpdateComponent(TTF_RenderUTF8_Blended(font, "Enter", (SDL_Color){255, 255, 0, 255}));
                        }
                        else
                        {
                            keyMap.UpdateComponent(TTF_RenderUTF8_Blended(font, "Enter", (SDL_Color){255, 255, 255, 255}));
                        }
                    }
                    else
                    {
                        char text[2];
                        // text += (i*j+1);
                        sprintf(text, "%c", 'a' + keynum);
                        if (keynum == KeyboardNum)
                        {
                            keyMap.UpdateComponent(TTF_RenderUTF8_Blended(font, text, (SDL_Color){255, 255, 0, 255}));
                        }
                        else
                        {
                            keyMap.UpdateComponent(TTF_RenderUTF8_Blended(font, text, (SDL_Color){255, 255, 255, 255}));
                        }
                    }

                    if (i == 2 && j == 8)
                    {
                        keyMap.RenderingComponent(
                            SCREEN_WIDTH / 2 - (keyMap._width * 2) + keyMap._width / 2 * j, 
                            SCREEN_HEIGHT * 2 / 3 + keyMap._height * i, 
                            1, 1, -1);
                    }
                    else
                    {
                        keyMap.RenderingComponent(SCREEN_WIDTH / 2 - (keyMap._width * 9) + keyMap._width * 2 * j,
                        SCREEN_HEIGHT * 2 / 3 + keyMap._height * i, 
                        1, 1, -1);
                    }

                    keynum++;
                }
            }
            //windowにレンダリングする
            SDL_RenderPresent(renderer);
            /*ホット一息*/
            SDL_Delay(10);
        }
    }
    TTF_CloseFont(font);
    TTF_Quit();
    NinitData.input->_setname = false;

    // すべてのコンポーネントを破棄
    for(int i = 0; i < componentNum; i++){
        allComponents[i].DestroyComponent();
    }
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

// Wiiリモコンによる文字の入力
void SetWiiString()
{
    if (WIData.left == false && NinitData.input->GetInputType().Left)
    {
        if (KeyboardNum != 0 && KeyboardNum != 9 && KeyboardNum != 18)
        {
            KeyboardNum--;
        }
        WIData.left = true;
    }
    else if (!NinitData.input->GetInputType().Left)
    {
        WIData.left = false;
    }

    if (WIData.right == false && NinitData.input->GetInputType().Right)
    {
        if (KeyboardNum != 8 && KeyboardNum != 17 && KeyboardNum != 26)
        {
            KeyboardNum++;
        }
        WIData.right = true;
    }
    else if (!NinitData.input->GetInputType().Right)
    {
        WIData.right = false;
    }

    if (WIData.up == false && NinitData.input->GetInputType().Up)
    {
        if (KeyboardNum > 8)
        {
            KeyboardNum -= 9;
        }
        WIData.up = true;
    }
    else if (!NinitData.input->GetInputType().Up)
    {
        WIData.up = false;
    }
    if (WIData.down == false && NinitData.input->GetInputType().Down)
    {
        if (KeyboardNum < 18)
        {
            KeyboardNum += 9;
        }
        WIData.down = true;
    }
    else if (!NinitData.input->GetInputType().Down)
    {
        WIData.down = false;
    }

    if (WIData.two == false && NinitData.input->GetInputType().Forward)
    {
        if (KeyboardNum != 26 && Index <= 8)
        {
            MyName[Index + 1] = 'a' + KeyboardNum;
            Index++;
            WIData.two = true;
        }
    }
    else if (!NinitData.input->GetInputType().Forward)
    {
        WIData.two = false;
    }

    if (WIData.one == false && NinitData.input->GetInputType().Jump)
    {
        MyName[Index] = '\0';
        Index--;
        WIData.one = true;
    }
    else if (!NinitData.input->GetInputType().Jump)
    {
        WIData.one = false;
    }
}
