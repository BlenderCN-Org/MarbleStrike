#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomusiclist.hpp"
#include "common/engine/audio/audiosoundbank.hpp"
#include "common/engine/system/fixedstring.hpp"
#include <vector>

#if defined PLAT_IOS
#include "ios/audio/iosaudiostreamsource.h"
#endif

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
    class AudioStaticSource;
    class AudioStreamSource;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
    class AudioManager
    {
    public:	

        static AudioManager* Create();
        static void Destroy();

        AudioManager();
        virtual ~AudioManager();        

        virtual void Initialize() = 0;
        virtual void Shutdown();
        void Update();

        //
        // Music
        //
        void SetMusicList(const MusicList& musicList);
        void ClearMusicList();

        void PlayMusic();
        bool PlayMusic(const char* filename);
        void StopMusic();
        void PauseMusic();
        void UnpauseMusic();

        bool IsMusicPaused();
        bool IsMusicPlaying();

        void NextMusic();
        void PrevMusic();
        void RewindMusic();

        void SetMusicVolume( float volume );
        void SetSoundVolume( float volume );        

        //
        // Sound FX
        //
        virtual void SetSoundBank(const SoundBank& soundBank) = 0;
        void ClearSoundBank();

        void PlayAudio(unsigned int index);
        void PlayAudio(const char* filename);
        void PauseAllAudio();
        void UnpauseAllAudio();

    protected:

        static AudioManager* mInstance;

        float mMusicVolume;
        float mSoundVolume;		

        std::vector< AudioStaticSource* > mStaticSources;

        MusicList mMusicList;
        AudioStreamSource* mStreamSource;        
    };
}

#endif