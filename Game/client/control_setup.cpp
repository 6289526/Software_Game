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
#define MESSAGE_NUM 3
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
    {"If you want to use Wiilimocon please push 1 and 2."},
    {"If you want to use Keyboard please push K Key."},
    {"."}};

#ifdef DEBUG
static char all_bace2_image[MAX_STRING] = "../data/ui/all_bace2.png";
static char title_logo_image[MAX_STRING] = "../data/ui/title_logo.png";
static char ui_image[MAX_STRING] = "../data/ui/UI.png";
static char timer_circle_Image[MAX_STRING] = "../data/ui/timer_circle.png";
#else
static char all_bace2_image[MAX_STRING] = "../../data/ui/all_bace2.png";
static char title_logo_image[MAX_STRING] = "../../data/ui/title_logo.png";
static char ui_image[MAX_STRING] = "../../data/ui/UI.png";
static char timer_circle_Image[MAX_STRING] = "../../data/ui/timer_circle.png";

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

char *GetIpAddress()
{
    FILE *fp;
    char command[MAX_STRING];
    char output[MAX_STRING];
    sprintf(command, "hostname -I| cut -d' ' -f1");

    fp = popen(command, "r");
    int index = 0;
    char *address = (char *)malloc(32);
    char trash1[MAX_STRING];
    char trash2[MAX_STRING];
    while (fgets(output, MAX_STRING, fp) != NULL)
    {
        fprintf(stderr, "%s\n", output);
        sscanf(output, "%s", address);
    }

    return address;
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

    SDL_Event event;

    /*くるくる回るやつの角度*/
    float angle = 0;

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

    char phoneMsg[64];
    sprintf(phoneMsg, "If you want to use tablet or phone,");
    componentNum++;
    Component phoneMessage;
    phoneMessage.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, phoneMsg, (SDL_Color){255, 255, 255, 255}));
    allComponents.push_back(phoneMessage);

    char urlMsg[64];
    char *ipAddress = (char *)malloc(32);
    sprintf(urlMsg, "access http://%s:8080.", ipAddress = GetIpAddress());
    componentNum++;
    Component GoServerAddress;
    GoServerAddress.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, urlMsg, (SDL_Color){255, 255, 255, 255}));
    allComponents.push_back(GoServerAddress);

    /*描画*/
    while (data.finish != 1 && !GoSInput.J && !(strcmp(WiiAddress, "")))
    {
        /*キーボード入力取得 kキーでwhileループ抜ける*/
        data = InputEvents(event);

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);

        allBace.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2, -1);
        titleLogo.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 10 / 30, 3, 3, -1);

        for (int i = 0; i < MESSAGE_NUM - 1; i++)
        {
            messages[i].RenderingComponent(
                SCREEN_WIDTH / 2,
                SCREEN_HEIGHT * 6 / 9 + i * messages[i]._height + i * messages[i]._height / 2,
                1,
                1,
                -1);
        }

        phoneMessage.RenderingComponent(
            SCREEN_WIDTH / 2,
            SCREEN_HEIGHT * 6 / 9 + (MESSAGE_NUM - 1) * phoneMessage._height + (MESSAGE_NUM - 1) * phoneMessage._height / 2,
            1,
            1,
            -1);

        GoServerAddress.RenderingComponent(
            SCREEN_WIDTH / 2,
            SCREEN_HEIGHT * 6 / 9 + MESSAGE_NUM * GoServerAddress._height + MESSAGE_NUM * GoServerAddress._height / 2,
            1,
            1,
            -1);

        /*くるくるするやつを描画*/
        timerCircle.RenderingComponentEx(
            SCREEN_WIDTH / 2 + GoServerAddress._width * 2 / 3,
            SCREEN_HEIGHT * 6 / 9 + MESSAGE_NUM * GoServerAddress._height + MESSAGE_NUM * GoServerAddress._height / 2,
            angle,
            10,
            10,
            -1);

        angle += 1;
        if (angle > 360)
        {
            angle = 0;
        }

        //windowにレンダリングする
        SDL_RenderPresent(renderer);
        /*ホット一息*/
        SDL_Delay(10);
    }

    /*結果の描画*/
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 85, 150, 255);

    allBace.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 2, -1);
    titleLogo.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 10 / 30, 3, 3, -1);

    baceUi.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, 3, -1);

    if (GetResult == 1)
    {
        struct Component findWiiMessage;
        componentNum++;
        findWiiMessage.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, "Find your Wiilimocon!", (SDL_Color){255, 255, 255, 255}));
        fprintf(stderr, "Find your Wiilimocon!\n");
        allComponents.push_back(findWiiMessage);
        findWiiMessage.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
    }
    else if (GoSInput.J)
    {
        struct Component selectSmartMessage;
        componentNum++;
        selectSmartMessage.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, "Select Phone", (SDL_Color){255, 255, 255, 255}));
        fprintf(stderr, "Select Browser!\n");
        allComponents.push_back(selectSmartMessage);
        selectSmartMessage.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
    }
    else
    {
        struct Component selectKeyboardMessage;
        componentNum++;
        selectKeyboardMessage.CreateComponent(renderer, TTF_RenderUTF8_Blended(font, "Select Keyboard", (SDL_Color){255, 255, 255, 255}));
        fprintf(stderr, "Select Keyboard!\n");
        allComponents.push_back(selectKeyboardMessage);
        selectKeyboardMessage.RenderingComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, -1);
    }
    //windowにレンダリングする
    SDL_RenderPresent(renderer);
    /*一秒待つ*/
    SDL_Delay(1000);

    // すべてのコンポーネントを破棄
    for (int i = 0; i < componentNum; i++)
    {
        allComponents[i].DestroyComponent();
    }

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
    char address[32];
    char trash1[MAX_STRING];
    char trash2[MAX_STRING];
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
