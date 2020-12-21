#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "image.hpp"

#define MAX_FILE_NAME 128

SDL_Surface CreateMiniMap();

//3桁
class TimeImg{
    private:
    Image nums[10];
    Image str_s;
    public:
        void Set(TTF_Font *font);
        void DrawTime(int time, FloatRect dst);
        void DestroyTexture();
};

class TimerUI{
    private:
        TimeImg timer;
        Image base; //img
        Image rotate; //img
        FloatRect dst = {0,0,0,0};
        FloatRect timeDst = {0,0,0,0};

        char basefile[MAX_FILE_NAME] = "../../data/ui/timer_bace.png";
        char rotatefile[MAX_FILE_NAME] = "../../data/ui/timer_circle.png";

    public:
        void Set(TTF_Font *font);
        void Draw(float dir, float time);
        void Destroy();
};

class PlayerNameUI{
    private:
        Image base; //img
        Image name; //text
        Image ID; //text
        FloatRect baseDst = {0,0,0,0};
        FloatRect nameDst = {0,0,0,0};
        FloatRect idDst = {0,0,0,0};

        char basefile[MAX_FILE_NAME] = "../../data/ui/name_bace.png";

    public:
        void Set(const PlayerData *pData, TTF_Font *font);
        void Draw();
        void Destroy();
};

class DirectionUI{
    private:
        Image base;
        Image rotate;
        Image Playerdir;
        Image meterRect;
        FloatRect dst = {0,0,0,0};


        char basefile[MAX_FILE_NAME] = "../../data/ui/direction_bace.png";
        char rotatefile[MAX_FILE_NAME] = "../../data/ui/direction_circleL.png";
        char dirfile[MAX_FILE_NAME] = "../../data/ui/direction_circleS.png";
        char meterfile[MAX_FILE_NAME] = "../../data/ui/direction_meter.png";

    public:
        void Set();
        void Draw(float playerdir, float rotdir);
        void Destroy();
};

class MiniMap{
    private:
        GLuint Texture;
        FloatRect src = {0,0,0,0};
        FloatRect dst = {0,0,0,0};
        FloatRect imgRect = {0,0,0,0};
    
    public:
        void SetMap(); //画像ファイル読込
        void SetDstRect(int x, int y, int w = 0, int h = 0);
        //範囲0~1
        void SetSrcRect(float x = -1, float y = -1, float w = 0, float h = 0);
        //srcRectの範囲0~1
        void Draw(FloatRect *argDst = NULL, FloatRect *argSrc = NULL, float dir = 0); //描画
        FloatRect GetImgRect(){
            return imgRect;
        };
        void DestroyTexture();
};

class MapUI{
    private:
        Image base;
        MiniMap minimap;
        FloatRect dst = {0,0,0,0};
        char basefile[MAX_FILE_NAME] = "../../data/ui/map_bace.png";

    public:
        void Set();
        void Draw();
        void Destroy();
};

class Gui2D{
    private:
        Image back;
        TimerUI timer;
        PlayerNameUI playerName;
        DirectionUI direction;
        MapUI minimap;
        TTF_Font *font;
        float rotdir = 0.0f;

        FloatRect dst;

        const PlayerData *pData;
        SDL_GLContext context;
        int time = 0;

        char basefile[MAX_FILE_NAME] = "../../data/ui/all_bace.png";
        char fontpath[MAX_FILE_NAME] = "../fonts/BadFennec.ttf";
    public:
        void Set();
        void Draw();
        void Destroy();
};
