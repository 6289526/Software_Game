#pragma once

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

class Image{
    private:
        GLuint Texture;
        int w;
        int h;
    public:
        void LoadImg(const char *filename); //画像ファイル読込
        void LoadText(TTF_Font *font, const char *text, SDL_Color color); //テキストの生成
        void Draw(int x,int y); //描画
        int Get_w(){return w;};
        int Get_h(){return h;};
};