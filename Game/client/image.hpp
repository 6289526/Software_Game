#pragma once

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include "client_common.h"

typedef struct{
    float x;
    float y;
    float w;
    float h;
}FloatRect;

class Image{
    protected:
        GLuint Texture;
        FloatRect src = {0,0,0,0};
        FloatRect dst = {0,0,0,0};
        FloatRect imgRect = {0,0,0,0};
    
    public:
        void LoadImg(const char *filename); //画像ファイル読込
        void LoadText(TTF_Font *font, const char *text, SDL_Color color); //テキストの生成
        void SetDstRect(int x, int y, int w = 0, int h = 0);
        //範囲0~1
        void SetSrcRect(float x = -1, float y = -1, float w = 0, float h = 0);
        //srcRectの範囲0~1
        void Draw(FloatRect *argDst = NULL, FloatRect *argSrc = NULL, float dir = 0); //描画
};