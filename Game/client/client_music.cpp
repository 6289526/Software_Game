#include "client_music.hpp"

using namespace Sound;

// ===== * ===== SoundController ===== * ===== //
/**
 * @brief 
 * サウンドモジュールの初期化
 */
void SoundController::Initialize(){
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
        printf( "Mix_OpenAudio could not initialize! Mixer_Error: %s\n", Mix_GetError());
        return;
    }
    fprintf(stderr, "Opened Audio\n");
    _BGMPlayer.Initialize();
    _SEPlayer.Initialize(_SESubject);
}

/**
 * @brief 
 * サウンドモジュールの終了
 */
void SoundController::Finalize(){
    _BGMPlayer.Finalize();
    _SEPlayer.Finalize();
    Mix_CloseAudio();
}

/**
 * @brief 
 * サウンドを鳴らす
 */
void SoundController::Playing(){ 
    Mix_Playing(-1);
}

// ===== * ===== BGMPlayer ===== * ===== //
/**
 * @brief 
 * BGMモジュールの初期化
 */
void BGMPlayer::Initialize(){
    fprintf(stderr, "BGM file path: %s\n", _BGMFilePath.c_str());
}

/**
 * @brief 
 * BGMモジュールの終了
 */
void BGMPlayer::Finalize(){
    Mix_FreeMusic(music);
}

/**
 * @brief 
 * BGMを再生する
 * @param state ゲームの状態
 */
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
/**
 * @brief 
 * SEの購読者を開放
 */
SoundEffectSubject::~SoundEffectSubject(){
    std::list<SoundEffectObserver*>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->SetSubject(NULL);
    }
}

/**
 * @brief 
 * SEを発行する
 * @param soundType SEの種類 
 */
void SoundEffectSubject::OnNest(SoundEffectType soundType){
    std::list<SoundEffectObserver *>::iterator it;
    for (it = _Subscribers.begin(); it != _Subscribers.end(); it++)
    {
        (*it)->OnUpdate(soundType);
    }
}

/**
 * @brief 
 * SEを購読する
 * @param pObserver 購読者
 */
void SoundEffectSubject::Subscribe(SoundEffectObserver *pObserver){
    _Subscribers.push_back(pObserver);
    pObserver->SetSubject(this);
}

// ===== * ===== SoundEffectObserver ===== * ===== //

// ===== * ===== SoundEffectPlayer ===== * ===== //
/**
 * @brief 
 * SEモジュールの初期化
 * @param sESubject SEの購読者
 */
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

/**
 * @brief 
 * SEモジュールの終了
 */
void SoundEffectPlayer::Finalize(){
    for (int i = 0; i < Sound::SoundEffectTypeNum; i++)
    {
        Mix_FreeChunk(_MusicList[i]);
    }
}

/**
 * @brief 
 * SEを再生する
 * @param soundType 再生するSEの種類
 */
void SoundEffectPlayer::OnUpdate(SoundEffectType soundType){
    Mix_VolumeMusic(40);
    if(Mix_PlayChannel(-1, _SEDictionary[soundType], 0) == -1)
        fprintf(stderr,"Error: %s\n", Mix_GetError());
    
    fprintf(stderr, "Played Music No. %d\n", soundType);
}
