#if defined AUDIO_XAUDIO2

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/audio/xaudio2/xaudio2audiomanager.hpp"
#include "common/engine/audio/xaudio2/XAudio2AudioStaticSource.hpp"
#include "common/engine/audio/format/ogg.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
    XAudio2AudioStaticSource::XAudio2AudioStaticSource()        
    {
    }

    //===========================================================================

    XAudio2AudioStaticSource::~XAudio2AudioStaticSource()
    {	
        for ( int i = 0; i < 10; ++i )
        {
            if ( mSourceVoice[i] )
            {                
                mSourceVoice[i]->DestroyVoice();
            }
        }

        DELETE_PTR( mAudioStaticFormat );
    }

    //===========================================================================

    void XAudio2AudioStaticSource::Load( const char* filename )
    {
        mAudioStaticFormat = NEW_PTR( "Audio Static Format" ) Ogg;
        mAudioStaticFormat->Load( filename );

        mWaveFormat.nChannels = static_cast<WORD>( mAudioStaticFormat->GetChannels() );
        mWaveFormat.cbSize = 0;
        mWaveFormat.nSamplesPerSec = mAudioStaticFormat->GetSamplesPerSecond();
        mWaveFormat.nAvgBytesPerSec = mWaveFormat.nSamplesPerSec * mWaveFormat.nChannels * 2;
        mWaveFormat.nBlockAlign = static_cast<WORD>( mWaveFormat.nChannels * 2 );        
        mWaveFormat.wBitsPerSample = static_cast<WORD>( mAudioStaticFormat->GetBitsPerSample() );
        mWaveFormat.wFormatTag = WAVE_FORMAT_PCM;

        mBuffer.AudioBytes = mAudioStaticFormat->GetDataSize();
        mBuffer.pAudioData = (BYTE*)mAudioStaticFormat->GetData(); 
        mBuffer.Flags = XAUDIO2_END_OF_STREAM;
        mBuffer.PlayBegin = 0;
        mBuffer.PlayLength = 0;
        mBuffer.LoopBegin = 0;
        mBuffer.LoopLength = 0;
        mBuffer.LoopCount = 0;

        HRESULT hr;
        
        for ( int i = 0; i < 10; ++i )
        {
            mSourceVoice[i] = NULL;
            hr = XAudio2AudioManager::GetDevice()->CreateSourceVoice( &mSourceVoice[i], ( WAVEFORMATEX* )&mWaveFormat );
            Assert( hr == S_OK, "" );
        }

        mCurrentSourceVoiceIndex = 0;
    }

    //===========================================================================

    void XAudio2AudioStaticSource::Play()
    {
        HRESULT hr;
        hr = mSourceVoice[mCurrentSourceVoiceIndex]->SubmitSourceBuffer( &mBuffer );
        Assert( hr == S_OK, "" );        	
        hr = mSourceVoice[mCurrentSourceVoiceIndex]->Start( 0 );
        Assert( hr == S_OK, "" );

        mCurrentSourceVoiceIndex++;
        mCurrentSourceVoiceIndex %= 10;
    }

    //===========================================================================

    void XAudio2AudioStaticSource::Pause()
    {       
        HRESULT hr;

        for ( int i = 0; i < 10; ++i )
        {
            hr = mSourceVoice[i]->Stop();
            Assert( hr == S_OK, "" );
        }
    }

    //===========================================================================

    void XAudio2AudioStaticSource::Stop()
    {
        HRESULT hr;

        for ( int i = 0; i < 10; ++i )
        {
            hr = mSourceVoice[i]->Stop();
            Assert( hr == S_OK, "" );
        }        
    }

    //===========================================================================

    AudioState XAudio2AudioStaticSource::GetState()
    {
        return AUDIOSTATE_UNDEFINED;
    }

    //===========================================================================

    void XAudio2AudioStaticSource::SetVolume( float volume )
    {
        HRESULT hr;
        
        for ( int i = 0; i < 10; ++i )
        {
            if ( mSourceVoice[i] )
            {
                hr = mSourceVoice[i]->SetVolume( volume );
                Assert( hr == S_OK, "" );
            }
        }
    }
}

#endif