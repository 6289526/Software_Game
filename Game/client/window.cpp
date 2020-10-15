#include "client_common.h"

SDL_Window* window;
// SDL_Renderer* renderer;

/*ウィンドウ初期化*/
void InitWindowSys(){
    window = SDL_CreateWindow(
        "test", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        Wd_Width,
        Wd_Height,
        SDL_WINDOW_OPENGL);

    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

/*描画*/
void RendererWindow(){
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
}

/*ウィンドウシステムの終了*/
void TerminateWindowSys(){
    SDL_DestroyWindow(window);
    // SDL_DestroyRenderer(renderer);
}
