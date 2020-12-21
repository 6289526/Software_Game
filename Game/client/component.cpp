#include "component.h"

using namespace React;
Component::Component()
{
}

Component::~Component()
{
}

void Component::CreateComponent(SDL_Renderer *render, SDL_Surface *surface)
{
    _texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height);
    _renderer = render;
}

void Component::UpdateComponent(SDL_Surface *surface)
{
    SDL_DestroyTexture(_texture);
    _texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height);
}

/*
    positionX << 中心座標X
    positionY << 中心座標Y
    scaleX   << 拡縮率X
    scaleY   << 拡縮率Y
    mode     << 1のとき拡大,それ以外縮小
*/
void Component::RenderingComponent(int positionX, int positionY, int scaleX, int scaleY, int mode)
{
    SDL_Rect loadRect = (SDL_Rect){
        0,
        0,
        _width,
        _height};
    SDL_Rect drawRect;
    if (mode == 1)
    { // <<拡大モード
        drawRect = (SDL_Rect){
            positionX - _width * scaleX / 2,
            positionY - _height * scaleY / 2,
            _width * scaleX,
            _height * scaleY};
    }
    else
    { // <<縮小モード
        drawRect = (SDL_Rect){
            positionX - _width / scaleX / 2,
            positionY - _height / scaleY / 2,
            _width / scaleX,
            _height / scaleY};
    }
    SDL_RenderCopy(_renderer, _texture, &loadRect, &drawRect);
}

/*
    positionX << 中心座標X
    positionY << 中心座標Y
    angle << 回転角度
    scaleX   << 拡縮率X
    scaleY   << 拡縮率Y
    mode     << 1のとき拡大,それ以外縮小
*/
void Component::RenderingComponentEx(int positionX, int positionY, double angle, int scaleX, int scaleY, int mode)
{
    SDL_Rect loadRect = (SDL_Rect){
        0,
        0,
        _width,
        _height};
    SDL_Rect drawRect;
    if (mode == 1)
    { // <<拡大モード
        drawRect = (SDL_Rect){
            positionX - _width * scaleX / 2,
            positionY - _height * scaleY / 2,
            _width * scaleX,
            _height * scaleY};
    }
    else
    { // <<縮小モード
        drawRect = (SDL_Rect){
            positionX - _width / scaleX / 2,
            positionY - _height / scaleY / 2,
            _width / scaleX,
            _height / scaleY};
    }
    SDL_RenderCopyEx(_renderer, _texture, &loadRect, &drawRect, angle, NULL, SDL_FLIP_NONE);
}

void Component::DestroyComponent()
{
    SDL_DestroyTexture(_texture);
}
