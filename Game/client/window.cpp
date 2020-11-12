#include "client_common.h"

#define WIDTH_MY_WINDOW 1000 //ウィンドウの横幅
#define HEIGHT_MY_WINDOW 640 //ウィンドウの縦幅
#define FONT_PATH "PixelMplus12-Regular.ttf"
typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;
typedef struct
{
    float x;
    float y;
    float z;
    float tox;
    float toy;
    float toz;
} LookAt;

int table[100];
int object_num;
static SDL_Renderer *renderer;
static SDL_Window *window;
TTF_Font *font;
SDL_Surface *box_surf;
GLuint box1[6] = {0};
LookAt lookAt;
SDL_Color black = {0x00, 0x00, 0x00, 0xFF}; // フォントの色を指定するための構造体（白色）

void InitWindowSys(int *argc, char *argsv[])
{
    glutInit(argc, argsv);

    window = SDL_CreateWindow(
        "Test",                  // window title
        SDL_WINDOWPOS_UNDEFINED, // 初めの x position
        SDL_WINDOWPOS_UNDEFINED, // 初めの y position
        WIDTH_MY_WINDOW,         // width, in pixels
        HEIGHT_MY_WINDOW,        // height, in pixels
        SDL_WINDOW_OPENGL        // OPENGLをつかう?
    );

    assert(window);
    //コンテキスト宣言
    SDL_GLContext Context = SDL_GL_CreateContext(window);

    //レンダラー作成
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //箱の面の画像の読み込みとテクスチャ化

    box_surf = IMG_Load("1.png");
    if (!box_surf)
    {
        fprintf(stderr, "cannnot load img");
    }

    for (int i = 0; i < 6; i++)
    {
        glGenTextures(1, &box1[i]);
        glBindTexture(GL_TEXTURE_2D, box1[i]);

        int Mode = GL_RGB;

        if (box_surf->format->BytesPerPixel == 4)
        {
            Mode = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, Mode, box_surf->w, box_surf->h, 0, Mode, GL_UNSIGNED_BYTE, box_surf->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    TTF_Init(); // TrueTypeフォントを用いるための初期化
    font = TTF_OpenFont(FONT_PATH, 12);
    if (!font)
    { //読み込みに失敗
        printf("cannot open font!");
        
    }
    // 出力デバイスの違いに対応
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // 拡大縮小を滑らかにする（線形フィルタで）
    SDL_RenderSetLogicalSize(renderer, 640, 480);         // デバイスに依存しない分解能を設定
}

void RendererWindow()
{
    const PlayerData *pData = GetPlayerData();
    float mode = pData[0].pos.x;
    float index = pData[0].pos.z;

    lookAt.x = WIDTH_MY_WINDOW / 2;
    lookAt.y = -HEIGHT_MY_WINDOW * 3;
    lookAt.z = -1000 + index * 50;
    lookAt.tox = 0 * cos(mode / 180 * M_PI) - (1000 + index * 50 - (-1000 + index * 50)) * sin(mode / 180 * M_PI) + WIDTH_MY_WINDOW / 2;
    lookAt.toy = HEIGHT_MY_WINDOW / 2;
    lookAt.toz = (0) * sin(mode / 180 * M_PI) + (1000 + index * 50 - (-1000 + index * 50)) * cos(mode / 180 * M_PI) + (-1000 + index * 50);

    lookAt.x += (lookAt.tox - lookAt.x) / 10;
    lookAt.tox += (lookAt.tox - lookAt.x) / 10;
    lookAt.z += (lookAt.toz - lookAt.z) / 10;
    lookAt.toz += (lookAt.toz - lookAt.z) / 10;
    //レンダラーの塗りつぶし
    SDL_RenderClear(renderer);
    char string[100] = "";
    //サーフェイスとテクスチャ
    SDL_Surface *strings;
    SDL_Texture *texture;
    //描画部分と描画場所
    SDL_Rect object_rect;
    SDL_Rect draw_rect;
    //テクスチャの本来の横幅と高さ
    int w, h;
    //文字列の描画
    snprintf(string, 100, "Button%d", 1);
    // 描画文字の作成と格納
    strings = TTF_RenderUTF8_Blended(font, string, black);
    texture = SDL_CreateTextureFromSurface(renderer, strings);
    //テクスチャから横幅と大きさを取得
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    //描画部分と描画場所の決定
    object_rect = (SDL_Rect){0, 0, w, h};
    draw_rect = (SDL_Rect){100 + w * 9 / 16 + w * 9 / 4, 120, w, h};

    SDL_RenderCopy(renderer, texture, &object_rect, &draw_rect);

    //テクスチャとサーフェイスの解放
    //これをしないと落ちる
    SDL_FreeSurface(strings);
    SDL_DestroyTexture(texture);

    //テクスチャの本来の横幅と高さ
    w = box_surf->w, h = box_surf->h;
    // actions(box_surf, draw_rect, index, mode);

    // 底面
    //描画場所の決定
    draw_rect = (SDL_Rect){WIDTH_MY_WINDOW / 2 - w / 2, HEIGHT_MY_WINDOW / 2 - h / 2, w, h * 50};
    int x = draw_rect.x;
    int y = draw_rect.y;
    int z = draw_rect.h;
    //openglによる描画
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, box1[4]);
    glBegin(GL_QUADS);
    // glTexCoord2f(0, 0); glVertex3f(x*cos(mode/180*M_PI)+index*50*sin(mode/180*M_PI)+WIDTH_MY_WINDOW/2, y, x*sin(mode/180*M_PI)+index*-50*cos(mode/180*M_PI)+index*50);
    // glTexCoord2f(1, 0); glVertex3f((x+draw_rect.w)*cos(mode/180*M_PI)+index*50*sin(mode/180*M_PI)+WIDTH_MY_WINDOW/2,y, (x+draw_rect.w)*sin(mode/180*M_PI)+index*-50*cos(mode/180*M_PI)+index*50);
    // glTexCoord2f(1, 1); glVertex3f((x+draw_rect.w)*cos(mode/180*M_PI)-z*sin(mode/180*M_PI)+WIDTH_MY_WINDOW/2, y, (x+draw_rect.w)*sin(mode/180*M_PI)+z*cos(mode/180*M_PI)+index*50);
    // glTexCoord2f(0, 1); glVertex3f(x*cos(mode/180*M_PI)-z*sin(mode/180*M_PI)+WIDTH_MY_WINDOW/2, y, x*sin(mode/180*M_PI)+z*cos(mode/180*M_PI)+index*50);
    glTexCoord2f(0, 0);
    glVertex3f(x, y, 0);
    glTexCoord2f(1, 0);
    glVertex3f((x + draw_rect.w), y, 0);
    glTexCoord2f(1, 1);
    glVertex3f((x + draw_rect.w), y, z);
    glTexCoord2f(0, 1);
    glVertex3f(x, y, z);
    glEnd();
    // カート
    //描画場所の決定
    draw_rect = (SDL_Rect){WIDTH_MY_WINDOW / 2 - w / 10, HEIGHT_MY_WINDOW / 2 - h / 10, w / 5, h / 5};
    //x = draw_rect.x - WIDTH_MY_WINDOW/2-index*50*sin(mode/180*M_PI); z = draw_rect.h-index*50*cos(mode/180*M_PI);
    //openglによる描画
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, box1[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(draw_rect.x + (lookAt.tox - lookAt.x) / 10, draw_rect.y - draw_rect.h, index * 50 + (lookAt.toz - lookAt.z) / 10);
    glTexCoord2f(1, 0);
    glVertex3f(draw_rect.w + draw_rect.x + (lookAt.tox - lookAt.x) / 10, draw_rect.y - draw_rect.h, index * 50 + (lookAt.toz - lookAt.z) / 10);
    glTexCoord2f(1, 1);
    glVertex3f(draw_rect.w + draw_rect.x + (lookAt.tox - lookAt.x) / 10, draw_rect.y - draw_rect.h, draw_rect.h + index * 50 + (lookAt.toz - lookAt.z) / 10);
    glTexCoord2f(0, 1);
    glVertex3f(draw_rect.x + (lookAt.tox - lookAt.x) / 10, draw_rect.y - draw_rect.h, draw_rect.h + index * 50 + (lookAt.toz - lookAt.z) / 10);
    glEnd();
    //openglによる描画
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, box1[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(draw_rect.x, draw_rect.y - draw_rect.h, index * 50);
    glTexCoord2f(1, 0);
    glVertex3f(draw_rect.x + draw_rect.w, draw_rect.y - draw_rect.h, index * 50);
    glTexCoord2f(1, 1);
    glVertex3f(draw_rect.x + draw_rect.w, draw_rect.y, index * 50);
    glTexCoord2f(0, 1);
    glVertex3f(draw_rect.x, draw_rect.y, index * 50);
    glEnd();
    for (int i = 0; i < object_num; i++)
    {
        //描画場所の決定
        draw_rect = (SDL_Rect){WIDTH_MY_WINDOW / 2 - w / 2, HEIGHT_MY_WINDOW / 2 - h / 2, w, h};
        //openglによる描画
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, box1[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(draw_rect.x, draw_rect.y, table[i]);
        glTexCoord2f(1, 0);
        glVertex3f(draw_rect.x + draw_rect.w, draw_rect.y, table[i]);
        glTexCoord2f(1, 1);
        glVertex3f(draw_rect.x + draw_rect.w, draw_rect.y - draw_rect.h, h + table[i]);
        glTexCoord2f(0, 1);
        glVertex3f(draw_rect.x, draw_rect.y - draw_rect.h, h + table[i]);
        glEnd();
    }

    // 視点
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(lookAt.x, lookAt.y, lookAt.z, lookAt.tox, lookAt.toy, lookAt.toz, 0, -1.0, 0);
    // gluLookAt(WIDTH_MY_WINDOW/2, -HEIGHT_MY_WINDOW*2,-10000+index*50
    // ,WIDTH_MY_WINDOW/2,HEIGHT_MY_WINDOW/2,1000+index*50
    // , 0,0,1.0);
    // gluLookAt(WIDTH_MY_WINDOW/2, -5000, -HEIGHT_MY_WINDOW*2
    // ,WIDTH_MY_WINDOW/2,1000, HEIGHT_MY_WINDOW/2
    // , 0,0,-1.0);
    // 視野
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, WIDTH_MY_WINDOW / HEIGHT_MY_WINDOW, 0, 50);

    //文字列の描画
    snprintf(string, 100, "YEEAAA");
    // 描画文字の作成と格納
    strings = TTF_RenderUTF8_Blended(font, string, black);
    texture = SDL_CreateTextureFromSurface(renderer, strings);
    //テクスチャから横幅と大きさを取得
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    w *= 100;
    h *= 100;
    //描画部分と描画場所の決定
    object_rect = (SDL_Rect){0, 0, w, h};
    draw_rect = (SDL_Rect){WIDTH_MY_WINDOW / 2 - w / 2, HEIGHT_MY_WINDOW / 2 - h / 2, w, h};

    SDL_RenderCopy(renderer, texture, &object_rect, &draw_rect);
    int angle = index * 60;
    if (angle > 360)
    {
        angle -= 360;
    }
    else if (angle < -360)
    {
        angle += 360;
    }
    // SDL_Point point = {0, 0};
    // SDL_RenderCopyEx(renderer, texture, &object_rect, &draw_rect, angle, &point, SDL_FLIP_NONE);

    //テクスチャとサーフェイスの解放
    //これをしないと落ちる
    SDL_FreeSurface(strings);
    SDL_DestroyTexture(texture);
    // 塗りつぶしの色を白色に戻す
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //ウィンドウに描く
    SDL_RenderPresent(renderer);
}

void TerminateWindowSys()
{
    //サーフェイスやテクスチャ等の削除
    SDL_FreeSurface(box_surf);
    //SDL_DestroyTexture(image_tex);

    if (renderer)
        SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
}
