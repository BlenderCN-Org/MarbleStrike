#if defined AUDIO_OPENAL

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/openal/openalaudiomanager.hpp"
#include "common/engine/audio/format/wave.hpp"
#include "common/engine/audio/openal/openalaudiostaticsource.hpp"
#include "common/engine/audio/openal/openalaudiostreamsource.hpp"
#include "common/engine/audio/openal/openalwrapper.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace Audio
{
	const int AUDIO_DEFAULT_SAMPLE_RATE = 44100;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	OpenALAudioManager::OpenALAudioManager()
	{
        mMixerOutputRate = AUDIO_DEFAULT_SAMPLE_RATE;
	}

	//===========================================================================

	OpenALAudioManager::~OpenALAudioManager()
	{
	}

	//===========================================================================

	void OpenALAudioManager::Initialize()
	{
		InitializeOpenAL();

#if defined PLAT_IOS
        mStreamSource = NEW_PTR("iOSAudioStreamSource") iOSAudioStreamSource;
#else
        mStreamSource = NEW_PTR( "StreamSource" ) OpenALAudioStreamSource;
#endif
		mStreamSource->SetVolume( mSoundVolume );
	}

	//===========================================================================

	void OpenALAudioManager::Shutdown()
	{
        AudioManager::Shutdown();

		ShutdownOpenAL();
	}

	//===========================================================================

	void OpenALAudioManager::SetSoundBank( const SoundBank& soundBank )
	{
		ClearSoundBank();

		int bankSize = soundBank.GetSize();

        mStaticSources.resize( bankSize );

		for ( int i = 0; i < bankSize; i++ )
		{
			const char* filename = soundBank.GetFileAt( i );
			OpenALAudioStaticSource* src = NEW_PTR("AudioStaticSource") OpenALAudioStaticSource;
			src->Load( filename );
            src->SetVolume( mSoundVolume );

			if ( src->GetSamplesPerSecond() != mMixerOutputRate )
			{
				System::PrintToConsole(
                    "AudioManager::SetSoundBank(), file [%s] has sample rate [%d], which doesn't match with default mixer output rate [%d].\n",
					filename, src->GetSamplesPerSecond(), 
                    mMixerOutputRate
                    );
			}

			mStaticSources[i] = src;
		}
	}

	//===========================================================================

	void OpenALAudioManager::InitializeOpenAL()
	{
		ALCdevice* device = alcOpenDevice( NULL );
		Assert( device != NULL, "OpenAL is not installed in your system." );

#if defined PLAT_IOS
		if ( mMixerOutputRate != 0 )
		{
			OpenAL::alcMacOSXMixerOutputRateProc(static_cast<float>(mMixerOutputRate));
		}
#endif

		ALCcontext* context = alcCreateContext( device, NULL );
		Assert( context != NULL, "OpenAL is not installed in your system." );
		alcMakeContextCurrent( context );
	}

    //===========================================================================

	void OpenALAudioManager::ShutdownOpenAL()
	{
		ALCcontext* context = alcGetCurrentContext();
		if (context != NULL)
		{
			ALCdevice* device = alcGetContextsDevice( context );

			alcMakeContextCurrent( NULL );
			alcDestroyContext( context );

			if (device != NULL)
			{
				alcCloseDevice( device );
			}
		}
	}
}

#endif