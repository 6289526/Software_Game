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
        "Jump",
        "Landing",
        "Puts",
        "Result",
        "Goal",
        "Finish" 
    };
}

namespace SoundLoader{
    class MusicFileLoader{
        private:
            std::map<Sound::SoundEffectType, std::string> _SEPathDictionary; // SEDictionary

            MusicFileLoader();
            ~MusicFileLoader();

        public:
            std::map<Sound::SoundEffectType, std::string>& GetSEPath(){ return _SEPathDictionary; }
    };
}
