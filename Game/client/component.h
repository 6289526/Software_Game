#pragma once

#include "client_common.h"
#include <vector>

namespace React
{
    // SDL2のテクスチャを使いやすくしたつもりのクラス
    // まず初期化無しで宣言
    // 次にCreateComponent関数でサーフェイスを読み込ませる
    // そしてRenderingComponent関数でレンダラーに描画する
    // また、UpdateComponent関数でサーフェイスとサイズ入れ替えれる
    // 最後にDestroyComponentで削除
    // コツは
    // int componentNum = 0;
    // vector<struct Component> allComponents;
    // って感じで総数とベクターを宣言してコンポーネントを作るたびにpush_backしてひとつにまとめると
    // 最後のDestroyComponentが楽になるよ
    typedef struct Component
    {
        SDL_Renderer *_renderer;
        SDL_Texture *_texture;
        int _width;     // << テクスチャの横幅
        int _height;    // << テクスチャの縦幅
        int _positionX; // << テクスチャの中心座標X
        int _positionY; // << テクスチャの中心座標Y

        Component();
        ~Component();
        void CreateComponent(SDL_Renderer *render, SDL_Surface *surface);

        void UpdateComponent(SDL_Surface *surface);

        void RenderingComponent(int positionX, int positionY, int scaleX, int scaleY, int mode);

        void DestroyComponent();
    } Component;
} // namespace React
