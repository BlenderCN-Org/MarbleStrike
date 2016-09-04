#if defined AUDIO_XAUDIO2

#ifndef XAUDIO2_AUDIO_STREAM_SOURCE_HPP
#define XAUDIO2_AUDIO_STREAM_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/xaudio2/xaudio2audiostreamsource.hpp"
#include "common/engine/audio/audiostreamsource.hpp"
#include "common/engine/system/fixedstring.hpp"
#include <Xaudio2.h>
#include <thread>
#include <mutex>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#define STREAMING_BUFFER_SIZE 65536*10
#define MAX_BUFFER_COUNT 3


//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
    class XAudio2AudioStreamSource;

    class AudioVoiceCallback : public IXAudio2VoiceCallback
    {
    public:
        AudioVoiceCallback()
        {
            mXAudio2AudioStreamSource = NULL;
        }

        ~AudioVoiceCallback()
        {
        }

        STDMETHOD_( void, OnStreamEnd )();
        
        STDMETHOD_( void, OnVoiceProcessingPassEnd )()
        { 
        }

        STDMETHOD_( void, OnVoiceProcessingPassStart )( UINT32 samplesRequired ) 
        {
            UNUSED_ALWAYS( samplesRequired );
        }

        STDMETHOD_( void, OnBufferEnd )( void* bufferContext )
        {
            UNUSED_ALWAYS( bufferContext );
        }
        
        STDMETHOD_( void, OnBufferStart )( void* bufferContext ) 
        {
            UNUSED_ALWAYS( bufferContext );
        }

        STDMETHOD_( void, OnLoopEnd )( void* bufferContext ) 
        {    
            UNUSED_ALWAYS( bufferContext );
        }

        STDMETHOD_( void, OnVoiceError )( void* bufferContext, HRESULT error ) 
        {
            UNUSED_ALWAYS( bufferContext );
            UNUSED_ALWAYS( error );
        }

        void SetXAudio2AudioStreamSource( XAudio2AudioStreamSource* streamSource )
        {
            mXAudio2AudioStreamSource = streamSource;
        }

    private:
        XAudio2AudioStreamSource* mXAudio2AudioStreamSource;
    };

    class XAudio2AudioStreamSource : public AudioStreamSource
    {
    public:	

        XAudio2AudioStreamSource();
        ~XAudio2AudioStreamSource();

        void Load( const char* filename );
        void Unload();

        void Play();
        void Stop();
        void Pause();
        void Unpause();
        void Rewind();
        bool IsFinished();

        AudioState GetState();

        void Update();

        void SetVolume(float volume);
        float GetVolume();

        void SetDone( bool value );

    private:

        void EmptyQueue();
        static void AudioThreadUpdate( XAudio2AudioStreamSource* lpParam );

        WAVEFORMATEX mWaveFormat;
        char mDataBuffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];
        IXAudio2SourceVoice* mSourceVoice;
        int mCurrentDiskReadBuffer;

        bool mDone;
        bool mEndOfFile;
        bool mIsThreadRunning;

        AudioState mAudioState;
        AudioVoiceCallback mAudioVoiceCallback;

		std::thread mAudioThread;
		std::mutex mAudioThreadMutex;
    };
}

#endif

#endif