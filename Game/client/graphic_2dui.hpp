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
};

class TimerUI{
    private:
        TimeImg timer;
        Image base; //img
        Image rotate; //img
        FloatRect dst = {0,0,0,0};
        FloatRect timeDst = {0,0,0,0};

        char basefile[MAX_FILE_NAME] = "../../data/ui/timer_bace.png";
        char rotatefile[MAX_FILE_NAME] = "../../data/ui/timer_cercle.png";

    public:
        void Set(TTF_Font *font);
        void Draw(float dir, float time);
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
};

class DirectionUI{
    private:
        Image base;
        Image rotate;
        Image Playerdir;
        Image meterRect;
        FloatRect dst = {0,0,0,0};


        char basefile[MAX_FILE_NAME] = "../../data/ui/direction_bace.png";
        char rotatefile[MAX_FILE_NAME] = "../../data/ui/direction_cercleL.png";
        char dirfile[MAX_FILE_NAME] = "../../data/ui/direction_cercleS.png";
        char meterfile[MAX_FILE_NAME] = "../../data/ui/direction_meter.png";

    public:
        void Set();
        void Draw(float playerdir, float rotdir);
};

class MapUI{
    private:
        Image base;
        //minimap
        FloatRect dst = {0,0,0,0};
        char basefile[MAX_FILE_NAME] = "../../data/ui/map_bace.png";

    public:
        void Set();
        void Draw();
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
        float time = 689;

        char basefile[MAX_FILE_NAME] = "../../data/ui/all_bace.png";
        char fontpath[MAX_FILE_NAME] = "../fonts/BadFennec.ttf";
    public:
        void Set();
        void Draw();
};
