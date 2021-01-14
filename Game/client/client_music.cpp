#include "client_music.hpp"

using namespace Sound;

// ===== * ===== SoundController ===== * ===== //
void SoundController::Initialize(){
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
        printf( "Mix_OpenAudio could not initialize! Mixer_Error: %s\n", Mix_GetError());
        return;
    }
    fprintf(stderr, "Opened Audio\n");
    _BGMPlayer.Initialize();
    _SEPlayer.Initialize(_SESubject);
}
void SoundController::Finalize(){
    _BGMPlayer.Finalize();
    _SEPlayer.Finalize();
    Mix_CloseAudio();
}
void SoundController::Playing(){ 
    Mix_Playing(-1);
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
    if(Mix_PlayMusic(music, -1) == -1){
        fprintf(stderr,"BGMの読み込みに失敗しました。\n");
        return;
    }

    fprintf(stderr, "Played BGM\n");
    Mix_VolumeMusic(BGMVolume);
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
void SoundEffectPlayer::Initialize(SoundEffectSubject& sESubject){
    for (int i = 0; i < Sound::SoundEffectTypeNum; i++)
    {
        char s[MAX_FILE_PATH_SIZE];
        sprintf(s, Loader.GetSEPath()[(Sound::SoundEffectType)i].c_str());
        _MusicList[i] = Mix_LoadWAV(Loader.GetSEPath()[(Sound::SoundEffectType)i].c_str());
        _SEDictionary.insert(std::make_pair((Sound::SoundEffectType)i, _MusicList[i]));
        if(_MusicList[i] == NULL){
            fprintf(stderr,"SE file path[%d] - %s - ファイルの読み取りに失敗しました。\n", i, s);
        }
    }

    sESubject.Subscribe(this);
}
void SoundEffectPlayer::Finalize(){
    for (int i = 0; i < Sound::SoundEffectTypeNum; i++)
    {
        Mix_FreeChunk(_MusicList[i]);
    }
}

void SoundEffectPlayer::OnUpdate(SoundEffectType soundType){
    Mix_VolumeMusic(40);
    if(Mix_PlayChannel(-1, _SEDictionary[soundType], 0) == -1)
        fprintf(stderr,"Error: %s\n", Mix_GetError());
    
    fprintf(stderr, "Played Music No. %d\n", soundType);
}
