#if defined AUDIO_XAUDIO2

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/audio/xaudio2/xaudio2audiomanager.hpp"
#include "common/engine/audio/xaudio2/xaudio2audiostaticsource.hpp"
#include "common/engine/audio/xaudio2/xaudio2audiostreamsource.hpp"
#include "common/engine/system/assert.hpp"

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include "winphone/audio/mediafoundationaudiostreamsource.hpp"
#include "winphone/audio/mediafoundationdevice.hpp"
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace Audio
{
    IXAudio2* XAudio2AudioManager::mXAudio2;
    IXAudio2MasteringVoice* XAudio2AudioManager::mMasterVoice;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
    IXAudio2* XAudio2AudioManager::GetDevice()
    {
        Assert( mXAudio2 != NULL, "" );
        return mXAudio2;
    }

    //===========================================================================

    void XAudio2AudioManager::CreateDevice()
    {
        HRESULT hr;
        int flags = 0;
#if defined PLAT_PC
	    hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
        Assert( hr == S_OK, "" );
#endif
        hr = XAudio2Create( &mXAudio2, flags, XAUDIO2_DEFAULT_PROCESSOR );
        Assert( hr == S_OK, "" );        
        hr = mXAudio2->CreateMasteringVoice( &mMasterVoice );
        Assert( hr == S_OK, "" );      
    }

    //===========================================================================

    void XAudio2AudioManager::DestroyDevice()
    {
        if ( mMasterVoice )
        {
            mMasterVoice->DestroyVoice();
        }

        if ( mXAudio2 )
        {
            mXAudio2->Release();
            mXAudio2 = NULL;
        }

#if defined PLAT_PC
        CoUninitialize();
#endif
    }

    //===========================================================================

    XAudio2AudioManager::XAudio2AudioManager()
    {
        mXAudio2 = NULL;
        mMasterVoice = NULL;
    }

    //===========================================================================

    XAudio2AudioManager::~XAudio2AudioManager()
    {
    }

    //===========================================================================

    void XAudio2AudioManager::Initialize()
    {   
        CreateDevice();

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
		MediaFoundationDevice::Create();
		mStreamSource = NEW_PTR( "MediaFoundationAudioStreamSource" ) MediaFoundationAudioStreamSource;
#else
		mStreamSource = NEW_PTR( "XAudio2StreamSource" ) XAudio2AudioStreamSource;	
#endif
    }

    //===========================================================================

    void XAudio2AudioManager::Shutdown()
    {
        AudioManager::Shutdown();

#if defined PLAT_WINDOWS_PHONE
		MediaFoundationDevice::Destroy();
#endif

        DestroyDevice();
    }

    //===========================================================================

    void XAudio2AudioManager::Update()
    {	
    }

    //===========================================================================

    void XAudio2AudioManager::PlayAudio( int type )
    {
        UNUSED_ALWAYS( type );
    }

    //===========================================================================

    void XAudio2AudioManager::SetSoundBank( const SoundBank& soundBank )
    {
		ClearSoundBank();

		int bankSize = soundBank.GetSize();

        mStaticSources.resize( bankSize );

		for ( int i = 0; i < bankSize; i++ )
		{
			const char* filename = soundBank.GetFileAt( i );
			XAudio2AudioStaticSource* audioStaticSource = NEW_PTR("XAudio2AudioStaticSource") XAudio2AudioStaticSource;
			audioStaticSource->Load( filename );
            audioStaticSource->SetVolume( mSoundVolume );

			//if ( audioStaticSource->GetSamplesPerSecond() != mMixerOutputRate )
			//{
			//	System::PrintToConsole(
   //                 "AudioManager::SetSoundBank(), file [%s] has sample rate [%d], which doesn't match with default mixer output rate [%d].\n",
			//		filename, audioStaticSource->GetSamplesPerSecond(), 
   //                 mMixerOutputRate
   //                 );
			//}

			mStaticSources[i] = audioStaticSource;
		}
    }
}

#endif