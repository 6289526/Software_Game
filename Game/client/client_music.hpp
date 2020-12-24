#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "client_gamestate.hpp"
#include "client_musicfile.hpp"
#include <map>

namespace Sound {
    class BGMController : public GameState::Observer{
        private:
            const std::string _BGMFilePath = "./../../data/game_maoudamashii_5_village10.ogg";
            Mix_Music* music = NULL;
            int BGMVolume = 30;
            bool initialized = false;

            void Initialize();
        public:
            BGMController();
            ~BGMController();
            virtual void Update(GameState::GameState state);
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
        virtual ~SoundEffectObserver();
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
            void Initialize();
            
        public:
            inline std::map<SoundEffectType, Mix_Music*> GetSEDictionary() { return _SEDictionary; }
            ~SoundEffectPlayer();
            void OnUpdate(SoundEffectType soundType);
    };
}
