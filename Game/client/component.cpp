/**
 * @file component.cpp
 * @brief 
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "component.h"

using namespace React;
Component::Component()
{
}

Component::~Component()
{
}

/**
 * @brief コンポーネントを作成する
 * 
 * @param render 
 * @param surface 
 */
void Component::CreateComponent(SDL_Renderer *render, SDL_Surface *surface)
{
    _texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height);
    _renderer = render;
}

/**
 * @brief コンポーネントを更新する
 * 
 * @param surface 
 */
void Component::UpdateComponent(SDL_Surface *surface)
{
    SDL_DestroyTexture(_texture);
    _texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_QueryTexture(_texture, NULL, NULL, &_width, &_height);
}

/**
 * @brief コンポーネントをレンダラーに貼り付ける
 * 
 * @param positionX 
 * @param positionY 
 * @param scaleX 
 * @param scaleY 
 * @param mode 
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

/**
 * @brief コンポーネントを回転させてレンダラーに貼り付ける
 * 
 * @param positionX 
 * @param positionY 
 * @param angle 
 * @param scaleX 
 * @param scaleY 
 * @param mode 
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

/**
 * @brief コンポーネントを削除する
 * 
 */
void Component::DestroyComponent()
{
    SDL_DestroyTexture(_texture);
}
