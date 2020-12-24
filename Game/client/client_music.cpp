#include "client_music.hpp"

using namespace Sound;

// ===== * ===== BGMController ===== * ===== //
BGMController::BGMController(){ }
BGMController::~BGMController(){ }
void BGMController::Initialize(){
    fprintf(stderr, "Path: %s\n", _BGMFilePath.c_str());
}
void BGMController::Finalize(){
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}
void BGMController::Update(GameState::GameState state){
    if(state != GameState::Init)
        return;
        
    if(!initialized) {
        if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
            printf( "Mix_OpenAudio could not initialize! Mixer_Error: %s\n", Mix_GetError());
            return;
        }
        initialized = true;
    }
    
    music = Mix_LoadMUS(_BGMFilePath.c_str());
    if(Mix_PlayMusic(music, -1) == -1)
        return;

    Mix_VolumeMusic(BGMVolume);
    Mix_PlayingMusic();
}
