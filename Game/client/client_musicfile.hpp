#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define MAX_FILE_PATH_SIZE 256

#ifdef DEBUG
    #define DATA_DIR_PATH "../data/"
#else
    #define DATA_DIR_PATH "../../data/"
#endif

namespace Sound{
    enum SoundEffectType{
        Jump,   // ジャンプ
        Landing,// 着地
        Puts,   // ブロック設置
        Result, // リザルト画面
        Goal,   // 自身ゴール
        Finish, // 全員ゴール
        SoundEffectTypeNum,    // 個数取得用
    };
    static const std::string SoundEffectTypeNames[] {
        "Jump.mp3",
        "Landing.mp3",
        "Puts.mp3",
        "Result.mp3",
        "Goal.mp3",
        "Finish.mp3" 
    };
}

namespace SoundLoader{
    class MusicFileLoader{
        private:
            std::map<Sound::SoundEffectType, std::string> _SEPathDictionary; // SEDictionary

        public:
            MusicFileLoader();
            ~MusicFileLoader();
            std::map<Sound::SoundEffectType, std::string>& GetSEPath(){ return _SEPathDictionary; }
    };
}
