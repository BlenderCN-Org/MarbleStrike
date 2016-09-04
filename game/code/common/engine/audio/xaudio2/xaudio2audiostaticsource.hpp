#if defined AUDIO_XAUDIO2

#ifndef XAUDIO2_AUDIO_STATIC_SOURCE_HPP
#define XAUDIO2_AUDIO_STATIC_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiostaticsource.hpp"
#include <Xaudio2.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
    class IAudioStaticFormat;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
    class XAudio2AudioStaticSource : public AudioStaticSource
    {
    public:	

        XAudio2AudioStaticSource();
        ~XAudio2AudioStaticSource();

        void Load( const char* filename );
        void Play();
        void Pause();
        void Stop();

        AudioState GetState(); 
        void SetVolume( float volume );

    private:	
                
        IAudioStaticFormat* mAudioStaticFormat;
        WAVEFORMATEX mWaveFormat;
        XAUDIO2_BUFFER mBuffer;
        IXAudio2SourceVoice* mSourceVoice[10];
        int mCurrentSourceVoiceIndex;
    };
}

#endif

#endif