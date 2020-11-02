/*
仮実行用
*/
#include "graphic.h"
#include "graphic_test.hpp"
using namespace std;
//仮宣言
ClientMap Map;

bool InitSDL2(){
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	return true;
}


int main(int ArgCount, char **Args)
{

	InitSDL2();
	InitGraphic();
	double dir = 0;
		int x = 0,y = 0,z = 0;

	bool Running = true;
	int maparray[20][20][20] = {0};
	maparray[1][0][1] = 1;
	maparray[2][0][3] = 1;
	maparray[2][1][3] = 1;

	PlaceData object = {0,{10,0,10}};

	Map.SetMapData(20,20,20,maparray);

	Map.PushBackObject(&object);

	while (Running)
	{
		x = 0;
		y = 0;
		z = 0;
		SDL_Event Event;
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_KEYDOWN)
			{
				switch (Event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						Running = false;
					break;
					case SDLK_UP:
						z = 1;
					break;
					case SDLK_DOWN:
						z = -1;
					break;
					case SDLK_RIGHT:
						x = -1;
					break;
					case SDLK_LEFT:
						x = 1;
					break;
					case SDLK_w:
						y = 1;
					break;
					case SDLK_s:
						y = -1;
					break;
					case SDLK_a:
						dir += 0.1;
					break;
					case SDLK_d:
						dir -= 0.1;
					break;
				}
			}
			else if (Event.type == SDL_QUIT)
			{
				Running = false;
			}
		}
		//setlookat(x,y,z,dir);
		SetPlayerPos(0,x,y,z,dir);
		Disp();

	}
	return 0;
}

