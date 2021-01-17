/**
 * @file client_musicfile.hpp
 * @brief 
 * 音楽ファイルの読み込みに関する定義
 */

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
    static const char* SoundEffectTypeNames[] {
        "Jump.wav",
        "Landing.wav",
        "Puts.mp3",
        "Result.wav",
        "Goal.wav",
        "Finish.mp3" 
    };
}

namespace SoundLoader{
    
    /**
     * @brief 
     * 音楽ファイル読み込みモジュール
     */
    class MusicFileLoader{
        private:
            std::map<Sound::SoundEffectType, std::string> _SEPathDictionary; // SEDictionary

        public:
            MusicFileLoader();
            
            /**
             * @brief 
             * SEファイルのパスを返す
             * @return std::map<Sound::SoundEffectType, std::string>& SEファイルのパス
             */
            std::map<Sound::SoundEffectType, std::string>& GetSEPath(){ return _SEPathDictionary; }
    };
}
