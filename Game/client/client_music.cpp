#include "client_music.hpp"

using namespace Sound;

// ===== * ===== BGMPlayer ===== * ===== //
void SoundController::Initialize(){
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
        printf( "Mix_OpenAudio could not initialize! Mixer_Error: %s\n", Mix_GetError());
        return;
    }
}
void SoundController::Finalize(){
    Mix_CloseAudio();
}

// ===== * ===== BGMPlayer ===== * ===== //
void BGMPlayer::Initialize(){
    fprintf(stderr, "BGM file path: %s\n", _BGMFilePath.c_str());
}
void BGMPlayer::Finalize(){
    Mix_FreeMusic(music);
}
void BGMPlayer::Update(GameState::GameState state){
    if(state != GameState::Init)
        return;
            
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

// ===== * ===== SoundEffectPlayer ===== * ===== //
void SoundEffectPlayer::Initialize(){
    for (int i = 0; i < Sound::SoundEffectTypeNum; i++)
    {
        char s[MAX_FILE_PATH_SIZE];
        sprintf(s, Loader.GetSEPath()[(Sound::SoundEffectType)i].c_str());
        fprintf(stderr, "SE file path[%d] : %s\n", i, s);
        _MusicList[i] = Mix_LoadMUS(Loader.GetSEPath()[(Sound::SoundEffectType)i].c_str());
        
        if(_MusicList[i] == NULL){
            fprintf(stderr,"%s のファイルの読み取りに失敗しました。", s);
        }
    }
}
void SoundEffectPlayer::Finalize(){
    for (int i = 0; i < Sound::SoundEffectTypeNum; i++)
    {
        Mix_FreeMusic(_MusicList[i]);
    }
}

void SoundEffectPlayer::OnUpdate(SoundEffectType soundType){ 
    if(Mix_PlayMusic(_SEDictionary[soundType], 0) == -1)
        fprintf(stderr,"Error");
}
