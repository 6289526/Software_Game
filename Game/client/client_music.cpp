#include "client_music.hpp"

using namespace Sound;

// ===== * ===== BGMController ===== * ===== //
BGMController::BGMController(){ }
BGMController::~BGMController(){
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}
void BGMController::Initialize(){
    fprintf(stderr, "BGM file path: %s\n", _BGMFilePath.c_str());
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

// ===== * ===== SoundEffectSubject ===== * ===== //
SoundEffectSubject::~SoundEffectSubject(){
    std::list<SoundEffectObserver*>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->SetSubject(NULL);
    }
}

void SoundEffectSubject::OnNest(SoundEffectType soundType){
    std::list<SoundEffectObserver *>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->OnUpdate(soundType);
    }
}

void SoundEffectSubject::Subscribe(SoundEffectObserver *pObserver){
    _Subscribers.push_back(pObserver);
    pObserver->SetSubject(this);
}

// ===== * ===== SoundEffectObserver ===== * ===== //
SoundEffectObserver::~SoundEffectObserver(){ 

}

// ===== * ===== SoundEffectPlayer ===== * ===== //
void SoundEffectPlayer::Initialize(){
    for (int i = 0; i < Sound::SoundEffectTypeNum; i++)
    {
        fprintf(stderr, "SE file path[%d] : %s", i, Loader.GetSEPath()[(Sound::SoundEffectType)i]);
        _MusicList[i] = Mix_LoadMUS(Loader.GetSEPath()[(Sound::SoundEffectType)i].c_str());
    }
}

void SoundEffectPlayer::OnUpdate(SoundEffectType soundType){ 
    if(Mix_PlayMusic(_SEDictionary[soundType], 0) == -1)
        fprintf(stderr,"Error");
}
