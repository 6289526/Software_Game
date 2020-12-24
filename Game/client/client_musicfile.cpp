#include "client_musicfile.hpp"

// ===== * ===== MusicFileLoader ===== * ===== //
SoundLoader::MusicFileLoader::MusicFileLoader(){
    for (int type = 0; type < Sound::SoundEffectTypeNum; type++)
    {
        std::string path = DATA_DIR_PATH + Sound::SoundEffectTypeNames[type];
        _SEPathDictionary.emplace(type, path);
    }
}

SoundLoader::MusicFileLoader::~MusicFileLoader(){
    
}
