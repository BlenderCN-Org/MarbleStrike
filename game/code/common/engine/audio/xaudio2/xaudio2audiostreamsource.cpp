#if defined AUDIO_XAUDIO2

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/audio/format/iaudiostreamformat.hpp"
#include "common/engine/audio/format/oggstream.hpp"
#include "common/engine/audio/xaudio2/xaudio2audiostreamsource.hpp"
#include "common/engine/audio/xaudio2/xaudio2audiomanager.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
    void AudioVoiceCallback::OnStreamEnd()
    {
        if ( mXAudio2AudioStreamSource )
        {
            mXAudio2AudioStreamSource->SetDone( true );
        }
    }

    //===========================================================================

    XAudio2AudioStreamSource::XAudio2AudioStreamSource()
        : AudioStreamSource()
    {
        mCurrentDiskReadBuffer = 0;
        mSourceVoice = 0;
        mDone = false;
        mEndOfFile = false;
        mIsThreadRunning = false;

		mAudioThread = std::thread( std::bind( XAudio2AudioStreamSource::AudioThreadUpdate, this ) );
    }

    //===========================================================================

    XAudio2AudioStreamSource::~XAudio2AudioStreamSource()
    {
        mIsThreadRunning = false;
		mAudioThread.join();
        Unload();  
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Load( const char* filename )
    {
		std::lock_guard<std::mutex> lock(mAudioThreadMutex);
        
        mAudioStreamFormat = NEW_PTR( "Audio Static Format" ) OggStream;
        mAudioStreamFormat->Load( filename );

        mWaveFormat.nChannels = static_cast<WORD>( mAudioStreamFormat->GetChannels() );
        mWaveFormat.cbSize = 0;
        mWaveFormat.nSamplesPerSec = mAudioStreamFormat->GetSamplesPerSecond();
        mWaveFormat.nAvgBytesPerSec = mWaveFormat.nSamplesPerSec * mWaveFormat.nChannels * 2;
        mWaveFormat.nBlockAlign = static_cast<WORD>( mWaveFormat.nChannels * 2 );        
        mWaveFormat.wBitsPerSample = static_cast<WORD>( mAudioStreamFormat->GetBitsPerSample() );
        mWaveFormat.wFormatTag = WAVE_FORMAT_PCM;

        mCurrentDiskReadBuffer = 0;

        memset( &mDataBuffers[mCurrentDiskReadBuffer], 0, sizeof( mDataBuffers[mCurrentDiskReadBuffer] ) );

        int result = 0;
        int size = 0;
        do
        {
            result = mAudioStreamFormat->ReadStream( mDataBuffers[mCurrentDiskReadBuffer] + size, STREAMING_BUFFER_SIZE - size );
            size += result;
        }
        while ( result && size < STREAMING_BUFFER_SIZE );

        XAUDIO2_BUFFER buffer = {0};
        buffer.AudioBytes = STREAMING_BUFFER_SIZE;
        buffer.pAudioData = (BYTE*)mDataBuffers[mCurrentDiskReadBuffer]; 

        mAudioVoiceCallback.SetXAudio2AudioStreamSource( this );

        HRESULT hr;
        hr = XAudio2AudioManager::GetDevice()->CreateSourceVoice( 
            &mSourceVoice, 
            ( WAVEFORMATEX* )&mWaveFormat,
            0,
            XAUDIO2_DEFAULT_FREQ_RATIO,
            &mAudioVoiceCallback
            );        
        Assert( hr == S_OK, "" );

        hr = mSourceVoice->SubmitSourceBuffer( &buffer );
        Assert( hr == S_OK, "" );

        mAudioState = AUDIOSTATE_STOPPED;

        mCurrentDiskReadBuffer++;   

    }

    //===========================================================================

    void XAudio2AudioStreamSource::Unload()
    {
        Stop();

		std::lock_guard<std::mutex> lock(mAudioThreadMutex);

        mEndOfFile = false; 
        mDone = false;

        if ( mAudioStreamFormat )
        {
            mAudioStreamFormat->CloseStream();
            DELETE_PTR( mAudioStreamFormat );
        }

        if ( mSourceVoice )
        {
            mSourceVoice->DestroyVoice();
            mSourceVoice = NULL;
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Play()
    {
		std::lock_guard<std::mutex> lock(mAudioThreadMutex);

        if ( mSourceVoice != NULL )
        {
            HRESULT hr;
            hr = mSourceVoice->Start( 0 );
            Assert( hr == S_OK, "" );  

            mAudioState = AUDIOSTATE_PLAYING;
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Stop()
    {
		std::lock_guard<std::mutex> lock(mAudioThreadMutex);

        if ( mSourceVoice != NULL )
        {
            HRESULT hr;
            hr = mSourceVoice->Stop( 0 );
            Assert( hr == S_OK, "" );

            mAudioState = AUDIOSTATE_STOPPED;
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Pause()
    {
		std::lock_guard<std::mutex> lock(mAudioThreadMutex);

        if ( mSourceVoice != NULL )
        {
            HRESULT hr;
            hr = mSourceVoice->Stop( 0 );
            Assert( hr == S_OK, "" );

            mAudioState = AUDIOSTATE_PAUSED;
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Unpause()
    {
		std::lock_guard<std::mutex> lock(mAudioThreadMutex);

        if ( mSourceVoice != NULL )
        {
            HRESULT hr;
            hr = mSourceVoice->Start( 0 );
            Assert( hr == S_OK, "" );

            mAudioState = AUDIOSTATE_PLAYING;
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Rewind()
    {
    }

    //===========================================================================

    bool XAudio2AudioStreamSource::IsFinished()
    {
        return mDone;
    }

    //===========================================================================

    AudioState XAudio2AudioStreamSource::GetState()
    {
        return mAudioState;
    }

    //===========================================================================

    void XAudio2AudioStreamSource::Update()
    {

    }

    //===========================================================================

    void XAudio2AudioStreamSource::EmptyQueue()
    {
    }

    //===========================================================================

	void XAudio2AudioStreamSource::AudioThreadUpdate( XAudio2AudioStreamSource* lpParam )
    {
        XAudio2AudioStreamSource* streamSource = reinterpret_cast<XAudio2AudioStreamSource*>( lpParam );

        streamSource->mIsThreadRunning = true;

        while ( streamSource->mIsThreadRunning )
        {
			streamSource->mAudioThreadMutex.lock();

            if ( 
                streamSource->mSourceVoice && 
                streamSource->mAudioState == AUDIOSTATE_PLAYING && 
                streamSource->mEndOfFile == false 
                )
            {        
                XAUDIO2_VOICE_STATE state;
                streamSource->mSourceVoice->GetState( &state );
                if ( state.BuffersQueued < MAX_BUFFER_COUNT - 1 )
                {
                    memset( &( streamSource->mDataBuffers[streamSource->mCurrentDiskReadBuffer] ), 0, sizeof( streamSource->mDataBuffers[streamSource->mCurrentDiskReadBuffer] ) );

                    int result = 0;
                    int size = 0;
                    do
                    {
                        result = streamSource->mAudioStreamFormat->ReadStream( streamSource->mDataBuffers[streamSource->mCurrentDiskReadBuffer] + size, STREAMING_BUFFER_SIZE - size );
                        size += result;
                    }
                    while ( result && size < STREAMING_BUFFER_SIZE );

                    XAUDIO2_BUFFER buffer = {0};
                    buffer.pAudioData = (BYTE*)&( streamSource->mDataBuffers[streamSource->mCurrentDiskReadBuffer] );
                    if ( result == 0 )
                    {
                        buffer.Flags = XAUDIO2_END_OF_STREAM;
                        streamSource->mEndOfFile = true;                        
                    }
                    buffer.AudioBytes = STREAMING_BUFFER_SIZE;

                    HRESULT hr;
                    hr = streamSource->mSourceVoice->SubmitSourceBuffer( &buffer );
                    Assert( hr == S_OK, "" );

                    streamSource->mCurrentDiskReadBuffer++;
                    streamSource->mCurrentDiskReadBuffer %= MAX_BUFFER_COUNT;
                }
            }

			streamSource->mAudioThreadMutex.unlock();
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::SetVolume( float volume )
    {
		std::lock_guard<std::mutex> lock(mAudioThreadMutex);

        if ( mSourceVoice != NULL )
        {
            HRESULT hr;
            hr = mSourceVoice->SetVolume( volume );
            Assert( hr == S_OK, "" );
        }
    }

    //===========================================================================

    void XAudio2AudioStreamSource::SetDone( bool value )
    {
        mDone = value;
    }
}

#endif