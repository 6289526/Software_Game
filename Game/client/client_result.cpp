/*
    キーボードかwiiリモコンかを選択でき,Wiiリモコンのアドレスを取得する
*/
#include "component.h"
#include <cmath>

#ifdef DEBUG
#define FONT_PATH "fonts/PixelMplus12-Regular.ttf"
#else
#define FONT_PATH "../fonts/PixelMplus12-Regular.ttf"
#endif
#define MAX_STRING 128
#define MESSAGE_NUM 1
const int SCREEN_WIDTH = Wd_Width;
const int SCREEN_HEIGHT = Wd_Height;

// フォント
static TTF_Font *font;
// メッセージ
static char Text[MESSAGE_NUM][MAX_STRING] = {
    {"This game's result!"}};

#ifdef DEBUG
static char all_bace2_image[MAX_STRING] = "../data/ui/all_bace2.png";
static char title_logo_image[MAX_STRING] = "../data/ui/title_logo.png";
static char ui_image[MAX_STRING] = "../data/ui/map_bace.png";
static char timer_circle_Image[MAX_STRING] = "../data/ui/timer_circle.png";
#else
static char all_bace2_image[MAX_STRING] = "../../data/ui/all_bace2.png";
static char title_logo_image[MAX_STRING] = "../../data/ui/title_logo.png";
static char ui_image[MAX_STRING] = "../../data/ui/map_bace.png";
static char timer_circle_Image[MAX_STRING] = "../../data/ui/timer_circle.png";

#endif

/*必要な情報を描画*/
int ShowResult()
{
    SDL_Renderer *renderer = GetWindowRenderer();

    //Initialize TTF
    TTF_Init();

    font = TTF_OpenFont(FONT_PATH, 40);

    if (!font)
    {
        printf("TTF_OpenFont: %s %s\n", TTF_GetError(), FONT_PATH);

        exit(1);
    }

    SDL_Event event;

    /*くるくる回るやつの角度*/
    float angle = 0;
    int Num_Clients = GetSystem().Num_Clients;
    // プレイヤーデータ
    const PlayerData *pData = new PlayerData[Num_Clients];
    pData = GetSystem().GetPlayerData();
    // 順位で並べたプレイヤーデータ
    PlayerData playerResult[Num_Clients];

    // 順位で並べ替える
    int rank = 1;
    for (int i = 0; i < Num_Clients; i++)
    {
        for (int j = 0; j < Num_Clients; j++)
        {
            if (rank == pData[j].rank)
            {
                playerResult[i] = pData[i];
                break;
            }
        }
        rank++;
    }

    using namespace React;
    // 描画要素の宣言
    int componentNum = 0;
    vector<Component> allComponents;
    componentNum++;
    Component allBace;
    allBace.CreateComponent(renderer, IMG_Load(all_bace2_image));
    allComponents.push_back(allBace);

    componentNum++;
    Component titleLogo;
    titleLogo.CreateComponent(renderer, IMG_Load(title_logo_image));
    allComponents.push_back(titleLogo);

    componentNum++;
    Component baceUi;
    baceUi.CreateComponent(renderer, IMG_Load(ui_image));
    allComponents.push_back(baceUi);

    componentNum++;
    Component timerCircle;
    timerCircle.CreateComponent(renderer, IMG_Load(timer_circle_Image));
    allComponents.push_back(timerCircle);

    Component messages[MESSAGE_NUM];
    for (int i = 0; i < MESSAGE_NUM; i++)
    {
        componentNum++;
        messages[i].CreateComponent(renderer, TTF_RenderUTF8_Blended(font, Text[i], (SDL_Color){255, 255, 255, 255}));
        allComponents.push_back(messages[i]);
    }

    Component resultMessage[Num_Clients];
    char resultMessageTxt[MAX_STRING];
    for (int i = 0; i < Num_Clients; i++)
    {
        sprintf(resultMessageTxt, "<< %d >> | %s |", playerResult[i].rank, playerResult[i].name);
        componentNum++;
        resultMessage[i].CreateComponent(renderer, TTF_RenderUTF8_Blended(font, resultMessageTxt, (SDL_Color){255, 255, 255, 255}));
        allComponents.push_back(resultMessage[i]);
    }

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);

    allBace.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2, -1);
    titleLogo.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 10 / 30, 3, 3, -1);
    baceUi.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4, 4, -1);

    for (int i = 0; i < MESSAGE_NUM; i++)
    {
        messages[i].RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 1, 1, 1);
    }

    int showRanker = 0;
    for (int i = 0; i < Num_Clients; i++)
    {
        if (showRanker == 3)
        {
            break;
        }
        resultMessage[i].RenderingComponent(
            SCREEN_WIDTH / 2, 
            SCREEN_HEIGHT * 2 / 5 + resultMessage[i]._height * i, 
            1, 
            1, 
            1
            );
        showRanker++;
    }

    //windowにレンダリングする
    SDL_RenderPresent(renderer);
    /*ホット一息*/
    SDL_Delay(10000);

    // すべてのコンポーネントを破棄
    for (int i = 0; i < componentNum; i++)
    {
        allComponents[i].DestroyComponent();
    }

    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
