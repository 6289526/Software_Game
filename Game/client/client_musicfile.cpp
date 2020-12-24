#include "client_musicfile.hpp"

using namespace SoundLoader;

// ===== * ===== MusicFileLoader ===== * ===== //
MusicFileLoader::MusicFileLoader(){
    for (int type = 0; type < Sound::SoundEffectTypeNum; type++)
    {
        std::string path = DATA_DIR_PATH + Sound::SoundEffectTypeNames[type];
        _SEPathDictionary.insert(std::make_pair((Sound::SoundEffectType)type, path));
    }
}

MusicFileLoader::~MusicFileLoader(){
    
}
