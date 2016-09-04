#if defined AUDIO_XAUDIO2

#ifndef XAUDIO2_MANAGER_HPP
#define XAUDIO2_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include <Xaudio2.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class XAudio2StaticSource;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
    class XAudio2AudioManager : public AudioManager
    {
    public:	

        static IXAudio2* GetDevice();
        static void CreateDevice();
        static void DestroyDevice();

        XAudio2AudioManager();
        ~XAudio2AudioManager();

        void Initialize();
        void Shutdown();
        void Update();	

        void PlayAudio( int type );	

        void SetSoundBank( const SoundBank& soundBank );

    private:

        static IXAudio2* mXAudio2;
        static IXAudio2MasteringVoice* mMasterVoice;
    };
}

#endif

#endif