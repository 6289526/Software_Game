#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "client_gamestate.hpp"
#include <map>

namespace Sound {
    enum SoundEffectType{
        Jump,   // ジャンプ
        Landing,// 着地
        Puts,   // ブロック設置
        Result, // リザルト画面
        Goal,   // 自身ゴール
        Finish, // 全員ゴール
    };

    class BGMController : public GameState::Observer{
        private:
            const std::string _BGMFilePath = "./../../data/game_maoudamashii_5_village10.ogg";
            Mix_Music* music = NULL;
            int BGMVolume = 30;
            bool initialized = false;

            void Initialize();
        public:
            BGMController();
            ~BGMController();
            virtual void Update(GameState::GameState state);
    };

    class SoundEffectPlayer{
        private:
            std::map<SoundEffectType, Mix_Music*> _SEDictionary; // SEDictionary

            void Initialize();
        public:
            inline std::map<SoundEffectType, Mix_Music*> GetSEDictionary() { return _SEDictionary; }
    };
}
