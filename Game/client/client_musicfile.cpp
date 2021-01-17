/**
 * @file client_musicfile.cpp
 * @brief 
 * 音楽読み込みモジュールの実装
 */

#include "client_musicfile.hpp"

using namespace SoundLoader;

// ===== * ===== MusicFileLoader ===== * ===== //
/**
 * @brief 
 * 音楽ファイルを読み込む
 */
MusicFileLoader::MusicFileLoader(){
    for (int type = 0; type < Sound::SoundEffectTypeNum; type++)
    {
        std::string path = DATA_DIR_PATH;
        path += Sound::SoundEffectTypeNames[type];
        fprintf(stderr, "Music File Loader[%d] : %s\n", type, path.c_str());
        _SEPathDictionary.insert(std::make_pair((Sound::SoundEffectType)type, path));
    }
}
