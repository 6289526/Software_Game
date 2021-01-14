#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "client_gamestate.hpp"
#include "client_musicfile.hpp"
#include <map>

namespace Sound {
    class BGMPlayer : public GameState::Observer{
        private:
            const std::string _BGMFilePath = "../../data/game_maoudamashii_5_village10.ogg";
            Mix_Music* music = NULL;
            int BGMVolume = 30;
        public:
            virtual void Update(GameState::GameState state);
            void Initialize();
            void Finalize();
    };

    class SoundEffectObserver;
    class SoundEffectSubject{
    public:
        virtual ~SoundEffectSubject();
        virtual void OnNest(SoundEffectType soundType);
        virtual void Subscribe(SoundEffectObserver *pObserver);
    protected:
        std::list<SoundEffectObserver *> _Subscribers;
    };

    class SoundEffectObserver{
    public:
        virtual void OnUpdate(SoundEffectType soundType) = 0;
        void SetSubject(SoundEffectSubject *pSubject){ _Subject = pSubject; }
    protected:
        SoundEffectSubject *_Subject;
    };

    class SoundEffectPlayer : SoundEffectObserver{
        private:
            std::map<SoundEffectType, Mix_Music*> _SEDictionary; // SEDictionary
            Mix_Music* _MusicList[Sound::SoundEffectTypeNum];

            SoundLoader::MusicFileLoader Loader;
        public:
            inline std::map<SoundEffectType, Mix_Music*> GetSEDictionary() { return _SEDictionary; }
            void OnUpdate(SoundEffectType soundType);
            void Initialize();
            void Finalize();
    };

    class SoundController {
        private:
            BGMPlayer _BGMPlayer;
            SoundEffectPlayer _SEPlayer;
        public:
            void Initialize();
            void Finalize();

            BGMPlayer& GetBGMPlayer(){ return _BGMPlayer; }
            SoundEffectPlayer& GetSoundEffectPlayer(){ return _SEPlayer; }
    };
}
