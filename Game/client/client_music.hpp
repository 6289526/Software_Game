#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "client_gamestate.hpp"

class BGMController : public Observer{
    private:
        const std::string _BGMFilePath = "./../../data/game_maoudamashii_5_village10.ogg";
        Mix_Music* music = NULL;
        int BGMVolume = 30;
        bool initialized = false;

        void Initialize();
    public:
        BGMController();
        ~BGMController();
        virtual void Update(GameState state);
};
