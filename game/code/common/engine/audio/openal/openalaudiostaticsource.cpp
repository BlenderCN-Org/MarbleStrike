//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostaticformat.hpp"
#include "common/engine/audio/format/ogg.hpp"
#include "common/engine/audio/openal/openalaudiostaticsource.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "ios/audio/format/iosformat.hpp"


//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#if defined PLAT_PC || defined PLAT_IOS

namespace Audio
{
	OpenALAudioStaticSource::OpenALAudioStaticSource()        
	{
		mBuffer = 0;
		mSource = 0;
		mSamplePerSecond = 0;
	}

	//===========================================================================

	OpenALAudioStaticSource::~OpenALAudioStaticSource()
	{	
		OpenAL::oalDeleteSources( 1, &mSource );
		OpenAL::oalDeleteBuffers( 1, &mBuffer );
	}

	//===========================================================================

	void OpenALAudioStaticSource::Load( const char* filename )
	{
        mFileName = filename;

		OpenAL::oalGenBuffers( 1, &mBuffer );	

#ifdef PLAT_PC
		IAudioStaticFormat* audioStaticFormat = NEW_PTR( "Audio Static Format" ) Ogg;
#endif
#ifdef PLAT_IOS
		IAudioStaticFormat* audioStaticFormat = NEW_PTR( "iOS Format" ) iOSFormat;
#endif

		audioStaticFormat->Load( filename );

		ALenum format = OpenAL::GetFormatType( audioStaticFormat->GetBitsPerSample(), audioStaticFormat->GetChannels() );
		ALvoid* data = audioStaticFormat->GetData();
		ALsizei freq = audioStaticFormat->GetSamplesPerSecond();
		ALsizei size = audioStaticFormat->GetDataSize();

		OpenAL::oalBufferData( mBuffer, format, data, size, freq );

		OpenAL::oalGenSources( 1, &mSource );
		OpenAL::oalSourcei( mSource, AL_BUFFER, mBuffer );	
		OpenAL::oalSourcef( mSource, AL_GAIN, mVolume );
        OpenAL::oalSourcei( mSource, AL_LOOPING, false );

		mSamplePerSecond = freq;
	
		DELETE_PTR( audioStaticFormat );
	}

	//===========================================================================

	void OpenALAudioStaticSource::Play()
	{
		OpenAL::oalSourcePlay( mSource );

        AudioStaticSource::Play();
	}

	//===========================================================================

	void OpenALAudioStaticSource::Pause()
	{
		OpenAL::oalSourcePause( mSource );
	}

	//===========================================================================

	void OpenALAudioStaticSource::Stop()
	{
		OpenAL::oalSourceStop( mSource );

        AudioStaticSource::Stop();		
	}

	//===========================================================================

	ALsizei OpenALAudioStaticSource::GetSamplesPerSecond()
	{
		return mSamplePerSecond;
	}

	//===========================================================================

	AudioState OpenALAudioStaticSource::GetState()
	{
		ALint sourceState;
		alGetSourcei( mSource, AL_SOURCE_STATE, &sourceState );

		switch ( sourceState )
		{
		case AL_INITIAL: 
            return AUDIOSTATE_INITIAL;
		case AL_PLAYING: 
            return AUDIOSTATE_PLAYING;
		case AL_PAUSED:  
            return AUDIOSTATE_PAUSED;
		case AL_STOPPED: 
            return AUDIOSTATE_STOPPED;
		default:         
            return AUDIOSTATE_UNDEFINED;
		}
	}

	//===========================================================================

	void OpenALAudioStaticSource::SetVolume( float volume )
	{
		if ( mVolume != volume )
		{
			mVolume = volume;
			if ( mSource != 0 )
			{
				OpenAL::oalSourcef( mSource, AL_GAIN, mVolume );
			}
		}
	}
}

#endif