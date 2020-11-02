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

#include "../header/client_common.h"
#include "client_map.hpp"

using namespace std;

#define BLOCK_TYPE_NUM 1

void Disp();
void InitGraphic();

//デバッグ用
void setlookat(int x, int y, int z, double dir);
